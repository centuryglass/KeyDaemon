define KD_HELPTEXT
## KeyDaemon Makefile ##
# This makefile builds the KeyDaemon, a helper program designed to listen for
# specific keyboard input events for a specific parent application.
#
## Quick Guide: ##
# 1. Ensure the following variables are defined when building the daemon:
#    - KD_TARGET_APP
#    - KD_INSTALL_DIR
#    - KD_PARENT_PATH
#    - KD_PIPE_PATH
#    - KD_LOCK_PATH
#    - KD_KEY_LIMIT
#
# 2. Optionally, provide valid definitions for these additional variables to
#    enable features or override default values:
#    - KD_CONFIG
#    - KD_VERBOSE
#    - KD_STRIP
#    - KD_TARGET_ARCH
#    - KD_BUILD_DIR
#
endef
export HELPTEXT


###################### Initialize project variables: ##########################
# Build type: either Debug or Release
KD_CONFIG?=Release
# Enable or disable verbose output
KD_VERBOSE?=0
V_AT:=$(shell if [ $(KD_VERBOSE) != 1 ]; then echo '@'; fi)

# Project directories:
PROJECT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
SOURCE_DIR:=$(PROJECT_DIR)/Source
INCLUDE_DIR:=$(PROJECT_DIR)/Include
KD_BUILD_DIR?=$(PROJECT_DIR)/build/$(KD_CONFIG)
OBJDIR:=$(KD_BUILD_DIR)/Intermediate
DAEMON_FRAMEWORK_DIR:=$(PROJECT_DIR)/deps/DaemonFramework

# Target paths:
KD_TARGET_PATH:=$(KD_BUILD_DIR)/$(KD_TARGET_APP)
KD_INSTALL_PATH:=$(KD_INSTALL_DIR)/$(KD_TARGET_APP)

# Command used to strip unneeded symbols from object files:
KD_STRIP?=strip

# Use the build system's architecture by default.
KD_TARGET_ARCH?=-march=native

# Command used to clean out build files:
CLEANCMD = rm -rf $(KD_TARGET_PATH) $(OBJDIR)

######################### Primary Build Target: ###############################
$(KD_TARGET_PATH): build
	@echo Linking "$(KD_TARGET_APP):"
	@if [ "$(KD_VERBOSE)" == "1" ]; then \
        $(DAEMON_FRAMEWORK_DIR)/cleanPrint.sh '$(CXX) $(LINK_ARGS)'; \
        echo ''; \
    fi
	@$(CXX) $(LINK_ARGS)

################ Configure and include framework makefile: ####################
DF_OBJDIR:=$(OBJDIR)
DF_CONFIG:=$(KD_CONFIG)
DF_VERBOSE:=$(KD_VERBOSE)
DF_DAEMON_PATH:=$(KD_INSTALL_PATH)
DF_REQUIRED_PARENT_PATH:=$(KD_PARENT_PATH)
DF_OUTPUT_PIPE_PATH:=$(KD_PIPE_PATH)
DF_LOCK_FILE_PATH:=$(KD_LOCK_PATH)

include $(DAEMON_FRAMEWORK_DIR)/Daemon.mk

############################### Set build flags: ##############################
#### Config-specific flags: ####
ifeq ($(KD_CONFIG),Debug)
    OPTIMIZATION?=1
    GDB_SUPPORT?=0
    # Debug-specific preprocessor definitions:
    CONFIG_FLAGS=-DKD_DEBUG=1
endif

ifeq ($(KD_CONFIG),Release)
    OPTIMIZATION?=1
    GDB_SUPPORT?=0
endif

# Set optimization level flags:
ifeq ($(OPTIMIZATION),1)
    CONFIG_CFLAGS=-O3 -flto
    CONFIG_LDFLAGS:=-flto
else
    CONFIG_CFLAGS=-O0
endif

# Set debugger flags:
ifeq ($(GDB_SUPPORT),1)
    CONFIG_CFLAGS:=$(CONFIG_CFLAGS) -g -ggdb
else
    CONFIG_LDFLAGS:=$(CONFIG_LDFLAGS) -fvisibility=hidden
endif

#### C compilation flags: ####
CFLAGS:=$(TARGT_ARCH) $(CONFIG_CFLAGS) $(CFLAGS)

#### C++ compilation flags: ####
CXXFLAGS:=-std=gnu++14 $(CXXFLAGS)

#### C Preprocessor flags: ####

# Include directories:
INCLUDE_FLAGS:=$(call recursiveInclude,$(INCLUDE_DIR)) \
               $(DF_INCLUDE_FLAGS)

# Disable dependency generation if multiple architectures are set
DEPFLAGS:=$(if $(word 2, $(KD_TARGET_ARCH)), , -MMD)

DEFINE_FLAGS:=$(call addDef,KD_KEY_LIMIT) \
              $(call addDef,KD_VERBOSE) \
              $(DF_DEFINE_FLAGS)

CPPFLAGS:=-pthread \
          $(DEPFLAGS) \
          $(CONFIG_FLAGS) \
          $(DEFINE_FLAGS) \
          $(INCLUDE_FLAGS) \
          $(CPPFLAGS)

#### Linker flags: ####
LDFLAGS:=-lpthread $(KD_TARGET_ARCH) $(CONFIG_LDFLAGS) $(LDFLAGS)

#### Aggregated build arguments: ####

OBJECTS:=$(OBJDIR)/Main.o \
         $(OBJDIR)/KeyLoop.o \
         $(OBJDIR)/KeyCode.o \
         $(OBJDIR)/EventType.o \
         $(OBJDIR)/KeyEventFiles.o \
         $(OBJDIR)/KeyReader.o \
         $(OBJECTS)

# Complete set of flags used to compile source files:
BUILD_FLAGS:=$(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)

# Complete set of arguments used to link the program:
LINK_ARGS:=-o $(KD_TARGET_PATH) $(OBJECTS) $(DF_OBJECTS_DAEMON) $(LDFLAGS)

###################### Supporting Build Targets: ##############################
.PHONY: build check_defs install clean strip uninstall help

$(OBJECTS) :
	@echo "Compiling $(<F):"
	$(V_AT)mkdir -p $(OBJDIR)
	@if [ "$(KD_VERBOSE)" == "1" ]; then \
        $(DAEMON_FRAMEWORK_DIR)/cleanPrint.sh '$(CXX) $(BUILD_FLAGS)'; \
        echo '    -o "$@" \'; \
        echo '    -c "$<"'; \
        echo ''; \
	fi
	@$(CXX) $(BUILD_FLAGS) -o "$@" -c "$<"

build : check_defs df-daemon $(OBJECTS)

check_defs:
	@if [ -z "$(KD_TARGET_APP)" ]; then \
        echo >&2 "Action failed, KD_TARGET_APP not defined."; exit 1; \
	elif [ -z "$(KD_INSTALL_DIR)" ]; then \
        echo >&2 "Action failed, KD_INSTALL_DIR not defined."; exit 1; \
	elif [ -z "$(KD_PARENT_PATH)" ]; then \
        echo >&2 "Action failed, KD_PARENT_PATH not defined."; exit 1; \
	elif [ -z "$(KD_PIPE_PATH)" ]; then \
        echo >&2 "Action failed, KD_PIPE_PATH not defined."; exit 1; \
	elif [ -z "$(KD_LOCK_PATH)" ]; then \
        echo >&2 "Action failed, KD_LOCK_PATH not defined."; exit 1; \
	elif [ -z "$(KD_KEY_LIMIT)" ]; then \
        echo >&2 "Action failed, KD_KEY_LIMIT count not defined."; exit 1; \
	fi

install: check_defs
	$(V_AT)sudo mkdir -p $(KD_INSTALL_DIR); \
	sudo cp $(KD_TARGET_PATH) $(KD_INSTALL_PATH); \
    sudo setcap -q cap_dac_override=ep $(KD_INSTALL_PATH);

clean: check_defs
	@echo "Cleaning $(KD_TARGET_APP)"
	$(V_AT)$(CLEANCMD)

strip: check_defs
	@echo "Stripping $(KD_TARGET_APP)"
	$(V_AT)$(KD_STRIP) --strip-unneeded $(KD_INSTALL_PATH)

uninstall: check_defs
	@echo echo "Uninstalling $(KD_TARGET_APP)"
	$(V_AT)if [ -f "$(KD_INSTALL_PATH)" ]; then \
	    sudo rm "$(KD_INSTALL_PATH)"; \
    fi

help:
	@echo "$$HELPTEXT"

-include $(OBJECTS:%.o=%.d)

$(OBJDIR)/Main.o: \
	$(SOURCE_DIR)/Main.cpp
$(OBJDIR)/KeyLoop.o: \
	$(SOURCE_DIR)/KeyLoop.cpp
$(OBJDIR)/KeyCode.o: \
	$(SOURCE_DIR)/KeyCode.cpp
$(OBJDIR)/EventType.o: \
	$(SOURCE_DIR)/EventType.cpp
$(OBJDIR)/KeyEventFiles.o: \
	$(SOURCE_DIR)/EventFiles.cpp
$(OBJDIR)/KeyReader.o: \
	$(SOURCE_DIR)/KeyReader.cpp
