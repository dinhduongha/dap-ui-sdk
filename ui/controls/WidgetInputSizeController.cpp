#include "WidgetInputSizeController.h"

WidgetInputSizeController::WidgetInputSizeController(QObject *a_parent)
    :QObject (a_parent)
{
    connect(QApplication::inputMethod(),&QInputMethod::keyboardRectangleChanged,this,&WidgetInputSizeController::setVisibilityWidgetDependingOnStateKeyboard);
}

WidgetInputSizeController::~WidgetInputSizeController()
{
    disconnect(QApplication::inputMethod(),&QInputMethod::keyboardRectangleChanged,this,&WidgetInputSizeController::setVisibilityWidgetDependingOnStateKeyboard);
}

void WidgetInputSizeController::setVisibilityWidgetDependingOnStateKeyboard()
{
    if(QApplication::inputMethod()->isVisible() && QApplication::inputMethod()->keyboardRectangle().height() == 0.0)
    {
        setVisibleWidgets(true);

        if(parent()!=nullptr)
            qobject_cast<QWidget*>(parent())->setFocus();
    }
}

void WidgetInputSizeController::addWidgetEmitsSignal(CustomLineEditBase *a_widget)
{
    connect(a_widget,&CustomLineEditBase::focusChanged,this,&WidgetInputSizeController::setVisibleWidgets);
}

void WidgetInputSizeController::addWidgetEmitsSignal(CustomTextEdit *a_widget)
{
    connect(a_widget,&CustomTextEdit::focusChanged,this,&WidgetInputSizeController::setVisibleWidgets);
}

void WidgetInputSizeController::addDisappearingWidget(QWidget *a_widget)
{
    m_disappearingWidget.append(a_widget);
}

void WidgetInputSizeController::setVisibleWidgets(bool a_visible)
{
        for(auto widget:m_disappearingWidget)
        {
            widget->setVisible(a_visible);
        }
}
