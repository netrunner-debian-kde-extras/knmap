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

#ifndef _TABWIDGETPTRLIST_H_
#define _TABWIDGETPTRLIST_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qptrlist.h>
#include "global.h"
#include "tabwidgetdata.h"

//	forward class definitions
//	=========================

class QPtrCollection;

//	TabWidgetPtrList class definition
//	=================================

class TabWidgetPtrList : public QPtrList<TabWidgetData>
{	public:
		TabWidgetPtrList( );

		int compareItems( QPtrCollection::Item item1, QPtrCollection::Item item2 );
};

#endif	// _TABWIDGETPTRLIST_H_
