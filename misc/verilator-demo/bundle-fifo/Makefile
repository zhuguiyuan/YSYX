TOP_MODULE = bundle_fifo
VSRCS = bundle_fifo.v
CSRCS = bundle_fifo_sim.cc
MDIR_SIM = target
WAVE_FILE = wave.fst

all: sim

WARN_FLAGS = -Wall -Wno-PROCASSINIT
WAVE_FLAGS = --trace-fst

# Verilator Targets
$(MDIR_SIM)/Vtop.mk: $(VSRCS)
	verilator -cc -exe -top-module $(TOP_MODULE) --prefix Vtop \
		-Mdir $(MDIR_SIM) $(WARN_FLAGS) $(WAVE_FLAGS) \
		$(CSRCS) $(VSRCS)

$(MDIR_SIM)/Vtop: $(MDIR_SIM)/Vtop.mk $(VSRCS) $(CSRCS)
	cd $(@D) && $(MAKE) -f Vtop.mk


# Quick Commands
sim: $(MDIR_SIM)/Vtop
	$<

# for cpp completion
dev: $(MDIR_SIM)/Vtop.mk

clean:
	rm -rf $(MDIR_SIM) $(WAVE_FILE)


.PHONY: all sim dev clean
