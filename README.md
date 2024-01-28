# GpsTracker

GPS tracker based on a MCU and GPS module.

## App

Platform independent code with the business logic and headers.
Common implementations are located in the platform directory, located depending on the platform type.
Specific implementation are located in the target directory, depending on the target type.

## Scripts

Some useful bash scripts. Setting default emu environment.

## Utils

Some useful Python scritps.

```bash
export PYTHONPATH="${PYTHONPATH}:utils"
```

## Related repositories

* [GpsTrackerTool](https://github.com/Mcublog/GpsTrackerTool)
* [GpsTrackerHardware](https://github.com/Mcublog/GpsTrackerHardware)

## Docs

* [Motion Detection Alogrithm](/doc/MotionDetectionAlgo.md)
* [Emulator Readme](/platforms/emu/README.md)
* [Open407V-C](https://www.waveshare.com/wiki/Open407V-C)