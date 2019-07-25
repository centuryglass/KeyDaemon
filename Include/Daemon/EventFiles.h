/**
 * @file  EventFiles.h
 *
 * @brief  Finds, identifies, and opens Linux keyboard event files.
 */

#pragma once
#include <vector>
#include <string>

namespace KeyDaemon
{
    namespace EventFiles
    {
        /**
         * @brief  Gets paths for all valid keyboard input event files.
         *
         * @return  The list of valid keyboard file paths.
         */
        std::vector<std::string> getPaths();
    }
}
