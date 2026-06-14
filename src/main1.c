#include "structure1.h"
#include <ctype.h>

void print_menu1() {
    printf("\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("║        家谱系统 - 树型存储结构          ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║ (1) 创建家谱                             ║\n");
    printf("║ (2) 查询家族成员信息                     ║\n");
    printf("║ (3) 查询指定成员的所有祖先               ║\n");
    printf("║ (4) 插入新成员                           ║\n");
    printf("║ (5) 显示家谱统计信息                     ║\n");
    printf("║ (0) 退出程序                             ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n请选择操作 (0-5): ");
}

int main() {
    FamilyTree *tree = initialize_family_tree();
    if (tree == NULL) {
        fprintf(stderr, "初始化家谱失败\n");
        return 1;
    }

    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║        欢迎使用家谱管理系统             ║\n");
    printf("║     （树型存储结构 - 孔子世家示例）    ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    int choice;
    char keyword[MAX_NAME_LEN];
    char parent_name[MAX_NAME_LEN];

    while (1) {
        print_menu1();
        scanf("%d", &choice);
        getchar();  /* 消费换行符 */

        switch (choice) {
            case 1: {
                printf("\n✓ 家谱已创建，初始数据已加载\n");
                printf("└─ 创始人：孔子（字仲尼，谥至圣）\n");
                printf("   ├─ 孔鲤（字伯鱼）\n");
                printf("   │   └─ 孔伋（字子思，谥述圣）\n");
                printf("   │       └─ 孔汲（字白）\n");
                printf("   │           └─ 孔夫（字君）\n");
                printf("   │               └─ 孔睦（字伯玉）\n");
                printf("\n分支：王羲之（字逸少，谥书圣）等\n");
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

                TreeNode *found = tree_search_member(tree, keyword);
                if (found != NULL) {
                    printf("\n✓ 查询成功！\n\n");
                    print_member(found->member);

                    if (found->parent != NULL) {
                        printf("\n父亲信息：\n");
                        printf("  %s（字%s）\n",
                               found->parent->member->name,
                               found->parent->member->courtesy_name);
                    } else {
                        printf("\n该成员为族谱的创始人\n");
                    }

                    printf("\n子女：\n");
                    int child_count = 0;
                    if (found->left != NULL) {
                        printf("  - %s\n", found->left->member->name);
                        child_count++;
                    }
                    if (found->right != NULL) {
                        printf("  - %s\n", found->right->member->name);
                        child_count++;
                    }
                    if (child_count == 0) {
                        printf("  （无记录）\n");
                    }
                } else {
                    printf("✗ 未找到相关成员\n");
                }
                break;
            }

            case 3: {
                printf("\n请输入要查询祖先的成员名字: ");
                fgets(keyword, MAX_NAME_LEN, stdin);
                keyword[strcspn(keyword, "\n")] = 0;

                if (strlen(keyword) == 0) {
                    printf("关键字不能为空\n");
                    break;
                }

                TreeNode *found = tree_search_member(tree, keyword);
                if (found != NULL) {
                    tree_search_ancestors(found, 5);
                } else {
                    printf("✗ 未找到该成员\n");
                }
                break;
            }

            case 4: {
                printf("\n请输入父亲的名字: ");
                fgets(parent_name, MAX_NAME_LEN, stdin);
                parent_name[strcspn(parent_name, "\n")] = 0;

                TreeNode *parent = tree_search_member(tree, parent_name);
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
                    tree_insert_member(tree, new_member, parent);
                    printf("✓ 新成员已添加到 %s 下\n", parent->member->name);
                }
                break;
            }

            case 5: {
                printf("\n╔══════════════════════════════════════════╗\n");
                printf("║        家谱统计信息                      ║\n");
                printf("╠══════════════════════════════════════════╣\n");
                printf("║ 创始人：孔子（字仲尼）                  ║\n");
                printf("║ 记录代数：5 代                           ║\n");
                printf("║ 主要分支：孔子世家、王氏家族             ║\n");
                printf("║ 特色功能：                               ║\n");
                printf("║   - 快速成员查询（二叉树）               ║\n");
                printf("║   - 直系祖先追溯                        ║\n");
                printf("║   - 完整历史坐标记录                    ║\n");
                printf("╚══════════════════════════════════════════╝\n");
                break;
            }

            case 0: {
                printf("\n感谢使用家谱管理系统，再见！\n\n");
                destroy_family_tree(tree);
                return 0;
            }

            default:
                printf("✗ 无效的选择，请重试\n");
        }
    }

    return 0;
}
