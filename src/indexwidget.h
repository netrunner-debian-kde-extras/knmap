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

#ifndef _INDEXWIDGET_H_
#define _INDEXWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qscrollview.h>

//	forward class definitions
//	=========================

class KConfig;
class MainWidget;
class QPainter;
class QTimer;
class ScanStack;
class ScanWidget;

//	IndexWidget class definition
//	============================

class IndexWidget : public QScrollView
{	Q_OBJECT

	public:
		IndexWidget( ScanStack* scanStack, MainWidget* parent, const char* name = NULL );

		void ensureCurrentVisible( );

	signals:
		void visibleScanWidgetChanged( ScanWidget* );

	public slots:
		void slotRepaint( );
		void slotScanStarted( );
		void slotScanStopped( ScanWidget* scanWidget );

	private:
		short calculateIndex( const ushort y );
		void  contentsContextMenuEvent( QContextMenuEvent* event );
		void  contentsDragMoveEvent( QDragMoveEvent* event );
		void  contentsMouseMoveEvent( QMouseEvent* event );
		void  contentsMousePressEvent( QMouseEvent* event );
		void  drawContents( QPainter* painter, int clipx, int clipy, int clipw, int cliph );

		QTimer*    m_blinkTimer;
		bool       m_highlight;
		ushort     m_scanCount;
		ScanStack* m_scanStack;
};

#endif	// _INDEXWIDGET_H_
