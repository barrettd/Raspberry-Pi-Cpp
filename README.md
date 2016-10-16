# Raspberry-Pi-Cpp
pi_lib version 0.00 - Under initial development. Do not use yet.

pi_lib is a little C++ library for the Raspberry Pi running Debian Raspbian Jessie (2016-09-23-raspbian-jessie.)

All of this code has been tested with a Raspberry Pi 3 Model B V1.2.  Although is *should* run on other versions of Pi with the same OS.
The GPIO control is done with the Linux sysfs system & drivers which should make this code fairly portable.

The Rasbain Jessie release that I am using does not natively come with CMake, so we use Makefiles to build the pi_lib and the test app.

There are Makefiles in the lib/ and test/ directories.  We build "out of source", meaning that the 
output of the build is not mixed in with our source files.

The first run the Makefile in lib/ to create the pi_lib.a, then run the Makefile in test/ to create the GpioTest to demonstrate how the code works.

The intermediate and final files are placed into a build/ directory, away from the source files.  
This allows you to easily keep the source and build products separate.

Here are the steps to follow to build the pi_lib and the test app:

1. cd lib/
2. make directories
3. make
4. cd ../test
5. make directories
6. make

Then to run the test app:

1. cd ../build
2. sudo bin/GpioTest

The test application, GpioTest, sets up an input GPIO object for GPIO_04 (a button) and an output GPIO object for GPIO_14 (a LED.)
The application loops for 10 seconds reading the state from the button and writing state to the LED.  Given the proper circuit protection, you should be able to put this together on a proto board and watch the LED respond to your button press.

Remember that the GPIO pins can be harmed by voltages higher than 3.3V.  A good value for the LED current limiting resistor may be around 330KΩ and the pull-up resisor on the button may be about 10KΩ.

I hope that you have fun with this.

Let me know if you have any issues or would like additions.

TODO:

1. Pulse Width Modulation (PWM)
2. Pull up / down resistors
3. Interrupts, callbacks
4. Circuit diagram (or photo) for GpioTest setup.

Regards,

Barrett Davis
Tree Frog Software
barrett@thefrog.com

https://github.com/barrettd/Raspberry-Pi-Cpp.git
