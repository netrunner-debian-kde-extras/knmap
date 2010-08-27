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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <qfile.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qtimer.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kstandarddirs.h>
#include <ktabwidget.h>
#include <kurl.h>

#include "commonwidget.h"
#include "compoundwidget.h"
#include "htmlwidget.h"
#include "knmap.h"
#include "loggingoptions.h"
#include "nmapoutputbuffer.h"
#include "outputwidget.h"
#include "pandsoptions.h"
#include "profiledialog.h"
#include "scanmonitor.h"
#include "scanmonitorevent.h"
#include "scanwidget.h"
#include "simpleoptions.h"
#include "tabwidgetdata.h"
#include "tabwidgetptrlist.h"
#include "timingwidget.h"

//	constructor
//	===========

ScanWidget::ScanWidget( const QString& scanName,
						const bool useTargetHost,
						QWidget* parent,
						const char* name )
: QFrame( parent, name ),
  m_dirty (false ),
  m_hideOptions( true ),
  m_ignoreTabChanges( false ),
  m_nmapProcess( NULL ),
  m_pipeStderr( NULL ),
  m_pipeStdout( NULL ),
  m_scanMonitor( NULL ),
  m_scanName( scanName ),
  m_state( dormant ),
  m_useTargetHost( useTargetHost )
{	setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
	createLayout( );
	m_clearOutputButton->setEnabled( false );
	m_stopButton->setEnabled( false );
	m_startButton->setDefault( true );

	srand( time( NULL ));
	QTimer::singleShot( 0, this, SLOT( slotFinaliseInitialisation( )));
}

//	destructor
//	==========

ScanWidget::~ScanWidget( )
{	if( m_nmapProcess != NULL )
	{	m_nmapProcess->kill( SIGKILL );
		delete m_nmapProcess;
	}

	if( m_pipeStderr != NULL )
	{	m_pipeStderr->remove( );
		delete m_pipeStderr;
	}

	if( m_pipeStdout != NULL )
	{	m_pipeStdout->remove( );
		delete m_pipeStdout;
	}

	if( m_scanMonitor != NULL )
	{	m_scanMonitor->terminate( );
		while( m_scanMonitor->running( )) ;
		delete m_scanMonitor;
	}
}

//	buildNmapOptionsList
//	====================

QStringList ScanWidget::buildNmapOptionsList( )
{	QStringList nmapOptions;
	nmapOptions << KStandardDirs::findExe ( "nmap" );

	if( m_commonWidget->resovleAlwaysState( ))
		nmapOptions << "-R";

	if( m_commonWidget->resovleNeverState( ))
		nmapOptions << "-n";

	if( m_timingWidget->hostTimeoutState( ))
	{	nmapOptions << "--host_timeout";
		nmapOptions << QString::number( m_timingWidget->hostTimeoutValue( ));
	}

	if( m_timingWidget->initialRTTTimeoutState( ))
	{	nmapOptions << "--initial_rtt_timeout";
		nmapOptions << QString::number( m_timingWidget->initialRTTTimeoutValue( ));
	}

	if( m_timingWidget->maxHostGroupState( ))
	{	nmapOptions << "--max_hostgroup";
		nmapOptions << QString::number( m_timingWidget->maxHostGroupValue( ));
	}

	if( m_timingWidget->minHostGroupState( ))
	{	nmapOptions << "--min_hostgroup";
		nmapOptions << QString::number( m_timingWidget->minHostGroupValue( ));
	}

	if( m_timingWidget->maxParallelismState( ))
	{	nmapOptions << "--max_parallelism";
		nmapOptions << QString::number( m_timingWidget->maxParallelismValue( ));
	}

	if( m_timingWidget->minParallelismState( ))
	{	nmapOptions << "--min_parallelism";
		nmapOptions << QString::number( m_timingWidget->minParallelismValue( ));
	}

	if( m_timingWidget->maxRTTTimeoutState( ))
	{	nmapOptions << "--max_rtt_timeout";
		nmapOptions << QString::number( m_timingWidget->maxRTTTimeoutValue( ));
	}

	if( m_timingWidget->minRTTTimeoutState( ))
	{	nmapOptions << "--min_rtt_timeout";
		nmapOptions << QString::number( m_timingWidget->minRTTTimeoutValue( ));
	}

	if( m_timingWidget->maxScanDelayState( ))
	{	nmapOptions << "--max_scan_delay";
		nmapOptions << QString::number( m_timingWidget->maxScanDelayValue( ));
	}

	if( m_timingWidget->scanDelayState( ))
	{	nmapOptions << "--scan_delay";
		nmapOptions << QString::number( m_timingWidget->scanDelayValue( ));
	}

	if( !m_timingWidget->simpleTiming( ).isEmpty( ))
	{	nmapOptions << "-T";
		nmapOptions << m_timingWidget->simpleTiming( );
	}

	if( m_pAndSWidget->ackState( ))
		nmapOptions << "-PA" + m_pAndSWidget->ackValue( );

	if( m_pAndSWidget->arpState( ))
		nmapOptions << "-PR";

	if( m_pAndSWidget->dontPingState( ))
		nmapOptions << "-P0";

	if( m_pAndSWidget->echoState( ))
		nmapOptions << "-PE";

	if( m_pAndSWidget->netmaskState( ))
		nmapOptions << "-PM";

	if( m_pAndSWidget->synState( ))
		nmapOptions << "-PS" + m_pAndSWidget->synValue( );

	if( m_pAndSWidget->timestampState( ))
		nmapOptions << "-PP";

	if( m_commonWidget->portRangesState( ))
	{	nmapOptions << "-p";
		nmapOptions << m_commonWidget->portRanges( )->join( "," );
	}

	if( m_compoundWidget->dataDirState( ))
	{	nmapOptions << "--datadir";
		nmapOptions << m_compoundWidget->dataDirValue( );
	}

	if( m_compoundWidget->debugLevelState( ))
		for( byte i = 0; i < m_compoundWidget->debugLevelValue( ); i++ )
			nmapOptions << "-d";

	if( m_compoundWidget->dataLengthState( ))
	{	nmapOptions << "--data_length";
		nmapOptions << QString::number( m_compoundWidget->dataLengthValue( ));
	}

	if( m_compoundWidget->decoyState( ))
	{	nmapOptions << "-D";
		nmapOptions << m_compoundWidget->decoyValue( );
	}

	if( m_compoundWidget->excludeState( ))
	{	nmapOptions << "--exclude";
		nmapOptions << m_compoundWidget->excludeValue( );
	}

	if( m_compoundWidget->excludeFileState( ))
	{	nmapOptions << "--excludefile";
		nmapOptions << m_compoundWidget->excludeFileValue( );
	}

	if( m_compoundWidget->fragmentLevelState( ))
		for( byte i = 0; i < m_compoundWidget->fragmentLevelValue( ); i++ )
			nmapOptions << "-f";

	if( m_compoundWidget->interfaceState( ))
	{	nmapOptions << "-e";
		nmapOptions << m_compoundWidget->interfaceValue( );
	}

	if( m_compoundWidget->maxSocketsState( ))
	{	nmapOptions << "-M";
		nmapOptions << QString::number( m_compoundWidget->maxSocketsValue( ));
	}

	if( m_compoundWidget->randomIPState( ))
	{	nmapOptions << "-iR";
		nmapOptions << QString::number( m_compoundWidget->randomIPValue( ));
	}

	if( m_compoundWidget->sourcePortState( ))
	{	nmapOptions << "--source_port";
		nmapOptions << QString::number( m_compoundWidget->sourcePortValue( ));
	}

	if( m_compoundWidget->sourceIPState( ))
	{	nmapOptions << "-S";
		nmapOptions << m_compoundWidget->sourceIPValue( );
	}

	if( m_compoundWidget->spoofMacState( ))
	{	nmapOptions << "--spoof_mac";
		nmapOptions << m_compoundWidget->spoofMacValue( );
	}

	if( m_compoundWidget->targetFileState( ))
	{	nmapOptions << "-iL";
		nmapOptions << m_compoundWidget->targetFileValue( );
	}

	if( m_compoundWidget->ttlState( ))
	{	nmapOptions << "--ttl";
		nmapOptions << QString::number( m_compoundWidget->ttlValue( ));
	}

	if( m_simpleWidget->ipV6State( ))
		nmapOptions << "-6";

	if( m_simpleWidget->allPortsState( ))
		nmapOptions << "--allports";

	if( m_loggingWidget->appendOutputState( ))
		nmapOptions << "--append_output";

	if( m_loggingWidget->baseFileNameState( ))
	{	nmapOptions << "-oA";
		nmapOptions << m_loggingWidget->baseFileNameValue( );
	}

	if( m_simpleWidget->fastScanState( ))
		nmapOptions << "-F";

	if( m_loggingWidget->grepableLogState( ))
	{	nmapOptions << "-oG";
		nmapOptions << m_loggingWidget->grepableLogValue( );
	}

	if( m_simpleWidget->noRandomPortsState( ))
		nmapOptions << "-r";

	if( m_loggingWidget->normalLogState( ))
	{	nmapOptions << "-oN";
		nmapOptions << m_loggingWidget->normalLogValue( );
	}

	if( m_loggingWidget->noStylesheetState( ))
		nmapOptions << "--no_stylesheet";

	if( m_simpleWidget->osDetectionState( ))
		nmapOptions << "-O";

	if( m_simpleWidget->osScanLimitState( ))
		nmapOptions << "--osscan_limit";

	if( m_simpleWidget->packetTraceState( ))
		nmapOptions << "--packet_trace";

	if( m_simpleWidget->privilegedState( ))
		nmapOptions << "--privileged";

	if( m_simpleWidget->randomizeHostsState( ))
		nmapOptions << "--randomize_hosts";

	if( m_loggingWidget->resumeState( ))
	{	nmapOptions << "--resume";
		nmapOptions << m_loggingWidget->resumeValue( );
	}

	if( m_loggingWidget->scriptKiddieState( ))
	{	nmapOptions << "-oS";
		nmapOptions << m_loggingWidget->scriptKiddieValue( );
	}

	if( m_pAndSWidget->scanAckState( ))
		nmapOptions << "-sA";

	if( m_pAndSWidget->scanFINState( ))
		nmapOptions << "-sF";

	if( m_pAndSWidget->scanFTPBounceState( ))
	{	nmapOptions << "-b";
		nmapOptions << m_pAndSWidget->scanFTPRelayHost( );
	}

	if( m_pAndSWidget->scanIdleState( ))
	{	nmapOptions << "-sI";
		nmapOptions << m_pAndSWidget->zombieDetails( );
	}

	if( m_pAndSWidget->scanListState( ))
		nmapOptions << "-sL";

	if( m_pAndSWidget->scanNullState( ))
		nmapOptions << "-sN";

	if( m_pAndSWidget->scanPingState( ))
		nmapOptions << "-sP";

	if( m_pAndSWidget->scanProtocolState( ))
		nmapOptions << "-sO";

	if( m_pAndSWidget->scanRPCState( ))
		nmapOptions << "-sR";

	if( m_pAndSWidget->scanTCPConnectState( ))
		nmapOptions << "-sT";

	if( m_pAndSWidget->scanSYNState( ))
		nmapOptions << "-sS";

	if( m_pAndSWidget->scanUDPState( ))
		nmapOptions << "-sU";

	if( m_pAndSWidget->scanVersionState( ))
		nmapOptions << "-sV";

	if( m_pAndSWidget->scanWindowState( ))
		nmapOptions << "-sW";

	if( m_pAndSWidget->scanXmasState( ))
		nmapOptions << "-sX";

	if( m_simpleWidget->sendEthState( ))
		nmapOptions << "--send_eth";

	if( m_simpleWidget->sendIPState( ))
		nmapOptions << "--send_ip";

	if( m_loggingWidget->stylesheetState( ))
	{	nmapOptions << "--stylesheet";
		nmapOptions << m_loggingWidget->stylesheetValue( );
	}

	if( m_simpleWidget->verboseState( ))
		nmapOptions << "-v";

	if( m_loggingWidget->xmlLogState( ))
	{	nmapOptions << "-oX";
		nmapOptions << m_loggingWidget->xmlLogValue( );
	}

	nmapOptions << m_commonWidget->host( );
	return nmapOptions;
}

//	closePipe
//	=========

void ScanWidget::closePipe( QFile*& pipe, int& pipeFD )
{	if( pipe == NULL )
		return;

	::close( pipeFD );

	pipe->remove( );
	delete pipe;
	pipe = NULL;
}

//	createLayout
//	============

void ScanWidget::createLayout( )
{	m_commonWidget   = new CommonWidget( this, "common widget" );
	m_compoundWidget = new CompoundWidget( this, "compound widget" );
	m_loggingWidget  = new LoggingOptions( this, "logging widget" );
	m_pAndSWidget    = new PAndSOptions( this, "p and s widget" );
	m_simpleWidget   = new SimpleOptions( this, "simple widget" );
	m_timingWidget   = new TimingWidget( this, "timing widget" );
	m_tabWidget      = new KTabWidget( this, "tab widget" );

	m_tabWidget->setTabReorderingEnabled( true );

	byte row = 0;
	QVBoxLayout* layout = new QVBoxLayout( this );
	layout->addWidget( m_tabWidget, row++ );

	QHBoxLayout* buttonLayout = new QHBoxLayout( );
	m_clearOutputButton       = new QPushButton( i18n( "Clear output" ), this, "clear output button" );
	m_hideOptionsButton       = new QPushButton( i18n( "Hide options" ), this, "hide options button" );
	m_startButton             = new QPushButton( i18n( "Start nmap"   ), this, "start button"        );
	m_stopButton              = new QPushButton( i18n( "Stop nmap"    ), this, "stop button"         );

	byte col = 0;
	buttonLayout->insertStretch( col++, 10 );
	buttonLayout->insertWidget(  col++, m_startButton,       5 );
	buttonLayout->insertStretch( col++,  3 );
	buttonLayout->insertWidget(  col++, m_stopButton,        5 );
	buttonLayout->insertStretch( col++,  3 );
	buttonLayout->insertWidget(  col++, m_hideOptionsButton, 5 );
	buttonLayout->insertStretch( col++,  3 );
	buttonLayout->insertWidget(  col++, m_clearOutputButton, 5 );
	buttonLayout->insertStretch( col++, 10 );
	layout->addLayout( buttonLayout, row );

	m_outputWidget = new OutputWidget( this, "output widget" );
	layout->addWidget( m_outputWidget, row++ );
}

//	createPipe
//	==========

bool ScanWidget::createPipe( const QString type, const QString& tempDir, QFile*& pipe, int& pipeFD )
{	while( true )
	{	pipe = new QFile( QString( "%1knmap_%2_%3" ).arg( tempDir ).arg( type ).arg( rand( )));

		if( !pipe->exists( ))
			break;

		delete pipe;
	}

	if( mkfifo( pipe->name( ), 0600 ))
	{	QString text = QString( i18n( "Couldn't create the named pipe \"%1\" for nmap output: %2\n" )).arg( pipe->name( )).arg( strerror( errno ));
		m_outputWidget->addOutput( OutputWidget::Stderr, text, text.length( ));

		delete pipe;
		pipe = NULL;
		return false;
	}

	pipeFD = ::open( pipe->name( ), O_RDONLY | O_NONBLOCK );

	if( pipeFD != -1 )
		return true;

	QString text = QString( i18n( "Couldn't open the named pipe \"%1\" for nmap output: %2\n" )).arg( pipe->name( ))
																								.arg( strerror( errno ));
	m_outputWidget->addOutput( OutputWidget::Stderr, text, text.length( ));

	delete pipe;
	pipe = NULL;
	return false;
}

//	createPipes
//	===========

bool ScanWidget::createPipes( )
{	ASSERT( m_pipeStderr == NULL );
	ASSERT( m_pipeStdout == NULL );

	KStandardDirs standardDirs;
	QStringList tempDir = standardDirs.resourceDirs( "tmp" );

	if( tempDir.isEmpty( ))
	{	QString text = QString( i18n( "Couldn't create the named pipe for nmap output: no temp file dir\n" ));
		m_outputWidget->addOutput( OutputWidget::Stderr, text, text.length( ));
		return false;
	}

	if( !createPipe( "stderr", tempDir[ 0 ], m_pipeStderr, m_pipeStderrFD ))
		return false;

	return createPipe( "stderr", tempDir[ 0 ], m_pipeStdout, m_pipeStdoutFD );
}

//	customEvent
//	===========

void ScanWidget::customEvent( QCustomEvent* event )
{	smePtr scanMonitorEvent = (ScanMonitorEvent*) event;
	nobPtr buffer;

	switch( event->type( ))
	{	case ScanMonitor::StderrType:
			buffer = scanMonitorEvent->buffer( );
			m_outputWidget->addOutput( OutputWidget::Stderr, buffer->buffer( ), buffer->length( ));
			buffer->setFree( );
			break;

		case ScanMonitor::StdoutType:
			buffer = scanMonitorEvent->buffer( );
			m_outputWidget->addOutput( OutputWidget::Stdout, buffer->buffer( ), buffer->length( ));
			buffer->setFree( );
			break;

		default:
			kdDebug( ) << "internal error in ScanWidget::customEvent - unknown event type "
					   << event->type( )
					   << " - ignoring it"
					   << endl;
		}
}

//	fileSave
//	========

void ScanWidget::fileSave( )
{	if( m_outputWidget->fileSave( ))
		emit( outputAvailable( false, true ));
}

//	fileSaveAs
//	==========

void ScanWidget::fileSaveAs( )
{	if( m_outputWidget->fileSaveAs( ))
		emit( outputAvailable( false, true ));
}

//	getOptions
//	==========

bool ScanWidget::getOptions( )
{	m_piping = false;

	if( !m_commonWidget->getOptions( ))
		return false;

	if( !m_compoundWidget->getOptions( m_piping ))
		return false;

	if( !m_loggingWidget->getOptions( ))
		return false;

	if( !m_pAndSWidget->getOptions( m_piping ))
		return false;

	if( !m_simpleWidget->getOptions( m_piping ))
		return false;

	if( !m_timingWidget->getOptions( ))
		return false;

	if( m_commonWidget->host( ).isEmpty( ) && !m_compoundWidget->targetFileState( ))
	{	KMessageBox::error( this, i18n( QString( "Target host(s) not specified by \"Target host(s)\" or \"Target hosts file\" options" )), i18n( "Target host(s) error" ));
		return false;
	}

	return true;
}

//	profileAskAndSave
//	=================

void ScanWidget::profileAskAndSave( )
{	if( !m_dirty )
		return;

	if( KMessageBox::Yes != KMessageBox::questionYesNo( this,
														i18n( "Profile has been updated - save it now?" ),
														i18n( "Profile data updated" )))
		return;

	profileSave( );
}

//	profileCopy
//	===========

void ScanWidget::profileCopy( )
{	ProfileDialog dlg( ProfileDialog::Copy, m_profileName, this, "profile dlg" );
	dlg.exec( );
}

//	profileDelete
//	=============

void ScanWidget::profileDelete( )
{	ProfileDialog dlg( ProfileDialog::Delete, m_profileName, this, "profile dlg" );
	dlg.exec( );
}

//	profileLoad
//	===========

void ScanWidget::profileLoad( )
{	profileAskAndSave( );
	ProfileDialog dlg( ProfileDialog::Load, m_profileName, this, "profile dlg" );

	if( dlg.exec( ) != QDialog::Accepted )
		return;

	m_profileName = dlg.profileName( );
	saveProfileName( );
	profileRead( );
	setInitialValues( );
}

//	profileRead
//	===========

void ScanWidget::profileRead( )
{	KConfig* config = kapp->config( );

	kapp->config( )->setGroup( m_profileName );
	m_commonWidget->readProfile( config );
	m_compoundWidget->readProfile( config );
	m_loggingWidget->readProfile( config );
	m_pAndSWidget->readProfile( config );
	m_outputWidget->readProfile( config );
	m_simpleWidget->readProfile( config );
	m_timingWidget->readProfile( config );

	m_commonIndex   = config->readNumEntry(  "commonWidget",   0     );
	m_compoundIndex = config->readNumEntry(  "compoundWidget", 2     );
	m_currentTab    = config->readNumEntry(  "currentTab",     0     );
	m_loggingIndex  = config->readNumEntry(  "loggingWidget",  3     );
	m_pAndSIndex    = config->readNumEntry(  "pAndSWidget",    4     );
	m_simpleIndex   = config->readNumEntry(  "simpleWidget",   1     );
	m_timingIndex   = config->readNumEntry(  "timingWidget",   5     );
	m_useTargetHost = config->readBoolEntry( "useTargetHost",  false );
}

//	profileRename
//	=============

void ScanWidget::profileRename( )
{	ProfileDialog dlg( ProfileDialog::Rename, m_profileName, this, "profile dlg" );
	dlg.exec( );
}

//	profileSave
//	===========

void ScanWidget::profileSave( )
{	if( !getOptions( ))
		if( KMessageBox::Yes != KMessageBox::questionYesNo( this, i18n( "Do you still want to save the profile? (It may be in an inconsistent state." ), i18n( "Save inconsistent profile" )))
			return;

	KConfig* config = kapp->config( );
	config->setGroup( m_profileName );

	m_commonWidget->saveProfile( config );
	m_compoundWidget->saveProfile( config );
	m_loggingWidget->saveProfile( config );
	m_outputWidget->saveProfile( config );
	m_pAndSWidget->saveProfile( config );
	m_simpleWidget->saveProfile( config );
	m_timingWidget->saveProfile( config );

	config->writeEntry( "commonWidget",   m_tabWidget->indexOf( m_commonWidget   ));
	config->writeEntry( "compoundWidget", m_tabWidget->indexOf( m_compoundWidget ));
	config->writeEntry( "currentTab",     m_tabWidget->currentPageIndex( ));
	config->writeEntry( "loggingWidget",  m_tabWidget->indexOf( m_loggingWidget  ));
	config->writeEntry( "pAndSWidget",    m_tabWidget->indexOf( m_pAndSWidget    ));
	config->writeEntry( "simpleWidget",   m_tabWidget->indexOf( m_simpleWidget   ));
	config->writeEntry( "timingWidget",   m_tabWidget->indexOf( m_timingWidget   ));
	config->writeEntry( "useTargetHost",  m_useTargetHost );

	slotOptionsDirty( false );
}

//	profileSaveAs
//	=============

void ScanWidget::profileSaveAs( )
{	ProfileDialog dlg( ProfileDialog::SaveAs, m_profileName, this, "profile dlg" );

	if( dlg.exec( ) != QDialog::Accepted )
		return;

	m_profileName = dlg.profileName( );
	saveProfileName( );
	profileSave( );
}

//	readSettings
//	============

void ScanWidget::readSettings( )
{	KConfig* config = kapp->config( );
	config->setGroup( m_scanName );
	m_profileName = config->readEntry( "profileName", DEFAULT_PROFILE );

	profileRead( );
}

//	renameScan
//	==========

void ScanWidget::renameScan( const QString& newScanName )
{	kapp->config( )->deleteGroup( m_scanName );
	m_scanName = newScanName;
}

//	saveProfileName
//	===============

void ScanWidget::saveProfileName( )
{	KConfig* config = kapp->config( );
	config->setGroup( m_scanName );
	config->writeEntry( "profileName", m_profileName );
}

//	saveSettings
//	============

void ScanWidget::saveSettings( )
{	saveProfileName( );
	profileSave( );
}

//	setInitialValues
//	================

void ScanWidget::setInitialValues( )
{	m_commonWidget->setInitialValues( );
	m_compoundWidget->setInitialValues( );
	m_loggingWidget->setInitialValues( );
	m_pAndSWidget->setInitialValues( );
	m_simpleWidget->setInitialValues( );
	m_timingWidget->setInitialValues( );

	TabWidgetPtrList list;
	list.append( new TabWidgetData( m_commonIndex,   i18n( "Common options"        ), m_commonWidget   ));
	list.append( new TabWidgetData( m_compoundIndex, i18n( "Compound options"      ), m_compoundWidget ));
	list.append( new TabWidgetData( m_loggingIndex,  i18n( "Logging options"       ), m_loggingWidget  ));
	list.append( new TabWidgetData( m_pAndSIndex,    i18n( "Ping and Scan options" ), m_pAndSWidget    ));
	list.append( new TabWidgetData( m_simpleIndex,   i18n( "Simple options"        ), m_simpleWidget   ));
	list.append( new TabWidgetData( m_timingIndex,   i18n( "Timing options"        ), m_timingWidget   ));
	list.sort( );

	while( m_tabWidget->count( ))
		m_tabWidget->removePage( m_tabWidget->page( 0 ));

    for( TabWidgetData* widgetData = list.first( ); widgetData != NULL; widgetData = list.next( ))
		m_tabWidget->insertTab( widgetData->widget( ), widgetData->title( ), widgetData->index( ));

	m_tabWidget->setCurrentPage( m_currentTab );
}

//	setProfileName
//	==============

void ScanWidget::setProfileName( const QString& profileName )
{	m_profileName = profileName;
	saveProfileName( );
	profileRead( );
}

//	slotClearOutput
//	===============

void ScanWidget::slotClearOutput( )
{	m_clearOutputButton->setEnabled( false );
	m_outputWidget->clearOutput( );
	emit( outputAvailable( false, false ));
}

//	slotDisplayDocBook
//	==================

void ScanWidget::slotDisplayDocBook( const QString& anchor )
{	KProcess* process = new KProcess;
	*process << "khelpcenter";
	*process << "help:/knmap//index.html#" + anchor;
	process->start( KProcess::DontCare );
}

//	slotDisplayUnknown
//	==================

void ScanWidget::slotDisplayUnknown( )
{	KMessageBox::sorry( this, i18n( "There is no help available for the item you have selected!\nTOUGH!!!\nDon't bother me with you pathetic whinges. Go get a life." ), i18n( "Help unavailable" ));
}

//	slotFinaliseInitialisation
//	==========================

void ScanWidget::slotFinaliseInitialisation( )
{	setInitialValues( );
	m_commonWidget->finaliseInitialisation( );
	m_compoundWidget->finaliseInitialisation( );
	m_pAndSWidget->finaliseInitialisation( );
	m_simpleWidget->finaliseInitialisation( );
	m_timingWidget->finaliseInitialisation( );

	connect( m_outputWidget,      SIGNAL( statusBarText( const QString& )),       SIGNAL( statusBarText( const QString& )));

	connect( m_clearOutputButton, SIGNAL( clicked( )),                            SLOT( slotClearOutput( )));
	connect( m_commonWidget,      SIGNAL( disableFastScan( )),    m_simpleWidget, SLOT( slotDisableFastScan( )));
	connect( m_commonWidget,      SIGNAL( targetChanged( const QString& )),       SLOT( slotTargetChanged( const QString& )));
	connect( m_hideOptionsButton, SIGNAL( clicked( )),                            SLOT( slotHideOptions( )));
	connect( m_simpleWidget,      SIGNAL( disablePortRanges( )),  m_commonWidget, SLOT( slotDisablePortRanges( )));
	connect( m_startButton,       SIGNAL( clicked( )),                            SLOT( slotStartClicked( )));
	connect( m_stopButton,        SIGNAL( clicked( )),                            SLOT( slotStopClicked( )));
	connect( m_tabWidget,         SIGNAL( currentChanged( QWidget* )),            SLOT( slotTabChanged( QWidget* )));

	connect( m_commonWidget,      SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));
	connect( m_compoundWidget,    SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));
	connect( m_loggingWidget,     SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));
	connect( m_pAndSWidget,       SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));
	connect( m_simpleWidget,      SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));
	connect( m_tabWidget,         SIGNAL( movedTab( int, int )),                  SLOT( slotOptionsDirty( )));
	connect( m_timingWidget,      SIGNAL( optionsDirty( )),                       SLOT( slotOptionsDirty( )));

	connect( m_commonWidget,      SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));
	connect( m_compoundWidget,    SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));
	connect( m_loggingWidget,     SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));
	connect( m_pAndSWidget,       SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));
	connect( m_simpleWidget,      SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));
	connect( m_timingWidget,      SIGNAL( displayHelp( const QString& )),         SIGNAL( displayHelp( const QString& )));

	connect( m_commonWidget,      SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));
	connect( m_compoundWidget,    SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));
	connect( m_loggingWidget,     SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));
	connect( m_pAndSWidget,       SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));
	connect( m_simpleWidget,      SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));
	connect( m_timingWidget,      SIGNAL( displayDocBook( const QString& )),      SLOT( slotDisplayDocBook( const QString&)));

	connect( m_commonWidget,      SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));
	connect( m_compoundWidget,    SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));
	connect( m_loggingWidget,     SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));
	connect( m_pAndSWidget,       SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));
	connect( m_simpleWidget,      SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));
	connect( m_timingWidget,      SIGNAL( displayUnknown( )),                     SLOT( slotDisplayUnknown( )));

	if( m_useTargetHost )
		renameScan( m_commonWidget->getHostName( ));
}

//	slotHideOptions
//	===============

void ScanWidget::slotHideOptions( )
{	if( m_hideOptions )
	{	m_tabWidget->hide( );
		m_hideOptionsButton->setText( i18n( "Show options" ));
	}
	else
	{	m_tabWidget->show( );
		m_hideOptionsButton->setText( i18n( "Hide options" ));
	}

	m_hideOptions = !m_hideOptions;
}

//	slotOptionsDirty
//	================

void ScanWidget::slotOptionsDirty( const bool dirty )
{	m_dirty = dirty;
	emit( optionsDirty( ));
}

//	slotProcessExited
//	=================

void ScanWidget::slotProcessExited( )
{	delete m_nmapProcess;
	m_nmapProcess = NULL;

	if( m_piping )
	{	closePipe( m_pipeStderr, m_pipeStderrFD );
		closePipe( m_pipeStdout, m_pipeStdoutFD );

		if( m_scanMonitor != NULL )
			while( m_scanMonitor->running( ))
				sleep( 1 );
	}

	m_stopButton->setEnabled( false );
	m_startButton->setEnabled( true );

	emit( scanStopped( this ));
}

//	slotReceivedStderr
//	==================

void ScanWidget::slotReceivedStderr( KProcess* /* process */, char* buffer, int buflen )
{	m_clearOutputButton->setEnabled( true );
	m_outputWidget->addOutput( OutputWidget::Stderr, buffer, buflen );
	emit( outputAvailable( true, true ));
}

//	slotReceivedStdout
//	==================

void ScanWidget::slotReceivedStdout( KProcess* /* process */, char* buffer, int buflen )
{	m_clearOutputButton->setEnabled( true );
	m_outputWidget->addOutput( OutputWidget::Stdout, buffer, buflen );
	emit( outputAvailable( true, true ));
}

//	slotStartClicked
//	================

void ScanWidget::slotStartClicked( )
{	if( !getOptions( ))
		return;

	m_stopButton->setEnabled( true );
	m_startButton->setEnabled( false );

	if( m_piping && !createPipes( ))
		return;

	QStringList nmapOptions = buildNmapOptionsList( );
	m_nmapProcess = new KProcess;

	if( !m_piping )
		*m_nmapProcess << nmapOptions;
	else
	{	nmapOptions << ">";
		nmapOptions << m_pipeStdout->name( );

		nmapOptions << "2>";
		nmapOptions << m_pipeStderr->name( );

		*m_nmapProcess << "kdesu";
		*m_nmapProcess << "-n";
		*m_nmapProcess << nmapOptions.join( " " );
	}

	QValueList<QCString> args = m_nmapProcess->args( );
	QString cmd;
	QValueList<QCString>::iterator it;

    for( it = args.begin( ); it != args.end( ); ++it )
		cmd += *it + " ";

	cmd.stripWhiteSpace( );
	m_clearOutputButton->setEnabled( true );
	m_outputWidget->addOutput( OutputWidget::Stdin, cmd, cmd.length( ));

	emit( outputAvailable( true, true ));
	connect( m_nmapProcess, SIGNAL( processExited( KProcess* )), SLOT( slotProcessExited( )));

	if( !m_piping )
	{	connect( m_nmapProcess,
				 SIGNAL( receivedStderr( KProcess*, char*, int )),
				 SLOT( slotReceivedStderr( KProcess*, char*, int )));

		connect( m_nmapProcess,
				 SIGNAL( receivedStdout( KProcess*, char*, int )),
				 SLOT( slotReceivedStdout( KProcess*, char*, int )));
	}

	m_nmapProcess->start( KProcess::NotifyOnExit, KProcess::AllOutput );
	m_state = running;
	emit( scanStarted( ));

	if( !m_piping )
		return;

	if( m_scanMonitor == NULL )
		m_scanMonitor = new ScanMonitor( this );

	m_scanMonitor->setPipeFDs( m_pipeStderrFD, m_pipeStdoutFD );
	m_scanMonitor->start( );
}

//	slotStopClicked
//	===============

void ScanWidget::slotStopClicked( )
{	m_stopButton->setEnabled( false );
	m_nmapProcess->kill( );
}

//	slotTabChanged
//	==============

void ScanWidget::slotTabChanged( QWidget* /* toWidget */)
{	if( !m_ignoreTabChanges )
		slotOptionsDirty( );
}

//	slotTabChanged
//	==============

void ScanWidget::slotTargetChanged( const QString& target )
{	if( m_useTargetHost )
		emit( scanRename( target ));
}

//	updateStatusBarText
//	===================

void ScanWidget::updateStatusBarText( )
{	m_outputWidget->slotUpdateStatusBarText( );
}

//	useTargetHost
//	==============

void ScanWidget::useTargetHost( const bool b )
{	m_useTargetHost = b;

	if( m_useTargetHost )
		emit( scanRename( m_commonWidget->getHostName( )));
}

//	wrapText
//	========

void ScanWidget::wrapText( const bool wrap )
{	m_outputWidget->setWordWrap( wrap ? QTextEdit::WidgetWidth : QTextEdit::NoWrap );
}
