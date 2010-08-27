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

#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "scannamedialog.h"
#include "scanwidget.h"

//	constructor
//	===========

ScanNameDialog::ScanNameDialog( ScanWidget* scanWidget, QWidget* parent, const char* name )
: KDialogBase( Plain, "Scan name", Ok | Cancel, Ok, parent, name )
{	createlayout( );

	if( scanWidget != NULL )
	{	m_scanNameEdit->setText( scanWidget->scanName( ));
		m_useHostNameCheckbox->setChecked( scanWidget->useTargetHost( ));
	}

	if( !m_useHostNameCheckbox->isChecked( ))
		m_scanNameEdit->setFocus( );
}

//	createlayout
//	============

void ScanNameDialog::createlayout( )
{	QGridLayout* gridLayout = new QGridLayout( plainPage( ));
	byte row = 1;

	m_useHostNameCheckbox = new QCheckBox( "Use Target Host name", plainPage( ), "use target checkbox" );
	gridLayout->addWidget( m_useHostNameCheckbox, row++, 1 );

	byte editRowTop = row++;
	m_scanNameEdit = new QLineEdit( plainPage( ), "name edit" );
	gridLayout->addWidget( new QLabel( "Scan name", plainPage( ), "name label" ), row++, 1 );
	gridLayout->addWidget( m_scanNameEdit, row++, 1 );

	gridLayout->setColStretch( 0, 1 );
	gridLayout->setColStretch( 1, 5 );
	gridLayout->setColStretch( 2, 1 );

	gridLayout->setRowStretch( 0, 1 );
	gridLayout->setRowStretch( editRowTop, 1 );
	gridLayout->setRowStretch( row, 1 );

	m_useHostNameCheckbox->setChecked( true );
	slotUseHostNameToggled( true );
	connect( m_useHostNameCheckbox, SIGNAL( toggled( bool )), SLOT( slotUseHostNameToggled( bool )));
}

//	slotOk
//	======

void ScanNameDialog::slotOk( )
{	m_scanName    = m_scanNameEdit->text( );
	m_useHostName = m_useHostNameCheckbox->isChecked( );

	if( !m_useHostName && m_scanName.isEmpty( ))
	{	KMessageBox::sorry( this, "Please enter a name or select \"Use Target Host name\"", "No scan name" );
		return;
	}

	KDialogBase::slotOk( );
}

//	slotUseHostNameToggled
//	======================

void ScanNameDialog::slotUseHostNameToggled( bool on )
{	m_scanNameEdit->setEnabled( !on );

	if( !on )
		m_scanNameEdit->setFocus( );
}
