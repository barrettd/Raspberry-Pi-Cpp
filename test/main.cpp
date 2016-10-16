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


}   // namespace tfs

int main( void ) {
    std::cout << "GPIO test application\n";
    
    struct timespec start;
    time_t elapsed_seconds;
    

    clock_gettime( CLOCK_MONOTONIC, &start );       // Get the start time.
    
    do {

        struct timespec now;
        clock_gettime( CLOCK_MONOTONIC, &now );
        elapsed_seconds = now.tv_sec - start.tv_sec;
    } while( elapsed_seconds < 10 );

    std::cout << "GPIO test application\n";

    return 0;
}

