#!/bin/bash

# This script installs poetry and the backend package dependencies

echo "********** Installing poetry **********"

poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3

cd ~/pufferfish-vent-software/backend
$ventserver_env $poetry install