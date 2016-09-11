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
#include "testsuite.h"
#include "csvcomparer.h"

#include <sstream>
#include <string>

#if QT_CORE_LIB
#  include <QtTest/QtTest>
#  include <QtCore/QDebug>
#endif


using namespace std;

/*! \class TestSuite
 *  \brief The class TestSuite contains macros to compare two streams.
 *
 * Requires the Qt Test Framework.
 *
 * The streams are compared line after line.
 * If the lines are different, the macros returns a TEST FAIL,
 * and publishes the details:
 * \li Macro \a COMPARE_STREAM() displays the failed line when failed.
 * \li Macro \a COMPARE_STREAM_X() displays the two whole streams when failed.
 *
 *
 */

/*! \internal
 */
bool TestSuite::compareCSV(std::istream &actual, std::istream &expected, const char *file, int line)
{
    std::string message;
    bool ok = CSVComparer::areStreamEqual(actual, expected, message);
#if QT_CORE_LIB
    if (!ok) {
        QTest::qFail( message.c_str(), file, line );
    }
#endif
    return ok;
}

/*! \internal
 */
void TestSuite::dumpStream(const char* title, const std::stringstream &idevice)
{
#if QT_CORE_LIB
    string str;
    str += "\n[" + string(title) + "]\n";
    str += idevice.str();
    QTest::qWarn( str.c_str() );
#endif
}


