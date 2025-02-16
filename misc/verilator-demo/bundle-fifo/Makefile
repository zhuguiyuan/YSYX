VSRCS = top.v
CSRCS = top.cc
MDIR = target
WAVE = wave.fst

all: sim


# Verilator Targets
$(MDIR)/Vtop.mk: $(VSRCS) $(CSRCS)
	verilator -cc -exe -Wall -Mdir $(MDIR) --trace-fst $(CSRCS) $(VSRCS) 

$(MDIR)/Vtop: $(MDIR)/Vtop.mk $(VSRCS) $(CSRCS)
	cd $(MDIR) && $(MAKE) -f Vtop.mk


# Quick Commands
sim: $(MDIR)/Vtop
	$(MDIR)/Vtop

# for cpp completion
dev: $(MDIR)/Vtop.mk

clean:
	rm -rf $(MDIR) $(WAVE)


.PHONY: all sim dev clean
