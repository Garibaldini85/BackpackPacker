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

    property bool   isWorking   : false
    property double decodeBar   : 0

    signal butClicked (string code, string dirKey, string dirFile, string dirDecFile)

    function checkFile(x) {
        var str = x
        //console.log(x.slice(-5), x)
        if (str.slice(-5) === ".skey") {
            str = str.slice(8)
            itemSecr.dir = str
        } else {
            str = str.slice(8)
            itemFile.dir = str
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
        width: parent.width
        height: parent.height * 0.1 - 4
        id: textCode
        placeholderText: "Введите код"
        text: qsTr("")
        ToolTip.visible: "Введенный код при генерации ключей" ? hovered : false
        ToolTip.text: "Введенный код при генерации ключей"
        visible: !isWorking
        background: Rectangle {
            anchors.fill: parent
            color: "#EEEBE7"
        }
    }

    ButAndTextField {
        id: itemSecr
        coeffHeight: 0.1
        ySpacing: parent.height * 0.1
        fieldText: "Выберите файл закрытого ключа"
        dir: ""
        isFolder: false
        filter: ["All OpenKey(*.skey)"]
        strToolTip: "Путь к закрытому ключу"
        strFileDialog : "Выберете закрытый ключ"
        isStoping: isWorking
    }

    ButAndTextField {
        id: itemFile
        coeffHeight: 0.1
        ySpacing:  parent.height * 0.2
        fieldText: "Выберите зашифрованный файл"
        dir: ""
        isFolder: false
        filter: ["All File(*)"]
        strToolTip: "Путь к зашифрованному файлу"
        strFileDialog : "Выберите зашифрованный файл"
        isStoping: isWorking
    }

    ButAndTextField {
        id: itemDecFile
        coeffHeight: 0.1
        ySpacing:  parent.height * 0.3
        fieldText: "Выберите директорию для сохранения файла"
        dir: ""
        isFolder: true
        filter: ["All File(*)"]
        strToolTip: "Путь к расшифрованному файлу. Можно оставить пустым"
        strFileDialog : "Выберите директорию для сохранения"
        isExisting: false
        isStoping: isWorking
    }

    Button {
        x: parent.width * 0.7
        y: parent.height * 0.8
        width: parent.width * 0.2
        height: parent.height * 0.1
        text: "Расшифровать"
        background: Rectangle {
            border.color: "grey"
            anchors.fill: parent
            color: "white"
        }
        onClicked: {
            if (!isWorking)
                parent.butClicked(textCode.text, itemSecr.dir, itemFile.dir, itemDecFile.dir)
        }
    }

    ProgressBar {
        x: 2
        y: 2
        width: parent.width - 4
        height: parent.height * 0.085
        value: decodeBar
        visible: isWorking

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
