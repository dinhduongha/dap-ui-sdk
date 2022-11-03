/* INCLUDES */

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
//import DapQmlModelSettings 1.0
import Qt.labs.platform 1.1
import StyleDebugTree 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Manage CDB Form
 * @ingroup groupDapQmlForms
 *
 * @date 17.10.22
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
    property string formName: "ManageCdb"

    enum Mode
    {
        M_LIST,
        M_EDIT,
        M_ADD
    }

    /// @brief controls form mode by aplying visibility rules
    property QtObject modeCtl: QtObject {
        property int mode: QuiManageCdbForm.Mode.M_LIST
        property int lastIndex: -1

        function switchToList() {
            setMode (QuiManageCdbForm.Mode.M_LIST);
        }

        function switchToEdit (a_index) {
            lastIndex   = a_index;
            fill (a_index);
            setMode (QuiManageCdbForm.Mode.M_EDIT);
        }

        function switchToAdd() {
            clearFields();
            setMode (QuiManageCdbForm.Mode.M_ADD);
        }

        function setMode (a_newMode) {
            /* store new mode */
            mode        = a_newMode;

            /* apply visibility */
            let list = (mode === QuiManageCdbForm.Mode.M_LIST);
            let edit = (mode === QuiManageCdbForm.Mode.M_EDIT);
            let add  = (mode === QuiManageCdbForm.Mode.M_ADD);

            /* display only required page */
            pageList.visible    = list;
            pageEdit.visible    = edit;
            pageAdd.visible     = add;

            /* both "edit" and "add" fields are the same */
            inputFields.visible = edit | add;
        }

        function clearFields() {
            inputAddress.clear();
        }

        function fill (a_index) {
            let model   = mancdbListView.model;

            if (a_index < 0 || a_index >= model.length || a_index === undefined)
                return;

            inputAddress.value    = model.value (a_index, "server");
        }

        function addNewServer() {
            /* get field values */
            let model   = mancdbListView.model;
            let address = inputAddress.value;

            /* store result & update model */
            model.add ({server:address});
            model.refreshContent();
        }

//        function removeServer (a_index) {
//            lastIndex   = a_index;

//            /* perform removing & update model */
//            let model   = mancdbListView.model;
//            model.remove (lastIndex);
//            model.refreshContent();
//        }

        function applyChanges() {
            /* get field values */
            let model   = mancdbListView.model;
            let address = inputAddress.value;

            /* store result & update model */
            model.edit (lastIndex, {server:address});
            model.refreshContent();
        }
    }

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief remove server menu clicked
    signal sigRemove(int a_index);

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
    function setMode (a_newMode, a_data) {

        switch(a_newMode)
        {
        case QuiManageCdbForm.Mode.M_LIST:  modeCtl.switchToList(); break;
        case QuiManageCdbForm.Mode.M_EDIT:  modeCtl.switchToEdit (a_data); break;
        case QuiManageCdbForm.Mode.M_ADD:   modeCtl.switchToAdd(); break;
        }

        // modeCtl.setMode(a_newMode);
    }

    function addNewServer() {
        modeCtl.addNewServer();
    }

    function removeServer (a_index) {
        root.sigRemove (a_index);
        //modeCtl.removeServer (a_index);
    }

    function applyChanges() {
        if (modeCtl.mode === QuiManageCdbForm.Mode.M_ADD)
            modeCtl.addNewServer();
        else
        if (modeCtl.mode === QuiManageCdbForm.Mode.M_EDIT)
            modeCtl.applyChanges();
    }

    function doImport() {
        if (mancdbListView.model === undefined)
            return;
        fileDialog.updateMode (FileDialog.OpenFile);
        fileDialog.open();
    }

    function doExport() {
        if (mancdbListView.model === undefined)
            return;
        fileDialog.updateMode (FileDialog.SaveFile);
        fileDialog.open();
    }

    function _pos (a_index) {
        return title.y + (title.height * 2.4) + (resizerItem.height * a_index);
    }

    Component.onCompleted: setMode (QuiManageCdbForm.Mode.M_LIST)

    /// @}
    /****************************************//**
     *
     * Debug
     *
     ********************************************/

//    Button {
//        x: 4; y: 4; z: 10; width: 64; height: 28; text: "list"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_LIST)
//    }

//    Button {
//        x: 4+72; y: 4; z: 10; width: 64; height: 28; text: "edit"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_EDIT)
//    }

//    Button {
//        x: 4+144; y: 4; z: 10; width: 64; height: 28; text: "add"
//        onClicked: root.setMode (QuiManageCdbForm.Mode.M_ADD)
//    }

    /****************************************//**
     * Import/Export dialog
     ********************************************/

    FileDialog {
        id: fileDialog

        property string titleName;
        property string filterImages:   qsTr("Json files (*.json)") + lang.notifier;
        property string filterAllFiles: qsTr("All files (*.*)") + lang.notifier;

        onAccepted: {
            var result      = new String (currentFile);
            var filename    = result.slice(7);

            if (fileMode === FileDialog.OpenFile)
            {
                mancdbListView.model.doImport (filename);
                mancdbListView.model.refreshContent();
            }
            else
            if (fileMode === FileDialog.SaveFile)
                mancdbListView.model.doExport (filename);

            fileDialog.close()
        }

        function updateMode (a_newMode) {
            /* save mode */
            fileMode        = a_newMode;

            /* update title based on new mode */
            titleName       = (fileMode === FileDialog.OpenFile)
            ? qsTr("Open JSON") + lang.notifier
            : qsTr("Save JSON") + lang.notifier;

            /* complete */
            fileDialog.title       = titleName;
            fileDialog.nameFilters = [ filterImages, filterAllFiles ];
        }
    }

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        visible: false
        qss: "radiobtn-resizer c-grey"
    }

    DapQmlLabel {
        id: resizeField
        visible: false
        qss: "mancdb-resizer-field c-brand"
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
            text: qsTr("Manage CDB") + lang.notifier
            qss: "dialog-title"

            /* settings button */
            DapQmlPushButton {
                id: settingsBtn
                qss: "mancdb-settings-btn"

                x: parent.width - width
                y: (parent.height - height) / 2
                z: 16

                onClicked: settingsBtnMenu.popup();

                DapQmlMenu {
                    id: settingsBtnMenu

                    /* actions */
                    Action { text: "Add new"; onTriggered: root.setMode (QuiManageCdbForm.Mode.M_ADD) }
                    Action { text: "Import list"; onTriggered: root.doImport(); }
                    Action { text: "Export list"; onTriggered: root.doExport(); }
                }
            }
        }

        /****************************************//**
         * Content
         ********************************************/

        ListView {
            id: mancdbListView
            objectName: "mancdbListView"

            property int dragItemIndex: -1
            property int dropItemIndex: -1

            x: (root.width - width) / 2
            y: title.y + title.height * 2
            width: resizerItem.width
            height: root.height - y
            clip: true

            function dropItem() {
                console.log (`dropItem ${dragItemIndex} to ${parent.myIndex}`);

                let model  = mancdbListView.model;

                model.move (dragItemIndex, dropItemIndex);
                model.refreshContent();

                dragItemIndex   = -1;
                dropItemIndex   = -1;
            }

            /****************************************//**
             * Resizers
             ********************************************/

            DapQmlLabel {
                visible: false
                id: resizerItem
                qss: "mancdb-resizer-item c-background"
                //onColorChanged: console.log(`resizerItem >> bgcolor: ${resizerItem.color}, textcolor: ${resizeField.color}`);
            }

            /****************************************//**
             * Delegate
             ********************************************/

            delegate: Item {
                id: delegateItem
                width: mancdbListView.width
                height: resizerItem.height

                /* highlight drop area */
                Rectangle {
                    id: dropArea
                    width: mancdbListView.width
                    height: resizerItem.height
                    z: 15

                    //property bool somethingOnTop: false
                    property int myIndex: model.index

                    color: resizeField.color
                    radius: 6
                    opacity: (mancdbListView.dropItemIndex === myIndex) ? 0.5 : 0

                    DropArea {
                        anchors.fill: parent
                        onEntered: {
                            console.log (`somethingOnTop [${mancdbListView.dragItemIndex},${parent.myIndex}]`);
                            //parent.somethingOnTop = true;
                            mancdbListView.dropItemIndex = parent.myIndex;
                        }
                        onExited: {
                            console.log (`!somethingOnTop [${mancdbListView.dragItemIndex},${parent.myIndex}]`);
                            //parent.somethingOnTop = false;
                        }
                    }
                }

                /* dragable item */
                Rectangle {
                    id: dragRect
                    z: 20

                    width: mancdbListView.width + mouseArea.drag.active * 20
                    height: resizerItem.height + mouseArea.drag.active * 4

                    color: (mouseArea.drag.active) ? resizerItem.color : "transparent"
                    border.color: resizer.color
                    border.width: mouseArea.drag.active // 1
                    radius: 6

                    states: [
                        State {
                            when: dragRect.Drag.active
                            ParentChange {
                                target: dragRect
                                parent: root
                            }

                            AnchorChanges {
                                target: dragRect
                                anchors.horizontalCenter: undefined
                                anchors.verticalCenter: undefined
                            }
                        }
                    ]

                    Drag.active: mouseArea.drag.active
                    Drag.hotSpot.x: dragRect.width / 2
                    Drag.hotSpot.y: dragRect.height / 2

                    DapQmlButton {
                        id: delegate

                        property int myIndex: model.index
                        property int ping: 109 // model.ping
                        property int quality: 4 // model.connectionQuality

                        x: mouseArea.drag.active * 10
                        width: mancdbListView.width
                        height: resizerItem.height
                        buttonStyle: DapQmlButton.Style.IconMainSubIcon
                        mainText: model.name
                        subText: ""
                        separator: true
                        qss: "mancdb-item"
                        mainQss: "mancdb-btn-lbl-main"
                        //subQss: "mancdb-btn-lbl-sub"
                        icon: "ic_cdb-index-icon"
                        iconSize: resizerItem.fontSize * 0.75

                        /* index */
                        DapQmlLabel {
                            y: (parent.height - height) / 2 - height * 0.25
                            width: resizerItem.fontSize * 0.75
                            height: resizerItem.fontSize * 0.75
                            qss: "mancdb-btn-lbl-main c-background"
                            text: (mouseArea.drag.active) ? "?" : (parent.myIndex + 1)
                        }

                        /* connection quality */
                        DapQmlLabel {
                            property int quality: (parent.quality === 0) ? (0) : (6 - parent.quality)
                            x: parent.width - (width * 1.35) - moreBtn.width
                            y: (parent.height - height) / 2
                            width: resizer.height * 0.5
                            height: resizer.height * 0.5
                            qss: `ic_conn-${quality}`

                            ToolTip {
                                id: id_tooltip
                                contentItem: Text{
                                    color: "#21be2b"
                                    text: "ping " + parent.ping + " ms"
                                }
                                background: Rectangle {
                                    border.color: "#21be2b"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: id_tooltip.visible = true
                                onExited: id_tooltip.visible = false
                            }
                        }

                        /* more button */
                        Button {
                            id: moreBtn
                            icon {
                                source: "qrc:/nonthemed/drag-drop-icon.png"
                                color: "transparent"
                                width: moreBtn.width
                                height: moreBtn.height
                            }
                            background: Rectangle {
                                color: "transparent"
                                border.color: "black"
                                border.width: 1
                                radius: 6
                            }
                            property int myIndex: parent.myIndex

                            x: parent.width - width
                            y: (parent.height - height) / 2 - height / 8
                            z: 16
                            width: resizerItem.fontSize * 1.25
                            height: resizerItem.fontSize * 1.25

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                drag.target: dragRect

                                drag.onActiveChanged: {
                                    if (mouseArea.drag.active) {
                                        mancdbListView.dragItemIndex = index;
                                    } else {
                                        mancdbListView.dropItem();
                                    }
                                }
                            }

        //                    onClicked: moreBtnMenu.popup();

        //                    DapQmlMenu {
        //                        id: moreBtnMenu
        //                        property int myIndex: parent.myIndex

        //                        /* actions */
        //                        Action { text: "Edit";      onTriggered: root.setMode (QuiManageCdbForm.Mode.M_EDIT, myIndex) }
        //                        Action { text: "Delete";    onTriggered: root.removeServer (myIndex); }
        //                    }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.setMode (QuiManageCdbForm.Mode.M_EDIT, parent.myIndex)
                        }

    //                Component.onCompleted: if (myIndex === 0)
    //                    StyleDebugTree.describe (
    //                        "mancdb-item",
    //                        ["x", "y", "width", "height", "icon", "iconSize"],
    //                        this);
                    }
                }
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
         * Title Buttons
         ********************************************/

        /* accept button */
        DapQmlPushButton {
            id: acceptBtn
            qss: "mancdb-accept-btn"

            x: title.x + title.width - width
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: {
                root.applyChanges();
                root.setMode (QuiManageCdbForm.Mode.M_LIST)
            }
        }

        /* cancel button */
        DapQmlPushButton {
            id: cancelBtn
            qss: "mancdb-cancel-btn"

            x: title.x
            y: title.y + (title.height - height) / 2
            z: 16

            onClicked: root.setMode (QuiManageCdbForm.Mode.M_LIST)
        }

        /****************************************//**
         * Adress Input
         ********************************************/

        DapQmlInputField {
            id: inputAddress
            x: (root.width - width) / 2
            y: _pos(0) // title.y + title.height * 2
            width: resizerItem.width
            height: resizeField.height
            clip: true
            title: "Address"
            inputMask: "000.000.000.000"
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
            hideClose: true
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
            hideClose: true
        }
    } // Add
}

/*-----------------------------------------*/
