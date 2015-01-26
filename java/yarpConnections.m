LoadYarp;
import yarp.Network;

%Network.connect('/testSender','/matlab/read');
Network.connect('/SmartBody/out','/matlab/read');