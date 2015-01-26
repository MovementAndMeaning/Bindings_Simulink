function [ myarr256 ] = encStr2Arr( mystr )
%coder.varsize('myarr256', [256 1], 2);
coder.varsize('myarr256', [256, 1], [0 0])
maxarrsize = 256;
%mystr = 'this is a very long string that we want to send around to some other place on a different planet';
arr = double(mystr)';
ss = size(arr, 1);
padsize = maxarrsize - ss;
tt = zeros(256, 1);
tt(1:ss, 1) = arr; %padarray would be the function, but crashes...
myarr256 = tt(1:256,1);
end

