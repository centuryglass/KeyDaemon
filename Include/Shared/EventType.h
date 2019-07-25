/**
 * @file  EventType.h
 *
 * @brief  Defines key event codes used when reading keyboard event files or
 *         sending keyboard events to KeyDaemon's parent process.
 */

#pragma once
#include <string>

namespace KeyDaemon
{
    enum class EventType
    {
        released = 0,
        pressed = 1,
        held = 2,
        trackedTypeCount = 3
    };

    /**
     * @brief  Gets the string representation of an EventType.
     *
     * @param type  The type value to describe.
     *
     * @return      A string representing the type, or "Invalid" if the type is
     *              invalid.
     */
    std::string getEventString(const EventType type);
}
