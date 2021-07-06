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

Item {

    property double coeffHeight     : 0.23
    property double coeffShift      : 0.7
    property int    coeffSpacing    : 2
    property int    coeffColSpacing : width * 0.1
    property int    xShift          : width * 0.1
    property int    yShift          : 2
    property int    yGridRowSpacing : 2
    property int    counterBlock    : 1
    property bool   isStoping       : false
    property int    bufBytes        : 50
    property int    bufBlock        : 1
    property int    bufSize         : 16

    width: parent.width
    height: parent.height
    Grid{
        x: xShift
        y: yShift
        width: parent.width
        height: parent.height
        columns: 1
        rows: 2
        rowSpacing: yGridRowSpacing
        Grid {
            width: parent.width / 2
            height: parent.height / 2
            rows: 2
            columns: 2
            columnSpacing: coeffColSpacing
            TextField {
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                text: "Размер ключей:"
                horizontalAlignment: Text.AlignHCenter
                focus: false
                enabled: true
                readOnly: true
                ToolTip.visible: "Размер открытого и закрытого ключей" ? hovered : false
                ToolTip.text: "Размер открытого и закрытого ключей"
            }
            TextField {
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                text: "Размер блока:"
                horizontalAlignment: Text.AlignHCenter
                enabled: true
                focus: false
                placeholderText: qsTr("")
                readOnly: true
                ToolTip.visible: "Размер единичного блока в байтах" ? hovered : false
                ToolTip.text: "Размер единичного блока в байтах"
            }

            SpinBox {
                id: spb_vecSize
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                stepSize: 16
                from: 16
                to: 320
                value: counterBlock * 16

                onValueModified: {
                    if (isStoping) {
                        value = bufSize
                    } else {
                        spb_blockSize.value = value / 16
                        bufBlock = value / 16
                        bufSize = value
                    }
                }
            }

            SpinBox {
                id: spb_blockSize
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                stepSize: 1
                from: 1
                to: 20
                value: counterBlock

                onValueModified: {
                    if (isStoping) {
                        value = bufBlock
                    } else {
                        spb_vecSize.value = value * 16
                        bufBlock = value
                        bufSize = value * 16
                    }
                }
            }
        }

        Grid {
            width: parent.width / 2
            height: parent.height / 2
            rows: 2
            columns: 1
            TextField {
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                text: "Кол-во байт:"
                horizontalAlignment: Text.AlignHCenter
                renderType: Text.QtRendering
                antialiasing: false
                focus: false
                layer.wrapMode: ShaderEffectSource.ClampToEdge
                enabled: true
                readOnly: true
                ToolTip.visible: "Минимальное количество байт в ключе" ? hovered : false
                ToolTip.text: "Минимальное количество байт в ключе"
            }

            SpinBox {
                id: spb_bytesSize
                width: parent.width * coeffShift - coeffSpacing
                height: parent.height * coeffHeight - coeffSpacing
                stepSize: 10
                from: 50
                to: 200
                onValueChanged: {
                    if (isStoping)
                        value = bufBytes
                    else
                        bufBytes = value
                }
            }
        }
    }
}
