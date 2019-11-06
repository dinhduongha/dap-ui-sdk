#pragma once

#include <QList>
#include <QString>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QSettings>
#include "DapServerInfo.h"

#define SERVER_LIST_FILE "vpn-servers.xml"

class DapDataLocal : public QObject
{
    Q_OBJECT
    DapDataLocal();
    const QString ServerListName;
protected:
    QString     mLogin;      ///< Login.
    QString     mPassword;   ///< Password.
    QString     mServerName; ///< Server name.

    QString     m_cdbServersList;

public:
    using picturesMap = QMap<DapServerLocation, QString>;
    static DapDataLocal * me(){ return _me?_me: _me = new DapDataLocal();}

    void addServer(DapServerLocation location, const QString& name,
                   const QString & address, quint16 port);
    void addServer(const DapServerInfo& dsi);
    QList<DapServerInfo>& servers(){return m_servers;}
    const DapServerInfo& serverTheBest(){ return m_servers.at(0) ;  }
    void setServerTheBest(const DapServerInfo& server);

    QString locationToIconPath(DapServerLocation loc);
    
    QString login() const;
    void setLogin(const QString &login);

    QString password() const;
    void setPassword(const QString &password);

    QString serverName() const;
    QString getServerNameByAddress(const QString& address);

    const QString & cdbServersList() { return  m_cdbServersList; }

    void connectComboBox(QObject *a_comboBox);

    void clearServerList();

public slots:
    void setServerName(const QString &serverName);

private:
    static DapDataLocal *_me;
    static QMap<DapServerLocation, QString> m_pictruePath;

    QList<DapServerInfo> m_servers;

    void parseXML(const QString& a_fname);
    
signals:
    /// Signal emitted if login has changed.
    /// @param login Login.
    void loginChanged(const QString& login);
    /// Signal emitted if password has changed.
    /// @param password Password.
    void passwordChanged(const QString& password);
    /// Signal emitted if server name has changed.
    /// @param serverName Server name.
    void serverNameChanged(const QString& serverName);
};
