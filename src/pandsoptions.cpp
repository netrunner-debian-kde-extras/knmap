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

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "pandsoptions.h"
#include "whatsthis.h"

//	constructor
//	===========

PAndSOptions::PAndSOptions( QWidget* parent, const char* name )
: QWidget( parent, name )
{	createLayout( );
}

//	createLayout
//	============

void PAndSOptions::createLayout( )
{	QGridLayout* gridLayout  = new QGridLayout( this );

	m_ackCheckBox            = new QCheckBox( i18n( "TCP ACK (-PA)"        ), this, "ack check"                );
	m_ackLineEdit            = new QLineEdit( this, "ack edit"             );
	m_arpCheckBox            = new QCheckBox( i18n( "ARP (-PR)"            ), this, "arp check"                );
	m_dontPingCheckBox       = new QCheckBox( i18n( "Don't ping (-P0)"     ), this, "dont ping check"          );
	m_echoCheckBox           = new QCheckBox( i18n( "ICMP echo (-PE)"      ), this, "icmp echo check"          );
	m_netmaskCheckBox        = new QCheckBox( i18n( "ICMP netmask (-PM)"   ), this, "icmp netmask check"       );
	m_synCheckBox            = new QCheckBox( i18n( "TCP SYN (-PS)"        ), this, "syn check"                );
	m_synLineEdit            = new QLineEdit( this, "syn edit"             );
	m_timestampCheckBox      = new QCheckBox( i18n( "ICMP timestamp (-PP)" ), this, "icmp timestamp check"     );

	byte row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_ackCheckBox,            row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_ackLineEdit,            row++, 3 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   1 );
	gridLayout->addWidget( m_arpCheckBox,            row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_dontPingCheckBox,       row++, 3 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   1 );
	gridLayout->addWidget( m_echoCheckBox,           row++, 3 );
	byte rowBlock1 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_netmaskCheckBox,        row++, 7 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_synCheckBox,            row++, 7 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_synLineEdit,            row++, 7 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   5 );
	gridLayout->addWidget( m_timestampCheckBox,      row++, 7 );
	byte rowBlock2 = row;

	m_scanFTPBounceCheckBox  = new QCheckBox( i18n( "FTP Bounce (-b)"         ), this, "ftp bounce scan check" );
	m_scanFTPRelayLineEdit   = new QLineEdit( this, "ftp relay edit"          );
	m_scanIdleCheckBox       = new QCheckBox( i18n( "Idle (-sI)"              ), this, "idle scan check"       );
	m_scanListCheckBox       = new QCheckBox( i18n( "List Scan (-sL)"         ), this, "list scan check"       );
	m_scanNullCheckBox       = new QCheckBox( i18n( "Null Scan (-sN)"         ), this, "null scan check"       );
	m_scanPingCheckBox       = new QCheckBox( i18n( "Ping Scan (-sP)"         ), this, "ping scan check"       );
	m_scanProtocolCheckBox   = new QCheckBox( i18n( "Protocol Scan (-sO)"     ), this, "protocol scan check"   );
	m_scanRPCCheckBox        = new QCheckBox( i18n( "RPC (-sR)"               ), this, "rpc scan check"        );
	m_scanFINCheckBox        = new QCheckBox( i18n( "Stealth FIN (-sF)"       ), this, "fin scan check"        );
	m_scanAckCheckBox        = new QCheckBox( i18n( "TCP Ack Scan (-sA)"      ), this, "ack scan check"        );
	m_scanSYNCheckBox        = new QCheckBox( i18n( "TCP SYN (-sS)"           ), this, "syn scan check"        );
	m_scanTCPConnectCheckBox = new QCheckBox( i18n( "TCP Connect (-sT)"       ), this, "connect scan check"    );
	m_scanUDPCheckBox        = new QCheckBox( i18n( "UDP Scan (-sU)"          ), this, "udp scan check"        );
	m_scanVersionCheckBox    = new QCheckBox( i18n( "Version Detection (-sV)" ), this, "version scan check"    );
	m_scanWindowCheckBox     = new QCheckBox( i18n( "Window Scan (-sW)"       ), this, "window scan check"     );
	m_scanXmasCheckBox       = new QCheckBox( i18n( "Xmas Tree (-sX)"         ), this, "xmas scan check"       );
	m_zombieLineEdit         = new QLineEdit( this, "zombie edit"             );

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_scanFTPBounceCheckBox,  row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_scanFTPRelayLineEdit,   row++, 11 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,    9 );
	gridLayout->addWidget( m_scanIdleCheckBox,       row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_zombieLineEdit,         row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_scanListCheckBox,       row++, 11 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,    9 );
	gridLayout->addWidget( m_scanNullCheckBox,       row++, 11 );
	byte rowBlock3 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,   13 );
	gridLayout->addWidget( m_scanPingCheckBox,       row++, 15 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   13 );
	gridLayout->addWidget( m_scanProtocolCheckBox,   row++, 15 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   13 );
	gridLayout->addWidget( m_scanRPCCheckBox,        row++, 15 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   13 );
	gridLayout->addWidget( m_scanFINCheckBox,        row++, 15 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   13 );
	gridLayout->addWidget( m_scanAckCheckBox,        row++, 15 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   13 );
	gridLayout->addWidget( m_scanSYNCheckBox,        row++, 15 );
	byte rowBlock4 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,   17 );
	gridLayout->addWidget( m_scanTCPConnectCheckBox, row++, 19 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   17 );
	gridLayout->addWidget( m_scanUDPCheckBox,        row++, 19 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   17 );
	gridLayout->addWidget( m_scanVersionCheckBox,    row++, 19 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   17 );
	gridLayout->addWidget( m_scanWindowCheckBox,     row++, 19 );
	gridLayout->addWidget( new QLabel( "#", this ),  row,   17 );
	gridLayout->addWidget( m_scanXmasCheckBox,       row++, 19 );
	byte rowBlock5 = row;

	gridLayout->setColStretch(  0,  5 );
	gridLayout->setColStretch(  2,  1 );
	gridLayout->setColStretch(  3, 20 );
	gridLayout->setColStretch(  4,  5 );
	gridLayout->setColStretch(  6,  1 );
	gridLayout->setColStretch(  7, 20 );
	gridLayout->setColStretch(  8,  5 );
	gridLayout->setColStretch( 10,  1 );
	gridLayout->setColStretch( 11, 20 );
	gridLayout->setColStretch( 12,  5 );
	gridLayout->setColStretch( 14,  1 );
	gridLayout->setColStretch( 15, 20 );
	gridLayout->setColStretch( 16,  5 );
	gridLayout->setColStretch( 18,  1 );
	gridLayout->setColStretch( 19, 20 );
	gridLayout->setColStretch( 20,  5 );

	gridLayout->setRowStretch( 0, 10 );
	gridLayout->setRowStretch( _max_( rowBlock1, _max_( rowBlock2, _max_( rowBlock3, _max_( rowBlock4, rowBlock5 )))), 10 );
}

//	finaliseInitialisation
//	======================

void PAndSOptions::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_ackCheckBox,            SIGNAL( clicked( )), SLOT( slotAckClicked( )));
	connect( m_scanFTPBounceCheckBox,  SIGNAL( clicked( )), SLOT( slotScanFTPBounceCheckBoxClicked( )));
	connect( m_scanIdleCheckBox,       SIGNAL( clicked( )), SLOT( slotScanIdleCheckBoxClicked( )));
	connect( m_synCheckBox,            SIGNAL( clicked( )), SLOT( slotSynClicked( )));
	connect( whatsThis,                SIGNAL( clicked( )), SLOT( slotWhatsThisClicked( )));

	connect( m_ackCheckBox,            SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_ackLineEdit,            SIGNAL( textChanged ( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_arpCheckBox,            SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_dontPingCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_echoCheckBox,           SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_netmaskCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanAckCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanFINCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanFTPBounceCheckBox,  SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanFTPRelayLineEdit,   SIGNAL( textChanged ( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_scanIdleCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanListCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanNullCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanPingCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanProtocolCheckBox,   SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanRPCCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanSYNCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanTCPConnectCheckBox, SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanUDPCheckBox,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanVersionCheckBox,    SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanWindowCheckBox,     SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_scanXmasCheckBox,       SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_synCheckBox,            SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_synLineEdit,            SIGNAL( textChanged ( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_timestampCheckBox,      SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_zombieLineEdit,         SIGNAL( textChanged ( const QString& )), SIGNAL( optionsDirty( )));
}

//	getOptions
//	==========

bool PAndSOptions::getOptions( bool& needRoot )
{	m_ackState            = m_ackCheckBox->isChecked( );
	m_ackValue            = m_ackLineEdit->text( );
	m_arpState            = m_arpCheckBox->isChecked( );
	m_dontPingState       = m_dontPingCheckBox->isChecked( );
	m_echoState           = m_echoCheckBox->isChecked( );
	m_netmaskState        = m_netmaskCheckBox->isChecked( );
	m_scanAckState        = m_scanAckCheckBox->isChecked( );
	m_scanFINState        = m_scanFINCheckBox->isChecked( );
	m_scanFTPBounceState  = m_scanFTPBounceCheckBox->isChecked( );
	m_scanFTPRelayHost    = m_scanFTPRelayLineEdit->text( );
	m_scanIdleState       = m_scanIdleCheckBox->isChecked( );
	m_scanListState       = m_scanListCheckBox->isChecked( );
	m_scanNullState       = m_scanNullCheckBox->isChecked( );
	m_scanPingState       = m_scanPingCheckBox->isChecked( );
	m_scanProtocolState   = m_scanProtocolCheckBox->isChecked( );
	m_scanRPCState        = m_scanRPCCheckBox->isChecked( );
	m_scanSYNState        = m_scanSYNCheckBox->isChecked( );
	m_scanTCPConnectState = m_scanTCPConnectCheckBox->isChecked( );
	m_scanUDPState        = m_scanUDPCheckBox->isChecked( );
	m_scanVersionState    = m_scanVersionCheckBox->isChecked( );
	m_scanWindowState     = m_scanWindowCheckBox->isChecked( );
	m_scanXmasState       = m_scanXmasCheckBox->isChecked( );
	m_synState            = m_synCheckBox->isChecked( );
	m_synValue            = m_synLineEdit->text( );
	m_timestampState      = m_timestampCheckBox->isChecked( );
	m_zombieDetails       = m_zombieLineEdit->text( );

	needRoot = needRoot            ||
			   m_arpState          ||
			   m_echoState         ||
			   m_scanAckState      ||
			   m_scanFINState      ||
			   m_scanIdleState     ||
			   m_scanNullState     ||
			   m_scanProtocolState ||
			   m_scanSYNState      ||
			   m_scanUDPState      ||
			   m_scanWindowState   ||
			   m_scanXmasState     ||
			   m_timestampState;

	return true;
}

//	readProfile
//	===========

void PAndSOptions::readProfile( KConfig* config )
{	m_ackState            = config->readBoolEntry( "ackState"            );
	m_ackValue            = config->readEntry(     "ackValue"            );
	m_arpState            = config->readBoolEntry( "arpState"            );
	m_dontPingState       = config->readBoolEntry( "dontPingState"       );
	m_echoState           = config->readBoolEntry( "echoState"           );
	m_netmaskState        = config->readBoolEntry( "netmaskState"        );
	m_scanAckState        = config->readBoolEntry( "scanAckState"        );
	m_scanFINState        = config->readBoolEntry( "scanFINState"        );
	m_scanFTPBounceState  = config->readBoolEntry( "scanFTPBounceState"  );
	m_scanFTPRelayHost    = config->readEntry(     "scanFTPRelayHost"    );
	m_scanIdleState       = config->readBoolEntry( "scanIdleState"       );
	m_scanListState       = config->readBoolEntry( "scanListState"       );
	m_scanNullState       = config->readBoolEntry( "scanNullState"       );
	m_scanPingState       = config->readBoolEntry( "scanPingState"       );
	m_scanProtocolState   = config->readBoolEntry( "scanProtocolState"   );
	m_scanRPCState        = config->readBoolEntry( "scanRPCState"        );
	m_scanSYNState        = config->readBoolEntry( "scanSYNState"        );
	m_scanTCPConnectState = config->readBoolEntry( "scanTCPConnectState" );
	m_scanUDPState        = config->readBoolEntry( "scanUDPState"        );
	m_scanVersionState    = config->readBoolEntry( "scanVersionState"    );
	m_scanWindowState     = config->readBoolEntry( "scanWindowState"     );
	m_scanXmasState       = config->readBoolEntry( "scanXmasState"       );
	m_synState            = config->readBoolEntry( "synState"            );
	m_synValue            = config->readEntry(     "synValue"            );
	m_timestampState      = config->readBoolEntry( "timestampState"      );
	m_zombieDetails       = config->readEntry(     "zombieDetails"       );
}

//	saveProfile
//	===========

void PAndSOptions::saveProfile( KConfig* config )
{	config->writeEntry( "ackState",            m_ackState            );
	config->writeEntry( "ackValue",            m_ackValue            );
	config->writeEntry( "arpState",            m_arpState            );
	config->writeEntry( "dontPingState",       m_dontPingState       );
	config->writeEntry( "echoState",           m_echoState           );
	config->writeEntry( "netmaskState",        m_netmaskState        );
	config->writeEntry( "scanAckState",        m_scanAckState        );
	config->writeEntry( "scanFINState",        m_scanFINState        );
	config->writeEntry( "scanFTPBounceState",  m_scanFTPBounceState  );
	config->writeEntry( "scanFTPRelayHost",    m_scanFTPRelayHost    );
	config->writeEntry( "scanIdleState",       m_scanIdleState       );
	config->writeEntry( "scanListState",       m_scanListState       );
	config->writeEntry( "scanNullState",       m_scanNullState       );
	config->writeEntry( "scanPingState",       m_scanPingState       );
	config->writeEntry( "scanProtocolState",   m_scanProtocolState   );
	config->writeEntry( "scanRPCState",        m_scanRPCState        );
	config->writeEntry( "scanSYNState",        m_scanSYNState        );
	config->writeEntry( "scanTCPConnectState", m_scanTCPConnectState );
	config->writeEntry( "scanUDPState",        m_scanUDPState        );
	config->writeEntry( "scanVersionState",    m_scanVersionState    );
	config->writeEntry( "scanWindowState",     m_scanWindowState     );
	config->writeEntry( "scanXmasState",       m_scanXmasState       );
	config->writeEntry( "synState",            m_synState            );
	config->writeEntry( "synValue",            m_synValue            );
	config->writeEntry( "timestampState",      m_timestampState      );
	config->writeEntry( "zombieDetails",       m_zombieDetails       );
}

//	setInitialValues
//	================

void PAndSOptions::setInitialValues( )
{	m_ackCheckBox->setChecked( m_ackState );
	m_ackLineEdit->setText( m_ackValue );
	m_arpCheckBox->setChecked( m_arpState );
	m_dontPingCheckBox->setChecked( m_dontPingState );
	m_echoCheckBox->setChecked( m_echoState );
	m_netmaskCheckBox->setChecked( m_netmaskState );
	m_scanAckCheckBox->setChecked( m_scanAckState );
	m_scanFINCheckBox->setChecked( m_scanFINState );
	m_scanFTPBounceCheckBox->setChecked( m_scanFTPBounceState );
	m_scanFTPRelayLineEdit->setText( m_scanFTPRelayHost );
	m_scanIdleCheckBox->setChecked( m_scanIdleState );
	m_scanListCheckBox->setChecked( m_scanListState );
	m_scanNullCheckBox->setChecked( m_scanNullState );
	m_scanPingCheckBox->setChecked( m_scanPingState );
	m_scanProtocolCheckBox->setChecked( m_scanProtocolState );
	m_scanRPCCheckBox->setChecked( m_scanRPCState );
	m_scanSYNCheckBox->setChecked( m_scanSYNState );
	m_scanTCPConnectCheckBox->setChecked( m_scanTCPConnectState );
	m_scanUDPCheckBox->setChecked( m_scanUDPState );
	m_scanVersionCheckBox->setChecked( m_scanVersionState );
	m_scanWindowCheckBox->setChecked( m_scanWindowState );
	m_scanXmasCheckBox->setChecked( m_scanXmasState );
	m_synCheckBox->setChecked( m_synState );
	m_synLineEdit->setText( m_synValue );
	m_timestampCheckBox->setChecked( m_timestampState );
	m_zombieLineEdit->setText( m_zombieDetails );

	slotAckClicked( );
	slotScanFTPBounceCheckBoxClicked( );
	slotScanIdleCheckBoxClicked( );
	slotSynClicked( );
}

//	slotAckClicked
//	==============

void PAndSOptions::slotAckClicked( )
{	m_ackLineEdit->setEnabled( m_ackCheckBox->isChecked( ));
}

//	slotScanFTPBounceCheckBoxClicked
//	================================

void PAndSOptions::slotScanFTPBounceCheckBoxClicked( )
{	m_scanFTPRelayLineEdit->setEnabled( m_scanFTPBounceCheckBox->isChecked( ));
}

//	slotScanIdleCheckBoxClicked
//	===========================

void PAndSOptions::slotScanIdleCheckBoxClicked( )
{	m_zombieLineEdit->setEnabled( m_scanIdleCheckBox->isChecked( ));
}

//	slotSynClicked
//	==============

void PAndSOptions::slotSynClicked( )
{	m_synLineEdit->setEnabled( m_synCheckBox->isChecked( ));
}

//	slotWhatsThisClicked
//	====================

void PAndSOptions::slotWhatsThisClicked( )
{		 if( m_ackCheckBox->hasMouse( ))            emit( displayHelp( "-PA" ));
	else if( m_ackLineEdit->hasMouse( ))            emit( displayHelp( "-PA" ));
	else if( m_arpCheckBox->hasMouse( ))            emit( displayHelp( "-PR" ));
	else if( m_dontPingCheckBox->hasMouse( ))       emit( displayHelp( "-P0" ));
	else if( m_echoCheckBox->hasMouse( ))           emit( displayHelp( "-PE" ));
	else if( m_netmaskCheckBox->hasMouse( ))        emit( displayHelp( "-PM" ));
	else if( m_scanAckCheckBox->hasMouse( ))        emit( displayHelp( "-sA" ));
	else if( m_scanFINCheckBox->hasMouse( ))        emit( displayHelp( "-sF" ));
	else if( m_scanFTPBounceCheckBox->hasMouse( ))  emit( displayHelp( "-b"  ));
	else if( m_scanFTPRelayLineEdit->hasMouse( ))   emit( displayHelp( "-b"  ));
	else if( m_scanIdleCheckBox->hasMouse( ))       emit( displayHelp( "-sI" ));
	else if( m_zombieLineEdit->hasMouse( ))         emit( displayHelp( "-sI" ));
	else if( m_scanListCheckBox->hasMouse( ))       emit( displayHelp( "-sL" ));
	else if( m_scanNullCheckBox->hasMouse( ))       emit( displayHelp( "-sN" ));
	else if( m_scanPingCheckBox->hasMouse( ))       emit( displayHelp( "-sP" ));
	else if( m_scanProtocolCheckBox->hasMouse( ))   emit( displayHelp( "-sO" ));
	else if( m_scanRPCCheckBox->hasMouse( ))        emit( displayHelp( "-sR" ));
	else if( m_scanTCPConnectCheckBox->hasMouse( )) emit( displayHelp( "-sT" ));
	else if( m_scanUDPCheckBox->hasMouse( ))        emit( displayHelp( "-sU" ));
	else if( m_scanVersionCheckBox->hasMouse( ))    emit( displayHelp( "-sV" ));
	else if( m_scanWindowCheckBox->hasMouse( ))     emit( displayHelp( "-sW" ));
	else if( m_scanSYNCheckBox->hasMouse( ))        emit( displayHelp( "-sS" ));
	else if( m_scanXmasCheckBox->hasMouse( ))       emit( displayHelp( "-sX" ));
	else if( m_synCheckBox->hasMouse( ))            emit( displayHelp( "-PS" ));
	else if( m_synLineEdit->hasMouse( ))            emit( displayHelp( "-PS" ));
	else if( m_timestampCheckBox->hasMouse( ))      emit( displayHelp( "-PP" ));
	else emit( displayUnknown( ));
}
