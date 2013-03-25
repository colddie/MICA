#ifndef _ArgumentsParse_H_
#define _ArgumentsParse_H_

#include "itkObject.h"


#include <string>
#include <vector>
#include <algorithm>

using namespace std;


class ArgumentParser : public itk::Object
{
public:

    ArgumentParser(){
    m_args.clear();
    m_required.clear();
    m_optional_required.clear();
    };
    ~ArgumentParser(){};

    /* parse the commandline arguments */
    void SetCommandLineArg(int argc, char *argv[], const char *help[]);

    /* -key with no arguments, return bool */
    bool BoolParse(const string &arg) const;

    /* -key with one arguument, return string */
    string OneParse(const string &arg);


    /* -key with multiple arguments, return vector (overload)*/
    vector<string> MultiParse(const string &arg);

    // have to overload here
    template <class T>
    inline vector<T> MultiParse(const string &arg, const string &isnum)
    {
        vector<string>::iterator iter;
        iter = find(m_args.begin(), m_args.end(),arg);

        vector<T> numvec;
        string str = *(iter+1);
        stringstream stream(str);
        T num;
        while(stream >> num) { numvec.push_back(num); }

        return numvec;
    }



    /* mark the required arguments */
    void MarkAsRequired(const string &arg);

    void MarkOneOfAsRequired(const vector<string> &vec);

    /* check if all required arguments found */
    bool IsRequiredFound() const;

    /* check if argument is a negative number, for MultiParse function */
    bool IsANumber(const string &arg) const;


    /* delete -key with one argument from vector*/
    void DeleteArg(vector<string> &vec, const string &arg);

    /* delete -key without arguments from vector */
    void DeleteArg(vector<string> &vec, const string &arg, const char *isbool);







protected:



private:
    vector<string> m_args;
    vector<string> m_required;
    vector<string> m_optional_required;


};
#endif
