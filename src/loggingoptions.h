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

#ifndef _LOGGINGOPTIONS_
#define _LOGGINGOPTIONS_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qwidget.h>
#include "global.h"

//	forward class definitions
//	=========================

class QCheckBox;
class QLineEdit;
class QPushButton;
class KConfig;

//	LoggingOptions class definition
//	===============================

class LoggingOptions : public QWidget
{	Q_OBJECT

	public:
		LoggingOptions( QWidget* parent, const char* name = NULL );

		void finaliseInitialisation( );
		bool getOptions( );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );
		void setInitialValues( );

		bool    appendOutputState( ) const { return m_appendOutputState; }
		bool    baseFileNameState( ) const { return m_baseFileNameState; }
		QString baseFileNameValue( ) const { return m_baseFileNameValue; }
		bool    grepableLogState( )  const { return m_grepableLogState;  }
		QString grepableLogValue( )  const { return m_grepableLogValue;  }
		bool    normalLogState( )    const { return m_normalLogState;    }
		QString normalLogValue( )    const { return m_normalLogValue;    }
		bool    noStylesheetState( ) const { return m_noStylesheetState; }
		bool    resumeState( )       const { return m_resumeState;       }
		QString resumeValue( )       const { return m_resumeValue;       }
		bool    scriptKiddieState( ) const { return m_scriptKiddieState; }
		QString scriptKiddieValue( ) const { return m_scriptKiddieValue; }
		bool    stylesheetState( )   const { return m_stylesheetState;   }
		QString stylesheetValue( )   const { return m_stylesheetValue;   }
		bool    xmlLogState( )       const { return m_xmlLogState;       }
		QString xmlLogValue( )       const { return m_xmlLogValue;       }

	signals:
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );

	private slots:
		void slotBaseFileNameButtonClicked( );
		void slotBaseFileNameCheckBoxClicked( );
		void slotGrepableLogButtonClicked( );
		void slotGrepableLogCheckBoxClicked( );
		void slotNormalLogButtonClicked( );
		void slotNormalLogCheckBoxClicked( );
		void slotNoStylesheetCheckBoxClicked( );
		void slotResumeButtonClicked( );
		void slotResumeCheckBoxClicked( );
		void slotScriptKiddieButtonClicked( );
		void slotScriptKiddieCheckBoxClicked( );
		void slotStylesheetButtonClicked( );
		void slotStylesheetCheckBoxClicked( );
		void slotWhatsThisClicked( );
		void slotXMLLogButtonClicked( );
		void slotXMLLogCheckBoxClicked( );

	private:
		void createLayout( );

		QCheckBox*   m_appendOutputCheckBox;
		bool         m_appendOutputState;
		QPushButton* m_baseFileNameButton;
		QCheckBox*   m_baseFileNameCheckBox;
		QLineEdit*   m_baseFileNameLineEdit;
		bool         m_baseFileNameState;
		QString      m_baseFileNameValue;
		QPushButton* m_grepableLogButton;
		QCheckBox*   m_grepableLogCheckBox;
		QLineEdit*   m_grepableLogLineEdit;
		bool         m_grepableLogState;
		QString      m_grepableLogValue;
		QPushButton* m_normalLogButton;
		QCheckBox*   m_normalLogCheckBox;
		QLineEdit*   m_normalLogLineEdit;
		bool         m_normalLogState;
		QString      m_normalLogValue;
		QCheckBox*   m_noStylesheetCheckBox;
		bool         m_noStylesheetState;
		QPushButton* m_resumeButton;
		QCheckBox*   m_resumeCheckBox;
		QLineEdit*   m_resumeLineEdit;
		bool         m_resumeState;
		QString      m_resumeValue;
		QPushButton* m_scriptKiddieButton;
		QCheckBox*   m_scriptKiddieCheckBox;
		QLineEdit*   m_scriptKiddieLineEdit;
		bool         m_scriptKiddieState;
		QString      m_scriptKiddieValue;
		QPushButton* m_stylesheetButton;
		QCheckBox*   m_stylesheetCheckBox;
		QLineEdit*   m_stylesheetLineEdit;
		bool         m_stylesheetState;
		QString      m_stylesheetValue;
		QPushButton* m_xmlLogButton;
		QCheckBox*   m_xmlLogCheckBox;
		QLineEdit*   m_xmlLogLineEdit;
		bool         m_xmlLogState;
		QString      m_xmlLogValue;
};

#endif	// _LOGGINGOPTIONS_
