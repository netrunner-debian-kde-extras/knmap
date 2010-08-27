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

#ifndef _SCANWIDGET_H_
#define _SCANWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qframe.h>
#include "global.h"

//	forward class definitions
//	=========================

class CommonWidget;
class CompoundWidget;
class HTMLWidget;
class KConfig;
class KProcess;
class KTabWidget;
class LoggingOptions;
class OutputWidget;
class PAndSOptions;
class QFile;
class QLineEdit;
class QPushButton;
class QStringList;
class ScanMonitor;
class SimpleOptions;
class TimingWidget;

//	ScanWidget class definition
//	===========================

class ScanWidget : public QFrame
{	Q_OBJECT

	public:
		ScanWidget( const QString& scanName,
					const bool     useTargetHost,
					QWidget*       parent = NULL,
					const char*    name = NULL );
		virtual ~ScanWidget( );

		enum State
		{	dormant,
			finished,
			running
		};

		void    fileSave( );
		void    fileSaveAs( );
		void    ignoreTabChanges( const bool b ) { m_ignoreTabChanges = b; }
		bool    isDirty( ) const { return m_dirty; }
		void    profileAskAndSave( );
		void    profileCopy( );
		void    profileDelete( );
		void    profileLoad( );
		QString profileName( ) const { return m_profileName; }
		void    profileRename( );
		void    profileSave( );
		void    profileSaveAs( );
		void    readSettings( );
		void    renameScan( const QString& newScanName );
		void    saveSettings( );
		QString scanName( ) const { return m_scanName; }
		void    setProfileName( const QString& profileName );
		State   state( ) const { return m_state; }
		void    state( const State s ) { m_state = s; }
		void    updateStatusBarText( );
		bool    useTargetHost( ) const { return m_useTargetHost; }
		void    useTargetHost( const bool b );
		void    wrapText( const bool wrap );

	signals:
		void displayHelp( const QString& );
		void errorLoadingLocalManPage( const QString& );
		void optionsDirty( );
		void outputAvailable( const bool, const bool );
		void scanRename( const QString& );
		void scanStarted( );
		void scanStopped( ScanWidget* );
		void setManPageActionStuff( const bool );
		void statusBarText( const QString& );

	private slots:
		void slotClearOutput( );
		void slotDisplayDocBook( const QString& anchor );
		void slotDisplayUnknown( );
		void slotFinaliseInitialisation( );
		void slotHideOptions( );
		void slotOptionsDirty( const bool dirty = true );
		void slotProcessExited( );
		void slotReceivedStderr( KProcess* process, char* buffer, int buflen );
		void slotReceivedStdout( KProcess* process, char* buffer, int buflen );
		void slotStartClicked( );
		void slotStopClicked( );
		void slotTabChanged( QWidget* toWidget );
		void slotTargetChanged( const QString& target );

	private:
		QStringList buildNmapOptionsList( );
		void        closePipe( QFile*& pipe, int& pipeFD );
		void        createLayout( );
		bool        createPipe( const QString type, const QString& tempDir, QFile*& pipe, int& pipeFD );
		bool        createPipes( );
		void        customEvent( QCustomEvent* event );
		bool        getOptions( );
		void        getPortRanges( QStringList* portRanges );
		void        initialiseManPage( );
		void        profileRead( );
		void        saveProfileName( );
		void        setInitialValues( );

		QPushButton*    m_clearOutputButton;
		int             m_commonIndex;
		CommonWidget*   m_commonWidget;
		int             m_compoundIndex;
		CompoundWidget* m_compoundWidget;
		int             m_currentTab;
		bool            m_dirty;
		bool            m_hideOptions;
		QPushButton*    m_hideOptionsButton;
		int             m_htmlIndex;
		bool            m_ignoreTabChanges;
		int             m_loggingIndex;
		LoggingOptions* m_loggingWidget;
		KProcess*       m_nmapProcess;
		OutputWidget*   m_outputWidget;
		int             m_pAndSIndex;
		PAndSOptions*   m_pAndSWidget;
		QFile*          m_pipeStderr;
		QFile*          m_pipeStdout;
		int             m_pipeStderrFD;
		int             m_pipeStdoutFD;
		bool            m_piping;
		QString         m_profileName;
		ScanMonitor*    m_scanMonitor;
		QString         m_scanName;
		int             m_simpleIndex;
		SimpleOptions*  m_simpleWidget;
		QPushButton*    m_startButton;
		State           m_state;
		QPushButton*    m_stopButton;
		KTabWidget*     m_tabWidget;
		int             m_timingIndex;
		TimingWidget*   m_timingWidget;
		bool            m_useTargetHost;
};

#endif	// _SCANWIDGET_H_
