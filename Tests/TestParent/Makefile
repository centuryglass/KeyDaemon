### KeyDaemon Test Parent Makefile ###
# The daemon parent program's executable name:
TARGET_APP=TestParent

###################### Primary Build Target: ##################################
$(TARGET_APP) : buildParent
	@echo Linking "$(TARGET_APP):"
	@if [ "$(VERBOSE)" == "1" ]; then \
        $(DAEMON_FRAMEWORK_DIR)/cleanPrint.sh '$(CXX) $(LINK_ARGS)'; \
        echo ''; \
    fi
	@$(CXX) $(LINK_ARGS)

######################## Initialize build variables: ##########################
# Set Debug or Release mode:
CONFIG?=Debug
# enable or disable verbose output:
VERBOSE?=1
V_AT:=$(shell if [ $(VERBOSE) != 1 ]; then echo '@'; fi)

# Select specific build architectures:
TARGET_ARCH?=-march=native

# Define daemon paths and file names:
TEST_PARENT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TEST_DIR:=$(shell dirname $(realpath $(TEST_PARENT_DIR)))
PROJECT_DIR:=$(shell dirname $(realpath $(TEST_DIR)))
EXEC_DIR:=$(TEST_DIR)/exec
BUILD_DIR:=$(TEST_DIR)/build
INSTALL_DIR:=$(EXEC_DIR)/secured
OBJDIR:=$(BUILD_DIR)/Intermediate
PARENT_PATH:=$(INSTALL_DIR)/$(TARGET_APP)
DAEMON_PATH:=$(INSTALL_DIR)/keyd
PIPE_PATH=$(EXEC_DIR)/.keyPipe
TARGET_BUILD_PATH:=$(BUILD_DIR)/$(TARGET_APP)

############# Configure and include KeyDaemon parent makefile: ################
# Define variables required by the main KeyDaemon parent makefile:
KD_OBJDIR:=$(OBJDIR)
KD_PARENT_PATH:=$(PARENT_PATH)
KD_DAEMON_PATH:=$(DAEMON_PATH)
KD_PIPE_PATH:=$(PIPE_PATH)
KD_KEY_LIMIT?=10
KD_CONFIG?=$(CONFIG)
KD_VERBOSE?=$(VERBOSE)

include $(PROJECT_DIR)/Parent.mk

############################### Set build flags: ##############################
#### Config-specific flags: ####
ifeq ($(CONFIG),Debug)
    OPTIMIZATION?=0
    GDB_SUPPORT?=1
    # Debug-specific preprocessor definitions:
    CONFIG_FLAGS=-DDEBUG=1
endif

ifeq ($(CONFIG),Release)
    OPTIMIZATION?=1
    GDB_SUPPORT?=0
endif

# Set optimization level flags:
ifeq ($(OPTIMIZATION),1)
    CONFIG_CFLAGS:=-O3 -flto
    CONFIG_LDFLAGS:=-flto
else
    CONFIG_CFLAGS:=-O0
endif

# Set debugging flags:
ifeq ($(GDB_SUPPORT),1)
    CONFIG_CFLAGS:=$(CONFIG_CFLAGS) -g -ggdb
else
    CONFIG_LDFLAGS:=$(CONFIG_LDFLAGS) -fvisibility=hidden
endif

#### C compilation flags: ####
CFLAGS:=$(TARGET_ARCH) $(CONFIG_CFLAGS) $(CFLAGS)

#### C++ compilation flags: ####
CXXFLAGS:=-std=gnu++14 $(CXXFLAGS)

#### C Preprocessor flags: ####

# C preprocessor definitions:

# Disable dependency generation if multiple architectures are set
DEPFLAGS:=$(if $(word 2, $(TARGET_ARCH)), , -MMD)

INCLUDE_FLAGS:="-I$(PROJECT_DIR)/Include/Daemon" $(KD_INCLUDE_FLAGS)

CPPFLAGS:=-pthread \
          $(DEPFLAGS) \
          $(CONFIG_FLAGS) \
          $(KD_DEFINE_FLAGS) \
          $(INCLUDE_FLAGS) \
          $(CPPFLAGS)

#### Linker flags: ####
LDFLAGS:=-lpthread $(TARGET_ARCH) $(CONFIG_LDFLAGS) $(LDFLAGS)

#### Aggregated build arguments: ####

OBJECTS_PARENT:=$(OBJDIR)/TestParent.o $(OBJDIR)/KeyCode.o

# Complete set of flags used to compile source files:
BUILD_FLAGS:=$(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)

# Complete set of arguments used to link the program:
LINK_ARGS:= -o $(TARGET_BUILD_PATH) $(OBJECTS_PARENT) $(KD_OBJECTS_PARENT) \
               $(LDFLAGS)

###################### Supporting Build Targets: ##############################
.PHONY: install uninstall clean buildParent

install:
	$(V_AT)sudo mkdir -p $(INSTALL_DIR); \
	sudo cp $(TARGET_BUILD_PATH) $(PARENT_PATH);

uninstall:
	@echo Uninstalling "$(TARGET_APP)"
	$(V_AT)sudo rm -f $(PARENT_PATH)

clean:
	@echo Cleaning "$(TARGET_APP)"
	$(V_AT)if [ -d $(OBJDIR) ]; then \
	    rm -rf $(OBJDIR); \
    fi; \
    if [ -f $(TARGET_BUILD_PATH) ]; then \
	    rm $(TARGET_BUILD_PATH); \
    fi

$(KD_OBJDIR)/TestParent.o: $(TEST_PARENT_DIR)/TestParent.cpp
$(KD_OBJDIR)/KeyCode.o: $(KD_PROJECT_DIR)/Source/KeyCode.cpp

$(OBJECTS_PARENT) :
	@echo "Compiling: $(<F):"
	$(V_AT)mkdir -p $(OBJDIR)
	@if [ "$(VERBOSE)" == "1" ]; then \
        $(DAEMON_FRAMEWORK_DIR)/cleanPrint.sh '$(CXX) $(BUILD_FLAGS)'; \
        echo '    -o "$@" \'; \
        echo '    -c "$<"'; \
        echo ''; \
	fi
	@$(CXX) $(BUILD_FLAGS) -o "$@" -c "$<"

buildParent : kd-parent $(OBJECTS_PARENT)

## Enable dependency generation: ##
-include $(OBJECTS_PARENT:%.o=%.d)
