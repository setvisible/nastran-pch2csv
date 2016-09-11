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
#include "filemanager.h"

#include <algorithm>    // std::transform()
#include <io.h>         // access()
#include <string>

using namespace std;

/*! \class FileManager
 *  \brief The class FileManager manages files and backup.
 *
 * To backup a file, use \a doBackup().
 *
 */
/******************************************************************************
 ******************************************************************************/
/*! \brief Ensures that the given \a filename is available.
 *
 * If the file doesn't exist, the function returns \a true.
 * If the file exists and the function moved it correctly (= backup), the function returns \a true.
 *
 * Otherwise, if the file exists but cannot be moved (used or permission denied),
 * the function returns \a false.
 *
 */
bool FileManager::doBackup(const string &filename)
{
    if (filename.empty())
        return true;

    if (FileManager::exists(filename)) {

        const string basename = fileBaseName(filename);
        const string extension = fileExtension(filename);

        string newname;
        int i = 0;
        do {
            newname = basename + ".bak" + std::to_string(i) + extension;
            i++;
        } while (FileManager::exists(newname));

        auto ret = std::rename( filename.c_str(), newname.c_str() );
        return (ret == 0);
    }
    return true;
}
/******************************************************************************
 ******************************************************************************/
/*! \brief Return true if the given \a filename ends with the given \a suffix.
 * Otherwise return false.
 *
 * \remark The function is case insensitive.
 */
bool FileManager::hasSuffix(const string &filename, const string &suffix)
{
    if (suffix.size() == 0 || suffix.size() > filename.size() )
        return false;

    string upper_suffix = suffix;
    string upper_filename = filename;
    std::transform( upper_suffix.begin(), upper_suffix.end(), upper_suffix.begin(), ::toupper);
    std::transform( upper_filename.begin(), upper_filename.end(), upper_filename.begin(), ::toupper);

    auto pos = upper_filename.size() - upper_suffix.size();
    auto res = upper_filename.compare(pos, upper_suffix.size(), upper_suffix);

    return (res == 0);
}


/******************************************************************************
 ******************************************************************************/
string FileManager::formatIncrement(const string &output, const int increment)
{
    const string basename = fileBaseName(output);
    const string extension = fileExtension(output);

    string ret
            = basename
            + "_format_"
            + to_string( increment )
            + extension;
    return ret;
}

/******************************************************************************
 ******************************************************************************/
inline bool FileManager::exists(const string &filename)
{
    return (access( filename.c_str(), F_OK ) != -1);
}

inline string FileManager::fileBaseName(const string &filename)
{
    auto separator1 = filename.find_last_of('/', string::npos);
    auto separator2 = filename.find_last_of('\\', string::npos);

    auto lastSeparator = max( (separator1 == string::npos? 0: separator1),
                              (separator2 == string::npos? 0: separator2));

    auto stringAfterSeparator = filename.substr(lastSeparator);
    auto pos = stringAfterSeparator.find_last_of('.', string::npos);

    if (pos == string::npos)
        return filename;

    pos += lastSeparator;
    string ret = filename.substr(0, pos );
    return ret;
}

inline string FileManager::fileExtension(const string &filename)
{
    auto basename = fileBaseName(filename);
    string ret = filename.substr(basename.length());
    return ret;
}
