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
#include <fstream>
#include <sstream>
#include "gpio.hpp"


namespace  tfs {
   
#pragma mark - GPIO Base class
    
    Gpio::Gpio( GPIO_ID id ):
    m_id( id ),
    m_status( STATUS_OK ) {
        std::stringstream path;
        path << "/sys/class/gpio/gpio" << m_id << "/value";
        m_value_path = path.str();
        writeExport();
    }
    
    Gpio::~Gpio( void ) {
        writeUnexport();
    }
    
    bool
    Gpio::setResistor( RESISTOR value ) {
        return true;
    }
    
    RESISTOR
    Gpio::getResistor( void ) {
        return RESISTOR_NONE;
    }
    
    STATUS
    Gpio::clearStatus( void ) {
        return m_status = STATUS_OK;
    }
    
    STATUS
    Gpio::getStatus( void ) const {
        return m_status;
    }

    bool
    Gpio::setStatus( STATUS status ) {
        m_status = status;
        return m_status == STATUS_OK;
    }
    
    bool
    Gpio::write( const char *path, const std::string &message ) {
        if( path == 0 || *path == 0 || message.empty()) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ofstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        bool result = setStatus( STATUS_OK );
        stream << message;
        if( stream.bad()) {
            result = setStatus( STATUS_ERROR_FILE_WRITE );
        }
        stream.close();
        return result;
    }
    
    bool
    Gpio::read( const char *path, std::string &value ) {
        if( path == 0 || *path == 0 ) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ifstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        bool result = setStatus( STATUS_OK );
        stream >> value;
        if( stream.bad()) {
            result = setStatus( STATUS_ERROR_FILE_READ );
        }
        stream.close();
        return result;              // Important to check read status.
    }
    
    bool
    Gpio::read( const char *path, bool &value ) {
        if( path == 0 || *path == 0 ) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ifstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        bool result = setStatus( STATUS_OK );
        const int val = stream.get();
        if( stream.bad()) {
            result = setStatus( STATUS_ERROR_FILE_READ );
        }
        stream.close();
        value = val == '1';         // We expect '1' or '0'
        return result;              // Important to check read status.
    }

    
    bool
    Gpio::writeExport( void ) {
        std::stringstream message;
        message << m_id;
        return write( "/sys/class/gpio/export", message.str());
    }
    
    bool
    Gpio::writeUnexport( void ) {
        std::stringstream message;
        message << m_id;
        return write( "/sys/class/gpio/unexport", message.str());
    }
    
    bool
    Gpio::writeDirection( bool input ) {
        std::string message;
        if( input ) {
            message = "in";
        } else {
            message = "out";
        }
        std::stringstream path;
        path << "/sys/class/gpio/gpio" << m_id << "/direction";
        return write( path.str().c_str(), message );
    }
    
    bool
    Gpio::readDirction( bool &input ) {
        std::string contents;
        std::stringstream path;
        path << "/sys/class/gpio/gpio" << m_id << "/direction";
        if( !read( path.str().c_str(), contents )) {
            return false;
        }
        if( contents.empty()) {
            return setStatus( STATUS_ERROR_FILE_READ );
        }
        input = contents.compare( "in" ) == 0;
        return setStatus( STATUS_OK );
    }
    
    bool
    Gpio::writeValue( bool value ) {
        if( value ) {
            return write( m_value_path.c_str(), "1" );
        }
        return write( m_value_path.c_str(), "0" );
    }
    
    bool
    Gpio::readValue( bool &value ) {
        return read( m_value_path.c_str(), value );
    }

    
#pragma mark - GPIO Input class
    
    GpioInput::GpioInput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( false );
    }
    
    bool
    GpioInput::read( bool &value ) {
        return readValue( value );
    }

    
#pragma mark - GPIO Output class
    
    GpioOutput::GpioOutput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( true );
    }
    
    bool
    GpioOutput::write( bool value ) {
        return writeValue( value );
    }
    
}   // namespace tfs
