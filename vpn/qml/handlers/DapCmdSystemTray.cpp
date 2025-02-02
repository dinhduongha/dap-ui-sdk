/* INCLUDES */
#include "DapCmdSystemTray.h"

#include <QMetaEnum>
#include <QJsonArray>

/* VARS */
static QMetaEnum *typeEnum        = nullptr;
static const QString actionParam  = "action";
static const QString actionValue  = "value";

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdSystemTray::DapCmdSystemTray(QObject *parent)
  : DapCmdServiceAbstract (DapJsonCmdType::CLIENT_INFO, parent)
{
  if (typeEnum == nullptr)
    typeEnum  = new QMetaEnum (QMetaEnum::fromType<RequestType> ());
}

DapCmdSystemTray::~DapCmdSystemTray()
{
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdSystemTray::handle (const QJsonObject *params)
{
  Q_UNUSED (params);

  if (params->contains (actionParam))
    {
      auto request  = params->value (actionParam).toString();
      auto type     = RequestType (typeEnum->keyToValue (request.toUtf8()));

      qDebug() << ("Request for show interface \"" + request + "\", type #%1").arg (int (type));

      switch (type)
        {
        case RequestType::setting:                    emit showSettingInterface();    break;
        case RequestType::statistic:                  emit showStatisticInterface();  break;
        case RequestType::login:                      emit showLoginInterface();      break;
        case RequestType::dashboard:                  emit showDashboardInterface();  break;
        case RequestType::client_started:             emit clientFound();             break;
        case RequestType::client_exited:              emit clientClosed();            break;
        case RequestType::apllication_quit:           emit quitRequest();             break;
        case RequestType::tray_application_running:   emit trayApplicationFound();    break;
        case RequestType::change_server:
          {
            QJsonArray args = params->value (actionValue).toArray();
            emit changeServer (args[0].toString(), args[1].toString());
          } break;
        }
    }
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdSystemTray::sendShowSettingInterface()
{
  sendShowInterface (RequestType::setting);
}

void DapCmdSystemTray::sendShowStatisticInterface()
{
  sendShowInterface (RequestType::statistic);
}

void DapCmdSystemTray::sendShowLoginInterface()
{
  sendShowInterface (RequestType::login);
}

void DapCmdSystemTray::sendShowDashboardInterface()
{
  sendShowInterface (RequestType::dashboard);
}

void DapCmdSystemTray::sendChangeServer (const QString &a_serverName, const QString &a_serverAddress)
{
  qDebug() << QString ("cmd sendChangeServer name[%1],ip[%2]").arg (a_serverName, a_serverAddress);
  auto name = typeEnum->valueToKey (int (RequestType::change_server));
  QJsonObject response = {
    {actionParam, name},
    {actionValue, QJsonArray {a_serverName, a_serverAddress}},
  };
  sendCmd (&response);
}

void DapCmdSystemTray::apllicationQuitRequest()
{
  sendShowInterface (RequestType::apllication_quit);
}

void DapCmdSystemTray::clientStarted()
{
  sendShowInterface (RequestType::client_started);
}

void DapCmdSystemTray::clientExited()
{
  sendShowInterface (RequestType::client_exited);
}

void DapCmdSystemTray::trayApplicationStarted()
{
  sendShowInterface (RequestType::tray_application_running);
}



/// @param address Server address.
void DapCmdSystemTray::sendShowInterface (const RequestType &a_type)
{
  auto name = typeEnum->valueToKey (int (a_type));
  qDebug() << "cmd sendShowInterface " << name;

  QJsonObject response = {{actionParam, name}};
  sendCmd (&response);
}

/*-----------------------------------------*/
