#ifndef _demonReg_H_
#define _demonReg_H_


//#include "RegistrationBase.h"
#include "readPar.h"



namespace itk {


class DemonReg
{


public:


    /* Standard calss typedefs */
    typedef DemonReg Self;
    //    typedef itk::Object Superclass;
    //    typedef SmartPointer< Self > Pointer;

    static Self * New()
    {
        return new Self;
    }

    /* Method for creation through the object factoAddry */
    //    itkNewMacro(Self);

    /* Runtime type information */
//    itkTypeMacro(BsplineMultiReg, AssignPointer);


    int Run(Par &par);
//    /* Update the filter with image type*/
//    void Update(Global &var, std::vector<Parameters> &par);
//    void Write(Global &var);


    DemonReg(){};
    //    BsplineMultiReg(const std::string &file1, const std::string &file2)
    //        :AssignPointer()
    //    {
    ////    this->fixedReader = _fixedReader;
    //    };
    ~DemonReg(){};




protected:




private:





};


} // namespace itk












#endif
