#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QImage>

class ScreenCapture
{
public:

    /**
     * @brief Capture an image from screen.
     * @param copyToClipboard Copy the captured image to clipboard?
     * @return The captured image, may be null image if user cancelled
     * This will show the capture dialog as modal, blocking until the user done capturing.
     * Call this again when previous call is executing will get a null image.
     */
    static QImage capture(bool copyToClipboard = true);
};

#endif // SCREENCAPTURE_H
