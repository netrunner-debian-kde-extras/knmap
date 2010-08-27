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
#ifndef _TIMINGWIDGET_H_
#define _TIMINGWIDGET_H_

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
class QRadioButton;

//	TimingWidget class definition
//	=============================

class TimingWidget : public QWidget
{	Q_OBJECT

	public:
		TimingWidget( QWidget* parent, const char* name = NULL );

		void    finaliseInitialisation( );
		bool    getOptions( );
		void    readProfile( KConfig* config );
		void    saveProfile( KConfig* config );
		void    setInitialValues( );
		QString simpleTiming( ) const;

		bool    hostTimeoutState( )       const { return m_hostTimeoutState;       }
		uint    hostTimeoutValue( )       const { return m_hostTimeoutValue;       }
		bool    initialRTTTimeoutState( ) const { return m_initialRTTTimeoutState; }
		uint    initialRTTTimeoutValue( ) const { return m_initialRTTTimeoutValue; }
		bool    maxHostGroupState( )      const { return m_maxHostGroupState;      }
		uint    maxHostGroupValue( )      const { return m_maxHostGroupValue;      }
		bool    maxParallelismState( )    const { return m_maxParallelismState;    }
		uint    maxParallelismValue( )    const { return m_maxParallelismValue;    }
		bool    maxRTTTimeoutState( )     const { return m_maxRTTTimeoutState;     }
		uint    maxRTTTimeoutValue( )     const { return m_maxRTTTimeoutValue;     }
		bool    maxScanDelayState( )      const { return m_maxScanDelayState;      }
		uint    maxScanDelayValue( )      const { return m_maxScanDelayValue;      }
		bool    minHostGroupState( )      const { return m_minHostGroupState;      }
		uint    minHostGroupValue( )      const { return m_minHostGroupValue;      }
		bool    minParallelismState( )    const { return m_minParallelismState;    }
		uint    minParallelismValue( )    const { return m_minParallelismValue;    }
		bool    minRTTTimeoutState( )     const { return m_minRTTTimeoutState;     }
		uint    minRTTTimeoutValue( )     const { return m_minRTTTimeoutValue;     }
		bool    scanDelayState( )         const { return m_scanDelayState;         }
		uint    scanDelayValue( )         const { return m_scanDelayValue;         }

	signals:
		void displayDocBook( const QString& );
		void displayHelp( const QString& );
		void displayUnknown( );
		void optionsDirty( );

	private slots:
		void slotAggressiveRadioClicked( );
		void slotDefaultRadioClicked( );
		void slotHostTimeoutButtonClicked( );
		void slotHostTimeoutCheckBoxClicked( );
		void slotInitialRTTTimeoutButtonClicked( );
		void slotInitialRTTTimeoutCheckBoxClicked( );
		void slotInsaneRadioClicked( );
		void slotMaxHostGroupButtonClicked( );
		void slotMaxHostGroupCheckBoxClicked( );
		void slotMaxParallelismButtonClicked( );
		void slotMaxParallelismCheckBoxClicked( );
		void slotMaxRTTTimeoutButtonClicked( );
		void slotMaxRTTTimeoutCheckBoxClicked( );
		void slotMaxScanDelayButtonClicked( );
		void slotMaxScanDelayCheckBoxClicked( );
		void slotMinHostGroupButtonClicked( );
		void slotMinHostGroupCheckBoxClicked( );
		void slotMinParallelismButtonClicked( );
		void slotMinParallelismCheckBoxClicked( );
		void slotMinRTTTimeoutButtonClicked( );
		void slotMinRTTTimeoutCheckBoxClicked( );
		void slotNormalRadioClicked( );
		void slotParanoidRadioClicked( );
		void slotPoliteRadioClicked( );
		void slotScanDelayButtonClicked( );
		void slotScanDelayCheckBoxClicked( );
		void slotSneakyRadioClicked( );
		void slotWhatsThisClicked( );

	private:
		void createLayout( );

		//	simple timing indices
		//	=====================
		
		enum Timing
		{	Default_Timing,
			Aggressive_T4_Timing,
			Insane_T5_Timing,
			Normal_T3_Timing,
			Paranoid_T0_Timing,
			Polite_T2_Timing,
			Sneaky_T1_Timing
		};

		QRadioButton* m_aggressiveRadio;
		QRadioButton* m_defaultRadio;
		QPushButton*  m_hostTimeoutButton;
		QCheckBox*    m_hostTimeoutCheckBox;
		QLineEdit*    m_hostTimeoutLineEdit;
		bool          m_hostTimeoutState;
		uint          m_hostTimeoutValue;
		QPushButton*  m_initialRTTTimeoutButton;
		QCheckBox*    m_initialRTTTimeoutCheckBox;
		QLineEdit*    m_initialRTTTimeoutLineEdit;
		bool          m_initialRTTTimeoutState;
		uint          m_initialRTTTimeoutValue;
		QRadioButton* m_insaneRadio;
		QPushButton*  m_maxHostGroupButton;
		QCheckBox*    m_maxHostGroupCheckBox;
		QLineEdit*    m_maxHostGroupLineEdit;
		bool          m_maxHostGroupState;
		uint          m_maxHostGroupValue;
		QPushButton*  m_maxParallelismButton;
		QCheckBox*    m_maxParallelismCheckBox;
		QLineEdit*    m_maxParallelismLineEdit;
		bool          m_maxParallelismState;
		uint          m_maxParallelismValue;
		QPushButton*  m_maxRTTTimeoutButton;
		QCheckBox*    m_maxRTTTimeoutCheckBox;
		QLineEdit*    m_maxRTTTimeoutLineEdit;
		bool          m_maxRTTTimeoutState;
		uint          m_maxRTTTimeoutValue;
		QPushButton*  m_maxScanDelayButton;
		QCheckBox*    m_maxScanDelayCheckBox;
		QLineEdit*    m_maxScanDelayLineEdit;
		bool          m_maxScanDelayState;
		uint          m_maxScanDelayValue;
		QPushButton*  m_minHostGroupButton;
		QCheckBox*    m_minHostGroupCheckBox;
		QLineEdit*    m_minHostGroupLineEdit;
		bool          m_minHostGroupState;
		uint          m_minHostGroupValue;
		QPushButton*  m_minParallelismButton;
		QCheckBox*    m_minParallelismCheckBox;
		QLineEdit*    m_minParallelismLineEdit;
		bool          m_minParallelismState;
		uint          m_minParallelismValue;
		QPushButton*  m_minRTTTimeoutButton;
		QCheckBox*    m_minRTTTimeoutCheckBox;
		QLineEdit*    m_minRTTTimeoutLineEdit;
		bool          m_minRTTTimeoutState;
		uint          m_minRTTTimeoutValue;
		QRadioButton* m_normalRadio;
		QRadioButton* m_paranoidRadio;
		QRadioButton* m_politeRadio;
		QPushButton*  m_scanDelayButton;
		QCheckBox*    m_scanDelayCheckBox;
		QLineEdit*    m_scanDelayLineEdit;
		bool          m_scanDelayState;
		uint          m_scanDelayValue;
		Timing        m_simpleTimingValue;
		QRadioButton* m_sneakyRadio;
};

#endif	// _TIMINGWIDGET_H_
