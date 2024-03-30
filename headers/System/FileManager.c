#include "FileManager.h"
#include "../../sfd.h"
#include "stdio.h"
#include "raylib.h"
#include "malloc.h"

Graph loadedGraph;

Graph* GetDefaultGraph(){
    InitializeGraph(&loadedGraph);
    return &loadedGraph;
}

FILE* GetFile(const char* mode){
    sfd_Options opt = {
            .title        = "Open graph",
            .filter       = "*.txt|*.mtx"
    };
    const char* fileName = sfd_open_dialog(&opt);
    if(!fileName)
        return 0;
    FILE *fd = fopen(fileName,"r");
    return fd;
}

Graph *OpenMtx(GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(&loadedGraph);
    char buffer[1000];
    fgets(buffer,1000,fd);
    fgets(buffer,1000,fd);
    const char** row = TextSplit(buffer,' ',0);
    int nodeCount = TextToInteger(row[0]);
    int edgeCount = TextToInteger(row[2]);

    for(int i = 0; i < nodeCount; ++i)
        CreateNode(&loadedGraph,i);
    for(int i = 0; i < edgeCount; ++i){
        fgets(buffer,1000,fd);
        row = TextSplit(buffer,' ',0);
        int lval = TextToInteger(row[0])-1;
        int rval = TextToInteger(row[1])-1;
        ConnectNodes(&loadedGraph,lval,rval);
    }
    fclose(fd);
    gc->verticesCount = nodeCount;
    gc->edgesCount = edgeCount;
    ShuffleNodes(&loadedGraph, gc->bounds);
    return &loadedGraph;
}

Graph *OpenEdgesList(GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(&loadedGraph);
    char buffer[1000];

    while(fgets(buffer,1000,fd)){
        const char** row = TextSplit(buffer,' ',0);
        int lval = TextToInteger(row[0]);
        int rval = TextToInteger(row[1]);
        while(rval>=loadedGraph.nodes.filled)
            CreateNode(&loadedGraph,loadedGraph.nodes.filled);
        ConnectNodes(&loadedGraph,lval,rval);
    }
    fclose(fd);
    gc->verticesCount = loadedGraph.nodes.filled;
    ShuffleNodes(&loadedGraph, gc->bounds);
    return &loadedGraph;
}

Graph* OpenAdjacencyMatrix(GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(&loadedGraph);
    char buffer[1000];
    fgets(buffer,1000,fd);
    int row = 0;
    int column;
    TextSplit(buffer,' ',&column);
    column -= 1;
    for(int i = 0; i < column; ++i)
        CreateNode(&loadedGraph, i);
    do{
        const char ** adj = TextSplit(buffer,' ',0);
        for(int i = 0; i < row; ++i) {
            if(TextToInteger(adj[i]))
                ConnectNodes(&loadedGraph, row, i);
        }
        ++row;
    } while(fgets(buffer,1000,fd));
    fclose(fd);
    gc->verticesCount = loadedGraph.nodes.filled;
    ShuffleNodes(&loadedGraph, gc->bounds);
    return &loadedGraph;
}