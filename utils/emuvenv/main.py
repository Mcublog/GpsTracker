#! /bin/env python3

import logging
from enum import Enum
from pathlib import Path
from threading import Event

from prompt_toolkit import PromptSession
from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.patch_stdout import patch_stdout
from pylogus import logger_init

from emuvenv.console import ConsoleCommand, ConsoleControlCmd, get_command_and_args

log = logger_init(__name__, logging.DEBUG)


class PinState(Enum):
    OFF = 0,
    ON = 1

#define CONFIG_FILE_NAME "/tmp/config"

#define RED_LED_GPIO_FILE_NAME      "/tmp/io_led_gpio_red"
#define GREEN_LED_GPIO_FILE_NAME    "/tmp/io_led_gpio_green"
EXT_PWR_GPIO_FILE_NAME = Path("/tmp/io_ext_pwr_pin")
#define ACCEL_IRQ_GPIO_FILE_NAME    "/tmp/io_accel_irq_pin"

#define RTC_TIME_FILE_NAME              "/tmp/rtc_time"
#define RTC_ALARM_0_TIME_FILE_NAME      "/tmp/rtc_alarm0"


def set_power_on(_, **__):
    set_pin_state(EXT_PWR_GPIO_FILE_NAME, PinState.ON)

def set_power_off(_, **__):
    set_pin_state(EXT_PWR_GPIO_FILE_NAME, PinState.OFF)

def set_pin_state(pin: Path, state: PinState):
    with pin.open("w+") as f:
        f.write(f"{state.value}\n")


COMMANDS = (ConsoleCommand('pin_power_on', ConsoleControlCmd.SET_POWER_ON,
                           set_power_on, None),
            ConsoleCommand('pin_power_off', ConsoleControlCmd.SET_POWER_ON,
                           set_power_off, None))


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

            result = get_command_and_args(input_cmd_string, tuple(COMMANDS))
            if result.cmd.pcmd == ConsoleControlCmd.UNKNOWN:
                log.warning(f"{result.cmd.alias} not found")
                continue
            cmd, args, kwargs = result.cmd, result.args, {}
            cmd.handler(args, **kwargs)


def main():
    console()


if __name__ == "__main__":
    main()
