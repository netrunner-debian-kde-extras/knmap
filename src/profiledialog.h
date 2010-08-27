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

#ifndef _GETPROFILEDIALOG_H_
#define _GETPROFILEDIALOG_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <kdialogbase.h>
#include "global.h"

//	forward class definitions
//	=========================

class KConfig;
class QLineEdit;
class QListBox;
class QListBoxItem;
class QString;

//	ProfileDialog class definition
//	==============================

class ProfileDialog : public KDialogBase
{	Q_OBJECT

	public:
		enum Action
		{	Copy,
			Delete,
			Load,
			Rename,
			SaveAs
		};

		ProfileDialog( Action         action,
					   const QString& currentProfile,
					   QWidget*       parent = NULL,
					   const char*    name = NULL );

		QString profileName( ) const { return m_profileName; }

	private slots:
		void slotOk( );
		void slotCopy( );
		void slotDelete( );
		void slotProfileListBoxChanged( );
		void slotProfileListContextMenu( QListBoxItem* item, const QPoint& pos );
		void slotRename( );

	private:
		bool    checkForDuplicateName( const QString& profileName );
		QString constructNewProfileName( const QString& initialProfileName ) const;
		void    copy( const QString& fromProfileName, const QString& toProfileName );
		void    createlayout( );
		void    deelete( const QString& profileName, const bool ask );
		void    deelete( QListBoxItem* item, const bool ask );
		bool    getNewProfileName( QString& profileName );
		void    setInitialValues( const QString& currentProfile );
		QString stripPrefix( const QString& profileName ) const;

		Action        m_action;
		QListBoxItem* m_contextItem;
		QString       m_profileName;
		QLineEdit*    m_profileLineEdit;
		QListBox*     m_profileListBox;
};

#endif	//	_GETPROFILEDIALOG_H_
