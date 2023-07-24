#include "log_system/LoggingSystem.h"
#include "nlohmann/JSON.hpp"
#include <opencv2/opencv.hpp>

/**
 * @brief 待序列化的结构体类型
 * 
 */
struct Data
{
    std::string message;
    int number;
};

/**
 * @brief         序列化函数
 * 
 * @param key     json实例对象
 * @param lock    待序列化数据
 * @return true   序列化成功
 * @return false  序列化失败
 */
bool write(nlohmann::json &key, struct Data &lock)
{
    try
    {
        key["message"] = lock.message;
        key["number"] = lock.number;
    }
    catch (const std::exception &e)
    {
        Log_ERROR << e.what() << Log_END;
        return false;
    }

    return true;
}


/**
 * @brief         反序列化函数
 * 
 * @param key     json实例对象
 * @param lock    待获取的数据
 * @return true   反序列化成功
 * @return false  反序列化失败
 */
bool read(nlohmann::json &key, struct Data &lock)
{
    try
    {
        key.at("message").get_to(lock.message);
        key.at("number").get_to(lock.number);
    }
    catch(const std::exception& e)
    {
        Log_ERROR << e.what() << Log_END;
        return false;
    }

    return true;
}

int main(int argc, char const *argv[])
{
    LoggingSystem::Initialize(LoggingSystem::Grade::INFO);

    Log_INFO << "you input message is :" << argv[1] << Log_END;  // 输入需要保存的json文件路径

    std::string Configure(argv[1]);
    struct Data data;
    data.message = "hello world json success";
    data.number = 4;

    JSON::Write_Json(Configure, write, data);

    data.message = "error";

    JSON::Read_Json(Configure, read, data);

    std::cout << data.message << std::endl;

    LoggingSystem::Release();
    return 0;
}
