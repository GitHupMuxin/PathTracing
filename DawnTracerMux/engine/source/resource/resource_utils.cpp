#include <iostream>
#include <sstream>
#include "engine/resource/resource_utils.h"


namespace resource
{
    std::vector<std::string> ResourceUtils::StringSplit(const std::string & str, char delim)
    {
        std::stringstream ss(str);
        std::vector<std::string> elems;
        std::string s;
        while (getline(ss, s, delim))
            elems.emplace_back(s);
        return elems;
    }



}

