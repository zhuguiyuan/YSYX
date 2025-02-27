# Project Variables
TOP_MODULE = running_light
VSRCS = $(TOP_MODULE).v
TARGET_DIR = target

all: sim

# Verilator Common Flags
VERILATOR_FLAGS = -Wall -MMD -O3 --x-assign fast --x-initial fast \
	--prefix Vtop --top-module $(TOP_MODULE)

# Verilator Targets
CSRCS_SIM = main_sim.cc
MDIR_SIM = $(TARGET_DIR)/sim
WAVE_FILE = wave.fst

$(MDIR_SIM)/Vtop: $(VSRCS) $(CSRCS_SIM)
	mkdir -p $(@D)
	cp $(CSRCS_SIM) $(MDIR_SIM)
	verilator -cc -exe $(VERILATOR_FLAGS) --trace-fst \
			--build -Mdir $(@D) \
			$(CSRCS_SIM) $(VSRCS) -o Vtop

# NVBoard Targets
CSRCS_NVB = main_nvb.cc
NXDC_NVB = top.nxdc
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


# Quick Commands
sim: $(MDIR_SIM)/Vtop
	$<

nvb: $(MDIR_NVB)/Vtop
	$<

clean:
	rm -rf $(TARGET_DIR) $(WAVE_FILE)

.PHONY: all sim nvb clean
