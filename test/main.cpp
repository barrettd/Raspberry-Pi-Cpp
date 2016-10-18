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
            case STATUS_TIMEOUT:          std::cerr << " time out\n";          break;
            case STATUS_INTERNAL_BAD_ARG: std::cerr << " error: internal\n";   break;
            case STATUS_ERROR_FILE_OPEN:  std::cerr << " error: file open\n";  break;
            case STATUS_ERROR_FILE_SEEK:  std::cerr << " error: file seek\n";  break;
            case STATUS_ERROR_FILE_WRITE: std::cerr << " error: file write\n"; break;
            case STATUS_ERROR_FILE_READ:  std::cerr << " error: file read\n";
        }
        return;
    }
    
    void testLoop( const GPIO_ID buttonPin, const GPIO_ID ledPin, const time_t maxTime ) {
        // ---------------------------------------------------------------------------
        // Set up an input pin & output pin.
        // Read the value from the input and write the value to the output.
        // Repeat for 10 seconds, then stop.
        // ---------------------------------------------------------------------------
        GpioInput  button( buttonPin );
        GpioOutput led(    ledPin );
        
        if( !button.ok() || !led.ok()) {
            std::cerr << "GPIO objects not well formed.\n";
            emitStatus( "button", button );
            emitStatus( "led",    led );
            return;                     // We have a problem, do not continue.
        }
        
        struct timespec start;
        clock_gettime( CLOCK_MONOTONIC, &start );           // Get the start time.
        
        time_t elapsed_seconds = 0;
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
        } while( elapsed_seconds < maxTime );

        return;
    }
    
    void testEdge( const GPIO_ID buttonPin, const GPIO_ID ledPin, const time_t maxTime ) {
        // ---------------------------------------------------------------------------
        // Set up an input pin & output pin.
        // Set the input pin to trigger on a rising edge.
        // Toggle a value and send it to the LED.
        // Repeat for 10 seconds, then stop.
        // ---------------------------------------------------------------------------
        GpioInput  button( buttonPin );
        GpioOutput led(    ledPin );
        
        if( !button.ok() || !led.ok()) {
            std::cerr << "GPIO objects not well formed.\n";
            emitStatus( "button", button );
            emitStatus( "led",    led );
            return;                     // We have a problem, do not continue.
        }
        
        // Set the button to trigger on a rising edge.
        if( !button.setEdge( EDGE_RISING )) {
            std::cerr << "Error setting button to trigger on rising edge.\n";
            emitStatus( "button", button );
            return;
        }
        
        bool ledState = false;                              // This is the LED state that we toggle.
        struct timespec start;
        clock_gettime( CLOCK_MONOTONIC, &start );           // Get the start time.
        
        time_t elapsed_seconds = 0;
        do {
            bool value;
            if( !button.read_wait( value, 1 )) {            // Read a value from the button with a blocking call.
                if( button.getStatus() == STATUS_TIMEOUT ) {
                    continue;                               // This is OK, just timeout with no activity.
                }
                std::cerr << "Error reading value from the button.\n";
                emitStatus( "button", button );
                return;
            }
            if( !led.write( ledState )) {                   // Write the value to the LED.
                std::cerr << "Error writing value to the LED.\n";
                emitStatus( "led", led );
                return;
            }
            ledState = !ledState;                           // Toggle the LED state value.
            struct timespec now;
            clock_gettime( CLOCK_MONOTONIC, &now );         // Get the current time.
            elapsed_seconds = now.tv_sec - start.tv_sec;    // Calculate the elapsed time in seconds.
        } while( elapsed_seconds < maxTime );
        
        return;
    }

    
    void gpio_test( void ) {
        const GPIO_ID buttonPin = GPIO_04;                  // PIN_07 - Input button on GPIO pin 4.
        const GPIO_ID ledPin    = GPIO_14;                  // PIN_08 - Output LED on GPIO pin 14.
        const time_t  MAX_RUN_TIME_IN_SECONDS = 10;         // Max time per test.
        
        std::cout << "Loop test start\n";
        testLoop( buttonPin, ledPin, MAX_RUN_TIME_IN_SECONDS );
        std::cout << "Loop test complete\n";

        std::cout << "Edge test start\n";
        testEdge( buttonPin, ledPin, MAX_RUN_TIME_IN_SECONDS );
        std::cout << "Edge test complete\n";

        
        return;
    }
    

}   // namespace tfs


int main( void ) {
    // ---------------------------------------------------------------------------
    // Main entry point for our test application.
    // ---------------------------------------------------------------------------
    std::cout << "GPIO tests start\n";
    
    tfs::gpio_test();    // Run our simple GPIO tests above.
    
    std::cout << "GPIO tests complete\n";
    return 0;
}

