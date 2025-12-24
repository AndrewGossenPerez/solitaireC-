
#Portable to Windows,macOs, and Linux

# Project
APP      := solitaire
SRC_DIR  := src
INC_DIR  := include
OBJ_DIR  := build

# Toolchain
CXX      ?= g++
CXXSTD   := -std=c++17
WARN     := -Wall -Wextra
OPT      ?= -O2
DBG      ?=
DEFS     :=
INCLUDES := -I$(INC_DIR)

CXXFLAGS := $(CXXSTD) $(WARN) $(OPT) $(DBG) $(DEFS) $(INCLUDES)
LDFLAGS  :=
LDLIBS   :=

# Source discovery
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

UNAME_S := $(shell uname -s 2>/dev/null) # Detect Os platform

# Helper: detect if a command exists
cmd-exists = $(shell command -v $(1) >/dev/null 2>&1 && echo yes || echo no)

# SFML Linkage

SFML_CFLAGS :=
SFML_LIBS   := -lsfml-graphics -lsfml-window -lsfml-system

# pkg-config detection
ifeq ($(call cmd-exists,pkg-config),yes)
  # Some systems name it sfml-all, but sfml-graphics is common.
  SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics 2>/dev/null)
  SFML_LIBS   := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null)
endif

# If pkg-config returned nothing, clear it so we can fall back
ifeq ($(strip $(SFML_CFLAGS)),)
  SFML_CFLAGS :=
endif
ifeq ($(strip $(SFML_LIBS)),)
  SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
endif

# macOS homebrew fallback
ifeq ($(UNAME_S),Darwin)
  ifeq ($(strip $(SFML_CFLAGS)),)
    ifeq ($(call cmd-exists,brew),yes)
      BREW_SFML_PREFIX := $(shell brew --prefix sfml 2>/dev/null)
      ifneq ($(strip $(BREW_SFML_PREFIX)),)
        SFML_CFLAGS := -I$(BREW_SFML_PREFIX)/include
        # Homebrew typically works with these; frameworks avoid common mac link errors.
        SFML_LIBS   := -L$(BREW_SFML_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system \
                       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
      endif
    endif
  endif
endif

# Window vcpckg fallback
ifneq (,$(findstring MINGW,$(UNAME_S)))
  ifeq ($(strip $(SFML_CFLAGS)),)
    ifneq ($(strip $(VCPKG_ROOT)),)
      # vcpkg triplets differ; user can override VCPKG_TRIPLET.
      VCPKG_TRIPLET ?= x64-windows
      VCPKG_INSTALLED := $(VCPKG_ROOT)/installed/$(VCPKG_TRIPLET)
      SFML_CFLAGS := -I$(VCPKG_INSTALLED)/include
      SFML_LIBS   := -L$(VCPKG_INSTALLED)/lib -lsfml-graphics -lsfml-window -lsfml-system
    endif
  endif
endif

# sfml dir fallback
ifeq ($(strip $(SFML_CFLAGS)),)
  ifneq ($(strip $(SFML_DIR)),)
    SFML_CFLAGS := -I$(SFML_DIR)/include
    SFML_LIBS   := -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system
  endif
endif

# Apply SFML flags
CXXFLAGS += $(SFML_CFLAGS)
LDLIBS   += $(SFML_LIBS)

# Build rules 
.PHONY: all clean run info

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(APP)
	./$(APP)

clean:
	rm -rf $(OBJ_DIR) $(APP)

# Prints what it detected/used (useful for debugging portability)
info:
	@echo "UNAME_S      = $(UNAME_S)"
	@echo "CXX          = $(CXX)"
	@echo "CXXFLAGS     = $(CXXFLAGS)"
	@echo "LDLIBS       = $(LDLIBS)"
	@echo "SRCS         = $(SRCS)"
