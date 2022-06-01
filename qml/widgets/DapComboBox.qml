import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

ComboBox {
    id: control

    implicitHeight: 45 * pt

    leftPadding: 15 * pt
    rightPadding: 10 * pt

    property int maximumPopupHeight: 200 * pt

    property string mainTextRole: "name"
    property string secondTextRole: "secondname"

    property string defaultText: qsTr("Undefined")

    displayText: currentIndex >= 0 ?
                     getModelData(currentIndex, mainTextRole) :
                     defaultText

    delegate:
    ItemDelegate
    {
        id: menuDelegate
        width: control.width

        contentItem:
        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: parent.leftPadding
            anchors.rightMargin: 20 * pt

            Text
            {
                Layout.fillWidth: true
                text: getModelData(index, mainTextRole)
                color: menuDelegate.highlighted ?
                           currTheme.hilightTextColorComboBox :
                           currTheme.textColor
                font: control.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Text
            {
                text: getModelData(index, secondTextRole)
                color: menuDelegate.highlighted ?
                           currTheme.hilightTextColorComboBox :
                           currTheme.textColor
                font.family: control.font.family
                font.pointSize: control.font.pointSize - 2
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        background:
        Rectangle
        {
            anchors.fill: parent
            color: menuDelegate.highlighted ?
                       currTheme.hilightColorComboBox :
                       currTheme.backgroundMainScreen
        }

        hoverEnabled: true
        highlighted: control.highlightedIndex === index
    }

    indicator:
    Image
    {
        id: canvas
        width: 24 * pt
        height: 24 * pt
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/icons/" + pathTheme + "/icon_arrow_down.png"
//        source: "qrc:/icon_arrow_down.png"
        sourceSize.width: 24 * pt
        rotation: control.popup.opened ? 180 : 0

        Behavior on rotation { NumberAnimation { duration: 200 } }
    }

    contentItem:
    Text
    {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
//        color: currTheme.textColorGray
        color: control.popup.visible? currTheme.textColorGray : currTheme.textColor
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background:
    Item
    {
        anchors.fill: parent

        Rectangle
        {
            id: backGrnd
            border.width: 0
            width: parent.width
            height: control.popup.visible ?
                        parent.height + popupBackGrnd.height :
                        parent.height
            y: control.popup.visible && control.popup.y < 0 ?
                 - popupBackGrnd.height :
                 0

            color: control.popup.visible ?
                       currTheme.backgroundMainScreen :
                       currTheme.backgroundElements
//            radius: 4
        }

        DropShadow
        {
            anchors.fill: backGrnd
            horizontalOffset: currTheme.hOffset
            verticalOffset: currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: backGrnd
            samples: 10
            cached: true
            visible: control.popup.visible
        }

        InnerShadow {
            anchors.fill: backGrnd
            horizontalOffset: 1
            verticalOffset: 1
            radius: 1
            samples: 10
            cached: true
            color: "#524D64"
            source: backGrnd
            spread: 0
            visible: control.popup.visible
        }
    }

    popup:
    Popup
    {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight + 3
            //+3 is needed to make ListView less moovable

        topPadding: 0
        bottomPadding: 0
        leftPadding: 1
        rightPadding:0

        contentItem:
            ListView
            {
                id: listView
                implicitHeight:
                    contentHeight < maximumPopupHeight ?
                        contentHeight : maximumPopupHeight

                clip: true

                model: control.popup.visible ?
                           control.delegateModel : null
                currentIndex: control.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

        background:
            Item{
                anchors.fill: parent

                Rectangle
                {
                    id: popupBackGrnd
                    anchors.fill: parent
                    color: currTheme.backgroundElements
                    border.width: 0
                }

                DropShadow
                {
                    anchors.fill: popupBackGrnd
                    horizontalOffset: currTheme.hOffset
                    verticalOffset: currTheme.vOffset
                    radius: currTheme.radiusShadow
                    color: currTheme.shadowColor
                    source: popupBackGrnd
                    samples: 10
                    cached: true
                }

                InnerShadow {
                    anchors.fill: popupBackGrnd
                    horizontalOffset: 1
                    verticalOffset: 0
                    radius: 1
                    samples: 10
                    cached: true
                    color: "#524D64"
                    source: popupBackGrnd
                }
            }
    }

    function getModelData(index, role)
    {
        var text = model.get(index)[role]

        if (text === undefined)
            return ""
        else
            return model.get(index)[role];
    }
}
