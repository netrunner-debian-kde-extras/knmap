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

#ifndef _OUTPUTWIDGET_H_
#define _OUTPUTWIDGET_H_

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <qtextedit.h>
#include "global.h"

//	forward class definitions
//	=========================

class KConfig;

//	OutputWidget class definition
//	=============================

class OutputWidget : public QTextEdit
{	Q_OBJECT

	public:
		OutputWidget( QWidget* parent = NULL, const char* name = NULL );

		//	output type definitions
		//	=======================
		
		enum OutputType
		{	Stderr,
			Stdin,
			Stdout
		};

		void addOutput( const OutputType type, const char* buffer, const int buflen );
		void clearOutput( );
		bool fileSave( const bool saveAs = false );
		bool fileSaveAs( );
		void readProfile( KConfig* config );
		void saveProfile( KConfig* config );

	signals:
		void statusBarText( const QString& );

	public slots:
		void slotUpdateStatusBarText( );

	private:
		void appendLine( const OutputType type, const QString& text );

		bool       m_append;
		uint       m_dataBytes;
		bool       m_firstSave;
		QString    m_path;
		OutputType m_priorType;
};

#endif	// _OUTPUTWIDGET_H_
