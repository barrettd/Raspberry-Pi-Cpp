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
    
    void test( void ) {
        GpioInput  button( GPIO_04 );
        GpioOutput led(    GPIO_14 );
        
        if( !button.ok() || !led.ok()) {
            std::cerr << "GPIO objects not well formed.\n";
            return;
        }
        
        struct timespec start;
        clock_gettime( CLOCK_MONOTONIC, &start );       // Get the start time.
        
        time_t elapsed_seconds;
        do {
            
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

