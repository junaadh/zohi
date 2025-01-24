@ECHO OFF
REM Build all

ECHO "Building all..."

PUSHD engine
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && exit)

PUSHD testbed
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo ERROR:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully..."