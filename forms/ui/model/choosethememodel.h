#ifndef CHOOSETHEMEMODEL_H
#define CHOOSETHEMEMODEL_H

/* INCLUDES */
#include <QStringList>
#include <dapguiradio.h>
#include "modelbase.h"

/* DEFS */
class ChooseTheme;

/****************************************//**
 * @brief choose server model list widget
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseThemeModel : public ModelBase
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QStringList m_array;
  ChooseTheme *m_cs;
  QList<DapGuiRadio*> m_list;
  bool _hook;
  int m_currentIndex;
  QString m_currentText;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ChooseThemeModel (QWidget *parent = nullptr);
  ~ChooseThemeModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  /// set model into this widget
  /// @note old model will not be deleted
  void setColorThemeArray (const QJsonArray &a_array, ChooseTheme *a_cs);
  void setCurrentColorTheme (const QString a_colorTheme);
  const QStringList &array() const;
  QString themeName (const QString a_colorTheme, const QString a_transColorTheme = QString());
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  void slotRetranslate();
private slots:
  /// radio button toggle
  void slotToggled (bool checked);
  /// @}
signals:
  void filled();
};

/*-----------------------------------------*/
#endif // CHOOSETHEMEMODEL_H
