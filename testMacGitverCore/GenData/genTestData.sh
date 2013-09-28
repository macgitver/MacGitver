#!/bin/bash

A="Frida Fridoline <fridoline@call.me>"

cd $(dirname $0)/..
rm -rf Data
mkdir Data
cd Data
base_dir=$PWD
echo $base_dir

mkdir SimpleRepo1
cd SimpleRepo1
git init
echo "File1" >File1
git add File1
git commit File1 -m"First file" --author "$A"


