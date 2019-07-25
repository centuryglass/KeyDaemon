"""
Provides constants and functions used when building DaemonFramework applications
with the 'make' command.
"""
import os
import subprocess
import tempfile
import time
from supportModules import pathConstants
paths = pathConstants.paths

"""Defines all relevant makefile variable names."""
class VarNames:
    def __init__(self):
        self._daemon      = 'KD_TARGET_APP'
        self._daemonDir   = 'KD_INSTALL_DIR'
        self._daemonPath  = 'KD_DAEMON_PATH'
        self._parentPath  = 'KD_PARENT_PATH'
        self._pipePath    = 'KD_PIPE_PATH'
        self._lockPath    = 'KD_LOCK_PATH'
        self._keyLimit    = 'KD_KEY_LIMIT'
        self._configMode  = 'KD_CONFIG'
        self._verbose     = 'KD_VERBOSE'
        self._timeout     = 'DF_TIMEOUT'
    """Return the daemon executable variable name."""
    @property
    def daemon(self):
        return self._daemon
    """Return the daemon installation directory variable name."""
    @property
    def daemonDir(self):
        return self._daemonDir
    """Return the daemon installation path variable name."""
    @property
    def daemonPath(self):
        return self._daemonPath
    """
    Return the daemon parent process path variable name.
    If defined, the daemon will only run if launched by an executable stored at
    this path.
    """
    @property
    def parentPath(self):
        return self._parentPath
    """Return the daemon output pipe file path variable name."""
    @property
    def pipePath(self):
        return self._pipePath
    """Return the daemon instance lock file path variable name."""
    @property
    def lockPath(self):
        return self._lockPath
    """Return the daemon tracked key limit variable name."""
    @property
    def keyLimit(self):
        return self._keyLimit
    """Return the Debug/Release mode build variable name."""
    @property
    def configMode(self):
        return self._configMode
    """Return the daemon verbose output build variable name."""
    @property
    def verbose(self):
        return self._verbose
    """Return the daemon's inherited timeout build variable name."""
    @property
    def timeout(self):
        return self._timeout
varNames = VarNames()

"""
Gets a complete list of build arguments to pass to the 'make' command.

All parameters have default test values provided. If all defaults are used, the
daemon should always build, install, and run correctly.
    
Keyword Arguments:
daemon     -- The name of the daemon executable.
              (default: paths.daemon)
daemonDir  -- The directory where the daemon will be installed.
              (default: paths.secureExeDir)
parentPath -- The required path to the daemon's parent application.
              (default: paths.parentSecureExePath)
pipePath   -- The daemon's output pipe path.
              (default: paths.pipePath)
lockPath   -- The daemon's instance lock path.
              (default: paths.lockPath)
keyLimit   -- The maximum number of key codes the daemon will track.
              (default: 239)
testArgs   -- A testArgs.Values object. (default: None)
              If not None, this object's properties override all parameters
              listed below.
debugBuild -- Whether the daemon builds in debug mode instead of release.
              (default: True)
verbose    -- Whether the daemon prints verbose messages.
              (default: True)
timeout    -- Seconds before the daemon exits, or False to disable timeout.
              (default: 1)
"""
def getBuildArgs(daemon = paths.daemon, \
                 daemonDir = paths.secureExeDir, \
                 parentPath = paths.parentSecureExePath, \
                 pipePath = paths.pipePath, \
                 lockPath = paths.lockPath, \
                 keyLimit = 239, \
                 testArgs = None, \
                 debugBuild = True, \
                 verbose = True,
                 timeout = 1):
    if testArgs is not None:
        debugBuild = testArgs.debugBuild
        verbose = testArgs.useVerbose
        if testArgs.timeout is not None:
            timeout = testArgs.timeout
    argList = [varNames.configMode + '=' + ('Debug' if debugBuild \
                                            else 'Release')]
    args = [(daemon, varNames.daemon), \
            (daemonDir, varNames.daemonDir), \
            (parentPath, varNames.parentPath), \
            (pipePath, varNames.pipePath), \
            (lockPath, varNames.lockPath), \
            (keyLimit, varNames.keyLimit), \
            (timeout,  varNames.timeout)]
    for value, varName in args:
        if value is not None:
            argList.append(varName + '=' + str(value))
    argList.append(varNames.verbose + '=' + ('1' if verbose else '0'))
    argList.append(varNames.daemonPath + '=' + paths.daemonSecureExePath)
    return argList

"""
Writes a set of build arguments to a log file.
Keyword Arguments:
makeArgs -- A list of build arguments.
logFile  -- The file where the build arguments will be logged.
title    -- A title to print before the argument list.
indent   -- The number of spaces to indent each logged line. (default: 2)
"""
def logBuildArgs(makeArgs, logFile, title, indent = 2):
    if title is not None:
        logFile.write((' ' * indent) + title + '\n')
    for arg in makeArgs:
        logFile.write((' ' * indent) + '-' + arg + '\n')


"""
Given a build argument set, return the value of a specific makefile variable.
Keyword Arguments:
makeArgs -- A set of makefile arguments that may affect the install path.

varName  -- The name of the variable to check.

makePath -- The path to the makefile where the variable should be checked.
            (default: 'Makefile')
"""
def readMakeVar(makeArgs, varName, makePath = 'Makefile'):
    with tempfile.TemporaryFile() as tempOutputFile:
        subprocess.run(['make', '-f', makePath, 'print-' + varName] + makeArgs,\
                         stdout = tempOutputFile)
        tempOutputFile.seek(0)
        return tempOutputFile.read().decode('utf-8').rstrip()

"""
Attempts to build a target, returning whether the build succeeded.

Keyword Arguments:
makeDir     -- The directory where the target's makefile is found

targetPath  -- The path where the target will be found after compilation.

makeArgs    -- The set of command line arguments to pass to the `make` process.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def buildTarget(makeDir, targetPath, makeArgs, outFile = subprocess.DEVNULL):
    print('Building target at "' + targetPath + '":')
    os.chdir(makeDir)
    if os.path.isfile(targetPath):
        os.remove(targetPath)
    print('Build process started:')
    subprocess.call(['make'] + makeArgs, stdout = outFile, stderr = outFile)
    print('Build process complete:')
    if not os.path.isfile(targetPath):
        print('Failed to build target at "' + targetPath + '"')
        return False
    print('Built target at "' + targetPath + '"')
    return True

"""
Attempts to install a target, returning whether the installation succeeded.

Keyword Arguments:
makeDir     -- The directory where the target's makefile is found

makeArgs    -- The set of command line arguments to pass to the `make` process.

installVar  -- The makefile variable name that sets the installation path.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def installTarget(makeDir, makeArgs, installVar, outFile = subprocess.DEVNULL):
    os.chdir(makeDir)
    targetPath = readMakeVar(makeArgs, installVar)
    preBuildTime = time.time()
    subprocess.call(['make'] + makeArgs, stdout = outFile, stderr = outFile)
    subprocess.call(['make', 'install'] + makeArgs, stdout = outFile, \
                    stderr = outFile)
    if not os.path.isfile(targetPath):
        outFile.write('make.installTarget: No file installed to path "' \
                      + targetPath + '"')
        return False
    if preBuildTime >= os.path.getmtime(targetPath):
        outFile.write('make.installTarget: File installed to path "' \
                      + targetPath + '" was not updated.')
        return False
    return True


"""
Deletes a target's build files.
Keyword Arguments:
makeDir     -- The directory where the target's makefile is found.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def cleanTarget(makeDir, pathVarName, outFile = subprocess.DEVNULL):
    os.chdir(makeDir)
    subprocess.call(['make', 'clean', 'CONFIG=Debug'], \
                    stdout = outFile, \
                    stderr = outFile)
    subprocess.call(['make', 'clean', 'CONFIG=Release'], \
                    stdout = outFile, \
                    stderr = outFile)

"""
Uninstalls a target.
Keyword Arguments:
makeDir     -- The directory where the target's makefile is found.

pathVarName  -- The makefile variable name that sets the target install path.

execPath     -- The path to the target's installed executable.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def uninstallTarget(makeDir, pathVarName, execPath, \
                    outFile = subprocess.DEVNULL):
    os.chdir(makeDir)
    subprocess.call(['make', 'uninstall', pathVarName + '=' + execPath], \
                    stdout = outFile, \
                    stderr = outFile)

"""
Attempts to build the KeyDaemon, returning whether the build succeeded.

Keyword Arguments:
makeArgs    -- The set of command line arguments to pass to the `make` process.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def buildKeyDaemon(makeArgs, outFile = subprocess.DEVNULL):
    return buildTarget(paths.testDaemonDir, paths.daemonBuildPath, makeArgs, \
                       outFile)

"""
Attempts to install the TestDaemon, returning whether installation succeeded.

Keyword Arguments:
makeArgs    -- The set of command line arguments to pass to the `make` process.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def installTestDaemon(makeArgs, outFile = subprocess.DEVNULL):
    return installTarget(paths.testDaemonDir, makeArgs, varNames.daemonPath, \
                         outFile)

"""
Deletes the daemon's build files.
Keyword Arguments:

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def cleanDaemon(outFile = subprocess.DEVNULL):
    cleanTarget(paths.testDaemonDir, outFile)

"""
Uninstalls the daemon.
Keyword Arguments:
daemonPath  -- The path to the installed daemon executable to remove.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def uninstallDaemon(daemonPath, outFile = subprocess.DEVNULL):
    uninstallTarget(paths.testDaemonDir, varNames.daemonPath, daemonPath, \
                    outFile)

"""
Attempts to build the TestParent, returning whether the build succeeded.

Keyword Arguments:
makeArgs    -- The set of command line arguments to pass to the `make` process.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def buildTestParent(makeArgs, outFile = subprocess.DEVNULL):
    return buildTarget(paths.basicParentDir, paths.parentBuildPath, makeArgs, \
           outFile)

"""
Attempts to install the TestParent, returning whether installation succeeded.

Keyword Arguments:
makeArgs    -- The set of command line arguments to pass to the `make` process.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def installTestParent(makeArgs, outFile = subprocess.DEVNULL):
    return installTarget(paths.basicParentDir, makeArgs, varNames.parentPath, \
                         outFile)

"""
Deletes the parent's build files.
Keyword Arguments:
outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def cleanParent(outFile = subprocess.DEVNULL):
    cleanTarget(paths.basicParentDir, outFile)

"""
Uninstalls the parent.
Keyword Arguments:
parentPath  -- The path to the installed parent executable to remove.

outFile     -- A file where test output from stdout and stderr will be sent.
               The default subprocess.DEVNULL value discards all output.
"""
def uninstallParent(parentPath, outFile = subprocess.DEVNULL):
    uninstallTarget(paths.basicParentDir, varNames.parentPath, parentPath, \
                    outFile)
