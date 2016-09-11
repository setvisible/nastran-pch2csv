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
#include "reader.h"

#include <assert.h>

using namespace std;

/*! \class Reader
 *  \brief The class Reader is a parser for PUNCH format streams.
 *
 * The Reader stores the data into a \a PunchFile.
 * Use \a parsePUNCH() to parse a stream.
 *
 * To check the errors after the parsing, use \a getWarnings().
 *
 */
/*! \brief Constructor.
 */
Reader::Reader()
{
    m_warningMessages.reserve(C_ERROR_MESSAGES_SIZE);
}

/******************************************************************************
 ******************************************************************************/
std::vector<string> Reader::getWarnings() const
{
    return m_warningMessages;
}


/******************************************************************************
 ******************************************************************************/
void Reader::warn(const int lineCounter, const std::string &message)
{
    if (m_warningMessages.size() == C_ERROR_MESSAGES_SIZE) {
        std::string limitMsg("[Warning] Too many errors...");
        m_warningMessages.push_back( limitMsg );
    }
    if (m_warningMessages.size() >= C_ERROR_MESSAGES_SIZE) {
        return;
    }

    std::string msg("[Warning] line "
                    + std::to_string(lineCounter)
                    + ": "
                    + message);
    m_warningMessages.push_back(msg);
}

/******************************************************************************
 ******************************************************************************/

static inline std::string trim_right(const std::string &str, const std::string &delimiters)
{
    auto strEnd = str.find_last_not_of(delimiters);
    auto strRange = strEnd + 1;
    return str.substr(0, strRange);
}

static inline std::string trim_left(const std::string &str, const std::string &delimiters)
{
    auto strBegin = str.find_first_not_of(delimiters);
    if (strBegin == std::string::npos) {
        return string();  /* no content */
    }
    auto strEnd = str.length();
    auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

static inline std::string trim(const std::string &str, const std::string &delimiters)
{
    return trim_left( trim_right( str, delimiters ), delimiters );
}

/******************************************************************************
 ******************************************************************************/
static inline bool startsWith(const std::string &text, const std::string &start)
{
    if (text.length() == 0 || start.length() == 0 || start.length() > text.length() )
        return false;
    auto p = text.begin();
    auto q = start.begin();
    while ( q != start.end() ) {
        if ((*p) != (*q))
            return false;
        p++;
        q++;
    }
    return true;
}

/******************************************************************************
 ******************************************************************************/
static void trimRightRow(PunchRow * const row)
{
    if (!row) return;
    int i = row->size();
    while (i>0) {
        --i;
        if ( row->at(i) != "" ) return;
        row->pop_back();
    }
}

/******************************************************************************
 ******************************************************************************/
PunchFile Reader::parsePUNCH(std::istream * const idevice)
{
    assert(idevice);

    PunchFile pch;

    std::list<PunchBlock> blocks;

    PunchRow currentRow;

    PunchBlock *currentblock = 0;
    bool isHeaderSection = false;

    int lineCounter = 0;
    string line;
    while( std::getline((*idevice), line) ) {

        ++lineCounter;

        /* Not a punch line */
        if (line.length() != 80) {
            this->warn(lineCounter, "A line should have at least 80 characters.");
            continue;
        }

        /* ********************* */
        /* Dollarized Section    */
        /* ********************* */
        if( line.front() == '$' ){

            /* Flush */
            trimRightRow( &currentRow );
            currentblock->append( currentRow );
            currentRow = PunchRow();

            if (!isHeaderSection) {

                blocks.push_back( PunchBlock() );
                currentblock = &(blocks.back());

                isHeaderSection = true;
            }

            auto p_equal = line.find( '=' );
            if (p_equal != string::npos ){

                string key   = line.substr( 1, p_equal - 1);
                string value = line.substr(p_equal + 1, 80 - 9 - (p_equal + 1));

                string key_trimmed   = trim(key, " \t");
                string value_trimmed = trim(value, " \t" );

                // insert or assign to element
                currentblock->insertPrefix(key_trimmed, value_trimmed);


            }
            continue;
        }
        isHeaderSection = false;

        /* ********************* */
        /* Data Block Section    */
        /* ********************* */
        if (!currentblock) {
            this->warn(lineCounter, "A header ('$' section) should prepend the data.");

            blocks.push_back( PunchBlock() );
            currentblock = &(blocks.back());
        }

        /* Fields are 18 char-long */
        std::vector<std::string> fields;
        for(int i = 0; i < 4; ++i) {
            auto f= line.substr(i*18, 18);
            f = trim( f, " \t\r\n" );
            fields.push_back( f );
        }


        if( startsWith(fields[0], string("-CONT-")) ) {
            if (currentRow.size() == 0) {
                this->warn(lineCounter, "A continued -CONT- field shouldn't starts a new block.");
            }
            /* skip fields[0] */
            currentRow.push_back( fields[1] );
            currentRow.push_back( fields[2] );
            currentRow.push_back( fields[3] );

        } else {
            /* Flush */
            trimRightRow( &currentRow );
            currentblock->append( currentRow );
            currentRow = PunchRow();

            currentRow.push_back( fields[0] );
            currentRow.push_back( fields[1] );
            currentRow.push_back( fields[2] );
            currentRow.push_back( fields[3] );
        }
    }

    /* Flush */
    trimRightRow( &currentRow );
    currentblock->append( currentRow );
    //currentRow = PunchRow();

    for (PunchBlock & block : blocks) {
        pch.append( block );
    }

    return pch;
}

