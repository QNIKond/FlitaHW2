#include "FileManager.h"
#include "../../sfd.h"
#include "stdio.h"
#include "raylib.h"
#include "malloc.h"

FILE* GetFile(const char* mode){
    sfd_Options opt = {
            .title        = "Open graph",
            .filter       = "*.txt|*.mtx"
    };
    const char* fileName = sfd_open_dialog(&opt);
    if(!fileName)
        return 0;
    SetWindowTitle(fileName);
    FILE *fd = fopen(fileName,"r");
    return fd;
}

Graph *OpenMtx(Graph *graph, GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(graph);
    char buffer[1000];
    int count = 0;
    const char **row;
    do {
        fgets(buffer, 1000, fd);
        row = TextSplit(buffer, ' ', &count);
    }while(count != 3);
    int nodeCount = TextToInteger(row[0]);
    int edgeCount = TextToInteger(row[2]);

    for(int i = 0; i < nodeCount; ++i)
        CreateNode(graph, i);
    for(int i = 0; i < edgeCount; ++i){
        fgets(buffer,1000,fd);
        row = TextSplit(buffer,' ',0);
        int lval = TextToInteger(row[0])-1;
        int rval = TextToInteger(row[1])-1;
        CreateNodeConnection(graph, lval, rval);
    }
    fclose(fd);
    gc->verticesCount = nodeCount;
    gc->edgesCount = edgeCount;
    ShuffleNodes(graph, gc->bounds);
    return graph;
}

Graph *OpenEdgesList(Graph *graph, GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(graph);
    char buffer[1000];

    while(fgets(buffer,1000,fd)){
        const char** row = TextSplit(buffer,' ',0);
        int lval = TextToInteger(row[0]);
        int rval = TextToInteger(row[1]);
        while(rval >= graph->nodes.filled)
            CreateNode(graph, graph->nodes.filled);
        CreateNodeConnection(graph, lval, rval);
    }
    fclose(fd);
    gc->verticesCount = graph->nodes.filled;
    ShuffleNodes(graph, gc->bounds);
    return graph;
}

Graph* OpenAdjacencyMatrix(Graph *graph, GraphConfig *gc){
    FILE *fd = GetFile("r");
    if(!fd)
        return 0;
    ResetGraph(graph);
    char buffer[1000];
    fgets(buffer,1000,fd);
    int row = 0;
    int column;
    TextSplit(buffer,' ',&column);
    column -= 1;
    for(int i = 0; i < column; ++i)
        CreateNode(graph, i);
    do{
        const char ** adj = TextSplit(buffer,' ',0);
        for(int i = 0; i < row; ++i) {
            if(TextToInteger(adj[i]))
                CreateNodeConnection(graph, row, i);
        }
        ++row;
    } while(fgets(buffer,1000,fd));
    fclose(fd);
    gc->verticesCount = graph->nodes.filled;
    ShuffleNodes(graph, gc->bounds);
    return graph;
}