#ifndef TOOLS_H
#define TOOLS_H


#include <algorithm>

//------------------------------------------------------------------------------
void qListToSTLVector(const QStringList& list,
                      std::vector<char*>& vector)
{
    // Resize if required
    if (list.count() != static_cast<int>(vector.size()))
    {
        vector.resize(list.count());
    }
    for (int i = 0; i < list.count(); ++i)
    {
        // Allocate memory
        char* str = new char[list[i].size()+1];
        strcpy(str, list[i].toLatin1());
        vector[i] = str;
    }
}

//------------------------------------------------------------------------------
namespace
{
static std::string qStringToSTLString(const QString& qstring)
{
    return qstring.toStdString();
}
}

//------------------------------------------------------------------------------
void qListToSTLVector(const QStringList& list,
                      std::vector<std::string>& vector)
{
    // To avoid unnessesary relocations, let's reserve the required amount of space
    vector.reserve(list.size());
    std::transform(list.begin(),list.end(),std::back_inserter(vector),&qStringToSTLString);
}

//------------------------------------------------------------------------------
void stlVectorToQList(const std::vector<std::string>& vector,
                      QStringList& list)
{
    std::transform(vector.begin(),vector.end(),std::back_inserter(list),&QString::fromStdString);
}














#endif // TOOLS_H
