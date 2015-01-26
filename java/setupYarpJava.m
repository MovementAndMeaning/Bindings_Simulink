% Augment search path for native method libraries by creating an ASCII text
% file named javalibrarypath.txt  in your preferences folder
% get preference dir: % prefdir

%adding the folder so we find LoadYarp
javaaddpath C:\src\yarp-git_VS10\bindings\build\src

% adding the yarp.jar:
javaaddpath('C:\src\yarp-git_VS10\bindings\build\src\yarp.jar')

%jdbc lib
javaaddpath('C:\Users\Ulysses\Dropbox\AffectiveCharacters_Implementation\Controllers\ARP_Simulink\sqlite-jdbc-3.7.2.jar')

%now this works:
%LoadYarp
%cd C:\src\yarp-git_VS10\example\matlab

cd C:\Users\Ulysses\Dropbox\AffectiveCharacters_Implementation\Controllers\ARP_Simulink