/**
 * @file  TestParent.cpp
 * 
 * @brief  A minimal parent application implementation used when testing
 *         the KeyDaemon.
 */

#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <unistd.h>
#include "Controller.h"
#include "../Daemon/KeyCode.h"
#include "EventType.h"

// Print the application name before all info/error output:
static const constexpr char* messagePrefix = "TestParent: ";

// Prints key codes read from the PipeReader:
class DaemonController : public KeyDaemon::Controller
{
public:
    DaemonController() { }
    virtual ~DaemonController() { }

private:
    virtual void handleKeyEvent(const KeyDaemon::KeyMessage& keyMessage)
    {
        std::string keyString
                = KeyDaemon::KeyCode::getKeyString(keyMessage.keyCode);
        std::string eventString = KeyDaemon::getEventString(keyMessage.event);
        std::cout << "Key " << keyString << "[" << keyMessage.keyCode
                << "]: " << eventString << "\n";
    }
};


int main(int argc, char** argv)
{
    bool killParent = false;
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "-k" || arg == "--kill")
        {
            killParent = true;
        }
    }
    using namespace KeyDaemon;
    DaemonController controller;
    std::vector<int> trackedCodes;
    trackedCodes.reserve(239);
    for (int i = 1; i < 240; i++)
    {
        trackedCodes.push_back(i);
    }
    controller.startKeyDaemon(trackedCodes);
    if (killParent)
    {
        sleep(1);
        std::cout << "Killing TestParent to see how the daemon reacts: \n";
        exit(0);
    }
    sleep(10);
    std::cout << "Timeout complete, stopping daemon.\n";
    controller.stopDaemon();
    return controller.getExitCode();
}
