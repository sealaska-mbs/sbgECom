#include "sbgEComBinaryLogShipMotion.h"
#include <streamBuffer/sbgStreamBuffer.h>

//----------------------------------------------------------------------//
//- Operations                                                         -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for the SBG_ECOM_LOG_SHIP_MOTION_# message and fill the corresponding structure.
 *	\param[in]	pPayload					Read only pointer on the payload buffer.
 *	\param[in]	payloadSize					Payload size in bytes.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseShipMotionData(const void *pPayload, uint32 payloadSize, SbgLogShipMotionData *pOutputData)
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

	//
	// Read the main heave period in seconds
	//
	pOutputData->mainHeavePeriod = sbgStreamBufferReadFloatLE(&inputStream);

	//
	// Read the surge, sway and heave ship motion
	//
	pOutputData->shipMotion[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->shipMotion[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->shipMotion[2] = sbgStreamBufferReadFloatLE(&inputStream);
	
	//
	// Read the ship accelerations
	//
	pOutputData->shipAccel[0] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->shipAccel[1] = sbgStreamBufferReadFloatLE(&inputStream);
	pOutputData->shipAccel[2] = sbgStreamBufferReadFloatLE(&inputStream);

	//
	// Check if we have other outputs in this frame
	//
	if (payloadSize >= 46)
	{
		//
		// Read new outputs
		//
		pOutputData->shipVel[0] = sbgStreamBufferReadFloatLE(&inputStream);
		pOutputData->shipVel[1] = sbgStreamBufferReadFloatLE(&inputStream);
		pOutputData->shipVel[2] = sbgStreamBufferReadFloatLE(&inputStream);

		pOutputData->status = sbgStreamBufferReadUint16LE(&inputStream);
	}
	else
	{
		//
		// Those outputs are not available in previous versions
		//
		pOutputData->shipVel[0] = 0.0f;
		pOutputData->shipVel[1] = 0.0f;
		pOutputData->shipVel[2] = 0.0f;

		pOutputData->status = 0;
	}

	//
	// TODO: check for an error on the input stream such as buffer overflow
	//
	return SBG_NO_ERROR;
}
