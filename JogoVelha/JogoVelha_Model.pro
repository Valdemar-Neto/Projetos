QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  JogoVelha_Model/jogovelhamodel.cpp \
  JogoVelha_Model/main.cpp \
  JogoVelha_Model/mainjogovelha.cpp \
  jogovelha.cpp

HEADERS += \
  JogoVelha_Model/jogovelhamodel.h \
  JogoVelha_Model/mainjogovelha.h \
  jogovelha.h

FORMS += \
  JogoVelha_Model/mainjogovelha.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
