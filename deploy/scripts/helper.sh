#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs
backend_dir=$script_dir/../../backend/ventserver
backend_file=$(realpath $backend_dir)/simulator.py
frontend_dir=$script_dir/../../frontend

# query git for the latest commit
latest_commit_hash=$(git rev-parse --short --verify HEAD)
tag=$(git tag --contains $latest_commit_hash)

if [ -z $tag ]
then
  version_number=$latest_commit_hash
else
  version_number=$tag
fi

# Function to exit script on failure
function exit_script {
  echo -e "${ERROR} $1 ${NC}"
  exit 1
}
