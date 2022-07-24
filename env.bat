@echo off

set cur_dir=%~dp0

set CMAKE_PATH=%cur_dir%tools\cmake\cmake-3.21.2-windows-i386\bin
set MAKE_PATH=%cur_dir%tools\make-3.81-bin\bin
rem set MINGW_PATH=%cur_dir%tools\mingw730_32
set TOOLCHAIN_PATH=%cur_dir%tools\gcc_arm_none_eabi\bin
set OPENOCD_PATH=%cur_dir%tools\OpenOCD-20211118-0.11.0\bin
rem set PYTHON_PATH=%cur_dir%tools\python-3.7.6rc1-embed-amd64
rem set PYTHON_SCRIPTS_PATH=%PYTHON_PATH%\Scripts
rem set PYTHON_LIB_PATH=%PYTHON_PATH%\Lib\site-packages
@REM set PATH=%CMAKE_PATH%;%TOOLCHAIN_PATH%;%OPENOCD_PATH%;%MAKE_PATH%;%PATH%
set PATH=%CMAKE_PATH%;%TOOLCHAIN_PATH%;%OPENOCD_PATH%;%MAKE_PATH%
echo %PATH%

