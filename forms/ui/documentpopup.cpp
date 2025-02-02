/* INCLUDES */
#include "documentpopup.h"
#include "ui_documentpopup.h"
#include "dapguicommon.h"
#include <QScroller>
#include <QScrollBar>
#include "helper/document.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DocumentPopup::DocumentPopup (Type t, QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::DocumentPopup),
  m_type (t),
  m_updated (false)
{
  ui->setupUi (this);

  m_layout  = new QVBoxLayout (ui->scrollArea->widget());
  m_label   = new DapGuiLabel;
  m_filter  = new Filter;
  auto *sp  = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  ui->scrollArea->widget()->setLayout(m_layout);
#ifndef Q_OS_ANDROID
  ui->scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
#else // Q_OS_ANDROID
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
  ui->scrollArea->widget()->installEventFilter (this);
#endif // Q_OS_ANDROID

  m_layout->addWidget (m_label);
  m_layout->addItem (sp);

  m_label->setObjectName ("DocumentContents");
  m_label->setCssStyle ("darkblue font16 normal");
  m_label->setWordWrap (true);

  switch (t)
    {
    case TermsOfUse: slotShowTermsOfUse(); break;
    case PrivacyPolicy: slotShowPrivacyPolicy(); break;
    }

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &DocumentPopup:: sigReturn,
           Qt::QueuedConnection);
}

DocumentPopup::~DocumentPopup()
{
  delete ui;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void DocumentPopup::slotShowTermsOfUse()
{
  m_type  = TermsOfUse;
  setObjectName (tr ("Terms of use"));
  ui->label->setText (tr ("Terms of use"));
  //m_label->setText (docTerms);//(Common::fromFile (":/gui/data/terms.html"));
}

void DocumentPopup::slotShowPrivacyPolicy()
{
  m_type  = PrivacyPolicy;
  setObjectName (tr ("Privacy policy"));
  ui->label->setText (tr ("Privacy policy"));
  //m_label->setText (docPrivacy);//(Common::fromFile (":/gui/data/privacy.html"));
}

void DocumentPopup::slotRetranslated()
{
  switch (m_type)
  {
    case PrivacyPolicy: return slotShowPrivacyPolicy();
    case TermsOfUse:    return slotShowTermsOfUse();
  }
  m_updated = false;
  slotUpdateContents();
}

void DocumentPopup::slotUpdateContents()
{
  if (m_updated)
    return;

  m_updated = true;

  switch (m_type)
    {
    case TermsOfUse:    m_label->setText (docTerms); break;
    case PrivacyPolicy: m_label->setText (docPrivacy); break;
    }
}

bool DocumentPopup::eventFilter(QObject *o, QEvent *e)
{
  // This works because QScrollArea::setWidget installs an eventFilter on the widget
  if(o && o == ui->scrollArea->widget() && e->type() == QEvent::Resize)
    setMinimumWidth (
      ui->scrollArea->widget()->minimumSizeHint().width()
      + ui->scrollArea->verticalScrollBar()->width());

  return m_filter->eventFilter (o, e);
}

/*-----------------------------------------*/

bool DocumentPopup::Filter::eventFilter(QObject *o, QEvent *e)
{
  return QScrollArea::eventFilter(o, e);
}

/*-----------------------------------------*/
