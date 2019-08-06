#include "Controller.h"
#include "KDDebug.h"
#include <algorithm>

#ifdef KD_DEBUG
static const constexpr char* messagePrefix = "KeyDaemon::Controller::";
#endif


// Configures the daemon output pipe on construction.
KeyDaemon::Controller::Controller() :
DaemonFramework::DaemonControl(KD_DAEMON_PATH, "", KD_PIPE_PATH,
        sizeof(KeyMessage)) { }


// Launches the KeyDaemon if it isn't already running.
void KeyDaemon::Controller::startKeyDaemon
(const std::vector<int> trackedKeyCodes)
{
    DBG_V(messagePrefix << __func__ << ": Launching daemon to track "
            << trackedKeyCodes.size() << " key codes.");
    std::vector<std::string> codeArguments;
    codeArguments.reserve(trackedKeyCodes.size());
    for (const int& code : trackedKeyCodes)
    {
        codeArguments.push_back(std::to_string(code));
    }
    startDaemon(codeArguments, this);
    DBG_V(messagePrefix << __func__ << ": Launching daemon with "
            << codeArguments.size() << " tracked code arguments.");
    keyCodes = trackedKeyCodes;
}


// Receives keyboard event data from the daemon output pipe, and passes it to
// the handleKeyEvent method if its data is valid.
void KeyDaemon::Controller::processData
(const unsigned char* data, const size_t size)
{
    if (size != sizeof(KeyMessage))
    {
        DBG(messagePrefix << __func__ << ": Received illegal message size "
                << size << " from KeyDaemon, expected " << sizeof(KeyMessage));
        return;
    }
    DBG_V(messagePrefix << __func__
            << ": Received key message with expected size " << size);
    // Cast to int pointer first to validate event codes. This assertion
    // shouldn't ever fail, unless the compiler is doing something truly
    // strange.
    ASSERT(sizeof(KeyMessage) == 2 * sizeof(int));
    const int* numericData = reinterpret_cast<const int*>(data);
    const int keyCode = numericData[0];
    const int eventType = numericData[1];
    if (std::find(keyCodes.begin(), keyCodes.end(), keyCode) == keyCodes.end())
    {
        DBG(messagePrefix << __func__ << ": Received illegal key code "
                << keyCode << " from KeyDaemon.");
        return;
    }
    if (eventType < 0 || eventType >= 
            static_cast<int>(EventType::trackedTypeCount))
    {
        DBG(messagePrefix << __func__ 
                << ": Received illegal event type value " << eventType
                << " from KeyDaemon.");
        return;
    }
    const KeyMessage message = { keyCode, static_cast<EventType>(eventType) };
    handleKeyEvent(message);
}
