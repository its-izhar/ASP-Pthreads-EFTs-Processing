/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:04-05:00
* @Email:  izharits@gmail.com
* @Filename: debugMacros.hpp
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-13T21:04:53-05:00
*/


#ifndef __DEBUG_MACROS__
#define __DEBUG_MACROS__

#include <iostream>
#include <iomanip>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define SUCCESS 0
#define FAIL    1

#define dbg_trace( print_message ) \
        do { if(DEBUG_TEST) \
            std::cerr << \
            "DEBUG" << "(" << __LINE__ << "):" << __func__ << "():" \
            << std::setw(40) << print_message << std::endl; \
        } while(0)

#define print_output( message ) \
        do { std::cout << message << std::endl; \
        } while(0)

#endif
