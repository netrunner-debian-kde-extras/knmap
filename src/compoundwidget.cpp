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
#include <qiconset.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "compoundwidget.h"
#include "knmap.h"
#include "whatsthis.h"

//	constructor
//	===========

CompoundWidget::CompoundWidget( QWidget* parent, const char* name )
: QWidget( parent, name )
{	createLayout( );
}

//	createLayout
//	============

void CompoundWidget::createLayout( )
{	QGridLayout* gridLayout       = new QGridLayout( this );
	KIconLoader* iconLoader       = KGlobal::iconLoader( );
	QIconSet     clearLeftIconSet = iconLoader->loadIconSet( "clear_left", KIcon::Small );
	QIconSet     fileOpenIconSet  = iconLoader->loadIconSet( "fileopen",   KIcon::Small );

	m_decoyButton           = new QPushButton( clearLeftIconSet, NULL, this, "target file button"  );
	m_excludeButton         = new QPushButton( clearLeftIconSet, NULL, this, "target file button"  );
	m_interfaceButton       = new QPushButton( clearLeftIconSet, NULL, this, "target file button"  );
	m_sourceIPButton        = new QPushButton( clearLeftIconSet, NULL, this, "target file button"  );
	m_spoofMacButton        = new QPushButton( clearLeftIconSet, NULL, this, "target file button"  );

	m_dataDirButton         = new QPushButton( fileOpenIconSet,  NULL, this, "datadir button"      );
	m_excludeFileButton     = new QPushButton( fileOpenIconSet,  NULL, this, "exclude file button" );
	m_targetFileButton      = new QPushButton( fileOpenIconSet,  NULL, this, "target file button"  );

	m_dataDirCheckBox       = new QCheckBox( i18n( "Data directory (--datadir <name>) "   ), this, "datadir checkbox"     );
	m_dataLengthCheckBox    = new QCheckBox( i18n( "Data length (--data_length <value>"   ), this, "data length checkbox" );
	m_debugLevelCheckBox    = new QCheckBox( i18n( "Debug level (-d)"                     ), this, "debug checkbox"       );
	m_decoyCheckBox         = new QCheckBox( i18n( "Decoy hosts (-D <host list>)"         ), this, "decoy checkbox"       );
	m_excludeCheckBox       = new QCheckBox( i18n( "Exclude hosts (--exclude <hosts>) "   ), this, "exclude checkbox"     );
	m_excludeFileCheckBox   = new QCheckBox( i18n( "Exclude file (--excludefile <path>) " ), this, "exclude checkbox"     );
	m_fragmentLevelCheckBox = new QCheckBox( i18n( "Fragment level (-f)"                  ), this, "fragment checkbox"    );
	m_interfaceCheckBox     = new QCheckBox( i18n( "Interface (-e)"                       ), this, "interface checkbox"   );
	m_maxSocketsCheckBox    = new QCheckBox( i18n( "Max sockets (-M <value>)"             ), this, "max sockets checkbox" );
	m_randomIPCheckBox      = new QCheckBox( i18n( "Random IP scan (-iR <value>)"         ), this, "random ip checkbox"   );
	m_sourceIPCheckBox      = new QCheckBox( i18n( "Source IP addr (-S <value>)"          ), this, "source ip checkbox"   );
	m_sourcePortCheckBox    = new QCheckBox( i18n( "Source port (-g <value>)"             ), this, "source port checkbox" );
	m_spoofMacCheckBox      = new QCheckBox( i18n( "Spoof MAC addr (--spoof_mac <value>)" ), this, "spoof mac checkbox"   );
	m_targetFileCheckBox    = new QCheckBox( i18n( "Target hosts file (-iL <path>) "      ), this, "target checkbox"      );
	m_ttlCheckBox           = new QCheckBox( i18n( "TTL (--ttl <value>)"                  ), this, "ttl checkbox"         );

	m_dataDirLineEdit       = new QLineEdit( this, "datadir line edit"      );
	m_decoyLineEdit         = new QLineEdit( this, "decoy edit"             );
	m_excludeLineEdit       = new QLineEdit( this, "exclude line edit"      );
	m_excludeFileLineEdit   = new QLineEdit( this, "exclude file line edit" );
	m_interfaceLineEdit     = new QLineEdit( this, "interface edit"         );
	m_sourceIPLineEdit      = new QLineEdit( this, "source ip edit"         );
	m_spoofMacLineEdit      = new QLineEdit( this, "spoof mac edit"         );
	m_targetFileLineEdit    = new QLineEdit( this, "target file line edit"  );

	m_dataLengthSpinBox     = new QSpinBox( 1,  1460, 128, this, "data length spin box" );
	m_debugLevelSpinBox     = new QSpinBox( 1,     5,   1, this, "debug spin box"       );
	m_fragmentLevelSpinBox  = new QSpinBox( 1,     2,   1, this, "fragment spin box"    );
	m_maxSocketsSpinBox     = new QSpinBox( 1,   999,  10, this, "max sockets spin box" );
	m_randomIPSpinBox       = new QSpinBox( 1,   999,  10, this, "random ip spin box"   );
	m_sourcePortSpinBox     = new QSpinBox( 1, 65535,  64, this, "source port spin box" );
	m_ttlSpinBox            = new QSpinBox( 1,   255,  32, this, "ttl spin box"         );

	byte row = 1;
	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_dataLengthCheckBox,    row,   3 );
	gridLayout->addWidget( m_dataLengthSpinBox,     row++, 4 );

	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_debugLevelCheckBox,    row,   3 );
	gridLayout->addWidget( m_debugLevelSpinBox,     row++, 4 );

	gridLayout->addWidget( new QLabel( "#", this ), row,   1 );
	gridLayout->addWidget( m_fragmentLevelCheckBox, row,   3 );
	gridLayout->addWidget( m_fragmentLevelSpinBox,  row++, 4 );

	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_maxSocketsCheckBox,    row,   3 );
	gridLayout->addWidget( m_maxSocketsSpinBox,     row++, 4 );

	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_randomIPCheckBox,      row,   3 );
	gridLayout->addWidget( m_randomIPSpinBox,       row++, 4 );

	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_sourcePortCheckBox,    row,   3 );
	gridLayout->addWidget( m_sourcePortSpinBox,     row++, 4 );

	gridLayout->addWidget( new QLabel( " ", this ), row,   1 );
	gridLayout->addWidget( m_ttlCheckBox,           row,   3 );
	gridLayout->addWidget( m_ttlSpinBox,            row++, 4 );
	byte rowBlock1 = row;

	row = 1;
	gridLayout->addWidget( new QLabel( "#", this ), row,    6 );
	gridLayout->addWidget( m_decoyCheckBox,         row,    8 );
	gridLayout->addWidget( m_decoyLineEdit,         row,    9 );
	gridLayout->addWidget( m_decoyButton,           row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_excludeCheckBox,       row,    8 );
	gridLayout->addWidget( m_excludeLineEdit,       row,    9 );
	gridLayout->addWidget( m_excludeButton,         row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_interfaceCheckBox,     row,    8 );
	gridLayout->addWidget( m_interfaceLineEdit,     row,    9 );
	gridLayout->addWidget( m_interfaceButton,       row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_sourceIPCheckBox,      row,    8 );
	gridLayout->addWidget( m_sourceIPLineEdit,      row,    9 );
	gridLayout->addWidget( m_sourceIPButton,        row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_spoofMacCheckBox,      row,    8 );
	gridLayout->addWidget( m_spoofMacLineEdit,      row,    9 );
	gridLayout->addWidget( m_spoofMacButton,        row++, 10 );

	row++;
	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_excludeFileCheckBox,   row,    8 );
	gridLayout->addWidget( m_excludeFileLineEdit,   row,    9 );
	gridLayout->addWidget( m_excludeFileButton,     row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_dataDirCheckBox,       row,    8 );
	gridLayout->addWidget( m_dataDirLineEdit,       row,    9 );
	gridLayout->addWidget( m_dataDirButton,         row++, 10 );

	gridLayout->addWidget( new QLabel( " ", this ), row,    6 );
	gridLayout->addWidget( m_targetFileCheckBox,    row,    8 );
	gridLayout->addWidget( m_targetFileLineEdit,    row,    9 );
	gridLayout->addWidget( m_targetFileButton,      row++, 10 );
	byte rowBlock2 = row;

	gridLayout->setColStretch(  0,  4 );
	gridLayout->setColStretch(  2,  1 );
	gridLayout->setColStretch(  4, 20 );
	gridLayout->setColStretch(  5,  4 );
	gridLayout->setColStretch(  7,  1 );
	gridLayout->setColStretch(  9, 20 );
	gridLayout->setColStretch( 11,  4 );

	gridLayout->setRowStretch( 0, 1 );
	gridLayout->setRowStretch( _max_( rowBlock1, rowBlock2 ), 1 );
}

//	finaliseInitialisation
//	======================

void CompoundWidget::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_dataDirButton,         SIGNAL( clicked( )), SLOT( slotDataDirButtonClicked( )));
	connect( m_dataDirCheckBox,       SIGNAL( clicked( )), SLOT( slotDataDirCheckBoxClicked( )));
	connect( m_dataLengthCheckBox,    SIGNAL( clicked( )), SLOT( slotDataLengthCheckBoxClicked( )));
	connect( m_debugLevelCheckBox,    SIGNAL( clicked( )), SLOT( slotDebugLevelCheckBoxClicked( )));
	connect( m_decoyButton,           SIGNAL( clicked( )), SLOT( slotDecoyButtonClicked( )));
	connect( m_decoyCheckBox,         SIGNAL( clicked( )), SLOT( slotDecoyCheckBoxClicked( )));
	connect( m_excludeButton,         SIGNAL( clicked( )), SLOT( slotExcludeButtonClicked( )));
	connect( m_excludeCheckBox,       SIGNAL( clicked( )), SLOT( slotExcludeCheckBoxClicked( )));
	connect( m_excludeFileButton,     SIGNAL( clicked( )), SLOT( slotExcludeFileButtonClicked( )));
	connect( m_excludeFileCheckBox,   SIGNAL( clicked( )), SLOT( slotExcludeFileCheckBoxClicked( )));
	connect( m_fragmentLevelCheckBox, SIGNAL( clicked( )), SLOT( slotFragmentLevelCheckBoxClicked( )));
	connect( m_interfaceButton,       SIGNAL( clicked( )), SLOT( slotInterfaceButtonClicked( )));
	connect( m_interfaceCheckBox,     SIGNAL( clicked( )), SLOT( slotInterfaceCheckBoxClicked( )));
	connect( m_maxSocketsCheckBox,    SIGNAL( clicked( )), SLOT( slotMaxSocketsCheckBoxClicked( )));
	connect( m_randomIPCheckBox,      SIGNAL( clicked( )), SLOT( slotRandomIPCheckBoxClicked( )));
	connect( m_sourceIPButton,        SIGNAL( clicked( )), SLOT( slotSourceIPButtonClicked( )));
	connect( m_sourceIPCheckBox,      SIGNAL( clicked( )), SLOT( slotSourceIPCheckBoxClicked( )));
	connect( m_sourcePortCheckBox,    SIGNAL( clicked( )), SLOT( slotSourcePortCheckBoxClicked( )));
	connect( m_spoofMacButton,        SIGNAL( clicked( )), SLOT( slotSpoofMACButtonClicked( )));
	connect( m_spoofMacCheckBox,      SIGNAL( clicked( )), SLOT( slotSpoofMacCheckBoxClicked( )));
	connect( m_targetFileButton,      SIGNAL( clicked( )), SLOT( slotTargetFileButtonClicked( )));
	connect( m_targetFileCheckBox,    SIGNAL( clicked( )), SLOT( slotTargetFileCheckBoxClicked( )));
	connect( m_ttlCheckBox,           SIGNAL( clicked( )), SLOT( slotTTLCheckBoxClicked( )));
	connect( whatsThis,               SIGNAL( clicked( )), SLOT( slotWhatsThisClicked( )));

	connect( m_dataDirCheckBox,       SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_dataDirLineEdit,       SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_dataLengthCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_dataLengthSpinBox,     SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_debugLevelCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_debugLevelSpinBox,     SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_decoyCheckBox,         SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_decoyLineEdit,         SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_excludeCheckBox,       SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_excludeLineEdit,       SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_excludeFileCheckBox,   SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_excludeFileLineEdit,   SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_fragmentLevelCheckBox, SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_fragmentLevelSpinBox,  SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_interfaceCheckBox,     SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_interfaceLineEdit,     SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_maxSocketsCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxSocketsSpinBox,     SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_randomIPCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_randomIPSpinBox,       SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_sourceIPCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_sourceIPLineEdit,      SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_sourcePortCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_sourcePortSpinBox,     SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
	connect( m_spoofMacCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_spoofMacLineEdit,      SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_targetFileCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_targetFileLineEdit,    SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_ttlCheckBox,           SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_ttlSpinBox,            SIGNAL( valueChanged( int )),           SIGNAL( optionsDirty( )));
}

//	getOptions
//	==========

bool CompoundWidget::getOptions( bool& needRoot )
{	bool    dataDirState     = m_dataDirCheckBox->isChecked( );
	QString dataDirValue     = m_dataDirLineEdit->text( );
	bool    decoyState       = m_decoyCheckBox->isChecked( );
	QString decoyValue       = m_decoyLineEdit->text( );
	bool    excludeState     = m_excludeCheckBox->isChecked( );
	QString excludeValue     = m_excludeLineEdit->text( );
	bool    excludeFileState = m_excludeFileCheckBox->isChecked( );
	QString excludeFileValue = m_excludeFileLineEdit->text( );
	bool    interfaceState   = m_interfaceCheckBox->isChecked( );
	QString interfaceValue   = m_interfaceLineEdit->text( );
	bool    sourceIPState    = m_sourceIPCheckBox->isChecked( );
	QString sourceIPValue    = m_sourceIPLineEdit->text( );
	bool    spoofMacState    = m_spoofMacCheckBox->isChecked( );
	QString spoofMacValue    = m_spoofMacLineEdit->text( );
	bool    targetFileState  = m_targetFileCheckBox->isChecked( );
	QString targetFileValue  = m_targetFileLineEdit->text( );

	if( dataDirState && dataDirValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Data directory name not specified" )), i18n( "Data directory name error" ));
		return false;
	}

	if( decoyState && decoyValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Decoy host list not specified" )), i18n( "Decoy hosts error" ));
		return false;
	}

	if( excludeState && excludeValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Exclude hosts not specified" )), i18n( "Exclude hosts error" ));
		return false;
	}

	if( excludeFileState && excludeFileValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Exclude hosts file name not specified" )), i18n( "Exclude hosts file name error" ));
		return false;
	}

	if( interfaceState && interfaceValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Interface name not specified" )), i18n( "Interface name error" ));
		return false;
	}

	if( sourceIPState && sourceIPValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Source IP address not specified" )), i18n( "Source IP address error" ));
		return false;
	}

	if( spoofMacState && spoofMacValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Spoof MAC address not specified" )), i18n( "Spoof MAC address error" ));
		return false;
	}

	if( targetFileState && targetFileValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Target hosts file name not specified" )), i18n( "Target hosts file name error" ));
		return false;
	}

	m_dataDirState        = dataDirState;
	m_dataDirValue        = dataDirValue;
	m_dataLengthState     = m_dataLengthCheckBox->isChecked( );
	m_dataLengthValue     = m_dataLengthSpinBox->value( );
	m_debugLevelState     = m_debugLevelCheckBox->isChecked( );
	m_debugLevelValue     = m_debugLevelSpinBox->value( );
	m_decoyState          = decoyState;
	m_decoyValue          = decoyValue;
	m_excludeState        = excludeState;
	m_excludeValue        = excludeValue;
	m_excludeFileState    = excludeFileState;
	m_excludeFileValue    = excludeFileValue;
	m_fragmentLevelState  = m_fragmentLevelCheckBox->isChecked( );
	m_fragmentLevelValue  = m_fragmentLevelSpinBox->value( );
	m_interfaceState      = interfaceState;
	m_interfaceValue      = interfaceValue;
	m_maxSocketsState     = m_maxSocketsCheckBox->isChecked( );
	m_maxSocketsValue     = m_maxSocketsSpinBox->value( );
	m_randomIPState       = m_randomIPCheckBox->isChecked( );
	m_randomIPValue       = m_randomIPSpinBox->value( );
	m_sourceIPState       = sourceIPState;
	m_sourceIPValue       = sourceIPValue;
	m_sourcePortState     = m_sourcePortCheckBox->isChecked( );
	m_sourcePortValue     = m_sourcePortSpinBox->value( );
	m_spoofMacState       = spoofMacState;
	m_spoofMacValue       = spoofMacValue;
	m_targetFileState     = targetFileState;
	m_targetFileValue     = targetFileValue;
	m_ttlState            = m_ttlCheckBox->isChecked( );
	m_ttlValue            = m_ttlSpinBox->value( );

	needRoot = needRoot || m_decoyState || m_fragmentLevelState;
	return true;
}

//	readProfile
//	===========

void CompoundWidget::readProfile( KConfig* config )
{	m_dataDirState        = config->readBoolEntry( "dataDirState"        );
	m_dataDirValue        = config->readEntry(     "dataDirValue"        );
	m_dataLengthState     = config->readBoolEntry( "dataLengthState"     );
	m_dataLengthValue     = config->readNumEntry(  "dataLengthValue", 40 );
	m_debugLevelState     = config->readBoolEntry( "debugLevelState"     );
	m_debugLevelValue     = config->readNumEntry(  "debugLevelValue"     );
	m_decoyState          = config->readBoolEntry( "decoyState"          );
	m_decoyValue          = config->readEntry(     "decoyValue"          );
	m_excludeState        = config->readBoolEntry( "excludeState"        );
	m_excludeValue        = config->readEntry(     "excludeValue"        );
	m_excludeFileState    = config->readBoolEntry( "excludeFileState"    );
	m_excludeFileValue    = config->readEntry(     "excludeFileValue"    );
	m_fragmentLevelState  = config->readBoolEntry( "fragmentLevelState"  );
	m_fragmentLevelValue  = config->readNumEntry(  "fragmentLevelValue"  );
	m_interfaceState      = config->readBoolEntry( "interfaceState"      );
	m_interfaceValue      = config->readEntry(     "interfaceValue"      );
	m_maxSocketsState     = config->readBoolEntry( "maxSocketsState"     );
	m_maxSocketsValue     = config->readNumEntry(  "maxSocketsValue"     );
	m_randomIPState       = config->readBoolEntry( "randomIPState"       );
	m_randomIPValue       = config->readNumEntry(  "randomIPValue"       );
	m_sourceIPState       = config->readBoolEntry( "sourceIPState"       );
	m_sourceIPValue       = config->readEntry(     "sourceIPValue"       );
	m_sourcePortState     = config->readBoolEntry( "sourcePortState"     );
	m_sourcePortValue     = config->readNumEntry(  "sourcePortValue"     );
	m_spoofMacState       = config->readBoolEntry( "spoofMacState"       );
	m_spoofMacValue       = config->readEntry(     "spoofMacValue"       );
	m_targetFileState     = config->readBoolEntry( "targetFileState"     );
	m_targetFileValue     = config->readEntry(     "targetFileValue"     );
	m_ttlState            = config->readBoolEntry( "ttlState"            );
	m_ttlValue            = config->readNumEntry(  "ttlValue"            );
}

//	saveProfile
//	===========

void CompoundWidget::saveProfile( KConfig* config )
{	config->writeEntry( "dataDirState",         m_dataDirState        );
	config->writeEntry( "dataDirValue",         m_dataDirValue        );
	config->writeEntry( "dataLengthState",      m_dataLengthState     );
	config->writeEntry( "dataLengthValue",      m_dataLengthValue     );
	config->writeEntry( "debugLevelState",      m_debugLevelState     );
	config->writeEntry( "debugLevelValue",      m_debugLevelValue     );
	config->writeEntry( "decoyState",           m_decoyState          );
	config->writeEntry( "decoyValue",           m_decoyValue          );
	config->writeEntry( "excludeState",         m_excludeState        );
	config->writeEntry( "excludeValue",         m_excludeValue        );
	config->writeEntry( "excludeFileState",     m_excludeFileState    );
	config->writeEntry( "excludeFileValue",     m_excludeFileValue    );
	config->writeEntry( "fragmentLevelState",   m_fragmentLevelState  );
	config->writeEntry( "fragmentLevelValue",   m_fragmentLevelValue  );
	config->writeEntry( "interfaceState",       m_interfaceState      );
	config->writeEntry( "interfaceValue",       m_interfaceValue      );
	config->writeEntry( "maxSocketsState",      m_maxSocketsState     );
	config->writeEntry( "maxSocketsValue",      m_maxSocketsValue     );
	config->writeEntry( "randomIPState",        m_randomIPState       );
	config->writeEntry( "randomIPValue",        m_randomIPValue       );
	config->writeEntry( "sourceIPState",        m_sourceIPState       );
	config->writeEntry( "sourceIPValue",        m_sourceIPValue       );
	config->writeEntry( "sourcePortState",      m_sourcePortState     );
	config->writeEntry( "sourcePortValue",      m_sourcePortValue     );
	config->writeEntry( "spoofMacState",        m_spoofMacState       );
	config->writeEntry( "spoofMacValue",        m_spoofMacValue       );
	config->writeEntry( "targetFileState",      m_targetFileState     );
	config->writeEntry( "targetFileValue",      m_targetFileValue     );
	config->writeEntry( "ttlState",             m_ttlState            );
	config->writeEntry( "ttlValue",             m_ttlValue            );
}

//	setInitialValues
//	================

void CompoundWidget::setInitialValues( )
{	m_dataDirCheckBox->setChecked( m_dataDirState );
	m_dataDirLineEdit->setText( m_dataDirValue );
	m_dataLengthCheckBox->setChecked( m_dataLengthState );
	m_dataLengthSpinBox->setValue( m_dataLengthValue );
	m_debugLevelCheckBox->setChecked( m_debugLevelState );
	m_debugLevelSpinBox->setValue( m_debugLevelValue );
	m_decoyCheckBox->setChecked( m_decoyState );
	m_decoyLineEdit->setText( m_decoyValue );
	m_excludeCheckBox->setChecked( m_excludeState );
	m_excludeLineEdit->setText( m_excludeValue );
	m_excludeFileCheckBox->setChecked( m_excludeFileState );
	m_excludeFileLineEdit->setText( m_excludeFileValue );
	m_fragmentLevelCheckBox->setChecked( m_fragmentLevelState );
	m_fragmentLevelSpinBox->setValue( m_fragmentLevelValue );
	m_interfaceCheckBox->setChecked( m_interfaceState );
	m_interfaceLineEdit->setText( m_interfaceValue );
	m_maxSocketsCheckBox->setChecked( m_maxSocketsState );
	m_maxSocketsSpinBox->setValue( m_maxSocketsValue );
	m_randomIPCheckBox->setChecked( m_randomIPState );
	m_randomIPSpinBox->setValue( m_randomIPValue );
	m_sourceIPCheckBox->setChecked( m_sourceIPState );
	m_sourceIPLineEdit->setText( m_sourceIPValue );
	m_sourcePortCheckBox->setChecked( m_sourcePortState );
	m_sourcePortSpinBox->setValue( m_sourcePortValue );
	m_spoofMacCheckBox->setChecked( m_spoofMacState );
	m_spoofMacLineEdit->setText( m_spoofMacValue );
	m_targetFileCheckBox->setChecked( m_targetFileState );
	m_targetFileLineEdit->setText( m_targetFileValue );
	m_ttlCheckBox->setChecked( m_ttlState );
	m_ttlSpinBox->setValue( m_ttlValue );

	slotDataDirCheckBoxClicked( );
	slotDataLengthCheckBoxClicked( );
	slotDebugLevelCheckBoxClicked( );
	slotDecoyCheckBoxClicked( );
	slotExcludeCheckBoxClicked( );
	slotExcludeFileCheckBoxClicked( );
	slotFragmentLevelCheckBoxClicked( );
	slotInterfaceCheckBoxClicked( );
	slotMaxSocketsCheckBoxClicked( );
	slotRandomIPCheckBoxClicked( );
	slotSourceIPCheckBoxClicked( );
	slotSourcePortCheckBoxClicked( );
	slotSpoofMacCheckBoxClicked( );
	slotTargetFileCheckBoxClicked( );
	slotTTLCheckBoxClicked( );
}

//	slotDataDirButtonClicked
//	========================

void CompoundWidget::slotDataDirButtonClicked( )
{	Knmap::getDirPath( "Data directory", m_dataDirValue, m_dataDirLineEdit );
}

//	slotDataDirCheckBoxClicked
//	==========================

void CompoundWidget::slotDataDirCheckBoxClicked( )
{	m_dataDirButton->setEnabled( m_dataDirCheckBox->isChecked( ));
	m_dataDirLineEdit->setEnabled( m_dataDirCheckBox->isChecked( ));
}

//	slotDataLengthCheckBoxClicked
//	=============================

void CompoundWidget::slotDataLengthCheckBoxClicked( )
{	m_dataLengthSpinBox->setEnabled( m_dataLengthCheckBox->isChecked( ));
}

//	slotDebugLevelCheckBoxClicked
//	=============================

void CompoundWidget::slotDebugLevelCheckBoxClicked( )
{	m_debugLevelSpinBox->setEnabled( m_debugLevelCheckBox->isChecked( ));
}

//	slotDecoyButtonClicked
//	======================

void CompoundWidget::slotDecoyButtonClicked( )
{	m_decoyLineEdit->clear( );
}

//	slotDecoyCheckBoxClicked
//	========================

void CompoundWidget::slotDecoyCheckBoxClicked( )
{	m_decoyButton->setEnabled( m_decoyCheckBox->isChecked( ));
	m_decoyLineEdit->setEnabled( m_decoyCheckBox->isChecked( ));
}

//	slotExcludeButtonClicked
//	========================

void CompoundWidget::slotExcludeButtonClicked( )
{	m_excludeLineEdit->clear( );
}

//	slotExcludeCheckBoxClicked
//	==========================

void CompoundWidget::slotExcludeCheckBoxClicked( )
{	m_excludeButton->setEnabled( m_excludeCheckBox->isChecked( ));
	m_excludeLineEdit->setEnabled( m_excludeCheckBox->isChecked( ));
}

//	slotExcludeFileButtonClicked
//	============================

void CompoundWidget::slotExcludeFileButtonClicked( )
{	Knmap::getFilePath( "Exclude hosts file", m_excludeFileValue, m_excludeFileLineEdit, false );
}

//	slotExcludeFileCheckBoxClicked
//	==============================

void CompoundWidget::slotExcludeFileCheckBoxClicked( )
{	m_excludeFileButton->setEnabled( m_excludeFileCheckBox->isChecked( ));
	m_excludeFileLineEdit->setEnabled( m_excludeFileCheckBox->isChecked( ));
}

//	slotFragmentLevelCheckBoxClicked
//	================================

void CompoundWidget::slotFragmentLevelCheckBoxClicked( )
{	m_fragmentLevelSpinBox->setEnabled( m_fragmentLevelCheckBox->isChecked( ));
}

//	slotInterfaceButtonClicked
//	==========================

void CompoundWidget::slotInterfaceButtonClicked( )
{	m_interfaceLineEdit->clear( );
}

//	slotInterfaceCheckBoxClicked
//	============================

void CompoundWidget::slotInterfaceCheckBoxClicked( )
{	m_interfaceButton->setEnabled( m_interfaceCheckBox->isChecked( ));
	m_interfaceLineEdit->setEnabled( m_interfaceCheckBox->isChecked( ));
}

//	slotMaxSocketsCheckBoxClicked
//	=============================

void CompoundWidget::slotMaxSocketsCheckBoxClicked( )
{	m_maxSocketsSpinBox->setEnabled( m_maxSocketsCheckBox->isChecked( ));
}

//	slotRandomIPCheckBoxClicked
//	===========================

void CompoundWidget::slotRandomIPCheckBoxClicked( )
{	m_randomIPSpinBox->setEnabled( m_randomIPCheckBox->isChecked( ));
}

//	slotSourceIPButtonClicked
//	=========================

void CompoundWidget::slotSourceIPButtonClicked( )
{	m_sourceIPLineEdit->clear( );
}

//	slotSourceIPCheckBoxClicked
//	===========================

void CompoundWidget::slotSourceIPCheckBoxClicked( )
{	m_sourceIPButton->setEnabled( m_sourceIPCheckBox->isChecked( ));
	m_sourceIPLineEdit->setEnabled( m_sourceIPCheckBox->isChecked( ));
}

//	slotSourcePortCheckBoxClicked
//	=============================

void CompoundWidget::slotSourcePortCheckBoxClicked( )
{	m_sourcePortSpinBox->setEnabled( m_sourcePortCheckBox->isChecked( ));
}

//	slotSpoofMACButtonClicked
//	=========================

void CompoundWidget::slotSpoofMACButtonClicked( )
{	m_spoofMacLineEdit->clear( );
}

//	slotSpoofMacCheckBoxClicked
//	===========================

void CompoundWidget::slotSpoofMacCheckBoxClicked( )
{	m_spoofMacButton->setEnabled( m_spoofMacCheckBox->isChecked( ));
	m_spoofMacLineEdit->setEnabled( m_spoofMacCheckBox->isChecked( ));
}

//	slotTargetFileButtonClicked
//	===========================

void CompoundWidget::slotTargetFileButtonClicked( )
{	Knmap::getFilePath( "Target hosts file", m_targetFileValue, m_targetFileLineEdit, false );
}

//	slotTargetFileCheckBoxClicked
//	=============================

void CompoundWidget::slotTargetFileCheckBoxClicked( )
{	m_targetFileButton->setEnabled( m_targetFileCheckBox->isChecked( ));
	m_targetFileLineEdit->setEnabled( m_targetFileCheckBox->isChecked( ));
}

//	slotTTLCheckBoxClicked
//	======================

void CompoundWidget::slotTTLCheckBoxClicked( )
{	m_ttlSpinBox->setEnabled( m_ttlCheckBox->isChecked( ));
}

//	slotWhatsThisClicked
//	====================

void CompoundWidget::slotWhatsThisClicked( )
{		 if( m_dataDirButton->hasMouse( ))         emit( displayHelp( "--datadir"     ));
	else if( m_dataDirCheckBox->hasMouse( ))       emit( displayHelp( "--datadir"     ));
	else if( m_dataDirLineEdit->hasMouse( ))       emit( displayHelp( "--datadir"     ));
	else if( m_dataLengthCheckBox->hasMouse( ))    emit( displayHelp( "--data_length" ));
	else if( m_dataLengthSpinBox->hasMouse( ))     emit( displayHelp( "--data_length" ));
	else if( m_decoyButton->hasMouse( ))           emit( displayHelp( "-D"            ));
	else if( m_decoyCheckBox->hasMouse( ))         emit( displayHelp( "-D"            ));
	else if( m_decoyLineEdit->hasMouse( ))         emit( displayHelp( "-D"            ));
	else if( m_debugLevelCheckBox->hasMouse( ))    emit( displayHelp( "-d"            ));
	else if( m_debugLevelSpinBox->hasMouse( ))     emit( displayHelp( "-d"            ));
	else if( m_excludeButton->hasMouse( ))         emit( displayHelp( "--exclude"     ));
	else if( m_excludeCheckBox->hasMouse( ))       emit( displayHelp( "--exclude"     ));
	else if( m_excludeLineEdit->hasMouse( ))       emit( displayHelp( "--exclude"     ));
	else if( m_excludeFileButton->hasMouse( ))     emit( displayHelp( "--exclude"     ));
	else if( m_excludeFileCheckBox->hasMouse( ))   emit( displayHelp( "--exclude"     ));
	else if( m_excludeFileLineEdit->hasMouse( ))   emit( displayHelp( "--exclude"     ));
	else if( m_fragmentLevelCheckBox->hasMouse( )) emit( displayHelp( "-f"            ));
	else if( m_fragmentLevelSpinBox->hasMouse( ))  emit( displayHelp( "-f"            ));
	else if( m_interfaceButton->hasMouse( ))       emit( displayHelp( "-e"            ));
	else if( m_interfaceCheckBox->hasMouse( ))     emit( displayHelp( "-e"            ));
	else if( m_interfaceLineEdit->hasMouse( ))     emit( displayHelp( "-e"            ));
	else if( m_maxSocketsCheckBox->hasMouse( ))    emit( displayHelp( "-M"            ));
	else if( m_maxSocketsSpinBox->hasMouse( ))     emit( displayHelp( "-M"            ));
	else if( m_randomIPCheckBox->hasMouse( ))      emit( displayHelp( "-iR"           ));
	else if( m_randomIPSpinBox->hasMouse( ))       emit( displayHelp( "-iR"           ));
	else if( m_sourceIPButton->hasMouse( ))        emit( displayHelp( "-S"            ));
	else if( m_sourceIPCheckBox->hasMouse( ))      emit( displayHelp( "-S"            ));
	else if( m_sourceIPLineEdit->hasMouse( ))      emit( displayHelp( "-S"            ));
	else if( m_sourcePortCheckBox->hasMouse( ))    emit( displayHelp( "--source_port" ));
	else if( m_sourcePortSpinBox->hasMouse( ))     emit( displayHelp( "--source_port" ));
	else if( m_spoofMacButton->hasMouse( ))        emit( displayHelp( "--spoof_mac"   ));
	else if( m_spoofMacCheckBox->hasMouse( ))      emit( displayHelp( "--spoof_mac"   ));
	else if( m_spoofMacLineEdit->hasMouse( ))      emit( displayHelp( "--spoof_mac"   ));
	else if( m_targetFileButton->hasMouse( ))      emit( displayHelp( "-iL"           ));
	else if( m_targetFileCheckBox->hasMouse( ))    emit( displayHelp( "-iL"           ));
	else if( m_targetFileLineEdit->hasMouse( ))    emit( displayHelp( "-iL"           ));
	else if( m_ttlCheckBox->hasMouse( ))           emit( displayHelp( "--ttl"         ));
	else if( m_ttlSpinBox->hasMouse( ))            emit( displayHelp( "--ttl"         ));
	else emit( displayUnknown( ));
}
