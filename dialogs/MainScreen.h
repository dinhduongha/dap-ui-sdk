#ifndef MAINSCREEN_H
#define MAINSCREEN_H


#include <QStateMachine>

#include "AdaptiveScreen.h"

#include "vpnDefine.h"


#include "defines.h"
#include "DapDataLocal.h"
#include "StyledDropShadowEffect.h"

#ifdef Q_OS_ANDROID
#include "ui_MainScreen.h"
#include <QFrame>

#else
#include "ui_MainScreen.h"

#include <QTimer>
#include "CustomComboBoxPopup.h"
#include <QComboBox>
#endif

class MainScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public:

    void setState(ConnectionStates a_state);


protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

    const QString BTN_CONNECTION    = "btnConnection";

    const QString CHB_AUTHORIZED      = "chbAuthorized";
    const QString CHB_STREAM_OPENED   = "chbStreamOpened";
    const QString CHB_VIRTUAL_NETWORK = "chbVirtualNetwork";

#ifdef Q_OS_ANDROID
    const QString FRM_CONNECT       = "frmConnect";
    const QString FRM_INFO          = "frmInfo";
    const QString FRM_STATUS        = "frmStatus";
    const QString BTN_CHANGE_SERVER = "btnChangeServer";
    const QString LBL_ACTUAL_SERVER = "lblActualServer";
    const QString LBL_LOGIN_TIME    = "lblLoginTime";
    const QString LBL_TIME_CONNECT  = "lblTimeConnect";
    const QString LBL_PACKETS_REC   = "lblPacketsRec";
    const QString LBL_PACKETS_SENT  = "lblPacetsSent";
#else
    const QString LBL_STATUS_MESSAGE        = "lblStatusMessage";
    const QString CBB_SERVER                = "cbbServer";
    const QString BTN_BYTES                 = "btnBytes";
    const QString BTN_PACKETS               = "btnPackets";
    const QString LBL_BYTES_PACKETS_CAPTION = "lblBytesPacketsCaption";
    const QString LBL_DOWNLOAD_SPEED        = "lblDownloadSpeed";
    const QString LBL_TIME_CONNECTED        = "lblTimeConnected";
    const QString LBL_DOWNLOAD              = "lblDownload";
    const QString LBL_PATES_PACKETS         = "lblBytesPackets";
#endif

signals:
    void disconnect();
    void serverChanged(const QString& serverName);

public slots:

    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);

private:
    static QString statusText(ConnectionStates a_state);

};

#endif // MAINSCREEN_H
