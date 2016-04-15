mkdir mnmBindingSimulink
cd mnmBindingSimulink
copy ..\yarpReadSHORE_sfun.mexw32     .
copy ..\yarpReadVarVector_sfun.mexw32 .
copy ..\yarpReadVars_sfun.mexw32	   .
copy ..\yarpSendText_sfun.mexw32	   .
copy ..\encStr2Arr.m                  .
copy ..\m+m.png                  .
copy ..\m+m_border.png                  .
copy ..\mnmBlocks.slx                  .

cd ..
echo mnmBindingSimulink_%DATE:/=%.zip > tmp
set /p filename=<tmp
set ZIP_EXE="C:\Program Files (x86)\GnuWin32\bin\zip.exe"
%ZIP_EXE% -r %filename% mnmBindingSimulink/

