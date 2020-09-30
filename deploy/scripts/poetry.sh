#!/bin/bash

# This script installs poetry and the backend package dependencies

echo "********** Installing poetry **********"

poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

if ! command -v pyenv &> /dev/null
then
    curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3
else
    echo "Poetry is already installed, skipping installation"
fi

# Getting absolute path of backend files
script_dir=$(dirname $(realpath $0))
backend_dir=$script_dir/../../backend

cd $backend_dir
$poetry config virtualenvs.create false
$ventserver_env $poetry install