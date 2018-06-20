#include "sbgEComBinaryLogUtc.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_UTC_DATA message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseUtcData(const void *pPayload, uint32 payloadSize, SbgLogUtcData *pOutputData)
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
	pOutputData->year = sbgStreamBufferReadUint16LE(&inputStream);
	pOutputData->month = sbgStreamBufferReadInt8LE(&inputStream);
	pOutputData->day = sbgStreamBufferReadInt8LE(&inputStream);
	pOutputData->hour = sbgStreamBufferReadInt8LE(&inputStream);
	pOutputData->minute = sbgStreamBufferReadInt8LE(&inputStream);
	pOutputData->second = sbgStreamBufferReadInt8LE(&inputStream);
	pOutputData->nanoSecond = sbgStreamBufferReadInt32LE(&inputStream);
	pOutputData->gpsTimeOfWeek = sbgStreamBufferReadUint32LE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
