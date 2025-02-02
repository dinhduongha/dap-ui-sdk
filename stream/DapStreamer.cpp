/*
 Copyright (c) 2017-2018 (c) Project "DeM Labs Inc" https://github.com/demlabsinc
  All rights reserved.

 This file is part of DAP (Deus Applications Prototypes) the open source project

    DAP (Deus Applicaions Prototypes) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DapStreamer.h"
#include <QTcpSocket>
#include <QSysInfo>
#include <QTimer>
#include <QNetworkProxy>
#include <stdlib.h>

constexpr quint8 daSig[] = {0xa0,0x95,0x96,0xa9,0x9e,0x5c,0xfb,0xfa};
QByteArray daSigQ((const char*) daSig, sizeof(daSig));

inline static int daSigDetect(const QByteArray& b) { return b.indexOf(daSigQ); }


QHash<char, DapChBase*> DapStreamer::m_dsb;

DapStreamer::DapStreamer(DapSession * session, QObject* parent) :
    QObject(parent),m_pktOutLastSeqID(0), m_streamSocket(this), m_streamState(SSS_NONE), m_isStreamOpened(false)
{
    qDebug() << "[DapConnectStream::DapConnectStream]";

    m_procPktInData.reserve(DAP_PKT_SIZE_MAX);
    m_procPktInDecData.reserve(DAP_PKT_SIZE_MAX);

    m_session = session;
    m_streamSocket.setReadBufferSize(DAP_PKT_SIZE_MAX);
    m_streamSocket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_streamSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_streamSocket.setProxy(QNetworkProxy::NoProxy);

    connect(&m_streamSocket,&QIODevice::readyRead, this,&DapStreamer::sltStreamProcess);
    //connect(m_streamSocket,&QAbstractSocket::hostFound, this,&DapConnectStream::sltStreamHostFound);
    connect(&m_streamSocket,&QAbstractSocket::connected, this,&DapStreamer::sltStreamConnected);
    connect(&m_streamSocket,&QAbstractSocket::disconnected, this,&DapStreamer::sltStreamDisconnected);
    connect(&m_streamSocket,SIGNAL(bytesWritten(qint64)), this, SLOT(sltStreamBytesWritten(qint64)) );

   typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);

   connect(&m_streamSocket,static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
           this, &DapStreamer::sltStreamError);

}

DapStreamer::~DapStreamer()
{
}

void DapStreamer::writeChannelPacket(DapChannelPacketHdr *a_pktHdr, void *data, uint64_t *dest_addr)
{

    if(a_pktHdr->size + sizeof (DapChannelPacketHdr) > DAP_PKT_SIZE_MAX) {
        qWarning() << "Too large package size " << a_pktHdr->size;
        return;
    }
    Q_UNUSED(dest_addr)

    DapPacketHdr* pktOut = (DapPacketHdr* ) m_writeEncDataOut;
    size_t pktOutDataSize = sizeof(DapPacketHdr);

    if (a_pktHdr->type == CH_KEEPALIVE_PKT) {

        pktOut->type = KEEPALIVE_PACKET;
        pktOut->size = 0;

    } else {

        pktOut->type = DATA_PACKET;

        a_pktHdr->seq_id = m_pktOutLastSeqID++;

        size_t dOutSize = a_pktHdr->size + sizeof(DapChannelPacketHdr);

        memcpy(m_writeDataOut, a_pktHdr, sizeof(DapChannelPacketHdr));
        memcpy(m_writeDataOut + sizeof(DapChannelPacketHdr), data, a_pktHdr->size);

        QByteArray dOutEnc, dOutRaw(m_writeDataOut, dOutSize);

        m_session->getDapCrypt()->encode(dOutRaw, dOutEnc, KeyRoleStream);

        pktOutDataSize += dOutEnc.size();

        pktOut->size = dOutEnc.size();

        memcpy(m_writeEncDataOut + sizeof(DapPacketHdr), dOutEnc, dOutEnc.size());
    }
    memcpy(pktOut->sig, daSig, sizeof(pktOut->sig));

    writeStreamRaw(m_writeEncDataOut, pktOutDataSize);

    ::free (data);
    delete a_pktHdr;
}


void DapStreamer::streamOpen(const QString& subUrl, const QString& query)
{
    if(m_streamSocket.isOpen()) {
        qWarning() << "Stream socket already open. "
                      "Closing current open socket";
        streamClose();
    }
    qDebug() << "Stream open SubUrl = " << subUrl;
    qDebug() << "Stream open query =" << query;
    m_streamID.clear();
    m_network_reply = m_session->streamOpenRequest(subUrl, query, this, SLOT(sltStreamOpenCallback()), QT_STRINGIFY(errorNetwork));
}

void DapStreamer::streamClose()
{
    emit streamDisconnecting();
    if(m_streamSocket.isOpen()){
        qDebug() <<"[SC] close the stream";
        m_streamSocket.close();
    }
    emit streamClosed();
    m_pktOutLastSeqID = 0;
    m_isStreamOpened=false;
}

qint64 DapStreamer::writeStreamRaw(const void * data, size_t data_size)
{
    quint64 outWriteSize, wr, ret = 0;
    if(m_streamSocket.isWritable())
    {
        while (ret < (quint64) data_size)
        {
            outWriteSize = ((quint64) data_size) - ret;
            wr = m_streamSocket.write((const char*)data + ret, outWriteSize);

            if(wr > 0)
                ret += wr;
            else
            {
                if(m_isStreamOpened)
                    return 0;
                qDebug() << "[DapConnectStream] Wr = " << wr << " stream closed!";
                break;
                //emit streamClosed(); ///todo in pervois version DAP this string be uncommented
            }
        }
        return ret;
    }
    else
    {
        //qWarning() << "Stream Socket is not writable";
        if(!m_streamSocket.isOpen()) {
            //qWarning() << "Stream socket is closed";
            emit streamClosed();
        }
        return 0;
    }
}

void DapStreamer::sltStreamOpenCallback()
{
    qDebug() << "Opening stream";

    if(m_network_reply->getReplyData().size() == 0) {
        qWarning() << "Reply is empty";
        emit sigStreamOpenBadResponseError();
        return;
    }

    QByteArray streamReplyDec;
    m_session->getDapCrypt()->decode(m_network_reply->getReplyData(), streamReplyDec, KeyRoleSession);
    QString streamReplyStr(streamReplyDec);

    QStringList str_list = streamReplyStr.split(" ");

    if(str_list.length() < 2)
    {
        qWarning() << "Bad response. Wrong Reply Format!" << streamReplyStr;
        emit sigStreamOpenBadResponseError();
        return;
    }

    m_streamID = str_list.at(0);

    QString streamServKey = str_list.at(1);

    if(!m_streamID.isEmpty()) {
        qDebug() << "Stream id:" << m_streamID;
        qDebug()  << "[DapConnectStream] Stream server key for client requests: "
                  << streamServKey;

        m_session->getDapCrypt()->initKey(DAP_ENC_KEY_TYPE_SALSA2012, streamServKey, KeyRoleStream);

        if(!m_streamSocket.isOpen()) {

            m_streamSocket.connectToHost(m_session->upstreamAddress(),
                                          m_session->upstreamPort(),
                                          QIODevice::ReadWrite);
//#ifndef Q_OS_WINDOWS
            if (m_streamSocket.waitForConnected(15000)) {
                return;
            } else {
                emit errorNetwork (tr ("Socket connection timeout"));
            }
//#endif
        } else {
            qCritical() << "Stream already open";
        }

    } else {
        qWarning() << "Can't open stream " << m_streamID;
        emit sigStreamOpenBadResponseError();
        m_streamID.clear();
    }
}

void DapStreamer::sltStreamBytesWritten(qint64 bytes)
{
    (void) bytes;
}

void DapStreamer::sltStreamError(QAbstractSocket::SocketError socketError)
{

    qWarning() << "Socket error: " <<m_streamSocket.errorString();
    switch (socketError) {
        case QAbstractSocket::NetworkError:
            emit errorNetwork( tr("Networking error, pls fix your network configuration"));
        break;
        case QAbstractSocket::SocketAccessError:
            m_isStreamOpened=false;
            emit errorNetwork( tr("Socket access error, has no privileges for socket operations"));
        break;
        case QAbstractSocket::DatagramTooLargeError:
            emit errorNetwork( tr("Too large datagram you're trying to send through the socket (more than 8192 bytes)"));
        break;
        case QAbstractSocket::UnsupportedSocketOperationError:
            emit errorNetwork( tr("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support)"));
        break;
        case QAbstractSocket::ProxyAuthenticationRequiredError:
            m_isStreamOpened=false;
            emit errorNetwork( tr("The socket is using a proxy, and the proxy requires authentication"));
        break;
        case QAbstractSocket::UnfinishedSocketOperationError:
            emit errorNetwork( tr("The last operation attempted has not finished yet (still in progress in the background)"));
        break;
        case QAbstractSocket::ProxyConnectionRefusedError:
            emit errorNetwork( tr("Could not contact the proxy server because the connection to that server was denied"));
        break;
        case QAbstractSocket::HostNotFoundError:
            emit errorNetwork( tr("The host %1 was not found. Please check the "
                           "host name and port %2 settings.")
                 .arg(m_session->upstreamAddress())
                 .arg(m_session->upstreamPort()));
        break;
        case QAbstractSocket::ConnectionRefusedError:
            emit errorNetwork( tr("The connection was refused by the peer. "
                           "Make sure the %1 is running, "
                           "and check that the host name and port %2 "
                           "is open")
                 .arg(m_session->upstreamAddress())
                 .arg(m_session->upstreamPort()));

            break;
        default:{
            emit errorNetwork(tr("The following error occurred: %1")
                       .arg(m_streamSocket.errorString()));

        }
    }
}

void DapStreamer::sltStreamDisconnected()
{
    qDebug() << "[DapConnectStream] sltStreamDisconnected";
    emit streamClosed();
}

void DapStreamer::sltStreamConnected()
{
    qDebug() << "[DapConnectStream] Connected to the server";
    //m_isStreamOpened=true;
    m_buf.clear();

    QString str_url = QString("/%1/fjskd9234j?fj913htmdgaq-d9hf=%2")
            .arg(DapSession::URL_STREAM).arg(m_streamID);

    qDebug() << "[DapConnectStream] Stream URL: " << str_url;

    // Use QNetworkRequest or DapConnectClient::request
    QString str_request = QString(
                          "POST %2 HTTP/1.1\r\n"
                          "Connection: Keep-Alive\r\n"
                          "Cookie: %1\r\n"
                          "User-Agent: DapClient %4 \r\n"
                          "Host: %3\r\n"
                          "\r\n")
                            .arg(m_session->cookie())
                            .arg(str_url).arg(m_session->upstreamAddress())
                            .arg(2); // Do something with versioning

    qDebug() << "[DapConnectStream] Request on out : " << str_request;
    m_isStreamOpened = false;
    QByteArray baReq( str_request.toLatin1() );
    qint64 ret = m_streamSocket.write(baReq.constData(), baReq.size());
    m_streamState = SSS_FRAME_SEARCH;
    if (m_streamSocket.flush()) {
        qDebug() << "[DapConnectStream] HTTP stream request sent "<< ret<< " bytes";
        emit streamOpened();
    } else {
        qCritical() << "Stream not opened";
        emit errorNetwork(m_streamSocket.errorString());
    }

    /*if( !m_streamSocket->waitForBytesWritten(
#ifndef Q_OS_WINDOWS
                10000
#else
#endif
                )) {
        qDebug() << "[DapConnectStream] Can't wait until all bytes are sent: "
                 << m_streamSocket->errorString();
    }*/
}


void DapStreamer::sltStreamProcess()
{
   // qDebug()<< "[DapConnectStream::sltStreamProcess]";
    if( m_streamSocket.bytesAvailable() == 0 )
    {
        qWarning() << "[DapConnectStream] No data after wait!";
        return;
    }

    if( m_streamSocket.bytesAvailable() < 0 )
    {
        qWarning() << "[DapConnectStream] No data";
        return;
    }

    QByteArray btAr = m_streamSocket.readAll();
    int bufferLengthBefore = m_buf.length();
    m_buf.append(btAr);
    bool cycle_flag = true;

    while (cycle_flag) {

     switch (m_streamState) {

        case SSS_HTTP_HEADER:
        {
            cycle_flag = false;
            qDebug() << "[DapConnectStream] case HTTP_HEADER";
            char str_hdr_end[] = { 0x0D,0x0A,0x0D,0x0A };
            int header_end = m_buf.indexOf(str_hdr_end);
            if(header_end != -1)
            {
                header_end += strlen(str_hdr_end);
                header_end -= bufferLengthBefore;
                qDebug() << "[DapConnectStream] HTTP header end = " << header_end;
                m_streamState = SSS_FRAME_SEARCH;
            }
            else
                qDebug() << "[DapConnectStream] HTTP header ends in the next frame";
        } break;

        case SSS_FRAME_SEARCH:
        {
            int sigPos = daSigDetect(m_buf);
            if(sigPos < 0)
            {
               // qWarning() << "[DapConnectStream] SigPos < 0 !";
                return;
            }
            else
            {
                m_buf = m_buf.mid(sigPos);
                m_streamState = SSS_FRAME_HEADER;
            }
        } break;

        case SSS_FRAME_HEADER:
        {
            if(m_buf.length() >= (int) sizeof(DapPacketHdr))
            {
                const DapPacketHdr* dapPktConstHdr = (DapPacketHdr* ) m_buf.constData();
                size_t dapPktSize = dapPktConstHdr->size;

                if(dapPktSize > DAP_PKT_SIZE_MAX)
                {
                    qWarning() << "[DapConnectStream] Package size too big = " << dapPktSize;
                    m_buf.clear();
                    m_streamState = SSS_FRAME_SEARCH;
                    m_dapDataPosition = 0;
                    return;
                } else if (dapPktSize == 0) {
                    switch (dapPktConstHdr->type) {
                    case 0x11: //Keep-alive pkt
                    case 0x12: //Keep-alive pkt too...
                        emit isAlive(true);
                        break;
                    default:
                        //qDebug() << "Pkt type " << dapPktConstHdr->type << " unrecognized, len " << m_buf.length();
                        break;
                    }
                    m_buf.clear();
                    m_streamState = SSS_FRAME_SEARCH;
                    m_dapDataPosition = 0;
                    return;
                }

                memcpy(&m_dapPktHdr, m_buf.constData(), sizeof(DapPacketHdr));

                m_buf = m_buf.mid(sizeof(DapPacketHdr));

                if( (int)dapPktSize <= m_buf.length() )
                {
                    memcpy(m_dapData, m_buf.constData(), dapPktSize);
                    procPktIn(&m_dapPktHdr, m_dapData);
                    m_streamState = SSS_FRAME_SEARCH;
                    m_buf = m_buf.mid(dapPktSize);
                    m_dapDataPosition = 0;
                }
                else
                {
                    m_dapDataPosition = m_buf.length();
                    memcpy(m_dapData, m_buf.constData(), m_dapDataPosition);
                    m_buf.clear();
                    m_streamState = SSS_FRAME_BODY;
                    cycle_flag = false;
                }
            } else {
                //qDebug() << "[DapConnectStream] Collecting data for DA frame header";
                cycle_flag = false;
            }
        } break;

        case SSS_FRAME_BODY:
        {
          if(m_buf.length() < ( (int)m_dapPktHdr.size) - m_dapDataPosition)
          {
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), (size_t) m_buf.length());
              m_dapDataPosition += m_buf.length();
              m_buf.clear();
              cycle_flag = false;
          }
          else
          {
              size_t bytesLeft = m_dapPktHdr.size - ( (size_t)m_dapDataPosition );
              memcpy(m_dapData + m_dapDataPosition, m_buf.constData(), bytesLeft);

              procPktIn(&m_dapPktHdr, m_dapData);
              m_streamState = SSS_FRAME_SEARCH;
              m_buf = m_buf.mid(bytesLeft);
          }

        } break;

        default: qWarning() << "[DapConnectStream] Unexcepted state";
            break;
     }
   }
}

void DapStreamer::_detectPacketLoose(quint64 currentSeqId)
{
    int countLoosedPackets = currentSeqId - (m_lastSeqId + 1);
    if (countLoosedPackets > 0) {
        qWarning() << "Packet Loosed count:" << countLoosedPackets;
        emit sigStreamPacketLoosed(countLoosedPackets);
    } else if((countLoosedPackets < 0) && (currentSeqId > 0)) {
        qWarning() << "Something wrong. countLoosedPackets is " << countLoosedPackets
                   << "can't be less than zero. Current seq id:" << currentSeqId
                   << "last seq id: " << m_lastSeqId;
    }
    m_lastSeqId = currentSeqId;
}

void DapStreamer::procPktIn(DapPacketHdr * pkt, void * data)
{
    m_procPktInData.append((const char*) data, pkt->size);
    m_session->getDapCrypt()->decode(m_procPktInData, m_procPktInDecData, KeyRoleStream);

    if(m_procPktInDecData.size() > sizeof(DapChannelPacketHdr)) {
        DapChannelPacketHdr* channelPkt = (DapChannelPacketHdr*) malloc(sizeof(DapChannelPacketHdr));
        memcpy(channelPkt, m_procPktInDecData.constData(), sizeof(DapChannelPacketHdr));
        _detectPacketLoose(channelPkt->seq_id);
        void* channelData = malloc(m_procPktInDecData.size() - sizeof(DapChannelPacketHdr) + 1);
        memcpy(channelData, m_procPktInDecData.constData() + sizeof(DapChannelPacketHdr),
               m_procPktInDecData.size() - sizeof(DapChannelPacketHdr));
        emit sigProcPacket(new DapChannelPacket(channelPkt, channelData));
    } else {
        qWarning() << "Error decode. Packet loosed";
    }

    m_procPktInData.clear();
    m_procPktInDecData.clear();
}

void DapStreamer::addChProc(char chId, DapChBase* obj) {
    if(m_dsb.contains(chId)) {
        qCritical() << "Proc with id" << chId << "already exists";
        return;
    }
    m_dsb.insert(chId, obj);
    connect(obj, &DapChBase::pktChOut, this, &DapStreamer::writeChannelPacket);
    connect (this, &DapStreamer::sigProcPacket, obj, &DapChBase::onPktIn);
}

