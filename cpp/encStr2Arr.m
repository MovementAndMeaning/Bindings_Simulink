function [ myarr2048 ] = encStr2Arr( mystr )
%coder.varsize('myarr2048', [2048 1], 2);
coder.varsize('myarr2048', [2048, 1], [0 0])
maxarrsize = 2048;
%mystr = 'this is a very long string that we want to send around to some other place on a different planet';
arr = double(mystr)';
ss = size(arr, 1);
padsize = maxarrsize - ss;
tt = zeros(2048, 1);
tt(1:ss, 1) = arr; %padarray would be the function, but crashes...
myarr2048 = tt(1:2048,1);
end

