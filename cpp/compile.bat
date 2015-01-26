set LIBDIRS=-L"C:/src/yarp-2.3.22_VS10/lib/Release/" -LC:/src/ACE_wrappers/lib -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"
set LIBS=-lYARP_dev -lYARP_init -lYARP_OS -lYARP_sig -lACE -lwinmm
set INCL=-IC:\src\yarp-2.3.22_VS10\src\libYARP_OS\include -IC:\src\yarp-2.3.22_VS10\src\libyarpc -IC:\src\yarp-2.3.22_VS10\generated_include
set FLAGS=-g

REM rm yarpSendText_sfun.mexw32.manifest
REM "C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpSendText_sfun.cpp

rm yarpReadVars_sfun.mexw32.manifest
"C:\Program Files (x86)\MATLAB\R2014a\bin\mex.bat" %FLAGS%  %LIBDIRS% %LIBS% %INCL%  yarpReadVars_sfun.cpp
