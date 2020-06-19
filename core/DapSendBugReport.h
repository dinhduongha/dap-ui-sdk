#ifndef DAPSENDBUGREPORT_H
#define DAPSENDBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class DapSendBugReport
{
public:
    DapSendBugReport();

    bool createBugReport();
    QString sendBugReport(QString email, QString message);
    bool runScriptPackaging(QString path);
    QString retLastModifyFile(QString path);
    //DapSession *m_session;
   // QNetworkReply * m_network_reply = Q_NULLPTR;

signals:
   // void sendingBugReportRequest(QString email, QString message);

};

#endif // DAPSENDBUGREPORT_H
