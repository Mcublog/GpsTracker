#!/usr/bin/env python3
import ctypes as ct
import json
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path

# ---------------------- Low level C references
time_t = ct.c_uint64


class int32_th(ct.Structure):
    _fields_ = [
        ('min', ct.c_int32),
        ('max', ct.c_int32)
    ]


class tph_cfg_t(ct.Structure):  # temperature, pressure, humidity config
    _fields_ = [
        ('period_s', ct.c_uint32),
        ('temperature_threshold',   int32_th),  # int xx.xx * 1000 C temp
        ('humidity_threshold',      int32_th),  # (%)
        ('pressure_threshold',      int32_th),  # (mBar)
    ]


class serial_num_t(ct.Structure):
    _fields_ = [
        ('serial', ct.c_uint32)
    ]


class accel_cfg_t (ct.Structure):  # accelerometer config
    _fields_ = [
        ('freq',            ct.c_uint8),  # descrete frequncy
        ('full_scale',      ct.c_uint8),
        ('acc_size',        ct.c_uint32),
        ('pre_threshold',   ct.c_uint32),  # threshold of start
        ('alert_threshold', ct.c_uint32),  # alert threshold
    ]


class accel_ofst_t(ct.Structure):
    _fields_ = [
        ('x',   ct.c_int32),
        ('y',   ct.c_int32),
        ('z',   ct.c_int32),
    ]


class rpt_config_t(ct.Structure):
    _fields_ = [
        ('accel',           accel_cfg_t),
        ('tph_cfg',         tph_cfg_t),
        ('time_to_start',   time_t),  # time to start logging
        ('time_to_stop',    time_t),  # time to stop logging
    ]


class config_t(ct.Structure):
    # _pack_ = 4
    _fields_ = [
        ('last',    ct.c_uint32),
        ('rpt_cfg', rpt_config_t),
        ('s',       serial_num_t),
        ('aoffst',  accel_ofst_t),
        ('crc32',   ct.c_uint32)
    ]

# ----------------------
# ---------------------- Config dataclasses


SETTINGS_FILENAME = "settings.json"


@dataclass
class Int32Th:
    min: int = 0
    max: int = 9


@dataclass
class TphConfig:
    period_s: int = 10
    temperature_threshold: Int32Th = field(default_factory=Int32Th)
    humidity_threshold: Int32Th = field(default_factory=Int32Th)
    pressure_threshold: Int32Th = field(default_factory=Int32Th)


@dataclass
class AccConfig:
    freq: int = 0x07
    full_scale: int = 0
    acc_size: int = 100
    pre_threshold: int = 50
    alert_threshold: int = 80


@dataclass
class AccOffset:
    x: int = 0
    y: int = 0
    z: int = 0


@dataclass
class ReportConfig:
    accel: AccConfig = field(default_factory=AccConfig)
    tph_cfg: TphConfig = field(default_factory=TphConfig)
    time_to_start: int = int(time.time())
    time_to_stop: int = int(time.time()) + 2400


@dataclass
class Config:
    last: int = 0
    report_config: ReportConfig = field(default_factory=ReportConfig)
    serial_number: int = 0xFFFF_FFFF
    acc_offset: AccOffset = field(default_factory=AccOffset)
    crc32: int = 0

    def serialize(self) -> config_t:
        ll_cfg = config_t()
        ll_cfg.last = ct.c_uint32(self.last)
        ll_cfg.crc32 = ct.c_uint32(self.crc32)
        ll_cfg.s = serial_num_t(serial=ct.c_uint32(self.serial_number))
        ll_cfg.rpt_cfg.time_to_start = time_t(self.report_config.time_to_start)
        ll_cfg.rpt_cfg.time_to_stop = time_t(self.report_config.time_to_stop)
        ll_cfg.rpt_cfg.accel.freq = ct.c_uint8(self.report_config.accel.freq)
        ll_cfg.rpt_cfg.accel.full_scale = ct.c_uint8(self.report_config.accel.full_scale)
        ll_cfg.rpt_cfg.accel.acc_size = ct.c_uint32(self.report_config.accel.acc_size)
        ll_cfg.rpt_cfg.accel.pre_threshold = ct.c_uint32(
            self.report_config.accel.pre_threshold)
        ll_cfg.rpt_cfg.accel.alert_threshold = ct.c_uint32(
            self.report_config.accel.alert_threshold)

        ll_cfg.rpt_cfg.tph_cfg.period_s = ct.c_uint32(
            self.report_config.tph_cfg.period_s)
        # TODO: add another fields
        return ll_cfg

    def save(self) -> None:
        with Path("settings.json").open("w+") as f:
            json.dump(asdict(self), f, indent=4)

    @staticmethod
    def load() -> 'Config':
        setting_file = Path(SETTINGS_FILENAME)
        if not (setting_file.exists() and setting_file.is_file()):
            print(f"{SETTINGS_FILENAME} not found")
            return Config()
        config = Config()
        with setting_file.open('r') as f:
            try:
                config = Config(**json.load(f))
            except Exception as e:
                print(f"file {SETTINGS_FILENAME} failed format")
                return Config()
        return config

# ----------------------


def main():
    cfg = Config()
    print(cfg)
    print(cfg.serialize())
    print(ct.sizeof(config_t))
    Config().save()
    cfg = Config.load()


if __name__ == '__main__':
    main()
