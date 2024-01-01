#! /bin/env python3

import logging
from threading import Event

from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.patch_stdout import patch_stdout
from pylogus import logger_init

from emuvenv.console import ConsoleCommand, ConsoleControlCmd

log = logger_init(__name__, logging.DEBUG)

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
