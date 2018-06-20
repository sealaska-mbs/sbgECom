#include "sbgEComBinaryLogEkf.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_EKF_EULER message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseEkfEulerData(const void *pPayload, uint32 payloadSize, SbgLogEkfEulerData *pOutputData)
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

	pOutputData->euler[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->euler[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->euler[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->eulerStdDev[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->eulerStdDev[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->eulerStdDev[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->status			= sbgStreamBufferReadUint32LE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}

/*!
 *	Parse data for the SBG_ECOM_LOG_EKF_QUAT message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseEkfQuatData(const void *pPayload, uint32 payloadSize, SbgLogEkfQuatData *pOutputData)
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

	pOutputData->quaternion[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->quaternion[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->quaternion[2] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->quaternion[3] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->eulerStdDev[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->eulerStdDev[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->eulerStdDev[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->status			= sbgStreamBufferReadUint32LE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}

/*!
 *	Parse data for the SBG_ECOM_LOG_EKF_NAV message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseEkfNavData(const void *pPayload, uint32 payloadSize, SbgLogEkfNavData *pOutputData)
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

	pOutputData->velocity[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocity[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->velocityStdDev[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityStdDev[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->velocityStdDev[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->position[0] = sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->position[1] = sbgStreamBufferReadDoubleLE(&inputStream);
	pOutputData->position[2] = sbgStreamBufferReadDoubleLE(&inputStream);

	pOutputData->undulation = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->positionStdDev[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->positionStdDev[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->positionStdDev[2] = sbgStreamBufferReadFloatLE(&inputStream);

	pOutputData->status			= sbgStreamBufferReadUint32LE(&inputStream);

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
