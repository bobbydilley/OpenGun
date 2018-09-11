# OpenArcadeGun

A project to produce hardware and software to replace standard camera based arcade guns with versions using internal movement sensors to calculate position. This project was inspired after playing the game 'Tomb Raider Arcade', where the guns in the system feature no cameras, and use internal movement sensors.

The project will also hopefully integrate some code from the JVSEmulator project, and produce a hardware version that can plug strait into the JVS port of a Sega Naomi/Chihiro/Lindbergh/Triforce etc.

## Benefits of the project

I can see multiple benefits of this project including:

- Cheeper hardware costs to make guns
- Less prone to error such as: gun lens damage, board failure

## Possible draw backs

The accuracy on this is unlikely to be quite as precise as an IR system with 4 IR anchor points. However this should work well enough, if calibrated at the position of play to play all games with a cross-hair and possibly even games without.

## Hardware

This project is based on an Arduino Nano and an MPU-9250 inertial measurement unit which includes a 3 Axis Gyroscope, 3 Axis Accelerometer and 3 Axis Magnetometer. The project will take advantage of the 9250's Digital Motion Processor (DMP) which can fairly accurately predict motion events which we can then map onto the screen as X, Y coordinates.

## Links

Below are some resources where initial code and designs are inspired from:

<https://www.hackster.io/movsensllc/diy-imu-based-smarttv-controller-mouse-with-arduino-micro-548353>
