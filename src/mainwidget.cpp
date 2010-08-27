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

#include <qtimer.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "htmlwidget.h"
#include "indexwidget.h"
#include "knmap.h"
#include "mainwidget.h"
#include "scannamedialog.h"
#include "scanstack.h"
#include "scanwidget.h"

//	constructor
//	===========

MainWidget::MainWidget( QWidget* parent, const char* name )
: QSplitter( parent, name ),
  m_dirty( false ),
  m_wrapText( false )
{	m_scanStack   = new ScanStack( this, "scan stack" );
	m_htmlWidget  = new HTMLWidget( &m_stylesheetURLs, this, "html widget" );
	m_indexWidget = new IndexWidget( m_scanStack, this, "index widget" );

	moveToFirst( m_indexWidget );
	m_scanStack->appendHTMLWidget( m_htmlWidget );
	setOpaqueResize( true );
	QTimer::singleShot( 0, this, SLOT( slotFinaliseInitialisation( )));

	connect( m_htmlWidget,
			 SIGNAL( errorLoadingLocalManPage( const QString& )),
			 SLOT( slotErrorLoadingLocalManPage( const QString& )));

	connect( m_indexWidget,
			 SIGNAL( visibleScanWidgetChanged( ScanWidget* )),
			 SLOT( slotVisibleScanWidgetChanged( ScanWidget* )));
}

//	createScan
//	==========

ScanWidget* MainWidget::createScan( const QString& scanName, const bool useTargetHost, const QString& profileName )
{	ScanWidget* scanWidget = new ScanWidget( scanName, useTargetHost, this, "scan widget" );

	m_scanStack->appendAndRaiseWidget( scanWidget );
	m_scanStack->wrapText( m_wrapText );

	if( profileName == QString::null )
		scanWidget->readSettings( );
	else
		scanWidget->setProfileName( profileName );

	connect( scanWidget,
			 SIGNAL( displayHelp( const QString& )),
			 SLOT( slotDisplayHelp( const QString& )));

	connect( scanWidget,
			 SIGNAL( optionsDirty( )),
			 SLOT( slotOptionsDirty( )));

	connect( scanWidget,
			 SIGNAL( outputAvailable( const bool, const bool )),
			 SIGNAL( outputAvailable( const bool, const bool )));

	connect( scanWidget,
			 SIGNAL( scanRename( const QString& )),
			 SLOT( slotScanRename( const QString& )));

	connect( scanWidget,
			 SIGNAL( scanStarted( )),
			 m_indexWidget,
			 SLOT( slotScanStarted( )));

	connect( scanWidget,
			 SIGNAL( scanStopped( ScanWidget* )),
			 m_indexWidget,
			 SLOT( slotScanStopped( ScanWidget* )));

	connect( scanWidget,
			 SIGNAL( statusBarText( const QString& )),
			 SIGNAL( statusBarText( const QString& )));

	return scanWidget;
}

//	loadManPage
//	===========

void MainWidget::loadManPage( )
{	m_htmlWidget->loadManPage( m_useLocalManPage );
}

//	readSettings
//	============

void MainWidget::readSettings( )
{	m_htmlWidget->readSettings( );

	KConfig* config = kapp->config( );
	config->setGroup( "General" );

	m_stylesheetURLs  = config->readListEntry( "stylesheetURLs" );
	m_useLocalManPage = config->readBoolEntry( "useLocalManPage", false );
	m_wrapText = config->readBoolEntry( "wrapText", false );

	QValueList<int> splitterSizes = config->readIntListEntry( "splitterSizes" );
	setSizes( splitterSizes );
	QStringList scanNames = config->readListEntry( "scanNames" );

	if( !scanNames.count( ))
		scanNames.append( "default" );

	QString     visibleScanName   = config->readEntry( "visibleScan", QString::null );
	ScanWidget* visibleScanWidget = NULL;

	for( QStringList::Iterator it = scanNames.begin( ); it != scanNames.end(); ++it )
	{	ScanWidget* scanWidget = createScan( *it, false, QString::null );

		if( visibleScanName == scanWidget->scanName( ))
			visibleScanWidget = scanWidget;
	}

	if( visibleScanWidget == NULL )
	{	visibleScanWidget = m_scanStack->firstScanWidget( );

		if( visibleScanWidget == NULL )
		{	KMessageBox::error( this, i18n( "Internal error - no ScanWidget to display!" ), i18n( "Internal error" ));
			kapp->quit( );
		}
	}

	m_scanStack->raiseWidget( visibleScanWidget );
}

//	saveAllDirtyProfiles
//	====================

void MainWidget::saveAllDirtyProfiles( )
{	ushort i;

	for( m_scanStack->resetScanWidgets( i ); m_scanStack->moreScanWidgets( i ); m_scanStack->nextScanWidget( i ))
	{	ScanWidget* scanWidget = m_scanStack->currentScanWidget( i );

		if( scanWidget->isDirty( ))
			scanWidget->profileSave( );
	}
}

//	saveSettings
//	============

void MainWidget::saveSettings( )
{	m_htmlWidget->saveSettings( );

	ushort i;
	QStringList scanNames;

	for( m_scanStack->resetScanWidgets( i ); m_scanStack->moreScanWidgets( i ); m_scanStack->nextScanWidget( i ))
	{	ScanWidget* scanWidget = m_scanStack->currentScanWidget( i );
		scanNames.append( scanWidget->scanName( ));
		scanWidget->saveSettings( );
	}

	QValueList<int> splitterSizes = sizes( );
	KConfig* config = kapp->config( );

	config->setGroup( "General" );
	config->writeEntry( "scanNames",       scanNames         );
	config->writeEntry( "splitterSizes",   splitterSizes     );
	config->writeEntry( "stylesheetURLs",  m_stylesheetURLs  );
	config->writeEntry( "useLocalManPage", m_useLocalManPage );
	config->writeEntry( "visibleScan",     m_scanStack->visibleWidget( )->scanName( ));
	config->writeEntry( "wrapText",        m_wrapText        );
}

//	setCaption
//	==========

void MainWidget::setCaption( ScanWidget* scanWidget )
{	QString profileName = scanWidget->profileName( );
	profileName = profileName.right( profileName.length( ) - strlen( PROFILE_PREFIX ));
	emit( setCaption( scanWidget->scanName( ) + " : " + profileName ));
}

//	slotDisplayHelp
//	===============

void MainWidget::slotDisplayHelp( const QString& anchor )
{	slotShowManPage( );
	m_htmlWidget->gotoAnchor( anchor );
}

//	slotErrorLoadingLocalManPage
//	============================

void MainWidget::slotErrorLoadingLocalManPage( const QString& errorText )
{	KMessageBox::sorry( this, errorText + "\nLoading the Knmap version.", "Couldn't load local nmap 'man' page" );
	slotSwitchManPages( );
}

//	slotFileSave
//	============

void MainWidget::slotFileSave( )
{	m_scanStack->visibleWidget( )->fileSave( );
}

//	slotFileSaveAs
//	==============

void MainWidget::slotFileSaveAs( )
{	m_scanStack->visibleWidget( )->fileSaveAs( );
}

//	slotFinaliseInitialisation
//	==========================

void MainWidget::slotFinaliseInitialisation( )
{	m_indexWidget->ensureCurrentVisible( );
	slotVisibleScanWidgetChanged( m_scanStack->visibleWidget( ));
	loadManPage( );
	emit( setManPageActionStuff( m_useLocalManPage ));
}

//	slotOptionsDirty
//	================

void MainWidget::slotOptionsDirty( )
{	m_dirty = false;
	ushort i;

	for( m_scanStack->resetScanWidgets( i ); m_scanStack->moreScanWidgets( i ); m_scanStack->nextScanWidget( i ))
		m_dirty |= m_scanStack->currentScanWidget( i )->isDirty( );
}

//	slotProfileCopy
//	===============

void MainWidget::slotProfileCopy( )
{	m_scanStack->visibleWidget( )->profileCopy( );
}

//	slotProfileDelete
//	=================

void MainWidget::slotProfileDelete( )
{	m_scanStack->visibleWidget( )->profileDelete( );
}

//	slotProfileLoad
//	===============

void MainWidget::slotProfileLoad( )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );
	scanWidget->profileLoad( );
	setCaption( scanWidget );
}

//	slotProfileRename
//	=================

void MainWidget::slotProfileRename( )
{	m_scanStack->visibleWidget( )->profileRename( );
}

//	slotProfileSave
//	===============

void MainWidget::slotProfileSave( )
{	m_scanStack->visibleWidget( )->profileSave( );
}

//	slotProfileSaveAs
//	=================

void MainWidget::slotProfileSaveAs( )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );
	scanWidget->profileSaveAs( );
	setCaption( scanWidget );
}

//	slotScanClose
//	=============

void MainWidget::slotScanClose( )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );
	scanWidget->profileAskAndSave( );
	m_scanStack->removeScanWidget( scanWidget );
	m_indexWidget->updateContents( );
	slotOptionsDirty( );
}

//	slotScanDuplicate
//	=================

void MainWidget::slotScanDuplicate( )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );
	scanWidget = createScan( scanWidget->scanName( ), scanWidget->useTargetHost( ), scanWidget->profileName( ));
	slotVisibleScanWidgetChanged( scanWidget );
}

//	slotScanNew
//	===========

void MainWidget::slotScanNew( )
{	ScanNameDialog dlg( NULL, this, "scan name dlg" );

	if( dlg.exec( ) != QDialog::Accepted )
		return;

	QString scanName;
	bool useTargetHost;

	if( dlg.useHostName( ))
	{	scanName = DEFAULT_SCAN_NAME;
		useTargetHost = true;
	}
	else
	{	scanName = dlg.scanName( );
		useTargetHost = false;
	}

	ScanWidget* scanWidget = createScan( scanName, useTargetHost, DEFAULT_PROFILE );
	slotVisibleScanWidgetChanged( scanWidget );
}

//	slotScanRename
//	==============

void MainWidget::slotScanRename( )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );
	ScanNameDialog dlg( scanWidget, this, "scan name dlg" );

	if( dlg.exec( ) != QDialog::Accepted )
		return;

	if( dlg.useHostName( ))
	{	slotScanRename( DEFAULT_SCAN_NAME );
		scanWidget->useTargetHost( true );
	}
	else
	{	slotScanRename( dlg.scanName( ));
		scanWidget->useTargetHost( false );
	}
}

//	slotScanRename
//	==============

void MainWidget::slotScanRename( const QString& scanName )
{	ScanWidget* scanWidget = m_scanStack->visibleWidget( );

	scanWidget->renameScan( scanName );
	m_indexWidget->updateContents( );
	setCaption( scanWidget );
}

//	slotShowManPage
//	===============

void MainWidget::slotShowManPage( )
{	m_scanStack->raiseWidget( m_htmlWidget );
}

//	slotSetStylesheet
//	=================

void MainWidget::slotSetStylesheet( )
{	m_htmlWidget->setStylesheet( );
}

//	slotSwitchManPages
//	==================

void MainWidget::slotSwitchManPages( )
{	m_useLocalManPage = !m_useLocalManPage;
	loadManPage( );
	emit( setManPageActionStuff( m_useLocalManPage ));
}

//	slotUseTargetHost
//	=================

void MainWidget::slotUseTargetHost( )
{	m_scanStack->visibleWidget( )->useTargetHost( true );
}

//	slotVisibleScanWidgetChanged
//	============================

void MainWidget::slotVisibleScanWidgetChanged( ScanWidget* scanWidget )
{	scanWidget->updateStatusBarText( );
	setCaption( scanWidget );
	m_indexWidget->updateContents( );
	emit( visibleScanWidgetChanged( scanWidget ));
}

//	slotWrapText
//	============

void MainWidget::slotWrapText( )
{	m_wrapText = !m_wrapText;
	m_scanStack->wrapText( m_wrapText );
}

//	slotZoomCustom
//	==============

void MainWidget::slotZoomCustom( )
{	bool ok;
	int  zoom = KInputDialog::getInteger( i18n( "Zoom percentage" ),
										  i18n( QString( "Enter zoom %'age (%1% - %2%)").arg( ZOOM_MIN_FACTOR ).arg( ZOOM_MAX_FACTOR)),
										  int( m_htmlWidget->zoomFactor( )),
										  ZOOM_MIN_FACTOR,
										  ZOOM_MAX_FACTOR,
										  10,
										  &ok,
										  NULL,
										  "zoom" );

	if( ok )
		m_htmlWidget->zoomFactor( zoom );
}

//	slotZoomIn
//	==========

void MainWidget::slotZoomIn( )
{	m_htmlWidget->zoomIn( );
}

//	slotZoomOut
//	===========

void MainWidget::slotZoomOut( )
{	m_htmlWidget->zoomOut( );
}
