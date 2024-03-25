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
            .filter       = "*.txt"
    };
    const char* fileName = sfd_open_dialog(&opt);
    if(!fileName)
        return 0;
    FILE *fd = fopen(fileName,"r");
    return fd;
}

Graph* OpenAdjacencyGraph(GraphConfig *gc){
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
    ShuffleNodes(&loadedGraph, gc->bounds);
    return &loadedGraph;
}