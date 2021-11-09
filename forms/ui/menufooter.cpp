/* INCLUDES */
#include "menufooter.h"
#include "ui_menufooter.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include "ui/helper/auxiliary/UiScaling.h"

/* DEFS */
#define HEIGHT (172)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

MenuFooter::MenuFooter (QWidget *parent) :
    BaseForm (parent),
  ui (new Ui::MenuFooter),
  m_state (BS_CONNECTION),
  m_lock (false)
{
  /* setup */
  ui->setupUi (this);
  setVisible (false);
  ui->progress->setVisible (false);
  setCssStyle ("footer backgroundcolor");
//#ifndef Q_OS_ANDROID
//  setCssStyle ("footer");
//#else
//  ui->Background->setCssStyle ("backgroundcolor");
//#endif // Q_OS_ANDROID
  qRegisterMetaType<ButtonState> ("ButtonState");
  qRegisterMetaType<MenuFooter::ButtonState> ("MenuFooter::ButtonState");
  //setWindowFlags(Qt::WindowStaysOnTopHint);

  /* fill state map */
  m_statesMap =
  {
    {ui->btnAccaunt,    BS_ACCAUNT},
    {ui->btnSettings,   BS_SETTINGS},
    {ui->btnConnection, BS_CONNECTION},
  };

  /* signals */
  connect (ui->btnAccaunt, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnSettings, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnConnection, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);

  /* move to bottom */
  slotMoveToBottom();
}

MenuFooter::~MenuFooter()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

MenuFooter::ButtonState MenuFooter::state() const
{
    return m_state;
}

/********************************************
 * SLOTS
 *******************************************/

void MenuFooter::slotSetProgressValue (int value)
{
  ui->progress->setValue (value);
}

void MenuFooter::slotSetProgressRange (int minimum, int maximum)
{
  ui->progress->setRange (minimum, maximum);
}

void MenuFooter::slotSetButtonState (MenuFooter::ButtonState state)
{
  m_lock = true;

  /* store state */
  if (state != BS_NONE)
    m_state = state;

  /* setup buttons */
  ui->btnSettings->setChecked (m_state == BS_SETTINGS);
  ui->btnConnection->setChecked (m_state == BS_CONNECTION);
  ui->btnAccaunt->setChecked (m_state == BS_ACCAUNT);

  m_lock = false;
}

void MenuFooter::slotMoveToBottom()
{
  raise();
//#ifndef Q_OS_ANDROID
  if (auto p = dynamic_cast<QWidget *> (parent()))
    {
      auto hh     = UiScaling::pointsToPixels (HEIGHT, UiScaling::getNativDPI());
      auto posY   = p->height() - hh;
      move (0, posY);
      //resize (p->width(), hh);
      setMinimumSize (p->width(), hh);
      setMaximumSize (p->width(), hh);
      ui->Background->setMinimumSize (p->width(), hh);
      ui->Background->setMaximumSize (p->width(), hh);
      qDebug() << __PRETTY_FUNCTION__ << "y:" << posY << ",screen:" << p->width() << p->height() << height();
    }
//#else // Q_OS_ANDROID
//  auto screen = QApplication::screens().first();
//  auto height = UiScaling::pointsToPixels(HEIGHT, UiScaling::getNativDPI());
//  auto posY   = screen->size().height() - height;
//  move (0, posY);
//  resize (screen->size().width(), height);
//  qDebug() << __PRETTY_FUNCTION__ << "y:" << posY << ",screen:" << screen->size().width() << screen->size().height() << height;
  //#endif // Q_OS_ANDROID
}

void MenuFooter::slotResizeEvent(QSize a_oldSize, QSize a_newSize)
{
  qDebug() << __PRETTY_FUNCTION__ << a_oldSize << a_newSize;
  Q_UNUSED (a_oldSize)
  Q_UNUSED (a_newSize)
  slotMoveToBottom();
}

void MenuFooter::slotButtonToggled (bool checked)
{
  /* ignore if locked (prevent recursion) */
  if (m_lock)
    return;

  /* if unset */
  if (checked == false)
    return slotSetButtonState (BS_NONE);

  /* if set, find in map and setup */
  auto state = m_statesMap.value (sender(), BS_NONE);
  slotSetButtonState (state);
  emit sigStateChanged (state);
}

/*-----------------------------------------*/
