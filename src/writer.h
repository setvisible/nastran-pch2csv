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
#ifndef WRITER_H
#define WRITER_H

#include <ostream>
#include <string>

class PunchBlock;

class Writer
{
    enum class HeaderType {
        Default,
        UserDefined,
        NoHeader
    };

public:
    explicit Writer();
    explicit Writer(const std::string &columnHeaderLine, const bool skipColumnHeaders);

    void enableHeader(const HeaderType enable);
    void setHeader(const std::string &header);

    bool writeCSV(PunchBlock &block, std::ostream * const odevice);

    static inline const char* separator();
    static inline const char* quote();
    static inline const char* unknown();

private:
    HeaderType m_headerEnable;
    std::string m_userDefinedHeader;
    std::string m_previousLeftHeaders;
};


#endif // WRITER_H

