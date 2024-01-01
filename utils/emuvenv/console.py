from dataclasses import dataclass
from enum import Enum, auto
from typing import Any, Callable


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
