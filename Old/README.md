# SimpleDataTree(aka sdTree)
This is a YAML inspired, simple data type and parser written in C# to use with Unity .<br>
'sdTree' currently supports Int32, float, double, boolean, and string data.<br>
<br>
Here's sample data file example.<br>
<br>
```
Tile:     
	name: "String tile name"
		assetName: "asset string name"
	hp: 12
	height: 1.3
	visible: true
	engine:
		name: "Unity"
Cover:
	name:
		first: "Cover's name"
version: 1
```
<br>
And here's example how to create a data and read values.<br>
<br>
```
string treeName = "sample";
string filePath = "sampleData.sdtree"
SimpleDataTree sdtree = new SimpleDataTree(treeName, filePath);
Console.WirteLine(sdtree.getString("Tile.name"));
Console.WirteLine(sdtree.getInt("version").ToString());
```
<br>
then the output will be...<br>
<br>
```
String tile name
1
```

<br>
###Restriction
This parser will kind of assume data files is correct and only recognize tab for indentation<br>
