/**
 * @file  KeyExitCode.h
 *
 * @brief  Defines the list of errors a KeyDaemon may return. This is intended
 *         to extend the set of codes provided by DaemonFramework::ExitCode
 */

#pragma once

namespace KeyDaemon { enum class KeyExitCode; }

enum class KeyDaemon::KeyExitCode
{
    // Tracked key code parameters were invalid:
    badTrackedKeys = 9,
    // No valid keyboard event files were found:
    missingKeyEventFiles = 10,
    // Key event readers all stopped reading:
    keyReadersStopped = 11
};
