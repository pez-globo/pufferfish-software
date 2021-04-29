#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

# query git for the latest commit
latest_commit_hash=$(git rev-parse --short --verify HEAD)
tag=$(git tag --contains $latest_commit_hash)

if [ -z $tag ]
then
  version_number=$latest_commit_hash
else
  version_number=$tag
fi

export REACT_APP_VERSION=$version_number
