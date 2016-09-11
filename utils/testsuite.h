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
#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <sstream>

/*!
 * Compare streams, and show the line when failed.
 */
#define COMPARE_STREAM(actual, expected) \
    do { \
    if (!TestSuite::compareCSV(actual, expected, __FILE__, __LINE__)) \
    return; \
    } while (0)


/*!
 * Compare streams, and dump the whole streams when failed.
 */
#define COMPARE_STREAM_X(actual, expected) \
    do { \
    if (!TestSuite::compareCSV(actual, expected, __FILE__, __LINE__)) { \
    TestSuite::dumpStream("ACTUAL", actual); \
    TestSuite::dumpStream("EXPECTED", expected); \
    return; \
    } \
    } while (0)


class TestSuite
{
public:
    static bool compareCSV(std::istream &actual, std::istream &expected,
                           const char *file = 0, int line = 0);
    static void dumpStream(const char *title, const std::stringstream &idevice);
};


#endif // TEST_SUITE_H
