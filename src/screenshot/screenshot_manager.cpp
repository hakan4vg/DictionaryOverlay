#include "screenshot/screenshot_manager.h"
#include <QApplication>
#include <QKeySequence>
#include <QScreen>
#include <QWindow>
#include <QDebug>
#include <QGuiApplication>
#include "utils/utils.h"

#ifdef Q_OS_WIN
ScreenshotManager* ScreenshotManager::instance = nullptr;
#endif


ScreenshotManager::ScreenshotManager(QObject* parent)
    : QObject(parent)
    , m_hotkeyRegistered(false)
{
#ifdef Q_OS_WIN
    instance = this;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"ScreenshotManagerClass";

    RegisterClassEx(&wc);

    m_hwnd = CreateWindow(
        L"ScreenshotManagerClass",
        L"ScreenshotManager",
        WS_OVERLAPPED,
        0, 0, 0, 0,
        HWND_MESSAGE,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
#endif
}

ScreenshotManager::~ScreenshotManager()
{
    unregisterHotkey();
#ifdef Q_OS_WIN
    if (m_hwnd){
        DestroyWindow(m_hwnd);
    }
    UnregisterClass(L"ScreenshotManagerClass", GetModuleHandle(nullptr));
    instance = nullptr;
#endif
}

bool ScreenshotManager::initialize() {
    return registerHotkey();
}

void ScreenshotManager::setHotkey(const QKeySequence& sequence) {
    if (m_currentSequence == sequence) return;

    unregisterHotkey();
    m_currentSequence = sequence;
    registerHotkey();
}

#ifdef Q_OS_WIN
LRESULT CALLBACK ScreenshotManager::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (msg == WM_HOTKEY && instance) {
        instance->handleHotkeyTriggered();
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
#endif


bool ScreenshotManager::registerHotkey() {
    if (m_hotkeyRegistered) {
        unregisterHotkey();
    }

#ifdef Q_OS_WIN
    // Convert Qt key sequence to Windows virtual key code and modifiers
    if (m_currentSequence.isEmpty()) {
        m_currentSequence = QKeySequence("Ctrl+Alt+S"); // Default hotkey
    }

    int key = m_currentSequence[0] & 0x01FFFFFF;
    UINT modifiers = 0;
    if (m_currentSequence[0] & Qt::CTRL) modifiers |= MOD_CONTROL;
    if (m_currentSequence[0] & Qt::ALT) modifiers |= MOD_ALT;
    if (m_currentSequence[0] & Qt::SHIFT) modifiers |= MOD_SHIFT;

    if (!RegisterHotKey(m_hwnd, 1, modifiers, key)) {
        emit error("Failed to register hotkey");
        return false;
    }

    m_hotkeyRegistered = true;
    return true;
#else
##TODO : Implement for other platforms (Linux, macOS)
    // For Linux, X11 or DBus
    // For macOS, Carbon Hot Keys API
    emit error("Global hotkeys not implemented for this platform");
    return false;
#endif
}

void ScreenshotManager::unregisterHotkey()
{
    if (!m_hotkeyRegistered) return;
#ifdef Q_OS_WIN
    UnregisterHotKey(m_hwnd, 1);
#else
##todo : Implement for other platforms (Linux, macOS)
#endif
    m_hotkeyRegistered = false;
}

void ScreenshotManager::handleHotkeyTriggered()
{
    QPixmap screenshot = captureFullScreen();
    if (!screenshot.isNull())
    {
        emit screenshotCaptured(screenshot);
    } else
    {
        emit error("Failed to capture screenshot");
    }
}

QPixmap ScreenshotManager::captureFullScreen()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen)
    {
        return {};
    }
    return screen->grabWindow(0);
}