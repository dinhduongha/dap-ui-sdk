/* INCLUDES */
#include "statistics.h"
#include "ui_statistics.h"
#include "helper/trafficstringhelper.h"
#include "helper/uptimestringhelper.h"
#include <QTimer>

/* VARS */
static DapGraphicSceneStyle graphicSceneStyle;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Statistics::Statistics (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Statistics),
  m_downloadSpeed (0),
  m_uploadSpeed (0),
  m_bytesReceived (0),
  m_bytesSent (0),
  m_packetsReceived (0),
  m_packetsSent (0),
  m_ping (0),
  m_scene (new QGraphicsScene),
  m_uptimeUpdateTimer (new QTimer),
  m_drawGraphTimer(new QTimer),
  m_diagramVisible(false)
{
  /* setup ui */
  ui->setupUi (this);
  ui->statPing->setVisible (false);

  /* setup scene */
  ui->graphicsView->setScene (m_scene);
  ui->graphicsView->setForm (this);
  ui->graphicsView->scale(0.965, 0.935);
  //ui->graphicsView->setBackgroundBrush(QBrush("#f7f8fa", Qt::SolidPattern));

  /* setup uptime timer */
  m_uptimeUpdateTimer->setSingleShot (false);
  m_uptimeUpdateTimer->setInterval (1000);

  /* set styles for graphics */
  addItemGraphicSceneStyle ("shChartDownload",  "color:#D61F5D;depth:1;background-color:rgba(0,181,43,0.00);");
  addItemGraphicSceneStyle ("shChartUpload",    "color:#2D1FD6;depth:1;background-color:rgba(3,167,198,0.00);");
  addItemGraphicSceneStyle ("shGrid",           "color:#F4F4F8;depth:0.1;");
  addItemGraphicSceneStyle ("graphicsLines",    "horizontalLines:19;verticalLines:8;");//"horizontalLines:19;verticalLines:8;sceneWidth:236px;sceneHeight:119px;");

  ui->graphicsView->updateG();

  /* update graph timer */
  m_drawGraphTimer->setInterval(500);
  connect (m_drawGraphTimer, &QTimer::timeout, [&]
  {
      m_drawGraphTimer->stop();
      updateGraph();
      m_drawGraphTimer->start();
  });

  /* signals */
  connect (m_uptimeUpdateTimer, &QTimer::timeout,
           this, &Statistics::_slotUpdateUptimeTime);

  m_drawGraphTimer->start();
}

Statistics::~Statistics()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

void Statistics::startImitatingSchedules()
{
  /* vars */
  static QTimer *t              = nullptr;

  /* checks */
  if (t)
    return;

  /* create timer */
  t = new QTimer;
  t->setSingleShot (false);
  t->setInterval (1000);

//  /* set styles for graphics */
////  addItemGraphicSceneStyle ("shChartDownload",   AppStyleSheetHandler::getWidgetStyleSheet ("#shChartDownload", "active"));
////  addItemGraphicSceneStyle ("shChartUpload",     AppStyleSheetHandler::getWidgetStyleSheet ("#shChartUpload", "active"));
////  addItemGraphicSceneStyle ("shGrid",            AppStyleSheetHandler::getWidgetStyleSheet ("#shGrid", "active"));
////  addItemGraphicSceneStyle ("graphicsLines",     AppStyleSheetHandler::getWidgetStyleSheet ("#graphicsLines", "active"));
//  addItemGraphicSceneStyle ("shChartDownload",  "color:#D61F5D;depth:1;background-color:rgba(0,181,43,0.00);");
//  addItemGraphicSceneStyle ("shChartUpload",    "color:#2D1FD6;depth:1;background-color:rgba(3,167,198,0.00);");
//  addItemGraphicSceneStyle ("shGrid",           "color:#ECECEC;depth:0.1;");
//  addItemGraphicSceneStyle ("graphicsLines",    "horizontalLines:19;verticalLines:8;");//"horizontalLines:19;verticalLines:8;sceneWidth:236px;sceneHeight:119px;");

  /* connect lambda to timer */
  connect (t, &QTimer::timeout, [&]
  {
    /* add random values */
    auto rec  = qrand() % 1024,
    sent = qrand() % 1024;
    schedules.addInp (rec);
    schedules.addOut (sent);

    /* setup random stats */
//    setDownloadSpeed (qrand() % (12 * 1024 * 1024));
//    setUploadSpeed (qrand() % (7 * 1024 * 1024));
    addBytesReceived (downloadSpeed());
    addBytesSent (uploadSpeed());
    addPacketsReceived (1);
    addPacketsSent (1);
    setPing (qrand() % 300);

    /* update uptime */
    ui->statUptime->setMainText (uptimeStr());
  });

  /* start timer */
  setStarted (QDateTime::currentDateTime());
  t->start();
}

void Statistics::addItemGraphicSceneStyle (const QString &widget, const QString &styleWidget)
{
  QStringList strParametrs = styleWidget.split (";");
  strParametrs.removeAll ("");
  QMap<QString, QVariant> param;
  for (QString str : strParametrs)
    {
      QStringList tempParam = str.split (":");
      param.insert (tempParam.at (0), tempParam.at (1));
    }
  graphicSceneStyle.insert (widget, param);

  schedules.setStyle (graphicSceneStyle);
}

quint64 Statistics::downloadSpeed() const
{
  return m_downloadSpeed;
}

void Statistics::setDownloadSpeed (const quint64 &downloadSpeed)
{
  m_downloadSpeed = downloadSpeed;
  schedules.addInp (m_downloadSpeed);
}

void Statistics::setDownloadSpeedString(const QString &downloadSpeed)
{
  ui->statDownSp->setMainText (downloadSpeed);
}

quint64 Statistics::uploadSpeed() const
{
  return m_uploadSpeed;
}

void Statistics::setUploadSpeed (const quint64 &uploadSpeed)
{
  m_uploadSpeed = uploadSpeed;
  schedules.addOut (m_uploadSpeed);
}

void Statistics::setUploadSpeedString(const QString &uploadSpeed)
{
  ui->statUpSp->setMainText (uploadSpeed);
}


quint64 Statistics::bytesReceived() const
{
  return m_bytesReceived;
}

void Statistics::setBytesReceived (const quint64 &bytesReceived)
{
  m_bytesReceived = bytesReceived;
  auto text       = TrafficStringHelper (m_bytesReceived).asString(); //QString ("%1 Bytes").arg (m_bytesReceived);
  ui->statBytesRec->setMainText (text);
}

void Statistics::addBytesReceived (const quint64 &bytesReceived)
{
  m_bytesReceived += bytesReceived;
  auto text       = TrafficStringHelper (m_bytesReceived).asString(); //QString ("%1 Bytes").arg (m_bytesReceived);
  ui->statBytesRec->setMainText (text);
}

quint64 Statistics::bytesSent() const
{
  return m_bytesSent;
}

void Statistics::setBytesSent (const quint64 &bytesSent)
{
  m_bytesSent = bytesSent;
  auto text   = TrafficStringHelper (m_bytesSent).asString(); //QString ("%1 Bytes").arg (m_bytesSent);
  ui->statBytesSent->setMainText (text);
}

void Statistics::addBytesSent (const quint64 &bytesSent)
{
  m_bytesSent += bytesSent;
  auto text   = TrafficStringHelper (m_bytesSent).asString(); //QString ("%1 Bytes").arg (m_bytesSent);
  ui->statBytesSent->setMainText (text);
}

quint64 Statistics::packetsReceived() const
{
  return m_packetsReceived;
}

void Statistics::setPacketsReceived (const quint64 &packetsReceived)
{
  m_packetsReceived = packetsReceived;
  auto text         = QString::number (m_packetsReceived);
  ui->statPackRec->setMainText (text);
}

void Statistics::addPacketsReceived (const quint64 &packetsReceived)
{
  m_packetsReceived += packetsReceived;
  auto text         = QString::number (m_packetsReceived);
  ui->statPackRec->setMainText (text);
}

quint64 Statistics::packetsSent() const
{
  return m_packetsSent;
}

void Statistics::setPacketsSent (const quint64 &packetsSent)
{
  m_packetsSent = packetsSent;
  auto text         = QString::number (m_packetsSent);
  ui->statPackSent->setMainText (text);
}

void Statistics::addPacketsSent (const quint64 &packetsSent)
{
  m_packetsSent += packetsSent;
  auto text         = QString::number (m_packetsSent);
  ui->statPackSent->setMainText (text);
}

quint64 Statistics::ping() const
{
  return m_ping;
}

void Statistics::setPing (const quint64 &ping)
{
  m_ping    = ping;
  auto text = QString ("%1 ms").arg (m_ping);
  ui->statPing->setMainText (text);
}

quint64 Statistics::uptime()
{
  return m_started.msecsTo (QDateTime::currentDateTime());
}

QString Statistics::uptimeStr()
{
  UptimeStringHelper u (uptime());
  return u.asString();
}

QDateTime Statistics::started() const
{
  return m_started;
}

void Statistics::setStarted (const QDateTime &started)
{
  m_started = started;

  if (!started.isNull())
  {
    m_uptimeUpdateTimer->start();
    m_diagramVisible = true;
  }
  else
  {
    m_uptimeUpdateTimer->stop();
    m_diagramVisible = false;
  }

  _slotUpdateUptimeTime();
}

void Statistics::updateGraph()
{
  m_scene->setSceneRect(QRectF(0,0, 500, 150));
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->fitInView(QRectF(0,0, 500, 150));
  schedules.draw_chart (m_scene, m_diagramVisible);
  m_scene->update();
}

void Statistics::resetGraph()
{
  schedules.reset();
  updateGraph();
}

const QColor &Statistics::colorBackground() const
{
  return schedules.colorBackground();
}

void Statistics::setColorBackground(const QColor &newColorBackground)
{
  schedules.setColorBackground (newColorBackground);
}

const QColor &Statistics::colorChartDownload() const
{
  return schedules.colorChartDownload();
}

void Statistics::setColorChartDownload(const QColor &newColorChartDownload)
{
  schedules.setColorChartDownload (newColorChartDownload);
}

const QColor &Statistics::colorChartUpload() const
{
  return schedules.colorChartUpload();
}

void Statistics::setColorChartUpload(const QColor &newColorChartUpload)
{
  schedules.setColorChartUpload (newColorChartUpload);
}

void Statistics::_slotUpdateUptimeTime()
{
  ui->statUptime->setMainText (uptimeStr());
}

void Statistics::slotRetranslated()
{
  ui->label->setText (tr ("Statistics"));

  ui->statDownSp->setSubText (tr ("download speed"));
  ui->statUpSp->setSubText (tr ("upload speed"));

  ui->statBytesRec->setSubText (tr ("bytes received"));
  ui->statBytesSent->setSubText (tr ("bytes sent"));

  ui->statPackRec->setSubText (tr ("packets received"));
  ui->statPackSent->setSubText (tr ("packets sent"));

  ui->statUptime->setSubText (tr ("uptime"));
  ui->statPing->setSubText (tr ("ping"));
}

/*-----------------------------------------*/
