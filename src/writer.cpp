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

#include <string>
#include <assert.h>
#include <ostream>

using namespace std;


/******************************************************************************
 ******************************************************************************/
bool Writer::writeCSV(const PunchFile_Ptr pch, std::ostream * const odevice)
{
    assert(odevice);
    assert(pch);

    string previousLeftHeaders;

    for (PunchBlock & block : pch->blocks()) {

        /* **************************************** */
        /* Prepare the header and the common rows   */
        /* **************************************** */
        string leftHeaders;
        string leftRows;
        for (std::pair<const string, string> &var : block.globalVars) {
            leftHeaders += var.first;
            leftHeaders += ",";
            leftRows += var.second;
            leftRows += ",";
        }
        if (!block.rows().empty()) {
            PunchRow r = block.rows().front();
            for (int i = r.size(); i>0; --i) {
                leftHeaders += "unknown,";
            }
        }

        /* **************************************** */
        /* Write the header                         */
        /* **************************************** */
        if (leftHeaders != previousLeftHeaders) {
            (*odevice) << leftHeaders;
            (*odevice) << std::endl;
            previousLeftHeaders = leftHeaders;
        }

        /* **************************************** */
        /* Write the rows                           */
        /* **************************************** */
        if (!block.rows().empty()) {
            for (PunchRow &row : block.rows()) {
                (*odevice) << leftRows;

                for (const auto& field : row) {

                    (*odevice) << field;
                    (*odevice) << ",";

                }
                (*odevice) << std::endl;
            }
        }
    }



}
