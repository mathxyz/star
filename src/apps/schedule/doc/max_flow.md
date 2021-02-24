# Max Flow Min Cut
This program provides the implementation of finding a max s-t flow of a given flow network

Input Format.
The first line of each case containes two numbers: Vnum and Enum, where Vnum and Enum are the number of vertices and edges of a graph.
The second line indicates the source and sink nodes of this flow network. The third number in this line means the n-th iteration which will be saved to files.
The following Enum lines describe the edges of this graph.
Each line has 3 numbers v1, v2 and cap which tell that there is an directed edge from v1 to v2 with capacity cap.

Sample Input.
4 5
1 2 3
1 3 3
1 4 2
3 2 2
3 4 5
4 2 3

This sample input describes a flow networks consists of 4 vertices and 5 directed edges.
