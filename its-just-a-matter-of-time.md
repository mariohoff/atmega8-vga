---
title: "Its Just a Matter of Time"
subtitle: "Road to VGA on an Atmega8 with only 12MHz"
date: 2018-05-10T14:25:10+02:00
draft: true
---

## The historical Introduction

Back when I was a first semester Computer Engineering student we had a course
which should introduce us to a few basics we would need throughout our
studies/careers. Besides learning simple Stuff like git, writing a scientific
paper or doing a presentation we also had a soldering course, which was pretty
fun.

The result of this soldering course was a small PCB with a few LEDs, a
potentiometer and an ATmega on it. The good thing about it that it was even
programmable with USB. So no extra programmer needed (see image 1).

Then in my third semester we learned basics about computer architectures and
used our soldered board to learn a tiny bit assembly, how interrupts work and
what they are and all fancy stuff you should know about computers. But after
this semester was done the board was just laying in my drawer collecting dust
and I didn't know what to do with it. I knew that the atmega had barely any
computing power, but some nice project should be possible. So I thought about
adding a display (obviously, cause displays are awesome). I tried HD44780 LCDs
a SSD1306 OLED and a few other things. They all worked quite good, but it wasnt
really exciting and all tutorials on it were straight forward.

Some day I then thought if it might be posisble to attach a VGA Port to it.
Googled around and found two documentations[1][2] on attaching VGA port to an arduino
Uno. Wow, this sounded promissing. They also have an atmega (328p) on them, run
only 16MHz and Arduino is just basically C++. So I should be able to downscale
this a tiny bit, right?

## VGA Theory

When I first found one forum post describing how to get VGA done on an
Atmega328p I was astonished by how good he explained VGA and how "complex" it
is.
When I look back now it doesn't feel that complex. You basically only need 3
(or 5) Signals:

- Vertical pulse: This is a 64µs pulse occuring at a rate of 60Hz. So this is
  basically our base-pulse.

- Horizontal pulse: Occurs after every line is finished. We have 480 lines plus
  35 lines backporch and 10 lines frontporch (Apprently no one knows why they
  are called porch). So that are 525 lines that have to be drawn at 60Hz. That
  makes 1/(60Hz\*525) = 31.7µs. So we have to have a pulse of around 32µs.

- One or three signals for the colors. If we tie them all togehter we only get
  a monochrome picture, but we don't need to waste space for colour codes or
  something else. But now it gets tricky. We have 32µs to draw 640 pixels plus
  plus 96 pixels for the horizontal sync pulse, plus 48 pixels horizontal back
  porch. That are in theory 800 pixels in 32µs. That's impossible with an 
  Atmega8 running at 12MHz. 


## Sources

- [1] http://www.gammon.com.au/forum/?id=11608
- [2] https://github.com/smaffer/vgax


