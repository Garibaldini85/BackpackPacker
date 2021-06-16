import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

Item {
    property string staticText   : "Static Text"
    property double coeffHeight  : 0.1
    property double coeffShift   : 0.1
    property int    coeffSpacing : 2
    property string code         : codeField.text
    property bool   isStoping    : false

    width: parent.width
    height: parent.height

    function generateNewCode() {
        var min = 65;
        var max = 122;
        var maxStr = 20
        var minStr = 10
        var count = Math.floor(Math.random() * (maxStr - minStr)) + minStr
        var intCode = []

        for (var i = 0; i < count; i++) {
            intCode.push(Math.floor(Math.random() * (max - min)) + min)
        }

        return String.fromCharCode.apply(null, intCode)
    }

    TextField {
        x: 2
        y: 2
        width: parent.width
        height: parent.height * coeffHeight - coeffSpacing
        text: staticText
        readOnly: true

        background: Rectangle {
            anchors.fill: parent
            color: "#E7DED2"
        }
    }

    Button {
        text: "Gen"
        x: 2
        y: parent.height * coeffHeight + coeffSpacing
        width: parent.width * coeffShift - coeffSpacing
        height: parent.height * coeffHeight - coeffSpacing
        onClicked: { if (!isStoping) codeField.text = generateNewCode() }
    }

    TextField {
        id: codeField
        x: parent.width * coeffShift + coeffSpacing
        y: parent.height * coeffHeight + coeffSpacing
        width: parent.width
        height: parent.height * coeffHeight - coeffSpacing
        readOnly: isStoping

        background: Rectangle {
            anchors.fill: parent
            color: "#EEEBE7"
        }

        placeholderText: qsTr("Сгенерируйте или впишите код")
        text: ""

        ToolTip.visible: "Код необходим для защиты ключей. Его стоит запомнить" ? hovered : false
        ToolTip.text: "Код необходим для защиты ключей. Его стоит запомнить"
    }

}
