LLVM_ARCH ?= x86_64
MSVC_ARCH ?= x64
MSVC_BASE ?= ../vstoolchain/VC/Tools/MSVC
MSVC_VER ?= 14.50.35717
UNAME = $(shell uname -s)
WINSDK_BASE ?= ../vstoolchain/Windows Kits/10
WINSDK_VER ?= 10.0.26100.0

CPPFLAGS :=
CFLAGS := --target=$(LLVM_ARCH)-windows-msvc /Zc:__STDC__ /MD /TC
LDFLAGS :=
LIBS :=

ifneq ($(LLVM_INCLUDE_PATH),)
	CPPFLAGS += "-I$(LLVM_INCLUDE_PATH)"
endif
ifneq ($(LLVM_LIB_PATH),)
	LIBS += "/LIBPATH:$(LLVM_LIB_PATH)"
endif

ifndef CLANG_CL
	ifdef LLVM_BASE
		CLANG_CL := $(LLVM_BASE)/bin/clang-cl
		LLVM_CONFIG := $(LLVM_BASE)/bin/llvm-config
	else
		CLANG_CL := clang-cl
		LLVM_CONFIG := llvm-config
	endif
	LLVM_INCLUDE_PATH := $(shell $(LLVM_CONFIG) --includedir)
	LLVM_LIB_PATH := $(shell $(LLVM_CONFIG) --libdir)
	CPPFLAGS += $(shell $(LLVM_CONFIG) --cppflags)

	ifneq ($(LLVM_INCLUDE_PATH),)
		CPPFLAGS += -I$(LLVM_INCLUDE_PATH)
	endif
	ifneq ($(LLVM_LIB_PATH),)
		LIBS += "/LIBPATH:$(LLVM_LIB_PATH)"
	endif
endif


ifdef DYNAMIC_BUILD
CPPFLAGS += -DDYNAMIC_BUILD=1
else
LIBS += /LIBPATH:. LLVM-C.lib
endif

CPPFLAGS += -W4
CPPFLAGS += -Wno-unused-command-line-argument
CPPFLAGS += -Wno-unknown-pragmas
CPPFLAGS += -Wno-int-in-bool-context
CPPFLAGS += -Wno-msvc-not-found
CPPFLAGS += -Wno-pragma-pack
CPPFLAGS += -Wno-nonportable-include-path
CPPFLAGS += -Wno-ignored-pragma-intrinsic
CPPFLAGS += -Wno-microsoft-anon-tag
CPPFLAGS += -Wno-microsoft-exception-spec

CPPFLAGS += -D_WIN32_WINNT=0x0601
CPPFLAGS += "-I$(MSVC_BASE)/$(MSVC_VER)/include"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/ucrt"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/shared"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/um"
CPPFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/winrt"
ifeq ($(UNAME), Linux)
CPPFLAGS += -Xclang -ivfsoverlay -Xclang "$(WINSDK_BASE)/winsdk_vfs_overlay.yaml"
endif
ifeq (,$(findstring MINGW, $(UNAME)))
CPPFLAGS += -DCROSS_COMPILE=1
endif

LDFLAGS += "/LIBPATH:$(MSVC_BASE)/$(MSVC_VER)/lib/$(MSVC_ARCH)"
LDFLAGS += "/LIBPATH:$(WINSDK_BASE)/Lib/$(WINSDK_VER)/ucrt/$(MSVC_ARCH)"
LDFLAGS += "/LIBPATH:$(WINSDK_BASE)/Lib/$(WINSDK_VER)/um/$(MSVC_ARCH)"
ifeq ($(UNAME), Linux)
LDFLAGS += "/LIBPATH:../vstoolchain/winsdk_lib_symlinks_$(MSVC_ARCH)"
endif

SRC := $(wildcard *.c)
DST := $(SRC:.c=.exe)

.PHONY: all clean

all: $(DST)

%.exe: %.c
	$(CLANG_CL) -o $@ $^ $(CPPFLAGS) $(CFLAGS) -fuse-ld=lld /link $(LIBS) $(LDFLAGS)

clean:
	-rm -f $(DST)
