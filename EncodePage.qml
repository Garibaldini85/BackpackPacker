import QtQuick 2.0
import QtQuick.Controls 2.5

import "qml.qrc:/ButAndTextField.qml"

Rectangle {
    color: "#E7DED2"

    property bool   isWorking   : false
    property double    encodeBar   : 0

    signal butClicked (string dirKey, string dirFile, string dirEncFile)

    function checkFile(x) {
        var str = x
        //console.log(x.slice(-5), x)
        if (str.slice(-5) === ".okey") {
            str = str.slice(8)
            itemOpen.dir = str
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

    ButAndTextField {
        id: itemOpen
        coeffHeight: 0.1
        ySpacing: parent.height * 0.1
        fieldText: "Выберите файл откытого ключа"
        dir: ""
        isFolder: false
        filter: ["All OpenKey(*.okey)"]
        strToolTip: "Путь к открытому ключу"
        strFileDialog : "Выберете открытый ключ"
        isStoping: isWorking
    }

    ButAndTextField {
        id: itemFile
        coeffHeight: 0.1
        ySpacing:  parent.height * 0.2
        fieldText: "Выберите файл для шифрования"
        dir: ""
        isFolder: false
        filter: ["All File(*)"]
        strToolTip: "Путь к файлу"
        strFileDialog : "Выберите файл для шифрования"
        isStoping: isWorking
    }

    ButAndTextField {
        id: itemEncFile
        coeffHeight: 0.1
        ySpacing:  parent.height * 0.3
        fieldText: "Выберите директорию для сохранения файла"
        dir: ""
        isFolder: false
        filter: ["All File(*)"]
        strToolTip: "Путь к зашифрованному файлу"
        strFileDialog : "Выберите файл для сохранения"
        isExisting: false
        isStoping: isWorking
    }

    Button {
        x: parent.width * 0.7
        y: parent.height * 0.8
        width: parent.width * 0.2
        height: parent.height * 0.1
        text: "Зашифровать"
        background: Rectangle {
            border.color: "grey"
            anchors.fill: parent
            color: "white"
        }
        onClicked: {
            if (!isWorking)
                parent.butClicked(itemOpen.dir, itemFile.dir, itemEncFile.dir)
        }
    }

    ProgressBar {
        x: 2
        y: 2
        width: parent.width - 4
        height: parent.height * 0.085
        value: encodeBar
        visible: isWorking
        //visible: true
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
