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

#include <string>
#include <io.h>         // access()
#include <algorithm>    // std::transform()

using namespace std;


/******************************************************************************
 ******************************************************************************/

PunchRow* const PunchBlock::appendRow()
{
    m_rows.push_back( PunchRow() );
    return &(m_rows.back());
}

PunchRow* PunchBlock::currentRow() {
    if (!m_rows.empty())
        return &(m_rows.back());
    return 0;
}

/******************************************************************************
 ******************************************************************************/


PunchBlock* const PunchFile::appendBlock()
{
    m_blocks.push_back( PunchBlock() );
    return &(m_blocks.back());
}

std::list<PunchBlock>& PunchFile::blocks()
{
    return m_blocks;
}

