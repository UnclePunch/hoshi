# Toolchain
DEVKITPPC ?= $(DEVKITPPC)
CC      := $(DEVKITPPC)/bin/powerpc-eabi-gcc
OBJDUMP := $(DEVKITPPC)/bin/powerpc-eabi-objdump
NM      := $(DEVKITPPC)/bin/powerpc-eabi-nm

PYTHON  := python

CFLAGS  := -O2 -Wall -mcpu=750 -meabi -mhard-float
ASFLAGS := -mcpu=750 -meabi -mhard-float -mregnames

SRC_DIR 	:= src
BUILD   	:= build
OUT_DIR     ?= out
INSTALL_DIR ?=

INC_DIR   := ../include
LD_SCRIPT := ../packtool/link.ld
ELF_OUT   := $(BUILD)/payload.elf
DOL_IN    ?= kar.dol
DOL_OUT   ?= $(OUT_DIR)/main.dol
RIIV_OUT   = $(OUT_DIR)/Riivolution

PREPROCESS_SCRIPT	:= dol_inject/preprocess.py
INJECT_SCRIPT 		:= dol_inject/dol_inject.py

SRC_C 		:= $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_S 		:= $(shell find $(SRC_DIR) -type f -name '*.S')
SRC_S_TMP 	:= $(shell find $(SRC_DIR) -type f -name '*.S.tmp')

# object paths in build/
OBJ_C := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRC_C))
OBJ_S := $(patsubst $(SRC_DIR)/%.S,$(BUILD)/%.o,$(SRC_S))
OBJS  := $(OBJ_C) $(OBJ_S)

.PHONY: all clean

all: $(DOL_OUT)

# ensure each target directory exists
DIRS := $(sort $(dir $(OBJS)))
$(DIRS):
	mkdir -p $@

# generate .S.tmp from .S
$(BUILD)/%.S.tmp: $(SRC_DIR)/%.S $(PREPROCESS_SCRIPT)
	$(PYTHON) $(PREPROCESS_SCRIPT) $< $@

# compile assembly -> build/%.o
$(BUILD)/%.o: $(BUILD)/%.S.tmp | $(BUILD)
	$(CC) -I $(INC_DIR) $(ASFLAGS) -x assembler-with-cpp -c $< -o $@

# compile C -> build/%.o
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	$(CC) -I $(INC_DIR) $(CFLAGS) -c $< -o $@

# link
$(ELF_OUT): $(OBJS)
	$(CC) -T $(LD_SCRIPT) $(OBJS) -o $(ELF_OUT) -r

# DOL pack
$(DOL_OUT): $(ELF_OUT)
	$(PYTHON) $(INJECT_SCRIPT) -e "$(ELF_OUT)" -d "$(DOL_IN)" -o "$(DOL_OUT)" --riiv-out $(RIIV_OUT) --game-id "GKYE01" --mod-name "Kirby Air Ride Deluxe"

# --- Install Target ---
# Copies the final .dol file
install: all
	@echo ""
	@echo "--- Installing dol to "$(INSTALL_DIR)/files" ---"
	cp "$(DOL_OUT)" "$(strip $(INSTALL_DIR)/sys)"
	@echo ""
	@echo "Successfully installed dol."

clean:
	rm -rf $(BUILD) $(ELF_OUT) $(DOL_OUT) $(SRC_DIR)/*.S.tmp
