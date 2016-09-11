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

#include <Utils/TestSuite.h>
#include <Reader.h>
#include <Writer.h>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

using namespace std;

static bool run(stringstream * const idevice, stringstream * const odevice)
{
    bool converted = true;

    Reader reader;
    PunchFile pch = reader.parsePUNCH(idevice);

    Writer writer;
    for (auto & key : pch.blockKeys()) {
        auto br = pch.blockRange(key);
        for (auto b = br.first; b != br.second; ++b) {
            PunchBlock block = b->second;
            converted &= writer.writeCSV(block, odevice);
        }
    }
    return converted;
}


class tst_Scanner : public QObject
{
    Q_OBJECT

private slots:
    /* test the invalid punch files */
    void test_empty();
    void test_empty_data();
    void test_invalid();

    /* test the content */
    void test_valid_simple();
    void test_valid_continued();
    void test_valid_1();
    void test_valid_simple_2();
    void test_several_types_in_a_single_input();
    void test_several_types();

    /* test the comment lines (column headers) */
    void test_comment();
    void test_comment_with_header();
    void test_unsorted_line_number();

    /*test the options */
    void test_option_output();
    void test_option_column_header();
    void test_option_skip_header();

};


/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_empty_data()
{
    QTest::addColumn<QString>("content");

    QTest::newRow("empty__1") << "";
    QTest::newRow("empty__2") << "\r\n";
    QTest::newRow("empty__3") << "\r\n\r\n\r\n";
    QTest::newRow("empty__4") << "\n";
    QTest::newRow("empty__5") << "\n\n\n";
    QTest::newRow("empty__6") << "\r";
    QTest::newRow("empty__7") << "\r\r\r";
    QTest::newRow("empty__8") << "       \n     \n       ";
    QTest::newRow("empty__9") << "\t\n\t\n";
}

void tst_Scanner::test_empty()
{
    // Given
    QFETCH(QString, content);
    std::string _content   = content.toStdString();
    std::stringstream buffer( _content );

    std::stringstream actual;
    std::stringstream expected(""); /* Must be empty because it's not a valid Punch format */

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_valid_simple()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "AAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDD99999999\n" );

    std::stringstream actual;
    std::stringstream expected(
                "unknown,unknown,unknown,unknown,\n"
                "AAAAAAAAAAAAAAAAAA,"
                "BBBBBBBBBBBBBBBBBB,"
                "CCCCCCCCCCCCCCCCCC,"
                "DDDDDDDDDDDDDDDDDD,\n"
                );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}
/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_valid_continued()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "AAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDD99999999\n"
                "-CONT-EEEEEEEEEEEEFFFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHHHH99999999\n"
                /*^^^^^ continued */
                );

    std::stringstream actual;
    std::stringstream expected(
                "unknown,unknown,unknown,unknown,unknown,unknown,unknown,\n"
                "AAAAAAAAAAAAAAAAAA,"
                "BBBBBBBBBBBBBBBBBB,"
                "CCCCCCCCCCCCCCCCCC,"
                "DDDDDDDDDDDDDDDDDD,"
                "FFFFFFFFFFFFFFFFFF," /* No EEEEE ! */
                "GGGGGGGGGGGGGGGGGG,"
                "HHHHHHHHHHHHHHHHHH\n"
                );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_several_types()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "     80004231           7.232352E+04                                           1\n"
                "  80000736             -1.600000E+00      8.633642E+01      6.911208E+01       2\n"
                "     30011          80004230        BAR                                        3\n"
                );

    std::stringstream actual;
    std::stringstream expected(
                /* -------- 1st structure -------- */

                "unknown,unknown,\n"
                "80004231,7.232352E+04,\n"

                /* -------- 2nd structure -------- */

                /// XXX TODO "unknown,unknown,unknown,unknown,\n"
                "80000736,-1.600000E+00,8.633642E+01,6.911208E+01,\n"

                /* -------- 3rd structure -------- */

                /// XXX TODO "unknown,unknown,unknown,\n"
                "30011,80004230,BAR,\n"
                );


    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_invalid()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "-cont-                  8.316166E+01  \n"
                " - This is not a punch file -\n"
                "Curiously, there are some values...\n"
                "123               456               789\n"
                "$SECOND_VALUE TOTO                                                           123 \n"
                "-> it's not a valid Punch line because has more than 80 characters...   1235123123456\n"
                "-> not valid because has 80 characters exactly but no ending line number...    \n"
                "-> it's not a valid Punch line because has 79 characters...                  1\n"
                "-> it's not a valid Punch line because has 81 characters...                    1 \n"
                "\n"
                "\n"
                "!!! BUT IT'S NOT A PUNCH !!!\n"
                );

    std::stringstream actual;
    std::stringstream expected(""); /* Must be empty because has no valid Punch line. */

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_comment()
{
    /* Here comments don't contain equal '=' symbols. Output must be empty. */
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "$ FIRST VALUE  DA DADA                                                         1\n"
                "$SECOND_VALUE TOTO                                                             2\n"
                "$   3rd value                                            some thing late       3\n"
                "$ A line with no value because there's no equal symbol                         4\n"
                "$ Next comment line is empty                                                   5\n"
                "$                                                                              6\n"
                );

    std::stringstream actual;
    std::stringstream expected("");

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );

}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_comment_with_header()
{
    /* Here comments contain equal '=' symbols, so the HEADERS are assigned. */
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "$ FIRST VALUE    = DA DADA                                                     1\n"
                "$SECOND_VALUE=TOTO                                                             2\n"
                "$   3rd value =                                          some thing late       3\n"
                "$ A line with no value because there's no equal symbol                         4\n"
                "$ Next comment line is empty                                                   5\n"
                "$                                                                              6\n"
                "\n" );

    std::stringstream actual;
    std::stringstream expected( "3rd value, FIRST VALUE, SECOND_VALUE\n" );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );

}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_unsorted_line_number()
{
    /* Ending line numbers are optional. Moreover, they don't need to be continuous. */
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "     80004230          -3.404367E+03                                        1237\n"
                "-CONT-                 -7.163730E+04                                         909\n"
                "     80004231           7.232352E+04                                           0\n"
                "-CONT-                 -2.301775E+04                                          11\n"
                "     80004232          -6.865467E+03                                      456457\n"
                "-CONT-                  2.288704E+04                                           8\n"
                "     80004233          -9.844569E+06                                          10\n"
                "-CONT-                 -2.9784151+04                                    99999999\n" );

    std::stringstream actual;
    std::stringstream expected( "unknown,unknown,unknown,unknown,unknown\n"
                                "80004230, -3.404367E+03, , , -7.163730E+04 \n"
                                "80004231,  7.232352E+04, , , -2.301775E+04 \n"
                                "80004232, -6.865467E+03, , ,  2.288704E+04 \n"
                                "80004233, -9.844569E+06, , , -2.9784151+04 \n");

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_valid_1()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "$Header1 = ID ELEMENT                                                          2\n"
                "$Header2 = FX                                                                  3\n"
                "$Header3 = FY                                                                  3\n"
                "$                                                                              5\n"
                "$SUBCASE ID =         666                                                      6\n"
                "     80004230          -3.404367E+03                                           7\n"
                "-CONT-                 -7.163730E+04                                           9\n"
                "     80004231           7.232352E+04                                          10\n"
                "-CONT-                 -2.301775E+04                                          11\n"
                "$                                                                              5\n"
                "$Header1 = ID ELEMENT                                                          2\n"
                "$Header2 = FX                                                                  3\n"
                "$Header3 = FY                                                                  3\n"
                "$                                                                              5\n"
                "$SUBCASE ID =         777                                                      6\n"
                "     80004230          -6.865467E+03                                           7\n"
                "-CONT-                  2.288704E+04                                           8\n"
                "     80004231          -9.844569E+06                                          10\n"
                "-CONT-                 -2.9784151+04                                          11\n" );

    std::stringstream actual;
    std::stringstream expected(
                "Header1, Header2, Header3, SUBCASE ID, unknown,unknown,unknown,unknown,unknown\n"
                "ID ELEMENT, FX, FY, 666, 80004230, -3.404367E+03, , , -7.163730E+04 \n"
                "ID ELEMENT, FX, FY, 666, 80004231,  7.232352E+04, , , -2.301775E+04 \n"
                "ID ELEMENT, FX, FY, 777, 80004230, -6.865467E+03, , ,  2.288704E+04 \n"
                "ID ELEMENT, FX, FY, 777, 80004231, -9.844569E+06, , , -2.9784151+04 \n" );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM_X( actual, expected );

}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_valid_simple_2()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "$TITLE   = MY FEA MODEL                                                        1\n"
                "$SUBTITLE=MY FIRST LOAD CASE                                                   2\n"
                "$LABEL   =MY FIRST LOAD CASE                                                   3\n"
                "$ELEMENT STRESSES                                                              4\n"
                "$REAL OUTPUT                                                                   5\n"
                "$SUBCASE ID =         666                                                      6\n"
                "     12345          80004230        BAR                                        7\n"
                "-CONT-                  2.288704E+04     -3.404367E+03      1.639255E+03       8\n"
                "-CONT-                 -7.163730E+04      9.975631E+05      3.060709E+06       9\n"
                "     12345          80004231        BAR                                       10\n"
                "-CONT-                 -2.301775E+04     -3.107557E+03      4.195733E+02      11\n"
                "-CONT-                  7.232352E+04     -9.979151E+05     -3.062225E+06      12\n" );

    std::stringstream actual;
    std::stringstream expected(
                "LABEL,SUBCASE ID,SUBTITLE,TITLE,unknown,unknown,unknown,"
                "unknown,unknown,unknown,unknown,unknown,unknown,unknown,\n"
                "MY FIRST LOAD CASE,666,MY FIRST LOAD CASE,MY FEA MODEL,12345,"
                "80004230,BAR,,2.288704E+04,-3.404367E+03,1.639255E+03,-7.163730E+04,"
                "9.975631E+05,3.060709E+06,\n"
                "MY FIRST LOAD CASE,666,MY FIRST LOAD CASE,MY FEA MODEL,12345,"
                "80004231,BAR,,-2.301775E+04,-3.107557E+03,4.195733E+02,7.232352E+04,"
                "-9.979151E+05,-3.062225E+06,\n" );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/******************************************************************************
 ******************************************************************************/
void tst_Scanner::test_several_types_in_a_single_input()
{
    // Given
    std::stringstream buffer(
                /*<--- 18 char ---><---- 18 char ---><---- 18 char ---><---- 18 char ---><8char->*/
                "$TITLE   = MY FEA MODEL                                                        1\n"
                "$SUBTITLE=MY FIRST LOAD CASE                                                   2\n"
                "$LABEL   =MY FIRST LOAD CASE                                                   3\n"
                "$ELEMENT STRESSES                                                              4\n"
                "$REAL OUTPUT                                                                   5\n"
                "$SUBCASE ID =         132                                                      6\n"
                "$ELEMENT TYPE =          33  QUAD4     VONM                                    7\n"
                "  12345678             -1.600000E+00      8.633642E+01      6.911208E+01       8\n"
                "-CONT-                  1.524100E+01      3.026534E+01      9.523018E+01       9\n"
                "-CONT-                  6.021832E+01      8.342920E+01      1.600000E+00      10\n"
                "-CONT-                  8.174755E+01      7.194228E+01      1.768816E+01      11\n"
                "-CONT-                  3.725410E+01      9.519994E+01      5.848990E+01      12\n"
                "-CONT-                  8.316166E+01                                          13\n"
                "  12345678             -1.600000E+00      8.752773E+01      7.001486E+01      14\n"
                "-CONT-                  1.574999E+01      3.046375E+01      9.679176E+01      15\n"
                "-CONT-                  6.075083E+01      8.472974E+01      1.600000E+00      16\n"
                "-CONT-                  8.222987E+01      7.079288E+01      1.859368E+01      17\n"
                "-CONT-                  3.645235E+01      9.596455E+01      5.705819E+01      18\n"
                "-CONT-                  8.360184E+01                                          19\n"
                "$TITLE   = MY FEA MODEL                                                       20\n"
                "$SUBTITLE=MY FIRST LOAD CASE                                                  21\n"
                "$LABEL   =MY FIRST LOAD CASE                                                  22\n"
                "$GRID POINT FORCE BALANCE                                                     23\n"
                "$REAL OUTPUT                                                                  24\n"
                "$SUBCASE ID =         132                                                     25\n"
                "     12345          80004230        BAR                                       26\n"
                "-CONT-                  2.288704E+04     -3.404367E+03      1.639255E+03      27\n"
                "-CONT-                 -7.163730E+04      9.975631E+05      3.060709E+06      28\n"
                "     12345          80004231        BAR                                       29\n"
                "-CONT-                 -2.301775E+04     -3.107557E+03      4.195733E+02      30\n"
                "-CONT-                  7.232352E+04     -9.979151E+05     -3.062225E+06      31\n"
                "     12345          80000851        QUAD4                                     32\n"
                "-CONT-                 -1.267647E+04      4.610322E+03      6.537858E+03      33\n"
                "-CONT-                 -3.128877E+02      4.934991E+02      1.653072E+03      34\n"
                "  12345678                          *TOTALS*                                  35\n"
                "-CONT-                 -3.492460E-10     -1.906592E-07     -3.516470E-07      36\n"
                "-CONT-                 -6.166636E-06      2.537854E-08     -7.888302E-07      37\n" );

    std::stringstream actual;
    std::stringstream expected(
                /* ------------ 1st data structure ------------ */
                "ELEMENT TYPE,LABEL,SUBCASE ID,SUBTITLE,TITLE,unknown,unknown,unknown,unknown,unknown,"
                "unknown,unknown,unknown,unknown,unknown,unknown,unknown,unknown,unknown,unknown,unknown,unknown,\n"
                "33  QUAD4     VONM,MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345678,"
                "-1.600000E+00, 8.633642E+01, 6.911208E+01,1.524100E+01,3.026534E+01,9.523018E+01,"
                "6.021832E+01,8.342920E+01,1.600000E+00,8.174755E+01,7.194228E+01,1.768816E+01,"
                "3.725410E+01,9.519994E+01,5.848990E+01,8.316166E+01,\n"
                "33  QUAD4     VONM,MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345678,"
                "-1.600000E+00, 8.752773E+01, 7.001486E+01,1.574999E+01,3.046375E+01,9.679176E+01,"
                "6.075083E+01,8.472974E+01,1.600000E+00,8.222987E+01,7.079288E+01,1.859368E+01,"
                "3.645235E+01,9.596455E+01,5.705819E+01,8.360184E+01,\n"

                /* ------------ 2nd data structure ------------ */
                "LABEL,SUBCASE ID,SUBTITLE,TITLE,unknown,unknown,unknown,unknown,"
                "unknown,unknown,unknown,unknown,unknown,unknown, \n"
                "MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345,80004230,BAR     ,,"
                "     2.288704E+04,-3.404367E+03, 1.639255E+03,-7.163730E+04, 9.975631E+05, 3.060709E+06 \n"
                "MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345,80004231,BAR     ,,"
                "    -2.301775E+04,-3.107557E+03, 4.195733E+02, 7.232352E+04,-9.979151E+05,-3.062225E+06 \n"
                "MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345,80000851,QUAD4   ,,"
                "    -1.267647E+04, 4.610322E+03, 6.537858E+03,-3.128877E+02, 4.934991E+02, 1.653072E+03 \n"
                "MY FIRST LOAD CASE,132,MY FIRST LOAD CASE,MY FEA MODEL,12345678,     ,*TOTALS*,,"
                "    -3.492460E-10,-1.906592E-07,-3.516470E-07,-6.166636E-06, 2.537854E-08,-7.888302E-07 \n"
                );

    // When
    run( &buffer, &actual );

    // Then
    COMPARE_STREAM( actual, expected );
}

/* *****************************************************************************
 ***************************************************************************** */

void tst_Scanner::test_option_output()
{
    /// \todo implement it
}

void tst_Scanner::test_option_column_header()
{
    /// \todo implement it
}

void tst_Scanner::test_option_skip_header()
{
    /// \todo implement it
}

/* *****************************************************************************
 ***************************************************************************** */


QTEST_APPLESS_MAIN(tst_Scanner)

#include "tst_scanner.moc"

