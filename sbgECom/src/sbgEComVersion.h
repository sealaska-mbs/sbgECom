/*!
 *	\file		sbgEComVersion.h
 *  \author		SBG Systems (Raphael Siryani)
 *	\date		05 February 2013
 *
 *	\brief		Header file that contains all versions related information such as change log.
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
#ifndef __SBG_E_COM_VERSION_H__
#define __SBG_E_COM_VERSION_H__

#include "sbgCommon.h"

//----------------------------------------------------------------------//
//- Version definitions                                                -//
//----------------------------------------------------------------------//

/*!
 *	Change log:<br>
 *	============<br>
 *	<br>
 *	<table border="0">
 *
 *	<tr><td valign="top">05/05/15:</td><td  valign="top">
 *	Version 1.4.3239:
 *
 *  <h2>Improvements</h2>
 *	<ul>
 *	<li>[SBGECOM-7] - Added support for both little and big endian platforms</li>
 *	<li>[SBGECOM-32] - Improved stream buffer error handling</li>
 *	<li>[SBGECOM-36] - Improved File naming and overal library organization</li>
 *	<li>[SBGECOM-37] - Modified firmware and software version numbering scheme</li>
 *	<li>[SBGECOM-38] - Increased raw GPS data buffer size from 2048 to 4096 bytes</li>
 *	</ul>
 *	    
 *	<h2>New Features</h2>
 *	<ul>
 *	<li>[SBGECOM-28] - Added differential correction age, diff base id and num sv to the SBG_ECOM_LOG_GPS#_POS</li>
 *	<li>[SBGECOM-29] - Added GNSS raw data log for the second GNSS receiver</li>
 *	<li>[SBGECOM-30] - Added official support for Ellipse additional output interfaces PORT C and PORT E</li>
 *	<li>[SBGECOM-33] - Added big/little endian support for stream buffer</li>
 *	<li>[SBGECOM-34] - Added sbgPlatform.h file to setup platform specific configuration such as endianness</li>
 *	</ul>
 *	<h2>Bug fixes</h2>
 *	<ul>
 *	<li>[SBGECOM-21] - CFixed SBG_ECOM_ETH#_RX_OK and SBG_ECOM_ETH#_TX_OK status definitions</li>
 *	<li>[SBGECOM-27] - Changed sbgEComHandle behavior so the error returned by receive call back is taken into account</li>
 *	<li>[SBGECOM-35] - Fixed improper comments in some configuration structures</li>
 *
 *	</ul>
 *	</td></tr>
 *
 *	<tr><td valign="top">20/03/14:</td><td  valign="top">
 *	Version 1.3.0.0:
 *
 *  <h2>Improvement</h2>
 *	<ul>
 *	<li>[SBGECOM-18] -         Fixed Typos in GPS pos, Vel and Hdt Fix Status</li>
 *	<li>[SBGECOM-20] -         Better error checking for sbgStreamBuffer with new method sbgStreamBufferGetLastError</li>
 *	<li>[SBGECOM-22] -         Added UTC &amp; Clock status to the binary log SbgLogUtcData </li>
 *	<li>[SBGECOM-23] -         Added Solution status to the binary log SbgLogEkfEuler,  SbgLogEkfQuat, SbgLogEkfNav	</li>
 *	<li>[SBGECOM-24] -         Added time stamp to the log SBG_ECOM_LOG_MAG_CALIB </li>
 *	</ul>
 *	    
 *	<h2>New Feature</h2>
 *	<ul>
 *	<li>[SBGECOM-10] -         Added sbgInterfaceChangeBaudrate for both windows and unix platforms </li>
 *	<li>[SBGECOM-19] -         Added SBG_ECOM_LOG_PRESSURE log for depth sensors and altimeters	</li>
 *	<li>[SBGECOM-25] -         Added support for Ellipse series	</li>
 *	<li>[SBGECOM-26] -         Added SBG_ECOM_LOG_USBL log for USBL aiding equipments (beta) </li>
 *	</ul>
 *	</td></tr>
 *
 *	<tr><td valign="top">22/11/13:</td><td  valign="top">
 *	Version 1.1.0.0:
 *	
 *	<h2>        Improvement
 *	</h2>
 *	<ul>
 *	<li>[SBGECOM-2] -         Added pitch information in the SbgLogGpsHdt GPS true heading log
 *	</li>
 *	<li>[SBGECOM-5] -         Now sbgEComProtocolReceive method returns the received command even if the CRC is not valid
 *	</li>
 *	</ul>
 *	    
 *	<h2>        New Feature
 *	</h2>
 *	<ul>
 *	<li>[SBGECOM-1] -         Added output log for DVL support
 *	</li>
 *	<li>[SBGECOM-3] -         Added output for GPS 1 raw data in order to support post processing
 *	</li>
 *	<li>[SBGECOM-4] -         Added event markers logs support
 *	</li>
 *	<li>[SBGECOM-6] -         Added Unix support and build script
 *	</li>
 *	<li>[SBGECOM-8] -         Added sbgEComReceiveAnyCmd method that return any received command that is not an output log
 *	</li>
 *	<li>[SBGECOM-9] -         Added settings import and export command
 *	</li>
 *	</ul>
 *	</td></tr>
 *
 *	<tr><td valign="top">01/04/13:</td><td>
 *	Version 1.0.0.0 initial release.<br>
 *	</td></tr>
 *	</table>
 */
#define SBG_E_COM_VERSION_MAJOR			1
#define SBG_E_COM_VERSION_MINOR			4
#define SBG_E_COM_VERSION_REV			3239
#define SBG_E_COM_VERSION_BUILD			SBG_VERSION_QUALIFIER_STABLE

#define SBG_E_COM_VERSION				SBG_VERSION_SOFTWARE(SBG_E_COM_VERSION_MAJOR,SBG_E_COM_VERSION_MINOR,SBG_E_COM_VERSION_REV,SBG_E_COM_VERSION_BUILD)

/*
 * Backward compatibility macro definitions.
 */
#define STR(X)		#X
#define ASSTR(X)	STR(X)
#define SBG_E_COM_VERSION_STR			ASSTR(SBG_E_COM_VERSION_MAJOR) "." ASSTR(SBG_E_COM_VERSION_MINOR) "." ASSTR(SBG_E_COM_VERSION_REV) "\0"

#endif
