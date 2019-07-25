#include "KeyLoop.h"
#include "KeyCode.h"
#include "KeyExitCode.h"
#include "KDDebug.h"
#include <iostream>

#ifdef KD_DEBUG
static const constexpr char* messagePrefix = "DaemonFramework Main: ";
#endif

int main(int argc, char** argv)
{
    using namespace KeyDaemon;
    DBG_V(messagePrefix << "Launching daemon with " << argc << " arguments.");
    std::vector<int> keyCodes = KeyCode::parseCodes(argc, argv);
    
    if (keyCodes.empty())
    {
        DBG(messagePrefix << "Exiting: tracked key codes were invalid.");
        return (int) KeyExitCode::badTrackedKeys;
    }
    DBG_V(messagePrefix << "Daemon tracking " << keyCodes.size() << " keys.");
    KeyDaemon::KeyLoop daemonLoop(keyCodes);
    int returnCode = daemonLoop.runLoop();
    DBG(messagePrefix << "KeyDaemon exiting with code " << returnCode);
}
