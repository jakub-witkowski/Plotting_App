QT += testlib         #Include QtTest to use SignalSpy, QTest::mouseClick, etc

QMAKE_LIBDIR_FLAGS += `root-config --cflags --libs`

TARGET = Tests
TEMPLATE = app

CONFIG += c++14
CONFIG += testcase    #Creates 'check' target in Makefile.
CONFIG -= debug_and_release
CONFIG += cmdline

include(../GoogleTest/GoogleTest.pri)
INCLUDEPATH += "/snap/root-framework/928/usr/local/include/"

HEADERS +=

SOURCES += tst_datasettests.cpp \
           "../LSR-Plot-GUI/tdata.cpp" \
           "../LSR-Plot-GUI/tsegment.cpp" \
           "../LSR-Plot-GUI/tpolynomial.cpp"
