#pragma once

/**
 * @file Json.h
 * @author jiyilin 2474803745@qq.com
 * @brief this program is used for serialization management of data
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "nlohmann/json.hpp"
#include <fstream>
#include <string>

namespace JSON
{
    /**
     * @brief this function is used to serialize data and write it to a json file for saving
     *
     * @tparam Function   must be of type bool fun(nlohmann::json&, Data&)
     * @tparam Data       structure types that require serialization
     * @param path        path to the json file to be saved
     * @param Fun         pass in a function pointer to convert the structure type to be serialized into nlohmann::json
     * @param config      data to be serialized
     * @return true       function execution successful
     * @return false      function execution failed
     */
    template <typename Function, typename Data>
    bool Write_Json(std::string &path, Function Fun, Data &config)
    {
        nlohmann::json data;
        try
        {
            if (Fun(data, config) == true)
            {
                std::ofstream file(path);
                if (file.is_open() == false)
                {
                    Log_ERROR << "json file opening failed unable to write data" << Log_END;
                    return false;
                }
                file << data;
                file.close();
                Log_INFO << "successfully written json file" << Log_END;
                return true;
            }
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
        }

        Log_ERROR << "json serialization failed" << Log_END;
        return false;
    }

    /**
     * @brief 该函数用于读取json文件内的数据
     *
     * @tparam Function    must be of type bool fun(nlohmann::json&, Data&)
     * @tparam Data        the data type that needs to be returned
     * @param path         path to the json file to be saved
     * @param Fun          pass in a function pointer to convert the structure type to be serialized into nlohmann::json
     * @param config       this formal parameter returns the read data
     * @return true        function execution successful
     * @return false       function execution failed
     */
    template <typename Function, typename Data>
    bool Read_Json(std::string &path, Function Fun, Data &config)
    {
        std::ifstream file(path);

        nlohmann::json data;
        file >> data;

        try
        {
            if (Fun(data, config) == true)
            {
                file.close();
                Log_INFO << "successfully read json data" << Log_END;
                return true;
            }
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
        }

        Log_ERROR << "json data read failure" << Log_END;
        return false;
    }

} // namespace JSON
