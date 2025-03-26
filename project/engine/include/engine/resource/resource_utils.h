#include <vector>
#include <string>

namespace resource
{
    class ResourceUtils
    {
        public:
            static std::vector<std::string> StringSplit(const std::string& str, char delim);  
    };    
}


