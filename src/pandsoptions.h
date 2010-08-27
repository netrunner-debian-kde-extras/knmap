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

#ifndef _PANDSOPTIONS_H_
#define _PANDSOPTIONS_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qwidget.h>
#include "global.h"

//	forward class definitions
//	=========================

class QCheckBox;
class QLineEdit;
class KConfig;

//	SimpleOptions class definition
//	==============================

class PAndSOptions : public QWidget
{	Q_OBJECT

	public:
		PAndSOptions( QWidget* parent, const char* name = NULL );

		void finaliseInitialisation( );
		bool getOptions( bool& needRoot );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );
		void setInitialValues( );

		bool    ackState( )            const { return m_ackState;            }
		QString ackValue( )            const { return m_ackValue;            }
		bool    arpState( )            const { return m_arpState;            }
		bool    dontPingState( )       const { return m_dontPingState;       }
		bool    echoState( )           const { return m_echoState;           }
		bool    netmaskState( )        const { return m_netmaskState;        }
		bool    scanAckState( )        const { return m_scanAckState;        }
		bool    scanFINState( )        const { return m_scanFINState;        }
		bool    scanFTPBounceState( )  const { return m_scanFTPBounceState;  }
		QString scanFTPRelayHost( )    const { return m_scanFTPRelayHost;    }
		bool    scanIdleState( )       const { return m_scanIdleState;       }
		bool    scanListState( )       const { return m_scanListState;       }
		bool    scanNullState( )       const { return m_scanNullState;       }
		bool    scanPingState( )       const { return m_scanPingState;       }
		bool    scanProtocolState( )   const { return m_scanProtocolState;   }
		bool    scanRPCState( )        const { return m_scanRPCState;        }
		bool    scanSYNState( )        const { return m_scanSYNState;        }
		bool    scanTCPConnectState( ) const { return m_scanTCPConnectState; }
		bool    scanUDPState( )        const { return m_scanUDPState;        }
		bool    scanVersionState( )    const { return m_scanVersionState;    }
		bool    scanWindowState( )     const { return m_scanWindowState;     }
		bool    scanXmasState( )       const { return m_scanXmasState;       }
		bool    synState( )            const { return m_synState;            }
		QString synValue( )            const { return m_synValue;            }
		bool    timestampState( )      const { return m_timestampState;      }
		QString zombieDetails( )       const { return m_zombieDetails;       }

	signals:
		void disableFastScan( );
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );

	private slots:
		void slotAckClicked( );
		void slotScanFTPBounceCheckBoxClicked( );
		void slotScanIdleCheckBoxClicked( );
		void slotSynClicked( );
		void slotWhatsThisClicked( );

	private:
		void createLayout( );

		QCheckBox* m_ackCheckBox;
		QLineEdit* m_ackLineEdit;
		bool       m_ackState;
		QString    m_ackValue;
		QCheckBox* m_arpCheckBox;
		bool       m_arpState;
		QCheckBox* m_dontPingCheckBox;
		bool       m_dontPingState;
		QCheckBox* m_echoCheckBox;
		bool       m_echoState;
		QCheckBox* m_netmaskCheckBox;
		bool       m_netmaskState;
		QCheckBox* m_scanAckCheckBox;
		bool       m_scanAckState;
		QCheckBox* m_scanFINCheckBox;
		bool       m_scanFINState;
		QCheckBox* m_scanFTPBounceCheckBox;
		bool       m_scanFTPBounceState;
		QString    m_scanFTPRelayHost;
		QLineEdit* m_scanFTPRelayLineEdit;
		QCheckBox* m_scanIdleCheckBox;
		bool       m_scanIdleState;
		QCheckBox* m_scanListCheckBox;
		bool       m_scanListState;
		QCheckBox* m_scanNullCheckBox;
		bool       m_scanNullState;
		QCheckBox* m_scanPingCheckBox;
		bool       m_scanPingState;
		QCheckBox* m_scanProtocolCheckBox;
		bool       m_scanProtocolState;
		QCheckBox* m_scanRPCCheckBox;
		bool       m_scanRPCState;
		QCheckBox* m_scanTCPConnectCheckBox;
		bool       m_scanTCPConnectState;
		QCheckBox* m_scanUDPCheckBox;
		bool       m_scanUDPState;
		QCheckBox* m_scanVersionCheckBox;
		bool       m_scanVersionState;
		QCheckBox* m_scanWindowCheckBox;
		bool       m_scanWindowState;
		QCheckBox* m_scanSYNCheckBox;
		bool       m_scanSYNState;
		QCheckBox* m_scanXmasCheckBox;
		bool       m_scanXmasState;
		QCheckBox* m_synCheckBox;
		QLineEdit* m_synLineEdit;
		bool       m_synState;
		QString    m_synValue;
		QCheckBox* m_timestampCheckBox;
		bool       m_timestampState;
		QString    m_zombieDetails;
		QLineEdit* m_zombieLineEdit;
};

#endif	// _PANDSOPTIONS_H_
