// ---------------------------------------------------------------------------
// gpio.hpp
//
// Created by Barrett Davis on 10/12/16.
// Copyright © 2016 Tree Frog Software. All rights reserved.
// MIT License: https://github.com/barrettd/Raspberry-Pi-Cpp/blob/master/LICENSE
// https://github.com/barrettd/Raspberry-Pi-Cpp.git
// ---------------------------------------------------------------------------
// Notes on GPIO / connector numbering (with diagrams)
// https://www.raspberrypi.org/documentation/usage/gpio-plus-and-raspi2/
// http://pinout.xyz
// ---------------------------------------------------------------------------
#ifndef gpio_hpp
#define gpio_hpp

#include <string>

namespace  tfs {
    
    // -----------------------------------------------------------------------
    // The GPIO pins can be referred to by the Broadcom pin numbers or the
    // P1 40 pin connector number.
    // -----------------------------------------------------------------------
    enum GPIO_ID {
    // -----------------------------------------------------------------------
    // Broadcom (BCM) logical ids
    // -----------------------------------------------------------------------
        GPIO_02 = 2,    // I2C1 SDA
        GPIO_03,        // I2C1 SCL
        GPIO_04,        // GPCLK0
        GPIO_05,
        GPIO_06,
        GPIO_07,        // SPI0 CE1
        GPIO_08,        // SPI0 CE0
        GPIO_09,        // SPI0 MISO
        GPIO_10,        // SPI0 MOSI
        GPIO_11,        // SPI0 SCLK
        GPIO_12,        // PWM0
        GPIO_13,        // PWM1
        GPIO_14,        // TXD
        GPIO_15,        // RXD
        GPIO_16,
        GPIO_17,
        GPIO_18,        // PWM0
        GPIO_19,
        GPIO_20,        // SPI1 MOSI
        GPIO_21,        // SPI1 SCLK
        GPIO_22,
        GPIO_23,
        GPIO_24,
        GPIO_25,
        GPIO_26,
        GPIO_27,
    // -----------------------------------------------------------------------
    // (Pi3) P1 connector pinouts, mapped to the Broadcom values internally.
    // -----------------------------------------------------------------------
    //  PIN_01 = 3.3 Volts
    //  PIN_02 = 5.0 Volts
        PIN_03 = GPIO_02,
    //  PIN_04 = 5.0 Volts
        PIN_05 = GPIO_03,
    //  PIN_06 = Ground
        PIN_07 = GPIO_04,
        PIN_08 = GPIO_14,
    //  PIN_09 = Ground
        PIN_10 = GPIO_14,
        PIN_11 = GPIO_17,
        PIN_12 = GPIO_18,
        PIN_13 = GPIO_27,
    //  PIN_14 = Ground,
        PIN_15 = GPIO_22,
        PIN_16 = GPIO_23,
    //  PIN_17 = 3.3 Volts
        PIN_18 = GPIO_24,
        PIN_19 = GPIO_10,
    //  PIN_20 = Ground,
        PIN_21 = GPIO_09,
        PIN_22 = GPIO_25,
        PIN_23 = GPIO_11,
        PIN_24 = GPIO_08,
    //  PIN_25 = Ground,
        PIN_26 = GPIO_07,
    // -----------------------------------------------------------------------
    // Early Pi2 connectors stopped here, Late Pi2 & Pi3 connectors continue.
    // -----------------------------------------------------------------------
    //  PIN_27 = GPIO_00 ID_SD, EEPROM Data
    //  PIN_28 = GPIO_01 ID_SC, EEPROM Clock
        PIN_29 = GPIO_05,
    //  PIN_30 = Ground,
        PIN_31 = GPIO_06,
        PIN_32 = GPIO_12,
        PIN_33 = GPIO_13,
    //  PIN_34 = Ground,
        PIN_35 = GPIO_19,
        PIN_36 = GPIO_16,
        PIN_37 = GPIO_26,
        PIN_38 = GPIO_20,
    //  PIN_39 = Ground,
        PIN_40 = GPIO_21
    };
    
    enum RESISTOR {
        RESISTOR_NONE = 0,
        RESISTOR_PULL_UP,
        RESISTOR_PULL_DOWN
    };
    
    enum STATUS {
        STATUS_OK = 0,              // Everything is awesome...
        STATUS_INTERNAL_BAD_ARG,    // This should not happen. Internal programming error.
        STATUS_ERROR_FILE_OPEN,     // Error opening a sysfs file. e.g. "/sys/class/gpio/export"
        STATUS_ERROR_FILE_WRITE,    // Error writing to a sysfs file after opening.
        STATUS_ERROR_FILE_READ,     // Error reading from a sysfs file after opening.
    };
    
    class Gpio {                    // Base class, use GpioInput or GpioOutput when you instantiate.
    protected:
        GPIO_ID     m_id;           // This is the Broadcom GPIO id.
        STATUS      m_status;       // This is the status from the last operation.
        std::string m_value_path;   // This is the cached Linux sysfs path for reading / writing GPIO values for this object.
        
    protected:
        bool setStatus( STATUS status );        // Set the status and return: status == STATUS_OK
        
        bool write( const char *path, const std::string &message ); // Write a string.
        bool write( const char *path, const bool value );           // Write a boolean.
        bool read(  const char *path, std::string &value );         // Read a string.
        bool read(  const char *path, bool &value );                // Read a boolean.
        
        bool writeExport(   void );             // "Open" the GPIO pin.
        bool writeUnexport( void );             // "Close" the GPIO pin.
        
        bool writeDirection( bool  input );     // Set the I/O direction: true (1) for input, false (0) for output
        bool readDirction(   bool &input );     // Get the I/O direction: true (1) for input, false (0) for output
        
        bool writeValue( bool  value );         // Write a boolean. Returns true for success, false for failure.
        bool readValue(  bool &value );         // Read a boolean.  Returns true for success, false for failure.
        
    public:
                 Gpio( GPIO_ID id );            // Constructor
        virtual ~Gpio( void );                  // Destructor (no need to call directly.)
        
        bool     setResistor( RESISTOR value ); // Set the resistor pull-up/down state
        RESISTOR getResistor( void );           // Get the resistor pull-up/down state
    
        STATUS clearStatus( void );             // Set the status to STATUS_OK
        STATUS getStatus( void ) const;         // Get the status
        bool   ok( void ) const;                // Test if the status == STATUS_OK
    };
    
    class GpioInput : public Gpio {             // Input GPIO object
    public:
        GpioInput( GPIO_ID id );                // Constructor
        
        bool read( bool &value );               // Read a boolean
    };
    
    class GpioOutput : public Gpio {            // Output GPIO object
    public:
        GpioOutput( GPIO_ID id );               // Constructor
        
        bool write( bool value );               // Write a boolean
    };
    
    
}   // namespace tfs

#endif // gpio_hpp 
