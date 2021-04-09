QT += core
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPS_SPOOFING
CONFIG += console
CONFIG -= app_bundle
CONFIG += link_pkgconfig
TEMPLATE = app


PKGCONFIG += gnuradio-analog\
             gnuradio-blocks\
             gnuradio-digital\
             gnuradio-filter\
             gnuradio-fft\
             gnuradio-runtime\
             gnuradio-uhd\
             uhd

unix:!macx {
    LIBS += -lboost_system$$BOOST_SUFFIX -lboost_program_options$$BOOST_SUFFIX
    LIBS += -lrt
    LIBS += -llog4cpp #ubuntu 20.04
}

macx{
    LIBS += -lboost_system-mt -lboost_program_options-mt
}

SOURCES += main.cpp \
    gps_source.cpp \
    transmitter.cpp \
    gpssim.c \
    runapp.cpp


HEADERS += \
    gps_source.h \
    gpssim.h \
    transmitter.h \
    main.h \
    runapp.h


