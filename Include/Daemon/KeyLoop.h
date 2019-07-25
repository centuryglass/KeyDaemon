/**
 * @file  KeyLoop.h
 *
 * @brief  Implements the KeyDaemon's main action loop.
 */

#pragma once
#include "DaemonLoop.h"
#include "KeyReader.h"
#include <vector>

namespace KeyDaemon
{
    class KeyLoop;
}

class KeyDaemon::KeyLoop : public DaemonFramework::DaemonLoop,
    public KeyReader::Listener
{
public:
    /**
     * @brief  Saves the list of tracked key codes on construction.
     *
     * @param keyCodes  Linux keyboard input codes the KeyDaemon should monitor.
     */
    KeyLoop(std::vector<int> keyCodes);

    /**
     * @brief  Ensures all key event file readers are closed and deleted on
     *         destruction.
     */
    virtual ~KeyLoop();

private:
    /**
     * @brief  Creates KeyReader objects for all keyboard event files before
     *         starting the daemon action loop.
     *
     * @return  Zero if keyboard event files were successfully located, 
     *          (int) KeyExitCode::missingKeyEventFiles if no event files were
     *          found.
     */
    virtual int initLoop() override;

    /**
     * @brief  Polls the list of KeyReaders, deleting any that have encountered
     *         errors, then briefly sleeps.
     *
     * @return  Zero if KeyReaders are still open, (int)
     *          KeyExitCode::keyReadersStopped if all readers have been removed.
     */
    virtual int loopAction() override;

    /**
     * @brief  Sends all tracked key events to the parent application.
     *
     * @param keyCode  The code value of a tracked key that was pressed.
     *
     * @param type     The type of key event that was detected.
     */
    virtual void keyEvent(const int keyCode, const EventType type) override;

    // All key codes tracked by the daemon:
    std::vector<int> keyCodes;
    // Holds KeyReaders for each keyboard event file:
    std::vector<KeyReader*> eventFileReaders;

};
