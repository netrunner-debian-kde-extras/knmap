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

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>

#include "simpleoptions.h"
#include "whatsthis.h"

//	constructor
//	===========

SimpleOptions::SimpleOptions( QWidget* parent, const char* name )
: QWidget( parent, name )
{	createLayout( );
}

//	createLayout
//	============

void SimpleOptions::createLayout( )
{	QGridLayout* gridLayout  = new QGridLayout( this );

	byte row = 1;
	m_allPortsCheckBox       = new QCheckBox( i18n( "All ports (--allports)"              ), this, "all ports check"       );
	m_fastScanCheckBox       = new QCheckBox( i18n( "Fast Scan (-F)"                      ), this, "fast scan check"       );
	m_ipV6CheckBox           = new QCheckBox( i18n( "IPV6 support (-6)"                   ), this, "ipv6 check"            );
	m_privilegedCheckBox     = new QCheckBox( i18n( "Assume privileged (--privileged)"    ), this, "privileged check"      );
	m_noRandomPortsCheckBox  = new QCheckBox( i18n( "No Random Ports (-r)"                ), this, "no random ports check" );
	m_osDetectionCheckBox    = new QCheckBox( i18n( "OS Detection (-O)"                   ), this, "os detection check"    );
	m_osScanLimitCheckBox    = new QCheckBox( i18n( "OS scan limit (--osscan_limit)"      ), this, "os scan limit check"   );
	m_packetTraceCheckBox    = new QCheckBox( i18n( "Packet trace (--packet_trace)"       ), this, "packet trace check"    );
	m_prepackagedCheckBox    = new QCheckBox( i18n( "Prepackaged options (-A)"            ), this, "prepackaged check"    );
	m_randomizeHostsCheckBox = new QCheckBox( i18n( "Randomize hosts (--randomize_hosts)" ), this, "randomize check"       );
	m_sendEthCheckBox        = new QCheckBox( i18n( "Send raw eth. packets (--send_eth)"  ), this, "send eth check"        );
	m_sendIPCheckBox         = new QCheckBox( i18n( "Send raw IP packets (--send_ip)"     ), this, "send ip check"         );
	m_verboseCheckBox        = new QCheckBox( i18n( "Verbose (-v)"                        ), this, "verbose check"         );

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_allPortsCheckBox,       row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_privilegedCheckBox,     row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_fastScanCheckBox,       row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_ipV6CheckBox,           row++, 3 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   1 );
	gridLayout->addWidget( m_noRandomPortsCheckBox,  row++, 3 );
	byte rowBlock1 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( "#", this ),  row,   5 );
	gridLayout->addWidget( m_osDetectionCheckBox,    row++, 7 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_osScanLimitCheckBox,    row++, 7 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_packetTraceCheckBox,    row++, 7 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,   5 );
	gridLayout->addWidget( m_prepackagedCheckBox,    row++, 7 );
	byte rowBlock2 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_randomizeHostsCheckBox, row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_sendEthCheckBox,        row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_sendIPCheckBox,         row++, 11 );
	gridLayout->addWidget( new QLabel( " ", this ),  row,    9 );
	gridLayout->addWidget( m_verboseCheckBox,        row++, 11 );
	byte rowBlock3 = row;

	gridLayout->setColStretch(  0, 8 );
	gridLayout->setColStretch(  2, 1 );
	gridLayout->setColStretch(  4, 8 );
	gridLayout->setColStretch(  6, 1 );
	gridLayout->setColStretch(  8, 8 );
	gridLayout->setColStretch( 10, 1 );
	gridLayout->setColStretch( 12, 8 );

	gridLayout->setRowStretch( 0, 10 );
	gridLayout->setRowStretch( _max_( rowBlock1, _max_( rowBlock2, rowBlock3 )) + 1, 10 );
}

//	finaliseInitialisation
//	======================

void SimpleOptions::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_fastScanCheckBox,       SIGNAL( clicked( )), SLOT( slotFastScanCheckBoxClicked( )));
	connect( whatsThis,                SIGNAL( clicked( )), SLOT( slotWhatsThisClicked( )));

	connect( m_allPortsCheckBox,       SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_fastScanCheckBox,       SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_ipV6CheckBox,           SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_noRandomPortsCheckBox,  SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_osDetectionCheckBox,    SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_osScanLimitCheckBox,    SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_packetTraceCheckBox,    SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_prepackagedCheckBox,    SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_privilegedCheckBox,     SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_randomizeHostsCheckBox, SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_sendEthCheckBox,        SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_sendIPCheckBox,         SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
	connect( m_verboseCheckBox,        SIGNAL( clicked( )), SIGNAL( optionsDirty( )));
}

//	getOptions
//	==========

bool SimpleOptions::getOptions( bool& needRoot )
{	m_allPortsState       = m_allPortsCheckBox->isChecked( );
	m_fastScanState       = m_fastScanCheckBox->isChecked( );
	m_ipV6State           = m_ipV6CheckBox->isChecked( );
	m_noRandomPortsState  = m_noRandomPortsCheckBox->isChecked( );
	m_osDetectionState    = m_osDetectionCheckBox->isChecked( );
	m_osScanLimitState    = m_osScanLimitCheckBox->isChecked( );
	m_packetTraceState    = m_packetTraceCheckBox->isChecked( );
	m_prepackagedState    = m_prepackagedCheckBox->isChecked( );
	m_privilegedState     = m_privilegedCheckBox->isChecked( );
	m_randomizeHostsState = m_randomizeHostsCheckBox->isChecked( );
	m_sendEthState        = m_sendEthCheckBox->isChecked( );
	m_sendIPState         = m_sendIPCheckBox->isChecked( );
	m_verboseState        = m_verboseCheckBox->isChecked( );

	needRoot = needRoot || m_osDetectionState;
	return true;
}

//	readProfile
//	===========

void SimpleOptions::readProfile( KConfig* config )
{	m_allPortsState       = config->readBoolEntry( "allPortsState"       );
	m_fastScanState       = config->readBoolEntry( "fastScanState"       );
	m_ipV6State           = config->readBoolEntry( "ipV6State"           );
	m_noRandomPortsState  = config->readBoolEntry( "noRandomPortsState"  );
	m_osDetectionState    = config->readBoolEntry( "osDetectionState"    );
	m_osScanLimitState    = config->readBoolEntry( "osScanLimitState"    );
	m_packetTraceState    = config->readBoolEntry( "packetTraceState"    );
	m_prepackagedState    = config->readBoolEntry( "prepackagedState"    );
	m_privilegedState     = config->readBoolEntry( "privilegedState"     );
	m_randomizeHostsState = config->readBoolEntry( "randomizeHostsState" );
	m_sendEthState        = config->readBoolEntry( "sendEthState"        );
	m_sendIPState         = config->readBoolEntry( "sendIPState"         );
	m_verboseState        = config->readBoolEntry( "verboseState"        );
}

//	saveProfile
//	===========

void SimpleOptions::saveProfile( KConfig* config )
{	config->writeEntry( "allPortsState",       m_allPortsState       );
	config->writeEntry( "fastScanState",       m_fastScanState       );
	config->writeEntry( "ipV6State",           m_ipV6State           );
	config->writeEntry( "noRandomPortsState",  m_noRandomPortsState  );
	config->writeEntry( "osDetectionState",    m_osDetectionState    );
	config->writeEntry( "osScanLimitState",    m_osScanLimitState    );
	config->writeEntry( "packetTraceState",    m_packetTraceState    );
	config->writeEntry( "prepackagedState",    m_prepackagedState    );
	config->writeEntry( "privilegedState",     m_privilegedState     );
	config->writeEntry( "randomizeHostsState", m_randomizeHostsState );
	config->writeEntry( "sendEthState",        m_sendEthState        );
	config->writeEntry( "sendIPState",         m_sendIPState         );
	config->writeEntry( "verboseState",        m_verboseState        );
}

//	setInitialValues
//	================

void SimpleOptions::setInitialValues( )
{	m_allPortsCheckBox->setChecked( m_allPortsState );
	m_fastScanCheckBox->setChecked( m_fastScanState );
	m_ipV6CheckBox->setChecked( m_ipV6State );
	m_noRandomPortsCheckBox->setChecked( m_noRandomPortsState );
	m_osDetectionCheckBox->setChecked(m_osDetectionState  );
	m_osScanLimitCheckBox->setChecked(m_osScanLimitState  );
	m_packetTraceCheckBox->setChecked( m_packetTraceState );
	m_prepackagedCheckBox->setChecked( m_prepackagedState );
	m_privilegedCheckBox->setChecked( m_privilegedState );
	m_randomizeHostsCheckBox->setChecked( m_randomizeHostsState );
	m_sendEthCheckBox->setChecked( m_sendEthState );
	m_sendIPCheckBox->setChecked( m_sendIPState );
	m_verboseCheckBox->setChecked( m_verboseState );
}

//	slotDisableFastScan
//	===================

void SimpleOptions::slotDisableFastScan( )
{	m_fastScanCheckBox->setChecked( false );
}

//	slotFastScanCheckBoxClicked
//	===========================

void SimpleOptions::slotFastScanCheckBoxClicked( )
{	if( m_fastScanCheckBox->isChecked( ))
		emit( disablePortRanges( ));
}

//	slotWhatsThisClicked
//	====================

void SimpleOptions::slotWhatsThisClicked( )
{		 if( m_allPortsCheckBox->hasMouse( ))       emit( displayHelp( "--allports"        ));
	else if( m_fastScanCheckBox->hasMouse( ))       emit( displayHelp( "-F"                ));
	else if( m_ipV6CheckBox->hasMouse( ))           emit( displayHelp( "-6"                ));
	else if( m_noRandomPortsCheckBox->hasMouse( ))  emit( displayHelp( "-r"                ));
	else if( m_osDetectionCheckBox->hasMouse( ))    emit( displayHelp( "-O"                ));
	else if( m_osScanLimitCheckBox->hasMouse( ))    emit( displayHelp( "--osscan_limit"    ));
	else if( m_packetTraceCheckBox->hasMouse( ))    emit( displayHelp( "--packet_trace"    ));
	else if( m_prepackagedCheckBox->hasMouse( ))    emit( displayHelp( "-A"                ));
	else if( m_privilegedCheckBox->hasMouse( ))     emit( displayHelp( "--privileged"      ));
	else if( m_randomizeHostsCheckBox->hasMouse( )) emit( displayHelp( "--randomize_hosts" ));
	else if( m_sendEthCheckBox->hasMouse( ))        emit( displayHelp( "--send_eth"        ));
	else if( m_sendIPCheckBox->hasMouse( ))         emit( displayHelp( "--send_ip"         ));
	else if( m_verboseCheckBox->hasMouse( ))        emit( displayHelp( "-v"                ));
	else emit( displayUnknown( ));
}
