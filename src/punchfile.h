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

#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>

typedef std::deque<std::string> PunchRow;

class PunchBlock
{
    friend class PunchFile;

public:
    explicit PunchBlock();

    /* Setters */
    void insertPrefix(const std::string & key, const std::string & value);
    void append(const PunchRow &row);

    /* Getters */
    int prefixCount() const;
    int columnCount() const;
    int rowCount() const;
    std::map<std::string, std::string> prefixRowAndHeader() const;
    std::list<PunchRow> rows() const;

protected:
    std::string hash() const;

private:
    std::list<PunchRow> m_rows;
    std::map<std::string, std::string> m_prefixRowAndHeader;

};

typedef std::multimap<std::string, PunchBlock> PunchBlockMMap;
typedef std::pair<PunchBlockMMap::const_iterator, PunchBlockMMap::const_iterator> PunchBlockRange;

class PunchFile
{
public:
    explicit PunchFile();

    /* Setters */
    void append(const PunchBlock &block);

    /* Getters */
    std::set<std::string> blockKeys() const;
    PunchBlockRange blockRange(const std::string & key) const;

    /* Operators */
    PunchFile& operator+=(const PunchFile& other);

private:
    std::set<std::string> m_keys;
    PunchBlockMMap m_blockMap;
};


#endif // PUNCH_FILE_H

