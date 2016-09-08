#include "filemanager.h"

#include <string>
#include <io.h>         // access()
#include <algorithm>    // std::transform()
#include <assert.h>

using namespace std;

/*******************************************************************************
 *******************************************************************************/
/*! \brief Return true if the given \a filename ends with the given \a suffix.
 * Otherwise return false.
 *
 * \remark The function is case insensitive.
 */
bool FileManager::hasSuffix(const std::string &filename, const std::string &suffix)
{
    assert(!filename.empty());
    assert(!suffix.empty());

    if (suffix.size() > filename.size() )
        return false;
    string upper_filename = filename;
    std::transform( upper_filename.begin(), upper_filename.end(), upper_filename.begin(), ::toupper);

    auto pos = upper_filename.size() - suffix.size();
    auto res = upper_filename.compare(pos, suffix.size(), suffix);

    return (res == 0);
}


/*******************************************************************************
 *******************************************************************************/
/*! \brief Ensures that the given \a filename is available.
 * If the file exists, the function moves it (= backup).
 */
int FileManager::doBackup(const string &filename)
{
    assert(!filename.empty());

    if( access( filename.c_str(), F_OK ) != -1 ) {

        /* if 'filename' exists */
        const string prefix = filename.substr(0, filename.length() - 4) + string(".bak_");
        const string suffix = string(".csv");
        string newname;
        int i = 0;
        do {
            newname = prefix + std::to_string(i) + suffix;
            ++i;
        } while ( access( newname.c_str(), F_OK ) != -1 );

        return std::rename( filename.c_str() , newname.c_str() );
    }
    return 0;
}
