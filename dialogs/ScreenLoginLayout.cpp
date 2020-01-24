#include "ScreenLoginLayout.h"

/**
 * @brief Overloaded constructor.
 * @param a_parent Parent.
 * @param a_sw Application window stack.
 */
ScreenLoginLayout::ScreenLoginLayout(QObject *a_parent, QStackedWidget *a_sw)
        :DapUIAnimationScreenAbstract(a_parent, a_sw)
{
    create<Ui::LoginLayout>();
}

/**
 * @brief Form initialization.
 * @param a_w Window GUI widget.
 * @param a_rotation Device display orientation.
 */
void ScreenLoginLayout::initUi(QWidget *a_w, ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation);

    initChangedScreen(a_w);

    //create and activate ScreenLogin
    m_screenLogin = activateScreen<StartScreen>();
}

/**
 * @brief Getter for m_screenLogin
 * @return Login screen
 */
StartScreen *ScreenLoginLayout::screenLogin() const
{
    return m_screenLogin;
}







