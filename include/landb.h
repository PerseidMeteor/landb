/**
 * @file landb.h
 * @author breezeTuT (y_q_email@163.com)
 * @brief
 * @date 2023-05-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __LANDB_H__
#define __LANDB_H__

#include <unordered_map>
#include <string>

namespace landb
{

    class landb
    {
    private:
        std::unordered_map<std::string,std::string> __hash_table;
    public:
        //constructor
        landb();

        //destructor
        ~landb();
    };
} // namespace landb

#endif // __LANDB_H__