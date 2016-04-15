/*  File    : yarpReadDicts_sfun.cpp  */

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <list>
#include "mex.h"
#include <Windows.h>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  yarpReadDicts_sfun


/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

int iNrOutDims;
std::list<std::string> lstMsgFormat;

//#define DEBUG

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
static void mdlInitializeSampleTimes(SimStruct *S) {
  //  ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
  ssSetSampleTime(S, 0, 0.001);
  ssSetOffsetTime(S, 0, 0.0);
  //is this needed:  ssSetModelReferenceSampleTimeDefaultInheritance(S);
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

  iNrOutDims = ssGetOutputPortWidth(S, 0);
  mexPrintf("ouput width: %d\n", iNrOutDims);


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


  //--------------------
  
  yarp::os::Network *yNetwork = (yarp::os::Network *) ssGetPWork(S)[0];
  if(yNetwork->exists(strPortNameReceiver.c_str())){
    mexErrMsgIdAndTxt("yarpReadDicts:mdlStart", "Port already exists");
  } 
  
  
#ifdef MEX 
  mexPrintf("opening_ port: %s\n", strPortNameReceiver.c_str());
#endif

  if(!yPortIn->open(strPortNameReceiver.c_str())){
     mexErrMsgIdAndTxt("yarpReadDicts:mdlStart", "Error opening port");
  } 

  //--------------------
  
  Sleep(500);

  if(!yNetwork->connect(strPortNameSender.c_str(), strPortNameReceiver.c_str(), "udp")){
#ifdef MEX 
    std::string strMessage = "error connecting ports \"" + strPortNameSender + "\" to \"" + strPortNameReceiver + "\"";
    mexWarnMsgTxt(strMessage.c_str());
#endif    
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
  
  yarp::os::BufferedPort<yarp::os::Bottle> *yPortIn = (yarp::os::BufferedPort<yarp::os::Bottle> *) ssGetPWork(S)[1];
  yarp::os::Bottle *bottleIn = yPortIn->read(false); // shouldwait = false
  if(bottleIn != NULL) {
    //#define DEBUG
#ifdef DEBUG    
//    mexPrintf("Receiving: #%s#\n", bottleIn->toString());
//    std::string strNull = std::string("is NULL: ") + std::string((bottleIn == NULL ? "yes": "no")) + std::string("\n");
//    mexPrintf(strNull.c_str());
#endif
    
    real_T *y = ssGetOutputPortRealSignal(S, 0);

#ifdef DEBUG	    
    mexPrintf("bottle size: %d\n", bottleIn->size());
#endif
    
    if(bottleIn->size()==1){
      yarp::os::Value item = bottleIn->get(0);
      if(item.isDict()){
#ifdef DEBUG	    
	mexPrintf("DICT\n");
#endif	
	int ii=0;
	for (auto&& token : lstMsgFormat) {
	  yarp::os::Value &val =  item.find(token);
	  if(!val.isNull()){
#ifdef DEBUG	    
	    mexPrintf("%s: %f\n", token, val.asDouble());
#endif
	    if(ii<iNrOutDims){
	      y[ii] = val.asDouble();
	    }
	  } else {
#ifdef DEBUG	    
	    mexPrintf("not found\n");
#endif
	  }
	  ii++;
	}
      }
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
  yPortIn->interrupt();
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

