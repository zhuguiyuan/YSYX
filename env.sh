#!/bin/bash

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

init NJU-ProjectN/fceux-am ics2021 fceux-am
init NJU-ProjectN/nvboard master nvboard

addenv NEMU_HOME nemu
addenv AM_HOME abstract-machine
addenv NPC_HOME npc
addenv NVBOARD_HOME nvboard
addenv STA_HOME yosys-sta
pushd "$STA_HOME" && make init && popd
