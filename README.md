# OpenArcadeGun

A project to produce hardware to tracker an infrared cameras point angle at a screen

## Protocol

Initially the arduino should output essentially a stream of comma seperated values which are the X and Y coordinates scaled between 0 and 255 (1 byte) along with the trigger status (0 off, 1 on).

Below is an example of the stream you might expect:

```
0,0,0
0,10,0
10,0,1
253,222,0
50,24,0
```

## Parts

Requires:

- 1 x DFRobot IR Sensor
- 8 x IR LEDs with resistors
- 1 x Switch for trigger
