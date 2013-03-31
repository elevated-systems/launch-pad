launch-pad
==========

Problem
-------
Embedded system platforms are becoming cheaper every day. While they are also getting simpler to approach over time, it seems to be slower than the price drop. In order to begin working with an Arduino or Raspbery Pi someone must get the right cabling, download an IDE (or at least understand how to write some source code and get it onto the device), learn a C style language (esp for Arduino) or some other computer language, begin to understand serial communication, etc.

Solution
--------
LaunchPad is an embedded system platform for building various simple (for now) applications while requiring no more of the developer/maker than:
* the ability to access a webpage from a computer on the same Wifi connection,
* some knowledge of HTML and Javascript, and
* building circuits with sensors and actuators on a breadboard

The hope is that removing some of the barriers of entry to play with embedded systems will bring in more creative and motivated minds into the world of embedded system development.

Licenses
--------
MIT and Creative Commons Attribution 3.0

Requirements
------------
* Arduino (Raspberry Pi coming soon...)
* [TinyWebServer|https://github.com/ovidiucp/TinyWebServer] compatible networking sheild/module (currently tested with Arduino Ethernet, Wifi coming soon...)
* SD card sheild/module
* ability to load a sketch onto the Arduino
* ability to load files onto SD card

Or just grab a LaunchPad kit (coming soon...)

Getting Started
---------------
1. Assemble Arduino, networking sheild (if not built-in), SD card sheild (if not built-in)
2. Load all files in /webapp directory onto SD card
3. Tweak launch_pad.ino sketch in /arduino/launch_pad directory to represent your network configuration
4. Load sketch onto Arduino
5. 

!!!More to come!!!

What: Embedded system used collect data for or communicate output from IoT sites like Mission Control or Cosm

Why: A great project to contribute to for anyone who is interested in wirless sensor networks, client-side web application development, and any embedded system applications which could make use of these technologies to collect or communicate.
