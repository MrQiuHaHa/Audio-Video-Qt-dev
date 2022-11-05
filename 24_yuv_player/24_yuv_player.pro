QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    yuvplayer.cpp

HEADERS += \
    mainwindow.h \
    yuvplayer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
    FFMPEG_HOME = F:/Dev/msys64/usr/local/ffmpeg

    INCLUDEPATH += $${FFMPEG_HOME}/include

    LIBS += -L$${FFMPEG_HOME}/lib \
            -lavutil \
            -lSDL2
}

mac {
    FFMPEG_HOME = /opt/homebrew/Cellar/ffmpeg/5.1.2
    SDL_HOME = /opt/homebrew/Cellar/sdl2/2.24.1

    INCLUDEPATH += $${FFMPEG_HOME}/include
    LIBS += -L$${FFMPEG_HOME}/lib \
            -lavutil

    INCLUDEPATH += $${SDL_HOME}/include
    LIBS += -L$${SDL_HOME}/lib \
            -lSDL2
}
