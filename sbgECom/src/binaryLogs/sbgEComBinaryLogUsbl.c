#include "sbgEComBinaryLogUsbl.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_USBL message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseUsblData(const void *pPayload, uint32 payloadSize, SbgLogUsblData *pOutputData)
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

	pOutputData->latitude = sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->longitude = sbgStreamBufferReadDoubleLE(&inputStream);

	pOutputData->depth = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->latitudeAccuracy = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->longitudeAccuracy = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->depthAccuracy = sbgStreamBufferReadFloatLE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
