#include "screencapture.h"

#include <QObject>
#include "captureeditpanel.h"
#include <QClipboard>
#include <QApplication>

static CaptureEditPanel* PanelInstance;

QImage ScreenCapture::capture(bool copyToClipboard)
{
    if (PanelInstance)
        return QImage();
    CaptureEditPanel panel;
    PanelInstance = &panel;
    panel.exec();
    PanelInstance = nullptr;

    auto img = panel.capturedImage();
    if (!img.isNull() && copyToClipboard)
    {
        auto clipboard = QApplication::clipboard();
        clipboard->setImage(img);
    }
    return img;
}
