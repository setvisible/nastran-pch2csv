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

#include "filemanager.h"
#include "reader.h"
#include "writer.h"
#include "version.h"

#include <assert.h>
#include <fstream>
#include <getopt.h>
#include <iostream> // std::cout
#include <stdio.h>
#include <string>


using namespace std;

void usage()
{
    cout << endl;
    cout << "- pch2csv - " << endl;
    cout << endl;
    cout << " [DESCRIPTION] " << endl;
    cout << "    Convert Nastran Punch format to Comma-Separated Values (CSV)." << endl;
    cout << endl;
    cout << " [USAGE] " << endl;
    cout << "    pch2csv [options] filename" << endl;
    cout << endl;
    cout << " [OPTIONS]" << endl;
    cout << "    -h, --help" << endl;
    cout << "        Show this help message." << endl;
    cout << endl;
    cout << "    -v or --version" << endl;
    cout << "        Show the version." << endl;
    cout << endl;
    cout << "    -o OUTPUT, --output=OUTPUT " << endl;
    cout << "        Specify the name of the output file. " << endl;
    cout << "        By default, the output takes the same name " << endl;
    cout << "        as the input, with .csv extension." << endl;
    cout << endl;
    cout << "    -c HEADER, --column-header=HEADER " << endl;
    cout << "        Specify the csv header content." << endl;
    cout << "        By default, the column headers are 'unknown'." << endl;
    cout << "        HEADER must be a sequence of words separated by commas, " << endl;
    cout << "        surrounded by a double-quote, for instance: " << endl;
    cout << "            -c \"CBUSH ID;;CS;X in mm;Y [mm];CSout\"" << endl;
    cout << endl;
    cout << "    -s, --skip-header " << endl;
    cout << "        Do not print the csv header. Data begins at the first line." << endl;
    cout << endl;
    cout << "    -u, --unique " << endl;
    cout << "        Force the tool to produce an unique csv, even if several" << endl;
    cout << "        element types / totals are detected." << endl;
    cout << endl;
}

void version()
{
    cout << endl;
    cout << "- pch2csv - Copyright (C) 2016 Sebastien Vavassori" << endl;
    cout << "Version " << APP_VERSION_LONG << " build " << __DATE__ << " " << __TIME__ << endl;
    cout << endl;
    cout << "This program is free software: you can redistribute it and/or modify" << endl;
    cout << "it under the terms of the GNU Lesser General Public License as published by" << endl;
    cout << "the Free Software Foundation, either version 3 of the License, or" << endl;
    cout << "(at your option) any later version." << endl;
    cout << endl;
    cout << "This program is distributed in the hope that it will be useful," << endl;
    cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
    cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl;
    cout << "GNU Lesser General Public License for more details." << endl;
    cout << endl;
    cout << "You should have received a copy of the GNU Lesser General Public License" << endl;
    cout << "along with this program.  If not, see <http://www.gnu.org/licenses/>." << endl;
    cout << endl;
}


/*******************************************************************************
 *******************************************************************************/
int main( int argc, char *argv[] )
{
    vector<string> filenames;
    string columnHeaderLine;
    string output;
    bool mustOutputBeUnique = false;
    bool skipColumnHeaders = false;

    int c;
    while (1) {
        static struct option long_options[] = {
        { "help"           , no_argument        , nullptr, 'h'},
        { "version"        , no_argument        , nullptr, 'v'},
        { "output"         , required_argument  , nullptr, 'o'},
        { "column-header"  , required_argument  , nullptr, 'c'},
        { "skip-header"    , no_argument        , nullptr, 's'},
        { "unique"         , no_argument        , nullptr, 'u'},
        {nullptr, 0, nullptr, 0}
    };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long(argc, argv, "hvo:c:su", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;

        case 'h':
            usage();
            exit(0);
            break;

        case 'v':
            version();
            exit(0);
            break;

        case 'o':
            output = optarg;
            break;

        case 'c':
            columnHeaderLine = optarg;
            break;

        case 's':
            skipColumnHeaders = true;
            break;

        case 'u':
            mustOutputBeUnique = true;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
            break;
        }
    }

    /* Remaining command line arguments */
    if (optind < argc) {
        while (optind < argc) {
            filenames.push_back( argv[optind++] );
        }
    }


    /* *********************************************** */
    /* Check the options                               */
    /* *********************************************** */
    if (filenames.empty()) {
        cerr << "Error: Need an argument; type '-h' for details." << endl;
        exit(EXIT_FAILURE);
    }

    for (auto& filename : filenames) {
        if (!FileManager::hasSuffix(filename, ".PCH")) {
            cerr << "Error: The file must have a '.pch' extension. "
                 << "Wrong extension in '" << filename << "'. "
                 << "Maybe not a punch file?" << endl;
            cerr << "Error: Type '-h' for details." << endl;
            exit(EXIT_FAILURE);
        }
    }

    /* *********************************************** */
    /* Set the output file                             */
    /* *********************************************** */
    assert(!filenames.empty());
    if (output.empty()) {
        string filename = filenames.front();
        output = filename.substr(0, filename.length() - 4);
        output += string(".csv");
    }
    if (!FileManager::doBackup(output)) {
        cerr << "Error: Backup failed, cannot move '" << output << "'." << endl;
        exit(EXIT_FAILURE);
    }


    /* *********************************************** */
    /* Do the conversion                               */
    /* *********************************************** */
    PunchFile pch;

    for (auto& filename : filenames) {

        ifstream ifs;
        ifs.open( filename.c_str(), std::ios::in | std::ios::binary );
        if( !ifs.is_open() ){
            cerr << "Error: Cannot open the file '" << filename << "'." << endl;
        } else {

            Reader reader;
            PunchFile p = reader.parsePUNCH( &ifs );
            pch += p;

            for (auto& msg : reader.getWarnings()) {
                std::cerr << msg << std::endl;
            }

            ifs.close();
        }
    }

    if (mustOutputBeUnique) {

        ofstream ofs;
        ofs.open( output.c_str() );
        if( !ofs.is_open() ){
            cerr << "Error: Cannot write the file '" << output << "'." << endl;
            exit(EXIT_FAILURE);
        } else {

            Writer writer(columnHeaderLine, skipColumnHeaders);

            bool converted = true;
            for (auto & key : pch.blockKeys()) {
                auto br = pch.blockRange(key);
                for (auto b = br.first; b != br.second; ++b) {
                    PunchBlock block = b->second;
                    converted &= writer.writeCSV(block, &ofs);
                }
            }
            ofs.close();

            if( !converted ) {
                cerr << "Error: scanner encountered an error." << endl;
                exit(EXIT_FAILURE);
            } else {
                cout << "file output: '" << output << "'." << endl;
            }
        }

    } else {

        bool converted = true;
        int i = 0;
        for (auto & key : pch.blockKeys()) {

            string outputIncr = FileManager::formatIncrement(output, i);
            FileManager::doBackup( outputIncr );

            ofstream ofs;
            ofs.open( outputIncr.c_str() );
            if( !ofs.is_open() ){
                cerr << "Error: Cannot write the file '" << outputIncr << "'." << endl;
                exit(EXIT_FAILURE);
            } else {

                Writer writer(columnHeaderLine, skipColumnHeaders);

                auto pp = pch.blockRange(key);
                for (auto p = pp.first; p != pp.second; ++p) {
                    PunchBlock block = p->second;
                    converted &= writer.writeCSV(block, &ofs);
                }

                ofs.close();
            }

            i++;
        }

        if( !converted ) {
            cerr << "Error: scanner encountered an error." << endl;
            exit(EXIT_FAILURE);
        } else {
            cout << "Warning: pch2csv detected " << to_string(i) << " different formats." << endl;
            cout << "Then, " << to_string(i) << " files are produced. " << endl;
        }

    }
    exit(EXIT_SUCCESS);

}
