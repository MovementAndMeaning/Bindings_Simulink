/*  File    : yarpReadSHORE_sfun.cpp  */

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include "mex.h"
#include <Windows.h>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  yarpReadSHORE_sfun


/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

int iNrOutDims;


#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))
#define IS_PARAM_CARRAY(pVal) (mxIsChar(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && !mxIsDouble(pVal))

/*====================*
 * S-function methods *
 *====================*/

/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S)
{

  const mxArray *pVal0 = ssGetSFcnParam(S,0);

  //  if ( !IS_PARAM_DOUBLE(pVal0)) {
  if ( !IS_PARAM_CARRAY(pVal0)) {
    //    ssSetErrorStatus(S, "Parameter to S-function must be a double scalar");
    ssSetErrorStatus(S, "Parameter to S-function must be a text");
    return;
  } 
}


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{

  ssSetNumSFcnParams(S, 2);  /* Number of expected parameters */
  
  // Parameter mismatch will be reported by Simulink
  if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != NULL) {
      return;
    }
  } else {
    return; /* Parameter mismatch will be reported by Simulink */
  }

  ssSetSFcnParamTunable(S, 0, 0);

  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);

  if (!ssSetNumInputPorts(S, 0)) return;
  //  ssSetInputPortDirectFeedThrough(S, 0, 1);
  
    
  if (!ssSetNumOutputPorts(S, 7)) return;
  ssSetOutputPortWidth(S, 0, 1);
  ssSetOutputPortWidth(S, 1, 1);
  ssSetOutputPortWidth(S, 2, 1);
  ssSetOutputPortWidth(S, 3, 1);
  ssSetOutputPortWidth(S, 4, 1);
  ssSetOutputPortWidth(S, 5, 1);
  ssSetOutputPortWidth(S, 6, 1);

	  
  ssSetNumSampleTimes(S, 1);

  ssSetNumPWork(S, 2); // reserve element in the pointers vector

  ssSetNumModes(S, 0); // to store a C++ object
  ssSetNumNonsampledZCs(S, 0);

  ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);

  ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S) {
  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetOffsetTime(S, 0, 0.0);
  ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */
#define MDL_START
static void mdlStart(SimStruct *S) {
  //      ssGetPWork(S)[0] = (void *) new counter; // store new C++ object in the pointers vector
  ssGetPWork(S)[0] = (void *) new yarp::os::Network();
  ssGetPWork(S)[1] = (void *) new yarp::os::BufferedPort<yarp::os::Bottle>();
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortIn = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1]; 

  //--  iNrOutDims = ssGetOutputPortWidth(S, 0);
  //-- mexPrintf("ouput width: %d\n", iNrOutDims);


#define LENGTH 100
  
  char_T buf01[LENGTH];
  mxGetString(ssGetSFcnParam(S, 0), buf01, LENGTH);

  char_T buf02[LENGTH];
  mxGetString(ssGetSFcnParam(S, 1), buf02, LENGTH);

  std::string strPortNameSender(buf01);
  std::string strPortNameReceiver(buf02);

  yarp::os::Network *yNetwork = (yarp::os::Network *) ssGetPWork(S)[0];
    if(yNetwork->exists(strPortNameReceiver.c_str())){
    mexErrMsgIdAndTxt("yarpReadSHORE:mdlStart", "Port already exists");
  } 

  
  mexPrintf("opening port: %s\n", strPortNameReceiver.c_str());

  yPortIn->open(strPortNameReceiver.c_str()); 

  Sleep(500);

  if(!yNetwork->connect(strPortNameSender.c_str(), strPortNameReceiver.c_str(), "udp")){
    std::string strMessage = "error connecting ports \"" + strPortNameSender + "\" to \"" + strPortNameReceiver + "\"";
    mexWarnMsgTxt(strMessage.c_str());
  }
  
  /* m-code:
     block.Dwork(1).Data = 0;
  */

      
}                                            

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid) {
  //    counter *c = (counter *) ssGetPWork(S)[0];   // retrieve C++ object from
  //    real_T  *y = ssGetOutputPortRealSignal(S,0); // the pointers vector and use
  //    y[0] = c->output();                          // member functions of the
  //    UNUSED_ARG(tid);                             // object


}                                                


#define MDL_UPDATE 
static void mdlUpdate(SimStruct *S, int_T tid) {
  /* message format:
     ((ID 0) (x 330) (y 331) (Gender Male) (Age 0.61) (Angry 0.0) (Happy 0.0) (Sad 0.0) (Surprised 0.0))
  */
   
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortIn = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1];
  yarp::os::Bottle *bottleIn = yPortIn->read(false); // shouldwait = false
  if(bottleIn != NULL) {
    //#define DEBUG
#ifdef DEBUG    
    mexPrintf("Receiving: #%s#\n", bottleIn->toString());
    std::string strNull = std::string("is NULL: ") + std::string((bottleIn == NULL ? "yes": "no")) + std::string("\n");
    mexPrintf(strNull.c_str());
#endif
    
    real_T *y = ssGetOutputPortRealSignal(S, 0);
    int iBS = bottleIn->size();

    if(iBS > 0){

      real_T *rx = ssGetOutputPortRealSignal(S, 0);
      real_T *ry = ssGetOutputPortRealSignal(S, 1);
      real_T *rAge = ssGetOutputPortRealSignal(S, 2);
      real_T *rAngry = ssGetOutputPortRealSignal(S, 3);
      real_T *rHappy = ssGetOutputPortRealSignal(S, 4);
      real_T *rSad = ssGetOutputPortRealSignal(S, 5);
      real_T *rSurprised = ssGetOutputPortRealSignal(S, 6);
	    
      yarp::os::Value item = bottleIn->get(0);
      mexPrintf("Receiving: #%s#\n", item.toString());
      *rx = item.find("x").asDouble();
      *ry = item.find("y").asDouble();
      *rAge = item.find("Age").asDouble();
      *rAngry = item.find("Angry").asDouble();
      *rHappy = item.find("Happy").asDouble();
      *rSad = item.find("Sad").asDouble();
      *rSurprised = item.find("Surprised").asDouble();
    }
  }
  

  
  
} 


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S) {
  
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortIn = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1]; 
  yPortIn->close();
    
  yarp::os::Network *yNetwork = (yarp::os::Network *) ssGetPWork(S)[0]; 
  yNetwork->fini();
  delete yNetwork;
  delete yPortIn;
    
}                                              
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

