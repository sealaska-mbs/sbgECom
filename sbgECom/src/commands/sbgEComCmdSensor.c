#include "sbgEComCmdSensor.h"
#include <streamBuffer/sbgStreamBuffer.h>
#include "transfer/sbgEComTransfer.h"

//----------------------------------------------------------------------//
//- Sensor commands		                                               -//
//----------------------------------------------------------------------//

/*!
 *	Set a Motion profile ID
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[id]	id							Motion profile ID to set
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorSetMotionProfileId(SbgEComHandle *pHandle, uint32 id)
{
	SbgEComCmdId cmd;

	//
	// Build command
	//
	cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_MOTION_PROFILE_ID);

	//
	// Call generic function with specific command name
	//
	return sbgEComCmdGenericSetModelId(pHandle, cmd, id);
}


/*!
 *	Retrieve Motion profile information.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[out]	pMotionProfileInfo			Pointer to a SbgEComModelInfo to contain the current motion profile info.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorGetMotionProfileInfo(SbgEComHandle *pHandle, SbgEComModelInfo *pModelInfo)
{
	SbgEComCmdId cmd;

	//
	// Build command
	//
	cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_MOTION_PROFILE_ID);

	//
	// Call generic function with specific command name
	//
	return sbgEComCmdGenericGetModelInfo(pHandle, cmd, pModelInfo);
}

/*!
 *  DEPRECATED FUNCTION. Please use sbgEComCmdSensorSetMotionProfileId instead
 *	Set the motion profile (as a raw buffer) to use by the device.
 *	The new configuration will only be applied after SBG_ECOM_CMD_SETTINGS_ACTION (01) command is issued, with SBG_ECOM_SAVE_SETTINGS parameter.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[in]	pBuffer						Read only buffer containing the motion profile buffer.
 *	\param[in]	size						Size of the buffer.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorSetMotionProfile(SbgEComHandle *pHandle, const void *pBuffer, uint32 size)
{
	SbgErrorCode errorCode = SBG_NO_ERROR;
	SbgEComCmdId cmd;

	//
	// Test that the protocol handle is valid
	//
	if (pHandle)
	{	
		//
		// Build command
		//
		cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_SET_MOTION_PROFILE);

		//
		// Call function that handle data transfer
		//
		errorCode = sbgEComTransferSend(pHandle, cmd, pBuffer, size);
	}
	else
	{
		//
		// Invalid protocol handle.
		//
		errorCode = SBG_INVALID_PARAMETER;
	}

	return errorCode;
}

/*!
 *	Retrieve the initial conditions settings.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[out]	pConf						Pointer to a SbgEComInitConditionConf to contain the current initial conditions settings.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorGetInitCondition(SbgEComHandle *pHandle, SbgEComInitConditionConf *pConf)
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint16				receivedCmd;
	uint32				receivedSize;
	uint8				receivedBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		inputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointers are valid
	//
	if ((pHandle) && (pConf))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_INIT_PARAMETERS);

			//
			// Send the command only since this is a no payload command
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, NULL, 0);

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComReceiveAnyCmd(pHandle, &receivedCmd, receivedBuffer, &receivedSize, sizeof(receivedBuffer), SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a SBG_ECOM_CMD_INIT_PARAMETERS command
				//
				if ((errorCode == SBG_NO_ERROR) && (receivedCmd == cmd))
				{
					//
					// Initialize stream buffer to read parameters
					//
					sbgStreamBufferInitForRead(&inputStream, receivedBuffer, receivedSize);

					//
					// Read parameters
					//
					pConf->latitude = sbgStreamBufferReadDoubleLE(&inputStream);
					pConf->longitude = sbgStreamBufferReadDoubleLE(&inputStream);
					pConf->altitude = sbgStreamBufferReadDoubleLE(&inputStream);
					pConf->year = sbgStreamBufferReadUint16LE(&inputStream);
					pConf->month = sbgStreamBufferReadUint8LE(&inputStream);
					pConf->day = sbgStreamBufferReadUint8LE(&inputStream);

					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Null pointer
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}

/*!
 *	Set the initial condition configuration.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[in]	pConf						Pointer to a SbgEComInitConditionConf containing the new initial condition configuration.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorSetInitCondition(SbgEComHandle *pHandle, const SbgEComInitConditionConf *pConf)
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint8				outputBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		outputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointer are valid
	//
	if ((pHandle) && (pConf))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Init stream buffer for output
			//
			sbgStreamBufferInitForWrite(&outputStream, outputBuffer, sizeof(outputBuffer));

			//
			// Build payload
			//
			sbgStreamBufferWriteDoubleLE(&outputStream, pConf->latitude);
			sbgStreamBufferWriteDoubleLE(&outputStream, pConf->longitude);
			sbgStreamBufferWriteDoubleLE(&outputStream, pConf->altitude);
			sbgStreamBufferWriteUint16LE(&outputStream, (uint16)pConf->year);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->month);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->day);

			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_INIT_PARAMETERS);

			//
			// Send the payload over ECom
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, sbgStreamBufferGetLinkedBuffer(&outputStream), sbgStreamBufferGetLength(&outputStream));

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComWaitForAck(pHandle, cmd, SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a valid ACK
				//
				if (errorCode == SBG_NO_ERROR)
				{
					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Null pointer.
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}

/*!
 *	Retrieve the assignment of the aiding sensors.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[out]	pConf						Pointer to a SbgEComAidingAssignConf to contain the current assignment of the aiding sensors.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorGetAidingAssignment(SbgEComHandle *pHandle, SbgEComAidingAssignConf *pConf)
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint16				receivedCmd;
	uint32				receivedSize;
	uint8				receivedBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		inputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointers are valid
	//
	if ((pHandle) && (pConf))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_AIDING_ASSIGNMENT);

			//
			// Send the command only since this is a no payload command
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, NULL, 0);

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComReceiveAnyCmd(pHandle, &receivedCmd, receivedBuffer, &receivedSize, sizeof(receivedBuffer), SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a SBG_ECOM_CMD_AIDING_ASSIGNMENT command
				//
				if ((errorCode == SBG_NO_ERROR) && (receivedCmd == cmd))
				{
					//
					// Initialize stream buffer to read parameters
					//
					sbgStreamBufferInitForRead(&inputStream, receivedBuffer, receivedSize);

					//
					// Read parameters
					//
					pConf->gps1Port = (SbgEComModulePortAssignment)sbgStreamBufferReadUint8LE(&inputStream);
					pConf->gps1Sync = (SbgEComModuleSyncAssignment)sbgStreamBufferReadUint8LE(&inputStream);
					sbgStreamBufferSeek(&inputStream, 6*sizeof(uint8), SB_SEEK_CUR_INC);						/*!< Reserved fields to ignore */
					pConf->rtcmPort = (SbgEComModulePortAssignment)sbgStreamBufferReadUint8LE(&inputStream);
					sbgStreamBufferSeek(&inputStream, 1*sizeof(uint8), SB_SEEK_CUR_INC);						/*!< Reserved field to ignore */
					pConf->odometerPinsConf = (SbgEComOdometerPinAssignment)sbgStreamBufferReadUint8LE(&inputStream);

					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Null pointer
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}

/*!
 *	Set the assignment of the aiding sensors.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[out]	pConf						Pointer to a SbgEComAidingAssignConf containing the new assignment of the aiding sensors.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorSetAidingAssignment(SbgEComHandle *pHandle, const SbgEComAidingAssignConf *pConf)
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint8				outputBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		outputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointer are valid
	//
	if ((pHandle) && (pConf))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Init stream buffer for output
			//
			sbgStreamBufferInitForWrite(&outputStream, outputBuffer, sizeof(outputBuffer));

			//
			// Build payload
			//
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->gps1Port);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->gps1Sync);
			sbgStreamBufferSeek(&outputStream, 6*sizeof(uint8), SB_SEEK_CUR_INC);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->rtcmPort);
			sbgStreamBufferSeek(&outputStream, 1*sizeof(uint8), SB_SEEK_CUR_INC);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pConf->odometerPinsConf);

			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_AIDING_ASSIGNMENT);

			//
			// Send the payload over ECom
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, sbgStreamBufferGetLinkedBuffer(&outputStream), sbgStreamBufferGetLength(&outputStream));

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComWaitForAck(pHandle, cmd, SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a valid ACK
				//
				if (errorCode == SBG_NO_ERROR)
				{
					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Null pointer.
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}

/*!
 *	Retrieve the alignment and lever arm configuration of the sensor.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[out]	pAlignConf					Pointer to a SbgEComSensorAlignmentInfo struct to hold alignment configuration of the sensor.
 *	\param[out] leverArm					Pointer to a table to contain lever arm X, Y, Z components in meters.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorGetAlignmentAndLeverArm(SbgEComHandle *pHandle, SbgEComSensorAlignmentInfo *pAlignConf, float leverArm[3])
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint16				receivedCmd;
	uint32				receivedSize;
	uint8				receivedBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		inputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointer are valid
	//
	if ((pHandle) && (pAlignConf) && (leverArm))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_IMU_ALIGNMENT_LEVER_ARM);

			//
			// Send the command only since this is a no-payload command
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, NULL, 0);

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComReceiveAnyCmd(pHandle, &receivedCmd, receivedBuffer, &receivedSize, sizeof(receivedBuffer), SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a SBG_ECOM_CMD_IMU_ALIGNMENT command
				//
				if ((errorCode == SBG_NO_ERROR) && (receivedCmd == cmd))
				{
					//
					// Initialize stream buffer to read parameters
					//
					sbgStreamBufferInitForRead(&inputStream, receivedBuffer, receivedSize);

					//
					// Read parameters
					//
					pAlignConf->axisDirectionX = (SbgEComAxisDirection)sbgStreamBufferReadUint8LE(&inputStream);
					pAlignConf->axisDirectionY = (SbgEComAxisDirection)sbgStreamBufferReadUint8LE(&inputStream);
					pAlignConf->misRoll = sbgStreamBufferReadFloatLE(&inputStream);
					pAlignConf->misPitch = sbgStreamBufferReadFloatLE(&inputStream);
					pAlignConf->misYaw = sbgStreamBufferReadFloatLE(&inputStream);
					leverArm[0] = sbgStreamBufferReadFloatLE(&inputStream);
					leverArm[1] = sbgStreamBufferReadFloatLE(&inputStream);
					leverArm[2] = sbgStreamBufferReadFloatLE(&inputStream);

					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Null pointer.
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}

/*!
 *	Set the alignment and lever arm configuration of the sensor.
 *	\param[in]	pHandle						A valid sbgECom handle.
 *	\param[in]	pAlignConf					Pointer to a SbgEComSensorAlignmentInfo struct holding alignment configuration for the sensor.
 *	\param[in]  leverArm					Pointer to a table containing lever arm X, Y, Z components in meters.
 *	\return									SBG_NO_ERROR if the command has been executed successfully.
 */
SbgErrorCode sbgEComCmdSensorSetAlignmentAndLeverArm(SbgEComHandle *pHandle, const SbgEComSensorAlignmentInfo *pAlignConf, const float leverArm[3])
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	uint32				trial;
	uint8				outputBuffer[SBG_ECOM_MAX_BUFFER_SIZE];
	SbgStreamBuffer		outputStream;
	SbgEComCmdId		cmd;

	//
	// Test that the input pointer are valid
	//
	if ((pHandle) && (pAlignConf) && (leverArm))
	{
		//
		// Send the command three times
		//
		for (trial = 0; trial < 3; trial++)
		{
			//
			// Init stream buffer for output
			//
			sbgStreamBufferInitForWrite(&outputStream, outputBuffer, sizeof(outputBuffer));

			//
			// Build payload
			//
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pAlignConf->axisDirectionX);
			sbgStreamBufferWriteUint8LE(&outputStream, (uint8)pAlignConf->axisDirectionY);
			sbgStreamBufferWriteFloatLE(&outputStream, pAlignConf->misRoll);
			sbgStreamBufferWriteFloatLE(&outputStream, pAlignConf->misPitch);
			sbgStreamBufferWriteFloatLE(&outputStream, pAlignConf->misYaw);
			sbgStreamBufferWriteFloatLE(&outputStream, leverArm[0]);
			sbgStreamBufferWriteFloatLE(&outputStream, leverArm[1]);
			sbgStreamBufferWriteFloatLE(&outputStream, leverArm[2]);

			//
			// Build command
			//
			cmd = SBG_ECOM_BUILD_ID(SBG_ECOM_CLASS_LOG_CMD_0, SBG_ECOM_CMD_IMU_ALIGNMENT_LEVER_ARM);

			//
			// Send the payload over ECom
			//
			errorCode = sbgEComProtocolSend(&pHandle->protocolHandle, cmd, sbgStreamBufferGetLinkedBuffer(&outputStream), sbgStreamBufferGetLength(&outputStream));

			//
			// Make sure that the command has been sent
			//
			if (errorCode == SBG_NO_ERROR)
			{
				//
				// Try to read the device answer for 500 ms
				//
				errorCode = sbgEComWaitForAck(pHandle, cmd, SBG_ECOM_DEFAULT_CMD_TIME_OUT);

				//
				// Test if we have received a valid ACK
				//
				if (errorCode == SBG_NO_ERROR)
				{
					//
					// The command has been executed successfully so return
					//
					break;
				}
			}
			else
			{
				//
				// We have a write error so exit the try loop
				//
				break;
			}
		}
	}
	else
	{
		//
		// Invalid protocol handle.
		//
		errorCode = SBG_NULL_POINTER;
	}

	return errorCode;
}