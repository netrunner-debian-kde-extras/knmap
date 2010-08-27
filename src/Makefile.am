# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = knmap.h global.h mainwidget.h outputwidget.h commonwidget.h \
					timingwidget.h compoundwidget.h htmlwidget.h stylesheetdialog.h profiledialog.h \
					loggingoptions.h simpleoptions.h pandsoptions.h whatsthis.h tabwidgetptrlist.h \
					tabwidgetdata.h scanwidget.h indexwidget.h scannamedialog.h scanstack.h scanmonitor.h \
	scanmonitorevent.h nmapoutputbuffer.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/knmap.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = knmap

# the application source, library search path, and link libraries
knmap_SOURCES = main.cpp knmap.cpp mainwidget.cpp outputwidget.cpp \
					commonwidget.cpp timingwidget.cpp compoundwidget.cpp htmlwidget.cpp stylesheetdialog.cpp \
					profiledialog.cpp loggingoptions.cpp simpleoptions.cpp pandsoptions.cpp whatsthis.cpp \
					tabwidgetptrlist.cpp tabwidgetdata.cpp scanwidget.cpp indexwidget.cpp scannamedialog.cpp \
			scanstack.cpp scanmonitor.cpp scanmonitorevent.cpp nmapoutputbuffer.cpp
knmap_LDFLAGS = $(KDE_RPATH) $(all_libraries)
knmap_LDADD = $(LIB_KIO) $(LIB_KHTML)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = knmap.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/knmap
shellrc_DATA = knmapui.rc nmap_manpage.html nmap_manpage.html.diff

kde_icon_KDEICON = hi16-app-scanclose.png hi32-app-scanclose.png \
		hi16-app-scannew.png hi32-app-scannew.png hi16-app-scanrename.png hi32-app-scanrename.png \
		hi16-app-scanduplicate.png hi32-app-scanduplicate.png hi16-app-manpage.png hi32-app-manpage.png
