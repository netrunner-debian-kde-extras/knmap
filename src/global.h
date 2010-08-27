/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2005, 2006 by Kevin Gilbert                             *
 *   kev.gilbert@cdu.edu.au                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//	forward class definitions
//	=========================

class NmapOutputBuffer;
class ScanMonitorEvent;

//	typedef's
//	=========

typedef bool*             boolPtr;
typedef unsigned char     byte;
typedef NmapOutputBuffer* nobPtr;
typedef ScanMonitorEvent* smePtr;
typedef char*             string;
typedef unsigned short    ushort;
typedef unsigned int      uint;

//	macros
//	======

#define _max_( a, b ) (a > b ? a : b)
#define _min_( a, b ) (a < b ? a : b)

//	general shit
//	============

const char DEFAULT_PROFILE[]   = "Knmap_Default";		// bloody obvious
const char DEFAULT_SCAN_NAME[] = "Target host";			// used to auto-name scan widgets
const char DOCBOOK[]           = "docbook";				// used to determine if documentation exists in the nmap html widget
const byte MAX_HOST_HISTORY    = 10;					// the maximum number of hosts that we will remember
const char PROFILE_PREFIX[]    = "Knmap_";				// wot's in the from of all our profile names
const uint ZOOM_MAX_FACTOR     = 300;					// for the HTML page
const uint ZOOM_MIN_FACTOR     = 20;					// for the HTML page

#endif	// _GLOBAL_H_
