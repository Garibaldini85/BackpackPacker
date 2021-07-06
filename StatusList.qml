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

import QtQuick 2.5
import QtQuick.Controls 2.12

import "qml.qrc:/StatusItem.qml"

Rectangle {
    color: "#ffdab9"
    //border.color : "#c8b498"
    //border.width : 2

    property int number    : 0
    property var listStatus: []

    function createTask(x, y) {
        var ans = delegate.createObject(listView1)
        ans.idTask   = x
        ans.nameTask = y
        listStatus.push(ans)
        number += 1
    }

    function searchTask(x) {
        for (let j = 0; j < listStatus.length; j++) {
            if (listStatus[j].idTask === x)
                return j
        }
    }

    function deleteTask(x) {
        var i = searchTask(x)
        var obj = listStatus[i]
        listStatus.splice(i, 1)
        obj.destroy()
        number -= 1
    }

    function setInterm(x, y) {
        listStatus[searchTask(x)].setIndeterminate = y
    }

    function setStatus(x, y) {
        listStatus[searchTask(x)].status = y
    }

    function setName(x, y) {
        listStatus[searchTask(x)].nameTask = y
    }

    Flickable {
        anchors.fill: parent
        interactive: true
        clip: false
        contentWidth:  parent.width
        contentHeight: number * 75
        Grid {
            id: listView1
            anchors.fill: parent
            columns: 1
        }
    }

    Component {
        id: delegate
        StatusItem {
            width: parent.width
            height: 75
        }
    }
}
