REM set LIBDIRS=-L"C:/src/yarp-2.3.22_VS10/lib/Release/" -LC:/src/ACE_wrappers/lib -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"
set LIBDIRS=-L"C:/src/yarp-git_VS10/lib/Release/" -LC:/src/ACE_wrappers/lib -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"

set LIBS=-lYARP_dev -lYARP_init -lYARP_OS -lYARP_sig -lACE -lwinmm
REM set INCL=-IC:\src\yarp-2.3.22_VS10\src\libYARP_OS\include -IC:\src\yarp-2.3.22_VS10\src\libyarpc -IC:\src\yarp-2.3.22_VS10\generated_include
set INCL=-IC:\src\yarp-git_VS10\src\libYARP_OS\include -IC:\src\yarp-git_VS10\src\libyarpc -IC:\src\yarp-git_VS10\generated_include

set FLAGS=-g

rm yarpSendText_sfun.mexw32.manifest
call "C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpSendText_sfun.cpp

rm yarpReadVars_sfun.mexw32.manifest
call "C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadVars_sfun.cpp


rm yarpReadVarVector_sfun.mexw32.manifest
call "C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadVarVector_sfun.cpp


rm yarpReadSHORE_sfun.mexw32.manifest
call "C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadSHORE_sfun.cpp


