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
    vector<double> MultiParse(const string &arg, const string &isnum);

    /* mark the required arguments */
    void MarkAsRequired(const string &arg);

    /* check if all required arguments found */
    bool IsRequiredFound() const;

    /* check if argument is a negative number, for MultiParse function */
    bool IsANumber(const string &arg) const;


    /* delete -key with one argument from vector*/
    void DeleteArg(vector<string> &vec, const string &arg);

    /* delete -key without arguments from vector */
    void DeleteBoolArg(vector<string> &vec, const string &arg);







protected:

    vector<string> m_args;
    vector<string> m_required;


private:



};
#endif
