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

#include <Utils/CSVComparer.h>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

using namespace std;

class tst_CSVComparer : public QObject
{
    Q_OBJECT

private slots:
    void test_areStreamEqual();
    void test_areStreamEqual_data();

};

void tst_CSVComparer::test_areStreamEqual_data()
{
    QTest::addColumn<QString>("text_1");
    QTest::addColumn<QString>("text_2");
    QTest::addColumn<bool>("areEquals");

    /* ***************************** */
    /* Tests 1000-1999 must succeed  */
    /* ***************************** */

    QTest::newRow("test_areStreamEqual_1001") << "    " << "" << true;
    QTest::newRow("test_areStreamEqual_1002") << "bla" << "bla,,," << true;
    QTest::newRow("test_areStreamEqual_1003") << "bla        " << "bla" << true;
    QTest::newRow("test_areStreamEqual_1004") << "     bla" << "bla" << true;
    QTest::newRow("test_areStreamEqual_1005") << "\"double quoted 1\", \"\", \"double quoted 2\","
                                              << "double quoted 1,,double quoted 2,," << true;
    QTest::newRow("test_areStreamEqual_1006") << "rock'n'roll, '', 'yo'"
                                              << "rock'n'roll,'','yo'," << true;
    QTest::newRow("test_areStreamEqual_1007") << "bla,bla" << "bla;bla" << true;


    /* ***************************** */
    /* Tests 2000-2999 must fail     */
    /* ***************************** */

    /* not the same number of lines */
    QTest::newRow("test_areStreamEqual_2001") << "\n" << "\n\n\n" << false;

    QTest::newRow("test_areStreamEqual_2002") << "bla,bla" << "bla," << false;
    QTest::newRow("test_areStreamEqual_2003") << "bla" << ",,bla" << false;

    /* Characters " and ' are not equivalent */
    QTest::newRow("test_areStreamEqual_2004") << "'quote'" << "quote" << false;


}

void tst_CSVComparer::test_areStreamEqual()
{
    // Given
    QFETCH(QString, text_1);
    QFETCH(QString, text_2);
    QFETCH(bool, areEquals);

    std::string _test_1 = text_1.toStdString();
    std::string _test_2 = text_2.toStdString();

    std::istringstream buffer_1(_test_1, std::ios::in | std::ios::binary | std::ios::ate);
    std::istringstream buffer_2(_test_2, std::ios::in | std::ios::binary | std::ios::ate);

    // When
    string msg;
    bool res = CSVComparer::areStreamEqual( buffer_1, buffer_2, msg );

    // Then
    QCOMPARE( res , areEquals );
}

QTEST_APPLESS_MAIN(tst_CSVComparer)

#include "tst_csvcomparer.moc"

