CC ?= clang
CFLAGS := --target=x86_64-windows-msvc $(shell llvm-config --cflags)
LDFLAGS := -fuse-ld=lld
LIBS := #-L. -lllvm-c
MSVC_BASE := ../vstoolchain/VC/Tools/MSVC
MSVC_VER := 14.29.30133
WINSDK_BASE := ../vstoolchain/Windows Kits/10
WINSDK_VER := 10.0.20348.0

CFLAGS += -D_WIN32_WINNT=0x0601
CFLAGS += "-I$(MSVC_BASE)/$(MSVC_VER)/include"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/ucrt"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/shared"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/um"
CFLAGS += "-I$(WINSDK_BASE)/Include/$(WINSDK_VER)/winrt"
CFLAGS += -Xclang -ivfsoverlay -Xclang "$(WINSDK_BASE)/winsdk_vfs_overlay.yaml"

LDFLAGS += "-L$(MSVC_BASE)/$(MSVC_VER)/lib/x64"
LDFLAGS += "-L$(WINSDK_BASE)/Lib/$(WINSDK_VER)/ucrt/x64"
LDFLAGS += "-L$(WINSDK_BASE)/Lib/$(WINSDK_VER)/um/x64"
LDFLAGS += "-L../vstoolchain/winsdk_lib_symlinks_x64"

SRC := $(wildcard *.c)
DST := $(SRC:.c=.exe)

.PHONY: all clean

all: $(DST)

%.exe: %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	-rm -f $(DST)
