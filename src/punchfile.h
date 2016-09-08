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
#ifndef PUNCH_FILE_H
#define PUNCH_FILE_H

#include <memory> // std::shared_ptr

#include <string>
#include <map>
#include <list>
#include <deque>


typedef std::deque<std::string> PunchRow;

class PunchBlock
{
public:
    PunchRow* const appendRow();
    PunchRow* currentRow();

    std::list<PunchRow>& rows() { return m_rows; }

    std::string filename;
    std::map<std::string, std::string> globalVars;

private:
    std::list<PunchRow> m_rows;

};

class PunchFile
{
public:
    PunchBlock* const appendBlock();
    std::list<PunchBlock>& blocks();

private:
    std::list<PunchBlock> m_blocks;

};

typedef std::shared_ptr<PunchFile> PunchFile_Ptr;


#endif // PUNCH_FILE_H

