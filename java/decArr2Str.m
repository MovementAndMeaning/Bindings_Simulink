function [ mystr ] = decArr2Str( myarr256 )
%decoding
mystr = char(myarr256(myarr256 ~= 0)');
end

