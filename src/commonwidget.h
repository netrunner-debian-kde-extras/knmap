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

#ifndef _COMMONWIDGET_H_
#define _COMMONWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qwidget.h>
#include "global.h"

//	forward class definitions
//	=========================

class KConfig;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QPushButton;
class QRadioButton;
class QStringList;

//	CommonWidget class definition
//	=============================

class CommonWidget : public QWidget
{	Q_OBJECT

	public:
		CommonWidget( QWidget* parent, const char* name = NULL );
		virtual ~CommonWidget( );

		void finaliseInitialisation( );
		bool getOptions( );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );
		void setInitialValues( );

		QString      getHostName( )          const;
		int          hostCurrentItem( )      const { return m_hostCurrentItem;      }
		QString      host( )                 const { return m_host;                 }
		int          portRangeCurrentItem( ) const { return m_portRangeCurrentItem; }
		QStringList* portRanges( )           const { return m_portRanges;           }
		bool         portRangesState( )      const { return m_portRangesState;      }
		bool         resovleAlwaysState( )   const { return m_resovleAlwaysState;   }
		bool         resovleDefaultState( )  const { return m_resovleDefaultState;  }
		bool         resovleNeverState( )    const { return m_resovleNeverState;    }

	signals:
		void disableFastScan( );
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );
		void targetChanged( const QString& );

	public slots:
		void slotDisablePortRanges( );

	private slots:
		void slotHostActivated( int index );
		void slotHostButtonClicked( );
		void slotHostTextChanged( const QString& text );
		void slotPortRangeActivated( int index );
		void slotPortRangeAddClicked( );
		void slotPortRangeDeleteClicked( );
		void slotPortRangeButtonClicked( );
		void slotPortRangeDoubleClicked( QListBoxItem* item );
		void slotPortRangeHighlighted( );
		void slotPortRangesClicked( );
		void slotPortRangeTextChanged( const QString& text );
		void slotWhatsThisClicked( );

	private:
		bool buildPortRangesTables( const boolPtr tcpPorts, const boolPtr udpPorts );
		void createLayout( );
		void enablePortRangeWidgets( );
		void getHosts( QStringList* hosts );
		void getPortRanges( QStringList* portRanges );
		bool portRangesOverlapping( const ushort portFirst, const ushort portLast );
		bool validatePortRange( QString& text );
		bool validatePortRangeText( QString& text, uint& portFirst, uint& portLast );

		//	protocol indices
		//	================
		
		enum Protocol
		{	BothProtocol,
			TCPProtocol,
			UDPProtocol
		};

		QString       m_host;
		QPushButton*  m_hostButton;
		QComboBox*    m_hostComboBox;
		int           m_hostCurrentItem;
		QStringList*  m_hosts;
		QPushButton*  m_portRangeAddButton;
		QPushButton*  m_portRangeButton;
		QComboBox*    m_portRangeComboBox;
		int           m_portRangeCurrentItem;
		QPushButton*  m_portRangeDeleteButton;
		bool          m_portRangeIgnoreChange;
		QLineEdit*    m_portRangeLineEdit;
		QListBox*     m_portRangeListBox;
		QStringList*  m_portRanges;
		QCheckBox*    m_portRangesCheckBox;
		bool          m_portRangesState;
		QRadioButton* m_resovleAlwaysRadio;
		bool          m_resovleAlwaysState;
		QRadioButton* m_resovleDefaultRadio;
		bool          m_resovleDefaultState;
		QRadioButton* m_resovleNeverRadio;
		bool          m_resovleNeverState;
};

#endif	// _COMMONWIDGET_H_
