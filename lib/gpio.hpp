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
    
    // Pull up, pull down resisters
    // Server, PWM
    
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
    // (Pi 3) P1 connector pinouts
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
    //  PIN_27 = GPIO_00 ID_SD,
    //  PIN_28 = GPIO_01 ID_SC,
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
        STATUS_OK = 0,
        STATUS_INTERNAL_BAD_ARG,
        STATUS_ERROR_FILE_OPEN,
        STATUS_ERROR_FILE_WRITE,
        STATUS_ERROR_FILE_READ,
    };
    
    class Gpio {
    protected:
        GPIO_ID     m_id;
        STATUS      m_status;
        std::string m_value_path;
        
    protected:
        bool setStatus( STATUS status );
        
        bool write( const char *path, const std::string &message );
        bool read(  const char *path, bool &value );
        
        bool writeExport( void );
        bool writeUnexport( void );
        
        bool writeDirection( bool input );      // true (1) for input, false (0) for output
        bool readDirction(  bool &input );
        
        bool writeValue( bool value );
        bool readValue( bool &value );
        
    public:
                 Gpio( GPIO_ID id );
        virtual ~Gpio( void );
        
        bool     setResistor( RESISTOR value );
        RESISTOR getResistor( void );
        
        STATUS clearStatus( void );
        STATUS getStatus( void ) const;

    };
    
    class GpioInput : public Gpio {
    public:
        GpioInput( GPIO_ID id );
        
        bool read( bool &value );
        
    };
    
    class GpioOutput : public Gpio {
    public:
        GpioOutput( GPIO_ID id );
        
        bool write( bool value );
    
    };
    
    
}   // namespace tfs

#endif // gpio_hpp 
