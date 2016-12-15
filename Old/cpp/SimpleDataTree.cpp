#include "SimpleDataTree.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif __APPLE__
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

//----------------------------SimpleDataTree::Node----------------------------

SimpleDataTree::Node::Node(std::string key, std::string value){
	this->value = value;
    this->key = key;
}


SimpleDataTree::Node::~Node(){
	for (auto node : children){
//		delete node.second;
        delete node;
	}
}

void
SimpleDataTree::Node::addChild(std::string key, std::string value){
//	children[key] = new Node(value);
    children.push_back(new Node(key, value));
}

SimpleDataTree::Node* SimpleDataTree::Node::getChild(std::string key){
    for(auto node : children){
        if(node->getKey() == key){
            return node;
        }
    }
    
    return nullptr;
    
//	auto findIt = children.find(key);
//	if (findIt == children.end()){
//		return nullptr;
//	}
//	else{
//		return children[key];
//	}
}

std::string SimpleDataTree::Node::getValue(){
	return this->value;
}

std::string SimpleDataTree::Node::getKey(){
    return this->key;
}

void SimpleDataTree::Node::setValue(std::string value){
	this->value = value;
}

std::vector<std::string> SimpleDataTree::Node::getKeySet(){
	std::vector<std::string> keySet;
	for (auto node : children){
//		keySet.push_back(n.first);
        keySet.push_back(node->getKey());
	}
	return keySet;
}

bool SimpleDataTree::Node::hasKey(std::string key){
    for(auto node : children){
        if(node->getKey() == key){
            return true;
        }
    }
    return false;
//	return children.find(key) != children.end();
}

int SimpleDataTree::Node::getChildrenSize(){
    return static_cast<int>(this->children.size());
}

//------------------------------SimpleDataTree------------------------------

SimpleDataTree::SimpleDataTree(std::string treeName, std::string filePath){
	this->treeName = treeName;

	bool debug = false;
#if _WIN32
	#if _DEBUG
		debug = true;
	#else
		debug = false;
	#endif
	#elif __APPLE__
	#if DEBUG
		debug = true;
	#else
		debug = false;
	#endif
#endif

	std::string workingDirectory;

	if (debug){
		char cCurrentPath[FILENAME_MAX];

		if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		{
			return;
		}

		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

		workingDirectory = std::string(cCurrentPath);
	}
    
#if _WIN32
    parse(workingDirectory + "\\Debug\\" + filePath);
#elif __APPLE__
    parse(filePath);
#endif

}

SimpleDataTree::~SimpleDataTree(){
    for(auto it : root){
        delete it.second;
    }
}

void SimpleDataTree::insertData(std::string path, std::string value, Node* node){
    std::string::size_type index = path.find(".", 0);
    if(index == std::string::npos){
        std::cout << "adding child to " << path << ", " << value << std::endl;
        node->addChild(path, value);
    }
    else{
        std::string restPath = path.substr(index+1);
        std::string key = path.substr(0, index);
        insertData(restPath, value, node->getChild(key));
    }
}

std::string SimpleDataTree::popPath(std::string path, int diff){
    for(int i = 0; i<diff; i++){
        std::string::size_type index = path.find_last_of(".");
        path = path.substr(0, index);
    }
    return path;
}

std::string SimpleDataTree::getKey(std::string line){
    std::string::size_type colonIndex = line.find(":", 0);
    if(colonIndex == std::string::npos){
        return "";
    }
    else{
        return line.substr(0, colonIndex);
    }
}

bool SimpleDataTree::doesLineHasKey(std::string line){
	return false;
}

bool SimpleDataTree::doesLineHasValue(std::string line){
    std::string::size_type colonIndex = line.find(":", 0);
    if(colonIndex == std::string::npos){
        return false;
    }
    else{
        std::string key = line.substr(colonIndex + 1);
        for(int i = 0; i<key.size(); i++){
            std::string chStr = key.substr(i, 1);
            if(chStr == " " || chStr == "\t"){
                continue;
            }
            else{
                return true;
            }
        }
    }
    
	return false;
}

std::string SimpleDataTree::removeTab(std::string line){
    line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
    return line;
}

std::string SimpleDataTree::ignoreWhiteSpaceBeforeKey(std::string line){
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    
    //indexof
    std::string::size_type index = line.find(":", 0);
    if(index == std::string::npos){
        return "";
    }
    std::string front = line.substr(0, index);
    std::string rest = line.substr(index, line.length()-1);
    
    
    front.erase(std::remove(front.begin(), front.end(), ' '), front.end());
    
	return front + rest;
}

int SimpleDataTree::getDepth(std::string line){
    int depth = 0;
    for(int i = 0; i<line.size(); i++){
        std::string chStr = line.substr(i, 1);
        if(chStr == "\t"){
            depth++;
        }
        else if(chStr == ":"){
            break;
        }
    }
    return depth;
}

std::string SimpleDataTree::getValueFromPath(std::string path){
    if(path.empty()){
        return "";
    }
    
    std::vector<std::string> pathSplit = split(path, '.');
    
    if(pathSplit.size() == 0){
        return "";
    }
    
    Node* target = root[treeName];
    
    for(int i = 0; i<pathSplit.size(); i++){
        target = target->getChild(pathSplit.at(i));
        if(target != nullptr){
            continue;
        }
        else{
            return "";
        }
    }
    
	return target->getValue();
}

std::string SimpleDataTree::getValue(std::string line){
    std::string::size_type colonIndex = line.find(":");
    if(colonIndex == std::string::npos){
        return "";
    }
    
    std::string valueStr = line.substr(colonIndex + 1);
    std::string chStr = valueStr.substr(0, 1);

    while(chStr == " " || chStr == "\t"){
        if(chStr == " " || chStr == "\t"){
            valueStr = valueStr.substr(1);
        }
        chStr = valueStr.substr(0, 1);
    }
    
    return valueStr;
}

bool SimpleDataTree::hasKey(std::string path){
    std::vector<std::string> pathSplit = split(path, '.');
    
    unsigned int index = 0;
    Node* root = this->root[treeName];
    
    while(index < pathSplit.size()){
        if(root->hasKey(pathSplit.at(index))){
            root = root->getChild(pathSplit.at(index));
            index++;
        }
        else{
            return false;
        }
    }
    
    return true;
}

void SimpleDataTree::setValue(std::string path, std::string value){
    bool hasPath = hasKey(path);
    std::vector<std::string> pathSplit = split(path, '.');
    unsigned int index = 0;
    Node* root = this->root[treeName];
    
    if(hasPath){
        while(index < pathSplit.size()){
            if(root->hasKey(pathSplit.at(index))){
                root = root->getChild(pathSplit.at(index));
                index++;
            }
            else{
                break;
            }
        }
        root->setValue(value);
    }
    else{
        std::string curPath = "";
        
        while(index < pathSplit.size()){
            std::string curKey = pathSplit.at(index);
            if(index == 0){
                curPath = curKey;
            }
            else{
                curPath += ("." + curKey);
            }
            
            if(index == pathSplit.size() - 1){
                insertData(curPath, value, this->root[treeName]);
                index++;
            }
            else{
                bool pathCheck = root->hasKey(curKey);
                if(pathCheck){
                    root = root->getChild(curKey);
                    index++;
                }
                else{
                    insertData(curPath, "", this->root[treeName]);
                    index++;
                }
            }
        }
    }
}

std::string SimpleDataTree::parseDataToString(Node* node, int depth){
    std::string data = "";
    if(node->getChildrenSize() == 0){
        return node->getValue() + "\n";
    }
    else{
        std::vector<std::string> keySet = node->getKeySet();
        int count = 0;
        for(auto key : keySet){
            if(node->getChild(key)->getChildrenSize() != 0){
                data += (key + ": \n");
                for(int i = 0; i<=depth; i++){
                    data += "\t";
                }
            }
            else{
                data += (key + ": ");
            }
            
            data += parseDataToString(node->getChild(key), depth + 1);
            if(count < keySet.size() - 1){
                for(int i = 0; i<depth; i++){
                    data += "\t";
                }
            }
            count++;
        }
        return data;
    }
}

void SimpleDataTree::clear(){
	for (auto n : root){
		delete n.second;
	}

	root.clear();

//	root.insert(std::pair<std::string, Node*>(this->treeName, new Node(std::string())));
    root.insert(std::pair<std::string, Node*>(this->treeName, new Node(treeName, std::string())));
}

void SimpleDataTree::writeFile(std::string path){
    std::string data = parseDataToString(root[treeName], 0);
    std::ofstream out(path);
    out << data;
    out.close();
}

std::vector<std::string>& SimpleDataTree::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> SimpleDataTree::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void SimpleDataTree::parseTree(std::string data){
    std::cout << "parsing " << data << std::endl;
	char delim = '\n';

	std::vector<std::string> splitRet = split(data, delim);

	std::string curPath("");
	int curDepth = 0;

	for (int i = 0; i < splitRet.size(); i++){
		splitRet.at(i) = ignoreWhiteSpaceBeforeKey(splitRet.at(i));
        if(splitRet.at(i) == ""){
            continue;
        }
        
        int depth = getDepth(splitRet.at(i));
        splitRet.at(i) = removeTab(splitRet.at(i));
        
        bool hasValue = doesLineHasValue(splitRet.at(i));
        
        
        if(curDepth == depth){
            if(curPath == "" && depth == 0){
                curPath = getKey(splitRet.at(i));
            }
            else{
                curPath = popPath(curPath, 1);
                curPath += ("." + getKey(splitRet.at(i)));
            }
        }
        else if(curDepth < depth){
            curDepth = depth;
            curPath += ("." + getKey(splitRet.at(i)));
        }
        else if(curDepth > depth){
            int diff;
            if(depth != 0){
                diff = curDepth - depth + 1;
                curPath = popPath(curPath, diff);
                curDepth = depth;
                curPath += "." + getKey(splitRet.at(i));
            }
            else{
                curDepth = depth;
                curPath = getKey(splitRet.at(i));
            }
        }
        
        std::cout << "cur path = " << curPath << ", cur depth = " << curDepth << std::endl;;
        
        if(hasValue){
            insertData(curPath, getValue(splitRet.at(i)), root[treeName]);
        }
        else{
            insertData(curPath, "", root[treeName]);
        }
        
        std::cout << std::endl;
	}
}

void SimpleDataTree::parse(std::string filePath){
	clear();

	std::string data;
	std::ifstream file;
	file.open(filePath, std::ios::in);
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		auto size = file.tellg();
		data.resize(size, ' ');
		file.seekg(0);
		file.read(&data[0], size);
	}

	parseTree(data);
}

int SimpleDataTree::getInt(std::string path){
    return getInt(path, 0);
}

int SimpleDataTree::getInt(std::string path, int defaultValue){
    std::string treeValue = getValueFromPath(path);
    
    if(treeValue == "" || treeValue == "null" || treeValue == "NULL"){
        if(defaultValue == 0){
            return 0;
        }
        else{
            return defaultValue;
        }
    }
    else{
        int number = 0;
        try {
            number = std::stoi(treeValue);
            return number;
        }
        catch (std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            return 0;
        }
        catch (std::out_of_range& e){
            std::cout << e.what() << std::endl;
            return 0;
        }
    }
}

float SimpleDataTree::getFloat(std::string path){
    return getFloat(path, 0);
}

float SimpleDataTree::getFloat(std::string path, float defaultValue){
    std::string treeValue = getValueFromPath(path);
    
    if(treeValue == "" || treeValue == "null" || treeValue == "NULL"){
        if(defaultValue == 0){
            return 0;
        }
        else{
            return defaultValue;
        }
    }
    else{
        int number = 0;
        try {
            number = std::stof(treeValue);
            return number;
        }
        catch (std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            return 0;
        }
        catch (std::out_of_range& e){
            std::cout << e.what() << std::endl;
            return 0;
        }
    }
}

double SimpleDataTree::getDouble (std::string path){
    return getDouble(path, 0);
}

double SimpleDataTree::getDouble(std::string path, double defaultValue){
    std::string treeValue = getValueFromPath(path);
    
    if(treeValue == "" || treeValue == "null" || treeValue == "NULL"){
        if(defaultValue == 0){
            return 0;
        }
        else{
            return defaultValue;
        }
    }
    else{
        int number = 0;
        try {
            number = std::stod(treeValue);
            return number;
        }
        catch (std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            return 0;
        }
        catch (std::out_of_range& e){
            std::cout << e.what() << std::endl;
            return 0;
        }
    }
}

bool SimpleDataTree::getBool(std::string path){
    return getBool(path, false);
}

bool SimpleDataTree::getBool(std::string path, bool defaultValue){
    std::string treeValue = getValueFromPath(path);
    
    if(treeValue == "" || treeValue == "null" || treeValue == "NULL"){
        return false;
    }
    else{
        std::transform(treeValue.begin(), treeValue.end(), treeValue.begin(), ::tolower);
        if(treeValue == "true"){
            return true;
        }
        else if(treeValue == "false"){
            return false;
        }
        else{
            return false;
        }
    }
}

std::string SimpleDataTree::getString(std::string path){
    return getString(path, "");
}

std::string SimpleDataTree::getString(std::string path, std::string defaultValue){
    std::string treeValue = getValueFromPath(path);
    
    if(treeValue == "" || treeValue == "null" || treeValue == "NULL"){
        return "";
    }
    else{
        return treeValue;
    }
}

void SimpleDataTree::setInt(std::string path, int value){
    setValue(path, std::to_string(value));
}

void SimpleDataTree::setFloat(std::string path, float value){
    setValue(path, std::to_string(value));
}

void SimpleDataTree::setDouble(std::string path, double value){
    setValue(path, std::to_string(value));
}

void SimpleDataTree::setBool(std::string path, bool value){
    if(value){
        setValue(path, "true");
    }
    else{
        setValue(path, "false");
    }
}

void SimpleDataTree::setString(std::string path, std::string value){
    setValue(path, value);
}
