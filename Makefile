
# vim: ft=make noexpandtab

SCREENSAVER=1
OPENMP=1
VDP=1
#WING=1
C_FILES := src/main.c src/demos.c

LIBS_GCC := -lm

include maug/Makefile.inc

# Target-specific options.
.PHONY: clean

all: mdemo.ale mdemo.sdl mdemod.exe mdemow.exe mdemont.exe mdemo.html mdemo64.exe mdemontg.exe

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

