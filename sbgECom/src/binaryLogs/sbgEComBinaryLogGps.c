#include "sbgEComBinaryLogGps.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_GPS#_VEL message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseGpsVelData(const void *pPayload, uint32 payloadSize, SbgLogGpsVel *pOutputData)
{
	SbgStreamBuffer inputStream;

	//
	// Create an input stream to read the payload
	//
	sbgStreamBufferInitForRead(&inputStream, pPayload, payloadSize);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp		= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->status			= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->timeOfWeek		= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->velocity[0]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[1]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[2]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityAcc[0]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityAcc[1]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityAcc[2]	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->course			= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->courseAcc		= sbgStreamBufferReadFloatLE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}

/*!
 *	Parse data for the SBG_ECOM_LOG_GPS#_POS message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseGpsPosData(const void *pPayload, uint32 payloadSize, SbgLogGpsPos *pOutputData)
{
	SbgStreamBuffer inputStream;

	//
	// Create an input stream to read the payload
	//
	sbgStreamBufferInitForRead(&inputStream, pPayload, payloadSize);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->status				= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->timeOfWeek			= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->latitude			= sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->longitude			= sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->altitude			= sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->undulation			= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->latitudeAccuracy	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->longitudeAccuracy	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->altitudeAccuracy	= sbgStreamBufferReadFloatLE(&inputStream);

	//
	// Test if we have a additional information such as base station id (since version 1.4)
	//
	if (payloadSize >= 57)
	{
		//
		// Read the additional information
		//
		pOutputData->numSvUsed			= sbgStreamBufferReadUint8LE(&inputStream);
		pOutputData->baseStationId		= sbgStreamBufferReadUint16LE(&inputStream);
		pOutputData->differentialAge	= sbgStreamBufferReadUint16LE(&inputStream);
	}
	else
	{
		//
		// Default the additional information
		//
		pOutputData->numSvUsed = 0;
		pOutputData->baseStationId = 0xFFFF;
		pOutputData->differentialAge = 0xFFFF;
	}

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}

/*!
 *	Parse data for the SBG_ECOM_LOG_GPS#_HDT message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseGpsHdtData(const void *pPayload, uint32 payloadSize, SbgLogGpsHdt *pOutputData)
{
	SbgStreamBuffer inputStream;

	//
	// Create an input stream to read the payload
	//
	sbgStreamBufferInitForRead(&inputStream, pPayload, payloadSize);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->status				= sbgStreamBufferReadUint16LE(&inputStream);
	pOutputData->timeOfWeek			= sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->heading			= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->headingAccuracy	= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->pitch				= sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->pitchAccuracy		= sbgStreamBufferReadFloatLE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}

/*!
 *	Parse data for the SBG_ECOM_LOG_GPS#_RAW message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseGpsRawData(const void *pPayload, uint32 payloadSize, SbgLogGpsRaw *pOutputData)
{
	SbgErrorCode	errorCode = SBG_NO_ERROR;

	//
	// This buffer is different from other because only a variable size raw buffer is stored
	// We use the payload size (read from the low level protocol) to know the buffer size
	// Check that the received buffer can stored in the RAW message log
	//
	if (payloadSize <= SBG_ECOM_GPS_RAW_MAX_BUFFER_SIZE)
	{
		//
		// Copy the buffer
		//
		memcpy(pOutputData->rawBuffer, pPayload, payloadSize);
		pOutputData->bufferSize = payloadSize;
	}
	else
	{
		//
		// Unable to store the received buffer due to buffer overflow
		//
		errorCode = SBG_BUFFER_OVERFLOW;
	}

	return errorCode;
}