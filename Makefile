
# vim: ft=make noexpandtab

SCREENSAVER=1
OPENMP=1
VDP=1
#WING=1
C_FILES := src/main.c src/demos.c
RETROFLAT_DOS_MEM_LARGE=1

LIBS_GCC := -lm
#DEFINES_WATCOM := -DRETROFLAT_SOFT_SHAPES
#DEFINES_GCC := -DRETROFLAT_SOFT_SHAPES
CFLAGS_OPT_GCC_NDS += -DMDEMO_NO_OPTIONS
GLOBAL_DEFINES += -DRETROFLAT_NO_STRING
#GLOBAL_DEFINES += -DRETROCON_DISABLE
GLOBAL_DEFINES += -DRETROCON_TRACE_LVL=1
GLOBAL_DEFINES += -DRETROGUI_TRACE_LVL=1
GLOBAL_DEFINES += -DRETROFLAT_NO_RETROGXC

include maug/Makefile.inc

# Target-specific options.
.PHONY: clean

all: mdemo.ale mdemo.sdl mdemod.exe mdemow.exe mdemont.exe mdemo.html mdemo64.exe mdemontg.exe mdemow3.exe mdemob.exe

$(eval $(call MVFS,unscii-8.hex))

$(eval $(call TGTNDSLIBN,mdemo,mdemo.bmp))

# Unix (Allegro)

$(eval $(call TGTUNIXALE,mdemo))

# Unix (SDL)

$(eval $(call TGTSDLICO,mdemo))

$(eval $(call TGTUNIXSDL,mdemo))

# WASM

$(eval $(call TGTWASMSDL,mdemo))

# DOS

$(eval $(call TGTDOSALE,mdemo))

$(eval $(call TGTDOSGAL,mdemo))

$(eval $(call TGTDOSBIOS,mdemo))

# Win386

$(eval $(call TGTWINICO,mdemo))

$(eval $(call TGTWIN386,mdemo))

$(eval $(call TGTWIN16,mdemo))

# WinNT

$(eval $(call TGTWINNT,mdemo))

$(eval $(call TGTWINNTGCC,mdemo))

$(eval $(call TGTWIN64GCC,mdemo))

# Clean

clean:
	rm -rf $(CLEAN_TARGETS)

