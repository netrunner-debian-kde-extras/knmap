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

#ifndef _WHATSTHIS_H_
#define _WHATSTHIS_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qobject.h>
#include <qwhatsthis.h>
#include "global.h"

//	forward class definitions
//	=========================

class QPoint;

//	WhatsThis class definition
//	==========================

class WhatsThis : public QObject, public QWhatsThis
{	Q_OBJECT

	public:
		WhatsThis( QWidget* widget );
		QString text( const QPoint& pos );

	signals:
		void clicked( );
};

#endif	// _WHATSTHIS_H_
