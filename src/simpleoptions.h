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

#ifndef _SIMPLEOPTIONS_H_
#define _SIMPLEOPTIONS_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qwidget.h>
#include "global.h"

//	forward class definitions
//	=========================

class QCheckBox;
class KConfig;

//	SimpleOptions class definition
//	==============================

class SimpleOptions : public QWidget
{	Q_OBJECT

	public:
		SimpleOptions( QWidget* parent, const char* name = NULL );

		void finaliseInitialisation( );
		bool getOptions( bool& needRoot );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );
		void setInitialValues( );

		bool allPortsState( )       const { return m_allPortsState;       }
		bool fastScanState( )       const { return m_fastScanState;       }
		bool ipV6State( )           const { return m_ipV6State;           }
		bool noRandomPortsState( )  const { return m_noRandomPortsState;  }
		bool osDetectionState( )    const { return m_osDetectionState;    }
		bool osScanLimitState( )    const { return m_osScanLimitState;    }
		bool packetTraceState( )    const { return m_packetTraceState;    }
		bool prepackagedState( )    const { return m_prepackagedState;     }
		bool privilegedState( )     const { return m_privilegedState;     }
		bool randomizeHostsState( ) const { return m_randomizeHostsState; }
		bool sendEthState( )        const { return m_sendEthState;        }
		bool sendIPState( )         const { return m_sendIPState;         }
		bool verboseState( )        const { return m_verboseState;        }

	signals:
		void disablePortRanges( );
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );

	public slots:
		void slotDisableFastScan( );

	private slots:
		void slotFastScanCheckBoxClicked( );
		void slotWhatsThisClicked( );

	private:
		void createLayout( );

		QCheckBox* m_allPortsCheckBox;
		bool       m_allPortsState;
		QCheckBox* m_fastScanCheckBox;
		bool       m_fastScanState;
		QCheckBox* m_ipV6CheckBox;
		bool       m_ipV6State;
		QCheckBox* m_noRandomPortsCheckBox;
		bool       m_noRandomPortsState;
		QCheckBox* m_osDetectionCheckBox;
		bool       m_osDetectionState;
		QCheckBox* m_osScanLimitCheckBox;
		bool       m_osScanLimitState;
		QCheckBox* m_packetTraceCheckBox;
		bool       m_packetTraceState;
		QCheckBox* m_prepackagedCheckBox;
		bool       m_prepackagedState;
		QCheckBox* m_privilegedCheckBox;
		bool       m_privilegedState;
		QCheckBox* m_randomizeHostsCheckBox;
		bool       m_randomizeHostsState;
		QCheckBox* m_sendEthCheckBox;
		bool       m_sendEthState;
		QCheckBox* m_sendIPCheckBox;
		bool       m_sendIPState;
		QCheckBox* m_verboseCheckBox;
		bool       m_verboseState;
};

#endif	// _SIMPLEOPTIONS_H_
