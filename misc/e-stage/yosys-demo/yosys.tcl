# Parsing
read_verilog counter.v

# Elaboration
hierarchy -check -top counter

# Coarse-grain synthesis
proc
opt
fsm
memory

# Fine-grain synthesis
techmap
splitnets -ports
opt -full

# Technology mapping
read_liberty -lib cell.lib
dfflibmap -liberty cell.lib

# Write netlist
write_verilog netlist.v
stat -liberty cell.lib