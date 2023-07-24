#pragma once
#define LOG_SYSTEM_SAVE
/**
 * @file LoggingSystem.h
 * @author jiyilin 2474803745@qq.com
 * @brief this is a high performance cross platform three level logging system
 * @version 0.1
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief this macro variable is used to control the printing position of the log
 *  if this variable is defined the log will be printed to journal log file otherwise
 *  it will be printed to the console
 */
// #define LOG_SYSTEM_SAVE

namespace LoggingSystem
{
    /**
     * @brief log level
     *
     */
    enum class Grade
    {
        INFO,
        WARM,
        ERROR
    };

    /**
     * @brief save current log level
     *
     */
    extern Grade nowGrade;

    /**
     * @brief log file pointer
     *
     */
    extern std::ofstream LogFile;

    /**
     * @brief this function is used to initialize the logging system
     *
     * @param now     : current log level
     * @return true   : log system initialization successful
     * @return false  : log system initialization failed
     */
    bool Initialize(Grade now);

    /**
     * @brief this function is used to shut down the logging system and release the file pointer
     *
     */
    void Release();

} // namespace LoggingSystem

#ifdef LOG_SYSTEM_SAVE

#define Log_ERROR                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::ERROR) \
    LoggingSystem::LogFile << "[ERROR: (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_WARM                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::WARM) \
    LoggingSystem::LogFile << "[WARM : (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_INFO                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::INFO) \
    LoggingSystem::LogFile << "[INFO : (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_END "\n"

#else

#define Log_ERROR                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::ERROR) \
    std::cout << "\033[1;31m[ERROR: (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_WARM                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::WARM) \
    std::cout << "\033[1;33m[WARM : (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_INFO                                               \
    if (LoggingSystem::nowGrade <= LoggingSystem::Grade::INFO) \
    std::cout << "\033[1;37m[INFO : (" << __FUNCTION__ << ") " << __FILE__ << ":" << __LINE__ << "] : "

#define Log_END "\033[0m" \
                    << "\n"

#endif // LOG_SYSTEM_SAVE
