//
// Created by xmrhk on 14.01.2025.
//
#include "ui/overlay_window.h"
#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QScreen>

OverlayWindow::OverlayWindow(QWidget* parent): QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                        Qt::WindowStaysOnTopHint|
                        Qt::Tool);

    setFocusPolicy(Qt::StrongFocus);
}

void OverlayWindow::showScreenshot(const QPixmap& screenshot)
{
    m_screenshot = screenshot;
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        setGeometry(screen->geometry());
    }

    showFullScreen();
    activateWindow();
    update();
}

void OverlayWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_screenshot.isNull())
    {
        painter.drawPixmap(rect(), m_screenshot);
    }

    painter.setPen(QPen(Qt::white, BORDER_WIDTH));
    painter.drawRect(rect().adjusted(BORDER_WIDTH/2, BORDER_WIDTH/2,
                                        -BORDER_WIDTH/2, -BORDER_WIDTH/2));
}

void OverlayWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hide();
        event->accept();
    } else
    {
        QWidget::keyPressEvent(event);
    }
}