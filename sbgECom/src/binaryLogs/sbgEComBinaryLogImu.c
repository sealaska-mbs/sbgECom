#include "sbgEComBinaryLogImu.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_IMU_DATA message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseImuData(const void *pPayload, uint32 payloadSize, SbgLogImuData *pOutputData)
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
				
	pOutputData->accelerometers[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->accelerometers[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->accelerometers[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->gyroscopes[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->gyroscopes[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->gyroscopes[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->temperature = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->deltaVelocity[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->deltaVelocity[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->deltaVelocity[2] = sbgStreamBufferReadFloatLE(&inputStream);
				
	pOutputData->deltaAngle[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->deltaAngle[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->deltaAngle[2] = sbgStreamBufferReadFloatLE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
