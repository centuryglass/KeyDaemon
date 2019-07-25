"""Defines all possible types of KeyDaemon test result."""
from enum import Enum

"""
Represents a result encountered before the daemon could exit.
"""
class InitCode(Enum):
    daemonBuildFailure = 50
    daemonInstallFailure = 51
    daemonInitSuccess = 52,
    parentBuildFailure = 53
    parentInstallFailure = 54,
    parentInitSuccess = 55,
    parentRunFailure = 56,
    parentRunSuccess = 57,
    daemonRunSuccess = 58

"""
Represents an exit code returned by a daemon.

These codes are also defined in 
'deps/DaemonFramework/Include/Implementation/ExitCode.h', and in
'Include/KeyExitCode.h.
"""
class ExitCode(Enum):
    success = 0
    badDaemonPath = 1
    badParentPath = 2
    insecureDaemonDir = 3
    insecureParentDir = 4
    daemonAlreadyRunning = 5
    daemonParentEnded = 6
    fdCleanupFailed = 7
    daemonExecFailed = 8,
    badTrackedKeys = 9,
    missingKeyEventFiles = 10,
    keyReadersStopped = 11

"""
Return a string describing an ExitCode or InitCode.
Keyword Arguments:
resultCode -- An ExitCode or InitCode value.
"""
def resultCodeDescription(resultCode):
    titleDict = { \
            ExitCode.success : \
                    'KeyDaemon exited normally.',
            ExitCode.badDaemonPath: \
                    'KeyDaemon was not installed at the required path.',
            ExitCode.badParentPath: \
                    'TestParent was not installed at the required path.',
            ExitCode.insecureDaemonDir: \
                    'KeyDaemon was installed in an unsecured directory.',
            ExitCode.insecureParentDir: \
                    'TestParent was installed in an unsecured directory.',
            ExitCode.daemonAlreadyRunning: \
                    'KeyDaemon was running in multiple processes.',
            ExitCode.daemonParentEnded: \
                    'KeyDaemon exited because TestParent stopped running.',
            ExitCode.fdCleanupFailed: \
                    'Failed to clear open file table before launching daemon.',
            ExitCode.daemonExecFailed: \
                    'Failed to run KeyDaemon executable.',
            ExitCode.badTrackedKeys: \
                    'Invalid tracked key arguments provided.',
            ExitCode.missingKeyEventFiles: \
                    'No keyboard event files found.',
            ExitCode.keyReadersStopped: \
                    'Keyboard event file readers stopped unexpectedly.',
            InitCode.daemonBuildFailure: \
                    'Failed to build KeyDaemon program.',
            InitCode.daemonInstallFailure: \
                    'Failed to install KeyDaemon program.',
            InitCode.daemonInitSuccess: \
                    'Built and installed KeyDaemon program.',
            InitCode.parentBuildFailure: \
                    'Failed to build TestParent program.',
            InitCode.parentInstallFailure: \
                    'Failed to install TestParent program.',
            InitCode.parentInitSuccess: \
                    'Built and installed TestParent program.',
            InitCode.parentRunFailure: \
                    'Failed to run TestParent.',
            InitCode.parentRunSuccess: \
                    'Successfully started TestParent.',
            InitCode.daemonRunSuccess: \
                    'Successfully started KeyDaemon.'
    }
    if resultCode in titleDict:
        return titleDict[resultCode]
    return 'Unknown result code ' + str(resultCode)

"""Stores information about a specific test result."""
class Result:
    def __init__(self, resultCode, expectedCode):
        self._resultCode = resultCode
        self._expectedCode = expectedCode
    """Gets the test's result code."""
    def getResultCode(self):
        return self._resultCode
    """Gets the test's expected result code."""
    def getExpectedCode(self):
        return self_expectedCode
    """Checks if the test occurred as expected."""
    def testPassed(self):
        return self._resultCode == self._expectedCode
    """Gets a full description of the test's result."""
    def getResultText(self):
        return resultCodeDescription(self._resultCode)
    """Gets a full description of the test's expected result."""
    def getExpectedResultText(self):
        return resultCodeDescription(self._expectedCode)
