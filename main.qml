import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

import "qml.qrc:/GenerationKeyPage.qml"
import "qml.qrc:/StatAndTextField.qml"
import "qml.qrc:/ButAndTextField.qml"
import "qml.qrc:/ThreeSpinBox.qml"
import "qml.qrc:/CheckingValidKeyPage.qml"
import "qml.qrc:/EncodePage.qml"
import "qml.qrc:/DecodePage.qml"

/*
 * "ver_0.1.1"
*/

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    minimumHeight : 500
    minimumWidth : 800
    title: titleWin

    property string titleWin      : "ver_0.1.1"
    property bool   isWorking     : false
    property string titleMess     : ""
    property string textMess      : ""
    property bool   isViewingMess : false
    property double winOpenBar    : 0
    property double winSecBar     : 0
    property bool   winIsFindSec  : false
    property int    winBufSize    : 0
    property int    winEncodeDone : 0
    property int    winFileBytes  : 0
    property int    winDecodeDone : 0

    function setTitleWin(x) {
        if (x === 0)
            titleWin = "Генерация ключей"
        else if (x === 1)
            titleWin = "Проверка ключей"
        else if (x === 2)
            titleWin = "Шифрование файлов"
        else if (x === 3)
            titleWin = "Расшифрование файлов"
        else if (x === 4)
            Qt.quit()
    }

    Connections {
        target: packer
        onSendStatus: {
            window.isWorking = status
        }
        onSendError: {
            titleMess = titleErr
            textMess = strErr
            isViewingMess = true
        }
        onSendSecCounter: {
            winSecBar = counter / winBufSize
        }
        onSendOpenCounter: {
            winOpenBar = counter / winBufSize
        }
        onSendCompletedFindSec: {
            winIsFindSec = isFind
        }
        onSendSizeFile: {
            winFileBytes = size
        }

        onSendStatusEnc: {
            winEncodeDone = status
        }

        onSendStatusDec: {
            winDecodeDone = status
        }

    }

    Row {
        Column {
            Repeater {
                model: view.count
                Rectangle {
                    width: window.width * 0.2
                    height: window.height * 0.2
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: view.getTab(index).title
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if (!window.isWorking) {
                                view.currentIndex = index
                                setTitleWin(index)
                            }
                        }

                    }
                }
            }
        }
        TabView {
            id: view
            x: window.width * 0.2

            width: window.width * 0.8
            height: window.height
            style: TabViewStyle {
                tab: Rectangle {color : "#C5DDE4"}
            }

            Tab {
                title: "Генерация ключей"
                GenerationKeyPage {
                    onButClicked: {
                        packer.generateKeys(code, dir, bufSize, bufBytes)
                        window.winBufSize = bufSize
                    }
                    isWorking: window.isWorking
                    openBar: winOpenBar
                    secBar: winSecBar
                    isFindSec: winIsFindSec
                }
            }
            Tab {
                title: "Проверка ключей"
                CheckingValidKeyPage {
                    onButClicked: {
                        //void QMainPackingPackageOfPackedPackPacker::checkValidKey(QString code, QString dirOpenKey, QString dirSecrKey)

                        //window.isWorking = sendWorking
                        packer.checkValidKey(code, dirOpenKey, dirSecKey)
                    }
                    isWorking: window.isWorking
                }
            }
            Tab {
                title: "Шифрование файлов"
                EncodePage {
                    onButClicked: {
                        packer.encodeFile(dirKey, dirFile, dirEncFile)
                    }
                    isWorking: window.isWorking
                    encodeBar: winEncodeDone / winFileBytes

                }
            }
            Tab {
                title: "Расшифрование файлов"
                DecodePage {
                    onButClicked: {
                        packer.decodeFile(code, dirKey, dirFile, dirDecFile)
                    }
                    isWorking: window.isWorking
                    decodeBar: winDecodeDone / winFileBytes
                }
            }
            Tab {
                title: "Выход"
            }
        }
    }

    Dialog {
        visible:isViewingMess
        id: viewMess
        title: titleMess
        Column {
            anchors.fill: parent
            Text {
                text: textMess
            }
        }
        standardButtons: StandardButton.Ok

        onButtonClicked: {
                    if (clickedButton == StandardButton.Ok)
                        isViewingMess = false
        }
    }
}

