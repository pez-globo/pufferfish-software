#!/bin/bash

# Installs pyenv and sets up an environment named ventserver
# with python 3.7.7 for backend

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing pyenv **********\n${NC}"

# Getting absolute path of backend and config files
script_dir=$(dirname $(realpath $0))
backend_dir=$script_dir/../../backend
config_dir=$script_dir/../configs

# Installing pyenv and required dependencies
if ! command -v pyenv &> /dev/null
then
    sudo apt update
    sudo apt install libffi-dev curl wget gcc make zlib1g-dev libsqlite3-dev -y
    sudo apt install build-essential libssl-dev libbz2-dev libreadline-dev -y
    sudo apt install libncurses5-dev libncursesw5-dev xz-utils libffi-dev liblzma-dev python-openssl -y
    curl https://pyenv.run | bash
else
    echo -e "${WARNING} pyenv is already installed, skipping installation${NC}"
fi

if [ 0 -eq $( cat $HOME/.bashrc | grep -c "pyenv" ) ]
then
    if [ 1 -eq $( ls $config_dir | grep -c "pyenv_config.txt" ) ]
    then
        cat $config_dir/pyenv_config.txt >> ~/.bashrc
    else
        echo -e "${ERROR} Configuration file (pyenv_config.txt) not found!${NC}"
        exit 1
    fi
else
    echo -e "${WARNING} pyenv already added to path${NC}"
fi

pyenv="$HOME/.pyenv/bin/pyenv"

# Installing python 3.7.7
if [ 0 -eq $( $pyenv versions | grep -c "3.7.7" ) ]
then
    $pyenv install 3.7.7
else
    echo -e "${WARNING} Python 3.7.7 is already installed${NC}"
fi

if [ 0 -eq $( $pyenv versions | grep -c "ventserver" ) ]
then
    $pyenv virtualenv 3.7.7 ventserver
else
    echo -e "${WARNING} ventserver environment already exists!${NC}"
fi

cd $backend_dir
$pyenv local ventserver

echo -e "\n${SUCCESS}Pyenv setup complete\n${NC}"