
# vim: ft=make noexpandtab

SCREENSAVER=1
C_FILES := src/main.c src/demos.c

LIBS_GCC := -lm
DEFINES_WATCOM := -DRETROFLAT_SOFT_SHAPES
DEFINES_GCC := -DRETROFLAT_SOFT_SHAPES

include maug/Makefile.inc

# Target-specific options.
.PHONY: clean

all: mdemo.ale mdemo.sdl mdemod.exe mdemow.exe mdemont.exe mdemo.html

$(eval $(call TGTNDSLIBN,mdemo,,mdemo.bmp))

# Unix (Allegro)

$(eval $(call TGTUNIXALE,mdemo))

# Unix (SDL)

$(eval $(call TGTUNIXSDL,mdemo))

# WASM

$(eval $(call TGTWASMSDL,mdemo))

# DOS

$(eval $(call TGTDOSALE,mdemo))

# WinNT

$(eval $(call TGTWINNT,mdemo))

# Win386

$(eval $(call TGTWIN386,mdemo))

# Clean

clean:
	rm -rf $(CLEAN_TARGETS)

