/**
 * @file log.h
 * @author breezeTuT (y_q_email@163.com)
 * @brief  header for bitcask log
 * @date 2023-05-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <fstream>
#include <map>
#include <string>

namespace landb
{
    class IndexEntry
    {
    public:
        uint32_t offset;
        uint32_t key_len;
        uint32_t value_len;
    };

    // bitcask log
    class log
    {
    private:
        /* data */
        std::string __file_path;
        std::fstream __fd;
        std::map<std::string, IndexEntry> __index;

    public:
        log(/* args */) = delete;

        explicit log(const std::string &file_path);

        void Append(const std::string &key, const std::string &value);

        std::string Get(const std::string &key);

        ~log();
    };

} // namespace landb
