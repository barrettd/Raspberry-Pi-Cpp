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
   
// #pragma mark - GPIO Base class
    
    Gpio::Gpio( GPIO_ID id ):
    m_id( id ),
    m_status( STATUS_OK ) {
        std::stringstream path;
        path << "/sys/class/gpio/gpio" << m_id << "/value";
        m_value_path = path.str();  // Cache this frequenly used sysfs path
        writeExport();              // Open this pin
    }
    
    Gpio::~Gpio( void ) {
        writeUnexport();            // Close this pin
    }
    
    bool
    Gpio::setResistor( RESISTOR value ) {
        return true;
    }
    
    RESISTOR
    Gpio::getResistor( void ) {
        return RESISTOR_NONE;
    }
    
    bool
    Gpio::ok( void ) const {
        return m_status == STATUS_OK;
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
        // ---------------------------------------------------------------------------
        // This method is used internally to set the status and provide a consistent return value.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        m_status = status;
        return m_status == STATUS_OK;
    }
    
    bool
    Gpio::write( const char *path, const std::string &message ) {
        // ---------------------------------------------------------------------------
        // Write a string to a file, set this object status.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( path == 0 || *path == 0 || message.empty()) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ofstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        STATUS status = STATUS_OK;
        stream << message;
        if( stream.bad()) {
            status = STATUS_ERROR_FILE_WRITE;
        }
        stream.close();
        return setStatus( status ); // Important for caller to check this result.
    }
    
    bool
    Gpio::write( const char *path, const bool value ) {
        // ---------------------------------------------------------------------------
        // Write a boolean to a file, set this object status.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( path == 0 || *path == 0 ) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        char buffer[2];
        if( value ) {
            buffer[0] = '1';
        } else {
            buffer[0] = '0';
        }
        buffer[1] = 0;              // Not strictly needed.
        std::ofstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        STATUS status = STATUS_OK;
        stream.write( buffer, 1 );
        if( stream.bad()) {
            status = STATUS_ERROR_FILE_WRITE;
        }
        stream.close();
        return setStatus( status ); // Important for caller to check this result.
    }
    
    bool
    Gpio::read( const char *path, std::string &value ) {
        // ---------------------------------------------------------------------------
        // Read a string from a file, set this object status.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( path == 0 || *path == 0 ) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ifstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        STATUS status = STATUS_OK;
        stream >> value;
        if( stream.bad()) {
            status = STATUS_ERROR_FILE_READ;
        }
        stream.close();
        return setStatus( status ); // Important for caller to check this result.
    }
    
    bool
    Gpio::read( const char *path, bool &value ) {
        // ---------------------------------------------------------------------------
        // Read a boolean from a file, set this object status.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( path == 0 || *path == 0 ) {
            return setStatus( STATUS_INTERNAL_BAD_ARG );
        }
        std::ifstream stream( path );
        if( !stream ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        STATUS status = STATUS_OK;
        const int val = stream.get();
        if( stream.bad()) {
            status = STATUS_ERROR_FILE_READ;
        }
        stream.close();
        value = val == '1';         // We expect '1' or '0'
        return setStatus( status ); // Important for caller to check this result.
    }

    bool
    Gpio::writeExport( void ) {
        // ---------------------------------------------------------------------------
        // Open the GPIO pin
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        std::stringstream message;
        message << m_id;
        return write( "/sys/class/gpio/export", message.str());
    }
    
    bool
    Gpio::writeUnexport( void ) {
        // ---------------------------------------------------------------------------
        // Close the GPIO pin
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        std::stringstream message;
        message << m_id;
        return write( "/sys/class/gpio/unexport", message.str());
    }
    
    bool
    Gpio::writeDirection( bool input ) {
        // ---------------------------------------------------------------------------
        // Set the write direction.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
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
        // ---------------------------------------------------------------------------
        // Read the write direction, set this object status.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
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
        // ---------------------------------------------------------------------------
        // Write a boolean to the GPIO pin
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        return write( m_value_path.c_str(), value );
    }
    
    bool
    Gpio::readValue( bool &value ) {
        // ---------------------------------------------------------------------------
        // Read a bolean from the GPIO pin.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        return read( m_value_path.c_str(), value );
    }

    
// #pragma mark - GPIO Input class
    
    GpioInput::GpioInput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( true );     // true (1) == input
    }
    
    bool
    GpioInput::read( bool &value ) {
        // ---------------------------------------------------------------------------
        // Read a boolean from the GPIO pin.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        return readValue( value );
    }

    
// #pragma mark - GPIO Output class
    
    GpioOutput::GpioOutput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( false );    // false (0) == output
    }
    
    bool
    GpioOutput::write( bool value ) {
        // ---------------------------------------------------------------------------
        // Write a boolean to the GPIO pin.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        return writeValue( value );
    }
    
}   // namespace tfs
