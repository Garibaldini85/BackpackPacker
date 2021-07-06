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

import "qml.qrc:/StatAndTextField.qml"
import "qml.qrc:/ButAndTextField.qml"
import "qml.qrc:/ThreeSpinBox.qml"

Rectangle {
    color: "#ffdab9"

    property bool   isWorking       : false
    property int    bufBytes        : itemSpinBox.bufBytes
    property int    bufBlock        : itemSpinBox.bufBlock
    property int    bufSize         : itemSpinBox.bufSize
    property double openBar         : 0
    property double secBar          : 0
    property bool   isFindSec       : false

    signal butClicked (string code, string dir, int key, int bytes)

    Grid {
        width: parent.width
        height: parent.height * 0.3
        columns: 1
        rows: 3
        rowSpacing: 2
        columnSpacing: 2

        StatAndInpTextField {
            id: itemGenCode
            height: parent.height
            coeffHeight: 0.3
            staticText: "Введите или сгенерируйте код:"
            isStoping: isWorking
        }

        ButAndTextField {
            id: itemDir
            coeffHeight: 0.3
            ySpacing: -1 * parent.height * 0.4
            isStoping: isWorking
        }

        ThreeSpinBox{
             id: itemSpinBox
             coeffHeight: 0.63
             xShift: parent.width * 0.102
             yShift: -1 * parent.height * 0.9
             yGridRowSpacing: parent.height * 0.35
             isStoping: isWorking
        }

    }

    ProgressBar {
        x: parent.width * 0.55
        y: parent.height * 0.596
        width: parent.width * 0.35
        height: parent.height * 0.085
        value: secBar
        visible: isWorking
    }

    ProgressBar {
        x: parent.width * 0.55
        y: parent.height * 0.683
        width: parent.width * 0.35
        height: parent.height * 0.085
        value: openBar
        indeterminate: !isFindSec
        visible: isWorking
    }

    Button {
        x: parent.width * 0.7
        y: parent.height * 0.8
        width: parent.width * 0.2
        height: parent.height * 0.1
        text: "Сгенерировать"

        background: Rectangle {
            border.color: "grey"
            anchors.fill: parent
            color: "white"
        }

        onClicked: {
            if (!isWorking)
                parent.butClicked(itemGenCode.code, itemDir.dir, itemSpinBox.key, itemSpinBox.bytes)
        }
    }
}
