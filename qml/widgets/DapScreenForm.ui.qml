//****************************************************************************
//                      Implements a screen widget.
//****************************************************************************

import QtQuick 2.4
import QtQuick.Controls 2.0

Rectangle 
{
    id: frameScreen

    ///@detalis Frame widget.
    property alias dapFrame: frameScreen
    ///@detalis Screen components.
    property Item dapContenetItemScreen

    anchors.fill: parent

    // Install screen components
    Item 
    {
        id: contenetItemScreen
        data: dapContenetItemScreen
        anchors.fill: parent
    }
}
