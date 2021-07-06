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
import QtQuick.Controls 2.12

Rectangle {
    color: "#ffefd5"
    //border.color : "#c8b498"
    //ffefd5 E7DED2 c8b498
    border.color : "#E7DED2"
    border.width : 2

    property string idTask: "ggg"
    property double status: 0
    property bool   setIndeterminate: false
    property string nameTask: "omnom"

    function getId() { return idTask }

    Text {
        x: 2
        y: 2
        height: parent.height * 0.5
        verticalAlignment: Text.AlignVCenter
        width: parent.width * 0.5 - 2
        text: idTask
    }

    Text {
        x: parent.width * 0.5
        y: 2
        height: parent.height * 0.5
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        width: parent.width * 0.5 - 2
        text: nameTask
    }

    ProgressBar {
        x: 2
        y: parent.height * 0.5 + 2
        width: parent.width - 2
        height: parent.height * 0.5 - 4
        indeterminate: setIndeterminate
        value: status
        //visible: isWorking
    }
}
