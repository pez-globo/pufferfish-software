#!/bin/bash

# Creates a build release for front-end with static files

# Importing logging colours, absolute paths and exit function
script_dir=$(dirname $(realpath $0))
. $script_dir/helper.sh

echo -e "\n${SUCCESS}********** Building frontend **********\n${NC}"
cd $frontend_dir
yarn build || exit_script "Could not create a release build"

echo -e "\n${SUCCESS}Yarn setup complete\n${NC}"