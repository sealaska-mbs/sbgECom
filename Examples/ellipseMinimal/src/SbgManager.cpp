
#include "SbgManager.h"


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

	m_hThread = 0;
	m_fStopping = false;

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
		retValue = -1;
	}

	errorCode = sbgEComInit(&comHandle, &sbgInterface);

	if (errorCode != SBG_NO_ERROR)
	{
		//
		// Unable to initialize the sbgECom
		//
		fprintf(stderr, "ellipseMinimal: Unable to initialize the sbgECom library.\n");
		retValue = -1;
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
		printf("Device : %0.9u found\n", deviceInfo.serialNumber);
	}
	else
	{
		fprintf(stderr, "ellipseMinimal: Unable to get device information.\n");
	}

	//
	// Configure some output logs to 25 Hz
	//
	if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_IMU_DATA, SBG_ECOM_OUTPUT_MODE_DIV_8) != SBG_NO_ERROR)
	{
		fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_IMU_DATA.\n");
	}
	if (sbgEComCmdOutputSetConf(&comHandle, SBG_ECOM_OUTPUT_PORT_A, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_EULER, SBG_ECOM_OUTPUT_MODE_DIV_8) != SBG_NO_ERROR)
	{
		fprintf(stderr, "ellipseMinimal: Unable to configure output log SBG_ECOM_LOG_EKF_EULER.\n");
	}

	//
	// Display a message for real time data display
	//
	printf("sbgECom properly Initialized.\n\nEuler Angles display with estimated standard deviation.\n");

	SbgEComReceiveFunc recFunc = onLogReceived;
	//
	// Define callbacks for received data
	//
	sbgEComSetReceiveCallback(&comHandle, recFunc, NULL);

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
	//
	// Handle separately each received data according to the log ID
	//
	switch (logCmd)
	{
#if 0
	case SBG_ECOM_LOG_EKF_EULER:
		//
		// Simply display euler angles in real time
		//
		printf("%d, Euler Angles: %3.1f\t%3.1f\t%3.1f\tStd Dev:%3.1f\t%3.1f\t%3.1f   \r", clock(),
			sbgRadToDegF(pLogData->ekfEulerData.euler[0]), sbgRadToDegF(pLogData->ekfEulerData.euler[1]), sbgRadToDegF(pLogData->ekfEulerData.euler[2]),
			sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[0]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[1]), sbgRadToDegF(pLogData->ekfEulerData.eulerStdDev[2]));
		break;
#else
	case SBG_ECOM_LOG_GPS1_POS:
		//
		// Simply display euler angles in real time
		//
		printf("%d, GPS Position: %3.1f\t%3.1f\t%3.1f \r", clock(),
			pLogData->gpsPosData.latitude, pLogData->gpsPosData.longitude, pLogData->gpsPosData.altitude);
		break;
#endif
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
