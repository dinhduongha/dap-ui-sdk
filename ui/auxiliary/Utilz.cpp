#include "Utilz.h"

#include <QVector>
#include <QStyle>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QMainWindow>


namespace Utils
{
    QRegularExpression regularEmail()
    {
        return QRegularExpression("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption) ;
    }


    int getNumberVersion(const QString &version,TypeVersion type)
    {
        int minVersion(0);
        int majVersion(0);
        int patVersion(0);

        QStringList versionList = version.split(".");
        if(!versionList[0].isEmpty())
            majVersion = versionList[0].toInt();
        if(versionList.count()>1)
        {
            QStringList minVersionList = versionList[1].split("-");
            if(!minVersionList[0].isEmpty())
                minVersion = minVersionList[0].toInt();
            if(minVersionList.count()>1)
                if(!minVersionList[1].isEmpty())
                    patVersion = minVersionList[1].toInt();
        }

        if(type == TypeVersion::MAJOR)
            return majVersion;
        if(type == TypeVersion::MINOR)
            return minVersion;
        if(type == TypeVersion::PAT)
            return patVersion;

        return 0;
    }

    QString getOSName()
    {
        #if defined(Q_OS_ANDROID)
        return QLatin1String("android");
        #elif defined(Q_OS_MACOS)
        return QLatin1String("mac");
        #elif defined(Q_OS_WIN)
        return QLatin1String("windows");
        #elif defined(Q_OS_IOS)
        return QLatin1String("ios");
        #elif defined(Q_OS_LINUX)
        return QLatin1String("linux");
        #else
        return QLatin1String("unknown");
        #endif
    }

    QColor toColor(const QString &strRGBA)
    {
        QString strColor(strRGBA.simplified());
        if (!strColor.isEmpty() && strColor[0] == '#')
            return QColor(strRGBA);

        QVector<int> result;
        QString temp;
        for(auto it(strColor.begin()); it != strColor.end(); ++it)
        {
            if((*it).isDigit() || (*it) == '.')
                temp += *it;
            else if(!(temp.isEmpty() || temp.isNull()))
            {
                result.append(temp.contains(".") ? int(temp.toDouble() * 255) : temp.toInt());
                temp.clear();
            }
        }
        return (result.size() == 4) ? QColor(result.at(0), result.at(1), result.at(2), result.at(3)) : QColor();
    }

    int toIntValue(const QString &a_text)
    {
        QRegExp regString("(\\d+)");
        regString.indexIn(a_text);
        return regString.cap(0).toInt();
    }

    QString convertByte(const quint64 &byte)
    {
        if (byte < 0){
            return QString("error");
        } else if (byte >= pow(2,40)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,40), 'f', 3)).arg("Tb");
        } else if (byte >= pow(2,30)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,30), 'f', 3)).arg("Gb");
        } else if (byte >= pow(2,20)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,20), 'f', 3)).arg("Mb");
        } else if (byte >= pow(2,10)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,10), 'f', 3)).arg("Kb");
        } else return QString("%1 %2").arg(QString::number(byte)).arg("bytes");
    }

    void setPropertyAndUpdateStyle(QWidget *a_widget, const QString &a_property, const QVariant &a_value /*= true*/)
    {
        QByteArray l_tempStr = a_property.toLatin1();
        const char* l_property = l_tempStr.constData();
        Utils::setPropertyAndUpdateStyle(a_widget, l_property, a_value);
    }

    void setPropertyAndUpdateStyle(QWidget *a_widget, const char* a_property, const QVariant &a_value /*= true*/)
    {

        if (a_widget->property(a_property) == a_value)
            return;
        a_widget->setProperty(a_property, a_value);

        Utils::updateStyle(a_widget);
    }

    void updateStyle(QWidget *a_widget)
    {
        a_widget->style()->unpolish(a_widget);
        a_widget->style()->polish(a_widget);
    }

    QString getTextFromFile(const QString &a_fname){
        QString fileText;
        QFile file(a_fname);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            fileText = QString(file.readAll());
            file.close();
        }
        return fileText;
    }

    Qt::LayoutDirection toQtLayoutDirection(QBoxLayout::Direction a_direction)
    {
        switch (a_direction) {
            case QBoxLayout::Direction::LeftToRight:
                return Qt::LeftToRight;
            case QBoxLayout::Direction::RightToLeft:
                return Qt::RightToLeft;
            default:
                return Qt::LayoutDirectionAuto;
        }
    }

    QBoxLayout::Direction toQBoxLayoutDirection(Qt::LayoutDirection a_direction)
    {
        switch (a_direction) {
            case Qt::LeftToRight:
                return QBoxLayout::Direction::LeftToRight;
            case Qt::RightToLeft:
                return QBoxLayout::Direction::RightToLeft;
            default:
                return QBoxLayout::Direction::LeftToRight;
        }
    }

    QString toNativeLanguageName(QLocale::Language a_language)
    {
        switch (a_language)
        {
            case QLocale::English   : return "English";
            case QLocale::Chinese   : return "中文";
            case QLocale::Russian   : return "Русский";
            case QLocale::Ukrainian : return "Український";
            case QLocale::French    : return "Française";
            case QLocale::Korean    : return "한국어";
            case QLocale::Thai      : return "ไทย";
            case QLocale::Khmer     : return "ខ្មែរ";
            default: return "";
        }
    }
}
