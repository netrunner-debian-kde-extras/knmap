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

#include <qfile.h>
#include <qtextstream.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kstandarddirs.h>

#include "htmlwidget.h"
#include "knmap.h"
#include "stylesheetdialog.h"

const float HTMLWidget::m_zoomIncrement = 25;

//	constructor
//	===========

HTMLWidget::HTMLWidget( QStringList* stylesheetURLs, QWidget* parent, const char* name )
: QWidget( parent, name ),
  m_dlg( NULL ),
  m_process( NULL ),
  m_stylesheetURLs( stylesheetURLs ),
  m_zoomFactor( 100 )
{	m_htmlPart = new KHTMLPart( this, "html part" );
	m_htmlPart->setOnlyLocalReferences( true );
}

//	destructor
//	==========

HTMLWidget::~HTMLWidget( )
{	if( m_dlg != NULL )
		delete m_dlg;

	if( m_process != NULL )
		delete m_process;
}

//	gotoAnchor
//	==========

void HTMLWidget::gotoAnchor( const QString& anchor )
{	if( !m_htmlPart->gotoAnchor( anchor ))
		KMessageBox::sorry( this, i18n( "There does not appear to be ANY help available for the item you requested.\nSorry! Now piss off and stop bothering me.\nYou stink.\nYou've got bad breath.\nYour ugly.\nNobody loves you.\nNow for your bad points..." ), i18n( "No help available in Knmap page" ));
}

//	loadKnmapManPage
//	================

void HTMLWidget::loadKnmapManPage( )
{	if( !tryKnmapFile( ))
	{	m_htmlPart->begin( );
		m_htmlPart->write( i18n( "<h1 align=center>Missing File</h1>" ));
		m_htmlPart->write( i18n( "<p align=center>The file <b>knmap/nmap_manpage.html</b>, which is part of the <i>Knmap</i> package, could not be found." ));
		m_htmlPart->write(i18n(  "<p align=center>As a result, the prepackaged <code>man</code> page for <i>Knmap</i> is not available." ));
		m_htmlPart->write( i18n( "<p align=center>You may be able to view your local <code>man</code> page for <i>nmap</i> by selecting <b>Settings / Use local man page</b>" ));
		m_htmlPart->end( );
	}

	m_htmlPart->setUserStyleSheet( m_stylesheetURL );
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
}

//	loadLocalManPage
//	================

void HTMLWidget::loadLocalManPage( )
{	m_process = new KProcess( );
	*m_process << "man";
	*m_process << "-w";
	*m_process << "nmap";

	connect( m_process, SIGNAL( processExited( KProcess* )), SLOT( slotManProcessExited( )));
	connect( m_process, SIGNAL( receivedStdout( KProcess*, char*, int )), SLOT( slotManReceivedStdout( KProcess*, char*, int )));

	m_manPagePath = QString::null;
	m_process->start( KProcess::NotifyOnExit, KProcess::Stdout );
}

//	loadManPage
//	===========

void HTMLWidget::loadManPage( const bool localManPage )
{	if( localManPage )
		loadLocalManPage( );
	else
		loadKnmapManPage( );
}

//	readSettings
//	============

void HTMLWidget::readSettings( )
{	KConfig* config = kapp->config( );
	config->setGroup( "General" );
	m_zoomFactor = config->readDoubleNumEntry( "zoomFactor", 100.0 );
	m_stylesheetURL.setPath( config->readPathEntry( "stylesheetURL" ));
}

//	resizeEvent
//	===========

void HTMLWidget::resizeEvent( QResizeEvent* event )
{	QWidget::resizeEvent( event );
	m_htmlPart->view( )->resize( width( ), height( ));
}

//	saveSettings
//	============

void HTMLWidget::saveSettings( )
{	KConfig* config = kapp->config( );
	config->setGroup( "General" );
	config->writeEntry( "stylesheetURL", m_stylesheetURL.path( ));
	config->writeEntry( "zoomFactor",    m_zoomFactor           );
}

//	setInitialValues
//	================

void HTMLWidget::setInitialValues( )
{	m_htmlPart->setUserStyleSheet( m_stylesheetURL );
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
}

//	setStylesheet
//	=============

void HTMLWidget::setStylesheet( )
{	m_dlg = new StylesheetDialog( m_stylesheetURL.path( ), *m_stylesheetURLs, this, "man stylesheet dlg" );
	connect( m_dlg, SIGNAL( stylesheetRemoved( )), SLOT( slotStylesheetRemoved( )));

	if( m_dlg->exec( ) != QDialog::Accepted )
	{	delete m_dlg;
		m_dlg = NULL;
		return;
	}

	*m_stylesheetURLs = m_dlg->stylesheetURLs( );
	m_stylesheetURL.setPath( m_stylesheetURLs->front( ));
	m_htmlPart->setUserStyleSheet( m_stylesheetURL );

	emit( optionsDirty( ));
	delete m_dlg;
	m_dlg = NULL;
}

//	slotGunzipProcessExited
//	=======================

void HTMLWidget::slotGunzipProcessExited( )
{	if( !m_process->normalExit( ))
	{	delete m_process;
		m_process = NULL;
		emit( errorLoadingLocalManPage( i18n( "Could not load the local 'man' page - the 'gunzip' process failed." )));
		return;
	}

	delete m_process;

	m_process = new KProcess( );
	*m_process << "man2html";
	*m_process << "-";

	connect( m_process, SIGNAL( processExited( KProcess* )), SLOT( slotMan2HTMLProcessExited( )));
	connect( m_process, SIGNAL( receivedStdout( KProcess*, char*, int )), SLOT( slotMan2HTMLReceivedStdout( KProcess*, char*, int )));

	m_htmlData = QString::null;
	m_process->start( KProcess::NotifyOnExit, KProcess::Communication( KProcess::Stdin | KProcess::Stdout ));
	m_process->writeStdin( m_manPageData, m_manPageData.length( ));
	m_process->closeStdin( );
}

//	slotGunzipReceivedStdout
//	========================

void HTMLWidget::slotGunzipReceivedStdout( KProcess* /* process */, char* buffer, int buflen )
{	m_manPageData += QString::fromLatin1( buffer, buflen );
}

//	slotMan2HTMLReceivedStdout
//	==========================

void HTMLWidget::slotMan2HTMLReceivedStdout( KProcess* /* process */, char* buffer, int buflen )
{	m_htmlData += QString::fromLatin1( buffer, buflen );
}

//	slotMan2HTMLProcessExited
//	=========================

void HTMLWidget::slotMan2HTMLProcessExited( )
{	if( !m_process->normalExit( ))
		emit( errorLoadingLocalManPage( i18n( "Could not load the local 'man' page - the 'man2html' process failed." )));

	delete m_process;
	m_process = NULL;

	m_htmlPart->begin( );
	QTextStream textStream( &m_htmlData, IO_ReadOnly );

	while( !textStream.atEnd( ))
	{	QString htmlLine = textStream.readLine( );

		if( htmlLine.startsWith( "<H2>TARGET SPECIFICATION</H2>" ))
			m_htmlPart->write( "<a id='target'>" );
		else
		if( htmlLine.startsWith( "<DT><B>-" ))
		{	QStringList nmapOptions = QStringList::split( " ", htmlLine.right( htmlLine.length( ) - 7 ));
			QString     anchors;

			for( QStringList::Iterator it = nmapOptions.begin( ); it != nmapOptions.end( ); ++it )
			{	if( (*it)[ 0 ] != '-' )
					break;

				QStringList nmapOption = QStringList::split( "<", *it );
				anchors += QString( "<a id='%1'>" ).arg( nmapOption.front( ));
			}

			m_htmlPart->write( anchors );
		}

		m_htmlPart->write( htmlLine + "\n" );
	}

	m_htmlPart->end( );

	m_htmlPart->setUserStyleSheet( m_stylesheetURL );
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
}

//	slotManProcessExited
//	====================

void HTMLWidget::slotManProcessExited( )
{
	if( !m_process->normalExit( ))
	{	delete m_process;
		m_process = NULL;
		emit( errorLoadingLocalManPage( i18n( "Could not load the local 'man' page - the 'man -w' process failed." )));
		return;
	}

	delete m_process;
	ushort length = m_manPagePath.length( );

	if( m_manPagePath[ length - 1 ] == '\n' )
		m_manPagePath = m_manPagePath.left( length - 1 );

	if( !m_manPagePath.endsWith( ".gz" ))
	{	m_process = NULL;
		emit( errorLoadingLocalManPage( i18n( "Could not load the local 'man' page - it either was not found or it not a 'gz' file." )));
		return;
	}

	m_process = new KProcess( );
	*m_process << "gunzip";
	*m_process << "-c";
	*m_process << m_manPagePath;

	connect( m_process, SIGNAL( processExited( KProcess* )), SLOT( slotGunzipProcessExited( )));
	connect( m_process, SIGNAL( receivedStdout( KProcess*, char*, int )), SLOT( slotGunzipReceivedStdout( KProcess*, char*, int )));

	m_manPageData = QString::null;
	m_process->start( KProcess::NotifyOnExit, KProcess::Stdout );
}

//	slotManReceivedStdout
//	=====================

void HTMLWidget::slotManReceivedStdout( KProcess* /* process */, char* buffer, int buflen )
{	m_manPagePath += QString::fromLatin1( buffer, buflen );
}

//	slotStylesheetRemoved
//	=====================

void HTMLWidget::slotStylesheetRemoved( )
{	*m_stylesheetURLs = m_dlg->stylesheetURLs( );
	emit( optionsDirty( ));
}

//	tryKnmapFile
//	============

bool HTMLWidget::tryKnmapFile( )
{
#ifdef _DEBUG
	QString path = "/home/c/knmap/src/nmap_manpage.html";
#else
	QString path = locate( "data", "knmap/nmap_manpage.html" );
#endif

	if( !QFile::exists( path ))
		return false;

	QFile manPageFile( path );

	if( !manPageFile.open( IO_ReadOnly ))
		return false;

	QTextStream manPageStream( &manPageFile );
	m_htmlPart->begin( );

	while ( !manPageStream.atEnd( ))
	{	QString line = manPageStream.read( );
		m_htmlPart->write( line );
	}

	m_htmlPart->end( );
	manPageFile.close();

	return true;
}

//	zoomFactor
//	==========

void  HTMLWidget::zoomFactor( const float factor )
{	m_zoomFactor = factor;
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
	emit( optionsDirty( ));
}
//	zoomIn
//	======

void HTMLWidget::zoomIn( )
{	m_zoomFactor = _min_( ZOOM_MAX_FACTOR, m_zoomFactor + m_zoomIncrement );
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
	emit( optionsDirty( ));
}

//	zoomOut
//	=======

void HTMLWidget::zoomOut( )
{	m_zoomFactor = _max_( ZOOM_MIN_FACTOR, m_zoomFactor - m_zoomIncrement );
	m_htmlPart->setZoomFactor( int( m_zoomFactor ));
	emit( optionsDirty( ));
}
