#pragma once

#include <QString>
#include <QDebug>
#include <QPoint>
#include <QRect>
#include <memory>
#include <optional>
#include <vector>

namespace utils {
    class Logger {
        public:
            enum class Level {
                Debug,
                Info,
                Warning,
                Error,
                Critical
            };

            static void init(const QString& logPath);
            static void log(Level level, const QString& message);
            static void setLogLevel(Level level);

        private:
            static Level s_currentLevel;
            static QString s_logPath;
    };    

    namespace Screen {
        QPoint ensurePointInScreen(const QPoint& point);
        QRect getScreenFromPoint(const QPoint& point);
        bool isPointInScreen(const QPoint& point);
        double getScreenScaleFactor();
    }

    namespace File {
        bool ensureDirectoryExists(const QString& path);
        bool ensureFileExists(const QString& path);
        QString getAppDataPath();
        QString sanitizeFileNmae(const QString& name);
        std::optional<QString> getFileName(const QString& path);
        std::optional<QString> readFileContent(const QString& path);
        bool writeFileContent(const QString& path, const QString& content);
    }

    namespace String {
        QString normalize(const QString& string);
        bool isValidWord(const QString& string);
        std::vector<QString> splitIntoWords(const QString& string);
        QString removeDiacriticts(const QString& string);
        QString removeNonAlphanumeric(const QString& string);
        QString toLower(const QString& string);
        bool fuzzyMatch(const QString& pattern, const QString& string);
    }

    namespace Math{ 
        template <typename T>
        T clamp(T value, T min, T max){
            return std::max(min, std::min(value, max));
        }

        float calculateDistance(const QPoint& point1, const QPoint& point2);
        QPoint findClosestPoint(const QPoint& target, const std::vector<QPoint>& points);
    }

    namespace Memory{
        template <typename T>
        using UniquePtr = std::unique_ptr<T>;
        template <typename T>
        using SharedPtr = std::shared_ptr<T>;
        template <typename T>
        using WeakPtr = std::weak_ptr<T>;
    }

    template <typename T, typename E = QString>
    class Result{
        public:
            static Result<T, E> ok(T value){
                return Result<T, E>(value);
            }

            static Result<T, E> error(E error){
                return Result<T, E>(error);
            }

            bool isOk() const{return m_isOk;}
            bool isError() const{return !m_isOk;}

            const T& value() const {return m_value;}
            const E& error() const {return m_error;}

        private:
            Result(const T& value): m_value(value), m_isOk(true){}
            Result(const E& error): m_error(error), m_isOk(false){}

            T m_value;
            E m_error;
            bool m_isOk;
    };

    //Performance
    class ScopedTimer{
        public:
            explicit ScopedTimer(const QString& operation);
            ~ScopedTimer();
        
        private:
            QString m_operation;
            std::chrono::high_resolution_clock::time_point m_start;
    };
} //namespace utils

#define LOG_DEBUG(message) utils::Logger::log(utils::Logger::Level::Debug, message)
#define LOG_INFO(message) utils::Logger::log(utils::Logger::Level::Info, message)
#define LOG_WARNING(message) utils::Logger::log(utils::Logger::Level::Warning, message)
#define LOG_ERROR(message) utils::Logger::log(utils::Logger::Level::Error, message)
#define LOG_CRITICAL(message) utils::Logger::log(utils::Logger::Level::Critical, message)

#define MEASURE_SCOPE(name) utils::ScopedTimer timer(name)

