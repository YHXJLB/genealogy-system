#ifndef STRUCTURE2_H
#define STRUCTURE2_H

#include "common.h"

#define HASH_TABLE_SIZE 1000

/* 邻接表节点 */
typedef struct AdjNode {
    int member_id;           /* 相连的成员ID */
    struct AdjNode *next;
} AdjNode;

/* 图的顶点 */
typedef struct {
    FamilyMember *member;
    AdjNode *parent;         /* 指向父亲 */
    AdjNode *children;       /* 指向所有子女 */
} GraphVertex;

/* 哈希表项 */
typedef struct {
    GraphVertex *vertex;
} HashTableEntry;

/* 邻接表图 */
typedef struct {
    HashTableEntry *hash_table;
    int vertex_count;
    int next_id;
} AdjacencyListGraph;

/* 创建邻接表图 */
AdjacencyListGraph* create_adjacency_graph();

/* 在图中添加成员 */
int graph_add_member(AdjacencyListGraph *graph, FamilyMember *member);

/* 在图中查询成员（通过姓名、字号或谥号） */
GraphVertex* graph_search_member(AdjacencyListGraph *graph, const char *keyword);

/* 添加父子关系 */
void graph_add_relationship(AdjacencyListGraph *graph, int parent_id, int child_id);

/* 查询指定成员的所有子孙（至少5代） */
void graph_search_descendants(AdjacencyListGraph *graph, GraphVertex *node, int generation);

/* 删除成员（保留子孙） */
void graph_delete_member(AdjacencyListGraph *graph, int member_id);

/* 哈希函数 */
int hash_function(int member_id);

/* 销毁图 */
void destroy_adjacency_graph(AdjacencyListGraph *graph);

/* 初始化家谱 */
AdjacencyListGraph* initialize_family_graph();

#endif
