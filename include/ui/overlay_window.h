#pragma once

#include <QWidget>
#include <QPixmap>

class OverlayWindow : public QWidget
{
    Q_OBJECT
public:
    explicit OverlayWindow(QWidget* parent = nullptr);
    void showScreenshot(const QPixmap& screenshot);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QPixmap m_screenshot;
    static constexpr int BORDER_WIDTH = 2;
};