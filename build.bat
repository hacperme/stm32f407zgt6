@echo off

set cur_dir=%~dp0

set MAKE_PATH=%cur_dir%tools\make-3.81-bin\bin\make.exe
set GCC_PATH=%cur_dir%tools\gcc_arm_none_eabi\bin
set OPENOCD_PATH=%cur_dir%tools\OpenOCD-20211118-0.11.0\bin\openocd.exe
@REM set PATH=%PATH%;%GCC_PATH%;%MAKE_PATH%
@REM echo %PATH%

set parm_num=0
for %%a in (%*) do set /a parm_num+=1

if %parm_num% EQU 0 (
  set TARGET_NAME=all
) else (
  if %parm_num% EQU 1 (
    set TARGET_NAME=%1%
  ) else (
    echo ****************************
    echo usage: build.bat [target_name]
    echo build.bat
    echo build.bat all
    echo build.bat clean
    echo ****************************
    pause
    exit 1
  )
)

set totalcores=Unknown
for /F %%A in ('wmic cpu get numberofcores') do @if %%A GTR 0 set totalcores=%%A
set /a N=2*totalcores
echo NumberOfCores=%totalcores%, N=%N%

if "%TARGET_NAME%"=="all" (
  @REM rd /s /q out
	call %MAKE_PATH% GCC_PATH=%GCC_PATH% -j%N%
  if %ERRORLEVEL% EQU 0 goto cp_target
  goto error_ocurr
)

if "%TARGET_NAME%"=="clean" (
	call %MAKE_PATH% clean
  rd /s /q target
)

if "%TARGET_NAME%"=="flash" (
  @REM call %OPENOCD_PATH% -f openocd.cfg -c init -c halt -c "flash write_image erase target\\stm32f407zgt6.bin 0x08000000" -c reset -c shutdown
  call %OPENOCD_PATH% -f openocd.cfg -c "program target\\stm32f407zgt6.elf reset exit"
)

goto byebye

:cp_target


rd /s /q target
md target
copy /y out\*.elf target
copy /y out\*.hex target
copy /y out\*.bin target
copy /y out\*.map target
echo "### build success !###"
goto byebye

:error_ocurr

echo "### build error !###"


:byebye