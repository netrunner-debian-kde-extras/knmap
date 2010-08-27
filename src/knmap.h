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

#ifndef _KNMAP_H_
#define _KNMAP_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qstringlist.h>
#include <kmainwindow.h>
#include "global.h"

//	forward class definitions
//	=========================

class KAction;
class KToggleAction;
class MainWidget;
class ScanWidget;

//	Knmap class definition
//	======================

class Knmap : public KMainWindow
{	Q_OBJECT

	public:
		Knmap( );

		static bool getDirPath( const QString& caption, QString& path );
		static void getDirPath( const QString& caption, QString& path, QLineEdit* lineEdit );
		static bool getFilePath( const QString& caption, QString& path, const bool save );
		static void getFilePath( const QString& caption, QString& path, QLineEdit* lineEdit, const bool save );

	private slots:
		void slotConfigureShortcuts( );
		void slotConfigureToolbars( );
		void slotSetCaption( const QString& caption );
		void slotNewToolbarConfig( );
		void slotOutputAvailable( const bool saveState, const bool saveAsState );
		void slotSetManPageActionStuff( const bool useLocalManPage );
		void slotVisibleScanWidgetChanged( ScanWidget* scanWidget );

	private:
		void createMenu( );
		void findNmap( );
		bool queryExit( );
		void readSettings( );
		void saveSettings( );
		void switchManPages( );

		KAction*       m_fileSaveAction;
		KAction*       m_fileSaveAsAction;
		MainWidget*    m_mainWidget;
		KAction*       m_settingsManAction;
		KToggleAction* m_useTargetHostAction;
		KToggleAction* m_wrapTextAction;
};

#endif // _KNMAP_H_
