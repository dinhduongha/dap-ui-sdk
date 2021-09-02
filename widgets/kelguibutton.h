#ifndef KELGUIBUTTON_H
#define KELGUIBUTTON_H

/* INCLUDES */
#include <QWidget>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QUrl>
#include <QMap>
#include <memory>

/* DEFS */
namespace Ui { class KelGuiButtonUI; };

/****************************************//**
 * @brief framed button with customiseable style
 * @ingroup groupKelGuiWidgets
 * @date 19.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiButton : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum ButtonStyle
  {
    TopMainBottomSub,     ///< top:main, bottom:sub
    TopSubBottomMain,     ///< top:sub,  bottom:main
    LeftTopMainBottomSub, ///< left:left, top:main, bottom:sub
    IconMainSub,          ///< icon, main, sub
  };
  Q_ENUM(ButtonStyle)
  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (ButtonStyle buttonStyle READ btnStyle     WRITE setBtnStyle)
  Q_PROPERTY (QString mainText        READ mainText     WRITE setMainText)
  Q_PROPERTY (QString subText         READ subText      WRITE setSubText)
  Q_PROPERTY (QString leftText        READ leftText     WRITE setLeftText)
  Q_PROPERTY (QString mainCssClass    READ mainCssClass WRITE setMainCssClass)
  Q_PROPERTY (QString subCssClass     READ subCssClass  WRITE setSubCssClass)
  Q_PROPERTY (QString leftCssClass    READ leftCssClass WRITE setLeftCssClass)
  Q_PROPERTY (bool separator          READ separator    WRITE setSeparator)
  Q_PROPERTY (bool link               READ link         WRITE setLink)
  Q_PROPERTY (bool frame              READ frame        WRITE setFrame)
  Q_PROPERTY (QUrl icon               READ icon         WRITE setIcon)

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::KelGuiButtonUI *ui;
  ButtonStyle m_btnStyle; ///< button style
  QString m_mainText;
  QString m_subText;
  QString m_leftText;
  QString m_mainCssClass;
  QString m_subCssClass;
  QString m_leftCssClass;
  bool m_separator;       ///< bottom separator
  bool m_link;            ///< link button
  bool m_frame;           ///< show frame
  QUrl m_icon;            ///< icon url

  QGraphicsDropShadowEffect *m_effect; ///< @note attached widget will take control. this means instance doesnt need to be deleted
  QMultiMap<QString,QWidget*> m_widgets; ///< categorized map for widget

  /// labels
  std::unique_ptr<QLabel> m_lLink;
//  std::unique_ptr<QLabel> m_lMain,
//      m_lSub,
//      m_lLeft,
//      m_lLink,
//      m_lIcon;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiButton (QWidget *parent = 0);
  ~KelGuiButton() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  ButtonStyle btnStyle() const;
  void setBtnStyle(const ButtonStyle &style);

  QString mainText() const;
  void setMainText (const QString &mainText);

  QString subText() const;
  void setSubText (const QString &subText);

  QString leftText() const;
  void setLeftText (const QString &leftText);

  QString mainCssClass() const;
  void setMainCssClass(const QString &mainStyle);

  QString subCssClass() const;
  void setSubCssClass(const QString &subStyle);

  QString leftCssClass() const;
  void setLeftCssClass(const QString &leftStyle);

  bool link() const;
  void setLink (bool link);

  bool frame() const;
  void setFrame (bool frame);

  bool showIcon() const;
  void setShowIcon(bool showIcon);

  QUrl icon() const;
  void setIcon(const QUrl &icon);

  bool separator() const;
  void setSeparator(bool separator);

  void setupStyle();
  void setupLabels();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paintEvent (QPaintEvent *) override;
  void mousePressEvent (QMouseEvent *) override;
  void resizeEvent (QResizeEvent *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();         ///< button is clicked
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIBUTTON_H
