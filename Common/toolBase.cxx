#include "toolBase.h"

#include <algorithm>


namespace std {



const string trim (const string &str, const string &whitespace)
{
    const size_t begin_str = str.find_first_not_of (whitespace);
    if (begin_str == string::npos)
    {
        // no content
        return "";
    }

    const size_t end_str = str.find_last_not_of(whitespace);
    const size_t range = end_str - begin_str + 1;

    return str.substr (begin_str, range);
}







// find substring (case insensitive)
size_t ci_find (const string& str1, const string& str2)
{
    string::const_iterator it = search (str1.begin(), str1.end(),
        str2.begin(), str2.end(), my_equal());
    if (it != str1.end()) return it - str1.begin();
    else return string::npos;
}








}
