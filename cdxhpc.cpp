/////////////////////////////////////////////////////////////////////
// Project Name:   [ CDX Class Library - CDX.lib ]
// Source File:    [ CDX High Performance Counter Implementation]
// Author:         [ Bil Simser - bsimser@home.com ]
// Contributions:  [ Michael Rich ]
// Revision:       [ 1.99b ]
/////////////////////////////////////////////////////////////////////

#include "cdxhpc.h"


/////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////
CDXHPC::CDXHPC()
{
	// init to zero
	freq.QuadPart = 0;
	restart.QuadPart = 0;
}

/////////////////////////////////////////////////////////////////////
// starts the counter
/////////////////////////////////////////////////////////////////////
BOOL CDXHPC::Start()
{
	QueryPerformanceCounter(&restart);
	if (!QueryPerformanceFrequency(&freq)) return FALSE;
	// convert it down to milliseconds
	freq.QuadPart /= 1000;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// returns the current # of ms since counter was started
/////////////////////////////////////////////////////////////////////
DWORD CDXHPC::GetValue()
{
    LARGE_INTEGER timer;
    QueryPerformanceCounter(&timer);
    return (DWORD)((timer.QuadPart - restart.QuadPart) / freq.QuadPart);
}

/////////////////////////////////////////////////////////////////////
// waits for a specified ms since the counter was started
// note that if its already been that long since it was started
// the function would exit immediately
/////////////////////////////////////////////////////////////////////
void CDXHPC::Wait(DWORD dwMilliSecs)
{
    while (TRUE)
	{
		if (GetValue()>=dwMilliSecs) break;
	}
}

/////////////////////////////////////////////////////////////////////
// resets the counter back to zero
/////////////////////////////////////////////////////////////////////
void CDXHPC::Reset()
{
    LARGE_INTEGER timer, excess;
    QueryPerformanceCounter(&timer);
    excess.QuadPart = (timer.QuadPart - restart.QuadPart) % freq.QuadPart;
    QueryPerformanceCounter(&restart);
    restart.QuadPart -= excess.QuadPart;
}