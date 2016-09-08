#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

/* Manages the files and their backup. */
class FileManager
{
public:
    /* Backup */
    static int doBackup(const std::string &filename);

    /* Helpers for files */
    static bool hasSuffix(const std::string &filename, const std::string &suffix);

};


#endif // FILE_MANAGER_H

