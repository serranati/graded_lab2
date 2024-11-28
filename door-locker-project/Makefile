CC = gcc

DEBUGFLAGS = -fdebug-prefix-map=$(CURDIR)=. -ggdb
BASECFLAGS = -O1 -Wextra -Wall -Isrc/validation -Isrc/action
LEGCFLAGS  = -m32 -D_FORTIFY_SOURCE=0 -fno-stack-protector -fPIE
LEGLDFLAGS = -m32 -pie

CFLAGS  = $(BASECFLAGS) $(DEBUGFLAGS) $(LEGCFLAGS)
LDFLAGS = $(LEGLDFLAGS)

# A makefile function to strip debug symbols
define gendbg =
@echo "-- Extracting Debug Symbols from $@ to $@.dbg before stripping..."
objcopy --only-keep-debug $@ $@.dbg
strip --strip-debug $@
objcopy --add-gnu-debuglink=$@.dbg $@
@echo "-- Done."
endef

all: door-locker

# Dependencies
HEADERS = src/validation/validation_functions.h \
		  src/action/action_functions.h
SOURCES = src/main.c \
		  src/validation/validation_functions.c \
		  src/action/action_functions.c
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

door-locker door-locker.dbg &: $(OBJECTS) | $(HEADERS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(gendbg)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean clear
clean:
	@$(RM) -rv $(OBJECTS)
clear: clean
	@$(RM) -rv door-locker door-locker.dbg
