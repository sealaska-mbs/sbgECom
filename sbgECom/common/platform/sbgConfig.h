/*!
 *	\file		sbgConfig.h
 *  \author		SBG Systems (Raphael Siryani)
 *	\date		17 March 2015
 *
 *	\brief		Header file used to configure the framework.
 *
 *	You can configure for example the logging system.
 *
 *	\section CodeCopyright Copyright Notice 
 *	Copyright (C) 2007-2013, SBG Systems SAS. All rights reserved.
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
#ifndef __SBG_CONFIG_H__
#define __SBG_CONFIG_H__

//----------------------------------------------------------------------//
//- Platform specific configurations                                   -//
//----------------------------------------------------------------------//
#define SBG_CONFIG_UNALIGNED_ACCESS_AUTH			DISABLE					/*!< Set to ENABLE if the platform supports unaligned access */
#define SBG_CONFIG_BIG_ENDIAN						DISABLE					/*!< Set to ENABLE if the platform byte order is big-endian */

//----------------------------------------------------------------------//
//- Debug / logging Configurations                                     -//
//----------------------------------------------------------------------//

/*!
 *	Define the error log configuration for debug and release modes.
 *	You should setup your C preprocessor to define SBG_NDEBUG in release mode.
 */
#ifdef SBG_NDEBUG
	#define SBG_CONFIG_ENABLE_ASSERT				DISABLE					/*!< Set to ENABLE to enable all assertion checks. */
	#define SBG_CONFIG_ENABLE_LOG_ERROR				ENABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_ERROR. */
	#define SBG_CONFIG_ENABLE_LOG_WARNING			DISABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_WARNING. */
	#define SBG_CONFIG_ENABLE_LOG_INFO				DISABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_INFO. */
	#define SBG_CONFIG_ENABLE_LOG_VERBOSE			DISABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_VERBOSE. */
#else
	#define SBG_CONFIG_ENABLE_ASSERT				ENABLE					/*!< Set to ENABLE to enable all assertion checks. */
	#define SBG_CONFIG_ENABLE_LOG_ERROR				ENABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_ERROR. */
	#define SBG_CONFIG_ENABLE_LOG_WARNING			ENABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_WARNING. */
	#define SBG_CONFIG_ENABLE_LOG_INFO				ENABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_INFO. */
	#define SBG_CONFIG_ENABLE_LOG_VERBOSE			ENABLE					/*!< Set to ENABLE to enable log of all errors thrown using SBG_LOG_VERBOSE. */
#endif

#endif	/* __SBG_CONFIG_H__ */
