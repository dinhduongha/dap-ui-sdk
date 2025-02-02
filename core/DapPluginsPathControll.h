#ifndef DAPPLUGINSPATHCONTROLL_H
#define DAPPLUGINSPATHCONTROLL_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QFileSystemWatcher>

#include "dap_common.h"
#include "dap_file_utils.h"

class DapPluginsPathControll : public QObject
{
    Q_OBJECT
public:
    explicit DapPluginsPathControll(QObject *parent = nullptr);

public:
    static QString defaultPluginPath(const QString a_brand);
    void setPathToPlugin(QString path){ m_pathToPlugin = path;
                                        m_pathToPluginsDownload = path + "/download";}
    QString getPathToPlugin(){ return m_pathToPlugin; }
    QString getPathToPluginsDownload(){ return m_pathToPluginsDownload; }

private:
    QString m_pathToPlugin;
    QString m_pathToPluginsDownload;


};

#endif // DAPPLUGINSPATHCONTROLL_H
