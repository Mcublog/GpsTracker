#! /bin/env python3

import logging
from dataclasses import dataclass
from enum import Enum, auto
from threading import Event
from typing import Any, Callable

from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.patch_stdout import patch_stdout
from pylogus import logger_init

log = logger_init(__name__, logging.DEBUG)


class ConsoleControlCmd(Enum):
    SET_PORT = auto()
    PORT_LIST = auto()
    EXIT = auto()
    FW_UPDATE = auto()
    TO_BOOT_AND_UPDATE = auto()
    SET_BAUDRATE = auto()
    CONNECT_TO_BOOT = auto()
    UNKNOWN = auto()


@dataclass
class ConsoleCommand:
    alias: str
    pcmd: Enum
    handler: Callable[..., Any]
    receive_hander: Callable | None = None


@dataclass
class ConsoleCommandParseResult:
    cmd: ConsoleCommand
    args: list[str]


COMMANDS = (ConsoleCommand('set_port', ConsoleControlCmd.SET_PORT,
                           lambda: None, None), )


def console():
    kill_evt = Event()
    session = PromptSession()
    while not kill_evt.wait(0.01):
        with patch_stdout(raw=True):
            input_cmd_string = ''
            try:
                input_cmd_string = session.prompt(
                    "> ",
                    auto_suggest=AutoSuggestFromHistory(),
                    completer=WordCompleter([c.alias for c in COMMANDS]))
            except KeyboardInterrupt:
                kill_evt.set()
            if not input_cmd_string:
                continue


def main():
    console()


if __name__ == "__main__":
    main()
