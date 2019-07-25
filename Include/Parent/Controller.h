/**
 * @file Controller.h
 *
 * @brief  An abstract class used to launch and control a KeyDaemon instance.
 */

#pragma once
#include "DaemonControl.h"
#include "Pipe_Listener.h"
#include "KeyMessage.h"
#include "EventType.h"

namespace KeyDaemon
{
    class Controller;
}

class KeyDaemon::Controller : protected DaemonFramework::DaemonControl, 
        public DaemonFramework::Pipe::Listener
{
public:
    /**
     * @brief  Configures the daemon output pipe on construction.
     */
    Controller();

    virtual ~Controller() { }

    /**
     * @brief  Launches the KeyDaemon if it isn't already running.
     *
     * @param trackedKeyCodes  The list of key codes the KeyDaemon should track.
     */
    void startKeyDaemon(const std::vector<int> trackedKeyCodes);


    // Grant limited access to DaemonControl public methods:
    using DaemonFramework::DaemonControl::stopDaemon;
    using DaemonFramework::DaemonControl::isDaemonRunning;
    using DaemonFramework::DaemonControl::getDaemonProcessID;
    using DaemonFramework::DaemonControl::getExitCode;

private:
    /**
     * @brief  The virual method called to handle all key events sent by the 
     *         KeyDaemon.
     *
     * @param keyMessage  The incoming key event message data.
     */
    virtual void handleKeyEvent(const KeyMessage& keyMessage) = 0;

    /**
     * @brief  Receives keyboard event data from the daemon output pipe, and 
     *         passes it to the handleKeyEvent method if its data is valid.
     *
     * @param data  A raw data array pointing to KeyMessage data.
     *
     * @param size  Size in bytes of the data array. If this does not match the
     *              size of the KeyMessage data structure, the message will be
     *              discarded.
     */
    virtual void processData
    (const unsigned char* data, const size_t size) final override;

    // The last set of tracked key codes used to launch the daemon.
    std::vector<int> keyCodes;
};
