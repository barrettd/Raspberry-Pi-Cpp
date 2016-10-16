# Raspberry-Pi-Cpp
Version 0.00 - Under initial development. Do not use yet.

pi_lib, a little C++ library for the Raspberry Pi running Debian Raspbian Jessie (2016-09-23-raspbian-jessie.)

All of this code has been tested with a Raspberry Pi 3 Model B V1.2.

The Rasbain Jessie release that I am using does not natively come with CMake, so we use Makefiles to build the pi_lib and test app.

There are Makefiles in the lib/ and test/ directories.  We build "out of source", meaning that the 
output of the build is not mixed in with our source files.

The first run the Makefile in lib/ to make the pi_lib.a, then run the Makefile in test/ to make the test app to see how the code works.

The intermediate and final files are placed into a build/ directory, away from the source files.  
This allows you to easily keep the source and build products separate.

Here are the steps to follow to build the pi_lib and the test app.
cd lib/
make directories
make
cd ../test
make directories
make

Then to run the test app:
cd ../build
bin/GpioTest

I hope that you have fun with this.

Let me know if you have any issues or would like additions.

Regards,

Barrett
barrett@thefrog.com

https://github.com/barrettd/Raspberry-Pi-Cpp.git
