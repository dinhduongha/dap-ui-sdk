#include "DapServerInfo.h"

DapServerInfo::countryMap DapServerInfo::m_countries = {
    {"ENGLAND", DapServerLocation::ENGLAND},
    {"FRANCE", DapServerLocation::FRANCE},
    {"GERMANY", DapServerLocation::GERMANY},
    {"USA", DapServerLocation::USA},
    {"NETHERLANDS", DapServerLocation::NETHERLANDS},
    {"RUSSIA", DapServerLocation::RUSSIA},
    {"UKRAINE", DapServerLocation::UKRAINE},
    {"UNKNOWN", DapServerLocation::UNKNOWN}
};

DapServerLocation DapServerInfo::stringToLaction(const QString& location) {
    DapServerLocation v = m_countries.value(location.toUpper());
    if (int(v) == 0) {
        qWarning() << "[DapServerInfo::stringToLaction] Unknown location! "
                      "Return default location(USSR)";
        return DapServerLocation::UNKNOWN;
    }
    return v;
}

bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs)
{
    if(lhs.address == rhs.address &&
            lhs.port == rhs.port &&
            lhs.name == rhs.name &&
            lhs.location == rhs.location) {
        return true;
    }
    return false;
}
