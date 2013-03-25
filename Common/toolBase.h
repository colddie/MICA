#ifndef _toolBase_H_
#define _toolBase_H_

#include <iostream>



using namespace std;








/** several useful tools and overloads */


/* overload ostream operator*/
template <typename T>
ostream& operator<<(ostream &os, const vector<T> &vec)
{
    typename vector<T>::const_iterator iter;
    for (iter=vec.begin(); iter!=vec.end(); iter++) {
        os<<*iter<<" ";
    }
    return os;
}



/* convert vector to stringstream */
template <typename T>
string vectorToString(const vector<T> &vec)
{
stringstream stream;
copy(vec.begin(), vec.end(), ostream_iterator<T>(stream, " "));

return stream.str();

}




/* convert itk matrix to stringstream */







#endif
