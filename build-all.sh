#!/bin/bash

set echo on

echo "Building everything..."

pushd engine
source build.sh
popd

errorlevel=$?
if [ $errorlevel -ne 0 ]; then
    echo "Error: "$errorlevel && exit
fi

pushd testbed
source build.sh
popd

errorlevel=$?
if [ $errorlevel -ne 0 ]; then
    echo "Error: "$errorlevel && exit
fi

echo "All assemblies built successfully..."
