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

#ifndef _HTMLWIDGET_H_
#define _HTMLWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qstringlist.h>
#include <qwidget.h>
#include <kurl.h>
#include "global.h"

//	forward class definitions
//	=========================

class KHTMLPart;
class KProcess;
class StylesheetDialog;

//	HTMLWidget class definition
//	===========================


class HTMLWidget : public QWidget
{	Q_OBJECT

	public:
		HTMLWidget( QStringList* stylesheetURLs, QWidget* parent = NULL, const char* name = NULL );
		virtual ~HTMLWidget( );

		void  gotoAnchor( const QString& anchor );
		void  loadManPage( const bool localManPage );
		void  readSettings( );
		void  saveSettings( );
		void  setInitialValues( );
		void  setStylesheet( );
		float zoomFactor( ) const { return m_zoomFactor; }
		void  zoomFactor( const float factor );
		void  zoomIn( );
		void  zoomOut( );

	signals:
		void errorLoadingLocalManPage( const QString& );
		void optionsDirty( );

	private slots:
		void slotGunzipProcessExited( );
		void slotGunzipReceivedStdout( KProcess* process, char* buffer, int buflen );
		void slotMan2HTMLProcessExited( );
		void slotMan2HTMLReceivedStdout( KProcess* process, char* buffer, int buflen );
		void slotManProcessExited( );
		void slotManReceivedStdout( KProcess* process, char* buffer, int buflen );
		void slotStylesheetRemoved( );

	private:
		void loadKnmapManPage( );
		void loadLocalManPage( );
		void resizeEvent( QResizeEvent* event );
		bool tryKnmapFile( );

		StylesheetDialog*  m_dlg;
		QString            m_htmlData;
		KHTMLPart*         m_htmlPart;
		QString            m_manPageData;
		QString            m_manPagePath;
		KProcess*          m_process;
		KURL               m_stylesheetURL;
		QStringList*       m_stylesheetURLs;
		float              m_zoomFactor;
		static const float m_zoomIncrement;
};

#endif	// _HTMLWIDGET_H_
