// VC++ Win32 C specific code
// compile: cl ftimevc.cpp user32.lib gdi32.lib
// or use or IDE

#include <windows.h> // has qpf and qpc
#include <iostream.h>

__int64 startclock;  // we want a start time
double rate_inv;

BOOL initftime(void) {
  __int64 rate;

  // we need the accuracy
  if(!QueryPerformanceFrequency((LARGE_INTEGER*)&rate)) {
    return FALSE; // win errors
  }

  // usually the rate will be 1193180
  if(!rate) {
    return FALSE;
  }

  rate_inv=1.0/(double)rate;

  if(!QueryPerformanceCounter((LARGE_INTEGER*)&startclock)) {
    return FALSE; // win errors
  }

  return TRUE; // there is a clock
}
// you would have to start up with initftime() at the beginning
// of the game. And check for errors
 
double ftime(void) {
  // by dividing by its rate you get accurate seconds

  __int64 endclock;

  QueryPerformanceCounter((LARGE_INTEGER*)&endclock);

  return (double)(endclock-startclock)*rate_inv;

  // note: I recommend that you multiply but the inverse of a constant.
  // (speed reasons)
}

// here is just a demo of the added frame times and frame rate.
main() {

  float frametime,framestart,frameend; // the vars really needed
  float first,fps,frametimes=0;

  if(!initftime()) { // start timer stuff
    cerr<<"initftime() failed"<<endl;
    return 1;
  }

  framestart=(float)ftime(); // get the first time
  first=framestart;

  // game loop
  do {
    do {
      // get the time till its not equal to start frame time, it should,
      // never loop at the rate of this timer, but if it does then
      // it will cap to the frame rate to the rate of this timer which
      // is 1193180 hz
      frameend=(float)ftime();  // get end of frame time
    } while(frameend==framestart);

    frametime=frameend-framestart; // find differnce to get the frametime
    framestart=frameend; // make the first time be the next time

    // the inverse of frame time is the frame rate
    fps=(float)1.0/frametime; // pretty cool huh

    // other game stuff
    cout<<"fps:"<<fps<<endl;

    frametimes+=frametime;

  } while(frameend-first<1.0f);

  cout<<"totaltime:"<<(frameend-first)<<endl;
  cout<<"frametimes:"<<frametimes<<endl;

  return 0;
}