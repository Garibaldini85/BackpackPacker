import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3

Item {
    property double coeffHeight   : 0.1
    property double coeffShift    : 0.1
    property int    coeffSpacing  : 2
    property string butText       : "Select"
    property string fieldText     : "Выберите или вставьте директорию"
    property string dir           : ""
    property int    ySpacing      : 2
    property var    filter        : ["All files (*)"]
    property bool   isFolder      : true
    property string strToolTip    : "Директория сохранения ключей"
    property string strFileDialog : "Выберете директорию"
    property bool   isExisting    : true
    property bool   isStoping     : false

    width: parent.width
    height: parent.height

    Button {
        id: but
        text: butText
        x: 2
        y: ySpacing
        width: parent.width * coeffShift - coeffSpacing
        height: parent.height * coeffHeight - coeffSpacing

        onClicked: { if (!isStoping) fileDialog.visible = true }
    }

    TextField {
        id: field
        x: parent.width * coeffShift + coeffSpacing
        y: ySpacing
        width: parent.width
        height: parent.height * coeffHeight - coeffSpacing
        placeholderText: fieldText
        text: dir
        readOnly: isStoping
        background: Rectangle {
            anchors.fill: parent
            color: "#EEEBE7"
        }
        ToolTip.visible: strToolTip ? hovered : false
        ToolTip.text: strToolTip
        onTextEdited: {
            dir = text
        }
    }

    FileDialog {
        id: fileDialog
        title: strFileDialog
        folder: shortcuts.home
        selectFolder: isFolder
        nameFilters: filter
        selectExisting: isExisting
        onAccepted: {
            dir = ("" + fileDialog.fileUrls).substring(8, ("" + fileDialog.fileUrls).length)
        }
        visible: false
    }
}
