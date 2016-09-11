/* - pch2csv - Copyright (C) 2016 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>


class FileManager
{
public:
    /* File Backup. */
    static bool doBackup(const std::string &filename);

    /* Helpers. */
    static bool hasSuffix(const std::string &filename, const std::string &suffix);
    static std::string formatIncrement(const std::string &output, const int increment);

    static bool exists(const std::string &filename);
    static std::string fileBaseName(const std::string &filename);
    static std::string fileExtension(const std::string &filename);

};


#endif // FILE_MANAGER_H

