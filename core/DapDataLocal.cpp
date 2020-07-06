#include <QStandardPaths>

#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QSettings>
#include <QCoreApplication>
#include <algorithm>
#include <QTime>

#include "DapDataLocal.h"

DapDataLocal::picturesMap DapDataLocal::m_pictruePath = {
    {DapServerLocation::ENGLAND, ":/country/GB.png"},
    {DapServerLocation::FRANCE, ":/country/FR.png"},
    {DapServerLocation::GERMANY, ":/country/DE.png"},
    {DapServerLocation::USA, ":/country/US.png"},
    {DapServerLocation::NETHERLANDS, ":/country/NL.png"},
    {DapServerLocation::RUSSIA, ":/country/RU.png"},
    {DapServerLocation::UKRAINE, ":/country/UA.png"},
    {DapServerLocation::UNKNOWN, ":/country/Flag_gf.png"},
    {DapServerLocation::Netherlands, ":/country/NL.png"},
    {DapServerLocation::Singapore, ":/country/SG.png"},
    {DapServerLocation::Germany, ":/country/DE.png"},
};

DapDataLocal *DapDataLocal::_me = Q_NULLPTR;

DapDataLocal::DapDataLocal()
{
    qDebug() << "[DL] DapDataLocal Constructor";
    parseXML(":/data.xml");

    this->loadAuthorizationDatas();
}

void DapDataLocal::parseXML(const QString& a_fname)
{
    QFile file(a_fname);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Can't open data.xml from built in resource";
        return;
    }
    qDebug() << "data.xml opened, size "<< file.size();

    QXmlStreamReader *sr = new QXmlStreamReader(&file);
    if(sr->readNextStartElement()){
        if(sr->name() == "data"){
            while ( sr->readNextStartElement() ){
                if( sr->name() == "servers"){
                    while ( sr->readNextStartElement() ){
                        if( sr->name() == "server") {
                            DapServerInfo item;
                            while ( sr->readNextStartElement() ){
                                if(sr->name() == "name"){
                                    item.name=sr->readElementText();
                                } else if (sr->name() == "address") {
                                    item.address=sr->readElementText();
                                } else if( sr->name() == "port") {
                                    bool ok;
                                    quint16 port = quint16(sr->readElementText().toInt(&ok));
                                    if (!ok) {
                                        throw std::runtime_error("Can't cast port to int "
                                                                 "from XML file");
                                    }
                                    item.port = port;
                                } else if(sr->name() == "location") {
                                    item.location = DapServerInfo::stringToLocation(sr->readElementText());
                                } else {
                                    qWarning() << "[DL] Inside tag 'server': Unknown tag "<<sr->name();
                                    sr->skipCurrentElement();
                                }
                            }
                            qDebug() << "[DL] Server "<<item.name<<" added";
                            m_servers.push_back(item);
                        }else{
                            qDebug() << "[DL] Inside tag 'servers': unknown tag "<<sr->name();
                            sr->skipCurrentElement();
                        }
                    }
                }else if( sr->name() == "cdb"){
                    m_cdbServersList.push_back(sr->readElementText());
                    qInfo() << "Add CDB address: " << m_cdbServersList.back();
                }else if( sr->name() == "network-default"){
                    m_networkDefault = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else if( sr->name() == "title"){
                    m_brandName = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else if( sr->name() == "sign_up"){
                    urlSignUp = sr->readElementText();
                    qInfo() << "Network defaut: " << m_networkDefault;
                }else{
                    qDebug() << "[DL] Inside tag 'data' unknown tag "<<sr->name();
                    sr->skipCurrentElement();
                }
            }
        }
    }
#ifdef  QT_DEBUG
    addServer(DapServerLocation::UNKNOWN, "local", "127.0.0.1",  8002);
#endif


    delete sr;
}

void DapDataLocal::addServer(const DapServerInfo& dsi) {

    m_servers.push_back(dsi);

    emit this->serverAdded(dsi);
}

/**
 * @brief DapDataLocal::clearServerList
 */
void DapDataLocal::clearServerList()
{
    m_servers.clear();
    this->setCurrentServer(nullptr);

    emit serversCleared();
}



void DapDataLocal::setServerTheBest(const DapServerInfo &server){
    int index = m_servers.indexOf(server);
    if (index == -1) {
        qWarning() << "Server not found";
        return;
    } else {
        m_servers.move(index, 0);
    }
}

DapServerInfo *DapDataLocal::currentServer()
{
    return m_currentServer;
}

void DapDataLocal::setCurrentServer(int a_serverIndex)
{
    if (a_serverIndex == -1)
        return this->setCurrentServer(nullptr);

    Q_ASSERT(a_serverIndex >= 0 && a_serverIndex < m_servers.count());

    this->setCurrentServer(&m_servers[a_serverIndex]);
}

void DapDataLocal::setCurrentServer(DapServerInfo *a_server)
{
    qDebug() << "DapDataLocal::setCurrentServer(" << (a_server ? a_server->name : "") << ")";
    if (m_currentServer == a_server)
        return;
    m_currentServer = a_server;

    emit this->serverNameChanged(a_server ? a_server->name : "");
}

void DapDataLocal::setRandomServerIfIsEmpty()
{
    if (!currentServer())
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int randIndex = qrand()%(this->servers().count());
        this->setCurrentServer(randIndex);
        qDebug()<<"Random server chosed:" << this->currentServerName();
    }
}


/// Get login.
/// @return Login.
QString DapDataLocal::login() const
{
    return m_login;
}

/// Set login.
/// @param login Login.
void DapDataLocal::setLogin(const QString &a_login)
{
    if (this->m_login == a_login)
        return;
    m_login = a_login;

    emit loginChanged(m_login);
}

/// Get password.
/// @return Password.
QString DapDataLocal::password() const
{
    return m_password;
}

/// Set password.
/// @param password Password.
void DapDataLocal::setPassword(const QString &a_password)
{
    if (this->m_password == a_password)
        return;
    this->m_password = a_password;

    emit this->passwordChanged(m_password);
}
/// Set serial key.
/// @param serial serial key.
void DapDataLocal::setSerialKey(const QString &a_serialKey)
{
    if (this->m_serialKey == a_serialKey)
        return;
    m_serialKey = a_serialKey;

    emit this->serialKeyChanged(m_serialKey);
}

QString DapDataLocal::serialKey() const
{
    return m_serialKey;
}
/// Get server name.
/// @return Server name.
QString DapDataLocal::currentServerName() const
{
    return m_currentServer ? m_currentServer->name : "";
}

/// Set server name.
/// @param server Server name.
void DapDataLocal::setServerName(const QString &a_serverName)
{
    if (currentServerName() == a_serverName)
        return;

    for (DapServerInfo& l_currentServer: servers())
    {
        if (a_serverName == l_currentServer.name)
            return this->setCurrentServer(&l_currentServer);
    }
    
    qFatal("There is no server with name %s", qPrintable(a_serverName));
}

void DapDataLocal::saveAuthorizationDatas()
{
    DapDataLocal::me()->saveSecretString(DapDataLocal::me()->TEXT_LOGIN     , DapDataLocal::me()->login());
    DapDataLocal::me()->saveSecretString(DapDataLocal::me()->TEXT_PASSWORD  , DapDataLocal::me()->password());
    DapDataLocal::me()->saveSecretString(DapDataLocal::me()->TEXT_SERIAL_KEY, DapDataLocal::me()->serialKey());
}

void DapDataLocal::loadAuthorizationDatas()
{
    this->setSerialKey(getSecretString(TEXT_SERIAL_KEY));
    this->setLogin(getSecretString(TEXT_LOGIN));
    this->setPassword(getSecretString(TEXT_PASSWORD));
}

void DapDataLocal::rotateCDBList() {
    if (m_cdbServersList.size() > 1) {
        auto tmp = m_cdbServersList.takeFirst();
        m_cdbServersList.push_back(tmp);
    }
}

void DapDataLocal::saveSecretString(QString key, QString string){

    QSettings settings;
    initSecretKey();
    QByteArray tempStringIn = string.toUtf8(), tempStringOut;
    secretKey->encode(tempStringIn, tempStringOut);
    settings.setValue(key, tempStringOut);
}

QString DapDataLocal::getSecretString(QString key){
    QSettings settings;
    QByteArray stringIn = settings.value(key).toByteArray(), stringOut;
    if (stringIn.isEmpty())
        return "";
    initSecretKey();
    secretKey->decode(stringIn, stringOut);
    return QString(stringOut);
}

bool DapDataLocal::initSecretKey(){

    QSettings settings;
    if (settings.value("key").toString().isEmpty()){
        settings.setValue("key", getRandomString(40));
    }
    if (secretKey != nullptr) {
        delete secretKey;
    }
    secretKey = new DapKeyAes();
    QString kexString = settings.value("key").toString() + "SLKJGN234njg6vlkkNS3s5dfzkK5O54jhug3KUifw23";
    return secretKey->init(QString(kexString));
}

QString DapDataLocal::getRandomString(int size)
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = size;

   QString randomString;
   for(int i=0; i < randomStringLength; ++i){
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

/// Get server name by its address.
/// @param address Server address.
/// @return Server name.
QString DapDataLocal::getServerNameByAddress(const QString &address)
{
    QList<DapServerInfo>::const_iterator it = std::find_if(m_servers.cbegin(), m_servers.cend(), 
        [=] (const DapServerInfo& server) 
        { 
        if (server.name != "Auto")
            return server.address == address;
        });
    
    if(it != m_servers.cend())
        return (*it).name;
    
    qCritical() << "Server with address " << address << " not found";
    
    return QString();
}

void /// Connect ComboBox to DataLocale. ComboBox currentText and DataLocale serverName will be synchronized
/// @param a_comboBox ComboBox
DapDataLocal::connectComboBox(QObject*a_comboBox)
{
    //connection when serverName in DataLocale changed
    connect(this,       SIGNAL(serverNameChanged(QString)),
            a_comboBox, SLOT  (setCurrentText(QString)));

    //connection when comboBoxChanged
    connect(a_comboBox, SIGNAL(currentTextChanged(QString)),
            this,       SLOT  (setServerName(QString)));
}

void DapDataLocal::addServer(DapServerLocation location, const QString& name,
                          const QString & address, quint16 port)
{
    DapServerInfo ss;
    ss.name = name;
    ss.location = location;
    ss.address = address;
    ss.port = port;
    addServer(std::move(ss));
}

/**
 * @brief DataLocal::locationToIcon
 * @param a_location
 * @return
 */
QString DapDataLocal::locationToIconPath(DapServerLocation loc)
{
    QString locPath = m_pictruePath.value(loc);
    if (locPath == "") {
        qWarning() << "Not found picture for current location. Return default!";
        return m_pictruePath.value(DapServerLocation::UNKNOWN);
    }
    return locPath;
}

/*void DapDataLocal::setCurrentServer(DapServerInfo *a_server)
{
    int index = 0;
    for (auto server : m_servers){
        if (server.address == a_server->address && server.name != "Auto"){
            this->setCurrentServer(index);
            emit this->serverNameChanged(server.name);
            return;
        }
        index++;
    }
    this->setCurrentServer(0);
}
*/

void DapDataLocal::clearCurrentServer()
{
    m_currentServer = nullptr;
}

bool DapDataLocal::newServerAfterNetworkError()
{
    if (m_serversForCheck.isEmpty())
        for (auto server : m_servers)
            m_serversForCheck.push_back(server);

    DapServerInfo *currentServer = this->currentServer();

    QMutableVectorIterator<DapServerInfo> i(m_serversForCheck);
    while(i.hasNext()) {
        DapServerInfo temp = i.next();
        if (temp.address == currentServer->address)
            i.remove();
    }

    if (m_serversForCheck.isEmpty())
        return false;

    DapServerLocation alocation = currentServer->location;

    if (currentServer->name == "Auto" || bBeforeAutoServer) {
        bBeforeAutoServer = true;
        for (DapServerInfo& server : m_serversForCheck){
            if (currentServer->address != server.address){
                this->setCurrentServer(&server);
                return true;
            }
        }
    }
    else {
        //сейчас спросить о том что не получилось и можно ли установить любой сервер из того же региона.
        for (DapServerInfo& server : m_serversForCheck){
            if (server.location == alocation && currentServer->address != server.address){
                this->setCurrentServer(&server);
                return true;
            }
        }
        //серверов в регионе больше нет. спросить о том что сейчас люой вообще сервер будет выбран
        for (DapServerInfo& server : m_serversForCheck){
            if (currentServer->address != server.address){
                this->setCurrentServer(&server);
                return true;
            }
        }
    }
    return false;
    //обработать случай когда  остаётся 0 серверов, выйти на экран и вызвать стандартуную обработку это йошибки
}
