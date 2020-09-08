#!/bin/bash

mkdir $1-cppcheck-build-dir
cppcheck --project=$1.cppcheck --inline-suppr --enable=all --error-exitcode=1
