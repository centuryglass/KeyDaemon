define KD_HELPTEXT
## KeyDaemon Parent Makefile ##
# This makefile builds KeyDaemon source code files for a parent application that
# launches and listens to a KeyDaemon.
#
## Quick Guide: ##
# 1. Provide valid definitions for the following variables:
#    - KD_OBJDIR
#    - KD_PARENT_PATH
#    - KD_DAEMON_PATH
#    - KD_PIPE_PATH
#    - KD_KEY_LIMIT
#
# 2. Optionally, provide valid definitions for these additional variables to
#    enable features or override default values:
#    - DAEMON_FRAMEWORK_DIR
#    - KD_CONFIG
#    - KD_VERBOSE
#    - KD_OPTIMIZATION
#    - KD_GDB_SUPPORT
#    
# 3. If necessary, define CFLAGS, CXXFLAGS, and/or CPPFLAGS with any extra
#    compilation flags that should be used when compiling KeyDaemon code files.
#
# 4. Include Parent.mk in your application's makefile after defining variables.
#
# 5. Add kd-parent as a dependency to your main build target to ensure relevant
#    KeyDaemon code files are compiled.
#
# 6. When building code files that include KeyDaemon header files, add
#    KD_DEFINE_FLAGS and KD_INCLUDE_FLAGS to CPPFLAGS.
#
# 7. When linking your application, add KD_OBJECTS_PARENT to your list of linked
#    object files.
endef
export HELPTEXT

######################## Initialize build variables: ##########################
# Define KeyDaemon directories:
KD_PROJECT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
KD_SOURCE_DIR:=$(KD_PROJECT_DIR)/Source
KD_INCLUDE_DIR:=$(KD_PROJECT_DIR)/Include

# Set Debug or Release mode:
KD_CONFIG?=Release

# enable or disable verbose output:
KD_VERBOSE?=0
KD_V_AT:=$(shell if [ $(KD_VERBOSE) != 1 ]; then echo '@'; fi)

# Select specific build architectures:
KD_TARGET_ARCH?=-march=native

################ Configure and include framework makefile: ####################
DAEMON_FRAMEWORK_DIR?=$(KD_PROJECT_DIR)/deps/DaemonFramework
DF_CONFIG:=$(KD_CONFIG)
DF_VERBOSE:=$(KD_VERBOSE)
DF_OBJDIR:=$(KD_OBJDIR)
DF_TARGET_ARCH:=$(KD_TARGET_ARCH)

include $(DAEMON_FRAMEWORK_DIR)/Parent.mk

############################### Set build flags: ##############################
#### Config-specific flags: ####
ifeq ($(KD_CONFIG),Debug)
    KD_OPTIMIZATION?=1
    KD_GDB_SUPPORT?=0
    # Debug-specific preprocessor definitions:
    KD_CONFIG_FLAGS=-DDEBUG=1
endif

ifeq ($(KD_CONFIG),Release)
    KD_OPTIMIZATION?=1
    KD_GDB_SUPPORT?=0
endif

# Set optimization level flags:
ifeq ($(KD_OPTIMIZATION),1)
    KD_CONFIG_CFLAGS:=-O3 -flto
else
    KD_CONFIG_CFLAGS:=-O0
endif

# Set debugging flags:
ifeq ($(KD_GDB_SUPPORT), 1)
    KD_CONFIG_CFLAGS:=$(KD_CONFIG_CFLAGS) -g -ggdb
endif

#### C compilation flags: ####
KD_CFLAGS:=$(KD_TARGET_ARCH) $(KD_CONFIG_CFLAGS) $(CFLAGS)

#### C++ compilation flags: ####
KD_CXXFLAGS:=-std=gnu++14 $(CXXFLAGS)

#### C Preprocessor flags: ####

# Include directories:
KD_INCLUDE_FLAGS:=$(call recursiveInclude,$(KD_INCLUDE_DIR)/Parent) \
                  $(call recursiveInclude,$(KD_INCLUDE_DIR)/Shared) \
                  $(DF_INCLUDE_FLAGS)

# Disable dependency generation if multiple architectures are set
KD_DEPFLAGS:=$(if $(word 2, $(DF_TARGET_ARCH)), , -MMD)

KD_DEFINE_FLAGS:=$(call addDef,KD_KEY_LIMIT) \
                 $(call addDef,KD_VERBOSE) \
                 $(call addStringDef,KD_DAEMON_PATH) \
                 $(call addStringDef,KD_PIPE_PATH) \
                 $(DF_DEFINE_FLAGS)

KD_CPPFLAGS:=-pthread \
             $(KD_DEPFLAGS) \
             $(KD_CONFIG_FLAGS) \
             $(KD_DEFINE_FLAGS) \
             $(KD_INCLUDE_FLAGS) \
             $(KD_CPPFLAGS)

KD_OBJECTS:=$(KD_OBJDIR)/Controller.o $(KD_OBJDIR)/EventType.o

KD_OBJECTS_PARENT:=$(KD_OBJECTS) $(DF_OBJECTS_PARENT)

KD_BUILD_FLAGS:=$(KD_CFLAGS) $(KD_CXXFLAGS) $(KD_CPPFLAGS)

###################### Supporting Build Targets: ##############################

.PHONY: kd-parent kd-check-defs kd-clean

## Main build target: ##
kd-parent : kd-check-defs df-parent $(KD_OBJECTS)


kd_check_defs:
	@if [ -z "$(KD_OBJDIR)" ]; then \
        echo >&2 "Build failed, KD_OBJDIR not defined."; exit 1; \
	elif [ -z "$(KD_PARENT_PATH)" ]; then \
        echo >&2 "Build failed, KD_PARENT_PATH not defined."; exit 1; \
	elif [ -z "$(KD_DAEMON_PATH)" ]; then \
        echo >&2 "Build failed, KD_DAEMON_PATH not defined."; exit 1; \
	elif [ -z "$(KD_PIPE_PATH)" ]; then \
        echo >&2 "Build failed, KD_PIPE_PATH not defined."; exit 1; \
	elif [ -z "$(KD_KEY_LIMIT)" ]; then \
        echo >&2 "Build failed, KD_KEY_LIMIT count not defined."; exit 1; \
	fi

$(KD_OBJECTS) :
	@echo "Compiling $(<F):"
	$(KD_V_AT)mkdir -p $(KD_OBJDIR)
	@if [ "$(KD_VERBOSE)" == "1" ]; then \
        $(DAEMON_FRAMEWORK_DIR)/cleanPrint.sh '$(CXX) $(KD_BUILD_FLAGS)'; \
        echo '    -o "$@" \'; \
        echo '    -c "$<"'; \
        echo ''; \
	fi
	@$(CXX) $(KD_BUILD_FLAGS) -o "$@" -c "$<"

-include $(KD_OBJECTS:%.o=%.d)

$(KD_OBJDIR)/Controller.o: \
	$(KD_SOURCE_DIR)/Controller.cpp
$(KD_OBJDIR)/EventType.o: \
	$(KD_SOURCE_DIR)/EventType.cpp
