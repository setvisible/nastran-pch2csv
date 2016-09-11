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
#ifndef READER_H
#define READER_H

#include "punchfile.h"

#include <istream>
#include <string>
#include <vector>

/*!
 * C_ERROR_MESSAGES_SIZE
 *
 * We define a maximum of messages to be shown,
 * in order to avoid too many messages.
 */
#define C_ERROR_MESSAGES_SIZE 100


class Reader
{
public:
    explicit Reader();

    /* Read */
    PunchFile parsePUNCH(std::istream * const idevice);

    /* Get detailed warning messages, if any. */
    std::vector<std::string> getWarnings() const;

private:
    void warn(const int lineCounter, const std::string &message);
    std::vector<std::string> m_warningMessages;

};

#endif // READER_H

