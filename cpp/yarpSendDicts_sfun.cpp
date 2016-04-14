/*  File    : yarpSendDicts_sfun.cpp
 *  Abstract:
 *
 *      Example of an C++ S-function which stores an C++ object in
 *      the pointers vector PWork.
 *
 *  Copyright 1990-2013 The MathWorks, Inc.
 */

//#define DEBUG

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <list>
#ifdef MEX 
#include "mex.h"
#endif     
#include <Windows.h>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  yarpSendDicts_sfun

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

std::list<std::string> lstMsgFormat;

std::string int_array_to_string(int int_array[], int size_of_array) {
  std::ostringstream oss("");
  for (int temp = 0; temp < size_of_array; temp++){
    oss << (char)int_array[temp];
  }
  return oss.str();
}

std::string double_array_to_string(InputRealPtrsType double_array, int size_of_array) {
  std::ostringstream oss("");
  for (int temp = 0; temp < size_of_array; temp++){
    oss << (char)*double_array[temp];
  }
  return oss.str();
}


#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))
#define IS_PARAM_CARRAY(pVal) (mxIsChar(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && !mxIsDouble(pVal))

/*====================*
 * S-function methods *
 *====================*/

/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S) {

  const mxArray *pVal0 = ssGetSFcnParam(S,0);

#ifdef MEX 
  //  if ( !IS_PARAM_DOUBLE(pVal0)) {
  if ( !IS_PARAM_CARRAY(pVal0)) {
    //    ssSetErrorStatus(S, "Parameter to S-function must be a double scalar");
    ssSetErrorStatus(S, "Parameter to S-function must be a text");
    return;
  }
#endif     

}


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S) {

  ssSetNumSFcnParams(S, 3);  /* Number of expected parameters */
  
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

  if (!ssSetNumInputPorts(S, 1)) return;
  ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
  ssSetInputPortDirectFeedThrough(S, 0, 1);
  
    
  if (!ssSetNumOutputPorts(S, 0)) return;
  //not needed?    ssSetOutputPortWidth(S, 0, 1);

  ssSetNumSampleTimes(S, 1);
  //--  ssSetNumRWork(S, 0);
  //--  ssSetNumIWork(S, 0);

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
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortOut = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1]; 


#define LENGTH 100
  
  
  char_T buf01[LENGTH];
  mxGetString(ssGetSFcnParam(S, 0), buf01, LENGTH);

  char_T buf02[LENGTH];
  mxGetString(ssGetSFcnParam(S, 1), buf02, LENGTH);


  std::string strPortNameSender(buf01);
  std::string strPortNameReceiver(buf02);

  char_T buf03[LENGTH];
  mxGetString(ssGetSFcnParam(S, 2), buf03, LENGTH);
  char * pch;
  const char s[2] = ";";  
  pch = strtok (buf03,s);
  while (pch != NULL){
    lstMsgFormat.push_back(pch);
    pch = strtok (NULL, s);
  }

  yarp::os::Network *yNetwork = (yarp::os::Network *) ssGetPWork(S)[0];
  if(yNetwork->exists(strPortNameSender.c_str())){
    mexErrMsgIdAndTxt("yarpSendDicts:mdlStart", "Port already exists");
  } 

    
#ifdef MEX 
  mexPrintf("writing to port: %s\n", strPortNameSender.c_str());
#endif
  yPortOut->open(strPortNameSender.c_str()); 

  Sleep(500);
  if(!yNetwork->connect(strPortNameSender.c_str(), strPortNameReceiver.c_str(), "udp")){
    std::string strMessage = "error connecting ports \"" + strPortNameSender + "\" to \"" + strPortNameReceiver + "\"";
#ifdef MEX 
    mexWarnMsgTxt(strMessage.c_str());
#endif
  }
  
     
}                                            

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid) {
  
}                                                


#define MDL_UPDATE 
static void mdlUpdate(SimStruct *S, int_T tid) {

    UNUSED_ARG(tid); /* not used in single tasking mode */

    

    
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortOut = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1]; 
  yarp::os::Bottle& bottleOut = yPortOut->prepare(); 
  bottleOut.clear();
  yarp::os::Property &prop = bottleOut.addDict();

  InputRealPtrsType  uPtrs = ssGetInputPortRealSignalPtrs(S,0);
  //  std::string strMsg = double_array_to_string(uPtrs, 2048);
  //  mexPrintf("Sending: #%s#\n", strMsg.c_str());

  
  std::string token = "";
  std::list<std::string>::iterator it = lstMsgFormat.begin();
 
  int_T nu = ssGetInputPortWidth(S,0);
  for (int_T j = 0; j < nu; j++) {
    if(j<lstMsgFormat.size()){
      //      token = lstMsgFormat[j];
      token = it->c_str();
      it++;
    } else {
      token = "#";
    }
    
#ifdef DEBUG    
    mexPrintf("Sending: %s %f\n", token, *uPtrs[j]);
#endif
    prop.put(token, *uPtrs[j]);
  }


#ifdef DEBUG      
  mexPrintf("--------------------\n");  
#endif 
  yPortOut->write();           
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

