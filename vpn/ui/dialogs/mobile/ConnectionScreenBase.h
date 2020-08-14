﻿#ifndef CONNECTIONSCREENBASE_H
#define CONNECTIONSCREENBASE_H

#include "ScreenWithScreenPopupsAbstract.h"
#include "vpnDefine.h"
#include "BrandAppProperties.h"

class ConnectionScreenBase : public ScreenWithScreenPopupsAbstract
{
    Q_OBJECT
public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    ConnectionScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);

signals:
    void disconnectionRequested();
    void serverChangingRequested(const QString& serverName);

public slots:

    virtual void setCurrentServer(const QString& a_currentServer);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;
    virtual QList<CustomPopup *> customPopups() override;

    QScopedPointer<CONNECTION_FORM> m_ui;

    const QString CBB_SERVER         = "cbbServer";

private:
    ConnectionState m_state {ConnectionState::Disconnected};
    QString statusText();
    ComboBox       *cbbServer;

    QString m_currentServer {};
};

#endif // CONNECTIONSCREENBASE_H
