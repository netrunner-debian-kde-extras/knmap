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

#include <errno.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/select.h> 
#include <sys/time.h> 

#include <qapplication.h>

#include <klocale.h>

#include "nmapoutputbuffer.h"
#include "scanmonitor.h"
#include "scanmonitorevent.h"
#include "scanwidget.h"

//	constructor
//	===========

ScanMonitor::ScanMonitor( ScanWidget* parent )
: QThread( ),
  m_buffer( NULL ),
  m_nmapBufferSize( 16384 ),
  m_parent( parent )
{	m_buffers.setAutoDelete( true );
}

//	getBuffer
//	=========

void ScanMonitor::getBuffer( )
{	for( m_buffer = m_buffers.first( ); m_buffer != NULL; m_buffer = m_buffers.next( ))
		if( m_buffer->isFree( ))
		{	m_buffer->setBusy( );
			m_buffers.take( );
			m_buffers.append( m_buffer );
			return;
		}

	m_buffer = new NmapOutputBuffer( m_nmapBufferSize );
	m_buffers.append( m_buffer );
}

//	readNmapOutput
//	==============

void ScanMonitor::readNmapOutput( const int pipeFD, const ScanMonitorEventType type )
{	while( true )
	{	if( m_buffer == NULL )
			getBuffer( );
		else
			m_buffer->setBusy( );

		int bytesRead = ::read( pipeFD, m_buffer->buffer( ), m_nmapBufferSize );

		if( bytesRead > 0 )
		{	m_buffer->length( bytesRead );
			QApplication::postEvent( m_parent, new ScanMonitorEvent( type, m_buffer ));
			m_buffer = NULL;
			continue;
		}

		if( !bytesRead || errno == EAGAIN || errno == EBADF )
		{	m_buffer->setFree( );
			break;
		}

		QString text = QString( i18n( "error reading nmap output from pipe: %2\n" )).arg( strerror( errno ));
		strcpy( m_buffer->buffer( ), text.ascii( ));
		m_buffer->length( text.length( ));
		QApplication::postEvent( m_parent, new ScanMonitorEvent( StderrType, m_buffer ));
		m_buffer = NULL;
		break;
	}
}

//	run
//	===

void ScanMonitor::run( )
{	fd_set efds;
	fd_set rfds;
	struct timeval timeout;

	while( true )
	{	FD_ZERO( &efds );
		FD_ZERO( &rfds );

		FD_SET( m_pipeStderrFD, &efds);
		FD_SET( m_pipeStdoutFD, &efds);

		FD_SET( m_pipeStderrFD, &rfds);
		FD_SET( m_pipeStdoutFD, &rfds);

		timeout.tv_sec  = 2;
		timeout.tv_usec = 0;

		if( !select( _max_( m_pipeStderrFD, m_pipeStdoutFD ) + 1, &rfds, NULL, &efds, &timeout ))
			continue;

		if( FD_ISSET( m_pipeStderrFD, &efds ) || FD_ISSET( m_pipeStdoutFD, &efds ))
			break;

		if( FD_ISSET( m_pipeStderrFD, &rfds ))
			readNmapOutput( m_pipeStderrFD, StderrType );

		if( FD_ISSET( m_pipeStdoutFD, &rfds ))
			readNmapOutput( m_pipeStdoutFD, StdoutType );
	}
}

//	setPipeFDs
//	==========

void ScanMonitor::setPipeFDs( const int pipeStderrFD, const int pipeStdoutFD )
{	m_pipeStderrFD = pipeStderrFD;
	m_pipeStdoutFD = pipeStdoutFD;
}
