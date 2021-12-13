/* INCLUDES */
#include "popupdialog.h"
#include "ui_popupdialog.h"

#include <QGraphicsBlurEffect>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PopupDialog::PopupDialog (QWidget *parent) :
  QWidget (parent),
  ui (new Ui::PopupDialog),
  m_parent (nullptr),
  m_shadow (new QWidget)
{
  ui->setupUi (this);

  connect (ui->btnYes, &DapGuiLabel::clicked,
           this, &PopupDialog::sigYes,
           Qt::QueuedConnection);
  connect (ui->btnNo, &DapGuiLabel::clicked,
           this, &PopupDialog::sigNo,
           Qt::QueuedConnection);

  connect (ui->btnYes, &DapGuiLabel::clicked,
           this, &PopupDialog::_slotButtonPress,
           Qt::QueuedConnection);
  connect (ui->btnNo, &DapGuiLabel::clicked,
           this, &PopupDialog::_slotButtonPress,
           Qt::QueuedConnection);

  m_shadow->setStyleSheet (
    "background-color: rgba(0,0,0,0.5);"
  );
}

PopupDialog::~PopupDialog()
{
  delete ui;
}


/********************************************
 * SLOTS
 *******************************************/

void PopupDialog::slotShow (QWidget *a_parent, const QString &a_title, const QString &a_description)
{
  /* create blur effect */
  QGraphicsBlurEffect *p_blur = new QGraphicsBlurEffect;
  p_blur->setBlurRadius (5);
  p_blur->setBlurHints (QGraphicsBlurEffect::PerformanceHint);

  /* setup effect and disable parent */
  m_parent  = a_parent;
  m_parent->setGraphicsEffect (p_blur);
  m_parent->setDisabled (true);

  /* attach shadow widget */
  m_shadow->setParent (a_parent);
  m_shadow->resize (a_parent->size());
  m_shadow->move (0,0);
  m_shadow->show();

  /* centering */
  setParent (a_parent->parentWidget());
  move (a_parent->width() / 2 - width() / 2,
        a_parent->height() / 2 - height() / 2);

  /* setup text */
  ui->lTitle->setText (a_title);
  ui->lDescription->setText (a_description);

  /* display */
  show();
}

void PopupDialog::_slotButtonPress()
{
  /* delete effect and enable parent wodget */
  m_parent->setGraphicsEffect (nullptr);
  m_parent->setEnabled (true);

  /* hide shadow overlay and this dialog */
  m_shadow->close();
  close();

  /* unattach shadow overlay and this dialog */
  m_shadow->setParent (nullptr);
  setParent (nullptr);
}

/*-----------------------------------------*/
