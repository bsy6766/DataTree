# DataTree
DataTree is a text file format uses key-value pair and hierarchy. It is very similar to YAML but more simple than YAML.

## Supported datatypes
Since DataTree is text file, all data will be stored as string in the code but you can always query as other datatypes such as int, float, etc. 
Current version supports int, float, double, bool and string.

## Example/Sample Code
Look on the example code in Example folder.

## Details
### Key-Value
DataTree stores data with key and value as pair. Each key must be unique and duplicated keys will be overwritten by last key that is parsed. Key must be a single word and can not have any whitespace, tab or newline. After the key, rest of the line will be a value followed from whitespace after key.

### Hierarchy
DataTree supports hierarchy, which means you can add multiple key-value to existing key-value. Each whitespace and tab are considered as one depth.

### Query
To query data, you need to know the exact key. Wrong or empty key will return default values. To query a multiple depth, use '.' to append keys.

### Comment
To write a comment in file, use '#' as a comment symbol. All line after '#' character will be ignored. DataTree does not keep track of comment so comments will be removed if you save the file.

## Todo/Idea
- I used std::unordered_map to store data. Because of this, saved files will possibly have different order compared to original file. This can be easily fixed by using std::vector to keep order.
- List can be handy to store multiple data in one key.
