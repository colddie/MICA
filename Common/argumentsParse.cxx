#include "argumentsParse.h"




/* -key with no arguments, return bool */
bool ArgumentParser::BoolParse(const string &arg) const
{

    if (find(m_args.begin(), m_args.end(), arg) !=  m_args.end()) {
        return true;
    } else {
        return false;
    }

}



/* -key with one arguument, return string */
string ArgumentParser::OneParse(const string &arg)
{
    vector<string>::iterator iter;
    iter = find(m_args.begin(), m_args.end(), arg);
    iter++;
    string returnstr = *iter;

    return returnstr;

}


/* -key with multiple arguments, return vector */
vector<string> ArgumentParser::MultiParse(const string &arg)
{
    vector<string>::iterator first, last, temp;
    last = m_args.end();
    first = find(m_args.begin(), m_args.end(),arg);
    for (temp = first+1; temp != m_args.end(); temp++ ) {
        if (strchr((*temp).c_str(), '-') != NULL) {

            if (!IsANumber(*(temp+1))) {
                last = temp;cout<<"negative detected";
                break;
            }
        }
    }
    //cout<<*last;

    //    last = find(first+1, m_args.end(), "-");   // need to check negative

    //    while (IsANumber(*(last+1)))
    //    {
    //        last = find(last, m_args.end(), "-");
    //    }

    vector<string> subvec(first+1, last);
    if (subvec.size()<2) { cout<<arg<<"wrong input number"<<endl; exit(1);}
    return subvec;

}

vector<double> ArgumentParser::MultiParse(const string &arg, const string &isnum)
{
    vector<string>::iterator iter;
    iter = find(m_args.begin(), m_args.end(),arg);
    //    for (temp = first+1; temp != m_args.end(); temp++ ) {
    //        if (strchr((*temp).c_str(), '-') != NULL) {

    //            if (!IsANumber(*(temp+1))) {
    //                last = temp;cout<<"negative detected";
    //                break;
    //            }
    //        }
    //    }
    //cout<<*last;

    //    last = find(first+1, m_args.end(), "-");   // need to check negative

    //    while (IsANumber(*(last+1)))
    //    {
    //        last = find(last, m_args.end(), "-");
    //    }
    vector<double> numvec;
    string str = *(iter+1);
    stringstream stream(str);
    double num;
    while(stream >> num) { numvec.push_back(num); }

    return numvec;

//    vector<string> subvec(first+1, last);
//    if (subvec.size()<2) { cout<<arg<<"wrong input number"<<endl; exit(1);}
//    return subvec;

}


/* mark the required arguments */
void ArgumentParser::MarkAsRequired(const string &arg)
{

    m_required.push_back(arg);

}


/* check if all required arguments found */
bool ArgumentParser::IsRequiredFound() const
{
    bool allrequiredexist = true;
    for(unsigned int i=1; i<m_required.size(); i++) {
        if (find(m_args.begin(), m_args.end(), m_required[i]) == m_args.end()) {
            cout<<m_required[i]<<" is required but not specified"<<endl;
            allrequiredexist = false;
        }
    }

    return allrequiredexist;
}


/* check if argument is a negative number, for MultiParse function */
bool ArgumentParser::IsANumber(const string &arg) const
{
    string cliphead = arg.substr(0, 1);
    bool isnumber = isdigit(cliphead.c_str()[0]);

    return isnumber;
}



/* delete -key with one argument from vector*/
void ArgumentParser::DeleteArg(vector<string> &vec, const string &arg)
{
    vector<string>::iterator iter;
    iter = find(vec.begin(), vec.end(),arg);
    if (iter == vec.end()) { cout<<arg<<"delete arg error"<<endl; exit(1); }
    iter = vec.erase(iter);
    iter++;
    vec.erase(iter);

}


/* delete -key without arguments from vector */
void ArgumentParser::DeleteBoolArg(vector<string> &vec, const string &arg)
{
    vector<string>::iterator iter;
    iter = find(vec.begin(), vec.end(),arg);
    if (iter == vec.end()) { cout<<arg<<"delete arg error"<<endl; exit(1); }
    vec.erase(iter);

}



/* parse the commandline arguments */
void ArgumentParser::SetCommandLineArg(int argc, char *argv[], const char *help[])
{
    m_args.reserve(argc-1);
    m_args.insert(m_args.begin(), argv+1, argv+argc);

    if (m_args.empty() || m_args.size() == 1) {
        cout<<(*help)<<endl; exit(EXIT_FAILURE);
    }

}
