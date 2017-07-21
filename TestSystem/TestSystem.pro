#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T10:10:53
#
#-------------------------------------------------

QT       += core gui network  webkit

TARGET = TestSystem
TEMPLATE = app


SOURCES += main.cpp\
        testsystemui.cpp \
    engine/keyboardwidget.cpp \
    engine/laprinter.cpp \
    engine/common.cpp \
    engine/lascanner.cpp \
    engine/lahw.cpp \
    engine/labcr.cpp \
    ui_utility_page.cpp \
    ui_hwinfo_page.cpp \
    ui_printer_page.cpp \
    ui_scanner_page.cpp \
    ImageViewerServer/mysocketcontroler.cpp \
    ImageViewerServer/imageviewerserver.cpp \
    ui_imageview_page.cpp \
    ui_check_status.cpp \
    choiceotherpro.cpp \
    copysysthread.cpp \
    videoplay.cpp

HEADERS  += testsystemui.h \
    engine/keyboardwidget.h \
    engine/laprinter.h \
    engine/def.h \
    engine/common.h \
    engine/lascanner.h \
    engine/lahw.h \
    engine/labcr.h \
    ImageViewerServer/mysocketcontroler.h \
    ImageViewerServer/imageviewerserver.h \
    choiceotherpro.h \
    copysysthread.h \
    videoplay.h

FORMS    += testsystemui.ui \
    engine/keyboardwidget.ui \
    choiceotherpro.ui \
    videoplay.ui

LIBS	+=	-ldl

#deployment
sources.files = TestSystem
target.path = /etc/LATECH/APP
sources.path = TestSystem
INSTALLS += target

RESOURCES += \
    images.qrc
