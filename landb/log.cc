/**
 * @file log.cc
 * @author breezeTuT (y_q_email@163.com)
 * @brief
 * @date 2023-05-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "log.h"

namespace landb
{
    explicit log::log(const std::string &file_path) : __file_path(file_path), __fd(file_path, std::ios::in | std::ios::out | std::ios::binary)
    {
        if (__fd.tellp() == 0)
        {
            uint32_t magic = 0x12345678;
            uint32_t version = 1;
            uint32_t create_time = time(nullptr);
            __fd.write(reinterpret_cast<const char *>(&magic), sizeof(magic));
            __fd.write(reinterpret_cast<const char *>(&version), sizeof(version));
            __fd.write(reinterpret_cast<const char *>(&create_time), sizeof(create_time));
        }
    }

    log::log(/* args */)
    {
    }

    void log::Append(const std::string &key, const std::string &value)
    {
        uint32_t key_len = key.size();
        uint32_t value_len = value.size();
        uint32_t offset = static_cast<uint32_t>(__fd.tellp());

        __fd.write(reinterpret_cast<const char *>(&key_len), sizeof(key_len));
        __fd.write(key.c_str(), key_len);
        __fd.write(reinterpret_cast<const char *>(&value_len), sizeof(value_len));
        __fd.write(value.c_str(), value_len);

        __index[key] = {offset, key_len, value_len};
    }

    std::string log::Get(const std::string &key)
    {
        auto it = __index.find(key);
        if (it == __index.end())
        {
            return "";
        }

        IndexEntry &entry = it->second;
        char *buf = new char[entry.value_len];

        __fd.seekg(entry.offset + entry.key_len, std::ios::beg);
        __fd.read(buf, entry.value_len);
        std::string value(buf, entry.value_len);

        delete[] buf;
        return value;
    }

    log::~log()
    {
    }

} // namespace landb
