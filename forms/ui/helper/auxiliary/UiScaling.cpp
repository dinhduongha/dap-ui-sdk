#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QScreen>

#ifdef Q_OS_WIN
#include "windows.h"
const GUID GUID_CLASS_MONITOR = { 0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 };
#elif defined(Q_OS_LINUX)
#ifndef Q_OS_ANDROID
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif
#elif defined(Q_OS_MACOS)
#include <CoreGraphics/CoreGraphics.h>
#endif

/* VARS */
static float s_DPI = 0.0;
static QSize s_resolution;

float UiScaling::pointsToPixels (float a_pointsValue, float dpi /*default = 0*/)
{
#ifndef Q_OS_ANDROID
  if (!dpi)
    dpi = getNativDPI();
  float valueInPixels = dpi * pointsToInches (a_pointsValue);//(aptToPt (a_pointsValue));
#else
//  Q_UNUSED (dpi)
//  static qreal dpi_Android (QGuiApplication::primaryScreen()->geometry().width());

  if (a_pointsValue == 428 && s_resolution.width() > 0)
    {
      qDebug() << "full width requested:" << s_resolution.width();
      return s_resolution.width();
    }

//  float valueInPixels = dpi_Android * a_pointsValue / 360.f;
  if (!dpi)
    dpi = getNativDPI();

  float valueInPixels = dpi * pointsToInches (a_pointsValue);
#endif
  return valueInPixels;
}

QSize UiScaling::pointsToPixels (const QSize &a_pointsSize)
{
  return QSize (UiScaling::pointsToPixels (a_pointsSize.width()),
                UiScaling::pointsToPixels (a_pointsSize.height()));
}

inline double UiScaling::pointsToInches (float a_pointsValue)
{
  return a_pointsValue / 160.f;
}

float UiScaling::getNativDPI()
{
  if (s_DPI != 0.0)
    return s_DPI;

#ifdef Q_OS_WIN
  HDC screen = GetDC (NULL);
  int hSize       = GetDeviceCaps (screen, HORZSIZE);
  int wSize       = GetDeviceCaps (screen, VERTSIZE);
  int hResolution = GetDeviceCaps (screen, HORZRES);
  int wResolution = GetDeviceCaps (screen, VERTRES);
#elif defined(Q_OS_LINUX)
#ifndef Q_OS_ANDROID
  Display *dpy;
  XRRScreenResources *screen;
  XRROutputInfo *outInfo;
  XRRCrtcInfo *crtc_info;

  dpy = XOpenDisplay (":0");
  screen = XRRGetScreenResources (dpy, DefaultRootWindow (dpy));
  crtc_info = XRRGetCrtcInfo (dpy, screen, screen->crtcs[0]); //0 to get the first monitor
  outInfo = XRRGetOutputInfo (dpy, screen, *crtc_info->outputs);

  int hSize       = crtc_info->height;
  int wSize       = crtc_info->width;
  int hResolution = outInfo->mm_height;
  int wResolution = outInfo->mm_width;
#else
  QScreen *screen = QApplication::primaryScreen();
  int hSize       = screen->physicalSize().height();
  int wSize       = screen->physicalSize().width();
  int hResolution = screen->size().height();
  int wResolution = screen->size().width();
  qDebug() << __PRETTY_FUNCTION__ << "physical: " << wSize << hSize << " virtual: " << wResolution << hResolution;
#endif
#elif defined(Q_OS_MACOS)
  auto mainDisplayId = CGMainDisplayID();
  CGSize screenSize = CGDisplayScreenSize (mainDisplayId);

  int hSize       = screenSize.height;
  int wSize       = screenSize.width;
  int hResolution = CGDisplayPixelsHigh (mainDisplayId);
  int wResolution = CGDisplayPixelsWide (mainDisplayId);
#else
  static qreal dpi (QGuiApplication::primaryScreen()->physicalDotsPerInch());
#endif
//#ifndef Q_OS_ANDROID
  float pixelsPerMM = ((float)wResolution / wSize + (float)hResolution / hSize) / 2;
  float dpi = pixelsPerMM * 25.4f;
  qInfo() << QString ("UiScaling - Pixels pre mm: %1 Resolution: %2x%3 Screen size: %4x%5 dpi: %6 According to qt: %7")
          .arg (pixelsPerMM).arg (hResolution).arg (wResolution).arg (hSize).arg (wSize).arg (dpi)
          .arg (QGuiApplication::primaryScreen()->physicalDotsPerInch()); //for statistic, delete later
  s_DPI = ((dpi < 50 || dpi > 350) ? QGuiApplication::primaryScreen()->physicalDotsPerInch() : dpi);
  s_resolution  = QSize (wResolution, hResolution);

#ifndef Q_OS_ANDROID
  // lock low dpi to 128 minimum
  if (s_DPI < 128)
    s_DPI = 128;
#endif // Q_OS_ANDROID

  return s_DPI;
//#else
//  return 1;
//#endif
}

void UiScaling::setPseudoDPI (const float &pseudoDPI)
{
  s_DPI = pseudoDPI;
}

QSize UiScaling::deviceResolution()
{
  return s_resolution;
}

float UiScaling::aptToPt (float apt)
{
  /*CSS has its own pt unit, which is 1.333 (96/72).
   times bigger than px however it is different from iOS p.*/
  //1.270f - relatively window Mira
  return apt * 1.270f;
}
