#include <QtDebug>
#include <QSettings>
#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif
#include "ServiceCtl.h"
#include "dap_common.h"
#include "registry.h"


ServiceCtl::ServiceCtl(DapJsonCmdController* controller, QObject *parent)
    : DapServiceClient("DAP_SERVICE_NAME", parent), m_controller(controller)
{
    Q_ASSERT(controller);

    connect(m_controller, &DapJsonCmdController::sendDapCmd,
            this, &DapServiceClient::sendCmd);

    connect(this,&ServiceCtl::ctlConnected, [=]{
        qInfo() << "[ServiceCtl] Connected to ctl socket,request for status";
        bServiceIsOn = true;
    });
    
    connect(this,&ServiceCtl::ctlDisconnected, [=]{

        qInfo() << "[ServiceCtl] Disconnected from backend";
        bServiceIsOn = false;
        startReconnectingToService();
        startService();
    });
}

bool ServiceCtl::startService(){
    qDebug() << "[ServiceCtl] startService()";
    for (int i; i < 2; i++){
#ifdef Q_OS_WIN
        int ret = exec_silent("sc start " DAP_BRAND "Service");
#else
        int ret = ::system("systemctl start " DAP_BRAND "Service");
#endif
        if (!ret) {
            qDebug() << "[ServiceCtl] Start " DAP_BRAND "Service";
            bServiceIsOn = true;
            serviceRestartCounter++;
            return true;
        } else {
            qCritical() << "[ServiceCtl] " DAP_BRAND "Service not starting";
        }
    }
    return false;
}

void ServiceCtl::procCmdController(const QByteArray &a_cmd)
{
    m_controller->handleCmd(a_cmd);
}
