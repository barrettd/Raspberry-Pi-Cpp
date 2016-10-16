//
//  main.cpp
//
//  Created by Barrett Davis on 10/12/16.
//  Copyright © 2016 Tree Frog Software. All rights reserved.
//
#include <ctime>      // clock_gettime
#include <iostream>
#include "gpio.hpp"

namespace  tfs  {
    
    void emitStatus( const char *label, Gpio &pin ) {
        std::cerr << label;
        switch( pin.getStatus()) {
            case STATUS_OK:               std::cerr << " ok\n"; break;
            case STATUS_INTERNAL_BAD_ARG: std::cerr << " error: internal\n"; break;
            case STATUS_ERROR_FILE_OPEN:  std::cerr << " error: file open\n"; break;
            case STATUS_ERROR_FILE_WRITE: std::cerr << " error: file write\n"; break;
            case STATUS_ERROR_FILE_READ:  std::cerr << " error: file read\n"; break;
        }
        return;
    }
    
    void test( void ) {
        GpioInput  button( GPIO_04 );   // We expect a button on GPIO pin 4
        GpioOutput led(    GPIO_14 );   // We expect an LED on GPIO pin 14
        
        if( !button.ok() || !led.ok()) {
            std::cerr << "GPIO objects not well formed.\n";
            emitStatus( "button", button );
            emitStatus( "led",    led );
            return;
        }
        
        struct timespec start;
        clock_gettime( CLOCK_MONOTONIC, &start );       // Get the start time.
        
        time_t elapsed_seconds;
        do {
            bool value;
            if( !button.read( value )) {
                std::cerr << "Error reading value from button.\n";
                emitStatus( "button", button );
                return;
            }
            if( !led.write( value )) {
                std::cerr << "Error writing value to LED.\n";
                emitStatus( "led", led );
                return;
            }
            struct timespec now;
            clock_gettime( CLOCK_MONOTONIC, &now );
            elapsed_seconds = now.tv_sec - start.tv_sec;
        } while( elapsed_seconds < 10 );

        return;
    }

    

}   // namespace tfs

int main( void ) {
    std::cout << "GPIO test application\n";
    
    tfs::test();
    
    std::cout << "GPIO test application\n";
    return 0;
}

