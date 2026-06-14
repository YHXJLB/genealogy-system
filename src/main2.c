#include "structure2.h"

void print_menu2() {
    printf("\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("║       家谱系统 - 邻接表存储结构         ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║ (1) 创建家谱                             ║\n");
    printf("║ (2) 查询家族成员信息                     ║\n");
    printf("║ (3) 查询指定成员的所有子孙               ║\n");
    printf("║ (4) 插入新成员                           ║\n");
    printf("║ (5) 删除成员（保留子孙）                 ║\n");
    printf("║ (6) 显示家谱统计信息                     ║\n");
    printf("║ (0) 退出程序                             ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n请选择操作 (0-6): ");
}

int main() {
    AdjacencyListGraph *graph = initialize_family_graph();
    if (graph == NULL) {
        fprintf(stderr, "初始化家谱失败\n");
        return 1;
    }

    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║        欢迎使用家谱管理系统             ║\n");
    printf("║  （邻接表存储结构 - 琅琊王氏示例）    ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    int choice;
    char keyword[MAX_NAME_LEN];
    char parent_name[MAX_NAME_LEN];

    while (1) {
        print_menu2();
        scanf("%d", &choice);
        getchar();  /* 消费换行符 */

        switch (choice) {
            case 1: {
                printf("\n✓ 家谱已创建，初始数据已加载\n");
                printf("└─ 创始人：王祥（字休征，谥孝王）\n");
                printf("   ├─ 王苏（字庶昱）\n");
                printf("   │   └─ 王恭（字茂弘）\n");
                printf("   │       └─ 王赵\n");
                printf("   │           └─ 王藏\n");
                printf("\n分支：王恭（东晋名士）、王羲之（"书圣"）\n");
                printf("       ├─ 王献之（"小圣贤庄"主人公）\n");
                printf("       └─ 王徽之（"王子玙乘兴夜访戴安道"故事主人公）\n");
                break;
            }

            case 2: {
                printf("\n请输入查询关键字（姓名、字号或谥号）: ");
                fgets(keyword, MAX_NAME_LEN, stdin);
                keyword[strcspn(keyword, "\n")] = 0;  /* 移除换行符 */

                if (strlen(keyword) == 0) {
                    printf("关键字不能为空\n");
                    break;
                }

                GraphVertex *found = graph_search_member(graph, keyword);
                if (found != NULL) {
                    printf("\n✓ 查询成功！\n\n");
                    print_member(found->member);

                    if (found->parent != NULL) {
                        GraphVertex *parent = get_vertex(graph, found->parent->member_id);
                        if (parent == NULL) {
                            /* 查找父亲 */
                            for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                                if (graph->hash_table[i].vertex != NULL &&
                                    graph->hash_table[i].vertex->member->member_id == found->parent->member_id) {
                                    printf("\n父亲信息：\n");
                                    printf("  %s（字%s）\n",
                                           graph->hash_table[i].vertex->member->name,
                                           graph->hash_table[i].vertex->member->courtesy_name);
                                    break;
                                }
                            }
                        }
                    } else {
                        printf("\n该成员为族谱的创始人\n");
                    }

                    printf("\n直接子女：\n");
                    if (found->children == NULL) {
                        printf("  （无记录）\n");
                    } else {
                        AdjNode *child = found->children;
                        while (child != NULL) {
                            GraphVertex *child_vertex = NULL;
                            for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                                if (graph->hash_table[i].vertex != NULL &&
                                    graph->hash_table[i].vertex->member->member_id == child->member_id) {
                                    child_vertex = graph->hash_table[i].vertex;
                                    break;
                                }
                            }
                            if (child_vertex != NULL) {
                                printf("  - %s\n", child_vertex->member->name);
                            }
                            child = child->next;
                        }
                    }
                } else {
                    printf("✗ 未找到相关成员\n");
                }
                break;
            }

            case 3: {
                printf("\n请输入要查询子孙的成员名字: ");
                fgets(keyword, MAX_NAME_LEN, stdin);
                keyword[strcspn(keyword, "\n")] = 0;

                if (strlen(keyword) == 0) {
                    printf("关键字不能为空\n");
                    break;
                }

                GraphVertex *found = graph_search_member(graph, keyword);
                if (found != NULL) {
                    graph_search_descendants(graph, found, 5);
                } else {
                    printf("✗ 未找到该成员\n");
                }
                break;
            }

            case 4: {
                printf("\n请输入父亲的名字: ");
                fgets(parent_name, MAX_NAME_LEN, stdin);
                parent_name[strcspn(parent_name, "\n")] = 0;

                GraphVertex *parent = graph_search_member(graph, parent_name);
                if (parent == NULL) {
                    printf("✗ 未找到该父亲\n");
                    break;
                }

                char new_name[MAX_NAME_LEN];
                char new_courtesy[MAX_COURTESY_NAME_LEN];
                char new_posthumous[MAX_POSTHUMOUS_NAME_LEN];

                printf("请输入新成员的姓名: ");
                fgets(new_name, MAX_NAME_LEN, stdin);
                new_name[strcspn(new_name, "\n")] = 0;

                printf("请输入新成员的字号: ");
                fgets(new_courtesy, MAX_COURTESY_NAME_LEN, stdin);
                new_courtesy[strcspn(new_courtesy, "\n")] = 0;

                printf("请输入新成员的谥号: ");
                fgets(new_posthumous, MAX_POSTHUMOUS_NAME_LEN, stdin);
                new_posthumous[strcspn(new_posthumous, "\n")] = 0;

                FamilyMember *new_member = create_member(
                    new_name, new_courtesy, new_posthumous,
                    parent->member->dynasty, "官员", "新增成员",
                    "待补充", "新增到家谱", 0
                );

                if (new_member != NULL) {
                    int new_id = graph_add_member(graph, new_member);
                    graph_add_relationship(graph, parent->member->member_id, new_id);
                    printf("✓ 新成员已添加到 %s 下\n", parent->member->name);
                }
                break;
            }

            case 5: {
                printf("\n请输入要删除的成员名字: ");
                fgets(keyword, MAX_NAME_LEN, stdin);
                keyword[strcspn(keyword, "\n")] = 0;

                if (strlen(keyword) == 0) {
                    printf("关键字不能为空\n");
                    break;
                }

                GraphVertex *to_delete = graph_search_member(graph, keyword);
                if (to_delete != NULL) {
                    printf("\n确认删除 %s 吗？其子孙将被保留。(y/n): ", keyword);
                    char confirm;
                    scanf("%c", &confirm);
                    getchar();

                    if (confirm == 'y' || confirm == 'Y') {
                        graph_delete_member(graph, to_delete->member->member_id);
                        printf("✓ 成员已删除，其子孙保留\n");
                    } else {
                        printf("取消删除\n");
                    }
                } else {
                    printf("✗ 未找到该成员\n");
                }
                break;
            }

            case 6: {
                printf("\n╔══════════════════════════════════════════╗\n");
                printf("║        家谱统计信息                      ║\n");
                printf("╠══════════════════════════════════════════╣\n");
                printf("║ 创始人：王祥（字休征）                  ║\n");
                printf("║ 总记录人数：%d                          ║\n", graph->vertex_count);
                printf("║ 主要分支：琅琊王氏、书法世家             ║\n");
                printf("║ 特色功能：                               ║\n");
                printf("║   - 灵活的图遍历（邻接表）               ║\n");
                printf("║   - 支持成员删除操作                    ║\n");
                printf("║   - 完整子孙查询功能                    ║\n");
                printf("╚══════════════════════════════════════════╝\n");
                break;
            }

            case 0: {
                printf("\n感谢使用家谱管理系统，再见！\n\n");
                destroy_adjacency_graph(graph);
                return 0;
            }

            default:
                printf("✗ 无效的选择，请重试\n");
        }
    }

    return 0;
}
