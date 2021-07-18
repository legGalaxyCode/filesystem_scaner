/* file analyze project using c++17 filesystem library
 * hash-table saves paths of files and dirs
 * recursive iterator on the root of directory and disk
 * if hashes of file are equal then duplicates
 * check meta-info of last modified time
 * and then delete
 * !---- legGalaxyCode 2020 ----!
*/

#include <boost/tokenizer.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <windows.h>
#include "hash_table.h"

using tokenizer = boost::tokenizer<boost::char_separator<char>>;

namespace fs = std::filesystem;

#define LAA(se) {{se},SE_PRIVILEGE_ENABLED|SE_PRIVILEGE_ENABLED_BY_DEFAULT}

#define BEGIN_PRIVILEGES(tp, n) static const struct {unsigned long PrivilegeCount;LUID_AND_ATTRIBUTES Privileges[n];} tp = {n,{
#define END_PRIVILEGES }};

// in case you not include wdm.h, where this defined
#define SE_BACKUP_PRIVILEGE (17L)

unsigned long AdjustPrivileges()
{
    if (ImpersonateSelf(SecurityImpersonation))
    {
        HANDLE hToken;
        if (OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, true, &hToken))
        {
            BEGIN_PRIVILEGES(tp, 1)
            LAA(SE_BACKUP_PRIVILEGE),
            END_PRIVILEGES
            AdjustTokenPrivileges(hToken, false, (PTOKEN_PRIVILEGES)&tp, 0, 0, 0);
            CloseHandle(hToken);
        }
    }

    return GetLastError();
}

int main() {
    {
        AdjustPrivileges();

        std::string str_path_to_trace;
        std::cout << "Input the path: "; std::cin >> str_path_to_trace;
        fs::path path_to_trace = str_path_to_trace;

        boost::char_separator<char> sep("\\");

        fs::recursive_directory_iterator f_begin(path_to_trace, fs::directory_options::skip_permission_denied);
        fs::recursive_directory_iterator f_end;
        std::vector<fs::path> subdirs_files;
        int counter = 0;
        std::copy_if(f_begin, f_end, std::back_inserter(subdirs_files), [&counter](const fs::path& path) {
            if (fs::is_regular_file(path))
                ++counter;
            return fs::is_regular_file(path);
        });
        counter += 10;
        hash_table<int, std::string> table(counter);
        for (auto &item : subdirs_files) {
            //++counter;
            std::string string_path = item.string();
            tokenizer tok(string_path, sep);
            std::string cur_dir_path;
            std::string cur_file_path;
            std::vector<std::string> paths;
            for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
                paths.push_back(*beg);
                paths.emplace_back("/");
            }
            paths.pop_back();
            cur_file_path = paths.back();
            for (auto& elem : paths)
                cur_dir_path += elem;
            table.add(cur_file_path, cur_dir_path); // the most important moment
            table.print_linked_list();
            //table.find_head("file.txt");
            }
        std::cout << table.get_table_size() << "\n";
        //std::cout << table.find("file.txt");
        /*fs::space_info info = fs::space(".");
        std::cout << std::setw(15) << "Capacity"
                  << std::setw(15) << "Free"
                  << std::setw(15) << "Available"
                  << std::setw(15) << "Nonfree"
                  << "\n"
                  << std::setw(15) << info.capacity
                  << std::setw(15) << info.free
                  << std::setw(15) << info.available
                  << std::setw(15) << info.capacity - info.free
                  << "\n";*/
        //std::cout << sizeof(table) << std::endl;
    }
    //fs::path current_path = fs::current_path();

    return 0;
}
