import QtQuick 2.4
import QtQuick.Layouts 1.3
//import DapQmlModelSettings 1.0
import StyleDebugTree 1.0
//import SettingsInterface 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    property string formName: "Settings"
    clip: true
    //DapQmlModelSettings { id: settingsModel }
    //SettingsInterface { id: settingsInterface; Component.onCompleted: setup(settingsModel); }

    /* defs */
    enum StyleId
    {
      SI_TITLE,
      SI_TITLETOP,
      SI_BUTTON,
      SI_BUTTONRED,
      SI_BUTTONGRAY,
      SI_LINK,
      SI_SPACER
    }

    /* methods */
    function isSep(sid) {
        if (sid === QuiSettingsForm.StyleId.SI_BUTTON
        || sid === QuiSettingsForm.StyleId.SI_BUTTONRED
        || sid === QuiSettingsForm.StyleId.SI_BUTTONGRAY
        || sid === QuiSettingsForm.StyleId.SI_LINK)
            return true;
        return false;
    }

    Rectangle {
        id: settingsContainer
        anchors.fill: parent
        color: "transparent"

        ListView {
            id: settingsListView
            objectName: "settingsListView"

            x: 36
            y: 0
            width: root.width - 72
            height: root.height

            clip: true
            //model: settingsModel

            /* this item simulates resizing to give values:*/
            /* height1 -> item.height, */
            /* height2 -> title.height, */
            /* fontSize1 -> item.iconSize */
            DapQmlLabel {
                visible: false
                id: resizer1
                qss: "sett-resizer1"
//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Resizer 1",
//                    ["x", "y", "width", "height"],
//                   this);
            }

            DapQmlLabel {
                visible: false
                id: resizer2
                qss: "sett-resizer2"
//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Resizer 2",
//                    ["x", "y", "width", "height"],
//                   this);
            }

            delegate: Item {
                id: delegate
                height: model.sid !== QuiSettingsForm.StyleId.SI_TITLE ? resizer1.height : resizer2.height
                property int mySid: model.sid
                property var settingsModel: settingsListView.model

                DapQmlButton {
                    property int myIndex: model.index
                    visible: model.sid !== QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: model.textMain
                    subText: model.textSub
                    separator: isSep(model.sid)
                    qss: "sett-item"
                    mainQss: "sett-btn-lbl-main"
                    subQss: model.sid !== QuiSettingsForm.StyleId.SI_BUTTONRED ? "sett-btn-lbl-sub" : "sett-btn-lbl-sub-red"
                    icon: model.icon
                    iconSize: resizer1.fontSize
                    MouseArea {
                        anchors.fill: parent
                        onClicked: settingsModel.exec(parent.myIndex)
                    }
                }

                DapQmlLabel {
                    visible: model.sid === QuiSettingsForm.StyleId.SI_TITLE
                    width: settingsListView.width
                    height: delegate.height
                    text: model.textMain
                    qss: "sett-title-lbl-main"
                }

//                Component.onCompleted: StyleDebugTree.describe (
//                   "Settings Item " + model.index,
//                    ["height", "mySid"],
//                   this);
            }
        }
    }
}
