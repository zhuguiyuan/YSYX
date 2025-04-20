# Project Variables
TOP_MODULE = top
VSRCS = $(TOP_MODULE).v bcd7seg.v priority_encoder.v
TARGET_DIR = target

all: nvb

# Verilator Common Flags
VERILATOR_FLAGS = -MMD -O3 --x-assign fast --x-initial fast \
	--prefix Vtop --top-module $(TOP_MODULE)

# NVBoard Targets
CSRCS_NVB = main_nvb.cc
NXDC_NVB = $(TOP_MODULE).nxdc
MDIR_NVB = $(TARGET_DIR)/nvb
BIND_SRC = auto_pin_bind.cc

$(MDIR_NVB)/$(BIND_SRC): $(NXDC_NVB)
	mkdir -p $(@D)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# provide `nvboard-archive` target and `$(NVBOARD_ARCHIVE)` variable
include $(NVBOARD_HOME)/scripts/nvboard.mk

CFLAGS += $(addprefix -I, $(INC_PATH)) -DTOP_NAME="Vtop"

$(MDIR_NVB)/Vtop: $(VSRCS) $(CSRCS_NVB) $(NVBOARD_ARCHIVE) $(MDIR_NVB)/$(BIND_SRC)
	cp $(CSRCS_NVB) $(MDIR_NVB)
	verilator -cc -exe $(VERILATOR_FLAGS) \
		--build --Mdir $(MDIR_NVB) \
		$(VSRCS) $(CSRCS_NVB) $(NVBOARD_ARCHIVE) $(BIND_SRC) \
		-CFLAGS "$(CFLAGS)" -LDFLAGS "$(LDFLAGS)" -o Vtop

# STA
STA_DIR = result
sta:
	$(MAKE) -C $(STA_HOME) O=$(PWD) DESIGN=sta_wrapper \
		RTL_FILES="$(realpath $(wildcard *.v))" sta

# Quick Commands
nvb: $(MDIR_NVB)/Vtop
	$<

clean:
	rm -rf $(TARGET_DIR) $(WAVE_FILE) $(STA_DIR)

.PHONY: all nvb sta clean
