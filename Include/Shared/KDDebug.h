/**
 * @file  Debug.h
 *
 * @brief  Provides debugging macros that are removed in release builds.
 */

// Use different colors for the parent, daemon, and others.
#define PROC_COLOR_CODED 1

#pragma once
#ifdef KD_DEBUG
#   include <iostream>
#   include <cassert>
#   include <cstdio>
#   ifdef PROC_COLOR_CODED
#       if DF_IS_DAEMON
#           define PROC_COLOR "\033[31mD: " <<
#       elif DF_IS_PARENT
#           define PROC_COLOR "\033[36mP: " <<
#       else
#           error "Failed to define daemon or parent!"
#           define PROC_COLOR "\033[37m?: " <<
#       endif
#       define PROC_RESET "\033[0m" <<
#   else  
#       define PROC_COLOR
#       define PROC_RESET
#   endif

// Prints a line of debug output:
#   define DBG(toPrint) std::cout << PROC_COLOR toPrint << PROC_RESET "\n";

// Prints a line of verbose debug output:
#   ifdef KD_VERBOSE
#       define DBG_V(toPrint) DBG(toPrint)
#   else
#       define DBG_V(toPrint)
#   endif

// Prints a C-style error message:
#   define PERROR(toPrint) perror(toPrint);

// Terminates the program if a test condition is not met:
#   define ASSERT(condition) assert(condition);

// Redefine debug macros as empty statements outside of debug builds:
#else
#   define DBG(toPrint)
#   define DBG_V(toPrint)
#   define PERROR(toPrint)
#   define ASSERT(condition)
#endif
