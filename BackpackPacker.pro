QT += quick concurrent

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
        main.cpp \
        qchecker.cpp \
        qdecoder.cpp \
        qencoder.cpp \
        qfilehandler.cpp \
        qgeneratorkey.cpp \
        qmainpackingpackageofpackedpackpacker.cpp \
        qtotalsqueezer.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Qt/Tools/OpenSSL/Win_x64/lib/ -llibcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Qt/Tools/OpenSSL/Win_x64/lib/ -llibcrypto
else:unix: LIBS += -L$$PWD/../../../../../../Qt/Tools/OpenSSL/Win_x64/lib/ -llibcrypto

INCLUDEPATH += $$PWD/../../../../../../Qt/Tools/OpenSSL/Win_x64/include
DEPENDPATH += $$PWD/../../../../../../Qt/Tools/OpenSSL/Win_x64/include

DISTFILES += \
    ButAndTextField.qml \
    CheckingValidKeyPage.qml \
    DecodePage.qml \
    EncodePage.qml \
    GenerationKeyPage.qml \
    StatAndInpTextField.qml \
    ThreeSpinBox.qml \
    icon.png

HEADERS += \
    Random.hpp \
    qchecker.h \
    qdecoder.h \
    qencoder.h \
    qfilehandler.h \
    qgeneratorkey.h \
    qmainpackingpackageofpackedpackpacker.h \
    qtotalsqueezer.h
