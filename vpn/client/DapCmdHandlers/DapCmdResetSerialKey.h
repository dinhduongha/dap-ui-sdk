#pragma once

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdResetSerialKey : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdResetSerialKey(QObject *parent = nullptr)
        : DapCmdServiceAbstract (DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, parent) {};
    ~DapCmdResetSerialKey() override {};

    void handle(const QJsonObject *params) override;

public slots:
    void sendResetSerialError(const int& a_errorCode, const QString&);

signals:
    void sigResetRequestSent(const QString&);
    void sigResetSerialKeyReplied(const QString&);
};
