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

#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmap.h>
#include <qpopupmenu.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "profiledialog.h"

//	constructor
//	===========

ProfileDialog::ProfileDialog( Action         action,
							  const QString& currentProfile,
							  QWidget*       parent,
							  const char*    name )
: KDialogBase( Plain, "", Ok | Cancel, Ok, parent, name ),
  m_action( action )
{	switch( m_action )
	{	case Copy:   setCaption( "Copy profile"    ); break;
		case Delete: setCaption( "Delete profile"  ); break;
		case Load:   setCaption( "Load profile"    ); break;
		case Rename: setCaption( "Rename profile"  ); break;
		case SaveAs: setCaption( "Save profile as" ); break;
	}

	createlayout( );
	setInitialValues( currentProfile );

	connect( m_profileListBox, SIGNAL( contextMenuRequested( QListBoxItem*, const QPoint& )), SLOT( slotProfileListContextMenu( QListBoxItem*, const QPoint& )));
	connect( m_profileListBox, SIGNAL( doubleClicked( QListBoxItem* )), SLOT( slotOk( )));
	connect( m_profileListBox, SIGNAL( selectionChanged( )), SLOT( slotProfileListBoxChanged( )));
}

//	checkForDuplicateName
//	=====================

bool ProfileDialog::checkForDuplicateName( const QString& profileName )
{	if( !kapp->config( )->hasGroup( PROFILE_PREFIX + profileName ))
		return true;

	KMessageBox::sorry( this, QString( i18n( "Profile \"%1\" already exists, please choose another name or press 'Cancel'" )).arg( profileName ), i18n( "Duplicate profile name" ));
	return false;
}

//	constructNewProfileName
//	=======================

QString ProfileDialog::constructNewProfileName( const QString& initialProfileName ) const
{	QString profileName = PROFILE_PREFIX + initialProfileName + "_copy";

	while( kapp->config( )->hasGroup( profileName ))
		profileName += "_copy";

	return stripPrefix( profileName );
}

//	copy
//	====

void ProfileDialog::copy( const QString& fromProfileName, const QString& toProfileName )
{	QMap<QString, QString> map = kapp->config( )->entryMap( PROFILE_PREFIX + fromProfileName );
	kapp->config( )->setGroup( PROFILE_PREFIX + toProfileName );

	for( QMap<QString, QString>::Iterator it = map.begin( ); it != map.end( ); ++it )
		kapp->config( )->writeEntry( it.key( ), it.data( ));

	m_profileListBox->insertItem( toProfileName );
	m_profileListBox->sort( );
}

//	createlayout
//	============

void ProfileDialog::createlayout( )
{	QGridLayout* layout = new QGridLayout( plainPage( ));
	m_profileListBox    = new QListBox( plainPage( ),  "profiles list box"  );
	m_profileLineEdit   = new QLineEdit( plainPage( ), "profiles line edit" );

	layout->addWidget( new QLabel( "Profile name: ", plainPage( ), "name label" ), 1, 1, Qt::AlignRight );
	layout->addWidget( m_profileLineEdit, 1, 2 );
	layout->addWidget( new QLabel( "Known profiles: ", plainPage( ), "name label" ), 3, 1, Qt::AlignRight | Qt::AlignTop );
	layout->addWidget( m_profileListBox,  3, 2 );

	layout->setColStretch( 0,  1 );
	layout->setColStretch( 2, 20 );
	layout->setColStretch( 3,  1 );

	layout->setRowStretch( 0,  2 );
	layout->setRowStretch( 2,  1 );
	layout->setRowStretch( 3, 10 );
	layout->setRowStretch( 4,  2 );

	switch( m_action )
	{	case Delete:
		case Load:
			m_profileLineEdit->setReadOnly( true );
			break;

		default:
			break;
	}
}

//	deelete
//	=======

void ProfileDialog::deelete( const QString& profileName, const bool ask )
{	if( !kapp->config( )->hasGroup( PROFILE_PREFIX + profileName ))
	{	ASSERT( false );
		return ;
	}

	QListBoxItem* item = m_profileListBox->findItem( profileName, Qt::ExactMatch );
	ASSERT( item != NULL );

	if( item != NULL )
		deelete( item, ask );
}

//	deelete
//	=======

void ProfileDialog::deelete( QListBoxItem* item, const bool ask )
{	if( ask && KMessageBox::Yes != KMessageBox::questionYesNo( this,
															   QString( i18n( "Delete profile \"%1\"?" )).arg( item->text( )),
															   i18n( "Confirm profile deletion" )))
		return;

	kapp->config( )->deleteGroup( PROFILE_PREFIX + item->text( ));
	m_profileListBox->takeItem( item );
	delete item;
}

//	getNewProfileName
//	=================

bool ProfileDialog::getNewProfileName( QString& profileName )
{	while( true )
	{	bool ok;
		profileName = KInputDialog::getText( i18n( "Profile name" ), i18n( "New profile name" ), profileName, &ok, this, "profile name" );

		if( !ok )
			return false;

		if( checkForDuplicateName( profileName ))
			break;
	}

	return true;
}

//	setInitialValues
//	================

void ProfileDialog::setInitialValues( const QString& currentProfile )
{	ushort      currentItem = ushort( -1 );
	QStringList profileList = kapp->config( )->groupList( );
	ushort      i;
	QStringList::Iterator it;

	for( i = 0, it = profileList.begin( ); it != profileList.end( ); ++it )
		if( (*it).startsWith( PROFILE_PREFIX ))
		{	m_profileListBox->insertItem( stripPrefix( *it ), i );

			if( *it == currentProfile )
				currentItem = i;

			i++;
		}

	if( currentItem != ushort( -1 ))
	{	m_profileListBox->setSelected( currentItem, true );

		if( m_action == Copy )
			m_profileLineEdit->setText( constructNewProfileName( m_profileListBox->currentText( )));
		else
			m_profileLineEdit->setText( m_profileListBox->currentText( ));
	}

	m_profileListBox->sort( );
}

//	slotCopy
//	========

void ProfileDialog::slotCopy( )
{	QString fromProfileName = m_contextItem->text( );
	QString toProfileName   = constructNewProfileName( fromProfileName );

	if( getNewProfileName( toProfileName ))
		copy( fromProfileName, toProfileName );
}

//	slotDelete
//	==========

void ProfileDialog::slotDelete( )
{	deelete( m_contextItem, true );
}

//	slotOk
//	======

void ProfileDialog::slotOk( )
{	QString       fromProfileName;
	QListBoxItem* item;
	QString       toProfileName;

	switch( m_action )
	{	case Copy:
			fromProfileName = m_profileListBox->currentText( );
			toProfileName   = m_profileLineEdit->text( );

			if( !checkForDuplicateName( toProfileName ))
				return;

			copy( fromProfileName, toProfileName );
			break;

		case Delete:
			deelete( m_profileLineEdit->text( ), true );
			break;

		case Load:
			m_profileName = PROFILE_PREFIX + m_profileLineEdit->text( );
			break;

		case Rename:
			fromProfileName = m_profileListBox->currentText( );
			toProfileName   = m_profileLineEdit->text( );

			if( !checkForDuplicateName( toProfileName ))
				return;

			copy( fromProfileName, toProfileName );
			deelete( fromProfileName, false );
			break;

		case SaveAs:
			m_profileName = m_profileLineEdit->text( );
			item          = m_profileListBox->findItem( m_profileName, Qt::ExactMatch );

			if( item != NULL )
				if( KMessageBox::Yes != KMessageBox::questionYesNo( this, QString( i18n( "Profile \"%1\" already exists - overwrite it?" )).arg( m_profileName ), i18n( "Profile exists" )))
					return;

			m_profileName = PROFILE_PREFIX + m_profileName;
			break;
	}

	KDialogBase::slotOk( );
}

//	slotProfileListBoxChanged
//	=========================

void ProfileDialog::slotProfileListBoxChanged( )
{	if( m_action == Copy )
		m_profileLineEdit->setText( constructNewProfileName( m_profileListBox->currentText( )));
	else
		m_profileLineEdit->setText( m_profileListBox->currentText( ));
}

//	slotProfileListContextMenu
//	==========================

void ProfileDialog::slotProfileListContextMenu( QListBoxItem* item, const QPoint& pos )
{	m_contextItem = item;
	QPopupMenu* contextMenu = new QPopupMenu( this, "context menu" );

	contextMenu->insertItem( i18n( "&Copy"   ), this, SLOT( slotCopy( )));
	contextMenu->insertItem( i18n( "&Delete" ), this, SLOT( slotDelete( )));
	contextMenu->insertItem( i18n( "&Rename" ), this, SLOT( slotRename( )));

	contextMenu->exec( pos );
}

//	slotRename
//	==========

void ProfileDialog::slotRename( )
{	QString profileName = m_contextItem->text( );

	if( getNewProfileName( profileName ))
	{	copy( m_contextItem->text( ), profileName );
		deelete( m_contextItem, false );
	}
}

//	stripPrefix
//	===========

QString ProfileDialog::stripPrefix( const QString& profileName ) const
{	return profileName.right( profileName.length( ) - strlen( PROFILE_PREFIX ));
}
