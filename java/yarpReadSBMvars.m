% Copyright: (C) 2010 RobotCub Consortium
% Authors: Lorenzo Natale
% CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


LoadYarp;
import yarp.Port;
import yarp.Bottle;
import yarp.Network;


global yportReadSMB;

done=0;

yportReadSMB=Port;
%first close the port just in case
yportReadSMB.close;

disp('Going to open port /matlab/read');
yportReadSMB.open('/matlab/read');

yarpConnections

ii = 0
bottleIn=Bottle;
while(ii<30)
  yportReadSMB.read(bottleIn);
  %disp(bottleIn);
  for bb=0:(bottleIn.size-1)
    item = bottleIn.get(bb);
    strKey = char(item.asList().get(0).asString());
    fValue = item.asList().get(1).asDouble();
    eval(sprintf('global %s;', strKey))
    eval(sprintf('%s=%d', strKey, fValue))
%    eval(sprintf('disp(%s)', strKey));
  end
  ii = ii + 1;
end

yportReadSMB.close;
  
  
  
