#!/bin/bash

echo "********** Setting up custom boot screen **********"

cd ~/pufferfish-vent-software/deploy
sudo mkdir -p /etc/plymouth/themes
sudo cp plymouth/plymouthd.conf /etc/plymouth/plymouthd.conf
sudo cp -r plymouth/themes/pufferfish /etc/plymouth/themes/

echo "********** Installing pyenv **********"

sudo apt update
sudo apt install libffi-dev curl wget gcc make zlib1g-dev libsqlite3-dev -y
sudo apt install build-essential libssl-dev libbz2-dev libreadline-dev -y
sudo apt install libncurses5-dev libncursesw5-dev xz-utils libffi-dev liblzma-dev python-openssl -y
curl https://pyenv.run | bash

cd ~/pufferfish-vent-software/deploy
cat pyenv_config.txt >> ~/.bashrc

pyenv="$HOME/.pyenv/bin/pyenv"
poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

$pyenv install 3.7.7
$pyenv virtualenv 3.7.7 ventserver

cd ~/pufferfish-vent-software/backend

$pyenv local ventserver

echo "********** Installing poetry **********"

curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3

$ventserver_env $poetry install

echo "********** Installing node **********"

cd ~/
wget https://nodejs.org/dist/v12.18.3/node-v12.18.3-linux-x64.tar.gz
tar -xzf node-v12.18.3-linux-x64.tar.gz
cd node-v12.18.3-linux-x64/
sudo cp -R * /usr/local/

echo "********** Installing yarn **********"

sudo npm i -g yarn
cd ~/pufferfish-vent-software/frontend
yarn install
yarn build

echo "********** Setup complete **********"
