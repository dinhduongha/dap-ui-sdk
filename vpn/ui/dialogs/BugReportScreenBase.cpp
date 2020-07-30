#include "BugReportWithEmailScreen.h"

BugReportScreenBase::BugReportScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);
}

QString BugReportScreenBase::message() const
{
    return m_ui->edtMessage->toPlainText();
}

void BugReportScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget);

#ifdef ANDROID
    m_ui->btnSend->setText(tr("SEND REPORT"));
    QScroller::grabGesture(m_ui->edtMessage->viewport(), QScroller::LeftMouseButtonGesture);
#else
    m_ui->layBugReportScreen->setAlignment(m_ui->frmBugReport, Qt::AlignHCenter);

    #if defined(Q_OS_MAC)
        m_ui->edtMessage->setAttribute(Qt::WA_MacShowFocusRect,false);
    #endif
#endif

    connect(this, &BugReportScreenBase::wrongMessage, [=](){
        Utils::setPropertyAndUpdateStyle(m_ui->edtMessage, Properties::WRONG, true);
    });

    connect(this->m_ui->edtMessage, &CustomTextEdit::textChanged, [=]()
    {
        QString str = this->message();

        if (validateText(str))
            return;

        emit this->messageChanged(str);
        Utils::setPropertyAndUpdateStyle(m_ui->edtMessage, Properties::WRONG, false);
    });

    connectBtnToSignall(m_ui->btnSend, &BugReportScreenBase::checkFieldsAndSendReport);
}

bool BugReportScreenBase::validateText(QString &str)
{
    QRegExp rx("\\\\");
    if (str.contains(rx) || str.length() > 500){
        QTextCursor fileViewerCursor = m_ui->edtMessage->textCursor();
        m_ui->edtMessage->setPlainText(str.remove(rx).mid(0, 500));
        fileViewerCursor.movePosition(QTextCursor::End);
        m_ui->edtMessage->setTextCursor(fileViewerCursor);
        return true;
    }
    return false;
}

void BugReportScreenBase::setMessage(const QString &a_message)
{   
    if (this->message() == a_message)
        return;

    m_ui->edtMessage->setPlainText(a_message);
}

bool BugReportScreenBase::checkFields()
{
    return (this->checkMessage());
}

void BugReportScreenBase::checkFieldsAndSendReport()
{
    qDebug() << "BugReportScreenBase::checkFieldsAndSendReport";
    if (this->checkFields())
    {
        qDebug() << "Successful check fields";
        emit this->sendRequested();
    }
}

bool BugReportScreenBase::checkMessage()
{
    if (m_ui->edtMessage->toPlainText().isEmpty())
    {
        emit this->wrongMessage();
        return false;
    }
    else
        return true;
}
