using System;
using System.Collections.Generic;

public class SimpleDataTree
{
    private class Node
    {
        // private string key;
        private string value;
        private Dictionary<string, Node> children;
        public Node(string value)
        {
            //this.key = key;
            this.value = value;
            children = new Dictionary<string, Node>();
        }

        public void addChild(string key, string value)
        {
            children.Add(key, new Node(value));
        }

        public Node getChild(string key)
        {
            if (children.ContainsKey(key))
            {
                return children[key];
            }
            else
            {
                return null;
            }
        }

        public string getValue()
        {
            return value;
        }
    }

    private string treeName;
    private Dictionary<string, Node> root = null;

    public SimpleDataTree(string treeName, string filePath)
    {
        this.treeName = treeName;
        parse(filePath);
    }

    public bool parse(string filePath)
    {
        if (root != null)
            root.Clear();

        root = new Dictionary<string, Node>();
        root.Add(treeName, new Node(null));

        string data = System.IO.File.ReadAllText(filePath);

        if (data.Length <= 0)
            return false;
        parseTree(data);
        return true;
    }

    private void parseTree(string data)
    {
        string[] splitRet = data.Split("\r".ToCharArray());

        string curPath = "";
        int curDepth = 0;

        for (int i = 0; i < splitRet.Length; i++)
        {
            splitRet[i] = ignoreWhiteSpaceBeforeKey(splitRet[i]);
            if (splitRet[i] == null)
                continue;
            int depth = getDepth(splitRet[i]);
            splitRet[i] = removeTab(splitRet[i]);

            bool hasKey = doesLineHasKey(splitRet[i]);
            bool hasValue = doesLineHasValue(splitRet[i]);

            if (curDepth == depth)
            {
                if (curPath == "" && depth == 0)
                {
                    //first key.
                    curPath = getKey(splitRet[i]);
                }
                else
                {
                    //childrens
                    //replace last path
                    curPath = popPath(curPath, 1);
                    //and add key
                    curPath += ("." + getKey(splitRet[i]));

                }
            }
            else if (curDepth < depth)
            {
                //depth inc. it's child
                curDepth = depth;
                curPath += ("." + getKey(splitRet[i]));
            }
            else if (curDepth > depth)
            {
                //depth dec
                int diff;
                if (depth != 0)
                {
                    diff = curDepth - depth + 1;
                    curPath = popPath(curPath, diff);
                    curDepth = depth;
                    //and add key
                    curPath += ("." + getKey(splitRet[i]));
                }
                else
                {
                    //go back to root
                    curDepth = depth;
                    curPath = getKey(splitRet[i]);
                }
            }

            if (hasValue)
                insertData(curPath, getValue(splitRet[i]), root[treeName]);
            else
                insertData(curPath, null, root[treeName]);
        }

        //what..
    }

    private void insertData(string path, string value, Node node)
    {
        int index = path.IndexOf(".");
        if (index == -1)
        {
            node.addChild(path, value);
        }
        else
        {
            string restPath = path.Substring(index + 1);
            string key = path.Substring(0, index);
            insertData(restPath, value, node.getChild(key));
        }
    }

    private string popPath(string path, int diff)
    {
        for (int i = 0; i < diff; i++)
        {
            int index = path.LastIndexOf(".");
            path = path.Substring(0, index);
        }
        return path;
    }

    private string getKey(string line)
    {
        int colonIndex = line.IndexOf(":");
        if (colonIndex == -1)
        {
            return null;
        }
        return line.Substring(0, colonIndex);
    }

    private string getValue(string line)
    {
        int colonIndex = line.IndexOf(":");
        if (colonIndex == -1)
        {
            return null;
        }
        return line.Substring(colonIndex + 2);
    }

    private bool doesLineHasKey(string line)
    {
        int colonIndex = line.IndexOf(":");
        if (colonIndex == -1)
        {
            return false;
        }
        return true;
    }

    private bool doesLineHasValue(string line)
    {
        int colonIndex = line.IndexOf(":");
        if (colonIndex == line.Length)
        {
            return false;
        }
        else
        {
            string key = line.Substring(colonIndex + 1);
            for (int i = 0; i < key.Length; i++)
            {
                string chStr = key.Substring(i, 1);
                if (chStr == " " || chStr == "\t")
                {
                    continue;
                }
                else
                {
                    return true;
                }
            }
            return false;
        }
    }

    private string removeTab(string line)
    {
        return line.Replace("\t", string.Empty);
    }

    private string ignoreWhiteSpaceBeforeKey(string line)
    {
        //line will be <whitespaaaaaace><possible tab>key:
        line = line.Replace("\n", "");
        int keyIndex = line.IndexOf(":");
        if (keyIndex == -1)
        {
            return null;
        }
        string front = line.Substring(0, keyIndex);
        string rest = line.Substring(keyIndex);
        front = front.Replace(" ", string.Empty);
        return front + rest;
    }

    private int getDepth(string line)
    {
        int depth = 0;
        for (int i = 0; i < line.Length; i++)
        {
            string chStr = line.Substring(i, 1);
            if (chStr == "\t")
            {
                depth++;
            }
            else if (chStr == ":")
            {
                break;
            }

        }
        return depth;
    }

    public string getValueFromPath(string path)
    {
        if (path == null)
            return null;

        string[] pathSplit = path.Split('.');

        if (pathSplit.Length == 0)
            return null;

        Node target = root[treeName];
        for (int i = 0; i < pathSplit.Length; i++)
        {
            target = target.getChild(pathSplit[i]);
            if (target != null)
            {
                continue;
            }
            else
            {
                return null;
            }
        }

        return target.getValue();
    }

    public string getString(string path)
    {
        return getString(path, null);
    }

    public string getString(string path, string value)
    {
        string treeValue = getValueFromPath(path);
        if (treeValue == null)
            if (value == null)
                return null;
            else
                return value;
        else
            return treeValue;
    }

    public int getInt(string path)
    {
        return getInt(path, 0);
    }

    public int getInt(string path, int value)
    {
        string treeValue = getValueFromPath(path);
        if (treeValue == null)
        {
            if (value == 0)
                return 0;
            else
                return value;
        }
        else
        {
            int number;
            bool result = Int32.TryParse(treeValue, out number);
            if (result)
                return number;
            else
                return 0;
        }
    }

    public float getFloat(string path)
    {
        return getFloat(path, 0);
    }

    public float getFloat(string path, float value)
    {
        string treeValue = getValueFromPath(path);
        if (treeValue == null)
        {
            if (value == 0)
                return 0;
            else
                return value;
        }
        else
        {
            float number;
            bool result = float.TryParse(treeValue, out number);
            if (result)
                return number;
            else
                return 0;
        }
    }

    public double getDouble(string path)
    {
        return getDouble(path, 0);
    }

    public double getDouble(string path, double value)
    {
        string treeValue = getValueFromPath(path);
        if (treeValue == null)
        {
            if (value == 0)
                return 0;
            else
                return value;
        }
        else
        {
            double number;
            bool result = double.TryParse(treeValue, out number);
            if (result)
                return number;
            else
                return 0;
        }
    }

    public bool getBoolean(string path)
    {
        return getBoolean(path, false);
    }

    public bool getBoolean(string path, bool value)
    {
        string treeValue = getValueFromPath(path);
        if (treeValue == null)
            if (value == false)
                return false;
            else
                return value;
        else
        {
            bool flag;
            bool result = Boolean.TryParse(treeValue, out flag);
            if (result)
                return flag;
            else
                return false;
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        SimpleDataTree sdTree = new SimpleDataTree("sample", "sampleData.sdtree");
        Console.WriteLine();
        Console.WriteLine("Tile.name = " + sdTree.getString("Tile.name"));
        Console.WriteLine("Tile.name.assetName = " + sdTree.getString("Tile.name.assetName"));
        Console.WriteLine("Tile.hp = " + sdTree.getInt("Tile.hp"));
        Console.WriteLine("Tile.height = " + sdTree.getFloat("Tile.height"));
        Console.WriteLine("Tile.height = " + sdTree.getDouble("Tile.height"));
        Console.WriteLine("Tile.visible = " + sdTree.getBoolean("Tile.name"));
        Console.WriteLine("Tile.engine.name = " + sdTree.getString("Tile.engine.name"));
        Console.WriteLine("Cover.name.first = " + sdTree.getString("Cover.name.first"));
        Console.WriteLine("version = " + sdTree.getString("version"));
        Console.WriteLine();
        Console.WriteLine("DONE!");
    }
}