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
#include "writer.h"

#include "punchfile.h"

#include <assert.h>
#include <ostream>
#include <string>

static const char str_separator[] = ";";
static const char str_quote[]     = "\"";
static const char str_unknown[]   = "unknown";

using namespace std;

/*! \class Writer
 *  \brief The class Writer converts a \a PunchFile into a CSV stream.
 *
 * For each PunchBlock in the PunchFile, use \a writeCSV() to output the stream.
 * The function returns \a true is correctly written, otherwise \a false.
 *
 * \example
 *
 * \code
 * // std::ofstream ofs;
 * // PunchFile pch;
 * Writer writer;
 * for (auto & key : pch.blockKeys()) {
 *      auto br = pch.blockRange(key);
 *      for (auto b = br.first; b != br.second; ++b) {
 *          PunchBlock block = b->second;
 *          writer.writeCSV(block, &ofs);
 *      }
 *  }
 * \endcode
 */
/*! \brief Constructor.
 */
Writer::Writer()
    : m_headerEnable(HeaderType::Default)
    , m_userDefinedHeader(string())
    , m_previousLeftHeaders(string())
{
}

/*! \brief Constructor.
 */
Writer::Writer(const std::string &columnHeaderLine,
               const bool skipColumnHeaders)
{
    if (skipColumnHeaders) {
        m_headerEnable = Writer::HeaderType::NoHeader;
    } else {
        if ( !columnHeaderLine.empty() ) {
            m_headerEnable = Writer::HeaderType::UserDefined;
            m_userDefinedHeader = columnHeaderLine;

        } else {
            m_headerEnable = Writer::HeaderType::Default;
        }
    }
}

/******************************************************************************
 ******************************************************************************/
void Writer::enableHeader(const HeaderType enable)
{
    m_headerEnable = enable;
}

void Writer::setHeader(const std::string &header)
{
    m_userDefinedHeader = header;
}

/******************************************************************************
 ******************************************************************************/
inline const char* Writer::separator()
{
    return str_separator;
}

inline const char* Writer::quote()
{
    return str_quote;
}

inline const char* Writer::unknown()
{
    return str_unknown;
}

/******************************************************************************
 ******************************************************************************/
bool Writer::writeCSV(PunchBlock &block, std::ostream * const odevice)
{
    assert(odevice);

    /* **************************************** */
    /* Prepare the header and the common rows   */
    /* **************************************** */
    string defaultBlockHeader;
    string prefixHeader;
    string prefixRow;

    for (std::pair<const string, string> &var : block.prefixRowAndHeader()) {
        prefixHeader += quote();
        prefixHeader += var.first;
        prefixHeader += quote();
        prefixHeader += separator();
        prefixRow += quote();
        prefixRow += var.second;
        prefixRow += quote();
        prefixRow += separator();
    }

    for( int i = block.columnCount(); i>0; --i) {
        defaultBlockHeader += quote();
        defaultBlockHeader += unknown();
        defaultBlockHeader += quote();
        defaultBlockHeader += separator();
    }

    /* **************************************** */
    /* Write the header                         */
    /* **************************************** */
    string defaultHeader = prefixHeader + defaultBlockHeader;
    if (defaultHeader != m_previousLeftHeaders) {

        switch(m_headerEnable) {
        case HeaderType::NoHeader:
            break;
        case HeaderType::UserDefined:
            (*odevice) << prefixHeader;
            (*odevice) << m_userDefinedHeader;
            (*odevice) << std::endl;
            break;
        case HeaderType::Default:
        default:
            (*odevice) << defaultHeader;
            (*odevice) << std::endl;
            break;
        }

        m_previousLeftHeaders = defaultHeader;
    }

    /* **************************************** */
    /* Write the rows                           */
    /* **************************************** */
    if (!block.rows().empty()) {
        for (PunchRow &row : block.rows()) {
            (*odevice) << prefixRow;

            for (const auto& field : row) {
                (*odevice) << quote();
                (*odevice) << field;
                (*odevice) << quote();
                (*odevice) << separator();
            }
            (*odevice) << std::endl;
        }
    }

    return true;
}
