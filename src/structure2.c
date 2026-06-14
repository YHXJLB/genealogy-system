#include "structure2.h"

/* 创建邻接表图 */
AdjacencyListGraph* create_adjacency_graph() {
    AdjacencyListGraph *graph = (AdjacencyListGraph*)malloc(sizeof(AdjacencyListGraph));
    if (graph == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }

    graph->hash_table = (HashTableEntry*)calloc(HASH_TABLE_SIZE, sizeof(HashTableEntry));
    if (graph->hash_table == NULL) {
        fprintf(stderr, "内存分配失败\n");
        free(graph);
        return NULL;
    }

    graph->vertex_count = 0;
    graph->next_id = 1;
    return graph;
}

/* 哈希函数 */
int hash_function(int member_id) {
    return member_id % HASH_TABLE_SIZE;
}

/* 创建顶点 */
static GraphVertex* create_vertex(FamilyMember *member) {
    GraphVertex *vertex = (GraphVertex*)malloc(sizeof(GraphVertex));
    if (vertex == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }
    vertex->member = member;
    vertex->parent = NULL;
    vertex->children = NULL;
    return vertex;
}

/* 在图中添加成员 */
int graph_add_member(AdjacencyListGraph *graph, FamilyMember *member) {
    if (graph == NULL || member == NULL) return -1;

    int id = graph->next_id++;
    member->member_id = id;

    int hash_index = hash_function(id);

    /* 处理哈希冲突 */
    while (graph->hash_table[hash_index].vertex != NULL) {
        hash_index = (hash_index + 1) % HASH_TABLE_SIZE;
    }

    GraphVertex *vertex = create_vertex(member);
    if (vertex == NULL) return -1;

    graph->hash_table[hash_index].vertex = vertex;
    graph->vertex_count++;

    return id;
}

/* 获取顶点 */
static GraphVertex* get_vertex(AdjacencyListGraph *graph, int member_id) {
    int hash_index = hash_function(member_id);
    int start_index = hash_index;

    while (graph->hash_table[hash_index].vertex != NULL) {
        if (graph->hash_table[hash_index].vertex->member->member_id == member_id) {
            return graph->hash_table[hash_index].vertex;
        }
        hash_index = (hash_index + 1) % HASH_TABLE_SIZE;
        if (hash_index == start_index) break;  /* 遍历完整个表 */
    }

    return NULL;
}

/* 递归搜索成员 */
static GraphVertex* _graph_search_recursive(AdjacencyListGraph *graph, int index, const char *keyword) {
    if (index >= HASH_TABLE_SIZE) return NULL;

    if (graph->hash_table[index].vertex != NULL) {
        FamilyMember *member = graph->hash_table[index].vertex->member;
        if (strcmp(member->name, keyword) == 0 ||
            strcmp(member->courtesy_name, keyword) == 0 ||
            strcmp(member->posthumous_name, keyword) == 0) {
            return graph->hash_table[index].vertex;
        }
    }

    return _graph_search_recursive(graph, index + 1, keyword);
}

/* 在图中查询成员 */
GraphVertex* graph_search_member(AdjacencyListGraph *graph, const char *keyword) {
    if (graph == NULL || keyword == NULL) return NULL;
    return _graph_search_recursive(graph, 0, keyword);
}

/* 添加父子关系 */
void graph_add_relationship(AdjacencyListGraph *graph, int parent_id, int child_id) {
    if (graph == NULL) return;

    GraphVertex *parent = get_vertex(graph, parent_id);
    GraphVertex *child = get_vertex(graph, child_id);

    if (parent == NULL || child == NULL) return;

    /* 添加子女关系 */
    AdjNode *new_child = (AdjNode*)malloc(sizeof(AdjNode));
    if (new_child == NULL) return;
    new_child->member_id = child_id;
    new_child->next = parent->children;
    parent->children = new_child;

    /* 添加父亲关系 */
    AdjNode *new_parent = (AdjNode*)malloc(sizeof(AdjNode));
    if (new_parent == NULL) {
        free(new_child);
        return;
    }
    new_parent->member_id = parent_id;
    new_parent->next = NULL;
    child->parent = new_parent;
}

/* 查询指定成员的所有子孙（至少5代） */
void graph_search_descendants(AdjacencyListGraph *graph, GraphVertex *node, int generation) {
    if (node == NULL || graph == NULL) {
        printf("未找到成员或图为空\n");
        return;
    }

    printf("\n========== 子孙查询结果 =========="\n);
    printf("从 %s 开始查询，最多显示 %d 代子孙\n\n", node->member->name, generation);

    /* 使用队列进行广度优先搜索 */
    typedef struct {
        GraphVertex *vertex;
        int gen;
    } QueueNode;

    QueueNode *queue = (QueueNode*)malloc(sizeof(QueueNode) * 10000);
    if (queue == NULL) return;

    int front = 0, rear = 0;
    queue[rear].vertex = node;
    queue[rear].gen = 0;
    rear++;

    while (front < rear && front < 10000) {
        GraphVertex *current = queue[front].vertex;
        int current_gen = queue[front].gen;
        front++;

        if (current_gen > 0 && current_gen <= generation) {
            printf("第 %d 代子孙：\n", current_gen);
            print_member(current->member);
            printf("\n");
        }

        if (current_gen < generation) {
            AdjNode *child = current->children;
            while (child != NULL && rear < 10000) {
                GraphVertex *child_vertex = get_vertex(graph, child->member_id);
                if (child_vertex != NULL) {
                    queue[rear].vertex = child_vertex;
                    queue[rear].gen = current_gen + 1;
                    rear++;
                }
                child = child->next;
            }
        }
    }

    if (front == 1) {
        printf("该成员没有子孙记录\n");
    } else {
        printf("总共查询了 %d 代子孙\n", front - 2);
    }
    printf("================================\n\n");

    free(queue);
}

/* 删除成员（保留子孙） */
void graph_delete_member(AdjacencyListGraph *graph, int member_id) {
    if (graph == NULL) return;

    GraphVertex *vertex = get_vertex(graph, member_id);
    if (vertex == NULL) return;

    /* 获取该成员的父亲 */
    GraphVertex *parent = NULL;
    if (vertex->parent != NULL) {
        parent = get_vertex(graph, vertex->parent->member_id);
    }

    /* 将该成员的所有子女改为父亲的直接子女 */
    AdjNode *child = vertex->children;
    while (child != NULL) {
        GraphVertex *child_vertex = get_vertex(graph, child->member_id);
        if (child_vertex != NULL && parent != NULL) {
            /* 更新子女的父亲关系 */
            free(child_vertex->parent);
            child_vertex->parent = (AdjNode*)malloc(sizeof(AdjNode));
            if (child_vertex->parent != NULL) {
                child_vertex->parent->member_id = parent->member->member_id;
                child_vertex->parent->next = NULL;
            }
        }
        child = child->next;
    }

    /* 清理该成员的邻接列表 */
    AdjNode *child_node = vertex->children;
    while (child_node != NULL) {
        AdjNode *temp = child_node;
        child_node = child_node->next;
        free(temp);
    }

    if (vertex->parent != NULL) {
        free(vertex->parent);
    }

    free(vertex->member);
    free(vertex);

    /* 标记哈希表项为空 */
    int hash_index = hash_function(member_id);
    int start_index = hash_index;
    while (graph->hash_table[hash_index].vertex != NULL) {
        if (graph->hash_table[hash_index].vertex->member->member_id == member_id) {
            graph->hash_table[hash_index].vertex = NULL;
            graph->vertex_count--;
            return;
        }
        hash_index = (hash_index + 1) % HASH_TABLE_SIZE;
        if (hash_index == start_index) break;
    }
}

/* 销毁图 */
void destroy_adjacency_graph(AdjacencyListGraph *graph) {
    if (graph == NULL) return;

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (graph->hash_table[i].vertex != NULL) {
            GraphVertex *vertex = graph->hash_table[i].vertex;
            AdjNode *child = vertex->children;
            while (child != NULL) {
                AdjNode *temp = child;
                child = child->next;
                free(temp);
            }
            if (vertex->parent != NULL) {
                free(vertex->parent);
            }
            if (vertex->member != NULL) {
                free(vertex->member);
            }
            free(vertex);
        }
    }

    free(graph->hash_table);
    free(graph);
}

/* 初始化家谱 - 琅琊王氏 */
AdjacencyListGraph* initialize_family_graph() {
    AdjacencyListGraph *graph = create_adjacency_graph();
    if (graph == NULL) return NULL;

    /* 添加王祥 */
    FamilyMember *wang_xiang = create_member(
        "王祥",
        "休征",
        "孝王",
        "三国",
        "晋国司徒",
        "琅琊王氏的奠基人",
        "178-268",
        "琅琊王氏的创始人，\"卧冰求鲤\"的故事主人公",
        1
    );
    int wang_xiang_id = graph_add_member(graph, wang_xiang);

    /* 添加王苏 */
    FamilyMember *wang_su = create_member(
        "王苏",
        "庶昱",
        "",
        "西晋",
        "侍中",
        "王祥之子",
        "209-288",
        "王祥的儿子，继续传承家族荣耀",
        2
    );
    int wang_su_id = graph_add_member(graph, wang_su);
    graph_add_relationship(graph, wang_xiang_id, wang_su_id);

    /* 添加王恭 */
    FamilyMember *wang_yan = create_member(
        "王恭",
        "茂弘",
        "",
        "西晋",
        "大将军",
        "王苏之子",
        "256-311",
        "著名的玄学家和政治家",
        3
    );
    int wang_yan_id = graph_add_member(graph, wang_yan);
    graph_add_relationship(graph, wang_su_id, wang_yan_id);

    /* 添加王赵 */
    FamilyMember *wang_zhao = create_member(
        "王赵",
        "",
        "",
        "西晋",
        "官员",
        "王恭之子",
        "280-310",
        "王恭的儿子",
        4
    );
    int wang_zhao_id = graph_add_member(graph, wang_zhao);
    graph_add_relationship(graph, wang_yan_id, wang_zhao_id);

    /* 添加王藏 */
    FamilyMember *wang_cang = create_member(
        "王藏",
        "",
        "",
        "西晋",
        "官员",
        "王赵之子",
        "295-330",
        "王氏家族的继承者",
        5
    );
    int wang_cang_id = graph_add_member(graph, wang_cang);
    graph_add_relationship(graph, wang_zhao_id, wang_cang_id);

    /* 添加王恭（东晋名士） */
    FamilyMember *wang_gong = create_member(
        "王恭",
        "孝伯",
        "",
        "东晋",
        "豫州刺史",
        "琅琊王氏的名士",
        "350-401",
        "著名的名士，清廉节操的典范",
        6
    );
    int wang_gong_id = graph_add_member(graph, wang_gong);

    /* 添加王徽之 */
    FamilyMember *wang_huizhi = create_member(
        "王徽之",
        "子玙",
        "",
        "东晋",
        "黄门侍郎",
        "\"王子玙乘兴夜访戴安道\"故事主人公",
        "363-388",
        "名士风流，有名言\"何必要州郡，但得一山水足矣\"",
        7
    );
    int wang_huizhi_id = graph_add_member(graph, wang_huizhi);
    graph_add_relationship(graph, wang_gong_id, wang_huizhi_id);

    /* 添加王献之 */
    FamilyMember *wang_xianzhi = create_member(
        "王献之",
        "子敬",
        "右军",
        "东晋",
        "侍中",
        "书法名家，继承父业成就卓著",
        "344-386",
        "王羲之之子，书法成就卓著，与父齐名，\"小圣贤庄\"主人公",
        8
    );
    int wang_xianzhi_id = graph_add_member(graph, wang_xianzhi);
    graph_add_relationship(graph, wang_gong_id, wang_xianzhi_id);

    /* 添加王羲之 */
    FamilyMember *wang_xizhi = create_member(
        "王羲之",
        "逸少",
        "书圣",
        "东晋",
        "右军将军",
        "永和九年兰亭雅集-《兰亭集序》",
        "303-361",
        "书法艺术的集大成者，\"书圣\"称号，著《兰亭集序》",
        9
    );
    int wang_xizhi_id = graph_add_member(graph, wang_xizhi);
    graph_add_relationship(graph, wang_gong_id, wang_xizhi_id);

    /* 添加王凝之 */
    FamilyMember *wang_ningzhi = create_member(
        "王凝之",
        "幼仪",
        "",
        "东晋",
        "太常",
        "王羲之之子",
        "341-388",
        "王羲之的儿子",
        10
    );
    int wang_ningzhi_id = graph_add_member(graph, wang_ningzhi);
    graph_add_relationship(graph, wang_xizhi_id, wang_ningzhi_id);

    /* 添加王询 */
    FamilyMember *wang_xun = create_member(
        "王询",
        "伯仁",
        "",
        "东晋",
        "左卫将军",
        "王羲之之子",
        "340-385",
        "王羲之的儿子",
        11
    );
    int wang_xun_id = graph_add_member(graph, wang_xun);
    graph_add_relationship(graph, wang_xizhi_id, wang_xun_id);

    /* 添加王萱 */
    FamilyMember *wang_xuan = create_member(
        "王萱",
        "季琶",
        "",
        "东晋",
        "尚书令",
        "王献之之子",
        "350-401",
        "王献之的儿子，继承家族传统",
        12
    );
    int wang_xuan_id = graph_add_member(graph, wang_xuan);
    graph_add_relationship(graph, wang_xianzhi_id, wang_xuan_id);

    /* 添加王坦之 */
    FamilyMember *wang_tanzhi = create_member(
        "王坦之",
        "文度",
        "",
        "东晋",
        "吏部尚书",
        "王献之之子",
        "351-399",
        "王献之的儿子",
        13
    );
    int wang_tanzhi_id = graph_add_member(graph, wang_tanzhi);
    graph_add_relationship(graph, wang_xianzhi_id, wang_tanzhi_id);

    return graph;
}
