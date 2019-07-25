#include "EventType.h"

// Gets the string representation of an EventType.
std::string KeyDaemon::getEventString(const EventType type)
{
    switch (type)
    {
        case EventType::released:
            return "released";
        case EventType::pressed:
            return "pressed";
        case EventType::held:
            return "held";
        default:
            return "Invalid";
    }
}
