QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileOperations/abstractfileoperations.cpp \
    FileOperations/fileoperations.cpp \
    FileSystem/abstractfilesystemworker.cpp \
    FileSystem/filesystemwidget.cpp \
    FileSystem/filesystemworker.cpp \
    OSInitializer/linuxinitializerstrategy.cpp \
    OSInitializer/osinitializercreator.cpp \
    OSInitializer/osinitializerstrategy.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FileOperations/abstractfileoperations.h \
    FileOperations/fileoperations.h \
    FileSystem/abstractfilesystemworker.h \
    FileSystem/filesystemwidget.h \
    FileSystem/filesystemworker.h \
    OSInitializer/linuxinitializerstrategy.h \
    OSInitializer/osinitializercreator.h \
    OSInitializer/osinitializerstrategy.h \
    mainwindow.h

FORMS += \
    FileSystem/filesystemwidget.ui \
    mainwindow.ui

INCLUDEPATH += \
    FileSystem \
    FileOperations \
    OSInitializer

TRANSLATIONS += \
    MyFileManager_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
