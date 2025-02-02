/* INCLUDES */

import QtQuick 2.1
import QtQuick.Controls 2.12
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Choose Server Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using simple ListView with DapQmlRadioButton as delegate
 *
 * Form contains resizer for ListView items
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ChooseServer"

    /// @brief items array
    ///
    /// Need to store all items
    property var items: new Array

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief item clicked
    signal sigSelect(int index, string name);
//    signal sigCurrentInexChanged();

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    Timer {
        interval: 500
        running: true
        repeat: false
        onTriggered: {
            let size = items.length;
            for (let i = 0; i < size; i++)
                items[i].hoverEnabled = true;
        }
    }

    /// @}
    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        id: title
        text: qsTr("Choose server") + lang.notifier
        qss: "dialog-title"
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlRectangle {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     * Listview
     ********************************************/

    ListView {
        id: csListView
        objectName: "chooseServerListView"

        x: (root.width - width) / 2
        y: title.y + title.height * 2
        width: resizer.width
        height: root.height - y
        clip: true

        delegate: Item {
            width: resizer.width
            height: resizer.height + spacer.height
            property bool checked: false

            DapQmlRadioButton {
                property int ping: model.ping + csListView.model.hookInt
                property int quality: model.connectionQuality + csListView.model.hookInt

                text: model.name + csListView.model.hook
                checked: csListView.model.current === model.index //model.checked + csListView.model.hookInt
                separator: true
                iconSize: resizer.height
                width: resizer.width
                height: resizer.height
                y: spacer.height / 2

                // for debug purposes - uncomment 'Text' below
//                Text {
//                    text: `${model.name} >> ping [${parent.ping}] quality [${parent.quality}]` + csListView.model.hook
//                }

                DapQmlLabel {
                    property int quality: (parent.quality === 0) ? (0) : (6 - parent.quality)
                    x: parent.width - (width * 1.35)
                    y: (parent.height - height) / 2
                    width: resizer.height * 0.5
                    height: resizer.height * 0.5
                    qss: `ic_conn-${quality}` + csListView.model.hook

                    ToolTip {
                        id: id_tooltip

                        background: Rectangle {
                            border.color: "#404040"
                        }

                        contentItem: Text {
                            color: "#404040"
                            text: (ping > -1)
                                  ? (`ping ${ping} ms` + csListView.model.hook)
                                  : ("unavailable" + csListView.model.hook)
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        //hoverEnabled: true
                        onEntered: id_tooltip.visible = true
                        onExited: id_tooltip.visible = false
                        Component.onCompleted: { items.push(this); }
                    }
                }

                onClicked: root.sigSelect (model.index, model.name)
            }
        }
    }
}

/*-----------------------------------------*/
