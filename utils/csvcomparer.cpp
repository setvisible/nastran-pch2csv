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
#include "csvcomparer.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*! \class CSVComparer
 *  \brief The class CSVComparer is a helper class for comparing
 *         CSV (Comma-Separated Format) files.
 *
 * The CSV format is very permissive :
 *  \li Can be with/without double-quotes
 *  \li Can contains white spaces or not
 *  \li Can have ending comma
 *
 * That makes the comparison between two lines of data more complex than
 * comparing two strings.
 *
 * See explanation in CSVComparer::areStreamEqual() for details.
 *
 */

/* *****************************************************************************
 ***************************************************************************** */
/*! \brief Returns \a true if the given \a stream1 and the given \a stream2 have
 * equivalent data, when in CSV format. Otherwise the function returns \a false.
 *
 * Indeed, in term of data in a CSV file, two files can have two different
 * representations (or streams), but be stricly equivalent in term of data.
 * See the two files below, they are different, but their data are strictly identical:
 *
 * (file 1)
 * \code
 * "bla bla",123,,Yo Text,,,,,,,
 * \endcode
 *
 * (file 2)
 * \code
 * bla bla,123.000,,"Yo Text"
 * \endcode
 *
 * In such case, \a areStreamEqual(file1, file2) will return \a true.
 *
 */
bool CSVComparer::areStreamEqual( std::istream &stream1, std::istream &stream2 )
{
    /* Compare the sizes */
    int size1 = getFileSize(stream1);
    int size2 = getFileSize(stream2);
    if ( size1 <= 0 && size2 <= 0 ) {
        return true;
    }

    /* Compare the line numbers */
    int lc1 = getFileLineCount(stream1);
    int lc2 = getFileLineCount(stream2);
    if ( lc1 != lc2 ) {
        return false;
    }

    /* Compare the contents */
    std::string line1;
    std::string line2;
    while( std::getline( stream1, line1)
           && std::getline( stream2, line2)){

        std::string resolvedLine1 = resolve(line1);
        std::string resolvedLine2 = resolve(line2);

        if ( resolvedLine1 != resolvedLine2 ){
            return false;
        }
    }

    return true;
}

/* *****************************************************************************
 ***************************************************************************** */
int CSVComparer::getFileLineCount(std::istream &device)
{
    int counter = 0;
    std::istream::pos_type fileSize = getFileSize(device);
    std::vector<char> bytes(fileSize);
    device.read(&bytes[0], fileSize);
    device.seekg(0, std::ios::beg);
    for (auto & v: bytes) {
        if (v == '\r' || v == '\n' ) {
            ++counter;
        }
    }
    return counter;
}


/* *****************************************************************************
 ***************************************************************************** */
int CSVComparer::getFileSize(std::istream &device)
{
    int length = -1;
    device.seekg(0, std::ios::end);
    length = device.tellg();
    device.seekg(0, std::ios::beg);
    return length;
}

/* *****************************************************************************
 ***************************************************************************** */
/*! \brief Resolves the given \a text. In other terms, simplify the text,
 *  i.e. it removes useless whitespaces, trailing commas, quotes, etc.
 *
 * Example:
 * \code
 * string text("  0, , , \"data 1\",   data 2  ,  ,,,    \r");
 * string res = resolve(text); // res == "0,,,data 1,data 2"
 * \endcode
 */
std::string CSVComparer::resolve(std::string &text)
{
    auto p = text.begin();

    /* Split by the comma */
    vector<string> vec;
    {
        string buf;
        while( p != text.end() ){
            if ( (*p) == ',' ) {
                vec.push_back(buf);
                buf.clear();
                ++p;
                continue;
            }
            buf.push_back(*p);
            ++p;
        }
        vec.push_back(buf);
    }

    /* Clean the fields */
    string str;
    for (auto & v: vec) {

        string buffer;
        bool escape = true;
        auto q = v.begin();

        while( q != v.end() ){

            switch(*q){

            case '\"':
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                if (!escape) {
                    buffer.push_back(' ');
                    escape = true;
                }
                break;

            default:
                escape = false;
                buffer.push_back(*q);
                break;
            }

            ++q;
        }
        str += trim( buffer );
        str.push_back(',');

    }

    /* Finally, removes the ending spaces and commas */
    string ret = trim_right( str, " ," );
    return ret;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Trim the given \a text with the given \a delimiters.
 *
 * Example:
 * \code
 * std::string s;
 * s = trim("   This   is a   string    ");  // s == "This   is a   string"
 * \endcode
 */
std::string CSVComparer::trim(const std::string &str, const std::string &delimiters)
{
    return trim_left( trim_right( str, delimiters ), delimiters );
}

std::string CSVComparer::trim_right(const std::string &str, const std::string &delimiters)
{
    auto strEnd = str.find_last_not_of(delimiters);
    auto strRange = strEnd + 1;
    return str.substr(0, strRange);
}

std::string CSVComparer::trim_left(const std::string &str, const std::string &delimiters)
{
    auto strBegin = str.find_first_not_of(delimiters);
    if (strBegin == std::string::npos) {
        return string();  /* no content */
    }
    auto strEnd = str.length();
    auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}
