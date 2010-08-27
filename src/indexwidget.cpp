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

#include <qdragobject.h>
#include <qfontmetrics.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qtimer.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>

#include "global.h"
#include "indexwidget.h"
#include "mainwidget.h"
#include "scanstack.h"
#include "scanwidget.h"

//	constructor
//	===========

IndexWidget::IndexWidget( ScanStack* scanStack, MainWidget* parent, const char* name )
: QScrollView( parent, name ),
  m_blinkTimer( NULL ),
  m_scanCount( 0 ),
  m_scanStack( scanStack )
{	setDragAutoScroll( true );
	viewport( )->setAcceptDrops( true );
}

//	calculateIndex
//	==============

short IndexWidget::calculateIndex( const ushort y )
{	QFontMetrics metrics = fontMetrics( );
	byte         height  = metrics.height( );
	ushort       index   = y / height;

	return index > m_scanStack->count( ) - 1 ? -1 : index;
}

//	contentsContextMenuEvent
//	========================

void IndexWidget::contentsContextMenuEvent( QContextMenuEvent* event )
{	short index = calculateIndex( event->y( ));

	if( index < 0 )
		return;

	ScanWidget* scanWidget = m_scanStack->makeScanWidgetVisible( index );
	emit( visibleScanWidgetChanged( scanWidget ));

	QPopupMenu*  contextMenu   = new QPopupMenu( this, "context menu" );
	KIconLoader* iconLoader    = KGlobal::iconLoader( );
	QIconSet     profileLoad   = iconLoader->loadIconSet( "profileload",   KIcon::Small );
	QIconSet     profileSave   = iconLoader->loadIconSet( "profilesave",   KIcon::Small );
	QIconSet     profileSaveAs = iconLoader->loadIconSet( "profilesaveas", KIcon::Small );
	QIconSet     scanClose     = iconLoader->loadIconSet( "scanclose",     KIcon::Small );
	QIconSet     scanDuplicate = iconLoader->loadIconSet( "scanduplicate", KIcon::Small );
	QIconSet     scanNew       = iconLoader->loadIconSet( "scannew",       KIcon::Small );
	QIconSet     scanRename    = iconLoader->loadIconSet( "scanrename",    KIcon::Small );

	contextMenu->insertItem( scanClose,     i18n( "&Close scan"      ), parent( ), SLOT( slotScanClose( )),     CTRL+Key_W );
	contextMenu->insertItem( scanDuplicate, i18n( "&Duplicate scan"  ), parent( ), SLOT( slotScanDuplicate( )), CTRL+Key_C );
	contextMenu->insertItem( scanNew,       i18n( "&New scan..."     ), parent( ), SLOT( slotScanNew( )),       CTRL+Key_N );
	contextMenu->insertItem( scanRename,    i18n( "&Rename scan..."  ), parent( ), SLOT( slotScanRename( )),    Key_F2     );
	contextMenu->insertItem( i18n( "&Use target host name" ), parent( ), SLOT( slotUseTargetHost( )));
	contextMenu->insertSeparator( );
	contextMenu->insertItem( profileLoad,   i18n( "&Load profile..."    ), parent( ), SLOT( slotProfileLoad( )));
	contextMenu->insertItem( profileSave,   i18n( "&Save profile..."    ), parent( ), SLOT( slotProfileSave( )));
	contextMenu->insertItem( profileSaveAs, i18n( "&Save profile as..." ), parent( ), SLOT( slotProfileSaveAs( )));

	contextMenu->exec( event->globalPos( ));
}

//	contentsDragMoveEvent
//	=====================

void IndexWidget::contentsDragMoveEvent( QDragMoveEvent* event )
{	short index = calculateIndex( event->pos( ).y( ));
	index = index < 0 ? m_scanStack->count( ) - 1 : index;

	ScanWidget* scanWidget = m_scanStack->makeScanWidgetVisible( index );
	emit( visibleScanWidgetChanged( scanWidget ));
}

//	contentsMouseMoveEvent
//	======================

void IndexWidget::contentsMouseMoveEvent( QMouseEvent* event )
{	if( event->stateAfter( ) != Qt::LeftButton )
		return;

	short index = calculateIndex( event->y( ));

	if( index < 0 )
		return;

	QStoredDrag* drag = new QStoredDrag( "application/octet-stream", this );
	drag->dragMove( );
	ScanWidget* scanWidget = m_scanStack->moveScanWidget( index );

	scanWidget->ignoreTabChanges( true );
	m_scanStack->raiseWidget( scanWidget );
	scanWidget->ignoreTabChanges( false );

	emit( visibleScanWidgetChanged( scanWidget ));
}

//	contentsMousePressEvent
//	=======================

void IndexWidget::contentsMousePressEvent( QMouseEvent* event )
{	if( event->stateAfter( ) != Qt::LeftButton )
		return;

	short index = calculateIndex( event->y( ));

	if( index < 0 )
		return;

	ScanWidget* scanWidget = m_scanStack->makeScanWidgetVisible( index );
	emit( visibleScanWidgetChanged( scanWidget ));

	if( scanWidget->state( ) == ScanWidget::finished )
		scanWidget->state( ScanWidget::dormant );
}

//	drawContents
//	============

void IndexWidget::drawContents( QPainter* painter, int clipx, int clipy, int clipw, int cliph )
{	painter->fillRect( clipx, clipy, clipw, cliph, KGlobalSettings::baseColor( ));
	painter->setBackgroundMode( Qt::OpaqueMode );

	QFontMetrics metrics = fontMetrics( );
	byte   height  = metrics.height( );
	byte   aWidth2 = 2 * metrics.width( "*" );
	ushort x       = height / 2;
	ushort xMax    = 0;
	ushort y       = 0;
	ushort i;

	for( m_scanStack->resetScanWidgets( i ); m_scanStack->moreScanWidgets( i ); y += height, m_scanStack->nextScanWidget( i ))
	{	ScanWidget* scanWidget = m_scanStack->currentScanWidget( i );

		switch( scanWidget->state( ))
		{	case ScanWidget::dormant:
				if( m_scanStack->isVisibleWidget( scanWidget ))
				{	painter->fillRect( 0, y, visibleWidth( ), height, KGlobalSettings::alternateBackgroundColor( ));
					painter->setBackgroundColor( KGlobalSettings::alternateBackgroundColor( ));
					painter->setPen( KGlobalSettings::buttonTextColor( ));
					break;
				}

				painter->setBackgroundColor( KGlobalSettings::baseColor( ));
				painter->setPen( KGlobalSettings::buttonTextColor( ));
				break;

			case ScanWidget::finished:
				painter->fillRect( 0, y, visibleWidth( ), height, KGlobalSettings::highlightColor( ));
				painter->setBackgroundColor( KGlobalSettings::highlightColor( ));
				painter->setPen( KGlobalSettings::activeTextColor( ));
				break;

			case ScanWidget::running:
				if( m_highlight )
				{	painter->fillRect( 0, y, visibleWidth( ), height, KGlobalSettings::highlightColor( ));
					painter->setBackgroundColor( KGlobalSettings::highlightColor( ));
					painter->setPen( KGlobalSettings::activeTextColor( ));
					break;
				}

				if( m_scanStack->isVisibleWidget( scanWidget ))
				{	painter->fillRect( 0, y, visibleWidth( ), height, KGlobalSettings::alternateBackgroundColor( ));
					painter->setBackgroundColor( KGlobalSettings::alternateBackgroundColor( ));
					painter->setPen( KGlobalSettings::buttonTextColor( ));
					break;
				}

				painter->fillRect( 0, y, visibleWidth( ), height, KGlobalSettings::baseColor( ));
				painter->setBackgroundColor( KGlobalSettings::baseColor( ));
				painter->setPen( KGlobalSettings::buttonTextColor( ));
				break;
		}

		if( scanWidget->useTargetHost( ))
			painter->drawText( x, y + metrics.ascent( ), "*" );

		painter->drawText( x + aWidth2, y + metrics.ascent( ), scanWidget->scanName( ));
		xMax = _max_( xMax, x + aWidth2 + metrics.width( scanWidget->scanName( )));
	}

	if( m_scanCount )
		m_highlight = !m_highlight;

	resizeContents( xMax + height / 2, y );
}

//	ensureCurrentVisible
//	====================

void IndexWidget::ensureCurrentVisible( )
{	short index = m_scanStack->indexOfVisibleScanWidget( );

	if( index < 0 )
		return;

	QFontMetrics metrics = fontMetrics( );
	short y = index * metrics.height( ) - height( ) / 2;
	setContentsPos( 0, y );
	updateContents( );
}

//	ensureCurrentVisible
//	====================

void IndexWidget::slotRepaint( )
{	repaintContents( false );
}

//	slotScanStarted
//	===============

void IndexWidget::slotScanStarted( )
{	if( m_scanCount++ )
		return;

	ASSERT( m_blinkTimer == NULL );
	m_blinkTimer = new QTimer( this );
	connect( m_blinkTimer, SIGNAL( timeout( )), SLOT( slotRepaint( )));
	m_blinkTimer->start( 500 );
	m_highlight = true;
}

//	slotScanStopped
//	===============

void IndexWidget::slotScanStopped( ScanWidget* scanWidget )
{	scanWidget->state( scanWidget == m_scanStack->visibleWidget( ) ? ScanWidget::dormant : ScanWidget::finished );
	updateContents( );

	if( --m_scanCount )
		return;

	delete m_blinkTimer;
	m_blinkTimer = NULL;
}
