# VGA for Atmega8

I will try to get VGA working on a atmega8 running on 12Mhz. Not sure if this is even possible, but we will find out!

## TODO:

- [x] LED control
- [x] Timer interrupts
- [x] VGA Sync timings
- [x] VGA routine
- [ ] Input for custom data (maybe i2c or something)

## Pin Connections

desc => atmega8 => vga dsub
vertical pulse => PB 0 => pin 14
horizontal pulse => PB2 => pin 13
red => PD6 => pin 1
green => PD5 => pin 2
blue => PD4 => pin 3

See picture for DSUB pinout. It's not 100% my connection layout!

![pinout](http://www.gammon.com.au/images/Arduino/VGA_Output_10.png)

## Useful links:

- https://github.com/smaffer/vgax
- http://www.gammon.com.au/forum/?id=11608
- https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8
- For creating simple bitmaps: https://github.com/prismspecs/uint-text-creator (to test output)
