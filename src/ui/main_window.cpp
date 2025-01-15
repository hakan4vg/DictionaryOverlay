#include "ui/main_window.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_screenshotManager(nullptr)
    , m_overlayWindow(nullptr)
{
    setupScreenshotSystem();
    hide();
}

MainWindow::~MainWindow() {
    delete m_overlayWindow;
}

void MainWindow::setupScreenshotSystem() {
    m_screenshotManager = new ScreenshotManager(this);
    m_overlayWindow = new OverlayWindow(nullptr);

    connect(m_screenshotManager, &ScreenshotManager::screenshotCaptured,
            m_overlayWindow, &OverlayWindow::showScreenshot);
    connect(m_screenshotManager, &ScreenshotManager::error,
            this, &MainWindow::handleError);

    if (!m_screenshotManager->initialize()) {
        handleError("Failed to initialize screenshot system");
    }
}

void MainWindow::handleError(const QString& message) {
    QMessageBox::critical(this, "Error", message);
}
