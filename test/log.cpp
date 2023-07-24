#include "log_system/LoggingSystem.h"

int main(int argc, char const *argv[])
{
    LoggingSystem::Initialize(LoggingSystem::Grade::INFO); // 初始化日至系统，这里设置日至等级为INFO

    for (size_t i = 0; i < 20; i++)
    {
        // 打印日至
        Log_INFO << "hello it is info" << Log_END;
        Log_WARM << "hello it is warm" << Log_END;
        Log_ERROR << "hello it is error" << Log_END;
    }

    // 析构日至系统
    LoggingSystem::Release();

    return 0;
}
