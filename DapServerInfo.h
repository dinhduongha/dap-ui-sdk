#ifndef DAPSERVERINFO_H
#define DAPSERVERINFO_H

#include <QString>
#include <QMap>
#include <QDebug>

enum class DapServerLocation {
    ENGLAND = 1,
    FRANCE,
    GERMANY,
    USA,
    NETHERLANDS,
    RUSSIA,
    UKRAINE,
    UNKNOWN
};

class DapServerInfo
{
public:
    using countryMap = QMap<QString, DapServerLocation>;
    DapServerInfo(){}
    DapServerInfo(const DapServerInfo&cp) {
        address     = cp.address;
        port        = cp.port;
        name        = cp.name;
        location    = cp.location;
    }

    QString address;
    quint16 port;
    QString name;
    DapServerLocation location;

    static DapServerLocation stringToLaction(const QString& location);
    friend bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs);
private:
    static countryMap m_countries;
};

#endif // DAPSERVERINFO_H
