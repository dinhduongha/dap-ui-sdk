/*
* Authors:
* Dmitriy Gerasimov <naeper@demlabs.net>
* Cellframe       https://cellframe.net
* DeM Labs Inc.   https://demlabs.net
* Copyright  (c) 2017-2019
* All rights reserved.

This file is part of CellFrame SDK the open source project

CellFrame SDK is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CellFrame SDK is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with any CellFrame SDK based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MultiScreenAbstract.h"

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
MultiScreenAbstract::MultiScreenAbstract(QWidget *parent)
    :AdaptiveScreen(parent),
    m_wgtChangingScreen(nullptr)
{
}

AdaptiveScreen *MultiScreenAbstract::activeScreen()
{
    QWidget* l_activeWidget = this->wgtChangingScreen()->currentWidget();
    return qobject_cast<AdaptiveScreen*>(l_activeWidget);
}

QString MultiScreenAbstract::activeScreenName()
{
    AdaptiveScreen* activeScreen = this->activeScreen();

    if (!activeScreen)
        return QString();

    return this->activeScreen()->screenName();
}

/**
 * @brief Activate child a_screen. a_screen must exist
 * @param a_screen pointer to child screen
 * @return pointer to this screen (parent)
 */
MultiScreenAbstract *MultiScreenAbstract::activateChildScreen(AdaptiveScreen *a_screen)
{
    Q_ASSERT_X(MultiScreenAbstract::parentMultiscreen(a_screen) == this, "activateDescendantScreen", "screen is not a child of this screen");

    m_wgtChangingScreen->setCurrentWidget(a_screen);

    return (this);
}

MultiScreenAbstract *MultiScreenAbstract::parentMultiscreen(AdaptiveScreen *a_screen)
{
    MultiScreenAbstract *l_parentMultiscr;

    for (QObject *parentObj = a_screen->parent(); parentObj; parentObj = parentObj->parent())
    {
        l_parentMultiscr = qobject_cast<MultiScreenAbstract*>(parentObj);
        if (l_parentMultiscr)
            return l_parentMultiscr;
    }
    return nullptr;
}

/**
 * @brief Getter for m_wgChangingScreen
 * @return pointer to AnimationChangingWidget
 */
AnimationChangingWidget *MultiScreenAbstract::wgtChangingScreen()
{
    return m_wgtChangingScreen;
}


/**
 * @brief Initialize m_wgChangedScreen. Must be called by initUi of inheritor class
 * @param a_widget pointer widget in which will be founded ChangedWidget
 */
void MultiScreenAbstract::initChangedScreen(QWidget *a_widget)
{
    // initialize default a_widget
    if (!a_widget)
        a_widget = variant(ScreenInfo::currentRotation());

    // Initializing pointers to window widgets
    m_wgtChangingScreen = a_widget->findChild<AnimationChangingWidget*>();
    Q_ASSERT(m_wgtChangingScreen);

    connect(m_wgtChangingScreen, &AnimationChangingWidget::animationFinished, [=]{
        emit animationFinished();
    });

}

