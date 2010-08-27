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
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "knmap.h"
#include "loggingoptions.h"
#include "whatsthis.h"

//	constructor
//	===========

LoggingOptions::LoggingOptions( QWidget* parent, const char* name )
: QWidget( parent, name )
{	createLayout( );
}

//	createLayout
//	============

void LoggingOptions::createLayout( )
{	QGridLayout* gridLayout      = new QGridLayout( this );
	KIconLoader* iconLoader      = KGlobal::iconLoader( );
	QIconSet     fileOpenIconSet = iconLoader->loadIconSet( "fileopen",   KIcon::Small );

	m_baseFileNameButton   = new QPushButton( fileOpenIconSet, NULL, this, "base file name button" );
	m_grepableLogButton    = new QPushButton( fileOpenIconSet, NULL, this, "grepable log button"   );
	m_normalLogButton      = new QPushButton( fileOpenIconSet, NULL, this, "normal log button"     );
	m_resumeButton         = new QPushButton( fileOpenIconSet, NULL, this, "resume button"         );
	m_scriptKiddieButton   = new QPushButton( fileOpenIconSet, NULL, this, "script kiddie button"  );
	m_stylesheetButton     = new QPushButton( fileOpenIconSet, NULL, this, "stylesheet button"     );
	m_xmlLogButton         = new QPushButton( fileOpenIconSet, NULL, this, "xml log button"        );

	m_appendOutputCheckBox = new QCheckBox( i18n( "Append output (--append_output)"       ), this, "append output checkbox" );
	m_baseFileNameCheckBox = new QCheckBox( i18n( "Base file name (-oA <name>) "          ), this, "base name checkbox"     );
	m_grepableLogCheckBox  = new QCheckBox( i18n( "grep'able log file (-oG <file>) "      ), this, "grepable log checkbox"  );
	m_normalLogCheckBox    = new QCheckBox( i18n( "Normal log file (-oN <file>) "         ), this, "normal log checkbox"    );
	m_noStylesheetCheckBox = new QCheckBox( i18n( "No XSL stylesheet (--no_stylesheet)"   ), this, "no stylesheet checkbox" );
	m_resumeCheckBox       = new QCheckBox( i18n( "Resume file (--resume <file>) "        ), this, "resume checkbox"        );
	m_scriptKiddieCheckBox = new QCheckBox( i18n( "Script kiddie log file (-oS <file>) "  ), this, "script kiddie checkbox" );
	m_stylesheetCheckBox   = new QCheckBox( i18n( "XSL stylesheet (--stylesheet <file>) " ), this, "stylesheet checkbox"    );
	m_xmlLogCheckBox       = new QCheckBox( i18n( "XML log file (-oX <file>) "            ), this, "xml log checkbox"       );

	m_baseFileNameLineEdit = new QLineEdit( this, "base file name line edit" );
	m_grepableLogLineEdit  = new QLineEdit( this, "grepable log line edit"   );
	m_normalLogLineEdit    = new QLineEdit( this, "normal log line edit"     );
	m_resumeLineEdit       = new QLineEdit( this, "resume line edit"         );
	m_scriptKiddieLineEdit = new QLineEdit( this, "script kiddie line edit"  );
	m_stylesheetLineEdit   = new QLineEdit( this, "stylesheet line edit"     );
	m_xmlLogLineEdit       = new QLineEdit( this, "xml log line edit"        );

	byte row = 1;
	gridLayout->addWidget( m_appendOutputCheckBox, row++, 1 );
	gridLayout->addWidget( m_noStylesheetCheckBox, row++, 1 );
	byte rowBlock1 = row;

	row = 1;
	gridLayout->addWidget( m_baseFileNameCheckBox, row,   3 );
	gridLayout->addWidget( m_baseFileNameLineEdit, row,   4 );
	gridLayout->addWidget( m_baseFileNameButton,   row++, 5 );

	gridLayout->addWidget( m_grepableLogCheckBox,  row,   3 );
	gridLayout->addWidget( m_grepableLogLineEdit,  row,   4 );
	gridLayout->addWidget( m_grepableLogButton,    row++, 5 );

	gridLayout->addWidget( m_normalLogCheckBox,    row,   3 );
	gridLayout->addWidget( m_normalLogLineEdit,    row,   4 );
	gridLayout->addWidget( m_normalLogButton,      row++, 5 );

	gridLayout->addWidget( m_resumeCheckBox,       row,   3 );
	gridLayout->addWidget( m_resumeLineEdit,       row,   4 );
	gridLayout->addWidget( m_resumeButton,         row++, 5 );

	gridLayout->addWidget( m_scriptKiddieCheckBox, row,   3 );
	gridLayout->addWidget( m_scriptKiddieLineEdit, row,   4 );
	gridLayout->addWidget( m_scriptKiddieButton,   row++, 5 );

	gridLayout->addWidget( m_xmlLogCheckBox,       row,   3 );
	gridLayout->addWidget( m_xmlLogLineEdit,       row,   4 );
	gridLayout->addWidget( m_xmlLogButton,         row++, 5 );

	gridLayout->addWidget( m_stylesheetCheckBox,   row,   3 );
	gridLayout->addWidget( m_stylesheetLineEdit,   row,   4 );
	gridLayout->addWidget( m_stylesheetButton,     row++, 5 );
	byte rowBlock2 = row;

	gridLayout->setColStretch( 0,  5 );
	gridLayout->setColStretch( 2,  5 );
	gridLayout->setColStretch( 4, 10 );
	gridLayout->setColStretch( 6,  5 );

	gridLayout->setRowStretch( 0, 10 );
	gridLayout->setRowStretch( _max_( rowBlock1, rowBlock2 ), 10 );
}

//	finaliseInitialisation
//	======================

void LoggingOptions::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_baseFileNameButton,   SIGNAL( clicked( )), SLOT( slotBaseFileNameButtonClicked( )));
	connect( m_baseFileNameCheckBox, SIGNAL( clicked( )), SLOT( slotBaseFileNameCheckBoxClicked( )));
	connect( m_grepableLogButton,    SIGNAL( clicked( )), SLOT( slotGrepableLogButtonClicked( )));
	connect( m_grepableLogCheckBox,  SIGNAL( clicked( )), SLOT( slotGrepableLogCheckBoxClicked( )));
	connect( m_normalLogButton,      SIGNAL( clicked( )), SLOT( slotNormalLogButtonClicked( )));
	connect( m_normalLogCheckBox,    SIGNAL( clicked( )), SLOT( slotNormalLogCheckBoxClicked( )));
	connect( m_noStylesheetCheckBox, SIGNAL( clicked( )), SLOT( slotNoStylesheetCheckBoxClicked( )));
	connect( m_resumeButton,         SIGNAL( clicked( )), SLOT( slotResumeButtonClicked( )));
	connect( m_resumeCheckBox,       SIGNAL( clicked( )), SLOT( slotResumeCheckBoxClicked( )));
	connect( m_scriptKiddieButton,   SIGNAL( clicked( )), SLOT( slotScriptKiddieButtonClicked( )));
	connect( m_scriptKiddieCheckBox, SIGNAL( clicked( )), SLOT( slotScriptKiddieCheckBoxClicked( )));
	connect( m_stylesheetButton,     SIGNAL( clicked( )), SLOT( slotStylesheetButtonClicked( )));
	connect( m_stylesheetCheckBox,   SIGNAL( clicked( )), SLOT( slotStylesheetCheckBoxClicked( )));
	connect( whatsThis,              SIGNAL( clicked( )), SLOT( slotWhatsThisClicked( )));
	connect( m_xmlLogButton,         SIGNAL( clicked( )), SLOT( slotXMLLogButtonClicked( )));
	connect( m_xmlLogCheckBox,       SIGNAL( clicked( )), SLOT( slotXMLLogCheckBoxClicked( )));

	connect( m_appendOutputCheckBox, SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_baseFileNameCheckBox, SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_baseFileNameLineEdit, SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_grepableLogCheckBox,  SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_grepableLogLineEdit,  SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_normalLogCheckBox,    SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_normalLogLineEdit,    SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_resumeCheckBox,       SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_resumeLineEdit,       SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_scriptKiddieCheckBox, SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_scriptKiddieLineEdit, SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_stylesheetCheckBox,   SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_stylesheetLineEdit,   SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
	connect( m_xmlLogCheckBox,       SIGNAL( clicked( )),                    SIGNAL( optionsDirty( )));
	connect( m_xmlLogLineEdit,       SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));
}

//	getOptions
//	==========

bool LoggingOptions::getOptions( )
{	bool    baseFileNameState   = m_baseFileNameCheckBox->isChecked( );
	QString baseFileNameValue   = m_baseFileNameLineEdit->text( );
	bool    grepableLogState    = m_grepableLogCheckBox->isChecked( );
	QString grepableLogValue    = m_grepableLogLineEdit->text( );
	bool    normalLogState      = m_normalLogCheckBox->isChecked( );
	QString normalLogValue      = m_normalLogLineEdit->text( );
	bool    resumeState         = m_resumeCheckBox->isChecked( );
	QString resumeValue         = m_resumeLineEdit->text( );
	bool    scriptKiddieState   = m_scriptKiddieCheckBox->isChecked( );
	QString scriptKiddieValue   = m_scriptKiddieLineEdit->text( );
	bool    stylesheetState     = m_stylesheetCheckBox->isChecked( );
	QString stylesheetValue     = m_stylesheetLineEdit->text( );
	bool    xmlLogState         = m_xmlLogCheckBox->isChecked( );
	QString xmlLogValue         = m_xmlLogLineEdit->text( );

	if( baseFileNameState && baseFileNameValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Base file name not specified" )), i18n( "Base file name error" ));
		return false;
	}

	if( grepableLogState && grepableLogValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "grep'able log file name not specified" )), i18n( "grep'able log file name error" ));
		return false;
	}

	if( normalLogState && normalLogValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Normal log file name not specified" )), i18n( "Normal log file name error" ));
		return false;
	}

	if( resumeState && scriptKiddieValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Resume file name not specified" )), i18n( "Resume file name error" ));
		return false;
	}

	if( scriptKiddieState && scriptKiddieValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Script kiddie log file name not specified" )), i18n( "Script kiddie log file name error" ));
		return false;
	}

	if( stylesheetState && stylesheetValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "XSL stylesheet file name not specified" )), i18n( "XSL stylesheet file name error" ));
		return false;
	}

	if( xmlLogState && xmlLogValue.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "XML log file name not specified" )), i18n( "XML log file name error" ));
		return false;
	}

	m_appendOutputState = m_appendOutputCheckBox->isChecked( );
	m_baseFileNameState = baseFileNameState;
	m_baseFileNameValue = baseFileNameValue;
	m_grepableLogState  = grepableLogState;
	m_grepableLogValue  = grepableLogValue;
	m_normalLogState    = normalLogState;
	m_normalLogValue    = normalLogValue;
	m_noStylesheetState = m_noStylesheetCheckBox->isChecked( );
	m_resumeValue       = resumeValue;
	m_scriptKiddieState = scriptKiddieState;
	m_scriptKiddieValue = scriptKiddieValue;
	m_stylesheetState   = stylesheetState;
	m_stylesheetValue   = stylesheetValue;
	m_xmlLogState       = xmlLogState;
	m_xmlLogValue       = xmlLogValue;

	return true;
}

//	readProfile
//	===========

void LoggingOptions::readProfile( KConfig* config )
{	m_appendOutputState = config->readBoolEntry( "appendOutputState"   );
	m_baseFileNameState = config->readBoolEntry( "baseFileNameState"   );
	m_baseFileNameValue = config->readEntry(     "baseFileNameValue"   );
	m_grepableLogState  = config->readBoolEntry( "grepableLogState"    );
	m_grepableLogValue  = config->readEntry(     "grepableLogValue"    );
	m_normalLogState    = config->readBoolEntry( "normalLogState"      );
	m_normalLogValue    = config->readEntry(     "normalLogValue"      );
	m_noStylesheetState = config->readBoolEntry( "noStylesheetState"   );
	m_resumeState       = config->readBoolEntry( "resumeState"         );
	m_resumeValue       = config->readEntry(     "resumeValue"         );
	m_scriptKiddieState = config->readBoolEntry( "scriptKiddieState"   );
	m_scriptKiddieValue = config->readEntry(     "scriptKiddieValue"   );
	m_stylesheetState   = config->readBoolEntry( "stylesheetState"     );
	m_stylesheetValue   = config->readEntry(     "stylesheetValue"     );
	m_xmlLogState       = config->readBoolEntry( "xmlLogState"         );
	m_xmlLogValue       = config->readEntry(     "xmlLogValue"         );
}

//	saveProfile
//	===========

void LoggingOptions::saveProfile( KConfig* config )
{	config->writeEntry( "appendOutputState", m_appendOutputState );
	config->writeEntry( "baseFileNameState", m_baseFileNameState );
	config->writeEntry( "baseFileNameValue", m_baseFileNameValue );
	config->writeEntry( "grepableLogState",  m_grepableLogState  );
	config->writeEntry( "grepableLogValue",  m_grepableLogValue  );
	config->writeEntry( "normalLogState",    m_normalLogState    );
	config->writeEntry( "normalLogValue",    m_normalLogValue    );
	config->writeEntry( "noStylesheetState", m_noStylesheetState );
	config->writeEntry( "resumeState",       m_resumeState       );
	config->writeEntry( "resumeValue",       m_resumeValue       );
	config->writeEntry( "scriptKiddieState", m_scriptKiddieState );
	config->writeEntry( "scriptKiddieValue", m_scriptKiddieValue );
	config->writeEntry( "stylesheetState",   m_stylesheetState   );
	config->writeEntry( "stylesheetValue",   m_stylesheetValue   );
	config->writeEntry( "xmlLogState",       m_xmlLogState       );
	config->writeEntry( "xmlLogValue",       m_xmlLogValue       );
}

//	setInitialValues
//	================

void LoggingOptions::setInitialValues( )
{	m_appendOutputCheckBox->setChecked( m_appendOutputState );
	m_baseFileNameCheckBox->setChecked( m_baseFileNameState );
	m_baseFileNameLineEdit->setText( m_baseFileNameValue );
	m_grepableLogCheckBox->setChecked( m_grepableLogState );
	m_grepableLogLineEdit->setText( m_grepableLogValue );
	m_normalLogCheckBox->setChecked( m_normalLogState );
	m_normalLogLineEdit->setText( m_normalLogValue );
	m_noStylesheetCheckBox->setChecked( m_noStylesheetState );
	m_resumeCheckBox->setChecked( m_resumeState );
	m_resumeLineEdit->setText( m_resumeValue );
	m_scriptKiddieCheckBox->setChecked( m_scriptKiddieState );
	m_scriptKiddieLineEdit->setText( m_scriptKiddieValue );
	m_stylesheetCheckBox->setChecked( m_stylesheetState );
	m_stylesheetLineEdit->setText( m_stylesheetValue );
	m_xmlLogCheckBox->setChecked( m_xmlLogState );
	m_xmlLogLineEdit->setText( m_xmlLogValue );

	slotBaseFileNameCheckBoxClicked( );
	slotGrepableLogCheckBoxClicked( );
	slotNormalLogCheckBoxClicked( );
	slotResumeCheckBoxClicked( );
	slotScriptKiddieCheckBoxClicked( );
	slotStylesheetCheckBoxClicked( );
	slotXMLLogCheckBoxClicked( );
}

//	slotBaseFileNameButtonClicked
//	=============================

void LoggingOptions::slotBaseFileNameButtonClicked( )
{	Knmap::getFilePath( "Base file name", m_baseFileNameValue, m_baseFileNameLineEdit, false );
}

//	slotBaseFileNameCheckBoxClicked
//	===============================

void LoggingOptions::slotBaseFileNameCheckBoxClicked( )
{	m_baseFileNameButton->setEnabled( m_baseFileNameCheckBox->isChecked( ));
	m_baseFileNameLineEdit->setEnabled( m_baseFileNameCheckBox->isChecked( ));
}

//	slotGrepableLogButtonClicked
//	============================

void LoggingOptions::slotGrepableLogButtonClicked( )
{	Knmap::getFilePath( "grep'able log file", m_grepableLogValue, m_grepableLogLineEdit, true );
}

//	slotGrepableLogCheckBoxClicked
//	==============================

void LoggingOptions::slotGrepableLogCheckBoxClicked( )
{	m_grepableLogButton->setEnabled( m_grepableLogCheckBox->isChecked( ));
	m_grepableLogLineEdit->setEnabled( m_grepableLogCheckBox->isChecked( ));
}

//	slotNormalLogButtonClicked
//	==========================

void LoggingOptions::slotNormalLogButtonClicked( )
{	Knmap::getFilePath( "Normal log file", m_normalLogValue, m_normalLogLineEdit, true );
}

//	slotNormalLogCheckBoxClicked
//	============================

void LoggingOptions::slotNormalLogCheckBoxClicked( )
{	m_normalLogButton->setEnabled( m_normalLogCheckBox->isChecked( ));
	m_normalLogLineEdit->setEnabled( m_normalLogCheckBox->isChecked( ));
}

//	slotNoStylesheetCheckBoxClicked
//	===============================

void LoggingOptions::slotNoStylesheetCheckBoxClicked( )
{	if( m_noStylesheetCheckBox->isChecked( ))
	{	m_stylesheetCheckBox->setChecked( false );
		slotStylesheetCheckBoxClicked( );
	}
}

//	slotResumeButtonClicked
//	=======================

void LoggingOptions::slotResumeButtonClicked( )
{	Knmap::getFilePath( "Resume file", m_resumeValue, m_resumeLineEdit, false );
}

//	slotResumeCheckBoxClicked
//	=========================

void LoggingOptions::slotResumeCheckBoxClicked( )
{	m_resumeButton->setEnabled( m_resumeCheckBox->isChecked( ));
	m_resumeLineEdit->setEnabled( m_resumeCheckBox->isChecked( ));
}

//	slotScriptKiddieButtonClicked
//	=============================

void LoggingOptions::slotScriptKiddieButtonClicked( )
{	Knmap::getFilePath( "Script kiddie log file", m_scriptKiddieValue, m_scriptKiddieLineEdit, true );
}

//	slotScriptKiddieCheckBoxClicked
//	===============================

void LoggingOptions::slotScriptKiddieCheckBoxClicked( )
{	m_scriptKiddieButton->setEnabled( m_scriptKiddieCheckBox->isChecked( ));
	m_scriptKiddieLineEdit->setEnabled( m_scriptKiddieCheckBox->isChecked( ));
}

//	slotStylesheetButtonClicked
//	===========================

void LoggingOptions::slotStylesheetButtonClicked( )
{	Knmap::getFilePath( "Stylesheet file", m_stylesheetValue, m_stylesheetLineEdit, false );
}

//	slotStylesheetCheckBoxClicked
//	=============================

void LoggingOptions::slotStylesheetCheckBoxClicked( )
{	m_stylesheetButton->setEnabled( m_stylesheetCheckBox->isChecked( ));
	m_stylesheetLineEdit->setEnabled( m_stylesheetCheckBox->isChecked( ));

	if( m_stylesheetCheckBox->isChecked( ))
		m_noStylesheetCheckBox->setChecked( false );
}

//	slotWhatsThisClicked
//	====================

void LoggingOptions::slotWhatsThisClicked( )
{		 if( m_appendOutputCheckBox->hasMouse( )) emit( displayHelp( "--append_output" ));
	else if( m_baseFileNameButton->hasMouse( ))   emit( displayHelp( "-oA"             ));
	else if( m_baseFileNameCheckBox->hasMouse( )) emit( displayHelp( "-oA"             ));
	else if( m_baseFileNameLineEdit->hasMouse( )) emit( displayHelp( "-oA"             ));
	else if( m_grepableLogButton->hasMouse( ))    emit( displayHelp( "-oG"             ));
	else if( m_grepableLogCheckBox->hasMouse( ))  emit( displayHelp( "-oG"             ));
	else if( m_grepableLogLineEdit->hasMouse( ))  emit( displayHelp( "-oG"             ));
	else if( m_normalLogButton->hasMouse( ))      emit( displayHelp( "-oN"             ));
	else if( m_normalLogCheckBox->hasMouse( ))    emit( displayHelp( "-oN"             ));
	else if( m_normalLogLineEdit->hasMouse( ))    emit( displayHelp( "-oN"             ));
	else if( m_noStylesheetCheckBox->hasMouse( )) emit( displayHelp( "--no_stylesheet" ));
	else if( m_resumeButton->hasMouse( ))         emit( displayHelp( "--resume"        ));
	else if( m_resumeCheckBox->hasMouse( ))       emit( displayHelp( "--resume"        ));
	else if( m_resumeLineEdit->hasMouse( ))       emit( displayHelp( "--resume"        ));
	else if( m_scriptKiddieButton->hasMouse( ))   emit( displayHelp( "-oS"             ));
	else if( m_scriptKiddieCheckBox->hasMouse( )) emit( displayHelp( "-oS"             ));
	else if( m_scriptKiddieLineEdit->hasMouse( )) emit( displayHelp( "-oS"             ));
	else if( m_stylesheetButton->hasMouse( ))     emit( displayHelp( "--stylesheet"    ));
	else if( m_stylesheetCheckBox->hasMouse( ))   emit( displayHelp( "--stylesheet"    ));
	else if( m_stylesheetLineEdit->hasMouse( ))   emit( displayHelp( "--stylesheet"    ));
	else if( m_xmlLogButton->hasMouse( ))         emit( displayHelp( "-oX"             ));
	else if( m_xmlLogCheckBox->hasMouse( ))       emit( displayHelp( "-oX"             ));
	else if( m_xmlLogLineEdit->hasMouse( ))       emit( displayHelp( "-oX"             ));
	else emit( displayUnknown( ));
}

//	slotXMLLogButtonClicked
//	=======================

void LoggingOptions::slotXMLLogButtonClicked( )
{	Knmap::getFilePath( "XML log file", m_xmlLogValue, m_xmlLogLineEdit, true );
}

//	slotXMLLogCheckBoxClicked
//	=========================

void LoggingOptions::slotXMLLogCheckBoxClicked( )
{	m_xmlLogButton->setEnabled( m_xmlLogCheckBox->isChecked( ));
	m_xmlLogLineEdit->setEnabled( m_xmlLogCheckBox->isChecked( ));
}
