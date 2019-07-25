"""pathConstants defines paths to KeyDaemon files and directories."""
import os

"""Defines path and filename constants used in testing."""
class TestPaths:
    def __init__(self):
        self._daemon      = 'keyd'
        self._parent      = 'TestParent'
        self._tempLog     = 'tempLog.txt'
        self._failureLog  = 'failureLog.txt'
        self._pipeFile    = '.keyPipe'
        self._lockFile    = '.keyLock'
        self._moduleDir   = os.path.dirname(os.path.realpath(__file__))
        self._testDir     = os.path.normpath(os.path.join(self._moduleDir, \
                                                          os.pardir))
        self._projectDir = os.path.normpath(os.path.join(self._testDir, \
                                                         os.pardir))
        self._includeDir     = os.path.join(self.projectDir, 'Include')
        self._testExecDir    = os.path.join(self.testDir, 'exec')
        self._secureExeDir   = os.path.join(self.testExecDir, 'secured')
        self._buildDir  = os.path.join(self._testDir, 'build')
        self._testDaemonDir = os.path.join(self._testDir, 'TestDaemon')
        self._testParentDir = os.path.join(self._testDir, 'TestParent')

    # Directory paths:
    """Return the path to the main project directory. """
    @property
    def projectDir(self):
        return self._projectDir
    """Return the path to the project's main test directory."""
    @property
    def testDir(self):
        return self._testDir
    """Return the path to the project's build directory."""
    @property
    def buildDir(self):
        return self._buildDir
    """Return the path to the daemon framework #include directory."""
    @property
    def includeDir(self):
        return self._includeDir
    """Return the path to the main test executable directory."""
    @property
    def testExecDir(self):
        return self._testExecDir
    """Return the path to the secured executable test directory."""
    @property
    def secureExeDir(self):
        return self._secureExeDir
    """Return the path to the TestDaemon source directory."""
    @property
    def testDaemonDir(self):
        return self._testDaemonDir
    """Return the path to the TestParent source directory."""
    @property
    def basicParentDir(self):
        return self._testParentDir

    # File names:
    """Return the name of the test daemon application file."""
    @property
    def daemon(self):
        return self._daemon
    """Return the name of the parent app used to launch the daemon."""
    @property
    def parent(self):
        return self._parent
    """Return the name of the temporary test output log file."""
    @property
    def tempLog(self):
        return self._tempLog
    """Return the name of the test failure log file."""
    @property
    def failureLog(self):
        return self._failureLog
    """Return the name of the daemon's output pipe file."""
    @property
    def pipeFile(self):
        return self._pipeFile
    """Return the name of the daemon's lock file."""
    @property
    def lockFile(self):
        return self._lockFile

    # Daemon paths:
    """Return the path to the daemon's source directory."""
    @property
    def daemonSourceDir(self):
        return self.basicDaemonDir
    """Return the path where the daemon is found after compilation."""
    @property
    def daemonBuildPath(self):
        return os.path.join(self.buildDir, self.daemon)
    """Return the path to the daemon in the secured executable directory."""
    @property
    def daemonSecureExePath(self):
        return os.path.join(self.secureExeDir, self.daemon)

    # Parent application paths:
    """Return the path to the parent's source directory."""
    @property
    def parentSourceDir(self):
        return self.basicParentDir
    """Return the path where parent app is found after compilation."""
    @property
    def parentBuildPath(self):
        return os.path.join(self.buildDir, self.parent)
    """Return the path to the parent app in the secured executable directory."""
    @property
    def parentSecureExePath(self):
        return os.path.join(self.secureExeDir, self.parent)
    """Return the path where the daemon's output pipe file will be created."""
    @property
    def pipePath(self):
        return os.path.join(self.testExecDir, self.pipeFile)
    """Return the path where the daemon's instance lock file will be created."""
    @property
    def lockPath(self):
        return os.path.join(self.testExecDir, self.lockFile)
    """Return the path where temporary log files will be stored."""
    @property
    def tempLogPath(self):
        return os.path.join(self.testDir, self._tempLog)
    """Return the path where the failure log file will be saved."""
    @property
    def failureLogPath(self):
        return os.path.join(self.testDir, self._failureLog)
paths = TestPaths()
