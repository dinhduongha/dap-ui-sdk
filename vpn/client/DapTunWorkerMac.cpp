#include "DapTunWorkerMac.h"

#include <fcntl.h>
#include <errno.h>
#include <QDebug>
#include <unistd.h>
#include <netinet/ip.h>

#include <DapTunAbstract.h>

/**
 * @brief DapTunWorkerMac::DapTunWorkerMac
 */
DapTunWorkerMac::DapTunWorkerMac(DapTunAbstract *a_tun)
    :DapTunWorkerAbstract(a_tun)

{
    m_breakerSet[0] = m_breakerSet[1] = -1;
    if (::pipe(m_breakerSet) < 0){
        qCritical() <<"Can't create unix named pipe!";
        throw QString("Can't create unix named pipe!");
    }
}

/**
 * @brief DapTunWorkerMac::loop
 */
void DapTunWorkerMac::loop()
{
    size_t l_bufSize = 0;
    qDebug() << "[DapTunWorkerMac] loop() start with MTU = " << DAP_IP_MTU;

    fd_set fds_read, fds_read_active;
    fd_set fds_write, fds_write_active;
    size_t pktOutPos=0;
    Dap::Stream::Packet * pktOut = nullptr;
    FD_ZERO (&fds_read);
    FD_ZERO (&fds_write);
    FD_SET (tunSocket(), &fds_read);
    FD_SET (tunSocket(), &fds_write);
    int select_breaker = breaker(0);
    FD_SET ( select_breaker,&fds_read);
    do{
        fds_read_active=fds_read;
        fds_write_active = fds_write;

        if (pktOut == nullptr) {  // if no non-sent data
            pktOut = writeDequeue();
        }
        int ret = 0;
        if (pktOut)
            ret = ::select(FD_SETSIZE,&fds_read_active,&fds_write_active,NULL,NULL);
        else
            ret = ::select(FD_SETSIZE,&fds_read_active,NULL,NULL,NULL) ;
        //
        if(ret > 0) {
            if (FD_ISSET (tunSocket(), &fds_read_active)){
               // qDebug() << "Tun socket is ready for read() ";
                int readRet = ::read(tunSocket(), m_tunBuf + l_bufSize, DAP_IP_MTU - l_bufSize);

                // qDebug() << "Read "<< readRet<<" bytes";
                if (readRet < 0) {
                    emit loopError(QString("read() returned %1 (%2)").arg(readRet).arg(::strerror(errno)));
                    qCritical() << "Read returned "<< readRet;
                    qDebug() << QString("read() returned %1 (%2)").arg(readRet).arg(::strerror(errno));
                    break;
                }
                l_bufSize += readRet;

                if (l_bufSize > (int)sizeof(struct ip)) {
                    emit bytesWrite(l_bufSize);
                    procDataFromTun(m_tunBuf, l_bufSize); // Pack data with SF headers and emit it out to DapStreamer
                    l_bufSize = 0;
                }

            }else if( (pktOut) && (FD_ISSET (tunSocket(), &fds_write_active))){ // Event to Tun socket
                int writeRet = ::write(tunSocket(), pktOut->data+pktOutPos,pktOut->header.op_data.data_size - pktOutPos );

                if (writeRet < 0) {
                    emit loopError(QString("write() returned %1 (%2)").arg(writeRet).arg(::strerror(errno)));
                    qCritical() << "Write returned "<< writeRet;
                    break;
                }
                pktOutPos += writeRet;
                if(pktOutPos>= pktOut->header.op_data.data_size ){ // Packet is sent into the tunnel
                    emit bytesRead(pktOutPos);
                    delete pktOut;
                    pktOut = nullptr;
                    pktOutPos = 0;
                }
            } else if(FD_ISSET (select_breaker, &fds_read_active)) { // Event to stop the socket
                char ctlCode=0;
                ::read(select_breaker, &ctlCode,1 );
                if( ctlCode == 0 ){ // Other variants - just waked up to write smth out
                    qInfo() << "Got the signal to stop the loop";
                    break;
                }
            }
            else {
                emit loopError(QString("Select has no tun handler in the returned set"));
                qCritical() << "[DapTunWorkerMac] listenTunnelThread() select has no tun handler in the returned set";
                break;
            }
        }else {
            qCritical() << "[DapTunWorkerMac] loop() select() returned "<< ret;
            emit loopError(QString("select() returned %1 (%2)").arg(ret).arg(::strerror(errno)));
            break;
        }

    }while(1);

    qDebug() << "[DapTunWorkerMac] Listen thread finished!";
    //emit loopStopped();
}
