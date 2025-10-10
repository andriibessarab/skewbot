# Skewb Solving Robot

<img src="https://hc-cdn.hel1.your-objectstorage.com/s/v3/863834e530550cf2c99fef21fbae26d638773245_image.png" width="400" />

## Purpose

The Skewb Solving Robot is designed to scan the cube using a single optical sensor, find the solution from a list of all solutions, and execute rotations using a four-motor system.

## Features

- Semi-autonomous Skewb solving from any valid state
- Distance sensor for cube placement verification
- LED touch sensor for start/abort control
- Encoder-based closed-looped control for rotation accuracy
    - Less cubes destroyed and faster solves

## Hardware

- 4x [VEX IQ Smart Motors](https://www.vexrobotics.com/228-2560.html)
- 1x [IQ Optical Sensor](https://www.vexrobotics.com/228-7082.html)
- 1x [IQ Distance Sensor](https://www.vexrobotics.com/228-7106.html)
- 1x [LED Touch Sensor](https://www.vexrobotics.com/228-3010.html)

## Software

The control system is written in C++ using the VEXcodeIQ.

## Team

[Adam Turaj](https://github.com/AdamTuraj): Mechanical & Electrical design

[Andrii Bessarab](https://github.com/andriibessarab): Solver Algorithm, Kinematics software 

[Jason Liang](https://github.com/JasonLiang12321): Solver Algorithm, Kinematics software

[Robert Vigneron](https://github.com/robertvign): Mechanical & Electrical design

## License

Hackclub is licensed under the MIT License. See the full license text in [LICENSE](LICENSE).
