/* INCLUDES */

import QtQuick 2.12
import DapQmlStyle 1.0
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import StyleDebugTree 1.0
import TextEditContextMenu 1.0
import Scaling 1.0
import "qrc:/dapqml-widgets"

/****************************************//**
 * @brief Bug Report Form
 * @ingroup groupDapQmlForms
 *
 * ### Structure
 *
 * Form is built using three separate Item's which only
 * visible if desired mode is active.
 *
 * Button presses also change mode, so it is not necessary to call
 * setmode function from outside of the form.
 *
 * Form contains resizer item for advanced styling technic.
 *
 * @date 06.06.22
 * @author Mikhail Shilenko
 *******************************************/

Item {
    id: root

    /****************************************//**
     * @name DEFS
     ********************************************/
    /// @{

    enum Mode
    {
        Write,
        Loading,
        Result
    }

    /// @}
    /****************************************//**
     * @name VARS
     ********************************************/
    /// @{

    /// @brief current mode
    ///
    /// @see QuiBugReportForm.Mode
    property int mode: 0
    /// @brief form name
    ///
    /// Used to connect interface via Manager
    property string formName: "BugReport"

    /// @}
    /****************************************//**
     * @name SIGNALS
     ********************************************/
    /// @{

    /// @brief button send clicked (report input)
    signal sigSend()
    /// @brief button cancel clicked (report sending)
    signal sigCancel()
    /// @brief button back clicked (report success\\error)
    signal sigResultBack()

    /// @}
    /****************************************//**
     * @name FUNCTIONS
     ********************************************/
    /// @{

    /// @brief set form mode
    ///
    /// Three modes available:
    /// | name          | description |
    /// | ------------- | ----------- |
    /// | QuiBugReportForm.Mode.Write | report input screen |
    /// | QuiBugReportForm.Mode.Loading | report is sending (spinner) |
    /// | QuiBugReportForm.Mode.Result | report success\\error |
    function setmode(a_mode) {
        mode    = a_mode;
        switch(a_mode) {
        case QuiBugReportForm.Mode.Write:
        case QuiBugReportForm.Mode.Loading:
        case QuiBugReportForm.Mode.Result:
        }
    }

    /// @brief set report sending result text
    function setResultText(a_text) {
        bugrepResult.text   = a_text;
    }

    /// @}

    /****************************************//**
     * Resizers
     ********************************************/

    DapQmlRectangle {
        id: resizer
        qss: "bugrep-input-content"
    }

    /****************************************//**
     * Title
     ********************************************/

    DapQmlDialogTitle {
        text: "Bug report"
        qss: "dialog-title"
    }

    /****************************************//**
     * Frame renderer
     *
     * @note uncomment to generate proper image file
     *
     * @warning dont forget to comment this, after
     * image is rendered into file. you dont need this to render every time, only once during development
     ********************************************/

//    Component.onCompleted: {
//        /* update dimensions */
//        temporalRenderer.width  = 64*3;
//        temporalRenderer.height = 64*3;

//        /* save result to image */
//        temporalRenderer.grabToImage(function(result){
//            result.saveToFile("/tmp/temporalRenderer.png");  // "C:/temporalRenderer.png");
//        });

//        /* print warning */
//        console.log("BugReport image is rendered into file!"
//                    +" If you see this message that means renderer is exists"
//                    +" when it must not. It is only used to generate image source"
//                    +" during development. You should not see this message.");

//        /* hide this element */
//        temporalRenderer.visible = false;
//    }

//    Item {
//        id: temporalRenderer

//        property real sideSize:     64
//        property size size:         Qt.size (390, 200)
//        property size cornerSize:   Qt.size (sideSize, sideSize)
//        property real maxX:         390 - sideSize
//        property real maxY:         200 - sideSize
//        property real midWidth:     width - 64*2
//        property real midHeight:    height - 64*2

//        Image {
//            id: sideTop
//            x: temporalRenderer.x + parent.sideSize
//            y: temporalRenderer.y
//            width: temporalRenderer.width - cornerLeftTop.width - cornerRightTop.width;
//            height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(parent.sideSize, 0, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: sideLeft
//            x: temporalRenderer.x
//            y: temporalRenderer.y + parent.sideSize
//            width: parent.sideSize;
//            height: temporalRenderer.height - cornerLeftTop.height - cornerLeftBottom.height;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(0, parent.sideSize, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: sideBottom
//            x: temporalRenderer.x + parent.sideSize
//            y: temporalRenderer.y + temporalRenderer.height - height
//            width: temporalRenderer.width - cornerLeftBottom.width - cornerRightBottom.width;
//            height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(parent.sideSize, parent.maxY, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: sideRight
//            x: temporalRenderer.x + temporalRenderer.width - width
//            y: temporalRenderer.y + parent.sideSize
//            width: parent.sideSize;
//            height: temporalRenderer.height - cornerRightTop.height - cornerRightBottom.height;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(parent.maxX, parent.sideSize, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: midFill
//            x: temporalRenderer.x + cornerLeftTop.width
//            y: temporalRenderer.y + cornerLeftTop.height
//            width: temporalRenderer.width - cornerLeftTop.width - cornerRightTop.width;
//            height: temporalRenderer.height - cornerLeftTop.height - cornerLeftBottom.height;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(parent.sideSize, parent.sideSize, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: cornerLeftTop
//            x: temporalRenderer.x
//            y: temporalRenderer.y
//            width: parent.sideSize; height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(0, 0, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: cornerRightTop
//            x: temporalRenderer.width - parent.sideSize
//            y: temporalRenderer.y
//            width: parent.sideSize; height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(sourceSize.width - width, 0, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: cornerLeftBottom
//            x: temporalRenderer.x
//            y: temporalRenderer.height - parent.sideSize
//            width: parent.sideSize; height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(0, sourceSize.height - height, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }

//        Image {
//            id: cornerRightBottom
//            x: temporalRenderer.width - parent.sideSize
//            y: temporalRenderer.height - parent.sideSize
//            width: parent.sideSize; height: parent.sideSize;
//            sourceSize: parent.size
//            sourceClipRect: Qt.rect(sourceSize.width - width, sourceSize.height - height, parent.sideSize, parent.sideSize)
//            source: inputFiller.scaledPixmap; mipmap: true
//        }
//    }

    /****************************************//**
     * Mode Input Item
     ********************************************/

    Item {
        anchors.fill: parent
        visible: root.mode == 0

        /* text input */
        Item {
            id: input
            property real yy: (input.height - resizer.height) / 2
            DapQmlStyle { qss: "bugrep-input"; item: input }

            /* corner images rectangle */
            DapQmlRectangle {
                id: inputFiller
                visible: false
                anchors.fill: input
                color: "transparent"
                borderWidth: 2
                borderColor: "EEEEEE"
                qss: "bugrep-bg"
                property string scaledPixmap: ""
            }

            /* corner images */

            BorderImage {
                x: inputFiller.x
                y: inputFiller.y
                width: inputFiller.width
                height: inputFiller.height
                border { left: 64; top: 64; right: 64; bottom: 64 }
                horizontalTileMode: BorderImage.Stretch
                verticalTileMode: BorderImage.Stretch
                source: inputFiller.scaledPixmap
            }

            /* placeholder */
            DapQmlLabel {
                id: placeholder
                x: (input.width - width) / 2
                y: input.yy
                z: 2
                width: resizer.width
                height: resizer.height
                horizontalAlign: Text.AlignLeft
                verticalAlign: Text.AlignTop
                text: "Please describe the details of problem you faced. What actions did you take and what happened."
                qss: "bugrep-input-placeholder"
                wrapMode: TextEdit.Wrap
                visible: bugRepInputField.text.length == 0 && ((Scaling.isAndroid()) ? !bugRepInputField.activeFocus : true)

//                    Component.onCompleted: StyleDebugTree.describe (
//                       "placeholder",
//                        ["x", "y", "width", "height"],
//                       this);
            }

            /* input scrollarea */
            Flickable {
                id: bugRepInput
                x: (input.width - resizer.width) / 2
                y: input.yy
                z: 3
                clip: true
                contentWidth: width
                contentHeight: calcContentHeight()

                DapQmlStyle { item: bugRepInput; qss: "bugrep-input-content"; }

//                Component.onCompleted: StyleDebugTree.describe (
//                   "Flickable",
//                    ["x", "y", "width", "height"],
//                   this);

                function ensureVisible(r) {
                    if (contentX >= r.x)
                        contentX = r.x;
                    else if (contentX+width <= r.x+r.width)
                        contentX = r.x+r.width-width;
                    if (contentY >= r.y)
                        contentY = r.y;
                    else if (contentY+height <= r.y+r.height)
                        contentY = r.y+r.height-height;
                }

                function calcContentHeight() {
                    if (bugRepInputField.paintedHeight < inputFiller.height)
                        return inputFiller.height;
                    else
                        return bugRepInputField.paintedHeight;
                }

                /* input */
                TextEdit {
                    id: bugRepInputField
                    z: 4
                    objectName: "bugRepInputField"
                    anchors.fill: parent
                    wrapMode: TextEdit.Wrap
                    persistentSelection: true
                    selectByMouse: true
                    clip: true
                    font.pixelSize: fontSize
                    font.weight: fontWeight

                    property int fontSize: 16
                    property int fontWeight: Font.Normal
                    property int maximumLength: 200
                    property string previousText: text

                    DapQmlStyle { item: bugRepInputField; qss: "bugrep-input-textarea"; }

                    TextEditContextMenu {
                        id: ctxMenu
                        Component.onCompleted: {
                            setSerialInpoutMode(false);
                            setTextEditWidget(bugRepInputField);
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        cursorShape: Qt.IBeamCursor
                        onClicked: {
                            if (Scaling.isDesktop())
                                if (mouse.button === Qt.RightButton)
                                    contextMenu.open()
                        }
                        onPressAndHold: {
                            if (Scaling.isDesktop())
                                if (mouse.source === Qt.MouseEventNotSynthesized)
                                    contextMenu.open()
                        }
                        Menu {
                            id: contextMenu
                            MenuItem {
                                text: "Cut"
                                shortcut: "Ctrl+X"
                                onTriggered: ctxMenu.execCut();
                            }
                            MenuItem {
                                text: "Copy"
                                shortcut: "Ctrl+C"
                                onTriggered: ctxMenu.execCopy();
                            }
                            MenuItem {
                                text: "Paste"
                                shortcut: "Ctrl+V"
                                onTriggered: ctxMenu.execPaste();
                            }
                            MenuItem {
                                text: "Delete"
                                //shortcut: "Delete"
                                onTriggered: ctxMenu.execDelete();
                            }
                        }
                    }

                    onCursorRectangleChanged: bugRepInput.ensureVisible(cursorRectangle)

                    onTextChanged: {
                        if (text.length > maximumLength) {
                            var cursor = cursorPosition;
                            text = previousText;
                            if (cursor > text.length) {
                                cursorPosition = text.length;
                            } else {
                                cursorPosition = cursor-1;
                            }
                        }
                        previousText = text

                        letterAmount.text = text.length + "/200"
                    }
                }
            }
        }

//        /* text edit */
//        Item {
//            id: inputOld
//            visible: false

//            property real yy: (input.height - resizer.height) / 2

//            DapQmlStyle { qss: "bugrep-input"; item: input }

//            /* background image */
//            DapQmlLabel {
//                id: inputbg
//                z: 1
//                anchors.fill: input
//                qss: "bugrep-bg"

//                /* placeholder */
//                DapQmlLabel {
//                    id: placeholder
//                    x: (input.width - width) / 2
//                    y: input.yy
//                    z: 2
//                    width: resizer.width
//                    height: resizer.height
//                    horizontalAlign: Text.AlignLeft
//                    verticalAlign: Text.AlignTop
//                    text: "Please describe the details of problem you faced. What actions did you take and what happened."
//                    qss: "bugrep-input-placeholder"
//                    wrapMode: TextEdit.Wrap
//                    visible: bugRepInputField.text.length == 0

////                    Component.onCompleted: StyleDebugTree.describe (
////                       "placeholder",
////                        ["x", "y", "width", "height"],
////                       this);
//                }
//            }

//            /* input scrollarea */
//            Flickable {
//                id: bugRepInput
//                x: (input.width - resizer.width) / 2
//                y: input.yy
//                z: 3
//                clip: true
//                contentWidth: width
//                contentHeight: calcContentHeight()

//                DapQmlStyle { item: bugRepInput; qss: "bugrep-input-content"; }

////                Component.onCompleted: StyleDebugTree.describe (
////                   "Flickable",
////                    ["x", "y", "width", "height"],
////                   this);

//                function ensureVisible(r) {
//                    if (contentX >= r.x)
//                        contentX = r.x;
//                    else if (contentX+width <= r.x+r.width)
//                        contentX = r.x+r.width-width;
//                    if (contentY >= r.y)
//                        contentY = r.y;
//                    else if (contentY+height <= r.y+r.height)
//                        contentY = r.y+r.height-height;
//                }

//                function calcContentHeight() {
//                    if (bugRepInputField.paintedHeight < inputbg.height)
//                        return inputbg.height;
//                    else
//                        return bugRepInputField.paintedHeight;
//                }

//                /* input */
//                TextEdit {
//                    id: bugRepInputField
//                    z: 4
//                    objectName: "bugRepInputField"
//                    anchors.fill: parent
//                    wrapMode: TextEdit.Wrap
//                    clip: true
//                    font.pixelSize: fontSize
//                    font.weight: fontWeight

//                    property int fontSize: 16
//                    property int fontWeight: Font.Normal
//                    property int maximumLength: 200
//                    property string previousText: text

//                    DapQmlStyle { item: bugRepInputField; qss: "bugrep-input-textarea"; }

//                    onCursorRectangleChanged: bugRepInput.ensureVisible(cursorRectangle)

//                    onTextChanged: {
//                        if (text.length > maximumLength) {
//                            var cursor = cursorPosition;
//                            text = previousText;
//                            if (cursor > text.length) {
//                                cursorPosition = text.length;
//                            } else {
//                                cursorPosition = cursor-1;
//                            }
//                        }
//                        previousText = text

//                        letterAmount.text = text.length + "/200"
//                    }
//                }
//            }
//        }

        /* letter counter */
        DapQmlLabel {
            id: letterAmount
            qss: "bugrep-letter-counter"
            text: "0/200"
            color: "#A4A3C0"
            horizontalAlign: Text.AlignRight
        }

        /* attach */
        DapQmlLabel {
            id: attach
            qss: "bugrep-attach-btn"
            text: "Click here to attach a screenshot"
            color: "#DA0B82"
            visible: false

            MouseArea {
                id: mouseArea
                anchors.fill: attach
                cursorShape: Qt.PointingHandCursor
                enabled: false
            }
        }

        /* send button */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "SEND REPORT"
            onClicked: { root.mode = 1; root.sigSend(); }
            enabled: bugRepInputField.length >= 3
            opacity: 0.4 + 0.6 * enabled
        }
    }

    /****************************************//**
     * Mode Sending Item
     ********************************************/

    Item {
        anchors.fill: parent
        visible: root.mode == 1

        /* info */
        DapQmlLabel {
            qss: "bugrep-sending"
            text: "Sending..."
        }

        /* animated spinner */
        AnimatedImage {
            id: animation
            source: "qrc:/dapqml-forms-asset/Spinner.gif"
            DapQmlStyle { qss: "bugrep-animation"; item: animation }
        }

        /* cancel */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "CANCEL"
            onClicked: { root.mode = 0; root.sigCancel(); }
        }
    }

    /****************************************//**
     * Mode Status Item
     ********************************************/

    Item {
        anchors.fill: parent
        visible: root.mode == 2

        /* status text */
        DapQmlLabel {
            id: bugrepResult
            qss: "bugrep-status"
            wrapMode: Text.WordWrap
            text: "Bug report # 368647 sent successfully"
        }

        /* back */
        DapQmlPushButton {
            qss: "bugrep-send-btn push-button"
            text: "BACK"
            onClicked: { root.mode = 0; root.sigResultBack(); }
        }
    }
}

/*-----------------------------------------*/
