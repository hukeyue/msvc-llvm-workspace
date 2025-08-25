LLVM_ARCH ?= x86_64
MSVC_ARCH ?= x64
MSVC_BASE ?= ../vstoolchain/VC/Tools/MSVC
MSVC_VER ?= 14.29.30133
UNAME = $(shell uname -s)
WINSDK_BASE ?= ../vstoolchain/Windows Kits/10
WINSDK_VER ?= 10.0.20348.0

ifdef LLVM_BASE
CLANG_CC := $(LLVM_BASE)/bin/clang
LLVM_CONFIG := $(LLVM_BASE)/bin/llvm-config
else
CLANG_CC := clang
LLVM_CONFIG := llvm-config
endif

CFLAGS := --target=$(LLVM_ARCH)-windows-msvc $(shell $(LLVM_CONFIG) --cflags)
LDFLAGS := -fuse-ld=lld
ifdef DYNAMIC_BUILD
CFLAGS += -DDYNAMIC_BUILD=1
LIBS :=
else
LIBS := -L. -lllvm-c
endif

CFLAGS += -D_WIN32_WINNT=0x0601
CFLAGS += "-I$(MSVC_BASE)/$(MSVC_VER)/include"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/ucrt"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/shared"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/um"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/winrt"
ifeq ($(UNAME), Linux)
CFLAGS += -Xclang -ivfsoverlay -Xclang "$(WINSDK_BASE)/winsdk_vfs_overlay.yaml"
endif

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
	$(CLANG_CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	-rm -f $(DST)
