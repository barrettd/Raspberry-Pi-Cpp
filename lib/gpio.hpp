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
// ---------------------------------------------------------------------------
#ifndef gpio_hpp
#define gpio_hpp

namespace  tfs {
    
    class Gpio {
    public:
                 Gpio( int number );     // Logical GPIO Number, not connector pin number.
        virtual ~Gpio( void );
        
        
        
    };
    
}   // namespace tfs

#endif // gpio_hpp 
