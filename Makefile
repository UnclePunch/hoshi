# === Tools ===
CC = powerpc-eabi-gcc
LD = powerpc-eabi-ld

# === Paths ===
SRCDIR   = src
LIBDIR   = Lib
BUILDDIR = build
BINDIR   = out
PACKDIR  = packtool

# === Compiler Flags ===
INCLUDES = -Iinclude -I$(LIBDIR) -I$(SRCDIR)

CFLAGS = -O1 -mcpu=750 -meabi -msdata=none -mhard-float -ffreestanding \
         -fno-unwind-tables -fno-exceptions -fno-asynchronous-unwind-tables \
         -fno-merge-constants -ffunction-sections -fdata-sections $(INCLUDES)

LDFLAGS = -r -T$(PACKDIR)/link.ld

# === Mod type and output ===
MODTYPE    = mdFunction
LINKED_O   = $(BUILDDIR)/linked.o
TARGET_BIN = $(BINDIR)/hoshi.bin

# === Collect source files ===
SRC_FILES := $(shell find $(SRCDIR) -type f \( -name '*.c' -o -name '*.s' -o -name '*.S' \))
LIB_FILES := $(shell find $(LIBDIR) -type f \( -name '*.c' -o -name '*.s' -o -name '*.S' \))
ALL_SOURCES := $(SRC_FILES) $(LIB_FILES)

# === Map each source file to a matching build object ===
OBJECTS := $(patsubst %.c,$(BUILDDIR)/%.o, \
          $(patsubst %.s,$(BUILDDIR)/%.o, \
          $(patsubst %.S,$(BUILDDIR)/%.o, \
          $(ALL_SOURCES))))

# === Default target ===
all: $(TARGET_BIN)

# === Compile or assemble all sources ===
$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# === Unified pattern rule for all ===
$(BUILDDIR)/%.o: %
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# === Link into relocatable object ===
$(LINKED_O): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

# === Pack custom binary ===
$(TARGET_BIN): $(LINKED_O)
	mkdir -p $(BINDIR)
	python $(PACKDIR)/main.py $< -m $(MODTYPE) -o $@

# === Clean ===
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean
