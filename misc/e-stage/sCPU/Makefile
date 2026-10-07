# Project Variables
TOP_MODULE ?= sCPU_top
VSRCS = $(wildcard rtl/*.v)
TARGET_DIR = target

all: sim

# Verilator Common Flags
VERILATOR_FLAGS = -Wall -MMD -O3 --x-assign fast --x-initial fast \
	--prefix Vtop --top-module $(TOP_MODULE) -o Vtop \

# Verilator Targets
CSRCS_SIM = sim/main_sim.cc
MDIR_SIM = $(TARGET_DIR)/sim
WAVE_FLAGS = --trace-fst
WAVE_FILE = wave.fst

$(MDIR_SIM)/Vtop: $(VSRCS) $(CSRCS_SIM)
	mkdir -p $(@D)
	cp $(CSRCS_SIM) $(MDIR_SIM)
	verilator -cc -exe --build $(VERILATOR_FLAGS) \
		-Mdir $(@D) $(WAVE_FLAGS) \
		$(CSRCS_SIM) $(VSRCS)

# NVBoard Targets
CSRCS_NVB = nvb/main_nvb.cc
NXDC_NVB = nvb/top.nxdc
MDIR_NVB = $(TARGET_DIR)/nvb
BIND_SRC = auto_pin_bind.cc

$(MDIR_NVB)/$(BIND_SRC): $(NXDC_NVB)
	mkdir -p $(@D)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# provide `nvboard-archive` target and `$(NVBOARD_ARCHIVE)` variable
-include $(NVBOARD_HOME)/scripts/nvboard.mk

CFLAGS += $(addprefix -I, $(INC_PATH)) -DTOP_NAME="Vtop"

$(MDIR_NVB)/Vtop: $(VSRCS) $(CSRCS_NVB) $(NVBOARD_ARCHIVE) $(MDIR_NVB)/$(BIND_SRC)
	cp $(CSRCS_NVB) $(MDIR_NVB)
	verilator -cc -exe $(VERILATOR_FLAGS) \
		--build --Mdir $(MDIR_NVB) \
		$(VSRCS) $(CSRCS_NVB) $(NVBOARD_ARCHIVE) $(BIND_SRC) \
		-CFLAGS "$(CFLAGS)" -LDFLAGS "$(LDFLAGS)"

# STA targets
MDIR_YOSYS = $(TARGET_DIR)/syn
syn:
	make -C $(STA_HOME) syn O=$(abspath $(MDIR_YOSYS)) \
		DESIGN=$(TOP_MODULE) CLK_PORT_NAME=clk_i RTL_FILES="$(abspath $(VSRCS))"

sta:
	make -C $(STA_HOME) sta O=$(abspath $(MDIR_YOSYS)) \
		DESIGN=$(TOP_MODULE) CLK_PORT_NAME=clk_i RTL_FILES="$(abspath $(VSRCS))"

# Quick Commands
sim: $(MDIR_SIM)/Vtop
	$<

nvb: $(MDIR_NVB)/Vtop
	$<

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all sim nvb syn sta clean
