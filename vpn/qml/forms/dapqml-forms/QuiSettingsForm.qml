import QtQuick 2.4
import QtQuick.Layouts 1.3
import DapQmlModelSettings 1.0
import "qrc:/dapqml-widgets"

Item {
    id: root
    DapQmlModelSettings { id: settingsModel }

    function isSep(sid) {
        if (sid === DapQmlModelSettings.SI_BUTTON
        || sid === DapQmlModelSettings.SI_BUTTONRED
        || sid === DapQmlModelSettings.SI_BUTTONGRAY
        || sid === DapQmlModelSettings.SI_LINK)
            return true;
        return false;
    }

    Rectangle {
        anchors.fill: parent
        color: "#F7F8FA"

        ListView {
            id: settingsListView

            x: (parent.width - width) / 2
            y: 0
            width: 350
            height: parent.height

            clip: true
            model: settingsModel

            delegate: Item {
                height: model.sid !== DapQmlModelSettings.SI_TITLE ? 64 : 108

                DapQmlButton {
                    property int myIndex: model.index
                    visible: model.sid !== DapQmlModelSettings.SI_TITLE
                    width: 350
                    height: 64
                    buttonStyle: DapQmlButton.Style.IconMainSub
                    mainText: model.textMain
                    subText: model.textSub
                    separator: isSep(model.sid)
                    qss: "sett-item"
                    mainQss: "sett-btn-lbl-main"
                    subQss: "sett-btn-lbl-sub"
                    icon: model.icon
                    MouseArea {
                        anchors.fill: parent
                        onClicked: settingsModel.exec(parent.myIndex)
                    }
                }

                DapQmlLabel {
                    visible: model.sid === DapQmlModelSettings.SI_TITLE
                    width: 350
                    height: 108
                    text: model.textMain
                    qss: "sett-title-lbl-main"
                }
            }
        }

//        GridLayout {
//            width: 400
//            columns: 1

//            DapQmlButton {
//                width: 350
//                height: 108

//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: width
//                Layout.preferredHeight: height

//                buttonStyle: DapQmlButton.Style.TopMainBottomSub
//                mainText: "Test item 1"
//                subText: "TopMainBottomSub"
//            }

//            DapQmlButton {
//                width: 350
//                height: 108

//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: width
//                Layout.preferredHeight: height

//                buttonStyle: DapQmlButton.Style.TopSubBottomMain
//                mainText: "Test item 2"
//                subText: "TopSubBottomMain"
//            }

//            DapQmlButton {
//                width: 350
//                height: 108

//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: width
//                Layout.preferredHeight: height

//                buttonStyle: DapQmlButton.Style.LeftTopMainBottomSub
//                leftText: "$ Third"
//                mainText: "Test item 3"
//                subText: "TopSubBottomMain"
//            }

//            DapQmlSeparator {
//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: width
//                Layout.preferredHeight: height
//            }

//            DapQmlPushButton {
//                id: btnConnect
//                checkable: true
//                checked: true
//                Layout.alignment: Qt.AlignHCenter
//                Layout.preferredWidth: width
//                Layout.preferredHeight: height
//                text: qsTr("CONNECT")
//            }

//        }
    }
}
