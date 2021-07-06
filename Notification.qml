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
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.1

Item {
    property int counter: 0
    property int _counter: 0
    property var path: []
    property var _path: []
    property string msg: ""
    property string ttl: ""
    property var arrName: []

    signal openFolder(string path)
    signal openGit()

    function showNotif(x, y, dir) {
        if (!timer.running && isShowNotif.checked)
            timer.start()
        counter += 1; ttl = x; msg = y; path.push(dir); arrName.push(y)
    }

    function arrToStr() {
        let str = ""
        let set = []
        for (let item of arrName) {
            let isFind = false
            for (let _item of set)
                if (item === _item[0]) {
                    _item[1] += 1
                    isFind = true
                }
            if (!isFind)
                set.push([item, 1])
        }

        for (let y of set) {
            str += y[0] + "[" + y[1] + "], "
        }
        str = str.slice(0, -2)
        return str
    }

    function getPath() {
        var set = new Set(_path)
        if (set.size === 1)
            return _path[0]
        else
            return ""
    }

    Timer {
        id: timer
        interval: 2000;
        onTriggered: {
            switch (counter) {
            case 1:
                notif.showMessage("Выполнена " + ttl + " задача", msg, 500)
                break
            case 2:
            case 3:
            case 4:
                notif.showMessage("Выполнено " + (counter).toString() + " задачи", arrToStr(), 500)
                break
            default:
                notif.showMessage("Выполнено " + (counter).toString() + " задач", arrToStr(), 500)
                break
            }
            _counter = counter
            _path = path
            path = []
            counter = 0
            arrName = []
        }
    }

    SystemTrayIcon {
        id: notif
        visible: true
        icon.source: "qrc:/icon.png"

        onMessageClicked: openFolder( getPath() )

        Component.onCompleted: showMessage("Подсказка", "Если работали в одной директории, то по клику она откроется")
        menu: Menu {
            MenuItem {
                id: isShowNotif
                checkable: true
                checked: true
                text: qsTr("Уведомления")
            }
            MenuItem {
                id: showGit
                text: qsTr("Исходный код")
                onTriggered: openGit()
            }
            MenuItem {
                text: qsTr("Выход")
                onTriggered: Qt.quit()
            }
        }
    }
}
