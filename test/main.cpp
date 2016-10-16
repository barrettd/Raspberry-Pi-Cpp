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
    struct timespec end;
    
    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
    clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */
    
//    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    return 0;
}

