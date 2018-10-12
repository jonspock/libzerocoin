// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2018 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// clang-format off
#ifndef BITCOIN_LOGGING_H
#define BITCOIN_LOGGING_H

#include <atomic>
#include <cstdint>
#include <list>
#include <mutex>
#include <string>

static const bool DEFAULT_LOGTIMEMICROS = false;
static const bool DEFAULT_LOGIPS = false;
static const bool DEFAULT_LOGTIMESTAMPS = true;

extern bool fLogIPs;

class Logger {
private:
    FILE *fileout = nullptr;
    std::mutex mutexDebugLog;
    std::list<std::string> vMsgsBeforeOpenLog;

    /**
     * fStartedNewLine is a state variable that will suppress printing of the
     * timestamp when multiple calls are made that don't end in a newline.
     */
    std::atomic_bool fStartedNewLine{true};

    /**
     * Log categories bitfield. Leveldb/libevent need special handling if their
     * flags are changed at runtime.
     */
    std::atomic<uint32_t> logCategories{0};

    std::string LogTimestampStr(const std::string &str);

public:
    bool fPrintToConsole = false;
    bool fPrintToDebugLog = true;

    bool fLogTimestamps = DEFAULT_LOGTIMESTAMPS;
    bool fLogTimeMicros = DEFAULT_LOGTIMEMICROS;

    std::atomic<bool> fReopenDebugLog{false};

    ~Logger();

    /** Send a string to the log output */
    int LogPrintStr(const std::string &str);

    void OpenDebugLog();
};


Logger &GetLogger();

#define LogPrint(category, ...)                                                \
    do {                                                                       \
      GetLogger().LogPrintStr(tfm::format(__VA_ARGS__));                \
    } while (0)

#define LogPrintf(...)                                                         \
    do {                                                                       \
        GetLogger().LogPrintStr(tfm::format(__VA_ARGS__));                     \
    } while (0)

#endif // BITCOIN_LOGGING_H

// clang-format off
