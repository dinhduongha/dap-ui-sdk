#include "DapCmdServerData.h"

void DapCmdServerData::handle(const QJsonObject *params)
{
    qDebug() << "DapCmdServerData::handle";
    Q_UNUSED(params);

    DapServerInfo dsi;

    if(params->contains("address")) {
        qDebug() << "address" << params->value("address").toString();
        dsi.setAddress (params->value("address").toString());
    }
    if(params->contains("address6")) {
        qDebug() << "address6" << params->value("address6").toString();
        dsi.setAddress6 (params->value("address6").toString());
    }
    if(params->contains("port")) {
        qDebug() << "port" << params->value("port").toString();
        dsi.setPort (params->value("port").toInt());
    }
    if(params->contains("name")) {
        qDebug() << "name" << params->value("name").toString();
        dsi.setName (params->value("name").toString());
    }
    if(params->contains("location")) {
        qDebug() << "location" << params->value("location").toString();
        dsi.setLocation (params->value("location").toString());
    }
    if (params->contains("state")) {
        qDebug() << "state" << params->value("location").toString();
        dsi.setOnline (params->value("state").toString());
    }

    emit sentCurrentServerInfo(dsi);
}

/// @param address Server address.
void DapCmdServerData::sendServerData(const QString &address, quint16 a_port)
{
    QJsonObject response;
    response["address"] = address;
    response["port"] = a_port;
    sendCmd(&response);
}
