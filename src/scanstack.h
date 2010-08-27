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

#ifndef _SCANSTACK_H_
#define _SCANSTACK_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qdict.h>
#include <qwidgetstack.h>
#include "global.h"

//	forward class definitions
//	=========================

class HTMLWidget;
class ScanWidget;

//	ScanStack class definition
//	==========================

class ScanStack : public QWidgetStack
{	Q_OBJECT

	public:
		ScanStack( QWidget* parent, const char* name = NULL );

		void        appendAndRaiseWidget( ScanWidget* scanWidget );
		void        appendHTMLWidget( HTMLWidget* htmlWidget );
		ushort      count( ) const { return m_scanWidgetDict->count( ); }
		ScanWidget* currentScanWidget( const ushort index ) { return scanWidget( index ); }
		short       findScanWidget( const ScanWidget* scanWidget );
		ScanWidget* firstScanWidget( ) const { return m_firstScanWidget; }
		short       indexOfVisibleScanWidget( );
		bool        isVisibleWidget( const ScanWidget* scanWidget ) { return scanWidget == visibleWidget( ); }
		ScanWidget* makeScanWidgetVisible( const ushort index );
		bool        moreScanWidgets( const ushort index ) const { return index < m_scanWidgetDict->count( ); }
		ScanWidget* moveScanWidget( const ushort fromIndex );
		void        nextScanWidget( ushort& index ) { index++;   }
		void        removeScanWidget( ScanWidget* scanWidget );
		void        resetScanWidgets( ushort& index ) { index = 0; }
		ScanWidget* visibleWidget( );
		void        wrapText( const bool wrap );

	private:
		ScanWidget* scanWidget( const ushort index ) { return (*m_scanWidgetDict)[ QString::number( index )]; }

		ushort      m_dictSize;
		ScanWidget* m_firstScanWidget;
		ushort      m_widgetId;

		QDict<ScanWidget>* m_scanWidgetDict;
};

#endif	// _SCANSTACK_H_
