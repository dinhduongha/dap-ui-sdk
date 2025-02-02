#include "CustomButtonDelegate.h"

#include "defines.h"
#include "Utilz.h"

CustomButtonDelegate::CustomButtonDelegate(QWidget* a_parent)
    : WidgetDelegateBase(a_parent)
    , m_button(new CustomPlacementButton(this))
{
    this          ->setObjectName("frmCustomButtonDelegate");
    m_button      ->setObjectName("btnCustomButton");

    this->layout()->addWidget(m_button);
    layout()->setMargin(0);
    layout()->setSpacing(0);

    connect(m_button, &QPushButton::clicked, [this]{
        emit this->selected();
    });
}



void CustomButtonDelegate::setData(const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
        m_button->setText(value.toString());
        break;
    case Qt::DecorationRole:
    case Qt::UserRole:
    case COUNTRY_FLAG_ROLE:
            m_button->setIcon(value.toString());
            m_button->image()->setScaledContents(true);
        break;
    default:
        qDebug() << "Wrong role";
        break;
    }
}

CustomPlacementButton *CustomButtonDelegate::button()
{
    return m_button;
}

void CustomButtonDelegate::setSelected(bool a_selected)
{
    this->WidgetDelegateBase::setSelected(a_selected);

    m_button->setProperty(Properties::SELECTED, a_selected);
}

WidgetDelegateBase *CustomButtonDelegate::create()
{
    return new CustomButtonDelegate;
}
