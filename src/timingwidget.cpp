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

#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qiconset.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "timingwidget.h"
#include "whatsthis.h"

//	constructor
//	===========

TimingWidget::TimingWidget( QWidget* parent, const char* name )
: QWidget( parent, name )
{	createLayout( );
}

//	createLayout
//	============

void TimingWidget::createLayout( )
{	QGridLayout* gridLayout       = new QGridLayout( this );
	KIconLoader* iconLoader       = KGlobal::iconLoader( );
	QIconSet     clearLeftIconSet = iconLoader->loadIconSet( "clear_left", KIcon::Small );

	byte row = 1;
	QButtonGroup* timingGroupBox = new QButtonGroup( 7, Qt::Vertical, "Simple Timing", this, "timing group box" );
	m_defaultRadio               = new QRadioButton( i18n( "Default"          ), timingGroupBox, "default radio button"    );
	m_aggressiveRadio            = new QRadioButton( i18n( "Aggressive (-T4)" ), timingGroupBox, "aggressive radio button" );
	m_insaneRadio                = new QRadioButton( i18n( "Insane (-T5)"     ), timingGroupBox, "insane radio button"     );
	m_normalRadio                = new QRadioButton( i18n( "Normal (-T3)"     ), timingGroupBox, "normal radio button"     );
	m_paranoidRadio              = new QRadioButton( i18n( "Paranoid (-T0)"   ), timingGroupBox, "paranoid radio button"   );
	m_politeRadio                = new QRadioButton( i18n( "Polite (-T2)"     ), timingGroupBox, "polite radio button"     );
	m_sneakyRadio                = new QRadioButton( i18n( "Sneaky (-T1)"     ), timingGroupBox, "sneaky radio button"     );
	gridLayout->addMultiCellWidget( timingGroupBox, row, row + 7, 1, 1 );
	row += 7;
	byte rowBlock1               = row;

	m_hostTimeoutButton          = new QPushButton( clearLeftIconSet, NULL, this, "host timeout button"        );
	m_initialRTTTimeoutButton    = new QPushButton( clearLeftIconSet, NULL, this, "initial rtt timeout button" );
	m_maxHostGroupButton         = new QPushButton( clearLeftIconSet, NULL, this, "max host group button"      );
	m_maxParallelismButton       = new QPushButton( clearLeftIconSet, NULL, this, "max parallelism button"     );
	m_maxRTTTimeoutButton        = new QPushButton( clearLeftIconSet, NULL, this, "max rtt timeout button"     );
	m_maxScanDelayButton         = new QPushButton( clearLeftIconSet, NULL, this, "max scan delay button"      );
	m_minHostGroupButton         = new QPushButton( clearLeftIconSet, NULL, this, "min host group button"      );
	m_minParallelismButton       = new QPushButton( clearLeftIconSet, NULL, this, "min parallelism button"     );
	m_minRTTTimeoutButton        = new QPushButton( clearLeftIconSet, NULL, this, "min rtt timeout button"     );
	m_scanDelayButton            = new QPushButton( clearLeftIconSet, NULL, this, "min scan delay button"      ); 

	m_hostTimeoutCheckBox        = new QCheckBox( i18n( "Host timeout (--host_timeout)"               ), this, "host timeout checkbox" );
	m_initialRTTTimeoutCheckBox  = new QCheckBox( i18n( "Initial RTT timeout (--initial_rtt_timeout)" ), this, "initial rtt timeout checkbox" );
	m_maxHostGroupCheckBox       = new QCheckBox( i18n( "Max Host group (--max_hostgroup)"            ), this, "max host group checkbox" );
	m_maxParallelismCheckBox     = new QCheckBox( i18n( "Max Parallelism (--max_parallelism)"         ), this, "max parallelism checkbox" );
	m_maxRTTTimeoutCheckBox      = new QCheckBox( i18n( "Max RTT timeout (--max_rtt_timeout)"         ), this, "max rtt timeout checkbox" );
	m_maxScanDelayCheckBox       = new QCheckBox( i18n( "Max Scan delay (--max_scan_delay)"           ), this, "max scan delay checkbox" );
	m_minHostGroupCheckBox       = new QCheckBox( i18n( "Min Host group (--min_hostgroup)"            ), this, "min host group checkbox" );
	m_minParallelismCheckBox     = new QCheckBox( i18n( "Min Parallelism (--min_parallelism)"         ), this, "min parallelism checkbox" );
	m_minRTTTimeoutCheckBox      = new QCheckBox( i18n( "Min RTT timeout (--min_rtt_timeout)"         ), this, "min rtt timeout checkbox" );
	m_scanDelayCheckBox          = new QCheckBox( i18n( "Min Scan delay (--scan_delay)"               ), this, "min scan delay checkbox" );

	m_hostTimeoutLineEdit        = new QLineEdit( this, "host timeout line edit"        );
	m_initialRTTTimeoutLineEdit  = new QLineEdit( this, "initial rtt timeout line edit" );
	m_maxHostGroupLineEdit       = new QLineEdit( this, "max host group line edit"      );
	m_maxParallelismLineEdit     = new QLineEdit( this, "max parallelism line edit"     );
	m_maxRTTTimeoutLineEdit      = new QLineEdit( this, "max rtt timeout line edit"     );
	m_maxScanDelayLineEdit       = new QLineEdit( this, "max scan delay line edit"      );
	m_minHostGroupLineEdit       = new QLineEdit( this, "min host group line edit"      );
	m_minParallelismLineEdit     = new QLineEdit( this, "min parallelism line edit"     );
	m_minRTTTimeoutLineEdit      = new QLineEdit( this, "min rtt timeout line edit"     );
	m_scanDelayLineEdit          = new QLineEdit( this, "min scan delay line edit"      );

	m_hostTimeoutLineEdit->setInputMask( "999999" );
	m_initialRTTTimeoutLineEdit->setInputMask( "999999" );
	m_maxHostGroupLineEdit->setInputMask( "999999" );
	m_maxParallelismLineEdit->setInputMask( "999999" );
	m_maxRTTTimeoutLineEdit->setInputMask( "999999" );
	m_maxScanDelayLineEdit->setInputMask( "999999" );
	m_minHostGroupLineEdit->setInputMask( "999999" );
	m_minParallelismLineEdit->setInputMask( "999999" );
	m_minRTTTimeoutLineEdit->setInputMask( "999999" );
	m_scanDelayLineEdit->setInputMask( "999999" );

	row = 1;
	gridLayout->addWidget( m_hostTimeoutCheckBox,       row,   3 );
	gridLayout->addWidget( m_hostTimeoutLineEdit,       row,   4 );
	gridLayout->addWidget( m_hostTimeoutButton,         row++, 5 );

	gridLayout->addWidget( m_maxHostGroupCheckBox,      row,   3 );
	gridLayout->addWidget( m_maxHostGroupLineEdit,      row,   4 );
	gridLayout->addWidget( m_maxHostGroupButton,        row++, 5 );

	gridLayout->addWidget( m_minHostGroupCheckBox,      row,   3 );
	gridLayout->addWidget( m_minHostGroupLineEdit,      row,   4 );
	gridLayout->addWidget( m_minHostGroupButton,        row++, 5 );

	gridLayout->addWidget( m_maxParallelismCheckBox,    row,   3 );
	gridLayout->addWidget( m_maxParallelismLineEdit,    row,   4 );
	gridLayout->addWidget( m_maxParallelismButton,      row++, 5 );

	gridLayout->addWidget( m_minParallelismCheckBox,    row,   3 );
	gridLayout->addWidget( m_minParallelismLineEdit,    row,   4 );
	gridLayout->addWidget( m_minParallelismButton,      row++, 5 );
	byte rowBlock2 = row;

	row = 1;
	gridLayout->addWidget( m_initialRTTTimeoutCheckBox, row,   7 );
	gridLayout->addWidget( m_initialRTTTimeoutLineEdit, row,   8 );
	gridLayout->addWidget( m_initialRTTTimeoutButton,   row++, 9 );

	gridLayout->addWidget( m_maxRTTTimeoutCheckBox,     row,   7 );
	gridLayout->addWidget( m_maxRTTTimeoutLineEdit,     row,   8 );
	gridLayout->addWidget( m_maxRTTTimeoutButton,       row++, 9 );

	gridLayout->addWidget( m_minRTTTimeoutCheckBox,     row,   7 );
	gridLayout->addWidget( m_minRTTTimeoutLineEdit,     row,   8 );
	gridLayout->addWidget( m_minRTTTimeoutButton,       row++, 9 );

	gridLayout->addWidget( m_maxScanDelayCheckBox,      row,   7 );
	gridLayout->addWidget( m_maxScanDelayLineEdit,      row,   8 );
	gridLayout->addWidget( m_maxScanDelayButton,        row++, 9 );

	gridLayout->addWidget( m_scanDelayCheckBox,         row,   7 );
	gridLayout->addWidget( m_scanDelayLineEdit,         row,   8 );
	gridLayout->addWidget( m_scanDelayButton,           row++, 9 );
	byte rowBlock3 = row;

	gridLayout->setColStretch(  0,  3 );
	gridLayout->setColStretch(  2,  3 );
	gridLayout->setColStretch(  4, 10 );
	gridLayout->setColStretch(  6,  3 );
	gridLayout->setColStretch(  8, 10 );
	gridLayout->setColStretch( 10,  3 );

	gridLayout->setRowStretch( 0, 10 );
	gridLayout->setRowStretch( _max_( rowBlock1, _max_( rowBlock2, rowBlock3 )) + 1, 10 );
}

//	finaliseInitialisation
//	======================

void TimingWidget::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_aggressiveRadio,           SIGNAL( clicked( )), SLOT( slotAggressiveRadioClicked( )));
	connect( m_defaultRadio,              SIGNAL( clicked( )), SLOT( slotDefaultRadioClicked( )));
	connect( m_hostTimeoutButton,         SIGNAL( clicked( )), SLOT( slotHostTimeoutButtonClicked( )));
	connect( m_hostTimeoutCheckBox,       SIGNAL( clicked( )), SLOT( slotHostTimeoutCheckBoxClicked( )));
	connect( m_initialRTTTimeoutButton,   SIGNAL( clicked( )), SLOT( slotInitialRTTTimeoutButtonClicked( )));
	connect( m_initialRTTTimeoutCheckBox, SIGNAL( clicked( )), SLOT( slotInitialRTTTimeoutCheckBoxClicked( )));
	connect( m_insaneRadio,               SIGNAL( clicked( )), SLOT( slotInsaneRadioClicked( )));
	connect( m_maxHostGroupButton,        SIGNAL( clicked( )), SLOT( slotMaxHostGroupButtonClicked( )));
	connect( m_maxHostGroupCheckBox,      SIGNAL( clicked( )), SLOT( slotMaxHostGroupCheckBoxClicked( )));
	connect( m_maxParallelismButton,      SIGNAL( clicked( )), SLOT( slotMaxParallelismButtonClicked( )));
	connect( m_maxParallelismCheckBox,    SIGNAL( clicked( )), SLOT( slotMaxParallelismCheckBoxClicked( )));
	connect( m_maxRTTTimeoutButton,       SIGNAL( clicked( )), SLOT( slotMaxRTTTimeoutButtonClicked( )));
	connect( m_maxRTTTimeoutCheckBox,     SIGNAL( clicked( )), SLOT( slotMaxRTTTimeoutCheckBoxClicked( )));
	connect( m_maxScanDelayButton,        SIGNAL( clicked( )), SLOT( slotMaxScanDelayButtonClicked( )));
	connect( m_maxScanDelayCheckBox,      SIGNAL( clicked( )), SLOT( slotMaxScanDelayCheckBoxClicked( )));
	connect( m_minHostGroupButton,        SIGNAL( clicked( )), SLOT( slotMinHostGroupButtonClicked( )));
	connect( m_minHostGroupCheckBox,      SIGNAL( clicked( )), SLOT( slotMinHostGroupCheckBoxClicked( )));
	connect( m_minParallelismButton,      SIGNAL( clicked( )), SLOT( slotMinParallelismButtonClicked( )));
	connect( m_minParallelismCheckBox,    SIGNAL( clicked( )), SLOT( slotMinParallelismCheckBoxClicked( )));
	connect( m_minRTTTimeoutButton,       SIGNAL( clicked( )), SLOT( slotMinRTTTimeoutButtonClicked( )));
	connect( m_minRTTTimeoutCheckBox,     SIGNAL( clicked( )), SLOT( slotMinRTTTimeoutCheckBoxClicked( )));
	connect( m_normalRadio,               SIGNAL( clicked( )), SLOT( slotNormalRadioClicked( )));
	connect( m_paranoidRadio,             SIGNAL( clicked( )), SLOT( slotParanoidRadioClicked( )));
	connect( m_politeRadio,               SIGNAL( clicked( )), SLOT( slotPoliteRadioClicked( )));
	connect( m_scanDelayButton,           SIGNAL( clicked( )), SLOT( slotScanDelayButtonClicked( )));
	connect( m_scanDelayCheckBox,         SIGNAL( clicked( )), SLOT( slotScanDelayCheckBoxClicked( )));
	connect( m_sneakyRadio,               SIGNAL( clicked( )), SLOT( slotSneakyRadioClicked( )));
	connect( whatsThis,                   SIGNAL( clicked( )), SLOT( slotWhatsThisClicked( )));

	connect( m_aggressiveRadio,           SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_defaultRadio,              SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_hostTimeoutCheckBox,       SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_hostTimeoutLineEdit,       SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_initialRTTTimeoutCheckBox, SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_initialRTTTimeoutLineEdit, SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_insaneRadio,               SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxHostGroupCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxHostGroupLineEdit,      SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_maxParallelismCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxParallelismLineEdit,    SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_maxRTTTimeoutCheckBox,     SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxRTTTimeoutLineEdit,     SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_maxScanDelayCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_maxScanDelayLineEdit,      SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_minHostGroupCheckBox,      SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_minHostGroupLineEdit,      SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_minParallelismCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_minParallelismLineEdit,    SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_minRTTTimeoutCheckBox,     SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_minRTTTimeoutLineEdit,     SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_normalRadio,               SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_paranoidRadio,             SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_politeRadio,               SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_scanDelayCheckBox,         SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_scanDelayLineEdit,         SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_sneakyRadio,               SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
}

//	getOptions
//	==========

bool TimingWidget::getOptions( )
{	bool    maxHostGroupState   = m_maxHostGroupCheckBox->isChecked( );
	uint    maxHostGroupValue   = m_maxHostGroupLineEdit->text( ).toUInt( );
	bool    maxParallelismState = m_maxParallelismCheckBox->isChecked( );
	uint    maxParallelismValue = m_maxParallelismLineEdit->text( ).toUInt( );
	bool    maxRTTTimeoutState  = m_maxRTTTimeoutCheckBox->isChecked( );
	uint    maxRTTTimeoutValue  = m_maxRTTTimeoutLineEdit->text( ).toUInt( );
	bool    maxScanDelayState   = m_maxScanDelayCheckBox->isChecked( );
	uint    maxScanDelayValue   = m_maxScanDelayLineEdit->text( ).toUInt( );
	bool    minHostGroupState   = m_minHostGroupCheckBox->isChecked( );
	uint    minHostGroupValue   = m_minHostGroupLineEdit->text( ).toUInt( );
	bool    minParallelismState = m_minParallelismCheckBox->isChecked( );
	uint    minParallelismValue = m_minParallelismLineEdit->text( ).toUInt( );
	bool    minRTTTimeoutState  = m_minRTTTimeoutCheckBox->isChecked( );
	uint    minRTTTimeoutValue  = m_minRTTTimeoutLineEdit->text( ).toUInt( );
	bool    scanDelayState      = m_scanDelayCheckBox->isChecked( );
	uint    scanDelayValue      = m_scanDelayLineEdit->text( ).toUInt( );

	if( minHostGroupValue > maxHostGroupValue )
	{	KMessageBox::error( this, i18n( QString( "Min host group not < Max host group" )), i18n( "Host group error" ));
		return false;
	}

	if( minParallelismValue > maxParallelismValue )
	{	KMessageBox::error( this, i18n( QString( "Min parallelism not < Max parallelism" )), i18n( "Parallelism error" ));
		return false;
	}

	if( minRTTTimeoutValue > maxRTTTimeoutValue )
	{	KMessageBox::error( this, i18n( QString( "Min RTT timeout not < Max RTT timeout" )), i18n( "RTT timeout error" ));
		return false;
	}

	if( scanDelayValue > maxScanDelayValue )
	{	KMessageBox::error( this, i18n( QString( "Min scan delay not < Max scan delay" )), i18n( "Scan delay error" ));
		return false;
	}

	m_hostTimeoutState       = m_hostTimeoutCheckBox->isChecked( );
	m_hostTimeoutValue       = m_hostTimeoutLineEdit->text( ).toUInt( );
	m_initialRTTTimeoutState = m_initialRTTTimeoutCheckBox->isChecked( );
	m_initialRTTTimeoutValue = m_initialRTTTimeoutLineEdit->text( ).toUInt( );
	m_maxHostGroupState      = maxHostGroupState;
	m_maxHostGroupValue      = maxHostGroupValue;
	m_maxParallelismState    = maxParallelismState;
	m_maxParallelismValue    = maxParallelismValue;
	m_maxRTTTimeoutState     = maxRTTTimeoutState;
	m_maxRTTTimeoutValue     = maxRTTTimeoutValue;
	m_maxScanDelayState      = maxScanDelayState;
	m_maxScanDelayValue      = maxScanDelayValue;
	m_minHostGroupState      = minHostGroupState;
	m_minHostGroupValue      = minHostGroupValue;
	m_minParallelismState    = minParallelismState;
	m_minParallelismValue    = minParallelismValue;
	m_minRTTTimeoutState     = minRTTTimeoutState;
	m_minRTTTimeoutValue     = minRTTTimeoutValue;
	m_scanDelayState         = scanDelayState;
	m_scanDelayValue         = scanDelayValue;

	return true;
}

//	readProfile
//	===========

void TimingWidget::readProfile( KConfig* config )
{	m_hostTimeoutState        = config->readBoolEntry( "hostTimeoutState"        );
	m_hostTimeoutValue        = config->readNumEntry(  "hostTimeoutValue"        );
	m_initialRTTTimeoutState  = config->readBoolEntry( "initialRTTTimeoutState"  );
	m_initialRTTTimeoutValue  = config->readNumEntry(  "initialRTTTimeoutValue"  );
	m_maxHostGroupState       = config->readBoolEntry( "maxHostGroupState"       );
	m_maxHostGroupValue       = config->readNumEntry(  "maxHostGroupValue"       );
	m_maxParallelismState     = config->readBoolEntry( "maxParallelismState"     );
	m_maxParallelismValue     = config->readNumEntry(  "maxParallelismValue"     );
	m_maxRTTTimeoutState      = config->readBoolEntry( "maxRTTTimeoutState"      );
	m_maxRTTTimeoutValue      = config->readNumEntry(  "maxRTTTimeoutValue"      );
	m_maxScanDelayState       = config->readBoolEntry( "maxScanDelayState"       );
	m_maxScanDelayValue       = config->readNumEntry(  "maxScanDelayValue"       );
	m_minHostGroupState       = config->readBoolEntry( "minHostGroupState"       );
	m_minHostGroupValue       = config->readNumEntry(  "minHostGroupValue"       );
	m_minParallelismState     = config->readBoolEntry( "minParallelismState"     );
	m_minParallelismValue     = config->readNumEntry(  "minParallelismValue"     );
	m_minRTTTimeoutState      = config->readBoolEntry( "minRTTTimeoutState"      );
	m_minRTTTimeoutValue      = config->readNumEntry(  "minRTTTimeoutValue"      );
	m_scanDelayState          = config->readBoolEntry( "scanDelayState"          );
	m_scanDelayValue          = config->readNumEntry(  "scanDelayValue"          );

	m_simpleTimingValue       = Timing( config->readNumEntry( "simpleTimingValue", Normal_T3_Timing ));
}

//	saveProfile
//	===========

void TimingWidget::saveProfile( KConfig* config )
{	config->writeEntry( "hostTimeoutState",       m_hostTimeoutState       );
	config->writeEntry( "hostTimeoutValue",       m_hostTimeoutValue       );
	config->writeEntry( "initialRTTTimeoutState", m_initialRTTTimeoutState );
	config->writeEntry( "initialRTTTimeoutValue", m_initialRTTTimeoutValue );
	config->writeEntry( "maxHostGroupState",      m_maxHostGroupState      );
	config->writeEntry( "maxHostGroupValue",      m_maxHostGroupValue      );
	config->writeEntry( "maxParallelismState",    m_maxParallelismState    );
	config->writeEntry( "maxParallelismValue",    m_maxParallelismValue    );
	config->writeEntry( "maxRTTTimeoutState",     m_maxRTTTimeoutState     );
	config->writeEntry( "maxRTTTimeoutValue",     m_maxRTTTimeoutValue     );
	config->writeEntry( "maxScanDelayState",      m_maxScanDelayState      );	
	config->writeEntry( "maxScanDelayValue",      m_maxScanDelayValue      );
	config->writeEntry( "minHostGroupState",      m_minHostGroupState      );
	config->writeEntry( "minHostGroupValue",      m_minHostGroupValue      );
	config->writeEntry( "minParallelismState",    m_minParallelismState    );
	config->writeEntry( "minParallelismValue",    m_minParallelismValue    );
	config->writeEntry( "minRTTTimeoutState",     m_minRTTTimeoutState     );
	config->writeEntry( "minRTTTimeoutValue",     m_minRTTTimeoutValue     );
	config->writeEntry( "scanDelayState",         m_scanDelayState         );
	config->writeEntry( "scanDelayValue",         m_scanDelayValue         );
	config->writeEntry( "simpleTimingValue",      m_simpleTimingValue      );
}

//	setInitialValues
//	================

void TimingWidget::setInitialValues( )
{	m_hostTimeoutCheckBox->setChecked( m_hostTimeoutState );
	m_hostTimeoutLineEdit->setEnabled( m_hostTimeoutState );
	m_hostTimeoutLineEdit->setText( QString::number( m_hostTimeoutValue ));
	m_initialRTTTimeoutCheckBox->setChecked( m_initialRTTTimeoutState );
	m_initialRTTTimeoutLineEdit->setEnabled( m_initialRTTTimeoutState );
	m_initialRTTTimeoutLineEdit->setText( QString::number( m_initialRTTTimeoutValue ));
	m_maxHostGroupCheckBox->setChecked( m_maxHostGroupState );
	m_maxHostGroupLineEdit->setEnabled( m_maxHostGroupState );
	m_maxHostGroupLineEdit->setText( QString::number( m_maxHostGroupValue ));
	m_maxParallelismCheckBox->setChecked( m_maxParallelismState );
	m_maxParallelismLineEdit->setEnabled( m_maxParallelismState );
	m_maxParallelismLineEdit->setText( QString::number( m_maxParallelismValue ));
	m_maxRTTTimeoutCheckBox->setChecked( m_maxRTTTimeoutState );
	m_maxRTTTimeoutLineEdit->setEnabled( m_maxRTTTimeoutState );
	m_maxRTTTimeoutLineEdit->setText( QString::number( m_maxRTTTimeoutValue ));
	m_maxScanDelayCheckBox->setChecked( m_maxScanDelayState );
	m_maxScanDelayLineEdit->setEnabled( m_maxScanDelayState );
	m_maxScanDelayLineEdit->setText( QString::number( m_maxScanDelayValue ));
	m_minHostGroupCheckBox->setChecked( m_minHostGroupState );
	m_minHostGroupLineEdit->setEnabled( m_minHostGroupState );
	m_minHostGroupLineEdit->setText( QString::number( m_minHostGroupValue ));
	m_minParallelismCheckBox->setChecked( m_minParallelismState );
	m_minParallelismLineEdit->setEnabled( m_minParallelismState );
	m_minParallelismLineEdit->setText( QString::number( m_minParallelismValue ));
	m_minRTTTimeoutCheckBox->setChecked( m_minRTTTimeoutState );
	m_minRTTTimeoutLineEdit->setEnabled( m_minRTTTimeoutState );
	m_minRTTTimeoutLineEdit->setText( QString::number( m_minRTTTimeoutValue ));
	m_scanDelayCheckBox->setChecked( m_scanDelayState );
	m_scanDelayLineEdit->setEnabled( m_scanDelayState );
	m_scanDelayLineEdit->setText( QString::number( m_scanDelayValue ));

	switch( m_simpleTimingValue )
	{	case Aggressive_T4_Timing: m_aggressiveRadio->setChecked( true ); break;
		case Default_Timing:       m_defaultRadio->setChecked( true );    break;
		case Insane_T5_Timing:     m_insaneRadio->setChecked( true );     break;
		case Normal_T3_Timing:     m_normalRadio->setChecked( true );     break;
		case Paranoid_T0_Timing:   m_paranoidRadio->setChecked( true );   break;
		case Polite_T2_Timing:     m_politeRadio->setChecked( true );     break;
		case Sneaky_T1_Timing:     m_sneakyRadio->setChecked( true );     break;

		default:
			kdDebug( ) << "internal error - unknown simple timing value " << m_simpleTimingValue << endl;
			ASSERT( false );
	}
}

//	simpleTiming
//	============

QString TimingWidget::simpleTiming( ) const
{	switch( m_simpleTimingValue )
	{	case Aggressive_T4_Timing: return "Aggressive";
		case Default_Timing:       return "";
		case Insane_T5_Timing:     return "Insane";
		case Normal_T3_Timing:     return "Normal";
		case Paranoid_T0_Timing:   return "Paranoid";
		case Polite_T2_Timing:     return "Polite";
		case Sneaky_T1_Timing:     return "Sneaky";
	}

	kdDebug( ) << "internal error - unknown simple timing value " << m_simpleTimingValue << endl;
	ASSERT( false );
	return QString::null;
}

//	slotAggressiveRadioClicked
//	==========================

void TimingWidget::slotAggressiveRadioClicked( )
{	m_simpleTimingValue = Aggressive_T4_Timing;
}

//	slotDefaultRadioClicked
//	=======================

void TimingWidget::slotDefaultRadioClicked( )
{	m_simpleTimingValue = Default_Timing;
}

//	slotHostTimeoutButtonClicked
//	============================

void TimingWidget::slotHostTimeoutButtonClicked( )
{	m_hostTimeoutLineEdit->clear( );
}

//	slotHostTimeoutCheckBoxClicked
//	==============================

void TimingWidget::slotHostTimeoutCheckBoxClicked( )
{	m_hostTimeoutButton->setEnabled( m_hostTimeoutCheckBox->isChecked( ));
	m_hostTimeoutLineEdit->setEnabled( m_hostTimeoutCheckBox->isChecked( ));
}

//	slotInitialRTTTimeoutButtonClicked
//	==================================

void TimingWidget::slotInitialRTTTimeoutButtonClicked( )
{	m_initialRTTTimeoutLineEdit->clear( );
}

//	slotInitialRTTTimeoutCheckBoxClicked
//	====================================

void TimingWidget::slotInitialRTTTimeoutCheckBoxClicked( )
{	m_initialRTTTimeoutButton->setEnabled( m_initialRTTTimeoutCheckBox->isChecked( ));
	m_initialRTTTimeoutLineEdit->setEnabled( m_initialRTTTimeoutCheckBox->isChecked( ));
}

//	slotInsaneRadioClicked
//	======================

void TimingWidget::slotInsaneRadioClicked( )
{	m_simpleTimingValue = Insane_T5_Timing;
}

//	slotMaxHostGroupButtonClicked
//	=============================

void TimingWidget::slotMaxHostGroupButtonClicked( )
{	m_maxHostGroupLineEdit->clear( );
}

//	slotMaxHostGroupCheckBoxClicked
//	===============================

void TimingWidget::slotMaxHostGroupCheckBoxClicked( )
{	m_maxHostGroupButton->setEnabled( m_maxHostGroupCheckBox->isChecked( ));
	m_maxHostGroupLineEdit->setEnabled( m_maxHostGroupCheckBox->isChecked( ));
}

//	slotMaxParallelismButtonClicked
//	===============================

void TimingWidget::slotMaxParallelismButtonClicked( )
{	m_maxParallelismLineEdit->clear( );
}

//	slotMaxParallelismCheckBoxClicked
//	=================================

void TimingWidget::slotMaxParallelismCheckBoxClicked( )
{	m_maxParallelismButton->setEnabled( m_maxParallelismCheckBox->isChecked( ));
	m_maxParallelismLineEdit->setEnabled( m_maxParallelismCheckBox->isChecked( ));
}

//	slotMaxRTTTimeoutButtonClicked
//	==============================

void TimingWidget::slotMaxRTTTimeoutButtonClicked( )
{	m_maxRTTTimeoutLineEdit->clear( );
}

//	slotMaxRTTTimeoutCheckBoxClicked
//	================================

void TimingWidget::slotMaxRTTTimeoutCheckBoxClicked( )
{	m_maxRTTTimeoutButton->setEnabled( m_maxRTTTimeoutCheckBox->isChecked( ));
	m_maxRTTTimeoutLineEdit->setEnabled( m_maxRTTTimeoutCheckBox->isChecked( ));
}

//	slotMaxScanDelayButtonClicked
//	=============================

void TimingWidget::slotMaxScanDelayButtonClicked( )
{	m_maxScanDelayLineEdit->clear( );
}

//	slotMaxScanDelayCheckBoxClicked
//	===============================

void TimingWidget::slotMaxScanDelayCheckBoxClicked( )
{	m_maxScanDelayButton->setEnabled( m_maxScanDelayCheckBox->isChecked( ));
	m_maxScanDelayLineEdit->setEnabled( m_maxScanDelayCheckBox->isChecked( ));
}

//	slotMinHostGroupButtonClicked
//	=============================

void TimingWidget::slotMinHostGroupButtonClicked( )
{	m_minHostGroupLineEdit->clear( );
}

//	slotMinHostGroupCheckBoxClicked
//	===============================

void TimingWidget::slotMinHostGroupCheckBoxClicked( )
{	m_minHostGroupButton->setEnabled( m_minHostGroupCheckBox->isChecked( ));
	m_minHostGroupLineEdit->setEnabled( m_minHostGroupCheckBox->isChecked( ));
}

//	slotMinParallelismButtonClicked
//	===============================

void TimingWidget::slotMinParallelismButtonClicked( )
{	m_minParallelismLineEdit->clear( );
}

//	slotMinParallelismCheckBoxClicked
//	=================================

void TimingWidget::slotMinParallelismCheckBoxClicked( )
{	m_minParallelismButton->setEnabled( m_minParallelismCheckBox->isChecked( ));
	m_minParallelismLineEdit->setEnabled( m_minParallelismCheckBox->isChecked( ));
}

//	slotMinRTTTimeoutButtonClicked
//	==============================

void TimingWidget::slotMinRTTTimeoutButtonClicked( )
{	m_minRTTTimeoutLineEdit->clear( );
}

//	slotMinRTTTimeoutCheckBoxClicked
//	================================

void TimingWidget::slotMinRTTTimeoutCheckBoxClicked( )
{	m_minRTTTimeoutButton->setEnabled( m_minRTTTimeoutCheckBox->isChecked( ));
	m_minRTTTimeoutLineEdit->setEnabled( m_minRTTTimeoutCheckBox->isChecked( ));
}

//	slotNormalRadioClicked
//	======================

void TimingWidget::slotNormalRadioClicked( )
{	m_simpleTimingValue = Normal_T3_Timing;
}

//	slotParanoidRadioClicked
//	========================

void TimingWidget::slotParanoidRadioClicked( )
{	m_simpleTimingValue = Paranoid_T0_Timing;
}

//	slotPoliteRadioClicked
//	======================

void TimingWidget::slotPoliteRadioClicked( )
{	m_simpleTimingValue = Polite_T2_Timing;
}

//	slotScanDelayButtonClicked
//	==========================

void TimingWidget::slotScanDelayButtonClicked( )
{	m_scanDelayLineEdit->clear( );
}

//	slotScanDelayCheckBoxClicked
//	============================

void TimingWidget::slotScanDelayCheckBoxClicked( )
{	m_scanDelayButton->setEnabled( m_scanDelayCheckBox->isChecked( ));
	m_scanDelayLineEdit->setEnabled( m_scanDelayCheckBox->isChecked( ));
}

//	slotSneakyRadioClicked
//	======================

void TimingWidget::slotSneakyRadioClicked( )
{	m_simpleTimingValue = Sneaky_T1_Timing;
}

//	slotWhatsThisClicked
//	====================

void TimingWidget::slotWhatsThisClicked( )
{		 if( m_aggressiveRadio->hasMouse( ))           emit( displayHelp( "-T"                    ));
	else if( m_defaultRadio->hasMouse( ))              emit( displayDocBook( "timing_options"     ));
	else if( m_hostTimeoutButton->hasMouse( ))         emit( displayHelp( "--host_timeout"        ));
	else if( m_hostTimeoutCheckBox->hasMouse( ))       emit( displayHelp( "--host_timeout"        ));
	else if( m_hostTimeoutLineEdit->hasMouse( ))       emit( displayHelp( "--host_timeout"        ));
	else if( m_initialRTTTimeoutButton->hasMouse( ))   emit( displayHelp( "--initial_rtt_timeout" ));
	else if( m_initialRTTTimeoutCheckBox->hasMouse( )) emit( displayHelp( "--initial_rtt_timeout" ));
	else if( m_initialRTTTimeoutLineEdit->hasMouse( )) emit( displayHelp( "--initial_rtt_timeout" ));
	else if( m_insaneRadio->hasMouse( ))               emit( displayHelp( "-T"                    ));
	else if( m_maxHostGroupButton->hasMouse( ))        emit( displayHelp( "--max_hostgroup"       ));
	else if( m_maxHostGroupCheckBox->hasMouse( ))      emit( displayHelp( "--max_hostgroup"       ));
	else if( m_maxHostGroupLineEdit->hasMouse( ))      emit( displayHelp( "--max_hostgroup"       ));
	else if( m_maxParallelismButton->hasMouse( ))      emit( displayHelp( "--max_parallelism"     ));
	else if( m_maxParallelismCheckBox->hasMouse( ))    emit( displayHelp( "--max_parallelism"     ));
	else if( m_maxParallelismLineEdit->hasMouse( ))    emit( displayHelp( "--max_parallelism"     ));
	else if( m_maxRTTTimeoutButton->hasMouse( ))       emit( displayHelp( "--max_rtt_timeout"     ));
	else if( m_maxRTTTimeoutCheckBox->hasMouse( ))     emit( displayHelp( "--max_rtt_timeout"     ));
	else if( m_maxRTTTimeoutLineEdit->hasMouse( ))     emit( displayHelp( "--max_rtt_timeout"     ));
	else if( m_maxScanDelayButton->hasMouse( ))        emit( displayHelp( "--max_scan_delay"      ));
	else if( m_maxScanDelayCheckBox->hasMouse( ))      emit( displayHelp( "--max_scan_delay"      ));
	else if( m_maxScanDelayLineEdit->hasMouse( ))      emit( displayHelp( "--max_scan_delay"      ));
	else if( m_minHostGroupButton->hasMouse( ))        emit( displayHelp( "--min_hostgroup"       ));
	else if( m_minHostGroupCheckBox->hasMouse( ))      emit( displayHelp( "--min_hostgroup"       ));
	else if( m_minHostGroupLineEdit->hasMouse( ))      emit( displayHelp( "--min_hostgroup"       ));
	else if( m_minParallelismButton->hasMouse( ))      emit( displayHelp( "--min_parallelism"     ));
	else if( m_minParallelismCheckBox->hasMouse( ))    emit( displayHelp( "--min_parallelism"     ));
	else if( m_minParallelismLineEdit->hasMouse( ))    emit( displayHelp( "--min_parallelism"     ));
	else if( m_minRTTTimeoutButton->hasMouse( ))       emit( displayHelp( "--min_rtt_timeout"     ));
	else if( m_minRTTTimeoutCheckBox->hasMouse( ))     emit( displayHelp( "--min_rtt_timeout"     ));
	else if( m_minRTTTimeoutLineEdit->hasMouse( ))     emit( displayHelp( "--min_rtt_timeout"     ));
	else if( m_normalRadio->hasMouse( ))               emit( displayHelp( "-T"                    ));
	else if( m_paranoidRadio->hasMouse( ))             emit( displayHelp( "-T"                    ));
	else if( m_politeRadio->hasMouse( ))               emit( displayHelp( "-T"                    ));
	else if( m_scanDelayButton->hasMouse( ))           emit( displayHelp( "--scan_delay"          ));
	else if( m_scanDelayCheckBox->hasMouse( ))         emit( displayHelp( "--scan_delay"          ));
	else if( m_scanDelayLineEdit->hasMouse( ))         emit( displayHelp( "--scan_delay"          ));
	else if( m_sneakyRadio->hasMouse( ))               emit( displayHelp( "-T"                    ));
	else emit( displayUnknown( ));
}
