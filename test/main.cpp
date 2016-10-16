// ---------------------------------------------------------------------------
//  main.cpp
//
//  Created by Barrett Davis on 10/12/16.
//  Copyright © 2016 Tree Frog Software. All rights reserved.
// ---------------------------------------------------------------------------
#include <ctime>      // Needed for clock_gettime()
#include <iostream>
#include "gpio.hpp"

namespace  tfs  {
    
    void emitStatus( const char *label, Gpio &pin ) {
        // ---------------------------------------------------------------------------
        // Emit the status of the given GPIO pin object.
        // ---------------------------------------------------------------------------
        std::cerr << label;
        switch( pin.getStatus()) {
            case STATUS_OK:               std::cerr << " ok\n";                break;
            case STATUS_INTERNAL_BAD_ARG: std::cerr << " error: internal\n";   break;
            case STATUS_ERROR_FILE_OPEN:  std::cerr << " error: file open\n";  break;
            case STATUS_ERROR_FILE_WRITE: std::cerr << " error: file write\n"; break;
            case STATUS_ERROR_FILE_READ:  std::cerr << " error: file read\n";
        }
        return;
    }
    
    void test( void ) {
        // ---------------------------------------------------------------------------
        // Set up an input pin & output pin.
        // Read the value from the input and write the value to the output.
        // Repeat for 10 seconds, then stop.
        // ---------------------------------------------------------------------------
        const time_t MAX_RUN_TIME_IN_SECONDS = 10;
        
        GpioInput  button( GPIO_04 );   // We expect an input button on GPIO pin 4.
        GpioOutput led(    GPIO_14 );   // We expect an output LED on GPIO pin 14.
        
        if( !button.ok() || !led.ok()) {
            std::cerr << "GPIO objects not well formed.\n";
            emitStatus( "button", button );
            emitStatus( "led",    led );
            return;                     // We have a problem, do not continue.
        }
        
        struct timespec start;
        clock_gettime( CLOCK_MONOTONIC, &start );           // Get the start time.
        
        time_t elapsed_seconds;
        do {
            bool value;
            if( !button.read( value )) {                    // Read a value from the button.
                std::cerr << "Error reading value from the button.\n";
                emitStatus( "button", button );
                return;
            }
            if( !led.write( value )) {                      // Write the value to the LED.
                std::cerr << "Error writing value to the LED.\n";
                emitStatus( "led", led );
                return;
            }
            struct timespec now;
            clock_gettime( CLOCK_MONOTONIC, &now );         // Get the current time.
            elapsed_seconds = now.tv_sec - start.tv_sec;    // Calculate the elapsed time in seconds.
        } while( elapsed_seconds < MAX_RUN_TIME_IN_SECONDS );

        return;
    }
    

}   // namespace tfs


int main( void ) {
    // ---------------------------------------------------------------------------
    // Main entry point for our test application.
    // ---------------------------------------------------------------------------
    std::cout << "GPIO test start\n";
    
    tfs::test();    // Run our simple loop test above.
    
    std::cout << "GPIO test complete\n";
    return 0;
}

