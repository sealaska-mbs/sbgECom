/*!
 *	\file		sbgVersion.h
 *  \author		SBG Systems (Raphael Siryani)
 *	\date		23 April 2015
 *
 *	\brief		Helper methods and definitions used to handle version.
 *
 *	Version information is stored within a single uint32.
 *	There are two different versions schemes to better handle software revisions and file format / hardware ones.
 *	
 *	Software version are only used for firmware and software.
 *		This versions is defined as a <Major>.<Minor>.<Build>-<Qualifier> for example: 1.3.1845-RC
 *		Major and Minor can range from 0 to 63.
 *		Build is an uint16 ranging from 0 to 65535.
 *		Qualifier is an enum encoded on 3 bits.
 *
 *	Basic version is used for hardware or file formats and is the legacy one for software.
 *		This versions is defined as a <Major>.<Minor>.<Revision>.<Build>
 *		Each element is stored in an uint8.
 *		The Major version should NEVER go above 31
 *
 *	\section CodeCopyright Copyright Notice 
 *	Copyright (C) 2007-2015, SBG Systems SAS. All rights reserved.
 *	
 *	This source code is intended for use only by SBG Systems SAS and
 *	those that have explicit written permission to use it from
 *	SBG Systems SAS.
 *	
 *	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 *	PARTICULAR PURPOSE.
 */
#ifndef __SBG_VERSION_H__
#define __SBG_VERSION_H__

#include "sbgDefines.h"
#include "sbgTypes.h"
#include "sbgDebug.h"

//----------------------------------------------------------------------//
//- Version related definitions                                        -//
//----------------------------------------------------------------------//
#define SBG_VERSION_SOFT_SCHEME						(0x00000001u << 31)	/*!< Set to 1 to indicate a version number that uses the software scheme. Set to 0 means a basic version scheme. */

#define SBG_VERSION_SOFT_SCHEME_QUALIFIER_MASK		(0x07)				/*!< Mask used to keep only the version qualifier enum. */
#define SBG_VERSION_SOFT_SCHEME_QUALIFIER_SHIFT		(28)				/*!< Bitshift to apply to get the qualifier enum. */

#define SBG_VERSION_SOFT_SCHEME_MAJOR_MASK			(0x3F)				/*!< Mask used to keep only the major version field. */
#define SBG_VERSION_SOFT_SCHEME_MAJOR_SHIFT			(22)				/*!< Bitshift used to get the major version field. */

#define SBG_VERSION_SOFT_SCHEME_MINOR_MASK			(0x3F)				/*!< Mask used to keep only the minor version field. */
#define SBG_VERSION_SOFT_SCHEME_MINOR_SHIFT			(16)				/*!< Bitshift used to get the minor version field. */

#define SBG_VERSION_SOFT_SCHEME_BUILD_MASK			(0xFFFF)			/*!< Mask used to keep only the build version field. */
#define SBG_VERSION_SOFT_SCHEME_BUILD_SHIFT			(0)					/*!< Bitshift used to get the build version field. */

/*!
 *	Version qualifier enum definition.
 *	This enum is used to identify if we have a basic version scheme or a software one.
 *	If it's a software one, we can easly know if the software version is a beta, a stable one.
 */
typedef enum _SbgVersionQualifier
{
	SBG_VERSION_QUALIFIER_DEV				= 0,							/*!< Development only version or pre-alpha. Customer shouldn't get this version qualifier. */
	SBG_VERSION_QUALIFIER_ALPHA				= 1,							/*!< Alpha version, missing features, can be unstable and could cause crashes or data loss. API can still change. */
	SBG_VERSION_QUALIFIER_BETA				= 2,							/*!< Beta version, features are freezed, can be unstable and could cause crashes or data loss. API shouldn't change. */
	SBG_VERSION_QUALIFIER_RC				= 3,							/*!< Release Candidate, features are freezed, with no known bug. API is freezed. */
	SBG_VERSION_QUALIFIER_STABLE			= 4,							/*!< Stable release, the version is the standard delivered one. */
	SBG_VERSION_QUALIFIER_HOT_FIX			= 5								/*!< Hot fixes were applied on a stable release. It should be bug fixes. This qualifier is temporary as the version should return to stable release as soon as the test procedure has been performed. */
} SbgVersionQualifier;

/*!
 *	Version qualifier enum definition.
 *	This enum is used to identify if we have a basic version scheme or a software one.
 *	If it's a software one, we can easly know if the software version is a beta, a stable one.
 */
typedef enum _SbgVersionCmpThresold
{
	SBG_VERSION_CMP_THRESOLD_MAJOR			= 0,							/*!< Compare only the major field. */
	SBG_VERSION_CMP_THRESOLD_MINOR			= 1,							/*!< Compare the major and minor fields. */
	SBG_VERSION_CMP_THRESOLD_REVISION		= 2,							/*!< Compare the major, minor and revision fields (only for basic versions). */
	SBG_VERSION_CMP_THRESOLD_BUILD			= 3,							/*!< Compare the majorj minor, revision and build fields. */
} SbgVersionCmpThresold;

/*!
 * This structure contains all the fields provided by a version number.
 *	It handles both basic and software version numbers.
 */
typedef struct _SbgVersion
{
	bool					softwareScheme;				/*!< Set to TRUE if it's a software scheme or FALSE if it's a basic one. */
	SbgVersionQualifier		qualifier;					/*!< Qualifier of the current version */
	uint8					major;						/*!< Major version */
	uint8					minor;						/*!< Minor version */
	uint8					rev;						/*!< Revision number - left to 0 in case of software version */
	uint16					build;						/*!< Build number */
} SbgVersion;

//----------------------------------------------------------------------//
//- Macro definitions for encode versions                              -//
//----------------------------------------------------------------------//

/*!
 *	Compile a version number using the basic scheme based on major, minor, revision and build information
 *	\param[in]	maj							The major version between 0 to 127.
 *	\param[in]	min							The minor version between 0 to 255.
 *	\param[in]	rev							The revision version between 0 to 255.
 *	\param[in]	build						The build number between 0 to 255.
 *	\return									The encoded version number.
 */
#define SBG_VERSION_BASIC(major, minor, rev, build)					(	(((uint32)(major)) << 24)	|		\
																		(((uint32)(minor)) << 16)	|		\
																		(((uint32)(rev)) << 8)		|		\
																		(((uint32)(build)))			)

/*!
 *	Compile a version number using the software scheme based on major, minor, build, qualifier.
 *	\param[in]	major						The major version between 0 to 63.
 *	\param[in]	minor						The major version between 0 to 63.
 *	\param[in]	build						The major version between 0 to 65535.
 *	\param[in]	qualifier					The version qualifier within the SbgVersionQualifier enum.
 *	\return									The encoded version number.
 */
#define SBG_VERSION_SOFTWARE(major, minor, build, qualifier)		(	SBG_VERSION_SOFT_SCHEME |																					\
																		((((uint32)(qualifier)) & SBG_VERSION_SOFT_SCHEME_QUALIFIER_MASK) << SBG_VERSION_SOFT_SCHEME_QUALIFIER_SHIFT) |	\
																		((((uint32)(major)) & SBG_VERSION_SOFT_SCHEME_MAJOR_MASK) << SBG_VERSION_SOFT_SCHEME_MAJOR_SHIFT)		|	\
																		((((uint32)(minor)) & SBG_VERSION_SOFT_SCHEME_MINOR_MASK) << SBG_VERSION_SOFT_SCHEME_MINOR_SHIFT)		|	\
																		((((uint32)(build)) & SBG_VERSION_SOFT_SCHEME_BUILD_MASK) << SBG_VERSION_SOFT_SCHEME_BUILD_SHIFT)		)

//----------------------------------------------------------------------//
//- Version encoding / decoding methods                                -//
//----------------------------------------------------------------------//

/*!
 *	Fill a SbgVersion structure based on an uint32 that stores the 'compiled' version information.
 *	\param[in]	encodedVersion											The version information stored within a uint32.
 *	\param[out]	pVersionInfo											Pointer on an allocated SbgVersion structure to fill.
 */
SBG_INLINE void sbgVersionDecode(uint32 encodedVersion, SbgVersion *pVersionInfo)
{
	//
	// Check input parameters
	//
	SBG_ASSERT(pVersionInfo, "Invalid input parameters");

	//
	// Test if we have a software version scheme
	//
	if (encodedVersion&SBG_VERSION_SOFT_SCHEME)
	{
		//
		// We have a software scheme, decode it
		//
		pVersionInfo->softwareScheme = TRUE;

		//
		// Decode the software scheme fields
		//
		pVersionInfo->qualifier	= (SbgVersionQualifier)((encodedVersion >> SBG_VERSION_SOFT_SCHEME_QUALIFIER_SHIFT) & SBG_VERSION_SOFT_SCHEME_QUALIFIER_MASK);
		pVersionInfo->major		= (encodedVersion >> SBG_VERSION_SOFT_SCHEME_MAJOR_SHIFT) & SBG_VERSION_SOFT_SCHEME_MAJOR_MASK;
		pVersionInfo->minor		= (encodedVersion >> SBG_VERSION_SOFT_SCHEME_MINOR_SHIFT) & SBG_VERSION_SOFT_SCHEME_MINOR_MASK;
		pVersionInfo->build		= (encodedVersion >> SBG_VERSION_SOFT_SCHEME_BUILD_SHIFT) & SBG_VERSION_SOFT_SCHEME_BUILD_MASK;

		//
		// Set the revision to zero as it's not used
		//
		pVersionInfo->rev		= 0;
	}
	else
	{
		//
		// We have a basic scheme, decode it
		//
		pVersionInfo->softwareScheme = FALSE;

		//
		// Decode the software scheme fields
		//
		pVersionInfo->major		= (encodedVersion >> 24) & 0xFF;
		pVersionInfo->minor		= (encodedVersion >> 16) & 0xFF;
		pVersionInfo->rev		= (encodedVersion >>  8) & 0xFF;
		pVersionInfo->build		= (encodedVersion >>  0) & 0xFF;

		//
		// Set the qualifier to zero
		//
		pVersionInfo->qualifier	= SBG_VERSION_QUALIFIER_DEV;
	}
}

/*!
 *	Construct a uint32 containing a version information based on a SbgVersion structure.
 *	\param[in]	pVersionInfo											Pointer on a read only version structure to encode.
 *	\return																The encoded version information on an uint32 or 0 if an error has occurred.
 */
SBG_INLINE uint32 sbgVersionEncode(const SbgVersion *pVersionInfo)
{
	uint32	encodedVersion = 0;

	//
	// Check input parameter
	//
	SBG_ASSERT(pVersionInfo, "Invalid input parameter.");

	//
	// Test if we have a software scheme or a basic one
	//
	if (pVersionInfo->softwareScheme)
	{
		//
		// We have a software, scheme, so test that the version is valid
		//
		SBG_ASSERT((pVersionInfo->major <= 63) && (pVersionInfo->minor <= 63) && (pVersionInfo->rev == 0), "Invalid version information for a software scheme.");

		//
		// Indicate that we have a software version scheme
		//
		encodedVersion = SBG_VERSION_SOFTWARE(pVersionInfo->major, pVersionInfo->minor, pVersionInfo->build, pVersionInfo->qualifier);
	}
	else
	{
		//
		// We have a basic version scheme so check parameter validty
		//
		SBG_ASSERT(pVersionInfo->major <= 127, "Invalid version information for a basic scheme.");

		//
		// Encode the basic version information
		//
		encodedVersion = SBG_VERSION_BASIC(pVersionInfo->major, pVersionInfo->minor, pVersionInfo->rev, pVersionInfo->build);
	}

	return encodedVersion;
}

/*!
 *	Convert a version information to a human readable string.
 *	\param[in]	pVersionInfo											Pointer on a read only version structure to convert to a human readable string.
 *	\param[out]	pBuffer													Buffer to store the version as a human readable null terminated string.
 *	\param[in]	sizeOfBuffer											Maximum buffer size including the null terminated char.
 *	\return																SBG_NO_ERROR if the version information has been converted to a string.
 *																		SBG_BUFFER_OVERFLOW is the buffer isn't big enough to store the converted version string.
 */
SBG_INLINE SbgErrorCode sbgVersionToString(const SbgVersion *pVersionInfo, char *pBuffer, size_t sizeOfBuffer)
{
	SbgErrorCode	errorCode = SBG_NO_ERROR;

	//
	// Check input parameters
	//
	SBG_ASSERT((pVersionInfo) && (pBuffer) && (sizeOfBuffer>0), "Invalid input parameters");

	//
	// Test if we have a basic or software version scheme
	//
	if (pVersionInfo->softwareScheme)
	{
		//
		// We have a software version scheme
		// Test that the buffer is big enough to store the generated string (31.31.65535-hotfix)
		//
		if (sizeOfBuffer >= 19)
		{
			//
			// Generate the string version
			//
			sprintf(pBuffer, "%u.%u.%u-", pVersionInfo->major, pVersionInfo->minor, pVersionInfo->build);

			//
			// Append the qualifier
			//
			switch (pVersionInfo->qualifier)
			{
			case SBG_VERSION_QUALIFIER_DEV:
				strcat(pBuffer, "dev");
				break;
			case SBG_VERSION_QUALIFIER_ALPHA:
				strcat(pBuffer, "alpha");
				break;
			case SBG_VERSION_QUALIFIER_BETA:
				strcat(pBuffer, "beta");
				break;
			case SBG_VERSION_QUALIFIER_RC:
				strcat(pBuffer, "rc");
				break;
			case SBG_VERSION_QUALIFIER_STABLE:
				strcat(pBuffer, "stable");
				break;
			case SBG_VERSION_QUALIFIER_HOT_FIX:
				strcat(pBuffer, "hotfix");
				break;
			default:
				break;
			}
		}
		else
		{
			//
			// Output buffer is to small
			//
			errorCode = SBG_BUFFER_OVERFLOW;
		}

	}
	else
	{
		//
		// We have a basic version scheme, generate the string
		// Test that the buffer is big enough to store the generated string
		//
		if (sizeOfBuffer >= 16)
		{
			//
			// Generate the string version
			//
			sprintf(pBuffer, "%u.%u.%u.%u", pVersionInfo->major, pVersionInfo->minor, pVersionInfo->rev, pVersionInfo->build);
		}
		else
		{
			//
			// Output buffer is to small
			//
			errorCode = SBG_BUFFER_OVERFLOW;
		}
	}

	//
	// Return status of operation
	//
	return errorCode;
}

//----------------------------------------------------------------------//
//- Version comparaison methods                                        -//
//----------------------------------------------------------------------//

/*!
 *	Compare two version information structures and return if the version A is greater, less or equal than the version B.
 *	The computation is roughly result = version A - version B
 *	We can define how far we will check if the version A is greater than the version B.
 *	For example, we can only check the major or major and minor fields.
 *	\param[in]	pVersionA					The first version to compare.
 *	\param[in]	pVersionB					The second version to compare.
 *	\param[in]	thresold					The comparaison thresold to know if we are checking the major, minor, revision, build, ...
 *	\return									A positive value if the version A is greater than B, a negative one if version A is less than B and 0 if the two versions are the same (according to the thresold).
 */
SBG_INLINE int32 sbgVersionCompare(const SbgVersion *pVersionA, const SbgVersion *pVersionB, SbgVersionCmpThresold thresold)
{
	int32	result;

	//
	// Check input parameter
	//
	SBG_ASSERT((pVersionA) && (pVersionB), "Invalid input parameters");

	//
	// Chekc that the two versions are using the same scheme
	//
	SBG_ASSERT(pVersionA->softwareScheme == pVersionB->softwareScheme, "You can only compare two versions that are using the same scheme.");

	//
	// Do the comparaison according to the selected thresold
	// Start by compairing the major field
	//
	result = pVersionA->major - pVersionB->major;

	//
	// Test if we have to also compare the minor field
	//
	if ( (result == 0) &&  ( (thresold == SBG_VERSION_CMP_THRESOLD_MINOR) || (thresold == SBG_VERSION_CMP_THRESOLD_REVISION) || (thresold == SBG_VERSION_CMP_THRESOLD_BUILD)) )
	{
		//
		// Update the result using the minor indication
		//
		result = pVersionA->minor - pVersionB->minor;

		//
		// Test if we have to also compare the revision field (for basic version only)
		//
		if ( (result == 0) &&  ( (thresold == SBG_VERSION_CMP_THRESOLD_REVISION) || (thresold == SBG_VERSION_CMP_THRESOLD_BUILD)) )
		{
			//
			// Test if we have a software scheme or a basic scheme version
			//
			if (pVersionA->softwareScheme)
			{
				//
				// We have a software scheme so set the result to 0
				//
				result = 0;
			}
			else
			{
				//
				// We have a basic scheme so we can compare the revision field
				//
				result = pVersionA->rev - pVersionB->rev;
			}

			//
			// Test if we have to also compare the build field
			//
			if ( (result == 0) &&  (thresold == SBG_VERSION_CMP_THRESOLD_BUILD) )
			{
				//
				// Compare the build field
				//
				result = pVersionA->build - pVersionB->build;
			}
		}
	}

	return result;
}

/*!
 *	Compare two encoded versions and return if the version A is greater, less or equal than the version B.
 *	The computation is roughly result = version A - version B
 *	We can define how far we will check if the version A is greater than the version B.
 *	For example, we can only check the major or major and minor fields.
 *	\param[in]	versionA					The first compiled version to compare.
 *	\param[in]	versionB					The second compiled version to compare.
 *	\param[in]	thresold					The comparaison thresold to know if we are checking the major, minor, revision, build, ...
 *	\return									A positive value if the version A is greater than B, a negative one if version A is less than B and 0 if the two versions are the same (according to the thresold).
 */
SBG_INLINE int32 sbgVersionCompareEncoded(uint32 versionA, uint32 versionB, SbgVersionCmpThresold thresold)
{
	SbgVersion	versionAInfo;
	SbgVersion	versionBInfo;

	//
	// Decode the versions
	//
	sbgVersionDecode(versionA, &versionAInfo);
	sbgVersionDecode(versionB, &versionBInfo);

	//
	// Do the comparaison
	//
	return sbgVersionCompare(&versionAInfo, &versionBInfo, thresold);
}

//----------------------------------------------------------------------//
//- Legacy version system methods                                      -//
//----------------------------------------------------------------------//

/*!
 *  Define all these methods as deprecated.
 */
SBG_DEPRECATED(SBG_INLINE uint32 SBG_VERSION(uint8 major, uint8 minor, uint8 rev, uint8 build));
SBG_DEPRECATED(SBG_INLINE uint8 SBG_VERSION_GET_MAJOR(uint32 encodedVersion));
SBG_DEPRECATED(SBG_INLINE uint8 SBG_VERSION_GET_MINOR(uint32 encodedVersion));
SBG_DEPRECATED(SBG_INLINE uint8 SBG_VERSION_GET_REV(uint32 encodedVersion));
SBG_DEPRECATED(SBG_INLINE uint8 SBG_VERSION_GET_BUILD(uint32 encodedVersion));

/*!
 *	DEPRECATED macro, please update your code
 *	Compile a version number using the basic scheme based on major, minor, revision and build information
 *	\param[in]	maj							The major version between 0 to 127.
 *	\param[in]	min							The minor version between 0 to 255.
 *	\param[in]	rev							The revision version between 0 to 255.
 *	\param[in]	build						The build number between 0 to 255.
 *	\return									The encoded version number.
 */
SBG_INLINE uint32 SBG_VERSION(uint8 major, uint8 minor, uint8 rev, uint8 build)
{
    //
    // The SBG_VERSION macro is the basic version scheme.
    //
    return SBG_VERSION_BASIC(major, minor, rev, build);
}

/*!
 *	DEPRECATED method, please update your code.
 *	Extract a basic version scheme using legacy methods.
 *	\param[in]	encodedVersion					The encoded version to extract the major version.
 *	\return										The major version.
 */
SBG_INLINE uint8 SBG_VERSION_GET_MAJOR(uint32 encodedVersion)
{
	SbgVersion	versionInfo;

	//
	// Decode the version information
	//
	sbgVersionDecode(encodedVersion, &versionInfo);

	//
	// Return the major version
	//
	return versionInfo.major;
}

/*!
 *	DEPRECATED method, please update your code.
 *	Extract a basic version scheme using legacy methods.
 *	\param[in]	encodedVersion					The encoded version to extract the major version.
 *	\return										The major version.
 */
SBG_INLINE uint8 SBG_VERSION_GET_MINOR(uint32 encodedVersion)
{
	SbgVersion	versionInfo;

	//
	// Decode the version information
	//
	sbgVersionDecode(encodedVersion, &versionInfo);

	//
	// Return the major version
	//
	return versionInfo.minor;
}

/*!
 *	DEPRECATED method, please update your code.
 *	Extract a basic version scheme using legacy methods.
 *	\param[in]	encodedVersion					The encoded version to extract the major version.
 *	\return										The major version.
 */
SBG_INLINE uint8 SBG_VERSION_GET_REV(uint32 encodedVersion)
{
	SbgVersion	versionInfo;

	//
	// Decode the version information
	//
	sbgVersionDecode(encodedVersion, &versionInfo);

	//
	// Return the major version
	//
	return versionInfo.rev;
}

/*!
 *	DEPRECATED method, please update your code.
 *	Extract a basic version scheme using legacy methods.
 *	\param[in]	encodedVersion					The encoded version to extract the major version.
 *	\return										The major version.
 */
SBG_INLINE uint8 SBG_VERSION_GET_BUILD(uint32 encodedVersion)
{
	SbgVersion	versionInfo;

	//
	// Decode the version information
	//
	sbgVersionDecode(encodedVersion, &versionInfo);

	//
	// Return the major version
	//
	return (uint8)versionInfo.build;
}



#endif	/* __SBG_VERSION_H__ */
