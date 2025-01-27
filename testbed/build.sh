#!/bin/bash
set echo on

mkdir -p ../bin

# get a list of all c files
cFilenames=$(find . -type f -name "*.c")

# echo "files: "$cFilenames

assembly="testbed"
compilerFlags="-g -fdeclspec -fPIC"
# -fms-extensions
# -Wall -Werror
includeFlags="-Isrc -I../engine/src/"
linkerFlags="-L../bin/ -lengine -Wl,-rpath,."
defines="-D_DEBUG -DZIMPORT"

echo "Building $assembly..."
echo clang $cFilenames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
clang $cFilenames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
