/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorBacklog.cpp
 *  Last Change: Automatically updated
 */

#include "vantorBacklog.h"

namespace vantor::Backlog
{

    static std::mutex            logMutex;
    static std::vector<LogEntry> logEntries;

    // Convert LogLevel to String
    std::string LogLevelToString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERR:
                return "ERROR";
        }
        return "UNKNOWN";
    }

    // Logging function
    void Log(const std::string &source, const std::string &msg, LogLevel level)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        LogEntry entry = {source, msg, level};
        logEntries.push_back(entry);

        // Print to console
        std::cout << "[" << LogLevelToString(level) << "::" << source << "] " << msg << std::endl;
    }

    // Save Logs to File
    void SaveLogs(const std::string &filename)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream               logFile(filename, std::ios::app);

        if (!logFile)
        {
            Log("Backlog", "Failed to open log file", LogLevel::ERR);
            return;
        }

        for (const auto &entry : logEntries)
        {
            logFile << "[" << LogLevelToString(entry.level) << "::" << entry.source << "] " << entry.text << std::endl;
        }
        logFile.close();
    }

} // namespace vantor::Backlog
