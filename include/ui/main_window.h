#pragma once

#include <QMainWindow>
#include "screenshot/screenshot_manager.h"
#include "ui/overlay_window.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    private slots:
        void handleError(const QString& message);

private:
    void setupScreenshotSystem();
    ScreenshotManager* m_screenshotManager;
    OverlayWindow* m_overlayWindow;
};