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
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qstringlist.h>
#include <qvalidator.h>

#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "commonwidget.h"
#include "whatsthis.h"

//	constructor
//	===========

CommonWidget::CommonWidget( QWidget* parent, const char* name )
: QWidget( parent, name ),
  m_hosts( NULL ),
  m_portRangeIgnoreChange( false ),
  m_portRanges( NULL )
{	createLayout( );
	m_portRangeAddButton->setEnabled( false );
	m_portRangeDeleteButton->setEnabled( false );
}

//	destructor
//	==========

CommonWidget::~CommonWidget( )
{	if( m_hosts != NULL )
		delete m_hosts;

	if( m_portRanges != NULL )
		delete m_portRanges;
}

//	buildPortRangesTables
//	=====================

bool CommonWidget::buildPortRangesTables( const boolPtr tcpPorts, const boolPtr udpPorts )
{	memset( tcpPorts, 0, 64 * 1024 );
	memset( udpPorts, 0, 64 * 1024 );

	QStringList portRanges;
	getPortRanges( &portRanges );

	for( QStringList::Iterator it = portRanges.begin( ); it != portRanges.end( ); ++it )
	{	QString portRange = *it;
		bool    tcp       = false;
		bool    udp       = false;

		if( portRange[ 0 ].isDigit( ))
		{	tcp = true;
			udp = true;
		}
		else
		if( portRange[ 0 ].lower( ) == 't' )
		{	tcp = true;
			portRange = portRange.right( portRange.length( ) - 2 );
		}
		else
		if( portRange[ 0 ].lower( ) == 'u' )
		{	udp = true;
			portRange = portRange.right( portRange.length( ) - 2 );
		}
		else
		{	KMessageBox::error( this, i18n( QString( "Internal error - Unknown Port Range protocol '%1'" ).arg( portRange[ 0 ])), i18n( "Internal error" ));
			return false;
		}

		QStringList ports = QStringList::split( "-", portRange );

		if( ports.count( ) > 2 )
		{	KMessageBox::error( this, i18n( "Internal error - Illegal Port Range encountered" ), i18n( "Internal error" ));
			return false;
		}

		ushort portFirst = ports.front( ).toUShort( );
		ushort portLast  = portFirst;

		if( ports.count( ) > 1 )
			portLast = ports.back( ).toUShort( );

		for( uint port = portFirst; port <= portLast; port++ )
		{	if( tcp )
				tcpPorts[ port ] = true;

			if( udp )
				udpPorts[ port ] = true;
		}
	}

	return true;
}

//	createLayout
//	============

void CommonWidget::createLayout( )
{	QGridLayout* gridLayout        = new QGridLayout( this );
	KIconLoader* iconLoader        = KGlobal::iconLoader( );
	QIconSet     clearRightIconSet = iconLoader->loadIconSet( "locationbar_erase", KIcon::Small );

 	byte row = 1;
	m_hostButton   = new QPushButton( clearRightIconSet, NULL, this, "host button" );
	m_hostComboBox = new QComboBox( true, this, "target hosts combobox" );
	QHBoxLayout* hostLayout = new QHBoxLayout( );

	byte col = 0;
	hostLayout->insertStretch( col++, 0 );
	hostLayout->insertWidget(  col++, new QLabel( i18n( "Target host(s)" ), this, "target hosts label" ), 0 );
	hostLayout->insertStretch( col++, 1 );
	hostLayout->insertWidget(  col++, m_hostButton, 0 );
	hostLayout->insertStretch( col++, 1 );
	hostLayout->insertWidget(  col++, m_hostComboBox, 100 );
	hostLayout->insertStretch( col++, 0 );
	gridLayout->addMultiCellLayout( hostLayout, row, row, 1, 5 );

	QButtonGroup* resloveGroup = new QButtonGroup( 3, Qt::Vertical, i18n( "Resolve names" ), this, "resolve names group" );
	m_resovleDefaultRadio      = new QRadioButton( i18n( "Default"     ), resloveGroup, "default resolve radio" );
	m_resovleAlwaysRadio       = new QRadioButton( i18n( "Always (-R)" ), resloveGroup, "always resolve radio"  );
	m_resovleNeverRadio        = new QRadioButton( i18n( "Never (-n)"  ), resloveGroup, "never resolve radio"   );

	row = 4;
	gridLayout->addMultiCellWidget( resloveGroup, row, row + 3, 1, 1 );
	byte rowBlock1 = ++row;

	m_portRangeAddButton    = new QPushButton( i18n( "Add" ),    this, "add port range button" );
	m_portRangeDeleteButton = new QPushButton( i18n( "Delete" ), this, "delete port range button" );
	m_portRangeComboBox     = new QComboBox( this, "port range combobox" );
	m_portRangeLineEdit     = new QLineEdit( this, "port range line edit" );
	m_portRangeListBox      = new QListBox( this, "port range listbox" );
	m_portRangesCheckBox    = new QCheckBox( i18n( "Port ranges (-p <port ranges>)" ), this, "port ranges checkbox" );

	m_portRangeComboBox->insertItem( i18n( "Both" ), BothProtocol );
	m_portRangeComboBox->insertItem( i18n( "TCP"  ), TCPProtocol  );
	m_portRangeComboBox->insertItem( i18n( "UDP"  ), UDPProtocol  );

	m_portRangeLineEdit->setValidator( new QRegExpValidator( QRegExp( "^\\d{1,5}-{1}\\d{1,5}$" ), this, "port range regx" ));

	row = 3;
	gridLayout->addMultiCellWidget( m_portRangesCheckBox, row, row, 3, 5, Qt::AlignCenter );
	row++;

	m_portRangeButton = new QPushButton( clearRightIconSet, NULL, this, "host button" );
	QHBoxLayout* portRangeLayout = new QHBoxLayout( );

	col = 0;
	portRangeLayout->insertStretch( col++, 0 );
	portRangeLayout->insertWidget(  col++, new QLabel( i18n( "New range data: " ), this, "new range data label" ), 0 );
	portRangeLayout->insertStretch( col++, 1 );
	portRangeLayout->insertWidget(  col++, m_portRangeButton, 0 );
	portRangeLayout->insertStretch( col++, 1 );
	portRangeLayout->insertWidget(  col++, m_portRangeLineEdit, 100 );
	portRangeLayout->insertStretch( col++, 0 );
	gridLayout->addMultiCellLayout( portRangeLayout, row, row, 3, 5 );

	gridLayout->addWidget( new QLabel( i18n( "Protocol" ), this, "protocol label" ), row + 1, 3 );
	gridLayout->addWidget( m_portRangeComboBox,     row + 2, 3 );
	gridLayout->addWidget( m_portRangeAddButton,    row + 4, 3 );
	gridLayout->addWidget( m_portRangeDeleteButton, row + 5, 3 );
	byte rowBlock2 = _max_( row + 5, rowBlock1 );
	gridLayout->addMultiCellWidget( m_portRangeListBox,  row + 1, rowBlock2, 5, 5 );

	gridLayout->setColStretch(  0,  4 );
	gridLayout->setColStretch(  2,  2 );
	gridLayout->setColStretch(  5, 10 );
	gridLayout->setColStretch(  6,  4 );

	gridLayout->setRowStretch( 0, 10 );
	gridLayout->setRowStretch( 2, 10 );
	gridLayout->setRowStretch( rowBlock2 + 1, 10 );
}

//	enablePortRangeWidgets
//	======================

void CommonWidget::enablePortRangeWidgets( )
{	bool portRangesState = m_portRangesCheckBox->isChecked( );

	m_portRangeAddButton->setEnabled( false );
	m_portRangeDeleteButton->setEnabled( false );
	m_portRangeButton->setEnabled( portRangesState );
	m_portRangeLineEdit->setEnabled( portRangesState );
	m_portRangeLineEdit->clear( );
	m_portRangeComboBox->setEnabled( portRangesState );
	m_portRangeListBox->setEnabled( portRangesState );

	if( portRangesState )
		emit( disableFastScan( ));
}

//	finaliseInitialisation
//	======================

void CommonWidget::finaliseInitialisation( )
{	WhatsThis* whatsThis = new WhatsThis( this );

	connect( m_hostButton,             SIGNAL( clicked( )),                     SLOT( slotHostButtonClicked( )));
	connect( m_hostComboBox,           SIGNAL( activated( int )),               SLOT( slotHostActivated( int )));
	connect( m_hostComboBox,           SIGNAL( textChanged( const QString& )),  SLOT( slotHostTextChanged( const QString& )));
	connect( m_portRangeAddButton,     SIGNAL( clicked( )),                     SLOT( slotPortRangeAddClicked( )));
	connect( m_portRangeButton,        SIGNAL( clicked( )),                     SLOT( slotPortRangeButtonClicked( )));
	connect( m_portRangeDeleteButton,  SIGNAL( clicked( )),                     SLOT( slotPortRangeDeleteClicked( )));
	connect( m_portRangeComboBox,      SIGNAL( activated( int )),               SLOT( slotPortRangeActivated( int )));
	connect( m_portRangeLineEdit,      SIGNAL( textChanged( const QString& )),  SLOT( slotPortRangeTextChanged( const QString& )));
	connect( m_portRangeListBox,       SIGNAL( doubleClicked( QListBoxItem* )), SLOT( slotPortRangeDoubleClicked( QListBoxItem* )));
	connect( m_portRangeListBox,       SIGNAL( highlighted( int )),             SLOT( slotPortRangeHighlighted( )));
	connect( m_portRangesCheckBox,     SIGNAL( clicked( )),                     SLOT( slotPortRangesClicked( )));
	connect( whatsThis,                SIGNAL( clicked( )),                     SLOT( slotWhatsThisClicked( )));

	connect( m_hostButton,             SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_hostComboBox,           SIGNAL( activated ( int )),              SIGNAL( optionsDirty( )));
	connect( m_hostComboBox,           SIGNAL( textChanged( const QString& )),  SIGNAL( optionsDirty( )));
	connect( m_portRangeAddButton,     SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_portRangeButton,        SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_portRangeComboBox,      SIGNAL( activated( int )),               SIGNAL( optionsDirty( )));
	connect( m_portRangeDeleteButton,  SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_portRangeLineEdit,      SIGNAL( textChanged( const QString& )),  SIGNAL( optionsDirty( )));
	connect( m_portRangeListBox,       SIGNAL( doubleClicked( QListBoxItem* )), SIGNAL( optionsDirty( )));
	connect( m_portRangeListBox,       SIGNAL( highlighted ( int )),            SIGNAL( optionsDirty( )));
	connect( m_portRangesCheckBox,     SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_resovleAlwaysRadio,     SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_resovleDefaultRadio,    SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));
	connect( m_resovleNeverRadio,      SIGNAL( clicked( )),                     SIGNAL( optionsDirty( )));

	emit( targetChanged( m_hostComboBox->currentText( )));
}
QString CommonWidget::getHostName( ) const
{	return m_hostComboBox->currentText( );
}

//	getHosts
//	========

void CommonWidget::getHosts( QStringList* hosts )
{	hosts->clear( );

	for( ushort i = 0; i < m_hostComboBox->count( ); i++ )
		hosts->append( m_hostComboBox->text( i ));
}

//	getOptions
//	==========

bool CommonWidget::getOptions( )
{	QStringList portRanges;
	getPortRanges( &portRanges );

	bool portRangesState = m_portRangesCheckBox->isChecked( );

	if( portRangesState && portRanges.isEmpty( ))
	{	KMessageBox::error( this, i18n( QString( "Port ranges not specified" )), i18n( "Port ranges error" ));
		return false;
	}

	m_host                 = m_hostComboBox->currentText( );
	m_hostCurrentItem      = m_hostComboBox->currentItem( );
	m_portRangeCurrentItem = m_portRangeComboBox->currentItem( );
	m_portRangesState      = portRangesState;
	m_resovleAlwaysState   = m_resovleAlwaysRadio->isChecked( );
	m_resovleDefaultState  = m_resovleDefaultRadio->isChecked( );
	m_resovleNeverState    = m_resovleNeverRadio->isChecked( );

	if( m_portRanges != NULL )
		delete m_portRanges;

	m_portRanges = new QStringList( portRanges );
	getHosts( m_hosts );

	disconnect( m_hostComboBox, SIGNAL( activated( int )),              this, SLOT( slotHostActivated( int )));
	disconnect( m_hostComboBox, SIGNAL( textChanged( const QString& )), this, SLOT( slotHostTextChanged( const QString& )));

	disconnect( m_hostComboBox, SIGNAL( activated ( int )),             this, SIGNAL( optionsDirty( )));
	disconnect( m_hostComboBox, SIGNAL( textChanged( const QString& )), this, SIGNAL( optionsDirty( )));

	byte i;
	QStringList::Iterator it;

	for( i = 0, it = m_hosts->begin( ); it != m_hosts->end( ); i++, ++it )
		if( *it == m_host )
			break;

	if( it != m_hosts->end( ))
	{	m_hostComboBox->removeItem( i );
		m_hosts->remove( it );
	}
	else
		while( m_hostComboBox->count( ) > MAX_HOST_HISTORY - 1 )
		{	m_hostComboBox->removeItem( MAX_HOST_HISTORY - 1 );
			m_hosts->pop_back( );
		}

	m_hostComboBox->insertItem( m_host, 0 );
	m_hostComboBox->setCurrentItem( 0 );
	m_hosts->prepend( m_host );

	connect( m_hostComboBox, SIGNAL( activated( int )),              SLOT( slotHostActivated( int )));
	connect( m_hostComboBox, SIGNAL( textChanged( const QString& )), SLOT( slotHostTextChanged( const QString& )));

	connect( m_hostComboBox, SIGNAL( activated ( int )),             SIGNAL( optionsDirty( )));
	connect( m_hostComboBox, SIGNAL( textChanged( const QString& )), SIGNAL( optionsDirty( )));

	return true;
}

//	getPortRanges
//	=============

void CommonWidget::getPortRanges( QStringList* portRanges )
{	portRanges->clear( );

	for( ushort i = 0; i < m_portRangeListBox->count( ); i++ )
		portRanges->append( m_portRangeListBox->text( i ));
}

//	portRangesOverlapping
//	=====================

bool CommonWidget::portRangesOverlapping( const ushort portFirst, const ushort portLast )
{	boolPtr tcpPorts = new bool[ 64 * 1024 ];
	boolPtr udpPorts = new bool[ 64 * 1024 ];

	if( !buildPortRangesTables( tcpPorts, udpPorts ))
	{	delete [] tcpPorts;
		delete [] udpPorts;
		return false;
	}

	bool tcp = false;
	bool udp = false;

	switch( m_portRangeComboBox->currentItem( ))
	{	case 0: tcp = udp = true; break;
		case 1: tcp = true;       break;
		case 2: udp = true;       break;

		default:
			kdDebug( ) << i18n( "unknown value in Port Range Protocol combo box" ) << endl;
			ASSERT( false );
			delete [] tcpPorts;
			delete [] udpPorts;
			return false;
	}

	uint    port;
	QString protocol;

	for( port = portFirst; port <= portLast; port++ )
	{	if( tcp && tcpPorts[ port ])
		{	protocol = "TCP";
			break;
		}

		if( udp && udpPorts[ port ])
		{	protocol = "UDP";
			break;
		}
	}

	if( port <= portLast )
	{	KMessageBox::sorry( this, i18n( "Overlapping Port Range - %1 port %2" ).arg( protocol ).arg( QString::number( port )), i18n( "Overlapping Port Range" ));
		delete [] tcpPorts;
		delete [] udpPorts;
		return false;
	}

	delete [] tcpPorts;
	delete [] udpPorts;
	return true;
}

//	readProfile
//	===========

void CommonWidget::readProfile( KConfig* config )
{	m_hostCurrentItem      = config->readNumEntry(  "hostCurrentItem"      );
	m_portRangeCurrentItem = config->readNumEntry(  "portRangeCurrentItem" );
	m_portRangesState      = config->readBoolEntry( "portRangesState"      );
	m_resovleAlwaysState   = config->readBoolEntry( "resovleAlwaysState"   );
	m_resovleDefaultState  = config->readBoolEntry( "resovleDefaultState"  );
	m_resovleNeverState    = config->readBoolEntry( "resovleNeverState"    );

	m_hosts      = new QStringList( config->readListEntry( "hosts"      ));
	m_portRanges = new QStringList( config->readListEntry( "portRanges" ));
}

//	saveProfile
//	===========

void CommonWidget::saveProfile( KConfig* config )
{	config->writeEntry( "hostCurrentItem",      m_hostCurrentItem      );
	config->writeEntry( "hosts",               *m_hosts                );
	config->writeEntry( "portRanges",          *m_portRanges           );
	config->writeEntry( "portRangeCurrentItem", m_portRangeCurrentItem );
	config->writeEntry( "portRangesState",      m_portRangesState      );
	config->writeEntry( "resovleAlwaysState",   m_resovleAlwaysState   );
	config->writeEntry( "resovleDefaultState",  m_resovleDefaultState  );
	config->writeEntry( "resovleNeverState",    m_resovleNeverState    );
}

//	setInitialValues
//	================

void CommonWidget::setInitialValues( )
{	m_hostComboBox->clear( );
	m_hostComboBox->insertStringList( *m_hosts );
	m_portRangeListBox->insertStringList( *m_portRanges );
	m_portRangeComboBox->setCurrentItem( m_portRangeCurrentItem );
	m_portRangesCheckBox->setChecked( m_portRangesState );
	m_resovleAlwaysRadio->setChecked( m_resovleAlwaysState );
	m_resovleDefaultRadio->setChecked( m_resovleDefaultState );
	m_resovleNeverRadio->setChecked( m_resovleNeverState );

	if( m_hostComboBox->count( ))
		m_hostComboBox->setCurrentItem( m_hostCurrentItem );

	enablePortRangeWidgets( );
}

//	slotDisablePortRanges
//	=====================

void CommonWidget::slotDisablePortRanges( )
{	m_portRangesCheckBox->setChecked( false );
	slotPortRangesClicked( );
}

//	slotHostActivated
//	=================

void CommonWidget::slotHostActivated( int /* index */)
{	emit( targetChanged( m_hostComboBox->currentText( )));
}

//	slotHostButtonClicked
//	=====================

void CommonWidget::slotHostButtonClicked( )
{	m_hostComboBox->clearEdit( );
}

//	slotHostTextChanged
//	===================

void CommonWidget::slotHostTextChanged( const QString& text )
{	emit( targetChanged( text ));
}

//	slotPortRangeAddClicked
//	=======================

void CommonWidget::slotPortRangeAddClicked( )
{	QString text;

	if( !validatePortRange( text ))
		return;

	switch( m_portRangeComboBox->currentItem( ))
	{	case BothProtocol:                     break;
		case TCPProtocol: text = "T:" + text; break;
		case UDPProtocol: text = "U:" + text; break;

		default:
			kdDebug( ) << i18n( "unknown value in Port Range Protocol combo box" ) << endl;
			ASSERT( false );
	}

	m_portRangeAddButton->setEnabled( false );
	emit( optionsDirty( ));
	m_portRangeLineEdit->clear( );
	m_portRangeListBox->insertItem( text );
}

//	slotPortRangeDeleteClicked
//	==========================

void CommonWidget::slotPortRangeDeleteClicked( )
{	m_portRangeIgnoreChange = true;
	m_portRangeListBox->removeItem( m_portRangeListBox->currentItem( ));
	m_portRangeAddButton->setEnabled( true );
	m_portRangeDeleteButton->setEnabled( false );
	emit( optionsDirty( ));
}

//	slotPortRangeActivated
//	======================

void CommonWidget::slotPortRangeActivated( int /* index */)
{	m_portRangeAddButton->setEnabled( true );
}

//	slotPortRangeActivated
//	======================

void CommonWidget::slotPortRangeButtonClicked( )
{	m_portRangeAddButton->setEnabled( false );
	m_portRangeDeleteButton->setEnabled( false );
	m_portRangeLineEdit->clear( );
}

//	slotPortRangeDoubleClicked
//	==========================

void CommonWidget::slotPortRangeDoubleClicked( QListBoxItem* item )
{	if( item == NULL )
		return;

	QString text = item->text( );

	if( text[ 0 ].isDigit( ))
		m_portRangeComboBox->setCurrentItem( BothProtocol ); 
	else
	{	switch( text[ 0 ].latin1( ))
		{	case 'T': m_portRangeComboBox->setCurrentItem( TCPProtocol ); break;
			case 'U': m_portRangeComboBox->setCurrentItem( UDPProtocol ); break;

			default:
				kdDebug( ) << i18n( "unknown value in Port Range Protocol" ) << endl;
				ASSERT( false );
				m_portRangeComboBox->setCurrentItem( BothProtocol ); 
		}

		text = text.right( text.length( ) - 2 );
	}

	m_portRangeIgnoreChange = true;
	m_portRangeLineEdit->setText( text );
	m_portRangeDeleteButton->setEnabled( true );
}

//	slotPortRangeHighlighted
//	========================

void CommonWidget::slotPortRangeHighlighted( )
{	if( m_portRangeIgnoreChange )
		m_portRangeIgnoreChange = false;
	else
	{	m_portRangeAddButton->setEnabled( false );
		m_portRangeDeleteButton->setEnabled( false );
		m_portRangeLineEdit->clear( );
	}
}

//	slotPortRangesClicked
//	=====================

void CommonWidget::slotPortRangesClicked( )
{	enablePortRangeWidgets( );
}

//	slotPortRangeTextChanged
//	========================

void CommonWidget::slotPortRangeTextChanged( const QString& /* text */)
{	if( m_portRangeIgnoreChange )
		m_portRangeIgnoreChange = false;
	else
		m_portRangeAddButton->setEnabled( m_portRangeLineEdit->text( ).length( ));
}

//	slotWhatsThisClicked
//	====================

void CommonWidget::slotWhatsThisClicked( )
{		 if( m_hostButton->hasMouse( ))            emit( displayHelp( "target" ));
	else if( m_hostComboBox->hasMouse( ))          emit( displayHelp( "target" ));
	else if( m_portRangeAddButton->hasMouse( ))    emit( displayDocBook( "common_options" ));
	else if( m_portRangeButton->hasMouse( ))       emit( displayDocBook( "common_options" ));
	else if( m_portRangeComboBox->hasMouse( ))     emit( displayDocBook( "common_options" ));
	else if( m_portRangeDeleteButton->hasMouse( )) emit( displayDocBook( "common_options" ));
	else if( m_portRangeLineEdit->hasMouse( ))     emit( displayDocBook( "common_options" ));
	else if( m_portRangeListBox->hasMouse( ))      emit( displayDocBook( "common_options" ));
	else if( m_portRangesCheckBox->hasMouse( ))    emit( displayHelp( "-p" ));
	else if( m_resovleAlwaysRadio->hasMouse( ))    emit( displayHelp( "-R" ));
	else if( m_resovleDefaultRadio->hasMouse( ))   emit( displayDocBook( "common_options" ));
	else if( m_resovleNeverRadio->hasMouse( ))     emit( displayHelp( "-n" ));
	else emit( displayUnknown( ));
}

//	validatePortRange
//	=================

bool CommonWidget::validatePortRange( QString& text )
{	uint portFirst;
	uint portLast;

	if( !validatePortRangeText( text, portFirst, portLast ))
		return false;

	if( !portRangesOverlapping( portFirst, portLast ))
		return false;

	return true;
}

//	validatePortRangeText
//	=====================

bool CommonWidget::validatePortRangeText( QString& text, uint& portFirst, uint& portLast )
{	text = m_portRangeLineEdit->text( );
	QStringList ports = QStringList::split( "-", text );

	if( ports.count( ) > 2 )
	{	KMessageBox::error( this, i18n( "Internal error - you seem to have been allowed to entered an illegal Port Range" ), i18n( "Internal error" ));
		return false;
	}

	portFirst = ports.front( ).toUInt( );
	portLast  = portFirst;

	if( ports.count( ) > 1 )
		portLast  = ports.back( ).toUInt( );

	if( !portFirst || portFirst > 65535 || !portLast || portLast > 65535 || portFirst > portLast )
	{	KMessageBox::sorry( this,
							i18n( "Ports must be in the range 1 to 65,535 and ranges must be in ascending order" ),
							i18n( "Invalid Port Range" ));
		return false;
	}

	return true;
}
