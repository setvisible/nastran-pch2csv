#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_scanner
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_scanner.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../../utils/csvcomparer.h
SOURCES += ../../utils/csvcomparer.cpp
HEADERS += ../../utils/testsuite.h
SOURCES += ../../utils/testsuite.cpp

HEADERS += ../../src/reader.h
SOURCES += ../../src/reader.cpp
HEADERS += ../../src/punchfile.h
SOURCES += ../../src/punchfile.cpp
HEADERS += ../../src/writer.h
SOURCES += ../../src/writer.cpp

