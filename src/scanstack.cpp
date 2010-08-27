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

#include <qdict.h>

#include <kdebug.h>
#include <klocale.h>

#include "htmlwidget.h"
#include "scanstack.h"
#include "scanwidget.h"

//	constructor
//	===========

ScanStack::ScanStack( QWidget* parent, const char* name )
: QWidgetStack( parent, name ),
  m_dictSize( 29 ),
  m_firstScanWidget( NULL ),
  m_widgetId( 1 )
{	m_scanWidgetDict = new QDict<ScanWidget>( m_dictSize );
	m_scanWidgetDict->setAutoDelete( true );
}

//	appendAndRaiseWidget
//	====================

void ScanStack::appendAndRaiseWidget( ScanWidget* scanWidget )
{	addWidget( scanWidget, m_widgetId );
	raiseWidget( m_widgetId );
	m_widgetId++;

	if( m_firstScanWidget == NULL )
		m_firstScanWidget = scanWidget;

	ushort index = m_scanWidgetDict->count( );

	if( index == m_dictSize )
	{	m_dictSize *= 2;		// yes - i know it's supposed to be prime!
		m_scanWidgetDict->resize( m_dictSize );
	}

	m_scanWidgetDict->insert( QString::number( index ), scanWidget );
}

//	appendHTMLWidget
//	================

void ScanStack::appendHTMLWidget( HTMLWidget* htmlWidget )
{	ASSERT( m_widgetId == 1 );
	addWidget( (QWidget*) htmlWidget, m_widgetId++ );
}

//	findScanWidget
//	==============

short ScanStack::findScanWidget( const ScanWidget* scanWidget )
{	ushort i;

	for( resetScanWidgets( i ); moreScanWidgets( i ); nextScanWidget( i ))
		if( scanWidget == currentScanWidget( i ))
			return i;

	return -1;
}

//	indexOfVisibleScanWidget
//	========================

short ScanStack::indexOfVisibleScanWidget( )
{	ushort i;

	for( resetScanWidgets( i ); moreScanWidgets( i ); nextScanWidget( i ))
		if( isVisibleWidget( currentScanWidget( i )))
			break;

	if( !moreScanWidgets( i ))
	{	ASSERT( false );
		return -1;
	}

	return i;
}

//	makeScanWidgetVisible
//	=====================

ScanWidget* ScanStack::makeScanWidgetVisible( const ushort index )
{	ScanWidget* current = scanWidget( index );

	if( current == NULL )
		ASSERT( false );
	else
	{	current->ignoreTabChanges( true );
		raiseWidget( current );
		current->ignoreTabChanges( false );
	}

	return current;
}

//	moveScanWidget
//	==============

ScanWidget* ScanStack::moveScanWidget( const ushort fromIndex )
{	short toIndex = indexOfVisibleScanWidget( );

	if( toIndex < 0 )
		return NULL;

	ScanWidget* scanWidget = m_scanWidgetDict->take( QString::number( fromIndex ));

	if( fromIndex < toIndex )
		for( ushort index = fromIndex + 1; index <= toIndex; index++ )
		{	ScanWidget* scanWidget = m_scanWidgetDict->take( QString::number( index ));
			m_scanWidgetDict->insert( QString::number( index - 1 ), scanWidget );
		}
	else
		for( short index = fromIndex - 1; index >= toIndex; index-- )
		{	ScanWidget* scanWidget = m_scanWidgetDict->take( QString::number( index ));
			m_scanWidgetDict->insert( QString::number( index + 1 ), scanWidget );
		}

	m_scanWidgetDict->insert( QString::number( toIndex ), scanWidget );
	return scanWidget;
}

//	removeScanWidget
//	================

void ScanStack::removeScanWidget( ScanWidget* scanWidgetToRemove )
{	ushort n = m_scanWidgetDict->count( );

	if( n == 1 )
		return;

	short index = findScanWidget( scanWidgetToRemove );

	if( index < 0 )
	{	ASSERT( false );
		return;
	}

	m_scanWidgetDict->remove( QString::number( index ));

	for( ushort i = index + 1; i < n; i++ )
	{	ScanWidget* scanWidget = m_scanWidgetDict->take( QString::number( i ));
		m_scanWidgetDict->insert( QString::number( i - 1 ), scanWidget );
	}

	if( index >=short( m_scanWidgetDict->count( )))
		index = m_scanWidgetDict->count( ) - 1;

	ScanWidget* newVisibleWidget = scanWidget( index );
	newVisibleWidget->ignoreTabChanges( true );
	raiseWidget( newVisibleWidget );
	newVisibleWidget->ignoreTabChanges( false );
}

//	visibleWidget
//	=============

ScanWidget* ScanStack::visibleWidget( )
{	return (ScanWidget*) QWidgetStack::visibleWidget( );
}

//	wrapText
//	========

void ScanStack::wrapText( const bool wrap )
{	ushort i;

	for( resetScanWidgets( i ); moreScanWidgets( i ); nextScanWidget( i ))
		currentScanWidget( i )->wrapText( wrap );
}
