@echo off
echo Copying SFML DLL files...

if exist "bin\x64\Debug\BreakingBonds.exe" (
    echo Copying Debug DLLs...
    copy /Y "sfml\bin\sfml-graphics-d-2.dll" "bin\x64\Debug\" 2>nul
    copy /Y "sfml\bin\sfml-window-d-2.dll" "bin\x64\Debug\" 2>nul
    copy /Y "sfml\bin\sfml-system-d-2.dll" "bin\x64\Debug\" 2>nul
    echo DLL files copied to Debug folder
)

if exist "bin\x64\Release\BreakingBonds.exe" (
    echo Copying Release DLLs...
    copy /Y "sfml\bin\sfml-graphics-2.dll" "bin\x64\Release\" 2>nul
    copy /Y "sfml\bin\sfml-window-2.dll" "bin\x64\Release\" 2>nul
    copy /Y "sfml\bin\sfml-system-2.dll" "bin\x64\Release\" 2>nul
    echo DLL files copied to Release folder
)

if not exist "bin\x64\Debug\BreakingBonds.exe" (
    if not exist "bin\x64\Release\BreakingBonds.exe" (
        echo Warning: No executable found. Build the project first.
    )
)

pause

