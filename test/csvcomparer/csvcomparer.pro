#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_csvcomparer
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_csvcomparer.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../../utils/csvcomparer.h
SOURCES += ../../utils/csvcomparer.cpp
