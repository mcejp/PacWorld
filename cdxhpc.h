#ifndef _H_CDXHPC
#define _H_CDXHPC

#define WIN32_EXTRA_LEAN

#include "stdafx.h"

class CDXHPC
{
public:
	// constructor
	CDXHPC();
	// destructor
	virtual ~CDXHPC() {};

	// starts the counter
	BOOL Start();
	// returns the current # of ms since counter was started
	DWORD GetValue();
	// waits for a specified ms since the counter was started
	// note that if its already been that long since it was started
	// the function would exit immediately
	void Wait(DWORD dwWaitms);
	// resets the counter back to zero
	void Reset();

protected:
	// counter variables
	LARGE_INTEGER freq, restart;
};

#endif		/* #ifndef _H_CDXHPC */
