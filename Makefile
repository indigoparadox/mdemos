
# vim: ft=make noexpandtab

MDEMO_C_FILES := src/main.c src/demos.c

CC_GCC := gcc
CC_WATCOM := wcc386
LD_WATCOM := wcl386
MD := mkdir -p

GLOBAL_DEFINES :=

CFLAGS_GCC := $(GLOBAL_DEFINES) -Imaug/src
CFLAGS_WATCOM := $(GLOBAL_DEFINES) -imaug/src
LDFLAGS_GCC := -lm
LDFLAGS_WATCOM :=

# Optional builds.
ifneq ("$(RELEASE)","RELEASE")
	CFLAGS_WATCOM += -we -d3
	CFLAGS_GCC += -DDEBUG -Wall -g -fsanitize=address -fsanitize=leak -fsanitize=undefined
	LDFLAGS_GCC += -g -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

ifeq ("$(API)","SDL")
	ifeq ("$(SDL_VER)","1")
		CFLAGS_GCC += -DRETROFLAT_API_SDL1 $(shell pkg-config sdl --cflags)
		LDFLAGS_GCC += $(shell pkg-config sdl --libs) -lSDL_ttf
	else
		CFLAGS_GCC += -DRETROFLAT_API_SDL2 $(shell pkg-config sdl2 --cflags)
		LDFLAGS_GCC += $(shell pkg-config sdl2 --libs) -lSDL_ttf
	endif
else
	CFLAGS_GCC += -DRETROFLAT_API_ALLEGRO $(shell pkg-config allegro --cflags)
	LDFLAGS_GCC += $(shell pkg-config allegro --libs)
	API := ALLEGRO
endif

# Target-specific options.
.PHONY: clean

all: mdemo mdemd.exe mdemw.exe mdemnt.exe

# Unix

mdemo: $(addprefix obj/$(shell uname -s)/,$(subst .c,.o,$(MDEMO_C_FILES)))
	$(CC_GCC) -o $@ $^ $(LDFLAGS_GCC)

obj/$(shell uname -s)/%.o: %.c
	$(MD) $(dir $@)
	$(CC_GCC) -c -o $@ $< -DRETROFLAT_OS_UNIX $(CFLAGS_GCC)

# WASM

mdemo.html: $(addprefix obj/wasm/,$(subst .c,.o,$(MDEMO_C_FILES)))
	emcc -o $@ $^ -s USE_SDL=2 -s USE_SDL_TTF=2

obj/wasm/%.o: %.c
	$(MD) $(dir $@)
	emcc -c -o $@ $< -DRETROFLAT_OS_WASM -DRETROFLAT_API_SDL2 -s USE_SDL=2 -Imaug/src -s USE_SDL_TTF=2

# DOS

mdemd.exe: $(addprefix obj/dos/,$(subst .c,.o,$(MDEMO_C_FILES)))
	#i586-pc-msdosdjgpp-gcc -o $@ $^ -L$(ALLEGRO_DJGPP_ROOT)/lib -lalleg
	wcl386 -l=dos32a -s -3s -k128k dos/clib3s.lib alleg.lib $(LDFLAGS_WATCOM) -fe=$@ $^

obj/dos/%.o: %.c
	$(MD) $(dir $@)
	#i586-pc-msdosdjgpp-gcc -fgnu89-inline -I$(ALLEGRO_DJGPP_ROOT)/include -DRETROFLAT_OS_DOS -DRETROFLAT_API_$(API) -c -o $@ $<
	wcc386 -bt=dos32a -s -3s -DRETROFLAT_OS_DOS -DRETROFLAT_API_$(API) $(CFLAGS_WATCOM) -fo=$@ $(<:%.c=%)

# WinNT

mdemnt.exe: $(addprefix obj/nt/,$(subst .c,.o,$(MDEMO_C_FILES)))
	wlink name $@ system nt_win fil {$^}

obj/nt/%.o: %.c
	$(MD) $(dir $@)
	wcc386 -bt=nt -i$(WATCOM)/h/nt -DRETROFLAT_API_WIN32 -DRETROFLAT_OS_WIN $(CFLAGS_WATCOM) -fo=$@ $(<:%.c=%) -DRETROFLAT_SCREENSAVER

# Win386

mdemw.rex: $(addprefix obj/win16/,$(subst .c,.o,$(MDEMO_C_FILES)))
	wcl386 -l=win386 $(LDFLAGS_WATCOM) -fe=$@ $^

mdemw.exe: mdemw.rex
	wbind $< -s $(WATCOM)/binw/win386.ext -R $@

obj/win16/%.o: %.c
	$(MD) $(dir $@)
	wcc386 -bt=windows -i$(WATCOM)/h/win -DRETROFLAT_API_WIN16 -DRETROFLAT_OS_WIN $(CFLAGS_WATCOM) -fo=$@ $(<:%.c=%) -DRETROFLAT_SCREENSAVER

# Clean

clean:
	rm -rf obj mdemo mdemw32.exe *.err mdemd.exe mdemw.exe *.rex mdemnt.exe

