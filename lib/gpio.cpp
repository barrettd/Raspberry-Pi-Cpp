// ---------------------------------------------------------------------------
// gpio.cpp
//
// Created by Barrett Davis on 10/12/16.
// Copyright © 2016 Tree Frog Software. All rights reserved.
// MIT License: https://github.com/barrettd/Raspberry-Pi-Cpp/blob/master/LICENSE
// https://github.com/barrettd/Raspberry-Pi-Cpp.git
// ---------------------------------------------------------------------------
// Notes on accessing GIPO with Linux sysfs:
// https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
// GPIO hardware discussion:
// http://elinux.org/RPi_Low-level_peripherals
// ---------------------------------------------------------------------------

#include "gpio.hpp"


namespace  tfs {
    static const char *filePath = "/sys/class/gpio/export";
    
#pragma mark - GPIO Base class
    
    Gpio::Gpio( GPIO_ID id ):
    m_id( id ),
    m_resistor( RESISTOR_NONE ) {
    }
    
    Gpio::~Gpio( void ) {
    }
    
    bool
    Gpio::setResistor( RESISTOR value ) {
        m_resistor = value;
        return true;
    }
    
    RESISTOR
    Gpio::getResistor( void ) {
        return m_resistor;
    }
    
    
#pragma mark - GPIO Input class
    
    GpioInput::GpioInput( GPIO_ID id ):
    Gpio( id ) {
    }
    
    bool
    GpioInput::read( void ) {
        return true;
    }

    
#pragma mark - GPIO Output class
    
    GpioOutput::GpioOutput( GPIO_ID id ):
    Gpio( id ) {
    }
    
    bool
    GpioOutput::write( bool value ) {
        return true;
    }
    
}   // namespace tfs
