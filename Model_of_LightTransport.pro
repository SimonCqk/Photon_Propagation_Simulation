#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T16:58:26
#
#-------------------------------------------------

QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Model_of_LightTransport
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mcml/mcml.cpp \
    mcml/mcml_io.cpp \
    mcml/utilitiy.cpp \
    mcml/mcml.cpp \
    mcml/mcml_io.cpp \
    mcml/utilitiy.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwidgets/about.cpp \
    mainwidgets/confparas.cpp \
    mainwidgets/runresults.cpp \
    mainwidgets/history.cpp

HEADERS += \
        mainwindow.h \
    mcml/mcml.h \
    mcml/mcml_fwd.h \
    mcml/mcml_impl.h \
    mcml/utility_fwd.h \
    mcml/mcml.h \
    mcml/mcml_fwd.h \
    mcml/mcml_impl.h \
    mcml/utility_fwd.h \
    mainwindow.h \
    mainwidgets/about.h \
    mainwidgets/confparas.h \
    mainwidgets/runresults.h \
    mainwidgets/history.h

FORMS += \
        mainwindow.ui \
    mainwidgets/about.ui \
    mainwidgets/confparas.ui \
    mainwidgets/runresults.ui \
    mainwidgets/history.ui

RESOURCES += \
    resources.qrc

RC_ICONS=image/logo.ico

DISTFILES += \
    image/logo.jpg \
    image/logo.ico \
    image/photon.jpeg \
    .gitignore
