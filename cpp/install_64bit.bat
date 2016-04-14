@echo OFF

set INSTALL_DIR=%USERPROFILE%\Documents\MATLAB

echo copying files to %INSTALL_DIR%
pause

copy mnmBlocks.slx                 %INSTALL_DIR%
copy yarpReadSHORE_sfun.mexw64     %INSTALL_DIR%
copy yarpReadVarVector_sfun.mexw64 %INSTALL_DIR%
copy yarpReadVars_sfun.mexw64	   %INSTALL_DIR%
copy yarpSendText_sfun.mexw64	   %INSTALL_DIR%
copy yarpSendDicts_sfun.mexw64	   %INSTALL_DIR%
copy yarpReadDicts_sfun.mexw64	   %INSTALL_DIR%
copy encStr2Arr.m                  %INSTALL_DIR% 


