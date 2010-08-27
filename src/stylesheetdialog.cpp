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

#include <qiconset.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>

#include <kdebug.h>
#include <kfiledialog.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "stylesheetdialog.h"

//	constructor
//	===========

StylesheetDialog::StylesheetDialog( const QString&     currentStylesheetURL,
									const QStringList& stylesheetURLs,
									QWidget*           parent,
									const char*        name )
: KDialogBase( Plain, "'man' stylesheet location", Ok | Cancel, Ok, parent, name ),
  m_currentStylesheetURL( currentStylesheetURL ),
  m_stylesheetURLs( stylesheetURLs )
{	QGridLayout* layout          = new QGridLayout( plainPage( ));
	KIconLoader* iconLoader      = KGlobal::iconLoader( );
	QIconSet     fileOpenIconSet = iconLoader->loadIconSet( "fileopen",   KIcon::Small );
	QPushButton* urlButton       = new QPushButton( fileOpenIconSet, NULL, plainPage( ), "stylesheet url button" );
	m_urlLineEdit                = new QLineEdit( plainPage( ), "stylesheet url line edit" );
	m_urlListBox                 = new QListBox( plainPage( ), "stylesheet url list box" );

	m_urlListBox->insertStringList( stylesheetURLs );
	m_urlLineEdit->setMinimumWidth( int( 1.1 * m_urlListBox->maxItemWidth( )));
	m_urlLineEdit->setText( currentStylesheetURL );

	layout->addWidget( new QLabel( "Stylesheet URL: ", plainPage( ), "url label" ), 1, 1, Qt::AlignRight );
	layout->addWidget( m_urlLineEdit, 1, 2 );
	layout->addWidget( urlButton,     1, 3 );
	layout->addWidget( new QLabel( "Known URLs: ", plainPage( ), "url label" ), 3, 1, Qt::AlignRight | Qt::AlignTop );
	layout->addWidget( m_urlListBox,  3, 2 );

	layout->setColStretch( 0,  1 );
	layout->setColStretch( 2, 20 );
	layout->setColStretch( 4,  1 );

	layout->setRowStretch( 0,  2 );
	layout->setRowStretch( 2,  1 );
	layout->setRowStretch( 3, 10 );
	layout->setRowStretch( 4,  2 );

	connect( urlButton,    SIGNAL( clicked( )),                     SLOT( slotURLButtonClicked( )));
	connect( m_urlListBox, SIGNAL( doubleClicked( QListBoxItem* )), SLOT( slotURLListBoxDoubleClicked( QListBoxItem* )));
}

//	slotOk
//	======

void StylesheetDialog::slotOk( )
{	QString currentStylesheetURL = m_urlLineEdit->text( );
	QStringList::Iterator it = m_stylesheetURLs.find( currentStylesheetURL );

	if( it != m_stylesheetURLs.end( ))
		m_stylesheetURLs.remove( it );

	if( !QFile::exists( currentStylesheetURL ))
	{	KMessageBox::sorry( this, QString( i18n( "The stylesheet \"%1\" does not exist" )).arg( currentStylesheetURL ), i18n( "Nonexistant stylesheet" ));
		QListBoxItem* item = m_urlListBox->findItem( currentStylesheetURL, Qt::ExactMatch );

		if( item != NULL )
		{	m_urlListBox->removeItem( m_urlListBox->index( item ));
			emit( stylesheetRemoved( ));
		}

		m_urlLineEdit->clear( );
		return;
	}

	m_stylesheetURLs.push_front( currentStylesheetURL );
	KDialogBase::slotOk( );
}

//	slotURLButtonClicked
//	====================

void StylesheetDialog::slotURLButtonClicked( )
{	QString filter        = "*.css|CSS files\n*.*|All files";
	QString startDir      = (m_currentStylesheetURL.isEmpty( )) ? KGlobalSettings::documentPath( ) : m_currentStylesheetURL;
	QString stylesheetURL = KFileDialog::getOpenFileName( startDir, filter, this, "url path dlg" );

	if( stylesheetURL.isEmpty( ))
		return;

	m_urlLineEdit->setText( stylesheetURL );
}

//	slotURLListBoxDoubleClicked
//	===========================

void StylesheetDialog::slotURLListBoxDoubleClicked( QListBoxItem* item )
{	m_urlLineEdit->setText( item->text( ));
	slotOk( );
}
