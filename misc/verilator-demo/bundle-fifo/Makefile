VSRCS = bundle_fifo.v
CSRCS = bundle_fifo_sim.cc
MDIR_SIM = target
WAVE_FILE = wave.fst

all: sim


# Verilator Targets
$(MDIR_SIM)/Vtop.mk: $(VSRCS) $(CSRCS)
	verilator -cc -exe -Wall  -Mdir $(MDIR_SIM) --trace-fst \
		--prefix Vtop -top-module bundle_fifo \
		$(CSRCS) $(VSRCS)

$(MDIR_SIM)/Vtop: $(MDIR_SIM)/Vtop.mk $(VSRCS) $(CSRCS)
	cd $(@D) && $(MAKE) -f Vtop.mk


# Quick Commands
sim: $(MDIR_SIM)/Vtop
	$>

# for cpp completion
dev: $(MDIR_SIM)/Vtop.mk

clean:
	rm -rf $(MDIR_SIM) $(WAVE_FILE)


.PHONY: all sim dev clean
