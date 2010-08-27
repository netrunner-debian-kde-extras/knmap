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

#ifndef _MAINWIDGET_H_
#define _MAINWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qsplitter.h>
#include <qstringlist.h>

#include "global.h"

//	forward class definitions
//	=========================

class HTMLWidget;
class IndexWidget;
class KConfig;
class ScanStack;
class ScanWidget;

//	MainWidget class definition
//	===========================

class MainWidget : public QSplitter
{	Q_OBJECT

	public:
		MainWidget( QWidget* parent = NULL, const char* name = NULL );

		bool isDirty( ) const { return m_dirty; }
		void readSettings( );
		void saveAllDirtyProfiles( );
		void saveSettings( );
		bool wrapText( ) const { return m_wrapText; }

	signals:
		void displayHelp( const QString& );
		void outputAvailable( const bool, const bool );
		void setManPageActionStuff( const bool );
		void setCaption( const QString& );
		void statusBarText( const QString& );
		void visibleScanWidgetChanged( ScanWidget* );

	public slots:
		void slotFileSave( );
		void slotFileSaveAs( );
		void slotProfileCopy( );
		void slotProfileDelete( );
		void slotProfileLoad( );
		void slotProfileRename( );
		void slotProfileSave( );
		void slotProfileSaveAs( );
		void slotScanClose( );
		void slotScanDuplicate( );
		void slotScanNew( );
		void slotScanRename( );
		void slotScanRename( const QString& scanName );
		void slotShowManPage( );
		void slotSwitchManPages( );
		void slotSetStylesheet( );
		void slotUseTargetHost( );
		void slotWrapText( );
		void slotZoomCustom( );
		void slotZoomIn( );
		void slotZoomOut( );

	private slots:
		void slotDisplayHelp( const QString& anchor );
		void slotErrorLoadingLocalManPage( const QString& errorText );
		void slotFinaliseInitialisation( );
		void slotOptionsDirty( );
		void slotVisibleScanWidgetChanged( ScanWidget* scanWidget );

	private:
		ScanWidget* createScan( const QString& scanName, const bool useTargetHost, const QString& profileName );
		void        loadManPage( );
		void        setCaption( ScanWidget* scanWidget );

		bool         m_dirty;
		HTMLWidget*  m_htmlWidget;
		IndexWidget* m_indexWidget;
		ScanStack*   m_scanStack;
		QStringList  m_stylesheetURLs;
		bool         m_useLocalManPage;
		bool         m_wrapText;
};

#endif	// _MAINWIDGET_H_
