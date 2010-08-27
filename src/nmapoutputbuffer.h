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

#ifndef _NMAPOUTPUTBUFFER_H_
#define _NMAPOUTPUTBUFFER_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "global.h"

class NmapOutputBuffer
{	public:
		NmapOutputBuffer( const uint bufferSize );
		virtual ~NmapOutputBuffer( );

		string buffer( ) const { return m_buffer; }
		bool   isFree( ) const { return m_free;   }
		uint   length( ) const { return m_length; }
		void   length( const uint l ) { m_length = l; }
		void   setBusy( )      { m_free = false;  }
		void   setFree( )      { m_free = true;   }

	private:
		string m_buffer;
		bool   m_free;
		uint   m_length;
};

#endif	// _NMAPOUTPUTBUFFER_H_
