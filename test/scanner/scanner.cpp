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
#include "scanner.h"

#include "Reader.h"
#include "Writer.h"

#if QT_CORE_LIB
#  include <QtCore/QString>
#  include <QtCore/QDebug>
#endif

using namespace std;

/******************************************************************************
 ******************************************************************************/
bool Scanner::convert(stringstream * const idevice,
                      stringstream * const odevice,
                      const bool enableDebug)
{
    Reader reader;
    PunchFile_Ptr pch = reader.parsePUNCH(idevice);

    Writer writer;
    auto ret = writer.writeCSV(pch, odevice);

    /* Following lines to make debugging easier. */
#if QT_CORE_LIB
    if (enableDebug) {
        string input = idevice->str();
        string output = odevice->str();
        qDebug() << "====================================================" ;
        qDebug() << "INPUT :" ;
        qDebug() << QString(input.c_str());
        qDebug() << "====================================================" ;
        qDebug() << "OUTPUT :" ;
        qDebug() << QString(output.c_str());

        exit(0);
    }
#else
    (void)enableDebug; // unused
#endif

    return ret;
}

