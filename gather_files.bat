@echo off
setlocal enabledelayedexpansion

set "SEARCH_DIR=Source"
set "OUTPUT_FILE=filenames.txt"
set "PREFIX=${SOURCE_DIR}"
set "EXCLUDE_DIR=ThirdParty"
set "EXCLUDE_FILE=main.cpp"

(
    for /r "%SEARCH_DIR%" %%F in (*) do (
        set "FILE=%%F"
        set "RELPATH=!FILE:%CD%\=!"
        set "DIRNAME=!RELPATH:%SEARCH_DIR%\=!"
        set "FILENAME=!FILE:*%SEARCH_DIR%\=!"
        
        rem Check if the current file's directory contains "ThirdParty"
        echo !DIRNAME! | findstr /C:"%EXCLUDE_DIR%" > nul
        if errorlevel 1 (
            rem Check if the current file's name is "main.cpp"
            echo !FILENAME! | findstr /C:"%EXCLUDE_FILE%" > nul
            if errorlevel 1 (
                set "UNIXPATH=!RELPATH:\=/!"
                set "FINALPATH=!UNIXPATH:%SEARCH_DIR%=!"
                echo !PREFIX!!FINALPATH!
            )
        )
    )
) > "%OUTPUT_FILE%"

type "%OUTPUT_FILE%" | clip
del "%OUTPUT_FILE%"

endlocal
