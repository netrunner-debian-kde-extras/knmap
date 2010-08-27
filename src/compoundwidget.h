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
#ifndef _COMPOUNDWIDGET_H_
#define _COMPOUNDWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qwidget.h>
#include "global.h"

//	forward class definitions
//	=========================

class KConfig;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;

//	CompountWidget class definition
//	===============================

class CompoundWidget : public QWidget
{	Q_OBJECT

	public:
		CompoundWidget( QWidget* parent, const char* name = NULL );

		void finaliseInitialisation( );
		bool getOptions( bool& needRoot );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );
		void setInitialValues( );

		bool    dataDirState( )        const { return m_dataDirState;        }
		QString dataDirValue( )        const { return m_dataDirValue;        }
		bool    dataLengthState( )     const { return m_dataLengthState;     }
		int     dataLengthValue( )     const { return m_dataLengthValue;     }
		bool    decoyState( )          const { return m_decoyState;          }
		QString decoyValue( )          const { return m_decoyValue;          }
		bool    debugLevelState( )     const { return m_debugLevelState;     }
		int     debugLevelValue( )     const { return m_debugLevelValue;     }
		bool    excludeState( )        const { return m_excludeState;        }
		QString excludeValue( )        const { return m_excludeValue;        }
		bool    excludeFileState( )    const { return m_excludeFileState;    }
		QString excludeFileValue( )    const { return m_excludeFileValue;    }
		bool    fragmentLevelState( )  const { return m_fragmentLevelState;  }
		int     fragmentLevelValue( )  const { return m_fragmentLevelValue;  }
		bool    interfaceState( )      const { return m_interfaceState;      }
		QString interfaceValue( )      const { return m_interfaceValue;      }
		bool    maxSocketsState( )     const { return m_maxSocketsState;     }
		int     maxSocketsValue( )     const { return m_maxSocketsValue;     }
		bool    randomIPState( )       const { return m_randomIPState;       }
		int     randomIPValue( )       const { return m_randomIPValue;       }
		bool    sourceIPState( )       const { return m_sourceIPState;       }
		QString sourceIPValue( )       const { return m_sourceIPValue;       }
		bool    sourcePortState( )     const { return m_sourcePortState;     }
		int     sourcePortValue( )     const { return m_sourcePortValue;     }
		bool    spoofMacState( )       const { return m_spoofMacState;       }
		QString spoofMacValue( )       const { return m_spoofMacValue;       }
		bool    targetFileState( )     const { return m_targetFileState;     }
		QString targetFileValue( )     const { return m_targetFileValue;     }
		bool    ttlState( )            const { return m_ttlState;            }
		int     ttlValue( )            const { return m_ttlValue;            }

	signals:
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );

	private slots:
		void slotDataDirButtonClicked( );
		void slotDataDirCheckBoxClicked( );
		void slotDataLengthCheckBoxClicked( );
		void slotDebugLevelCheckBoxClicked( );
		void slotDecoyButtonClicked( );
		void slotDecoyCheckBoxClicked( );
		void slotExcludeButtonClicked( );
		void slotExcludeCheckBoxClicked( );
		void slotExcludeFileButtonClicked( );
		void slotExcludeFileCheckBoxClicked( );
		void slotFragmentLevelCheckBoxClicked( );
		void slotInterfaceButtonClicked( );
		void slotInterfaceCheckBoxClicked( );
		void slotMaxSocketsCheckBoxClicked( );
		void slotRandomIPCheckBoxClicked( );
		void slotSourceIPButtonClicked( );
		void slotSourceIPCheckBoxClicked( );
		void slotSourcePortCheckBoxClicked( );
		void slotSpoofMACButtonClicked( );
		void slotSpoofMacCheckBoxClicked( );
		void slotTargetFileButtonClicked( );
		void slotTargetFileCheckBoxClicked( );
		void slotTTLCheckBoxClicked( );
		void slotWhatsThisClicked( );

	private:
		void createLayout( );

		QPushButton* m_dataDirButton;
		QCheckBox*   m_dataDirCheckBox;
		QLineEdit*   m_dataDirLineEdit;
		bool         m_dataDirState;
		QString      m_dataDirValue;
		QCheckBox*   m_dataLengthCheckBox;
		QSpinBox*    m_dataLengthSpinBox;
		bool         m_dataLengthState;
		int          m_dataLengthValue;
		QPushButton* m_decoyButton;
		QCheckBox*   m_decoyCheckBox;
		QLineEdit*   m_decoyLineEdit;
		bool         m_decoyState;
		QString      m_decoyValue;
		QCheckBox*   m_debugLevelCheckBox;
		QSpinBox*    m_debugLevelSpinBox;
		bool         m_debugLevelState;
		int          m_debugLevelValue;
		QPushButton* m_excludeButton;
		QCheckBox*   m_excludeCheckBox;
		QLineEdit*   m_excludeLineEdit;
		bool         m_excludeState;
		QString      m_excludeValue;
		QPushButton* m_excludeFileButton;
		QCheckBox*   m_excludeFileCheckBox;
		QLineEdit*   m_excludeFileLineEdit;
		bool         m_excludeFileState;
		QString      m_excludeFileValue;
		QCheckBox*   m_fragmentLevelCheckBox;
		QSpinBox*    m_fragmentLevelSpinBox;
		bool         m_fragmentLevelState;
		int          m_fragmentLevelValue;
		QPushButton* m_interfaceButton;
		QCheckBox*   m_interfaceCheckBox;
		QLineEdit*   m_interfaceLineEdit;
		bool         m_interfaceState;
		QString      m_interfaceValue;
		QCheckBox*   m_maxSocketsCheckBox;
		QSpinBox*    m_maxSocketsSpinBox;
		bool         m_maxSocketsState;
		int          m_maxSocketsValue;
		QCheckBox*   m_randomIPCheckBox;
		QSpinBox*    m_randomIPSpinBox;
		bool         m_randomIPState;
		int          m_randomIPValue;
		QPushButton* m_sourceIPButton;
		QCheckBox*   m_sourceIPCheckBox;
		QLineEdit*   m_sourceIPLineEdit;
		bool         m_sourceIPState;
		QString      m_sourceIPValue;
		QCheckBox*   m_sourcePortCheckBox;
		QSpinBox*    m_sourcePortSpinBox;
		bool         m_sourcePortState;
		int          m_sourcePortValue;
		QPushButton* m_spoofMacButton;
		QCheckBox*   m_spoofMacCheckBox;
		QLineEdit*   m_spoofMacLineEdit;
		bool         m_spoofMacState;
		QString      m_spoofMacValue;
		QPushButton* m_targetFileButton;
		QCheckBox*   m_targetFileCheckBox;
		QLineEdit*   m_targetFileLineEdit;
		bool         m_targetFileState;
		QString      m_targetFileValue;
		QCheckBox*   m_ttlCheckBox;
		QSpinBox*    m_ttlSpinBox;
		bool         m_ttlState;
		int          m_ttlValue;
};

#endif	// _COMPOUNDWIDGET_H_
