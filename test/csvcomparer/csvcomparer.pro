#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_csvcomparer
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_csvcomparer.cpp

# Dependancies:
HEADERS += csvcomparer.h
SOURCES += csvcomparer.cpp
