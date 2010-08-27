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

#include <qdir.h>
#include <qlineedit.h>

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kglobalsettings.h>
#include <kinputdialog.h>
#include <kkeydialog.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kstdaction.h>

#include "htmlwidget.h"
#include "knmap.h"
#include "mainwidget.h"
#include "scanwidget.h"

//	constructor
//	===========

Knmap::Knmap( )
: KMainWindow( NULL, "knmap" ),
  m_mainWidget( new MainWidget( this, "main widget" ))
{	findNmap( );
	setCentralWidget( m_mainWidget );
	readSettings(  );
	createMenu( );
	setAutoSaveSettings( );

	connect( m_mainWidget,
			 SIGNAL( outputAvailable( const bool, const bool )),
			 SLOT( slotOutputAvailable( const bool, const bool)));

	connect( m_mainWidget,
			 SIGNAL( setManPageActionStuff( const bool )),
			 SLOT( slotSetManPageActionStuff( const bool )));

	connect( m_mainWidget,
			 SIGNAL( setCaption( const QString& )),
			 SLOT( slotSetCaption( const QString& )));

	connect( m_mainWidget,
			 SIGNAL( statusBarText( const QString& )), statusBar( ),
			 SLOT( message( const QString& )));

	connect( m_mainWidget,
			 SIGNAL( visibleScanWidgetChanged( ScanWidget* )),
			 SLOT( slotVisibleScanWidgetChanged( ScanWidget* )));
}

//	createMenu
//	==========

void Knmap::createMenu( )
{	KStdAction::configureToolbars( this, SLOT( slotConfigureToolbars( )), actionCollection( ));
	KStdAction::keyBindings( this, SLOT( slotConfigureShortcuts( )), actionCollection( ));
	KStdAction::quit( kapp, SLOT( quit( )), actionCollection( ));

	m_fileSaveAction = KStdAction::save( m_mainWidget,
										 SLOT( slotFileSave( )),
										 actionCollection( ));

	m_fileSaveAsAction = KStdAction::saveAs( m_mainWidget,
											 SLOT( slotFileSaveAs( )),
											 actionCollection( ));

	m_settingsManAction = new KAction( i18n( "&Use local man page" ),
										"localman.png",
										0,
										m_mainWidget,
										SLOT( slotSwitchManPages( )),
										actionCollection( ),
										"settings_local" );

	m_useTargetHostAction = new KToggleAction( i18n( "Use target host name" ),
											   NULL,
											   0,
											   m_mainWidget,
											   SLOT( slotUseTargetHost( )),
											   actionCollection( ),
											   "scan_use" );

	m_wrapTextAction = new KToggleAction( i18n( "Wrap text" ),
										  NULL,
										  0,
										  m_mainWidget,
										  SLOT( slotWrapText( )),
										  actionCollection( ),
										  "settings_wrap_text" );

	new KAction( i18n( "Close scan" ),
				 "scanclose",
				 CTRL+Key_W,
				 m_mainWidget,
				 SLOT( slotScanClose( )),
				 actionCollection( ),
				 "scan_close" );

	new KAction( i18n( "Copy profile..." ),
				 "profilecopy",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileCopy( )),
				 actionCollection( ),
				 "profile_copy" );

	new KAction( i18n( "Delete profile..." ),
				 "profiledelete",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileDelete( )),
				 actionCollection( ),
				 "profile_delete" );

	new KAction( i18n( "Display nmap man page" ),
				 "manpage",
				 0,
				 m_mainWidget,
				 SLOT( slotShowManPage( )),
				 actionCollection( ),
				 "help_man_page" );

	new KAction( i18n( "Duplicate scan" ),
				 "scanduplicate",
				 CTRL+Key_C,
				 m_mainWidget,
				 SLOT( slotScanDuplicate( )),
				 actionCollection( ),
				 "scan_duplicate" );

	new KAction( i18n( "Load profile..." ),
				 "profileload",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileLoad( )),
				 actionCollection( ),
				 "profile_load" );

	new KAction( i18n( "man page stylesheet..." ),
				 "manstylesheet",
				 0,
				 m_mainWidget,
				 SLOT( slotSetStylesheet( )),
				 actionCollection( ),
				 "settings_stylesheet" );

	new KAction( i18n( "New scan..." ),
				 "scannew",
				 CTRL+Key_N,
				 m_mainWidget,
				 SLOT( slotScanNew( )),
				 actionCollection( ),
				 "scan_new" );

	new KAction( i18n( "Rename profile..." ),
				 "profilerename",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileRename( )),
				 actionCollection( ),
				 "profile_rename" );

	new KAction( i18n( "Rename scan..." ),
				 "scanrename",
				 Key_F2,
				 m_mainWidget,
				 SLOT( slotScanRename( )),
				 actionCollection( ),
				 "scan_rename" );

	new KAction( i18n( "Save profile..." ),
				 "profilesave",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileSave( )),
				 actionCollection( ),
				 "profile_save" );

	new KAction( i18n( "Save profile as..." ),
				 "profilesaveas",
				 0,
				 m_mainWidget,
				 SLOT( slotProfileSaveAs( )),
				 actionCollection( ),
				 "profile_save_as" );

	new KAction( i18n( "Zoom custom"  ),
				 "zoomcustom",
				 0,
				 m_mainWidget,
				 SLOT( slotZoomCustom( )),
				 actionCollection( ),
				 "settings_zoom_custom"  );

	new KAction( i18n( "Zoom in"  ),
				 "zoomin",
				 0,
				 m_mainWidget,
				 SLOT( slotZoomIn( )),
				 actionCollection( ),
				 "settings_zoom_in"  );

	new KAction( i18n( "Zoom out" ),
				 "zoomout",
				 0,
				 m_mainWidget,
				 SLOT( slotZoomOut( )),
				 actionCollection( ),
				 "settings_zoom_out" );

	m_fileSaveAction->setEnabled( false );
	m_fileSaveAsAction->setEnabled( false );
	m_wrapTextAction->setChecked( m_mainWidget->wrapText( ));

	createStandardStatusBarAction( );
	setStandardToolBarMenuEnabled( true );

#ifdef _DEBUG
	createGUI( QDir::homeDirPath( ) + "/.kde/share/apps/knmap/knmapui.rc" );
#else
	createGUI( );
#endif
}

//	findNmap
//	========

void Knmap::findNmap( )
{	QString nmapPath = KStandardDirs::findExe( "nmap" );

	if( nmapPath.isEmpty( ))
	{	KMessageBox::error( this,
							i18n( "nmap is not installed on the computer - we cannot proceed without it. To download nmap go to \"http://www.insecure.org/nmap\"" ),
							i18n( "nmap not found" ));
		kapp->quit( );
	}
}

//	getDirPath
//	==========

bool Knmap::getDirPath( const QString& caption, QString& path )
{	QString startDir = (path.isEmpty( )) ? KGlobalSettings::documentPath( ) : path;
	QString tempPath = KFileDialog::getExistingDirectory( startDir, NULL, caption );

	if( tempPath.isEmpty( ))
		return false;

	path = tempPath;
	return true;
}

//	getDirPath
//	==========

void Knmap::getDirPath( const QString& caption, QString& path, QLineEdit* lineEdit )
{	if( getDirPath( caption, path ))
		lineEdit->setText( path );
}

//	getFilePath
//	===========

bool Knmap::getFilePath( const QString& caption, QString& path, const bool save )
{	QString filter   = "*.txt|Text files\n*.*|All files";
	QString startDir = (path.isEmpty( )) ? KGlobalSettings::documentPath( ) : path;
	QString tempPath = KFileDialog::getSaveFileName( startDir, filter, NULL, caption );

	if( tempPath.isEmpty( ))
		return false;

	if( save && QFile::exists( tempPath ))
	{	if( KMessageBox::Yes != KMessageBox::questionYesNo( NULL, QString( i18n( "File \"%1\" exists.\nOverwrite?" )).arg( tempPath ), i18n( "File exists" )))
			return false;
	}

	path = tempPath;
	return true;
}

//	getFilePath
//	===========

void Knmap::getFilePath( const QString& caption, QString& path, QLineEdit* lineEdit, const bool save )
{	if( getFilePath( caption, path, save ))
		lineEdit->setText( path );
}

//	queryExit
//	=========

bool Knmap::queryExit( )
{	if( m_mainWidget->isDirty( ) && KMessageBox::Yes == KMessageBox::questionYesNo( this, i18n( "Some profiles has been updated - save them now?" ), i18n( "Profile data updated" )))
		m_mainWidget->saveAllDirtyProfiles( );

	saveSettings( );
	return true;
}

//	readSettings
//	============

void Knmap::readSettings( )
{	m_mainWidget->readSettings( );
}

//	saveSettings
//	============

void Knmap::saveSettings( )
{	m_mainWidget->saveSettings( );
}

//	slotConfigureShortcuts
//	======================

void Knmap::slotConfigureShortcuts( )
{	KKeyDialog::configure( actionCollection( ), this);
}

//	slotConfigureToolbars
//	=====================

void Knmap::slotConfigureToolbars( )
{	saveMainWindowSettings( KGlobal::config( ), autoSaveGroup( ));

	KEditToolbar dlg( actionCollection( ));
	connect( &dlg, SIGNAL( newToolbarConfig( )),this, SLOT( slotNewToolbarConfig( )));
	dlg.exec( );
}

//	slotNewToolbarConfig
//	====================

void Knmap::slotNewToolbarConfig( )
{
#ifdef _DEBUG
	createGUI( QDir::homeDirPath( ) + "/.kde/share/apps/knmap/knmapui.rc" );
#else
	createGUI( );
#endif

	applyMainWindowSettings( KGlobal::config( ), autoSaveGroup( ));
}

//	slotOutputAvailable
//	===================

void Knmap::slotOutputAvailable( const bool saveState, const bool saveAsState )
{	m_fileSaveAction->setEnabled( saveState );
	m_fileSaveAsAction->setEnabled( saveAsState );
}

//	slotSetCaption
//	==============

void Knmap::slotSetCaption( const QString& caption )
{	setCaption( caption );
}

//	slotSetManPageActionStuff
//	=========================

void Knmap::slotSetManPageActionStuff( const bool useLocalManPage )
{	m_settingsManAction->setIcon( useLocalManPage ? "knmapman.png" : "localman.png" );
	m_settingsManAction->setText( useLocalManPage ? i18n( "&Use Knmap man page" ) : i18n( "&Use local man page" ));
}

//	slotVisibleScanWidgetChanged
//	============================

void Knmap::slotVisibleScanWidgetChanged( ScanWidget* scanWidget )
{	m_useTargetHostAction->setChecked( scanWidget->useTargetHost( ));
}

#include "knmap.moc"
