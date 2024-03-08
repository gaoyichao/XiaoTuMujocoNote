#include <tinyxml2/tinyxml2.h>
#include <filesystem>
#include <strings.h>
#include <vector>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

/**
 * @brief 解析 MJCF 文件的引用文件
 * 
 * ${mujuco}/src/xml/xml.cc
 */
tinyxml2::XMLElement * mjIncludeXML(tinyxml2::XMLElement* elem,
                                    std::string dir,
                                    std::vector<std::string>& included,
                                    std::string prefix)
{
    if (!strcasecmp(elem->Value(), "include")) {
        if (!elem->NoChildren())
            throw std::runtime_error("include 标签不能没有子节点");

        // 获取引用文件路径
        const char* pstr = elem->Attribute("file");
        std::string filename = dir + "/" + pstr;
        std::cout << filename << std::endl;

        // 过滤重复的引用文件
        for (size_t i = 0; i < included.size(); i++) {
            if (filename == included[i])
                throw std::runtime_error("重复引用文件");
        }
        
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        included.push_back(filename);

        tinyxml2::XMLElement* docroot = doc.RootElement();
        tinyxml2::XMLElement* eleminc = docroot->FirstChildElement();

        std::cout << ">>>> " << docroot->Value() << std::endl;
        std::cout << ">>>> " << eleminc->Value() << std::endl;
        
        tinyxml2::XMLElement* parent = (tinyxml2::XMLElement*)elem->Parent();
        tinyxml2::XMLNode* first = parent->InsertAfterChild(elem, eleminc->DeepClone(parent->GetDocument()));
        parent->DeleteChild(elem);
        elem = first->ToElement();
        eleminc = eleminc->NextSiblingElement();
        while (eleminc) {
            elem = (tinyxml2::XMLElement*)parent->InsertAfterChild(elem, eleminc->DeepClone(parent->GetDocument()));
            eleminc = eleminc->NextSiblingElement();
        }

        prefix = prefix + "::" + first->Value();
        std::cout << prefix << std::endl;
        return mjIncludeXML(first->ToElement(), dir, included, prefix);
    } else {
        tinyxml2::XMLElement * child = elem->FirstChildElement();
        while (child) {
            prefix = prefix + "::" + child->Value();
            std::cout << prefix << std::endl;
            
            child = mjIncludeXML(child, dir, included, prefix);
            if (child) {
                child = child->NextSiblingElement();
            }
        }
        return elem;
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

    std::cout << "---------------------" << std::endl;
    std::vector<std::string> included;
    mjIncludeXML(root, p.parent_path(), included, root->Value());

    auto name = std::string(root->Attribute("model"));
    std::cout << "模型名称:" << name << std::endl;

    if (root->FirstChild() && root->FirstChild()->ToComment()) {
        auto comment = std::string(root->FirstChild()->Value());
        std::cout << "注释:" << comment << std::endl;
    }

    doc.SaveFile("nibienao.xml");
}

