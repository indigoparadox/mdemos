
# vim: ft=make noexpandtab

SCREENSAVER=1
C_FILES := src/main.c src/demos.c

LIBS_GCC := -lm
DEFINES_WATCOM := -DRETROFLAT_SOFT_SHAPES
DEFINES_GCC := -DRETROFLAT_SOFT_SHAPES

include maug/Makefile.inc

# Target-specific options.
.PHONY: clean

all: mdemo.ale mdemo.sdl mdemod.exe mdemow.exe mdemont.exe mdemo.html mdemo64.exe mdemontg.exe

$(eval $(call TGTNDSLIBN,mdemo,,mdemo.bmp))

# Unix (Allegro)

$(eval $(call TGTUNIXALE,mdemo))

# Unix (SDL)

$(eval $(call TGTSDLICO,mdemo))

$(eval $(call TGTUNIXSDL,mdemo))

# WASM

$(eval $(call TGTWASMSDL,mdemo))

# DOS

$(eval $(call TGTDOSALE,mdemo))

# WinNT

$(eval $(call TGTWINICO,mdemo))

$(eval $(call TGTWINNT,mdemo))

$(eval $(call TGTWINNTGCC,mdemo))

$(eval $(call TGTWIN64GCC,mdemo))

# Win386

$(eval $(call TGTWIN386,mdemo))

# Clean

clean:
	rm -rf $(CLEAN_TARGETS)

