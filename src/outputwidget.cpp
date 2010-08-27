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
#include <qtimer.h>

#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "knmap.h"
#include "outputwidget.h"

//	constructor
//	===========

OutputWidget::OutputWidget( QWidget* parent, const char* name )
: QTextEdit( parent, name ),
  m_append( false ),
  m_dataBytes( 0 ),
  m_firstSave( true ),
  m_priorType( OutputType( -1 ))
{	setFamily( "monospace" );
	setReadOnly( true );
	QTimer::singleShot( 0, this, SLOT( slotUpdateStatusBarText( )));
}

//	addOutput
//	=========

void OutputWidget::addOutput( const OutputType type, const char* buffer, const int buflen )
{	m_dataBytes       += buflen;
	QString outputText = QString::fromLatin1( buffer, buflen );
	QString prependText;

	if( m_append && type == m_priorType && m_priorType != OutputType( -1 ))
	{	uint paraNo = paragraphs( ) - 1;
		prependText = text( paraNo );;
		prependText = prependText.left( prependText.length( ) - 1 );
		removeParagraph( paraNo );
	}

	m_append    = buffer[ buflen - 1 ] != '\n';
	m_priorType = type;

	switch( type )
	{	case Stderr:
			m_dataBytes += 14;
			append( "<b><i>" + prependText + outputText + "</i></b>" );
			break;

		case Stdin:
			m_dataBytes +=7;
			append( "<b>" + prependText + outputText + "</b>" );
			break;

		case Stdout:
			append( prependText + outputText );
			break;
	}

	slotUpdateStatusBarText();
}

//	clearOutput
//	===========

void OutputWidget::clearOutput( )
{	m_dataBytes = 0;
	clear( );
	slotUpdateStatusBarText();
}

//	fileSave
//	========

bool OutputWidget::fileSave( const bool saveAs )
{	if( (saveAs || m_firstSave) && !Knmap::getFilePath( "Output save file", m_path, true ))
		return false;

	m_firstSave = false;
	QFile file( m_path );

	if( !file.open( IO_WriteOnly ))
	{	KMessageBox::error( this, QString( i18n( "Could not open \"%1\"\n%2" )).arg( m_path ).arg( file.errorString( )));
		return false;
	}

	QTextStream stream( &file );
	uint noOfParagraphs = paragraphs( );
	uint paragraphNo;

    for( paragraphNo = 0; paragraphNo < noOfParagraphs; paragraphNo++ )
		stream << text( paragraphNo ) << endl;

	file.close( );
	return true;
}

//	fileSaveAs
//	==========

bool OutputWidget::fileSaveAs( )
{	return fileSave( true );
}

//	readProfile
//	===========

void OutputWidget::readProfile( KConfig* config )
{	m_path = config->readPathEntry( "path" );
}

//	saveProfile
//	===========

void OutputWidget::saveProfile( KConfig* config )
{	config->writeEntry( "path", m_path );
}

//	slotUpdateStatusBarText
//	=======================

void OutputWidget::slotUpdateStatusBarText( )
{	QString dataBytes;

	if( m_dataBytes < 1024 )
		dataBytes = QString::number( m_dataBytes ) + " bytes";
	else
	if( m_dataBytes < 1024 * 1024 )
	{	float kiloBytes = m_dataBytes / float( 1024 );

		if( kiloBytes < 10 )
			dataBytes.sprintf( "%.3f KBytes", kiloBytes );
		else
		if( kiloBytes < 100 )
			dataBytes.sprintf( "%.2f KBytes", kiloBytes );
		else
			dataBytes.sprintf( "%.1f KBytes", kiloBytes );
	}
	else
	{	float megaBytes = m_dataBytes / float( 1024 * 1024 );

		if( megaBytes < 10 )
			dataBytes.sprintf( "%.3f MBytes", megaBytes );
		else
		if( megaBytes < 100 )
			dataBytes.sprintf( "%.2f MBytes", megaBytes );
		else
			dataBytes.sprintf( "%.1f MBytes", megaBytes );
	}

	emit( statusBarText( QString( "Output: %1 lines (%2)" ).arg( QString::number( paragraphs( ) - 1 )).arg( dataBytes )));
}
