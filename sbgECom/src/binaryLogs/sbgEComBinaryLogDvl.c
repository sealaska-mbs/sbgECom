#include "sbgEComBinaryLogDvl.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_DVL_BOTTOM_TRACK / SBG_ECOM_LOG_DVL_WATER_TRACK message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseDvlData(const void *pPayload, uint32 payloadSize, SbgLogDvlData *pOutputData)
{
	SbgStreamBuffer inputStream;

	//
	// Create an input stream to read the payload
	//
	sbgStreamBufferInitForRead(&inputStream, pPayload, payloadSize);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp = sbgStreamBufferReadUint32LE(&inputStream);
	pOutputData->status = sbgStreamBufferReadUint16LE(&inputStream);

	pOutputData->velocity[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->velocityStdDev[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityStdDev[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityStdDev[2] = sbgStreamBufferReadFloatLE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
