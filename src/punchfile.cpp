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

#include "punchfile.h"

using namespace std;


/*! \class PunchBlock
 *  \brief Contains a block of data represented in the Punch file by a header
 *  (commented-out with '$' dollar) followed by similar-format lines.
 *
 * \subsection sec-hood Under the Hood
 *
 * The data in a PunchBlock can be represented in a table,
 * composed by 4 sub-tables:
 *
 *    (n columns)      (m columns)
 * :----------------:---------------:
 * : prefixHeader   : rowHeader     : (1 line)
 * :----------------:---------------:
 * :                : row 1         :
 * :   prefixRow    : row 2         :
 * :                : ...           : (r lines)
 * :                : row r         :
 * :----------------:---------------:
 *
 * Legend:
 *  \li prefixHeader : a n-dimensional row that contains the headers for this block
 *  \li rowHeader    : a m-dimensional row that contains the headers of the rows.
 *                     They are user-defined.
 *  \li prefixRow    : a (n,r) table that contains the block data as row (repeated r times)
 *  \li rows         : a (m,r) table that contains the rows data
 *
 */
/*! \brief Constructor.
 */
PunchBlock::PunchBlock()
{
}

/******************************************************************************
 ******************************************************************************/
void PunchBlock::insertPrefix(const std::string & key, const std::string & value)
{
    m_prefixRowAndHeader[key] = value;
}

/******************************************************************************
 ******************************************************************************/
void PunchBlock::append(const PunchRow &row)
{
    if (row.empty())
        return;
    m_rows.push_back( row );
}

/******************************************************************************
 ******************************************************************************/
int PunchBlock::prefixCount() const
{
    return m_prefixRowAndHeader.size();
}

int PunchBlock::columnCount() const
{
    if (m_rows.empty())
        return 0;
    PunchRow row = m_rows.front();
    return row.size();
}

int PunchBlock::rowCount() const
{
    return m_rows.size();
}

/******************************************************************************
 ******************************************************************************/
std::map<std::string, std::string> PunchBlock::prefixRowAndHeader() const
{
    return m_prefixRowAndHeader;
}

std::list<PunchRow> PunchBlock::rows() const
{
    return m_rows;
}

/******************************************************************************
 ******************************************************************************/
string PunchBlock::hash() const
{
    string key;
    for (const std::pair<const string, string> &var : m_prefixRowAndHeader) {
        key += var.first;
        key += ",";
    }
    key += std::to_string( columnCount() );
    return key;
}


/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/
/*! \class PunchFile
 *  \brief Contains the data of a PUNCH format stream or file.
 *
 * The PunchFile is a dictionary (multi-map) that stores one or several PunchBlock.
 *
 * A PunchBlock represents a continuous block of data in the PUNCH format stream.
 * These blocks are actually delimited by lines commented out with dollar symbol '$'.
 *
 * To insert a block, use the function \a append() or the operator '+='.
 *
 * To access the blocks, you need to get the hash key among those returned with \a blockKeys().
 * Then use \a blockRange() to get all the blocks using this key.
 *
 */
/*! \brief Constructor.
 */
PunchFile::PunchFile()
{
}

/******************************************************************************
 ******************************************************************************/
void PunchFile::append(const PunchBlock &block)
{
    auto key = block.hash();
    this->m_keys.emplace(key);
    this->m_blockMap.emplace(key, block);
}

/******************************************************************************
 ******************************************************************************/
std::set<std::string> PunchFile::blockKeys() const
{
    return m_keys;
}

PunchBlockRange PunchFile::blockRange(const std::string & key) const
{
    return m_blockMap.equal_range(key);
}

/******************************************************************************
 ******************************************************************************/
PunchFile& PunchFile::operator+=(const PunchFile& other)
{
    for (auto & key : other.blockKeys()) {
        auto pp = other.blockRange(key);
        for (auto p = pp.first; p != pp.second; ++p) {
            PunchBlock block = p->second;
            append(block);
        }
    }
    return *this;
}
