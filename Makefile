LLVM_ARCH ?= x86_64
MSVC_ARCH ?= x64
MSVC_BASE ?= ../vstoolchain/VC/Tools/MSVC
MSVC_VER ?= 14.29.30133
UNAME = $(shell uname -s)
WINSDK_BASE ?= ../vstoolchain/Windows Kits/10
WINSDK_VER ?= 10.0.20348.0

ifdef LLVM_INCLUDE_PATH
	CPPFLAGS ?= "-I$(LLVM_INCLUDE_PATH)"
	LIBS ?= "-L$(LLVM_LIB_PATH)"
	CLANG_CC ?= clang
else
	ifdef LLVM_BASE
		CLANG_CC := $(LLVM_BASE)/bin/clang
		LLVM_CONFIG := $(LLVM_BASE)/bin/llvm-config
	else
		CLANG_CC := clang
		LLVM_CONFIG := llvm-config
	endif
	LLVM_INCLUDE_PATH := $(shell $(LLVM_CONFIG) --includedir)
	LLVM_LIB_PATH := $(shell $(LLVM_CONFIG) --libdir)
	CPPFLAGS := $(shell $(LLVM_CONFIG) --cflags)
	LIBS := "-L$(LLVM_LIB_PATH)"
endif

ifdef DYNAMIC_BUILD
CPPFLAGS += -DDYNAMIC_BUILD=1
else
LIBS += -L. -lLLVM-C
endif

CPPFLAGS += -D_WIN32_WINNT=0x0601
CPPFLAGS += "-I$(MSVC_BASE)/$(MSVC_VER)/include"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/ucrt"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/shared"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/um"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/winrt"
ifeq ($(UNAME), Linux)
CPPFLAGS += -Xclang -ivfsoverlay -Xclang "$(WINSDK_BASE)/winsdk_vfs_overlay.yaml"
endif

CFLAGS := --target=$(LLVM_ARCH)-windows-msvc
LDFLAGS := -fuse-ld=lld

LDFLAGS += "-L$(MSVC_BASE)/$(MSVC_VER)/lib/$(MSVC_ARCH)"
LDFLAGS += "-L$(WINSDK_BASE)/Lib/$(WINSDK_VER)/ucrt/$(MSVC_ARCH)"
LDFLAGS += "-L$(WINSDK_BASE)/Lib/$(WINSDK_VER)/um/$(MSVC_ARCH)"
ifeq ($(UNAME), Linux)
LDFLAGS += "-L../vstoolchain/winsdk_lib_symlinks_$(MSVC_ARCH)"
endif

SRC := $(wildcard *.c)
DST := $(SRC:.c=.exe)

.PHONY: all clean

all: $(DST)

%.exe: %.c
	$(CLANG_CC) -o $@ $^ $(CPPFLAGS) $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	-rm -f $(DST)
