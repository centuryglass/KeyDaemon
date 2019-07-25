### KeyDaemon Test Daemon Makefile ###
# Define daemon paths and file names:
TEST_DAEMON_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TEST_DIR:=$(shell dirname $(realpath $(TEST_DAEMON_DIR)))
PROJECT_DIR:=$(shell dirname $(realpath $(TEST_DIR)))
EXEC_DIR:=$(TEST_DIR)/exec
BUILD_DIR:=$(TEST_DIR)/build
PARENT_APP=TestParent
PIPE_FILE=.keyPipe
LOCK_FILE=.keyLock
KD_CONFIG=Debug
KD_VERBOSE?=1

# Define variables required by the main KeyDaemon makefile:
KD_TARGET_APP?=keyd
KD_INSTALL_DIR?=$(EXEC_DIR)/secured
KD_BUILD_DIR?=$(BUILD_DIR)
KD_PARENT_PATH?=$(KD_INSTALL_DIR)/$(PARENT_APP)
KD_PIPE_PATH?=$(EXEC_DIR)/$(PIPE_FILE)
KD_LOCK_PATH?=$(EXEC_DIR)/$(LOCK_FILE)
KD_KEY_LIMIT?=10

# Include main KeyDaemon makefile:
include $(PROJECT_DIR)/Makefile

fake: keyd
