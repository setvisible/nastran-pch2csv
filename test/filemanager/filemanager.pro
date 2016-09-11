#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_filemanager
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_filemanager.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../../src/filemanager.h
SOURCES += ../../src/filemanager.cpp
