
#include <string>

class Project
{
public:
    Project(std::string path);
    ~Project();

private:
    std::string path = "";
    void parseProjectData();
};