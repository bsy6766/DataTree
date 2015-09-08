#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class SimpleDataTree{
private:
	class Node{
	private:
		std::string value;
		std::unordered_map<std::string, Node*> children;
	public:
		Node(std::string value);
		~Node();

		void addChild(std::string key, std::string value);
		Node* getChild(std::string key);
		std::string getValue();
		void setValue(std::string value);
		std::vector<std::string> getKeySet();
		bool hasKey(std::string key);
		unsigned int getChildrenSize();
	};

	std::string treeName;
	std::string fileName;
	std::unordered_map<std::string, Node*> root;

	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	void insertData(std::string path, std::string value, Node* node);
	std::string popPath(std::string path, int diff);
	std::string getKey(std::string line);
	bool doesLineHasKey(std::string line);
	bool doesLineHasValue(std::string line);
	std::string removeTab(std::string line);
	std::string ignoreWhiteSpaceBeforeKey(std::string line);
	int getDepth(std::string line);
	std::string getValueFromPath(std::string path);
	void setValue(std::string path, std::string value);
	std::string parseDataToString(Node* node, int depth);
	void parseTree(std::string data);
public:
	SimpleDataTree(std::string treeName, std::string filePath);
	~SimpleDataTree();

	void clear();
	void writeFile(std::string path);
	void parse(std::string filePath);
	int getInt(std::string path);
	float getFloat(std::string path);
	double getDouble (std::string path);
	bool getBool(std::string path);
	std::string getString(std::string path);

	void setInt(std::string path, int value);
	void setFloat(std::string path, float value);
	void setDouble(std::string path, double value);
	void setBool(std::string path, bool value);
	void setString(std::string path);
	void setString(std::string path, std::string value);
};