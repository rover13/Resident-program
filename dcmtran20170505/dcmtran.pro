#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T21:20:04
#
#-------------------------------------------------

QT       += core gui widgets network xml

TARGET = dcmtran
TEMPLATE = app
CONFIG	+= qt thread

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    WorkThread.cpp \
    Transfer.cpp \
    Settings.cpp \
    SendPacsFile.cpp \
    SendCasesFile.cpp \
    YYTFileSystemWatcher.cpp \
    NetworkHelper.cpp \
    OperationXML.cpp \
    FileManage.cpp

HEADERS += \
    WorkThread.h \
    Transfer.h \
    Settings.h \
    SendPacsFile.h \
    SendCasesFile.h \
    YYTFileSystemWatcher.h \
    NetworkHelper.h \
    OperationXML.h \
    FileManage.h

FORMS +=

LIBS += -L"$(DCMTK_DIR)/lib" \
    -ldcmnet -ldcmdata -loflog -lofstd \
    -lz -lkernel32 -lws2_32 -lnetapi32

INCLUDEPATH += "$(DCMTK_DIR)/include/"

DISTFILES += win32_res.rc app_icon.ico

RC_FILE += win32_res.rc
