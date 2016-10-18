# Raspberry-Pi-Cpp
pi_lib version 1.10 - 18 Oct 2016 Added edge detection and blocking GPIO read.  

pi_lib version 1.00 - 16 Oct 2016 Initial release.  


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

The test application, GpioTest, sets up two GPIO pins.  

One pin, GPIO_04, is configured as an input to sense a momentary button.  

The other pin, GPIO_14, is configured as an output to drive a LED.

The first test loops for 10 seconds, reading the button state and then sending that state to the LED.

The second test also runs for 10 seconds, but this time the GPIO_04 line is configured to trigger on a rising edge signal.  
We perform a blocking read on the input pin.  When the edge is detected, we toggle the LED state.

Remember that the GPIO pins can be harmed by voltages higher than 3.3V or by excessive currents.
A good value for the LED current limiting resistor may be around 330Ω and the pull-up resistor on the button may be about 10KΩ.

![Wiring for Gpiotest](https://github.com/barrettd/Raspberry-Pi-Cpp/blob/master/images/breadboard.png)

Thanks to the [Fritzing](http://fritzing.org/home/) for a great open source ECAD project. 

See our friends at Adafruit for detailed example of how to connect LEDs to Pi GPIO Pins: 
https://learn.adafruit.com/raspberry-pi-e-mail-notifier-using-leds/wire-leds

I hope that you have fun with this.

Please let me know if you have any issues or would like additions.  
I would love to hear about how you incorporate this library into your projects.

TODO:

1. Pulse Width Modulation (PWM)
2. Pull up / down resistors

Regards,

[Barrett Davis](http://thefrog.com/barrett/)
barrett@thefrog.com

[Tree Frog Software](http://www.thefrog.com)


https://github.com/barrettd/Raspberry-Pi-Cpp


