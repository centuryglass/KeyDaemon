/**
 * @file  KeyMessage.h
 *
 * @brief  The data format used by the KeyLoop to send new key events to the
 *         parent application.
 */

#pragma once
#include "EventType.h"

namespace KeyDaemon
{
    struct KeyMessage
    {
        // A Linux keyboard input code:
        int keyCode = 0;
        // The type of keyboard input event:
        EventType event = EventType::pressed;
    };
}
