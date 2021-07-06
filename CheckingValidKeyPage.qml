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

import QtQuick 2.0
import QtQuick.Controls 2.5

import "qml.qrc:/ButAndTextField.qml"

Rectangle {
    color: "#ffdab9"

    property string dirOpenKey: ""
    property string dirSecKey : ""
    property string code      : textCode.text
    property bool   isWorking : false

    signal butClicked (bool sendWorking)

    function checkFile(x) {
        var str = x
        //console.log(x.slice(-5), x)
        if (str.slice(-5) === ".okey") {
            str = str.slice(8)
            dirOpenKey = str
        } else if (str.slice(-5) === ".skey") {
            str = str.slice(8)
            dirSecKey = str
        }
    }

    function parserDrop(drop) {
        drop = drop.split("\n")
        if (drop.length === 1) {
            checkFile(drop[0])
        } else if (drop[1] !== "" && drop.length > 1){
            checkFile(drop[0])
            checkFile(drop[1])
        }
    }

    Rectangle {
        id: rectDrop
        anchors.fill: parent
        color: "#A7CFCE"
        visible: false

    }

    TextField {
        x:2
        y:2
        width: parent.width - 4
        height: parent.height * 0.1 - 4
        id: textCode
        placeholderText: "Введите код"
        text: qsTr("")
        background: Rectangle {
            anchors.fill: parent
            color: "#EEEBE7"
        }
        ToolTip.visible: "Введенный код при генерации ключей" ? hovered : false
        ToolTip.text: "Введенный код при генерации ключей"
    }

    ButAndTextField {
        id: itemOpen
        coeffHeight: 0.1
        ySpacing: parent.height * 0.1
        fieldText: "Выберите файл откытого ключа"
        dir: dirOpenKey
        isFolder: false
        filter: ["All OpenKey(*.okey)"]
        strToolTip: "Путь к открытому ключу"
        strFileDialog : "Выберете открытый ключ"
        isStoping: isWorking
        onDirChanged: {
            dirOpenKey = dir
        }
    }

    ButAndTextField {
        id: itemSec
        coeffHeight: 0.1
        ySpacing:  parent.height * 0.2
        fieldText: "Выберите файл закрытого ключа"
        dir: dirSecKey
        isFolder: false
        filter: ["All OpenKey(*.skey)"]
        strToolTip: "Путь к закрытому ключу"
        strFileDialog : "Выберете закрытый ключ"
        isStoping: isWorking
        onDirChanged: {
            dirSecKey = dir
        }
    }

    Button {
        objectName: "but_checkingPage"
        x: parent.width * 0.7
        y: parent.height * 0.8
        width: parent.width * 0.2
        height: parent.height * 0.1
        text: "Проверить ключи"
        background: Rectangle {
            border.color: "grey"
            anchors.fill: parent
            color: "white"
        }
        onClicked: {
            isWorking = !isWorking
            parent.butClicked(isWorking)
        }
    }

    DropArea {
        id: dropArea;
        anchors.fill: parent
        onEntered: {
            drag.accept (Qt.LinkAction);
            rectDrop.visible = true
        }
        onDropped: {
            rectDrop.visible = false
            parserDrop(drop.text)
        }
        onExited: {
            rectDrop.visible = false
        }
    }

}
