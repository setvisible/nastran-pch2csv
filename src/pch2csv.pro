#-------------------------------------------------
# pch2csv - NASTRAN PUNCH to CSV
#-------------------------------------------------
TEMPLATE = app
TARGET = pch2csv

# Remove from CONFIG all the Qt dependencies and unnecessary stuff
CONFIG -= lex yacc
CONFIG -= qt qpa link_prl qml_debug precompile_header
CONFIG -= testcase_targets
CONFIG -= import_qpa_plugin import_plugins
CONFIG -= rtti rtti_off
CONFIG -= exceptions
CONFIG -= shared
CONFIG -= no_plugin_manifest
CONFIG -= file_copies copy_dir_files
CONFIG -= incremental_off
CONFIG -= depend_includepath
CONFIG -= windows # BUG: 'windows' prevents std::cout to write in the console.
CONFIG += c++11

#message($${CONFIG})

LANGUAGE = C++

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}

#-------------------------------------------------
# VERSION
#-------------------------------------------------
include($$PWD/../version.pri)

#-------------------------------------------------
# SOURCES
#-------------------------------------------------
HEADERS  += \
    $$PWD/filemanager.h \
    $$PWD/punchfile.h \
    $$PWD/reader.h \
    $$PWD/qsystemdetection.h \
    $$PWD/writer.h \
    $$PWD/version.h

SOURCES += \
    $$PWD/filemanager.cpp \
    $$PWD/punchfile.cpp \
    $$PWD/reader.cpp \
    $$PWD/writer.cpp \
    $$PWD/main.cpp

OTHER_FILES += \
    $$PWD/../README.md

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------



