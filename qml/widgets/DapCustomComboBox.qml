import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

Item
{
    id: mainItem

    implicitHeight: 45

    property int maximumPopupHeight: 200
    property int padding: 15
    property int spacing: 15

    property alias model: popupListView.model

    property int currentIndex: -1
    property string currentText: displayText
    property int count: popupListView.model.count

    property bool popupVisible: false

    property font font: mainFont.dapFont.medium14

    property string mainTextRole: "name"
    property string secondTextRole: "secondname"

    property string defaultText: qsTr("Undefined")

    property string displayText: defaultText

    property color backgroundColor: currTheme.backgroundElements

    onModelChanged:
    {
        print("DapCustomComboBox", "onModelChanged",
              "popupListView.currentIndex", popupListView.currentIndex)

        if (popupListView.currentIndex < 0)
            displayText = getModelData(0, mainTextRole)
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole)
    }

    onCountChanged:
    {
        print("DapCustomComboBox", "onCountChanged",
              "popupListView.currentIndex", popupListView.currentIndex)

        if (popupListView.currentIndex < 0)
            displayText = getModelData(0, mainTextRole)
        else
            displayText = getModelData(popupListView.currentIndex, mainTextRole)
    }

    Rectangle
    {
        id: background
        border.width: 0
        anchors.fill: parent

        color: popupVisible ?
                   currTheme.backgroundMainScreen :
                   backgroundColor

        Text
        {
            id: mainTextItem
            x: 0
            y: 0
            padding: mainItem.padding
            width: mainItem.width - indicator.width - mainItem.padding
            height: mainItem.height

            text: mainItem.displayText
            font: mainItem.font
            color: popupVisible ?
                       currTheme.textColorGray : currTheme.textColor
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Image
        {
            id: indicator
            width: 24
            height: 24
            x: mainItem.width - width - padding
            y: (mainItem.height - height)*0.5

            fillMode: Image.PreserveAspectFit
            source: "qrc:/Resources/" + pathTheme + "/icons/other/icon_arrow_down.png"
//            source: "qrc:/icon_arrow_down.png"
            sourceSize.width: 24
            rotation: popupVisible ? 180 : 0

            Behavior on rotation
            {
                NumberAnimation
                {
                    duration: 200
                }
            }
        }
    }

    DropShadow
    {
        visible: popupVisible
        anchors.fill: background
        horizontalOffset: currTheme.hOffset
        verticalOffset: currTheme.vOffset
        radius: currTheme.radiusShadow
        color: currTheme.shadowColor
        source: background
        samples: 10
        cached: true
    }

    InnerShadow
    {
        visible: popupVisible
        anchors.fill: background
        horizontalOffset: 1
        verticalOffset: 1
        radius: 1
        samples: 10
        cached: true
        color: "#524D64"
        source: background
        spread: 0
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed:
        {
            popupVisible = !popupVisible

            popup.visible = popupVisible

//            print("DapCustomComboBox", "mouseArea",
//                  "popupVisible", popupVisible)

            if (popupVisible)
            {
                popupListView.positionViewAtIndex(
                            currentIndex, ListView.Contain)
            }
        }
    }

    Popup
    {
        id: popup

//        visible: popupVisible

        scale: mainWindow.scale

//        x: 0
//        y: mainItem.height

        x: -width*(1/scale-1)*0.5
        y: mainItem.height - height*(1/scale-1)*0.5

        width: popupBackground.width
        height: popupBackground.height

        padding: 0

        onVisibleChanged:
        {
//            print("DapCustomComboBox", "onVisibleChanged",
//                  "visible", visible,
//                  "popupVisible", popupVisible)

            if (!mouseArea.containsMouse &&
                visible === false && popupVisible === true)
                popupVisible = false
        }

        Rectangle
        {
            id: popupBackground

            width: mainItem.width
            height: popupListView.height + border.width*2

            color: currTheme.backgroundElements

            border.width: 1
            border.color: currTheme.backgroundElements

            ListView
            {
                id: popupListView

//                visible: popupVisible

                x: popupBackground.border.width
                y: popupBackground.border.width
                width: mainItem.width - popupBackground.border.width*2
                implicitHeight:
                    contentHeight < maximumPopupHeight ?
                        contentHeight : maximumPopupHeight

                clip: true

                ScrollIndicator.vertical:
                    ScrollIndicator { }

                delegate:
                ItemDelegate
                {
                    id: menuDelegate
                    width: mainItem.width

                    contentItem:
                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.leftMargin: parent.leftPadding
                        anchors.rightMargin: 20

                        Text
                        {
                            Layout.fillWidth: true
                            text: getModelData(index, mainTextRole)
                            color: menuDelegate.highlighted ?
                                       currTheme.hilightTextColorComboBox :
                                       currTheme.textColor
                            font: mainItem.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        Text
                        {
                            text: getModelData(index, secondTextRole)
                            color: menuDelegate.highlighted ?
                                       currTheme.hilightTextColorComboBox :
                                       currTheme.textColor
        //                    font: mainItem.font
                            font.family: mainItem.font.family
                            font.pointSize: mainItem.font.pointSize - 3
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

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            popupListView.currentIndex = index
                            popup.visible = false
//                            popupVisible = false
                        }
                    }

                    hoverEnabled: true
                    highlighted: popupListView.currentIndex === index
                }

                onCurrentIndexChanged:
                {
                    displayText = getModelData(currentIndex, mainTextRole)
                    mainItem.currentIndex = currentIndex
                }

            }

        }

        DropShadow
        {
            visible: popupVisible
            anchors.fill: popupBackground
            horizontalOffset: currTheme.hOffset
            verticalOffset: currTheme.vOffset
            radius: currTheme.radiusShadow
            color: currTheme.shadowColor
            source: popupBackground
            samples: 10
            cached: true
        }

        InnerShadow {
            visible: popupVisible
            anchors.fill: popupBackground
            horizontalOffset: 1
            verticalOffset: 0
            radius: 1
            samples: 10
            cached: true
            color: "#524D64"
            source: popupBackground
        }
    }

    function getModelData(index, role)
    {
        var text = model.get(index)[role]

        if (text === undefined)
            return ""
        else
            return text;
    }
}
