/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
//import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Manage Servers Form
 * @ingroup groupDapQmlForms
 *
 * @date 15.07.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root
    clip: true

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "ManageServers"

    enum Mode
    {
        M_LIST,
        M_EDIT,
        M_ADD
    }

    /// @brief controls form mode by aplying visibility rules
    property QtObject modeCtl: QtObject {
        property int mode: QuiManageServersForm.Mode.M_LIST

        function setMode(a_newMode) {
            /* store new mode */
            mode        = a_newMode;

            /* apply visibility */
            let list = (mode === QuiManageServersForm.Mode.M_LIST);
            let edit = (mode === QuiManageServersForm.Mode.M_EDIT);
            let add  = (mode === QuiManageServersForm.Mode.M_ADD);
            pageList.visible    = list;
            pageEdit.visible    = edit;
            pageAdd.visible     = add;
            inputFields.visible = edit | add;
        }
    }

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief get current mode
    function mode() {
        return modeCtl.mode;
    }

    /// @brief set new mode
    function setMode(a_newMode) {
        modeCtl.setMode(a_newMode);
    }

    function _pos(a_index) {
        return title.y + (title.height * 2.4) + (resizerItem.height * a_index);
    }

    Component.onCompleted: setMode (QuiManageServersForm.Mode.M_LIST)

    /// @}
    /****************************************//**
     *
     * Debug
     *
     ********************************************/

    Button {
        x: 4; y: 4; z: 10; width: 64; height: 28; text: "list"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_LIST)
    }

    Button {
        x: 4+72; y: 4; z: 10; width: 64; height: 28; text: "edit"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_EDIT)
    }

    Button {
        x: 4+144; y: 4; z: 10; width: 64; height: 28; text: "add"
        onClicked: root.setMode (QuiManageServersForm.Mode.M_ADD)
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer"
    }

    DapQmlLabel {
        id: resizeField
        visible: false
        qss: "manser-resizer-field"
        //onColorChanged: console.log(`resizeField >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
    }

    DapQmlLabel {
        id: spacer
        visible: false
        qss: "radiobtn-spacer"
    }

    /****************************************//**
     *
     * List
     *
     ********************************************/

    Item {
        id: pageList
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            id: title
            text: qsTr("Manage Servers") + lang.notifier
            qss: "dialog-title"

            /* settings button */
            DapQmlPushButton {
                id: settingsBtn
                qss: "manser-settings-btn"

                x: parent.width - width
                y: (parent.height - height) / 2
                z: 16

                onClicked: settingsBtnMenu.popup();

                DapQmlMenu {
                    id: settingsBtnMenu

                    /* actions */
                    Action { text: "Restart server"; }
                    Action { text: "Add new" }
                    Action { text: "Import list" }
                    Action { text: "Export list" }
                }
            }
        }

        /****************************************//**
         * Content
         ********************************************/

        ListView {
            id: manserListView
            objectName: "manserListView"

            x: (root.width - width) / 2
            y: title.y + title.height * 2
            width: resizerItem.width
            height: root.height - y
            clip: true

            /****************************************//**
             * Resizers
             ********************************************/

            DapQmlLabel {
                visible: false
                id: resizerItem
                qss: "manser-resizer-item"
                //onColorChanged: console.log(`resizerItem >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
            }

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: DapQmlButton {
                id: delegate

                property int myIndex: model.index

                width: manserListView.width
                height: resizerItem.height
                buttonStyle: DapQmlButton.Style.IconMainSubIcon
                mainText: model.name
                subText: ""
                separator: true
                qss: "manser-item"
                mainQss: "manser-btn-lbl-main"
                //subQss: "manser-btn-lbl-sub"
                icon: model.icon
                iconSize: resizerItem.fontSize

                /* icon favorite */
                Button {
                    id: checkFavorite
                    checkable: true
                    checked: model.favorite
                    icon {
                        source: (checkFavorite.checked)
                                ? "qrc:/nonthemed/star-checked.png"
                                : "qrc:/nonthemed/star-unchecked.png"
                        color: "transparent"
                        width: checkFavorite.width
                        height: checkFavorite.height
                    }
                    background: Rectangle { color: "transparent" }

                    x: parent.width - width * 2// - (74 / 2)
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: parent.iconSize * 1.25
                    height: parent.iconSize * 1.25

                    //onCheckedChanged: internal.showPassword = checked
                }

                /* more button */
                Button {
                    id: moreBtn
                    icon {
                        source: "qrc:/nonthemed/more.png"
                        color: "transparent"
                        width: moreBtn.width
                        height: moreBtn.height
                    }
                    background: Rectangle { color: "transparent" }

                    x: parent.width - width
                    y: (parent.height - height) / 2 - height / 8
                    z: 16
                    width: parent.iconSize * 1.25
                    height: parent.iconSize * 1.25

                    onClicked: moreBtnMenu.popup();

                    DapQmlMenu {
                        id: moreBtnMenu

                        /* actions */
                        Action { text: "Edit"; }
                        Action { text: "Delete" }
                    }
                }

                function buttonClicked(a_isButtonSignal) {
                    if(!a_isButtonSignal)
                        clicked();
                    model.exec (myIndex, this);
                }

                onClicked: buttonClicked(true)

                MouseArea {
                    anchors.fill: parent
                    onClicked: parent.buttonClicked(false)
                }

//                Component.onCompleted: if (myIndex === 0)
//                    StyleDebugTree.describe (
//                        "manser-item",
//                        ["x", "y", "width", "height", "icon", "iconSize"],
//                        this);
            }
        }
    } // List

    /****************************************//**
     *
     * Input Fields
     *
     ********************************************/

    Item {
        id: inputFields
        anchors.fill: parent

        /****************************************//**
         * Title Input
         ********************************************/

        DapQmlInputField {
            id: inputTitle
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Title"
        }

        /****************************************//**
         * Adress Input
         ********************************************/

        DapQmlInputField {
            id: inputAddress
            x: (root.width - width) / 2
            y: _pos(1) // inputTitle.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Address"
        }

        /****************************************//**
         * Port Input
         ********************************************/

        DapQmlInputField {
            id: inputPort
            x: (root.width - width) / 2
            y: _pos(2) // inputAddress.y + resizerItem.height
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Port"
        }

    }

    /****************************************//**
     *
     * Edit
     *
     ********************************************/

    Item {
        id: pageEdit
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Edit server") + lang.notifier
            qss: "dialog-title"
        }

    } // Edit

    /****************************************//**
     *
     * Add
     *
     ********************************************/

    Item {
        id: pageAdd
        anchors.fill: parent

        /****************************************//**
         * Title
         ********************************************/

        DapQmlDialogTitle {
            text: qsTr("Add a new server") + lang.notifier
            qss: "dialog-title"
        }
    } // Add
}

/*-----------------------------------------*/
