#include "MainScreen.h"
#include "Utilz.h"

const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());
    create<Ui::MainScreen>();

    AdaptiveScreen::initScreen(this);

    m_connectedTimer.setInterval(CONNECTED_TIME_INTERVAL);

    // Signal-slot connection that updates the total connection time with a timeout of 1 second
    connect(&m_connectedTimer, &QTimer::timeout, [this]
    {
        this->updateTimeIndicators();
    });
}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(a_state)
#else
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::STATE, a_state);

    this->updateChildStyle  (LBL_STATUS_MESSAGE);

    if(a_state == ConnectionStates::Disconnected)
        this->stopConnectionTimer();
#endif
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    this->connectBtnToSignall(BTN_CONNECTION, &MainScreen::disconnect, a_widget);

    QCheckBox *chbAuthorized    ; Utils::findChild(a_widget, CHB_AUTHORIZED     , chbAuthorized);
    QCheckBox *chbStreamOpened  ; Utils::findChild(a_widget, CHB_STREAM_OPENED  , chbStreamOpened);
    QCheckBox *chbVirtualNetwork; Utils::findChild(a_widget, CHB_VIRTUAL_NETWORK, chbVirtualNetwork);

#ifdef Q_OS_ANDROID

    QFrame      *frmConnect     ; Utils::findChild(a_widget, FRM_CONNECT        , frmConnect);
    QFrame      *frmInfo        ; Utils::findChild(a_widget, FRM_INFO           , frmInfo);
    QFrame      *frmStatus      ; Utils::findChild(a_widget, FRM_STATUS         , frmStatus);
    QPushButton *btnChangeServer; Utils::findChild(a_widget, BTN_CHANGE_SERVER  , btnChangeServer);

    //========================================================================
    btnChangeServer ->setGraphicsEffect(new StyledDropShadowEffect(btnChangeServer));
    frmConnect      ->setGraphicsEffect(new StyledDropShadowEffect(frmConnect)     );
    frmInfo         ->setGraphicsEffect(new StyledDropShadowEffect(frmInfo)        );
    frmStatus       ->setGraphicsEffect(new StyledDropShadowEffect(frmStatus)      );

    btnChangeServer->hide();

#else
    QPushButton *btnBytes   ; Utils::findChild(a_widget, BTN_BYTES  , btnBytes  );
    QPushButton *btnPackets ; Utils::findChild(a_widget, BTN_PACKETS, btnPackets);

    connect(btnBytes,&QPushButton::clicked,[=]{
        setIndicatorUnits(IndicatorsUnits::Bytes);
    });
    connect(btnPackets,&QPushButton::clicked,[=]{
        setIndicatorUnits(IndicatorsUnits::Packets);
    });

#endif

}

void MainScreen::setAuthorized(bool a_authorized /*= true*/)
{
    this->setChildProperties(CHB_AUTHORIZED, Properties::CHECKED, a_authorized);
}

void MainScreen::setStreamOpened(bool a_streamOpened /*= true*/)
{
    this->setChildProperties(CHB_STREAM_OPENED, Properties::CHECKED, a_streamOpened);
}

void MainScreen::setVirtualNetwork(bool a_virtualNetwork /*= true*/)
{
    this->setChildProperties(CHB_VIRTUAL_NETWORK, Properties::CHECKED, a_virtualNetwork);
}

void MainScreen::setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent)
{
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;

    this->updateSentRecievedIndicators();
}

uint64_t MainScreen::connectedTime()
{
    return m_loginTime.secsTo(QDateTime::currentDateTime());
}

void MainScreen::updateSentRecievedIndicators()
{
//TODO:
    // Check why sigReadWriteBytesStat signal send wrong datas (recieved/sent are contrarily)
    this->setChildProperties(LBL_SENT    , Properties::TEXT, this->indicatorUnitsIsBytes() ? m_bytesReceived : m_packetsReceived);
    this->setChildProperties(LBL_RECEIVED, Properties::TEXT, this->indicatorUnitsIsBytes() ? m_bytesSent     : m_packetsSent    );
}

void MainScreen::updateTimeIndicators()
{
    QString loginTime;
    if (m_loginTime == QDateTime())
        loginTime = EMPTY_TYME;
    else if (m_loginTime.date() == QDate::currentDate())
        loginTime = m_loginTime.toString("hh:mm");
    else
        loginTime = m_loginTime.toString("MM-dd-yy hh:mm");

    this->setChildProperties(LBL_LOGIN_TIME, Properties::TEXT, loginTime);


    QString connectedTime;
    if (this->connectedTime() == 0)
        connectedTime = EMPTY_TYME;
    else
        connectedTime = MainScreen::toTimeString(this->connectedTime());

    this->setChildProperties(LBL_CONNECTED_TIME, Properties::TEXT, connectedTime);
}

QString MainScreen::statusText(ConnectionStates a_state)
{
    switch (a_state)
    {
        case ConnectionStates::Disconnected:
            return  "Not connected";
        case ConnectionStates::Connecting:
            return "Connecting...";
        case ConnectionStates::Connected:
            return  "Connected";
        case ConnectionStates::Disconnecting:
            return  "Server down";
    }
    return QString();
}


bool MainScreen::indicatorUnitsIsBytes() const
{
#ifdef ANDROID
    return true;
}
#else
    return (this->indicatorUnits() == IndicatorsUnits::Bytes);
}

void MainScreen::setIndicatorUnits(const IndicatorsUnits &a_indicatorUnits)
{
    if (m_indicatorUnits == a_indicatorUnits)
        return;
    m_indicatorUnits = a_indicatorUnits;

    this->setChildProperties(BTN_BYTES  , Properties::CHECKED, a_indicatorUnits == IndicatorsUnits::Bytes);
    this->setChildProperties(BTN_PACKETS, Properties::CHECKED, a_indicatorUnits == IndicatorsUnits::Packets);

    this->setChildProperties(LBL_RECREIVED_TITLE, Properties::TEXT, this->receivedIndicatorTitle());
    this->setChildProperties(LBL_SENT_TITLE     , Properties::TEXT, this->sendIndicatorTitle());

    this->updateSentRecievedIndicators();
}

QString MainScreen::receivedIndicatorTitle() const
{
    return (this->indicatorUnitsIsBytes() ? BYTES : PACKETS) + " received";
}

QString MainScreen::sendIndicatorTitle() const
{
    return (this->indicatorUnitsIsBytes() ? BYTES : PACKETS) + " send";
}
#endif

QDateTime MainScreen::loginTime() const
{
    return m_loginTime;
}

void MainScreen::setLoginTime(const QDateTime &loginTime)
{
    m_loginTime = loginTime;

    this->updateTimeIndicators();
}

MainScreen::IndicatorsUnits MainScreen::indicatorUnits() const
{
    return m_indicatorUnits;
}


QString MainScreen::toTimeString(quint64 seconds)
{
    const qint64 DAY = 86400;
    qint64 days = seconds / DAY;
    QTime t = QTime(0,0).addSecs(seconds % DAY);

    QString res = QString();
    if (days > 0)
        res.sprintf("%d %02d:%02d:%02d", static_cast<int>(days), t.hour(), t.minute(), t.second());
    else
        res.sprintf("%02d:%02d:%02d", t.hour(), t.minute(), t.second());

    return res;
}

void MainScreen::startConnectionTimer(const QDateTime &a_startTime)
{   
    this->setLoginTime(a_startTime);

    m_connectedTimer.start();
}

void MainScreen::stopConnectionTimer()
{
    this->setLoginTime(QDateTime());

    m_connectedTimer.stop();
}
