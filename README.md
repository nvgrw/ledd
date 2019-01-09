# ledd

_ledd_ is a simple ws281x LED strip mapper and spooler compatible with [Open
Pixel Control (OPC)](http://openpixelcontrol.org).

I developed this project for personal use and do not have time to polish it very
much. Feel free to use it!

My current use-case is to control an RGBW LED strip connected to a Raspberry Pi
using Apple Homekit. _ledd_ is a daemon bridging the communication between the
[Homebridge OPC plugin](https://www.npmjs.com/package/homebridge-opc) and the
LED strip itself. [Homebridge](https://homebridge.io) allows you to then set up
different types of lights on the LED strip and control everything from an iOS
device.
