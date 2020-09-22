#!/bin/bash

echo "********** Installing poetry **********"

poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

if [ 0 -eq $( ls -a $HOME | grep -c ".poetry" ) ]
then
    curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3
else
    echo "Poetry is already installed"
fi

cd ~/pufferfish-vent-software/backend
$ventserver_env $poetry install