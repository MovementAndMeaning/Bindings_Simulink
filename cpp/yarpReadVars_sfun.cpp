/*  File    : yarpReadVars_sfun.cpp  */

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
#define S_FUNCTION_NAME  yarpReadVars_sfun


/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"


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

  if (!ssSetNumInputPorts(S, 0)) return;
  //  ssSetInputPortDirectFeedThrough(S, 0, 1);
  
    
  if (!ssSetNumOutputPorts(S, 1)) return;
  ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);

  
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
static void mdlInitializeSampleTimes(SimStruct *S)
{
  ssSetSampleTime(S, 0, mxGetScalar(ssGetSFcnParam(S, 0)));
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

  

  std::string strPortNameWrite(buf01);
  std::string strPortNameRead(buf02);

  mexPrintf("Opening port: #%s#\n", strPortNameRead.c_str());

  yPortOut->open(strPortNameRead.c_str()); 

  Sleep(1);
  yarp::os::Network *yNetwork = (yarp::os::Network *) ssGetPWork(S)[0]; 
  yNetwork->connect(strPortNameWrite.c_str(), strPortNameRead.c_str());
  
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

  yarp::os::BufferedPort<yarp::os::Bottle> *yPortOut = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1]; 
  yarp::os::Bottle *bottleIn = yPortOut->read();
  mexPrintf("Receiving: #%s#\n", bottleIn->toString());

  for (int bb=0;bb<(bottleIn->size()-1);bb++){
    yarp::os::Value item = bottleIn->get(bb);
    std::string strKey = item.asList()->get(0).asString();
    double fValue = item.asList()->get(1).asDouble();
    // if strcmp(strKey,strVarName){
    //     val = fValue;
    //   }

    real_T *y = ssGetOutputPortRealSignal(S, 0);
    y[0] = fValue;
    
  }
  
    
  
} 

//--/* For now mdlG[S]etSimState are only supported in normal simulation */
//--/* Define to indicate that this S-Function has the mdlG[S]etSimState mothods */
//--#define MDL_SIM_STATE
//--
//--
//--/* Function: mdlGetSimState =====================================================
//-- * Abstract:
//-- *
//-- */
//--static mxArray* mdlGetSimState(SimStruct* S)
//--{
//--  //    counter* c = (counter*) ssGetPWork(S)[0];
//--  mxArray* outSS = mxCreateDoubleMatrix(1,1,mxREAL);
//--  //  mxGetPr(outSS)[0] = c->getX();
//--  return outSS;
//--}
//--/* Function: mdlGetSimState =====================================================
//-- * Abstract:
//-- *
//-- */
//--static void mdlSetSimState(SimStruct* S, const mxArray* ma)
//--{
//--  //    counter* c = (counter*) ssGetPWork(S)[0];
//--  //   c->setX(mxGetPr(ma)[0]);
//--}



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
  
  // counter *c = (counter *) ssGetPWork(S)[0]; 
  // delete c;

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

