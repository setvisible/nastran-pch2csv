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
#ifndef CSV_COMPARER_H
#define CSV_COMPARER_H

#include <sstream>
#include <string>

class CSVComparer
{
public:
    /* Compare two CSV streams */
    static bool areStreamEqual(std::istream &actual, std::istream &expected, std::string &message);

private:
    static int getFileLineCount(std::istream &device);
    static int getFileSize(std::istream &device);
    static std::string resolve(std::string &text);

    static inline std::string trim(const std::string &str, const std::string &delimiters = " \f\n\r\t\v");
    static inline std::string trim_right(const std::string &str, const std::string &delimiters);
    static inline std::string trim_left(const std::string &str, const std::string &delimiters);

};

#endif // CSV_COMPARER_H
