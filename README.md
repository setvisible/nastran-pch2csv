# Nastran Punch Reader (pch2csv)

## Description

A simple tool to convert [Nastran](https://en.wikipedia.org/wiki/Nastran "NASA Nastran")
Punch format into [CSV](https://en.wikipedia.org/wiki/Comma-separated_values "Comma-Separated Values (CSV)").

It is written in C++ and uses the
[C++ standard library](https://en.wikipedia.org/wiki/C%2B%2B_Standard_Library "C++ standard library").

## Features

The tool reads a Nastran Punch file (`*.pch`) and converts its scary format into
Comma-Separated Values (`*.csv`) readable by
[Calc](https://en.wikipedia.org/wiki/LibreOffice_Calc "LibreOffice Calc") or
[Excel](https://en.wikipedia.org/wiki/Microsoft_Excel "Microsoft Excel").


Basically, it converts this:

    $TITLE   = MY FEA MODEL                                                        1
    $SUBTITLE=MY FIRST LOAD CASE                                                   2
    $LABEL   =MY FIRST LOAD CASE                                                   3
    $ELEMENT STRESSES                                                              4
    $REAL OUTPUT                                                                   5
    $SUBCASE ID =         666                                                      6
         12345          80004230        BAR                                        7
    -CONT-                  2.288704E+04     -3.404367E+03      1.639255E+03       8
    -CONT-                 -7.163730E+04      9.975631E+05      3.060709E+06       9
         12345          80004231        BAR                                       10
    -CONT-                 -2.301775E+04     -3.107557E+03      4.195733E+02      11
    -CONT-                  7.232352E+04     -9.979151E+05     -3.062225E+06      12


into this:

    "TITLE";"SUBTITLE";"LABEL";"SUBCASE ID";unknown;unknown;unknown;unknown;unknown;unknown;unknown;unknown;unknown;unknown;
    "MY FEA MODEL";"MY FIRST LOAD CASE";"MY FIRST LOAD CASE";"666";12345;80004230;BAR;;2.288704E+04;-3.404367E+03;1.639255E+03;-7.163730E+04;9.975631E+05;3.060709E+06;
    "MY FEA MODEL";"MY FIRST LOAD CASE";"MY FIRST LOAD CASE";"666";12345;80004231;BAR;;-2.301775E+04;-3.107557E+03;4.195733E+02;7.232352E+04;-9.979151E+05;-3.062225E+06;



__Remark:__

The **Nastran solver** produces a PUNCH file as result of a
[FEA](https://en.wikipedia.org/wiki/Finite_element_analysis "Finite Element Analysis (FEA)")
when the `PUNCH` keyword replaces the default `PRINT` or `PLOT` output option, which is defined in the Finite Element model `CASE CONTROL` section.

For instance, Nastran creates a PUNCH file containing all the GRIDs displacement
if the FE model file contains the line `DISPLACEMENT(SORT1,PUNCH,REAL)=ALL`
in its `CASE CONTROL` section.

Punch are sometimes more 'readable' than the F06 (see `PRINT` default option).

__Remark 2:__

Punch formats aren't that bad, however several types of elements
(with different *formats*) can be stored inside (and also *totals*...),
so it can sometimes be tricky to convert them easily into a single 2-dimensional table.

The work-around found here is to output as many CSV as element / totals types there are.
It's not ideal but this allows a better reorganisation of the data.

In such case, the command:

    $ ./pch2csv input.pch -o output.csv
    Warning: pch2csv detected 4 different formats.
    Then, 4 files are produced.

will produce these files:

    output_format_0.csv
    output_format_1.csv
    output_format_2.csv
    output_format_3.csv

Note that the tool emits **warnings** when such case appears.
However this 'auto-split' feature can be disabled with `-u`.

## Under the hood

### Simple case

The simplest case is given below.
One punch file containing one format of data, is converted into an unique CSV.

![Simple case](images/unique.png)

### Concatenate several punch files into an unique output

If several files are proceeded (i.e. with the command `pch2csv file1 file2 file3`),
the tool concatenates them into an unique CSV if they only contain *one format* of data.
If not (i.e. when several formats are detected), pch2scv outputs one CSV file per format, except if the option `-u` is used.

![Concatenate the Punch](images/concat.png)

### Split formats to separated files

If the `-u` flag isn't specified, the tool writes each *format*
from Punch files to a separated CSV.

![Formats](images/combo.png)


## Build

1. Download `pch2csv.tar.gz`.

2. Unzip:

        $ tar zxvf pch2csv.tar.gz
        $ cd pch2csv/

3. Build with CMake

     - On Unix / Mac OS X

            $ mkdir -p build
            $ cd build/
            $ cmake ..
            $ make
            $ make install

     - On Windows (MinGW)

            > mkdir build
            > cd build/
            > cmake .. -G "MinGW Makefiles"
            > make

     - On Windows (MSVC)

            > mkdir build
            > cd build/
            > cmake .. -G "Visual Studio 14 2015"
            > start .

         Then, double click the Visual Studio project (vcxproj).


## Usage

On Unix / Mac OS X

    $ ./pch2csv input.pch -o output.csv

On Windows

    > pch2csv.exe input.pch -o output.csv

__Remarks:__

 - By default, CSV aren't overwritten.
   Instead, *pch2csv* increments the former CSV (backup).
 - Additionally, *pch2csv* writes user-defined column titles in CSV with `-c` option.


__Options:__

 - `-h`, `--help`    
   Show the help message and exit

 - `-v`, `--version`    
   Show the version

 - `-o OUTPUT`, `--output=OUTPUT`    
   Specify the name of the output file.
   By default, the output takes the same name as the input, with `.csv` extension

 - `-c %HEADER%`, `--column-header=%HEADER%`    
   Specify the csv header content. By default, the column headers are *unknown*.
   %HEADER% must be a sequence of words separated by commas,
   surrounded by a double-quote, for instance: `-c "CBUSH ID;;CS;X in mm;Y [mm];CSout"`

 - `-s`, `--skip-header`    
   Do not print the csv header. Data begins at the first line.

 - `-u`, `--unique`    
   Force the tool to produce an unique csv, even if several formats are detected.


## Similar work from Github's Community

At the time of writing these lines, some interesting things about
Nastran and PUNCH files can be found on Github:

 - The **nastran\_pch\_reader** by anick107, a Python Parser for PUNCH files:
 [https://github.com/anick107/nastran\_pch\_reader](https://github.com/anick107/nastran\_pch\_reader)

 - The **pyNastran** by SteveDoyle2, a Python-based interface tool for Nastran's file formats:
 [https://github.com/SteveDoyle2/pyNastran](https://github.com/SteveDoyle2/pyNastran)
 (in particular, see pyNastran/bdf/test/unit/test\_read\_write.py)

Punch files found in those projects are used here, mainly for demo and unit testing.

## License

The code is released under the GNU **LGPLv3** open source license.

[https://www.gnu.org/licenses/lgpl-3.0.en.html](https://www.gnu.org/licenses/lgpl-3.0.en.html)
