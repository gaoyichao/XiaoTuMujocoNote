#include <tinyxml2/tinyxml2.h>
#include <filesystem>
#include <strings.h>
#include <vector>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

tinyxml2::XMLElement * mjIncludeXML(tinyxml2::XMLElement* elem,
                                    std::string dir,
                                    std::vector<std::string>& included)
{
    if (!strcasecmp(elem->Value(), "include")) {
        std::cout << "lalalal" << std::endl;
        return NULL;
    } else {
        std::cout << "hahah" << std::endl;
        return NULL;
    }
}


int main(int argc, char * argv[])
{
    if (2 != argc) {
        std::cout << "./load_xml /path/to/xml" << std::endl;
        return -1;
    }

    auto p = fs::canonical(fs::absolute(argv[1]));
    std::cout << "加载:" << p << std::endl;
    std::cout << "p.parent:" << p.parent_path() << std::endl;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(p.c_str());

    tinyxml2::XMLElement* root = doc.RootElement();
    std::cout << "root:" << root->Value() << std::endl;

    // XMLElement 与 XMLNode 有啥区别?
    tinyxml2::XMLElement* eleminc = root->FirstChildElement();
    std::cout << "first child element:" << eleminc->Value() << std::endl;

    tinyxml2::XMLNode * first_child = root->FirstChild();
    std::cout << "first child:" << first_child->Value() << std::endl;
    std::cout << "first child:" << first_child->GetLineNum() << std::endl;

    std::vector<std::string> included;
    mjIncludeXML(root, p.parent_path(), included);
}

