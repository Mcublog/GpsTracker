from dataclasses import dataclass
from enum import Enum
from typing import Any, Callable


@dataclass
class ConsoleCmd:
    alias: str
    pcmd: Enum
    handler: Callable[[list[str]], Any]
    receive_hander: Callable | None = None
