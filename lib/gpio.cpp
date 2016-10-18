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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "gpio.hpp"


namespace  tfs {
   
    static const int CLOSED_FD = -1;
    
// ---------------------------------------------------------------------------
// #pragma mark - GPIO Base class
// ---------------------------------------------------------------------------
    
    Gpio::Gpio( GPIO_ID id ):
    m_id( id ),
    m_status( STATUS_OK ),
    m_fd( CLOSED_FD ) {
        writeExport();              // Open this pin
    }
    
    Gpio::~Gpio( void ) {
        close();
        writeUnexport();            // Close this pin
    }
    
    GPIO_ID
    Gpio::getId( void ) const {
        return m_id;
    }

    bool
    Gpio::open( const int direction ) {     // direction = O_RDONLY or O_WRONLY
        // Open the value file and keep it open for the lifetime of the object.
        // We use the low level C methods to give us access to poll(), select(), seek() etc.
        std::stringstream path;
        path << "/sys/class/gpio/gpio" << m_id << "/value";
        m_fd = ::open( path.str().c_str(), direction );
        if( m_fd < 0 ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        return setStatus( STATUS_OK );
    }
    
    void
    Gpio::close( void ) {
        if( m_fd >= 0 ) {
            ::close( m_fd );
            m_fd = CLOSED_FD;
        }
    }
    
    int
    Gpio::getFileDescriptor( void ) const {
        return m_fd;
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
    
    
// ---------------------------------------------------------------------------
// #pragma mark - GPIO Input class
// ---------------------------------------------------------------------------

    GpioInput::GpioInput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( true );     // true (1) == input
        open( O_RDONLY );
    }
    
    bool
    GpioInput::read( bool &value ) {
        // ---------------------------------------------------------------------------
        // Read a boolean from the GPIO pin.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( m_fd < 0 ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        const size_t offset = lseek( m_fd, 0, SEEK_SET );       // Seek to beginning
        if( offset != 0 ) {
            return setStatus( STATUS_ERROR_FILE_SEEK );
        }
        char buffer[2];
        buffer[0] = 0;
        buffer[1] = 0;          // Unused, but provides null termination if debugging
        const size_t count = ::read( m_fd, buffer, 1 );
        if( count < 1 ) {
            return setStatus( STATUS_ERROR_FILE_READ );
        }
        value = buffer[0] == '1';
        return setStatus( STATUS_OK );
    }

    
// ---------------------------------------------------------------------------
// #pragma mark - GPIO Output class
// ---------------------------------------------------------------------------

    GpioOutput::GpioOutput( GPIO_ID id ):
    Gpio( id ) {
        writeDirection( false );    // false (0) == output
        open( O_WRONLY );
    }
    
    bool
    GpioOutput::write( bool value ) {
        // ---------------------------------------------------------------------------
        // Write a boolean to the GPIO pin.
        // Returns true for success, false for failure.
        // ---------------------------------------------------------------------------
        if( m_fd < 0 ) {
            return setStatus( STATUS_ERROR_FILE_OPEN );
        }
        const size_t offset = lseek( m_fd, 0, SEEK_SET );       // Seek to beginning
        if( offset != 0 ) {
            return setStatus( STATUS_ERROR_FILE_SEEK );
        }
        char buffer[2];
        if( value ) {
            buffer[0] = '1';
        } else {
            buffer[0] = '0';
        }
        buffer[1] = 0;          // Unused, but provides null termination if debugging
        const size_t count = ::write( m_fd, buffer, 1 );
        if( count < 1 ) {
            return setStatus( STATUS_ERROR_FILE_WRITE );
        }
        return setStatus( STATUS_OK );
    }
    
}   // namespace tfs
