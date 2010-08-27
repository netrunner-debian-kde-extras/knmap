/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2005 by Kevin Gilbert                                   *
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

#ifndef _SCANMONITOR_H_
#define _SCANMONITOR_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qptrlist.h>
#include <qthread.h>

#include "global.h"

//	forward class definitions
//	=========================

class NmapOutputBuffer;
class ScanWidget;

//	ScanMonitor class definition
//	============================

class ScanMonitor : public QThread
{	public:
		enum ScanMonitorEventType
		{	StderrType = QEvent::User,
			StdoutType
		};

		ScanMonitor( ScanWidget* parent );
		void setPipeFDs( const int pipeStderrFD, const int pipeStdoutFD );

	private:
		void getBuffer( );
		void readNmapOutput( const int pipeFD, const ScanMonitorEventType type );
		void run( );

		nobPtr      m_buffer;
		const uint  m_nmapBufferSize;
		ScanWidget* m_parent;
		int         m_pipeStderrFD;
		int         m_pipeStdoutFD;

		QPtrList<NmapOutputBuffer> m_buffers;
};

#endif	// _SCANMONITOR_H_
