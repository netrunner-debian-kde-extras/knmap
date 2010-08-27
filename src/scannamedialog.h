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

#ifndef _SCANNAMEDIALOG_H_
#define _SCANNAMEDIALOG_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <kdialogbase.h>
#include "global.h"

//	forward class definitions
//	=========================

class QCheckBox;
class QLineEdit;
class ScanWidget;

//	ScanNameDialog class definition
//	===============================

class ScanNameDialog : public KDialogBase
{	Q_OBJECT

	public:
		ScanNameDialog( ScanWidget* scanWidget, QWidget* parent = NULL, const char* name = NULL );

		QString scanName( )    const { return m_scanName;    }
		bool    useHostName( ) const { return m_useHostName; }

	private slots:
		void slotOk( );
		void slotUseHostNameToggled( bool on );

	private:
		void createlayout( );

		QString    m_scanName;
		QLineEdit* m_scanNameEdit;
		bool       m_useHostName;
		QCheckBox* m_useHostNameCheckbox;
};

#endif	// _SCANNAMEDIALOG_H_
