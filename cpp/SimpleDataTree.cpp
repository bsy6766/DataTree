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

SimpleDataTree::Node::Node(std::string value){
	this->value = value;
}


SimpleDataTree::Node::~Node(){
	for (auto node : children){
		delete node.second;
	}
}

void
SimpleDataTree::Node::addChild(std::string key, std::string value){
	children[key] = new Node(value);
}

SimpleDataTree::Node* SimpleDataTree::Node::getChild(std::string key){
	auto findIt = children.find(key);
	if (findIt == children.end()){
		return nullptr;
	}
	else{
		return children[key];
	}
}

std::string SimpleDataTree::Node::getValue(){
	return this->value;
}

void SimpleDataTree::Node::setValue(std::string value){
	this->value = value;
}

std::vector<std::string> SimpleDataTree::Node::getKeySet(){
	std::vector<std::string> keySet;
	for (auto n : children){
		keySet.push_back(n.first);
	}
	return keySet;
}

bool SimpleDataTree::Node::hasKey(std::string key){
	return children.find(key) != children.end();
}

unsigned int SimpleDataTree::Node::getChildrenSize(){
	return this->children.size();
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

	parse(workingDirectory + "\\Debug\\" + filePath);
}

SimpleDataTree::~SimpleDataTree(){

}

void SimpleDataTree::insertData(std::string path, std::string value, Node* node){

}

std::string SimpleDataTree::popPath(std::string path, int diff){
	return "";
}

std::string SimpleDataTree::getKey(std::string line){
	return "";
}

bool SimpleDataTree::doesLineHasKey(std::string line){
	return false;
}

bool SimpleDataTree::doesLineHasValue(std::string line){

	return false;
}

std::string SimpleDataTree::removeTab(std::string line){

	return "";
}

std::string SimpleDataTree::ignoreWhiteSpaceBeforeKey(std::string line){

	return "";
}

int SimpleDataTree::getDepth(std::string line){
	return -1;
}

std::string SimpleDataTree::getValueFromPath(std::string path){

	return "";
}

void SimpleDataTree::setValue(std::string path, std::string value){

}

std::string SimpleDataTree::parseDataToString(Node* node, int depth){

	return "";
}

void SimpleDataTree::clear(){
	for (auto n : root){
		delete n.second;
	}

	root.clear();

	root.insert(std::pair<std::string, Node*>(this->treeName, new Node(std::string())));
}

void SimpleDataTree::writeFile(std::string path){

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
	char delim = '\n';

	std::vector<std::string> splitRet = split(data, delim);

	std::string curPath("");
	int curDepth = 0;

	for (int i = 0; i < splitRet.size(); i++){
		splitRet.at(i) = ignoreWhiteSpaceBeforeKey(splitRet.at(i));
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

	return -1;
}

float SimpleDataTree::getFloat(std::string path){

	return -1;
}

double SimpleDataTree::getDouble(std::string path){

	return -1;
}

bool SimpleDataTree::getBool(std::string path){
	return false;
}

std::string SimpleDataTree::getString(std::string path){

	return "";
}


void SimpleDataTree::setInt(std::string path, int value){

}

void SimpleDataTree::setFloat(std::string path, float value){

}

void SimpleDataTree::setDouble(std::string path, double value){

}

void SimpleDataTree::setBool(std::string path, bool value){

}

void SimpleDataTree::setString(std::string path){

}

void SimpleDataTree::setString(std::string path, std::string value){

}
