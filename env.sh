#!/bin/bash

if [[ "$0" == "${BASH_SOURCE[0]}" ]]; then
    echo "This script is designed to be sourced, not executed directly."
    exit 1
fi

# usage: addenv env_name path
function addenv() {
  echo "export $1=$(readlink -f $2)"
  export $1=$(readlink -f $2)
}

# usage: init repo branch directory
function init() {
  if [ -d $3 ]; then
    echo "$3 is already initialized, skipping..."
    return
  fi

  while [ ! -d $3 ]; do
    git clone -b $2 git@github.com:$1.git $3
  done
}

function warn() {
  echo -e "\e[33m$*\e[0m"
}

warn "Install essential packages"
sudo apt install build-essential man gcc-doc gdb valgrind git wget \
  libreadline-dev libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev \
  libfmt-dev libgoogle-glog-dev xxd llvm-dev bash-completion flex bison

warn "Ensure submodules exist"
init NJU-ProjectN/fceux-am ics2021 fceux-am
init NJU-ProjectN/nvboard master nvboard
init OSCPU/yosys-sta master yosys-sta
init NJU-ProjectN/am-kernels ics2021 am-kernels

warn "Now you should add following lines to .bashrc"
addenv NEMU_HOME nemu
addenv AM_HOME abstract-machine
addenv NPC_HOME npc
addenv NVBOARD_HOME nvboard
addenv STA_HOME yosys-sta

warn "Initialize Yosys-STA"
pushd "$STA_HOME" && make init && popd

warn "Finally you should goto https://github.com/YosysHQ/oss-cad-suite-build to download Yosys, Icarus Verilog, and GtkWave"

