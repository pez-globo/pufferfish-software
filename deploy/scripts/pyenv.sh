#!/bin/bash

echo "********** Installing pyenv **********"

sudo apt update
sudo apt install libffi-dev curl wget gcc make zlib1g-dev libsqlite3-dev -y
sudo apt install build-essential libssl-dev libbz2-dev libreadline-dev -y
sudo apt install libncurses5-dev libncursesw5-dev xz-utils libffi-dev liblzma-dev python-openssl -y

if ! command -v pyenv &> /dev/null
then
    curl https://pyenv.run | bash
else
    echo "pyenv is already installed, skipping installation."
fi

if [ 0 -eq $( cat $HOME/.bashrc | grep -c "pyenv" ) ]
then
    cd ~/pufferfish-vent-software/deploy
    cat configs/pyenv_config.txt >> ~/.bashrc
else
    echo "pyenv already added to path"
fi

pyenv="$HOME/.pyenv/bin/pyenv"

if [ 0 -eq $( $pyenv versions | grep -c "3.7.7" ) ]
then
    $pyenv install 3.7.7
else
    echo "Python 3.7.7 is already installed"
fi

if [ 0 -eq $( $pyenv versions | grep -c "ventserver" ) ]
then
    $pyenv virtualenv 3.7.7 ventserver
else
    echo "ventserver environment already exists!"
fi

cd ~/pufferfish-vent-software/backend
$pyenv local ventserver