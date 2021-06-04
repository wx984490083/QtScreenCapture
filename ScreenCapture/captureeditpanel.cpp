#include "captureeditpanel.h"
#include <QDesktopWidget>
#include <QScreen>
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

static QRect RectForPoints(const QPoint& p1, const QPoint& p2)
{
    auto w = std::minmax(p1.x(), p2.x());
    auto h = std::minmax(p1.y(), p2.y());
    return {w.first, h.first, w.second - w.first + 1, h.second - h.first + 1};
}

class ToolContainer : public QWidget
{
public:
    ToolContainer(QWidget* parent) : QWidget(parent)
    {
        setAttribute(Qt::WA_StyledBackground);
        setStyleSheet(R"(
                      border-radius: 19;
                      background-color: white;
                      )");

        resize(96, 38);

        auto effect = new QGraphicsDropShadowEffect(this);
        effect->setBlurRadius(16);
        effect->setOffset(0,0);
        setGraphicsEffect(effect);

        btnSave = new QPushButton(this);
        btnSave->setGeometry(width() - 20 - 20, (height() - 20) / 2, 20, 20);
        btnSave->setIcon(QIcon(QStringLiteral(":/capture/save.svg")));

        btnCancel = new QPushButton(this);
        btnCancel->setGeometry(20, (height() - 20) / 2, 20, 20);
        btnCancel->setIcon(QIcon(QStringLiteral(":/capture/cancel.svg")));
    }

    void mousePressEvent(QMouseEvent* event)
    {
        event->accept();
    }
    void mouseReleaseEvent(QMouseEvent* event)
    {
        event->accept();
    }

    QPushButton* btnSave;
    QPushButton* btnCancel;
};

class CaptureEditPanelPriv
{
public:
    CaptureEditPanel* owner;

    QPixmap pm;
    QImage result;

    ToolContainer* toolContainer;

    bool isPressed = false;
    QPoint pressPos;

    QRect selectionRect;

    QPoint bestContainerPos()
    {
        static const int spacing = 4;
        int x = selectionRect.right() + 1 - toolContainer->width();
        int y = 0;
        if (owner->height() - selectionRect.bottom() - 1 >= toolContainer->height() + spacing)
        {
            y = selectionRect.bottom() + 1 + spacing;
        }
        else if (selectionRect.top() >= toolContainer->height() + spacing)
        {
            y = selectionRect.top() - toolContainer->height() - spacing;
        }
        else
        {
            y = owner->height() - toolContainer->height();
        }
        return {x, y};
    }

    void generateResultImage()
    {
        selectionRect.setLeft(qBound(0, selectionRect.left(), owner->width() - 1));
        selectionRect.setTop(qBound(0, selectionRect.top(), owner->height() - 1));
        selectionRect.setRight(qBound(0, selectionRect.right(), owner->width() - 1));
        selectionRect.setBottom(qBound(0, selectionRect.bottom(), owner->height() - 1));

        if (selectionRect.isEmpty())
            return;

        QImage img(selectionRect.size(), QImage::Format_ARGB32);
        {
            QPainter p(&img);
            p.drawPixmap(0, 0, selectionRect.width(), selectionRect.height(), pm, selectionRect.x(), selectionRect.y(), selectionRect.width(), selectionRect.height());
        }
        result = img;
    }
};


CaptureEditPanel::CaptureEditPanel(QWidget *parent) : QDialog(parent), priv(new CaptureEditPanelPriv)
{
    priv->owner = this;

    auto screen = QApplication::primaryScreen();
    auto desktop = QApplication::desktop();

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setGeometry(0, 0, desktop->width(), desktop->height());
    priv->pm = screen->grabWindow(desktop->winId());

    priv->toolContainer = new ToolContainer(this);
    priv->toolContainer->hide();

    QObject::connect(priv->toolContainer->btnSave, &QPushButton::clicked, this, [=]
    {
        priv->generateResultImage();
        done(0);
    });

    QObject::connect(priv->toolContainer->btnCancel, &QPushButton::clicked, this, [=]
    {
        done(0);
    });
}

CaptureEditPanel::~CaptureEditPanel()
{
    delete priv;
}

QImage CaptureEditPanel::capturedImage()
{
    return priv->result;
}

void CaptureEditPanel::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // draw captured pixmap
    p.drawPixmap(0, 0, priv->pm);

    // draw shadow out of selected rect
    auto leftRect = QRect{0, 0, priv->selectionRect.left(), height()};
    auto rightRect = QRect{priv->selectionRect.right() + 1, 0, width() - priv->selectionRect.right() - 1, height()};
    auto upRect = QRect{priv->selectionRect.left(), 0, priv->selectionRect.width(), priv->selectionRect.top()};
    auto downRect = QRect{priv->selectionRect.left(), priv->selectionRect.bottom() + 1, priv->selectionRect.width(), height() - priv->selectionRect.bottom() - 1};
    auto shadow = QColor("#80000000");
    p.fillRect(leftRect, shadow);
    p.fillRect(rightRect, shadow);
    p.fillRect(upRect, shadow);
    p.fillRect(downRect, shadow);

    // draw a border for selected rect
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor("#0066ed"),1 ));
    p.drawRect(priv->selectionRect);
}

void CaptureEditPanel::showEvent(QShowEvent *)
{
    raise();
    activateWindow();
    setFocus();
}

void CaptureEditPanel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        priv->isPressed = true;
        priv->pressPos = event->pos();
        priv->selectionRect = QRect(event->pos(), event->pos());
        update();
        priv->toolContainer->hide();
    }
}

void CaptureEditPanel::mouseReleaseEvent(QMouseEvent*)
{
    auto bestPos = priv->bestContainerPos();
    priv->toolContainer->move(bestPos);
    priv->toolContainer->show();
}

void CaptureEditPanel::mouseMoveEvent(QMouseEvent* event)
{
    if (priv->isPressed)
    {
        auto pos = event->pos();
        priv->selectionRect = RectForPoints(pos, priv->pressPos);
        update();
    }
}
