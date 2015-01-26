function yarpSendBML_sfun(block)
%MSFUNTMPL_BASIC A Template for a Level-2 MATLAB S-Function
%   The MATLAB S-function is written as a MATLAB function with the
%   same name as the S-function. Replace 'msfuntmpl_basic' with the 
%   name of your S-function.
%
%   It should be noted that the MATLAB S-function is very similar
%   to Level-2 C-Mex S-functions. You should be able to get more
%   information for each of the block methods by referring to the
%   documentation for C-Mex S-functions.
%
%   Copyright 2003-2010 The MathWorks, Inc.

%%
%% The setup method is used to set up the basic attributes of the
%% S-function such as ports, parameters, etc. Do not add any other
%% calls to the main body of the function.
%%
setup(block);

%endfunction

%% Function: setup ===================================================
%% Abstract:
%%   Set up the basic characteristics of the S-function block such as:
%%   - Input ports
%%   - Output ports
%%   - Dialog parameters
%%   - Options
%%
%%   Required         : Yes
%%   C-Mex counterpart: mdlInitializeSizes
%%
function setup(block)
disp('setup')

%% Register dialog parameters
block.NumDialogPrms  = 2;

% Register number of ports
block.NumInputPorts  = 2;
block.NumOutputPorts = 0;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties
block.InputPort(1).Dimensions        = 1;
block.InputPort(1).DatatypeID  = 0;  % 2 for 'int8',
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;

block.InputPort(1).Dimensions        = 1;
block.InputPort(1).DatatypeID  = 0;  % 2 for 'int8',
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;

% Override output port properties
%block.OutputPort(1).Dimensions       = 1;
%block.OutputPort(1).DatatypeID  = 0; % double
%block.OutputPort(1).Complexity  = 'Real';

% Register sample times
%  [0 offset]            : Continuous sample time
%  [positive_num offset] : Discrete sample time
%
%  [-1, 0]               : Inherited sample time
%  [-2, 0]               : Variable sample time
block.SampleTimes = [-1, 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'CustomSimState',  < Has GetSimState and SetSimState methods
%    'DisallowSimState' < Error out when saving or restoring the model sim state
block.SimStateCompliance = 'DefaultSimState';

%% -----------------------------------------------------------------
%% The MATLAB S-function uses an internal registry for all
%% block methods. You should register all relevant methods
%% (optional and required) as illustrated below. You may choose
%% any suitable name for the methods and implement these methods
%% as local functions within the same file. See comments
%% provided for each function for more information.
%% -----------------------------------------------------------------

block.RegBlockMethod('PostPropagationSetup',    @DoPostPropSetup);
block.RegBlockMethod('InitializeConditions', @InitializeConditions);
block.RegBlockMethod('Start', @Start);
block.RegBlockMethod('Outputs', @Outputs);     % Required
block.RegBlockMethod('Update', @Update);
block.RegBlockMethod('Derivatives', @Derivatives);
block.RegBlockMethod('Terminate', @Terminate); % Required



%end setup

%%
%% PostPropagationSetup:
%%   Functionality    : Setup work areas and state variables. Can
%%                      also register run-time methods here
%%   Required         : No
%%   C-Mex counterpart: mdlSetWorkWidths
%%
function DoPostPropSetup(block)
block.NumDworks = 2;
  
  block.Dwork(1).Name            = 'x1';
  block.Dwork(1).Dimensions      = 1;
  block.Dwork(1).DatatypeID      = 0;      % double
  block.Dwork(1).Complexity      = 'Real'; % real
  block.Dwork(1).UsedAsDiscState = true;

  block.Dwork(2).Name            = 'portNr';
  block.Dwork(2).Dimensions      = 1;
  block.Dwork(2).DatatypeID      = 2;      % double
  block.Dwork(2).Complexity      = 'Real'; % real
  block.Dwork(2).UsedAsDiscState = true;
  
%%
%% InitializeConditions:
%%   Functionality    : Called at the start of simulation and if it is 
%%                      present in an enabled subsystem configured to reset 
%%                      states, it will be called when the enabled subsystem
%%                      restarts execution to reset the states.
%%   Required         : No
%%   C-MEX counterpart: mdlInitializeConditions
%%
function InitializeConditions(block)
disp('InitializeConditions')
%end InitializeConditions


%%
%% Start:
%%   Functionality    : Called once at start of model execution. If you
%%                      have states that should be initialized once, this 
%%                      is the place to do it.
%%   Required         : No
%%   C-MEX counterpart: mdlStart
%%
function Start(block)
disp('Start')

block.Dwork(2).Data = int8(randi([0 100], 1,1));
strYPortName = sprintf('port%03d', block.Dwork(2).Data);

disp('strYPortName')
disp(strYPortName)

LoadYarp;
import yarp.Port;
import yarp.Bottle;
import yarp.Network;

%global yportSendBML;  REPLACED BY
eval(sprintf('global %s;', strYPortName));

%yportSendBML=yarp.Port; REPLACED BY
eval(sprintf('%s=yarp.Port;', strYPortName));

strPortNameWrite = block.DialogPrm(1).Data;
strPortNameRead = block.DialogPrm(2).Data;

assert (size(strPortNameWrite,2)<16)
assert (size(strPortNameRead,2)<16)

disp('opening port')

%yportSendBML.open(strPortNameWrite); REPLACED BY
eval(sprintf('%s.open(strPortNameWrite);', strYPortName));

pause(1);
Network.connect(strPortNameWrite,strPortNameRead);

block.Dwork(1).Data = 0;

%end Start

%%
%% Outputs:
%%   Functionality    : Called to generate block outputs in
%%                      simulation step
%%   Required         : Yes
%%   C-MEX counterpart: mdlOutputs
%%
function Outputs(block)

%block.OutputPort(1).Data = block.Dwork(1).Data + block.InputPort(1).Data;

%end Outputs

%%
%% Update:
%%   Functionality    : Called to update discrete states
%%                      during simulation step
%%   Required         : No
%%   C-MEX counterpart: mdlUpdate
%%
function Update(block)
strYPortName = sprintf('port%03d', block.Dwork(2).Data);

%global yportSendBML; REPLACED BY
eval(sprintf('global %s;', strYPortName))

%block.InputPort(1).Data -> message type
%block.InputPort(1).Data -> message parameters

msgType = block.InputPort(2).Data;
msg = '';
if msgType> 1
    disp(msgType)
end

switch msgType,
    case 0,
        msg = 'bml0';
    case 1,
        msg = 'bml1';
    case 2,
        msg = 'bml2';
    case 3,
        msg = 'bml3';
end
    
%disp('sending')
disp(msg)
b=yarp.Bottle;
b.addString(msg);
%yportSendBML.write(b); REPLACED BY
%disp(strYPortName);
eval(sprintf('%s.write(b);', strYPortName));

block.Dwork(1).Data = block.InputPort(1).Data;

%end Update

%%
%% Derivatives:
%%   Functionality    : Called to update derivatives of
%%                      continuous states during simulation step
%%   Required         : No
%%   C-MEX counterpart: mdlDerivatives
%%
function Derivatives(block)

%end Derivatives

%%
%% Terminate:
%%   Functionality    : Called at the end of simulation for cleanup
%%   Required         : Yes
%%   C-MEX counterpart: mdlTerminate
%%
function Terminate(block)
disp('Terminate')

strYPortName = sprintf('port%03d', block.Dwork(2).Data);

%global yportSendBML; REPLACED BY
eval(sprintf('global %s;', strYPortName));


if exist(strYPortName)
    disp('closing port')
    disp(strYPortName)
%    yportSendBML.close; REPLACED BY
    eval(sprintf('%s.close;', strYPortName));

end
%end Terminate

