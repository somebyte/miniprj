DEFDESTDIR  =  ./distr
DEFTMPDIR   =  ./tmp
DEFDESTLIB  = $$DEFDESTDIR/lib
DEFDESTBIN  = $$DEFDESTDIR/bin

DESTDIR     = $$DEFDESTBIN

OBJECTS_DIR = $$DEFTMPDIR/tmp/$$TARGET/obj
RCC_DIR     = $$DEFTMPDIR/tmp/$$TARGET/rcc
MOC_DIR     = $$DEFTMPDIR/tmp/$$TARGET/moc

CONFIG += c++11

INCLUDEPATH += ./include 
LIBS += 

