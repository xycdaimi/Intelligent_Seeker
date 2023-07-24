# include "log_system/LoggingSystem.h"

namespace LoggingSystem
{
    /**
     * @brief save current log level
     *
     */
    Grade nowGrade = Grade::INFO;

    /**
     * @brief log file pointer
     *
     */
    std::ofstream LogFile;

    /**
     * @brief this function is used to initialize the logging system
     *
     * @param now     : current log level
     * @return true   : log system initialization successful
     * @return false  : log system initialization failed
     */
    bool Initialize(Grade now)
    {
        nowGrade = now;

#ifdef LOG_SYSTEM_SAVE
        LogFile.open("./Journal.log", std::ios_base::out);
        return LogFile.is_open();
#endif // LOG_SYSTEM_SAVE

        return true;
    }

    /**
     * @brief this function is used to shut down the logging system and release the file pointer
     *
     */
    void Release()
    {
#ifdef LOG_SYSTEM_SAVE
        if (LogFile.is_open() == true)
        {
            LogFile.close();
        }
#endif // LOG_SYSTEM_SAVE
    }

} // namespace LoggingSystem
