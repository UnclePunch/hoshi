ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment.")
endif

# === Tools ===
CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc
LD = $(DEVKITPPC)/bin/powerpc-eabi-ld

# === Paths ===
SRCDIR   		= src
LIBDIR   		= Lib
PACKDIR  		= packtool
INSTALL_DIR 	?= 	#can override this in the command line: make install INSTALL_DIR=/path/to/your/files

# === Compiler Flags ===
INCLUDES = -Iinclude -I$(LIBDIR) -I$(SRCDIR)

CFLAGS = -O1 -mcpu=750 -meabi -msdata=none -mhard-float -ffreestanding \
         -fno-unwind-tables -fno-exceptions -fno-asynchronous-unwind-tables \
         -fno-merge-constants -ffunction-sections -fdata-sections  -MMD -MP \
		 $(INCLUDES)

CFLAGS += $(ENABLE_LOGGING)

LDFLAGS = -r -T$(PACKDIR)/link.ld

# === Log Flags ===
BUILD ?= release

ifeq ($(BUILD),release)
    CFLAGS += -DLOG_LEVEL=3
endif
ifeq ($(BUILD),debug)
    CFLAGS += -DLOG_LEVEL=4
endif

# === Per Build Paths ===
BUILDDIR 		= build/$(BUILD)
BINDIR   		= out/$(BUILD)

# === Mod type and output ===
MODTYPE    = mdFunction
LINKED_O   = $(BUILDDIR)/linked.o
TARGET_BIN = $(BINDIR)/hoshi.bin

# Recursive wildcard function
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# === Collect source files ===
SRC_FILES := $(call rwildcard,$(SRCDIR)/,*.c *.s *.S)
LIB_FILES := $(call rwildcard,$(LIBDIR)/,*.c *.s *.S)
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

# === Link into relocatable object ===
$(LINKED_O): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

# === Pack custom binary ===
$(TARGET_BIN): $(LINKED_O)
	mkdir -p $(BINDIR)
	python $(PACKDIR)/main.py $< -m $(MODTYPE) -o $@

-include $(OBJECTS:.o=.d)

# --- Install Target ---
# Copies the final .bin files from $(OUT_DIR) to $(INSTALL_DIR)
install: all
	@echo ""
	@echo "--- Installing hoshi to "$(INSTALL_DIR)" ---"
	cp "$(TARGET_BIN)" "$(strip $(INSTALL_DIR))"
	@echo ""
	@echo "Installation complete."

# === Clean ===
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean install
 