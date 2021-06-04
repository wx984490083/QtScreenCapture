#ifndef CAPTUREEDITPANEL_H
#define CAPTUREEDITPANEL_H

#include <QDialog>

class CaptureEditPanel : public QDialog
{
    Q_OBJECT
public:
    explicit CaptureEditPanel(QWidget *parent = nullptr);
    ~CaptureEditPanel();

    QImage capturedImage();

protected:
    void paintEvent(QPaintEvent*) override;
    void showEvent(QShowEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
private:
    class CaptureEditPanelPriv* priv;
};

#endif // CAPTUREEDITPANEL_H
