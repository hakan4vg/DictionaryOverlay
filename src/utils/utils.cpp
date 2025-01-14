#include "utils/utils.h"
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QScreen>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <algorithm>
#include <chrono>
#include <QObject>
#include <QRegularExpression>

namespace Utils
{
    Logger::Level Logger::s_currentLevel = Logger::Level::Info;
    QString Logger::s_logPath;

    void Logger::init(const QString& logPath){
        s_logPath = logPath;
        File::ensureDirectoryExists(QFileInfo(logPath).path());
    }

    void Logger::log(Level level, const QString& message){
        if (level < s_currentLevel) return;

        QFile file(s_logPath);
        if (!file.open(QIODevice::Append | QIODevice::Text)){
            qWarning() << "Failed to open log file for writing";
            return;
        }

        QTextStream stream(&file);
        QString levelStr;
        switch (level){
            case Level::Debug: levelStr = "DEBUG"; break;
            case Level::Info: levelStr = "INFO"; break;
            case Level::Warning: levelStr = "WARNING"; break;
            case Level::Error: levelStr = "ERROR"; break;
            case Level::Critical: levelStr = "CRITICAL"; break;
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        stream << QString("[%1] [%2] %3\n").arg(timestamp, levelStr, message);
    }

    void Logger::setLogLevel(Level level){
        s_currentLevel = level;
    }

    namespace Screen
    {
        QPoint ensurePointInScreen(const QPoint& point, const QRect& screenGeometry){
            return QPoint(
                Math::clamp(point.x(), screenGeometry.left(), screenGeometry.right()),
                Math::clamp(point.y(), screenGeometry.top(), screenGeometry.bottom())
                );
        }

        QRect getScreenFromPoint(const QPoint& point){
            QScreen* screen = QApplication::screenAt(point);
            return screen ? screen->geometry() : QRect();
        }

        bool isPointInScreen(const QPoint& point){
            return QApplication::screenAt(point) != nullptr;
        }
    }


    namespace File
    {
        bool ensureDirectoryExists(const QString& path)
        {
            return QDir().mkpath(path);
        }

        QString getAppDataPath()
        {
            QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            ensureDirectoryExists(path);
            return path;
        }

        QString sanitizeFileName(const QString& fileName)
        {
            QString sanitized = fileName;
            return sanitized.replace(QRegularExpression("[<>:\"/\\|?*]"), "_");
        }

        std::optional<QString> readFileContent(const QString& path)
        {
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return std::nullopt;
            QTextStream stream(&file);
            return stream.readAll();
        }

        bool writeFileContent(const QString& path, const QString& content)
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream stream(&file);
            stream << content;
            return true;
        }
    }


    namespace String
    {
        QString normalize(const QString& input)
        {
            QString normalized = input.normalized(QString::NormalizationForm_D);
            normalized = normalized.toLower().trimmed();
            return normalized;
        }


##TODO : PLACEHOLDER for Word Validation
        bool isValidWord(const QString& word)
        {
            if (word.isEmpty()) return false;

            static const QRegularExpression wordRegex("^[a-zA-Z]+(-[a-zA-Z]+)*$");
            return wordRegex.match(word).hasMatch();
        }

        std::vector<QString> splitIntoWords(const QString& text)
        {
            std::vector<QString> words;
            QString normalized = normalize(text);
            for (const QString& word : normalized.split(QRegularExpression("\\s+")))
                if (isValidWord(word))
                    words.push_back(word);
        }

        QString removeDiacritics(const QString& text) {
            QString normalized = text.normalized(QString::NormalizationForm_D);
            QString result;
            for (const QChar& ch : normalized) {
                if (ch.category() != QChar::Mark_NonSpacing) {
                    result.append(ch);
                }
            }
            return result;
        }
##TODO : PLACEHOLDER for FUZZY MATCH
        float fuzzyMatch(const QString& str1, const QString& str2, float threshold) {
            QString s1 = normalize(str1);
            QString s2 = normalize(str2);

            int distance = 0;
            int maxLen = std::max(s1.length(), s2.length());
            if (maxLen == 0) return 1.0f;

            distance = std::abs(s1.length() - s2.length());
            int minLen = std::min(s1.length(), s2.length());

            for (int i = 0; i < minLen; ++i) {
                if (s1[i] != s2[i]) distance++;
            }

            float similarity = 1.0f - (float)distance / maxLen;
            return similarity;
        }
    }

    namespace Math
    {
        float calculateDistance(const QPoint& point1, const QPoint& point2)
        {
            QPoint diff = point1 - point2;
            return std::sqrt(diff.x() * diff.x() + diff.y() * diff.y());
        }

        QPoint findClosestPoint(const QPoint& target, const std::vector<QPoint>& points)
        {
            if (points.empty()) return QPoint();

            QPoint closest = points[0];
            float minDistance = calculateDistance(target, closest);

            for (const auto& point : points)
            {
                float distance = calculateDistance(target, point);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closest = point;
                }
            }
            return closest;
        }
    }


    ScopedTimer::ScopedTimer(const QString& operation)
        : m_operation(operation)
        , m_start(std::chrono::high_resolution_clock::now())
    {}

    ScopedTimer::~ScopedTimer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
        LOG_DEBUG(QString("%1 took %2 μs").arg(m_operation).arg(duration.count()));
    }
}
