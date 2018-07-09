
#pragma once

#include <sbgEComLib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>

#include "commonDefinition.h"

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

	void	outputLog(const char* message, SBG_LOG_TYPE	m_eType = SBG_LOG_TYPE_ANGLE, int type = LOG_TYPE_FILE);	// print to output window or status bar

	bool	SetParameter(int fps);
	bool	isInitialized();

	void	SetCountLines(int val)				{ m_lines = val; }
	void	SetDeviceScanRepeatBool(bool val)	{ m_bScanRepeat = val; }
	void	SetDeviceScanRepeatCount(int val)	{ m_nScanRepeat = val; }
	void	SetFlagValidGPS(bool val)			{ m_bValidGPS = val; }	

	bool	GetFlagValidGPS()		{ return m_bValidGPS; }

protected:

	// Start the listener thread
	LONG StartListener(void);

	// Stop the listener thread. Because the other thread might be
	// busy processing data it might take a while, so you can specify
	// a time-out.
	LONG StopListener(DWORD dwTimeout = INFINITE);

	static DWORD WINAPI ThreadProc(LPVOID lpArg);
	DWORD ThreadProc(void);

	std::string FormatTimeString();

	void scanRepeat( );

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

	// file 
	std::string	m_sFileNameLog;
	std::ofstream	m_pFileLog;		// 日志写文件
	std::ostringstream m_osCache[2];

	long	m_nLastClock;
	int		m_nFrameCurrent;

	int		m_fps;
	bool	m_bInitialized;

	int m_lines;	//推扫行数目
	bool	m_bScanRepeat;	// 重复行扫描开关 
	int		m_nScanRepeat;	// 重复行扫描次数
	int		m_nScanRepeatCurrent;	// 重复行扫描当前次数 

	bool	m_bValidGPS; 
};

