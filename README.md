# Graph drawer
This is an app, that allows you to create images of graphs.\
![FlitaHW2_oajB1992qP](https://github.com/QNIKond/FlitaHW2/assets/57915280/990ad7d1-42bc-4478-98cb-e2519ea6a9cd)
Entirely based on the article:\
[Hu, Yifan. (2005). Efficient and High Quality Force-Directed Graph Drawing](http://yifanhu.net/PUB/graph_draw_small.pdf)

## How it works:
When you load graph from file, each node is placed in a random position. 
Then when the simulation starts, each node is pushed away from the other nodes and is attracted to adjacent nodes.
To speed up the process, you can create a simpler version of the graph 
with fewer vertices and edges. The process of creating this graph is called graph coarsening. 
This application provides two ways to do this. You can approximate a graph by collapsing
 the maximum independent set of edges, or by extracting the maximum set of independent vertices and connecting it
 according to a finer graph. The first method gives a more accurate representation and is preferred. 
You can apply this method several times until you get a graph with roughly two vertices, 
and then refine it back to its original state, giving each graph a little time to settle into its optimal position.

## Controls:
### Mouse/keyboard
_SPACE+LClick_ - Move canvas\
_SPACE+Scroll_ - Resize canvas\
_LClick_ - Add vertice\
_RClick_ - Delete vertice\
_LClick_ on vertice and drag to other vertice to connect them\
_Shift+LClick_ and drag - Move vertice

### Toolbar:
_RUN_ - start applying forces on vertices\
_STOP_ - stop applying forces\
_Shuffle_ - assign random position to each vertice\
_Coarse EC_ - coarse graph by edge collapsing\
_Coarce MIVS_ - coarse graph by extracting maximal independant vertices set\
_Refine_ - restore coarsened graph to the original and asign new vertices position of ancestors\
_Back_ - revert to original without applying changes\
_File->open ..._ - open matrix\
_File->save png_ - create and save image of canvas\
Hold _T_ to see quad tree

If your graph doesn't fit on the canvas, adjust the "Edge resize constant" setting.\
To increase speed, adjust the "Low heat" setting.

## File formats:
_adj mtx_ - adjacency matrix (integers arranged as matrix splitted with space)\
_edges list_ - list of edges (two integers in each line, smaller number goes first)\
_mtx_ - matrixMarket format

Site with matrices: \
[MatrixMarket](https://math.nist.gov/MatrixMarket/data/)\
[Cool matrices](https://math.nist.gov/MatrixMarket/data/Harwell-Boeing/jagmesh/jagmesh.html)  (jagmesh1 - donut)
