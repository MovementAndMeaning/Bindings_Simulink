function yarpReadVars_sfun(block)
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
block.NumDialogPrms  = 3; %VARIABLE NAME(s)

% Register number of ports
block.NumInputPorts  = 0;
block.NumOutputPorts = 1;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties
%block.InputPort(1).Dimensions        = [2 1];
%block.InputPort(1).DatatypeID  = 0;  % 2 for 'int8',
%block.InputPort(1).Complexity  = 'Real';

% Direct feedthrough means that the output (or the variable sample time for 
% variable sample time blocks) is controlled directly by the value of an input port signal
% block.InputPort(1).DirectFeedthrough = true;


% Override output port properties
% will be the image number
block.OutputPort(1).Dimensions       = 1;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'sample';

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
block.NumDworks = 1;
  
  block.Dwork(1).Name            = 'x1';
  block.Dwork(1).Dimensions      = 1;
  block.Dwork(1).DatatypeID      = 0;      % double
  block.Dwork(1).Complexity      = 'Real'; % real
  block.Dwork(1).UsedAsDiscState = true;

  
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

LoadYarp;
import yarp.Port;
import yarp.Bottle;
import yarp.Network;
import yarp.BufferedPortBottle;


global yportRead;
%%% yportRead=Port;
yportRead=BufferedPortBottle;


%strPortNameWrite = '/testSender'
%strPortNameRead = '/simulink/in';

strPortNameWrite = block.DialogPrm(1).Data;
strPortNameRead = block.DialogPrm(2).Data;

assert (size(strPortNameWrite,2)<16)
assert (size(strPortNameRead,2)<16)


sprintf('Going to open port %s', strPortNameRead)
yportRead.open(strPortNameRead)

Network.connect(strPortNameWrite, strPortNameRead);

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
import yarp.Bottle;
import yarp.BufferedPortBottle;

global yportRead;

strVarName = block.DialogPrm(3).Data; %'var0';
val = -99999;

%bottleIn=Bottle;
%yportRead.read(bottleIn);

bottleIn=Bottle;
bottleIn = yportRead.read;

disp(bottleIn);
for bb=0:(bottleIn.size-1)
    item = bottleIn.get(bb);
    strKey = char(item.asList().get(0).asString());
    fValue = item.asList().get(1).asDouble();
    % eval(sprintf('global %s;', strKey))
    % eval(sprintf('%s=%d', strKey, fValue))
    % eval(sprintf('disp(%s)', strKey));
    if strcmp(strKey,strVarName)
        val = fValue;
    end
end


block.OutputPort(1).Data = val;


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

global yportRead;

if exist('yportRead', 'var')
     yportRead.close;
    
    
end
%end Terminate

