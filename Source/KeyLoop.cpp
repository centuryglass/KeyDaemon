#include "KeyLoop.h"
#include "KeyExitCode.h"
#include "EventFiles.h"
#include "KeyMessage.h"
#include "KDDebug.h"
#include <ctime>

#ifdef KD_DEBUG
static const constexpr char* messagePrefix = "KeyDaemon::KeyLoop::";
#endif

// Frequency in milliseconds between key reader checks.
static const constexpr int readerCheckFrequencyMS = 100;


// Saves the list of tracked key codes on construction.
KeyDaemon::KeyLoop::KeyLoop(std::vector<int> keyCodes) :
    keyCodes(keyCodes) { }


// Ensures all key event file readers are closed and deleted on destruction.
KeyDaemon::KeyLoop::~KeyLoop()
{
    DBG_V(messagePrefix << __func__ << ": Closing "
            << eventFileReaders.size() << " KeyReader objects:");
    for (KeyReader* reader : eventFileReaders)
    {
        reader->stopReading();
        delete reader;
    }
    eventFileReaders.clear();
    DBG_V(messagePrefix << __func__ << ": KeyLoop destroyed.");
}


// Creates KeyReader objects for all keyboard event files before starting the
// daemon action loop.
int KeyDaemon::KeyLoop::initLoop()
{
    // Create KeyReader objects for each keyboard event file:
    std::vector<std::string> eventFilePaths = EventFiles::getPaths();
    DBG_V(messagePrefix << "Creating KeyReader objects for "
            << eventFilePaths.size() << " event files:");
    for (const std::string& path : eventFilePaths)
    {
        eventFileReaders.push_back(
                new KeyReader(path.c_str(), keyCodes, this));
    }
    if (eventFileReaders.empty())
    {
        DBG(messagePrefix << __func__ 
                << ": Exiting: no valid event files found.");
        return static_cast<int>(KeyExitCode::missingKeyEventFiles);
    }
    return 0;
}


// Polls the list of KeyReaders, deleting any that have encountered errors, then
// briefly sleeps.
int KeyDaemon::KeyLoop::loopAction()
{
    static bool firstLoop = true;
    if (firstLoop)
    {
        DBG("KeyLoop first loop");
        firstLoop = false;
    }
    if (eventFileReaders.empty())
    {
        DBG(messagePrefix << __func__
                << ": No readers left, closing daemon.");
        return static_cast<int>(KeyExitCode::keyReadersStopped);
    }
    // Find and remove failed file readers:
    for (int i = 0; i < eventFileReaders.size(); i++)
    {
        using State = DaemonFramework::InputReader::State;
        State readerState = eventFileReaders[i]->getState();
        if (readerState == State::closed || readerState == State::failed)
        {
            DBG(messagePrefix << "Reader for path \""
                    << eventFileReaders[i]->getPath()
                    << "\" stopped unexpectedly, "
                    << (eventFileReaders.size() - 1)
                    << " readers remaining.");
            KeyReader* removedReader = eventFileReaders[i];
            eventFileReaders.erase(eventFileReaders.begin() + i);
            delete removedReader;
            i--;
        }
    }
    struct timespec sleepTimer;
    sleepTimer.tv_sec = 0;
    sleepTimer.tv_nsec = readerCheckFrequencyMS * 1000000;
    nanosleep(&sleepTimer, nullptr);
    return 0;
}


// Sends all tracked key events to the parent application.
void KeyDaemon::KeyLoop::keyEvent(const int keyCode, const EventType type)
{
    const KeyMessage newEvent = { keyCode, type };
    messageParent((const unsigned char*) &newEvent, sizeof(KeyMessage));
}
