/*
 *  BackpackPacker Copyright (C) 2021  Kambarov I. G.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Subsequent modifications must be distributed under the same license.
 */

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

import "qml.qrc:/GenerationKeyPage.qml"
import "qml.qrc:/StatAndTextField.qml"
import "qml.qrc:/ButAndTextField.qml"
import "qml.qrc:/ThreeSpinBox.qml"
import "qml.qrc:/CheckingValidKeyPage.qml"
import "qml.qrc:/EncodePage.qml"
import "qml.qrc:/DecodePage.qml"
import "qml.qrc:/StatusItem.qml"
import "qml.qrc:/StatusList.qml"
import "qml.qrc:/Notification.qml"

/*
 * "ver_0.1.1"
*/

ApplicationWindow {
    id: window
    width: winWidth
    height: 480
    visible: true
    minimumHeight : 500
    minimumWidth : 800
    title: titleWin

    property string titleWin      : "ver_0.2.0"
    property bool   isWorking     : false
    property string titleMess     : ""
    property string textMess      : ""
    property bool   isViewingMess : false
    property bool   butPress      : false
    property int    winWidth      : 640
    property int    xShift        : 0

    signal setIndeterminate (string idTask, bool sendBool)
    signal winCreateTask    (string idTask, string description)
    signal setStatusTask    (string idTask, double status)

    function closeListStatus() {
        winWidth = window.width - 300
        xShift = 0
    }

    function openListStatus() {
        winWidth = window.width + 300
        xShift = 300
    }

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

        onSendError: {
            titleMess = titleErr
            textMess = strErr
            isViewingMess = true
        }
        onCreateTask:       { list.createTask(idTask, description) }
        onDeleteTask:       { list.deleteTask(idTask) }
        onSetIndeterminate: { list.setInterm(idTask, boolshit) }
        onSetStatusTask:    { list.setStatus(idTask, status) }
        onSetNameTask:      { list.setName(idTask, nameTask) }
        onViewSystemNotif:  { notif.showNotif(titleNotification, textNotification, dir) }
    }

    Row {
        Column {
            Repeater {
                model: view.count
                Rectangle {
                    width: window.width * 0.2 - xShift * 0.2
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
            x: window.width * 0.2 - xShift * 0.2

            width: window.width * 0.8 - xShift * 0.8
            height: window.height
            style: TabViewStyle {
                tab: Rectangle {color : "#C5DDE4"}
            }

            Tab {
                title: "Генерация ключей"
                GenerationKeyPage {
                    onButClicked: {
                        packer.generateKeys(code, dir, bufSize, bufBytes)
                    }
                    isWorking: window.isWorking
                }
            }
            Tab {
                title: "Проверка ключей"
                CheckingValidKeyPage {
                    onButClicked: {
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
                }
            }
            Tab {
                title: "Расшифрование файлов"
                DecodePage {
                    onButClicked: {
                        packer.decodeFile(code, dirKey, dirFile, dirDecFile)
                    }
                    isWorking: window.isWorking
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
                    if (clickedButton === StandardButton.Ok)
                        isViewingMess = false
        }
    }

    StatusList {
        id: list
        width: xShift
        height: parent.height
        x: parent.width - xShift
        y: 0
        visible: xShift
    }

    Button {
        id: but_list_close
        x: parent.width * 0.98 - xShift * 0.978
        y: parent.height * 0.4
        width: 15
        height: parent.height * 0.15
        onClicked: {
            if (!butPress)
                openListStatus()
            else
                closeListStatus()
            butPress = !butPress
        }
        ToolTip.visible: hovered
        ToolTip.text: "Кнопка для представление всех процессов"
    }

    Notification {
        id: notif
        onOpenFolder: { packer.openFolder(path) }
        onOpenGit:    { packer.openGitRep() }
    }

}

