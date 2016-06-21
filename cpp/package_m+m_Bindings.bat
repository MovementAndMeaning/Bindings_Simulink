set SRC_DIR=C:\Users\Ulysses\Dropbox\+SH_Code\m+m_Bindings_Simulink_github\cpp
set ARCH=w64
set YARP_DIR=C:\src\yarp-git_VS15_64bit
set ACE_DIR=C:\src\ACE-6.3.3_64bit

mkdir m+m_Bindings
cd m+m_Bindings
copy "%SRC_DIR%\yarpReadSHORE_sfun.mex%ARCH%" .
copy "%SRC_DIR%\yarpReadVarVector_sfun.mex%ARCH%" .
copy "%SRC_DIR%\yarpReadVars_sfun.mex%ARCH%" .
copy "%SRC_DIR%\yarpSendText_sfun.mex%ARCH%" .
copy "%SRC_DIR%\yarpSendDicts_sfun.mex%ARCH%" .
copy "%SRC_DIR%\yarpReadDicts_sfun.mex%ARCH%" .
copy "%SRC_DIR%\mnmBlocks.slx" .
copy "%SRC_DIR%\m+m.png" .
copy "%SRC_DIR%\m+m_border.png" .
copy "%SRC_DIR%\encStr2Arr.m" .
copy "%SRC_DIR%\slblocks.m" .
copy "%SRC_DIR%\INSTALL.txt" .
copy "%YARP_DIR%\bin\Release\YARP_dev.dll" .
copy "%YARP_DIR%\bin\Release\YARP_init.dll" .
copy "%YARP_DIR%\bin\Release\YARP_OS.dll" .
copy "%YARP_DIR%\bin\Release\YARP_sig.dll" .
copy "%ACE_DIR%\lib\ACE.lib" .
REM copy "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/winmm.lib" .
cd ..

REM echo %date% | sed s/-//g | awk '{print "m+m_Bindings_"$1".zip"}' > tmp
REM set powershell get-date -format "{yyyMMdd}"
for /f %%i in ('powershell get-date -format "{yyyMMdd}"') do set MYDATE=%%i

REM echo "m+m_Bindings_%ARCH%_"%DATE:/=%.zip > tmp #does not work if %DATE% returns weekday e.g. sat 2016/05/21
echo m+m_Bindings_%ARCH%_%MYDATE% > tmp
set /p filename=<tmp
set ZIP_EXE="C:\Program Files (x86)\GnuWin32\bin\zip.exe"
%ZIP_EXE% -r %filename% m+m_Bindings/

REM rmdir /S /Q m+m_Bindings
del tmp


C:\src\yarp-git_VS15_64bit\lib\Release\YARP_dev.dll

