@echo OFF
set YARP_DIR=C:\src\yarp-git_VS15_64bit
set MATLAB_DIR=C:\Program Files\MATLAB\R2016a
set ACE_DIR=C:/src/ACE-6.3.3_64bit
set WIN_SDK_DIR=C:\Program Files\Microsoft SDKs\Windows\v7.1A\

REM set LIBDIRS=-L"C:/src/yarp-2.3.22_VS10/lib/Release/" -LC:/src/ACE_wrappers/lib -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"
REM set LIBDIRS=-L"C:/src/yarp-git_VS10/lib/Release/" -LC:/src/ACE_wrappers/lib -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"

set LIBDIRS=-L"%YARP_DIR%/lib/Release/" -L"%ACE_DIR%/lib" -L"%WIN_SDK_DIR%\Lib"


set LIBS=-lYARP_dev -lYARP_init -lYARP_OS -lYARP_sig -lACE -lwinmm
REM set INCL=-IC:\src\yarp-2.3.22_VS10\src\libYARP_OS\include -IC:\src\yarp-2.3.22_VS10\src\libyarpc -IC:\src\yarp-2.3.22_VS10\generated_include
REM set INCL=-IC:\src\yarp-git_VS10\src\libYARP_OS\include -IC:\src\yarp-git_VS10\src\libyarpc -IC:\src\yarp-git_VS10\generated_include
set INCL=-I%YARP_DIR%\src\libYARP_OS\include -I%YARP_DIR%\src\libyarpc -I%YARP_DIR%\generated_include


set FLAGS=-g -DMEX


echo %LIBDIRS%
echo %LIBS%
echo %INCL%

del  yarpSendText_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpSendText_sfun.cpp

del  yarpSendDicts_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpSendDicts_sfun.cpp

del  yarpReadDicts_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadDicts_sfun.cpp

del yarpReadVars_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadVars_sfun.cpp

del yarpReadVarVector_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadVarVector_sfun.cpp

del yarpReadSHORE_sfun.mexw64.manifest
call "%MATLAB_DIR%\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadSHORE_sfun.cpp





