#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "${SUCCESS}********** Installing pyenv **********"

sudo apt update
sudo apt install libffi-dev curl wget gcc make zlib1g-dev libsqlite3-dev -y
sudo apt install build-essential libssl-dev libbz2-dev libreadline-dev -y
sudo apt install libncurses5-dev libncursesw5-dev xz-utils libffi-dev liblzma-dev python-openssl -y

# Getting absolute path of backend and config files
script_dir=$(dirname $(realpath $0))
backend_dir=$script_dir/../../backend
config_dir=$script_dir/../configs

if ! command -v pyenv &> /dev/null
then
    curl https://pyenv.run | bash
else
    echo -e "${WARNING} pyenv is already installed, skipping installation."
fi

if [ 0 -eq $( cat $HOME/.bashrc | grep -c "pyenv" ) ]
then
    if [ 1 -eq $( ls $config_dir | grep -c "pyenv_config.txt" ) ]
    then
        cat $config_dir/pyenv_config.txt >> ~/.bashrc
    else
        echo -e "${ERROR} Configuration file (pyenv_config.txt) not found!"
        exit 1
    fi
else
    echo -e "${SUCCESS}pyenv already added to path"
fi

pyenv="$HOME/.pyenv/bin/pyenv"

if [ 0 -eq $( $pyenv versions | grep -c "3.7.7" ) ]
then
    $pyenv install 3.7.7
else
    echo -e "${WARNING} Python 3.7.7 is already installed"
fi

if [ 0 -eq $( $pyenv versions | grep -c "ventserver" ) ]
then
    $pyenv virtualenv 3.7.7 ventserver
else
    echo -e "${WARNING} ventserver environment already exists!"
fi

cd $backend_dir
$pyenv local ventserver

echo -e "${SUCCESS}Pyenv setup complete"