
#pragma once

#include <sbgEComLib.h>
#include <time.h>

#include <windows.h>

class SbgManager
{
public:
	SbgManager();
	~SbgManager();

	friend	SbgErrorCode onLogReceived(SbgEComHandle *pHandle, SbgEComCmdId logCmd,
		const SbgBinaryLogData *pLogData, void *pUserArg);

	bool	initialize();
	bool	release();

	bool	run();
	bool	GetStoppingState();

protected:

	// Start the listener thread
	LONG StartListener(void);

	// Stop the listener thread. Because the other thread might be
	// busy processing data it might take a while, so you can specify
	// a time-out.
	LONG StopListener(DWORD dwTimeout = INFINITE);

	static DWORD WINAPI ThreadProc(LPVOID lpArg);
	DWORD ThreadProc(void);

private:
	// sbg
	SbgEComHandle			comHandle;
	SbgErrorCode			errorCode;
	SbgInterface			sbgInterface;
	int32					retValue = 0;
	SbgEComDeviceInfo		deviceInfo;

	// threading
	LONG	m_lLastError;		// Last serial error
	bool	m_fStopping;
	HANDLE	m_hThread;

};

