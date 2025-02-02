#include "DapCmdPurchase.h"
#include "DapCmdNews.h"

DapCmdPurchase::DapCmdPurchase(QObject *parent):
    DapCmdClientAbstract (DapJsonCmdType::REQUEST_PURCHASE_VERIFY, parent)
{

}

void DapCmdPurchase::requestVerify(const QString &packageName, const QString &productId, const QString &token)
{
    qDebug() << "Verify purchase id " << productId << ", token " << token;
    QJsonObject obj;
    obj["packageName"] = packageName;
    obj["productId"] = productId;
    obj["token"] = token;
    sendCmd(&obj);
}

void DapCmdPurchase::handleResult(const QJsonObject &result)
{
    qDebug()<<"DapCmdPurchase::handleResult";
    if (result.contains("S/N")) {
        QString key = result.value("S/N").toString();
        qInfo() << "We have purchased key" << key;
        emit purchaseVerified(key);
    }
    else
    {
        emit purchaseError(QString(""));
    }
}

void DapCmdPurchase::handleError(int code, const QString &message)
{
    qDebug() <<"DapCmdPurchase::handleError" << " " << code << ": " << message;
    emit purchaseError(message);
}
