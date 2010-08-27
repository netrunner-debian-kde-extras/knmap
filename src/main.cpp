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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>

#include "global.h"
#include "knmap.h"

static const char      description[] = I18N_NOOP( "A KDE frontend for nmap" );
static KCmdLineOptions options[]     = { KCmdLineLastOption };

//	main
//	====

int main( int argc, string argv[])
{	QString version = QString( "Version 2.1, %1 %2" ).arg( __TIME__ ).arg( __DATE__ );
	KAboutData about( "knmap",
					  I18N_NOOP( "Knmap" ),
 					  version,
					  description,
					  KAboutData::License_GPL,
					  "(C) 2005, 2006 Kevin Gilbert",
					  I18N_NOOP( "This program is a complete re-write of one by the same name written by Alexandre\n"
								 "Sagala. The last version of that program was 0.9 which was released on 2003-03-09\n"
								 "and targeted the KDE 2.2 and QT 2.3 environments. Unfortunately it does not compile\n"
								 "on today's KDE / QT environments.\n"
								 "\n"
								 "Not to mention that it did not cater for the full set of 'nmap' options. Or, perhaps,\n"
								 "'nmap' progressed whilst that version of Knmap languished.\n"
								 "\n"
								 "The icons used for this application are from the Open Clip Art Library. I am indebted\n"
								 "to those damn fine folk as my artistic ability is zero - which is marginally above\n"
								 "my programming ability.  :-(" ),
					  "http://informatics.cdu.edu.au/staff/kgilbert" );
	about.addAuthor( "Kevin Gilbert",
					 "Initial rewrite released as version 1.0\nVersions 2.0, 2.1",
					 "kev.gilbert@cdu.edu.au",
					 "http://informatics.cdu.edu.au/staff/kgilbert" );
	KCmdLineArgs::init( argc, argv, &about );
	KCmdLineArgs::addCmdLineOptions( options );
	KApplication app;

	if( app.isRestored( ))
	{	RESTORE( Knmap );
	}
	else
	{	KCmdLineArgs* args = KCmdLineArgs::parsedArgs( );
		Knmap* mainWin = new Knmap;
		app.setMainWidget( mainWin );
		mainWin->show( );
		args->clear( );
	}

	return app.exec( );
}
