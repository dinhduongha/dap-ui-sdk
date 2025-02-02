#include <QtDebug>
#include <QProcess>
#include <DapUtils.h>
#include <unistd.h>

#include "DapTunWorkerUnix.h"
#include "DapTunUnixAbstract.h"

DapTunUnixAbstract::DapTunUnixAbstract()
    :DapTunAbstract()
{
    qDebug() << "[DapTunUnixAbstract::DapTunUnixAbstract]";
    tunThread = new QThread();
    tunWorker = (tunWorkerUnix=new DapTunWorkerUnix(this));
    breaker0 = tunWorkerUnix->breaker(0);
    breaker1 = tunWorkerUnix->breaker(1);
    connect(this, &DapTunAbstract::sigStartWorkerLoop, tunWorker, &DapTunWorkerAbstract::loop);
    initWorker();

}

void DapTunUnixAbstract::workerStart() {
    qInfo() <<"Tun device created" << m_tunSocket;
    if (m_tunSocket > 0){
        tunWorker->setTunSocket(m_tunSocket);
        tunWorker->moveToThread(tunThread);
        tunThread->start();
        emit sigStartWorkerLoop();
        qInfo() << "tunThread started, tun socket: " << m_tunSocket;
    } else {
        qInfo() << "Tun socket " << m_tunSocket << " already opened";
        return;
    }
    onWorkerStarted();
}

/**
 * @brief DapTunUnix::workerStop
 * @details Sends request to worker loop to stop
 * @return
 */
void DapTunUnixAbstract::workerStop()
{
    //char buffer[1];
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }/*else{
       // nanosleep(&DapTunWorkerUnix::BREAK_LATENCY, NULL);
        qDebug() << "Write in breaker " << breaker0;
        if (::read(breaker0, buffer, 1) <= 0 || buffer[0] != '\0'){
            qCritical() <<"Can't read to the breaker's pipe!";
            return;
        }
    }*/
    tunThread->quit();
    tunThread->wait();
    onWorkerStopped();
}

void DapTunUnixAbstract::workerPause()
{
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
    tunThread->quit();
    tunThread->wait();
}

/**
 * @brief DapTunUnixAbstract::signalWriteQueueProc
 */
void DapTunUnixAbstract::signalWriteQueueProc()
{
    if ( ::write( breaker1, "w", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
    }
}


/**
 * @brief DapTunUnix::tunDeviceDestroy
 */
void DapTunUnixAbstract::tunDeviceDestroy()
{
    qInfo() << "Close tun device (and usualy destroy that after)";
    if(m_tunSocket>0){
        ::close(m_tunSocket);
    }
    m_tunSocket = -1;
    emit destroyed();
}

/**
 * @brief DapTunUnix::workerPrepare
 */
void DapTunUnixAbstract::workerPrepare()
{
    qInfo() <<"Prepare worker before the work";
}

void DapTunUnixAbstract::addNewUpstreamRoute(const QString &a_dest) {
    QString run = QString("route add -host %2 gw %1 metric 10")
            .arg(m_defaultGwOld).arg(qPrintable(a_dest));
    qDebug() << "Execute "<<run;
    ::system(run.toLatin1().constData());
}

