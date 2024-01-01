from dataclasses import dataclass
from enum import Enum, auto
from typing import Any, Callable

JUST_READ_RESPONSE = lambda _, __: (_, __)


class ConsoleControlCmd(Enum):
    SET_POWER_ON = auto()
    SET_POWER_OFF = auto()
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


def get_command_and_args(
        cmd_raw: str, commands: tuple[ConsoleCommand,
                                      ...]) -> ConsoleCommandParseResult:
    args = cmd_raw.split(' ')
    for command in commands:
        if command.alias != args[0]:
            continue
        return ConsoleCommandParseResult(command, list(args[1:]))
    return ConsoleCommandParseResult(
        ConsoleCommand(args[0], ConsoleControlCmd.UNKNOWN, JUST_READ_RESPONSE),
        [])
