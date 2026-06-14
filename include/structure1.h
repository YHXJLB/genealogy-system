#ifndef STRUCTURE1_H
#define STRUCTURE1_H

#include "common.h"

/* 树型节点 - 用于二叉搜索树 */
typedef struct TreeNode {
    FamilyMember *member;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;  /* 指向父亲节点，用于查询祖先 */
} TreeNode;

/* 链表节点 - 用于存储子女列表 */
typedef struct ChildNode {
    struct TreeNode *child;
    struct ChildNode *next;
} ChildNode;

/* 扩展树节点，包含子女链表 */
typedef struct {
    TreeNode *root;
} FamilyTree;

/* 创建家谱树 */
FamilyTree* create_family_tree();

/* 在树中插入成员 */
void tree_insert_member(FamilyTree *tree, FamilyMember *member, TreeNode *parent_node);

/* 在树中查询成员（通过姓名、字号或谥号） */
TreeNode* tree_search_member(FamilyTree *tree, const char *keyword);

/* 查询指定成员的所有祖先（至少5代） */
void tree_search_ancestors(TreeNode *node, int generation);

/* 添加子女关系 */
void tree_add_child(TreeNode *parent, TreeNode *child);

/* 删除树 */
void tree_delete(TreeNode *node);

/* 销毁家谱树 */
void destroy_family_tree(FamilyTree *tree);

/* 初始化家谱 */
FamilyTree* initialize_family_tree();

#endif
