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

#include <FileManager.h>

#include <QtTest/QtTest>
#include <QtCore/QDebug>

using namespace std;

class tst_FileManager : public QObject
{
    Q_OBJECT

private slots:
    void test_hasSuffix();
    void test_hasSuffix_data();

    void test_fileBaseName();
    void test_fileBaseName_data();

    void test_fileExtension();
    void test_fileExtension_data();

};

/******************************************************************************
 ******************************************************************************/
void tst_FileManager::test_hasSuffix_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("suffix");
    QTest::addColumn<bool>("expected");

    QTest::newRow("test_hasSuffix_1001") << "" << "" << false;
    QTest::newRow("test_hasSuffix_1002") << "" << "txt" << false;
    QTest::newRow("test_hasSuffix_1003") << "" << ".txt" << false;
    QTest::newRow("test_hasSuffix_1004") << "file" << "" << false;

    QTest::newRow("test_hasSuffix_2001") << "file" << "file" << true;

    QTest::newRow("test_hasSuffix_3001") << "file" << "txt" << false;
    QTest::newRow("test_hasSuffix_3002") << "file" << ".txt" << false;
    QTest::newRow("test_hasSuffix_3003") << "file.txt" << "txt" << true;
    QTest::newRow("test_hasSuffix_3004") << "file.txt" << ".txt" << true;

    QTest::newRow("test_hasSuffix_4001") << "file.txt.csv" << "txt" << false;
    QTest::newRow("test_hasSuffix_4002") << "file.txt.csv" << ".txt" << false;

    QTest::newRow("test_hasSuffix_5001") << "file.txt.csv" << "txt.csv" << true;
    QTest::newRow("test_hasSuffix_5002") << "file.txt.csv" << ".txt.csv" << true;

    QTest::newRow("test_hasSuffix_6001") << "file.txt" << "TXT" << true;  /* case-insensitive */
    QTest::newRow("test_hasSuffix_6002") << "file.txt" << "LE.TXT" << true;

}

void tst_FileManager::test_hasSuffix()
{
    // Given
    QFETCH(QString, filename);
    QFETCH(QString, suffix);
    QFETCH(bool, expected);
    std::string _filename = filename.toStdString();
    std::string _suffix   = suffix.toStdString();

    // When
    bool actual = FileManager::hasSuffix( _filename, _suffix );

    // Then
    QCOMPARE(actual, expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_FileManager::test_fileBaseName_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("expected");

    QTest::newRow("test_fileBaseName_1001") << "" << "" ;
    QTest::newRow("test_fileBaseName_1002") << "file" << "file";
    QTest::newRow("test_fileBaseName_1002") << "/dir/file" << "/dir/file";

    QTest::newRow("test_fileBaseName_2001") << ".bashrc" << "" ;

    QTest::newRow("test_fileBaseName_3001") << "./../../file.0.txt.csv" << "./../../file.0.txt";
    QTest::newRow("test_fileBaseName_3002") << "./../../file" << "./../../file";
    QTest::newRow("test_fileBaseName_3003") << "path.to.dir/file" << "path.to.dir/file";

    QTest::newRow("test_fileBaseName_4001") << "file.txt" << "file";
    QTest::newRow("test_fileBaseName_4002") << "/dir/file.txt" << "/dir/file";

    QTest::newRow("test_fileBaseName_5001") << "file.txt.csv" << "file.txt";
    QTest::newRow("test_fileBaseName_5002") << "/dir/file.txt.csv" << "/dir/file.txt";


}

void tst_FileManager::test_fileBaseName()
{
    // Given
    QFETCH(QString, filename);
    QFETCH(QString, expected);
    std::string _filename = filename.toStdString();
    std::string _expected = expected.toStdString();

    // When
    std::string actual = FileManager::fileBaseName(_filename);

    // Then
    QCOMPARE(actual, _expected);
}

/******************************************************************************
 ******************************************************************************/
void tst_FileManager::test_fileExtension_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("expected");

    QTest::newRow("test_fileExtension_1001") << "" << "" ;
    QTest::newRow("test_fileExtension_1002") << "file" << "";
    QTest::newRow("test_fileExtension_1002") << "/dir/file" << "";

    QTest::newRow("test_fileExtension_2001") << ".bashrc" << ".bashrc" ;

    QTest::newRow("test_fileExtension_3001") << "./../../file.0.txt.csv" << ".csv";
    QTest::newRow("test_fileExtension_3002") << "./../../file" << "";
    QTest::newRow("test_fileExtension_3003") << "path.to.dir/file" << "";

    QTest::newRow("test_fileExtension_4001") << "file.txt" << ".txt";
    QTest::newRow("test_fileExtension_4002") << "/dir/file.txt" << ".txt";

    QTest::newRow("test_fileExtension_5001") << "file.txt.csv" << ".csv";
    QTest::newRow("test_fileExtension_5002") << "/dir/file.txt.csv" << ".csv";

}

void tst_FileManager::test_fileExtension()
{
    // Given
    QFETCH(QString, filename);
    QFETCH(QString, expected);
    std::string _filename = filename.toStdString();
    std::string _expected = expected.toStdString();

    // When
    std::string actual = FileManager::fileExtension(_filename);

    // Then
    QCOMPARE(actual, _expected);
}

/******************************************************************************
 ******************************************************************************/

QTEST_APPLESS_MAIN(tst_FileManager)

#include "tst_filemanager.moc"

