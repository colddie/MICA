/** several useful tools and overloads */

#ifndef _toolBase_H_
#define _toolBase_H_

#include "itkArray2D.h"

#include <iostream>
#include <iterator>
#include <vector>




namespace std {



/* overload ostream operator*/
template <typename T>
inline ostream& operator<<(ostream &os, const vector<T> &vec)
{
    typename vector<T>::const_iterator iter;
    for (iter=vec.begin(); iter!=vec.end(); ++iter) {
        os<<*iter<<" ";
    }
    return os;
}




/* convert vector to stringstream */
template <typename T>
inline string vectorToString(const vector<T> &vec)
{
stringstream stream;
copy(vec.begin(), vec.end(), ostream_iterator<T>(stream, " "));

return stream.str();
}



///* convert itk matrix to stringstream */
//template <typename T, unsigned int N>
//ostream& operator<<(ostream &os, const itk::Matrix<T> &vec)
//{


//    return os;
//}




/* trim the whitespace in the beginning by GMan */
const string trim (const string& str, const string& whitespace);




/* Case-insensitive string::find() by Kirill V. Lyadvinsky */
struct my_equal {
    bool operator()(char ch1, char ch2) {
#ifdef _WIN32
      return toupper(ch1) == toupper(ch2);
#else
      return std::toupper(ch1) == std::toupper(ch2);
#endif
    }
};
// find substring (case insensitive)
size_t ci_find (const string& str1, const string& str2);




}



#endif
