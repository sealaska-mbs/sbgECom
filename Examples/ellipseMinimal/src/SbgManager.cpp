
#include "SbgManager.h"

#include <iomanip>

SbgManager::SbgManager()
{
	initialize();
}

SbgManager::~SbgManager()
{
	release();
}

bool SbgManager::initialize()
{
	m_lines = 0;
	m_bScanRepeat = false;
	m_nScanRepeat = 0;
	m_nScanRepeatCurrent = 0;

	m_bInitialized = false;
	m_fps = 5;
	m_hThread = 0;
	m_fStopping = false;
	m_nLastClock = 0;
	m_nFrameCurrent = 0;
	m_sFileNameLog = "0000.sbg.log";

	m_pFileLog.open(m_sFileNameLog.c_str(), std::ios::out);

	//outputLog("latitude \t longitude \t altitude ", SBG_LOG_TYPE_ANGLE);
	//outputLog("yaw	\t\t roll	\t\t pitch ", SBG_LOG_TYPE_GPS);
	//
	// Create an interface: 
	// We can choose either a serial for real time operation, or file for previously logged data parsing
	// Note interface closing is also differentiated !
	//
	//errorCode = sbgInterfaceSerialCreate(&sbgInterface, "/dev/cu.usbserial-FTX2GE57", 921600);		// Example for Unix using a FTDI Usb2Uart converter
	errorCode = sbgInterfaceSerialCreate(&sbgInterface, "COM3", 921600);							// Example for Windows serial communication

	if ( errorCode != SBG_NO_ERROR )
	{
		//
		// Unable to create the interface
		//
		fprintf(stderr, "ellipseMinimal: Unable to create the interface.\n");
		return false;
	}

	errorCode = sbgEComInit(&comHandle, &sbgInterface);

	if (errorCode != SBG_NO_ERROR)
	{
		//
		// Unable to initialize the sbgECom
		//
		fprintf(stderr, "ellipseMinimal: Unable to initialize the sbgECom library.\n");
		return false;
	}

	//
	// Get device inforamtions
	//
	errorCode = sbgEComCmdGetInfo(&comHandle, &deviceInfo);

	//
	// Display device information if no error
	//
	if (errorCode == SBG_NO_ERROR)
	{
		;// printf("Device : %0.9u found\n", deviceInfo.serialNumber);
	}
	else
	{
		fprintf(stderr, "ellipseMinimal: Unable to get device information.\n");
		return false;
	}

	//
	// Display a message for real time data display
	//
	//printf("sbgECom properly Initialized.\n\nEuler Angles display with estimated standard deviation.\n");

	SbgEComReceiveFunc recFunc = onLogReceived;
	//
	// Define callbacks for received data
	//
	sbgEComSetReceiveCallback(&comHandle, recFunc, this);

	m_bInitialized = true;
	return true;

}

bool SbgManager::release()
{

	//
	// Close the sbgEcom library
	//
	sbgEComClose(&comHandle);

	return true;
}

bool SbgManager::run()
{
	if (!isInitialized())
		return false;

	m_fStopping = false;
	StartListener();

	return true;
}

bool SbgManager::GetStoppingState()
{
	return m_fStopping;
}

//----------------------------------------------------------------------//
//  Call backs                                                          //
//----------------------------------------------------------------------//

/*!
*	Callback definition called each time a new log is received.
*	\param[in]	pHandle									Valid handle on the sbgECom instance that has called this callback.
*	\param[in]	logCmd									Contains the binary received log command id.
*	\param[in]	pLogData								Contains the received log data as an union.
*	\param[in]	pUserArg								Optional user supplied argument.
*	\return												SBG_NO_ERROR if the received log has been used successfully.
*/
SbgErrorCode onLogReceived(SbgEComHandle *pHandle, SbgEComCmdId logCmd, const SbgBinaryLogData *pLogData, void *pUserArg)
{
	SbgManager* pSM = (SbgManager*)pUserArg;
	char msgLog[4096];
	//
	// Handle separately each received data according to the log ID
	//
	switch (logCmd)
	{

	case SBG_ECOM_LOG_EKF_EULER:
		//
		// Simply display euler angles in real time
		//
		sprintf(msgLog, "%3.1f\t\t%3.1f\t\t%3.1f ", 
			sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]));
		
		pSM->outputLog(msgLog, SBG_LOG_TYPE_ANGLE);

		break;

	case SBG_ECOM_LOG_GPS1_POS:
		//
		// Simply display euler angles in real time
		//
		sprintf(msgLog, "%f\t\t%f\t\t%f",
			pLogData->gpsPosData.latitude, pLogData->gpsPosData.longitude, pLogData->gpsPosData.altitude);

		pSM->outputLog(msgLog, SBG_LOG_TYPE_GPS);

		break;

	default:
		break;
	}

	return SBG_NO_ERROR;
}

LONG SbgManager::StartListener(void)
{
	// Check if the watcher thread was already running
	if (m_hThread == 0)
	{
		// Make sure the thread has stopped
		//_ASSERTE(!m_fStopping);

		// Start the watcher thread
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0, 0, ThreadProc, LPVOID(this), 0, &dwThreadId);

		if (m_hThread == 0)
		{
			// Set the error code and exit
			m_lLastError = ::GetLastError();
			return m_lLastError;
		}
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

LONG SbgManager::StopListener(DWORD dwTimeout)
{
	// Check if the thread is running
	if (m_hThread)
	{
		// Set the flag that the thread must be stopped
		m_fStopping = true;

		// Wait until the watcher thread has stopped
		::WaitForSingleObject(m_hThread, dwTimeout);

		// Close handle to the thread
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI SbgManager::ThreadProc(LPVOID lpArg)
{
	// Route the method to the actual object
	SbgManager* pThis = reinterpret_cast<SbgManager*>(lpArg);
	return pThis->ThreadProc();
}

DWORD SbgManager::ThreadProc(void)
{
	// Keep looping
	do
	{
				//
		// Try to read a frame
		//
		errorCode = sbgEComHandle(&comHandle);

		//
		// Test if we have to release some CPU (no frame received)
		//
		if (errorCode == SBG_NOT_READY)
		{
			//
			// Release CPU
			//
			sbgSleep(1);
		}
		else
		{
			fprintf(stderr, "Error\n");
		}

	} while (!m_fStopping);

	// Bye bye
	return 0;
}

std::string SbgManager::FormatTimeString()
{
	time_t t;
	struct tm * lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。
	std::ostringstream os;
	os /*<< lt->tm_year + 1900 << '/'
		<< lt->tm_mon << '/'
		<< lt->tm_mday << ' '
		<< lt->tm_hour << ':'
		<< lt->tm_min << ':'
		<< lt->tm_sec << ' '*/
		<< clock()/1000.0f << "\t\t";
	return std::string(os.str());
}

void SbgManager::scanRepeat()
{
	if (m_bScanRepeat && m_nScanRepeatCurrent < m_nScanRepeat)
	{
		m_nScanRepeatCurrent++;

		if (m_nScanRepeatCurrent < m_nScanRepeat)
		{
			m_nFrameCurrent = 0;

			// create new bil 
			std::ostringstream os;
			os << std::setw(4) << std::setfill('0') << m_nScanRepeatCurrent << ".sbg.log";

			m_pFileLog.open(os.str().c_str(), std::ios::out);

		}
		else
		{
			m_fStopping = true;
			outputLog("...持续行扫描结束！写入文件数");
		}

	}
	else
	{
		m_fStopping = true;
		outputLog("...一次性行扫描结束！");
	}
}

void SbgManager::outputLog(const char* message, SBG_LOG_TYPE m_eType, int type /*= LOG_TYPE_FILE */)
{

	//std::string stringTime = FormatTimeString();

	if (type & LOG_TYPE_FILE)
	{
		long msTime = clock(); 
		//if (abs(msTime - m_nLastClock) < 50)
		{
			if (m_eType & SBG_LOG_TYPE_ANGLE)
			{
				m_osCache[0] << m_nFrameCurrent << "\t\t";

				m_osCache[0] << message << "\t\t";
			}
			
			if (m_eType & SBG_LOG_TYPE_GPS)
			{
				m_osCache[1] << message << "\t\t";

				m_osCache[1] << clock() / 1000.0f << std::endl;
				
				if (false == m_osCache[0].str().empty())
					m_pFileLog << m_osCache[0].str() << m_osCache[1].str();
				else
					m_nFrameCurrent--;

				m_osCache[0].str("");
				m_osCache[1].str("");

				if (++m_nFrameCurrent == m_lines)
				{
					m_pFileLog.close();

					if (!m_bScanRepeat)
						StopListener(100);

					scanRepeat();
				}
			}

			m_pFileLog.flush();
		}

		m_nLastClock = msTime;
	}

}

bool SbgManager::SetParameter(int fps)
{
	if (!isInitialized())
		return false;

	m_fps = fps;

	_SbgEComOutputMode mode = _SbgEComOutputMode(200 / fps);

	//
	// Configure some output logs to 25 Hz
	//
	if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_EULER, mode) != SBG_NO_ERROR)
	{
		fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_EKF_EULER.\n");
		return false;
	}

	return true;
}

bool SbgManager::isInitialized()
{
	return m_bInitialized;
}
