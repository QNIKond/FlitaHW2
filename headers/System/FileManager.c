#include "FileManager.h"
#include "../../sfd.h"
#include "stdio.h"
#include "raylib.h"
#include "malloc.h"
#include "../GUI/Layout.h"

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
        int t;
        for(int i = 0; i < row; ++i) {
            t = TextToInteger(adj[i]);
            if(t) {
                AddWeight(graph, row, i, t);
            }
        }
        t = TextToInteger(adj[row]);
        if(t)
            GETNODES(graph)[row].weight += 1;
        ++row;
    } while(fgets(buffer,1000,fd));
    fclose(fd);
    gc->verticesCount = graph->nodes.filled;
    ShuffleNodes(graph, gc->bounds);
    return graph;
}

void DrawToImage(Graph *graph, GraphConfig *gc){
    Image img = GenImageColor(gc->bounds.width,gc->bounds.height,RAYWHITE);

    edgeID curEdge;
    for (int i = 0; i < graph->nodes.filled; ++i) {
        if(!GETNODES(graph)[i].state)
            continue;
        if(gc->showDots) {
            int weight = (GETNODES(graph)[i].weight-1) * 40;
            if(weight > 255)
                weight = 255;
            ImageDrawCircleV(&img,GETNODES(graph)[i].pos, DOTRADIUS, (Color){weight,0,0,255});
        }
        curEdge = GETNODES(graph)[i].edges;
        while(curEdge != EOEDGELIST)
        {

            if((GETEDGES(graph)[curEdge].node>i)&&(GETEDGES(graph)[curEdge].state&1)&&
               (GETNODES(graph)[GETEDGES(graph)[curEdge].node].state&1)) {
                int weight = (GETEDGES(graph)[curEdge].weight-1) * 40;
                if(weight > 255)
                    weight = 255;
                ImageDrawLineV(&img,GETNODES(graph)[i].pos,
                          GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos,
                          (Color){weight,0,0,255});
                /*if((!gc->showEdgeWeights) || (GETEDGES(graph)[curEdge].weight == 1))
                    DrawLineV(GetAbs(GETNODES(graph)[i].pos),
                              GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos), BLACK);
                else
                    DrawLineEx(GetAbs(GETNODES(graph)[i].pos),
                            GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos),
                            GETEDGES(graph)[curEdge].weight, BLACK);*/
            }
            curEdge = GETEDGES(graph)[curEdge].nextEdge;
        }
    }
    sfd_Options opt = {
            .title        = "Save graph image",
            .filter       = "*.png"
    };
    const char* fileName = sfd_save_dialog(&opt);
    const char* t = TextJoin((const char*[]){fileName,".png"},2,"");
    if(!fileName)
        return;
    ExportImage(img,t);
}