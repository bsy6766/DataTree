#include <iostream>

#include "DataTree.h"

using namespace std;


/**
*			This is a test code.
*			Most of the feature and function can be easily found on header with detailed comment.
*/

int main()
{
	cout << "DataTree example code" << endl;

	// Creates empty DataTree.
	DataTree* empty = DataTree::create();

	cout << "Print empty data ====" << endl;
	cout << empty->toString() << endl;


	// Reads sample file
	DataTree* data = DataTree::create("Test.txt");

	if (data == nullptr)
	{
		cout << "Failed to read Test.txt" << endl;
	}
	else
	{
		cout << "Print test data ====" << endl;
		cout << data->toString() << endl;

		// Query data
		int month = data->getInt("month");
		cout << "Month as int = " << month << endl;

		float pif = data->getFloat("pi");
		cout << "pi as float = " << pif << endl;

		double pid = data->getDouble("pi");
		cout << "Also can get as double = " << pid << endl;

		// To query nested key, use '.' between key
		bool red = data->getBool("bool.red");
		cout << "Red = " << red << endl;

		// Set "Save.key.here" key with "To test!" as a key
		data->setString("Save.key.here", "To test!");
		std::string check = data->getString("Save.key.here");
		cout << "Checking saved data... " << check << endl;

		// Save data
		data->save("Test-new.txt");

		// Delete data
		delete data;
	}
	

	delete empty;

	system("pause");
	return 0;
}