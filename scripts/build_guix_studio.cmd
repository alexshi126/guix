@echo off

rem Initialize the developer environment just like a developer box. Note that 'call' keyword that ensures that the script does not exist after 
rem calling the other batch file.
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64 -winsdk=10.0.22621.0

rem Set Python path. Not needed if installed with WinGet.
rem SET PATH=%PATH%;"C:\Program Files\Python36";"C:\Program Files\Python36\scripts"

rem Save working directory so that we can restore it back after building everything. This will make developers happy and then 
rem switch to the folder this script resides in. Don't assume absolute paths because on the build host and on the dev host the locations may be different.
pushd "%~dp0"

cd ..\test\guix_studio_test\test_demo
call python test_main.py -b

exit /B %ERRORLEVEL%
