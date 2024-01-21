#!/usr/bin/env python3

import argparse
import ctypes as ct
import logging
import time
from dataclasses import dataclass
from enum import Enum, auto
from threading import Event
from typing import TypeAlias

import serial
from colorama import Fore as Clr
from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.patch_stdout import patch_stdout
from pylogus import logger_init

# Local application imports
from console.commands import ConsoleCmd
from console.proto.config import Config
from console.proto.proto import CommandId, Message, Reports, RtcTime
from console.version import VERSION

log = logger_init(__name__, logging.DEBUG)

DESCRIPTION = f'COBS console {Clr.GREEN}v{VERSION}{Clr.RESET}'
EXIT_MESSAGE = f"Exit from {DESCRIPTION}"

ConsoleCommands: TypeAlias = tuple[ConsoleCmd]


class ConsoleControlCmd(Enum):
    SET_PORT = auto()
    EXIT = auto()
    UNKNOWN = auto()


@dataclass
class ConsoleCommandParseResult:
    cmd: ConsoleCmd
    args: list[str]


def set_port_cmd_handler(args) -> str:

    @dataclass
    class PortArgs:
        port: str

    try:
        return PortArgs(*args).port
    except Exception as e:
        log.error(e)
    return ''


def set_rtc_time(args) -> bytes:
    msg = Message.serialize(CommandId.CMDID_SET_RTC, 1,
                            RtcTime(time=int(time.time())))

    @dataclass
    class SetRtcCommandArgs:
        time: str = ""

    rtc_args = SetRtcCommandArgs()
    if not args:
        try:
            rtc_args = SetRtcCommandArgs(*args)
        except Exception as e:
            log.error(e)
            return msg

    if not rtc_args.time.isnumeric():
        return msg
    return Message.serialize(CommandId.CMDID_SET_RTC, 1,
                             RtcTime(time=int(rtc_args.time)))


def set_settings(_) -> bytes:
    config = Config()
    return Message.serialize(CommandId.CMDID_SET_SETTINGS, 1,
                             config.serialize())


def get_command_and_args(
        cmd_raw: str, commands: ConsoleCommands) -> ConsoleCommandParseResult:
    args = cmd_raw.split(' ')
    for command in commands:
        if command.alias == args[0]:
            return ConsoleCommandParseResult(command, list(args[1:]))
    return ConsoleCommandParseResult(
        ConsoleCmd(args[0], ConsoleControlCmd.UNKNOWN, lambda _: _), [])


COMMANDS = ConsoleCommands(
    (ConsoleCmd('set_port', ConsoleControlCmd.SET_PORT, set_port_cmd_handler),
     ConsoleCmd('exit', ConsoleControlCmd.EXIT, lambda _: print(EXIT_MESSAGE)),
     ConsoleCmd('set_time', CommandId.CMDID_SET_RTC, set_rtc_time,
                lambda _: _),
     ConsoleCmd('set_settings', CommandId.CMDID_SET_SETTINGS, set_settings,
                lambda _: _),
     ConsoleCmd(
         'storage_clear', CommandId.CMDID_STORAGE_CLEAR,
         lambda _: Message.serialize(CommandId.CMDID_STORAGE_CLEAR, 1, None),
         lambda _: _),
     ConsoleCmd(
         'get_reports', CommandId.CMDID_GET_REPORTS,
         lambda _: Message.serialize(CommandId.CMDID_GET_REPORTS, 1, None),
         lambda _: _)))


def console(sport: str):
    port = sport
    kill_evt = Event()
    session = PromptSession()
    log.info(f"Working on port: {port}")
    while not kill_evt.wait(0.01):
        with patch_stdout(raw=True):
            input_cmd_string = ''
            try:
                input_cmd_string = session.prompt(
                    "> ",
                    auto_suggest=AutoSuggestFromHistory(),
                    completer=WordCompleter([c.alias for c in COMMANDS]))
            except KeyboardInterrupt:
                print(EXIT_MESSAGE)
                kill_evt.set()
            if not input_cmd_string:
                continue
            result = get_command_and_args(input_cmd_string, COMMANDS)
            if result.cmd.pcmd == ConsoleControlCmd.UNKNOWN:
                log.warning(f"{result.cmd.alias} not found")
                continue
            cmd, args = result.cmd, result.args
            msg = cmd.handler(args)
            if cmd.pcmd == ConsoleControlCmd.SET_PORT:
                port = msg
                continue
            elif cmd.pcmd == ConsoleControlCmd.EXIT:
                kill_evt.set()
            if not msg:
                continue
            try:
                with serial.Serial(port, timeout=0.5) as ser:
                    ser.write(msg)
                    log.info(f"PC ==> TX: {msg}")
                    if cmd.receive_hander is None:
                        continue
                    if rx := ser.read_until(expected=b'\x00'):
                        log.info(f"PC <== RX: {rx}")
            except Exception as e:
                log.error(e)


def main():
    parser = argparse.ArgumentParser(prog='Console',
                                     description=f"{DESCRIPTION}")
    parser.add_argument('-p',
                        '--port',
                        type=str,
                        help='Target port name (/dev/ttyACM0)',
                        default='/dev/ttyACM0')

    console(parser.parse_args().port)


if __name__ == '__main__':
    main()
