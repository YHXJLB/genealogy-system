#include "structure1.h"

/* 创建家谱树 */
FamilyTree* create_family_tree() {
    FamilyTree *tree = (FamilyTree*)malloc(sizeof(FamilyTree));
    if (tree == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

/* 创建树节点 */
static TreeNode* create_tree_node(FamilyMember *member, TreeNode *parent) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }
    node->member = member;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent;
    return node;
}

/* 在树中插入成员 */
void tree_insert_member(FamilyTree *tree, FamilyMember *member, TreeNode *parent_node) {
    if (tree == NULL || member == NULL) return;

    if (tree->root == NULL) {
        /* 第一个成员作为根节点 */
        tree->root = create_tree_node(member, NULL);
        return;
    }

    /* 在指定的父节点下添加子女 */
    if (parent_node == NULL) return;

    TreeNode *new_node = create_tree_node(member, parent_node);
    if (new_node == NULL) return;

    /* 添加到父节点的子女列表中（简化处理：左右子树存储） */
    if (parent_node->left == NULL) {
        parent_node->left = new_node;
    } else if (parent_node->right == NULL) {
        parent_node->right = new_node;
    } else {
        /* 如果左右子树都已满，则递归添加到左子树 */
        tree_insert_member(tree, member, parent_node->left);
    }
}

/* 递归搜索成员 */
static TreeNode* _tree_search_recursive(TreeNode *node, const char *keyword) {
    if (node == NULL) return NULL;

    FamilyMember *member = node->member;

    /* 按姓名、字号、谥号搜索 */
    if (strcmp(member->name, keyword) == 0 ||
        strcmp(member->courtesy_name, keyword) == 0 ||
        strcmp(member->posthumous_name, keyword) == 0) {
        return node;
    }

    /* 递归搜索左右子树 */
    TreeNode *result = _tree_search_recursive(node->left, keyword);
    if (result != NULL) return result;

    return _tree_search_recursive(node->right, keyword);
}

/* 在树中查询成员 */
TreeNode* tree_search_member(FamilyTree *tree, const char *keyword) {
    if (tree == NULL || keyword == NULL) return NULL;
    return _tree_search_recursive(tree->root, keyword);
}

/* 查询指定成员的所有祖先（至少5代） */
void tree_search_ancestors(TreeNode *node, int generation) {
    if (node == NULL) {
        printf("未找到祖先\n");
        return;
    }

    printf("\n========== 祖先查询结果 =========="\n);
    printf("从 %s 开始查询，���多显示 %d 代祖先\n\n", node->member->name, generation);

    TreeNode *current = node->parent;
    int gen = 1;

    while (current != NULL && gen <= generation) {
        printf("第 %d 代祖先：\n", gen);
        print_member(current->member);
        printf("\n");
        current = current->parent;
        gen++;
    }

    if (gen == 1) {
        printf("该成员没有祖先记录\n");
    } else {
        printf("总共查询了 %d 代祖先\n", gen - 1);
    }
    printf("================================\n\n");
}

/* 添加子女关系 */
void tree_add_child(TreeNode *parent, TreeNode *child) {
    if (parent == NULL || child == NULL) return;

    if (parent->left == NULL) {
        parent->left = child;
    } else if (parent->right == NULL) {
        parent->right = child;
    }
    child->parent = parent;
}

/* 删除树 */
void tree_delete(TreeNode *node) {
    if (node == NULL) return;

    tree_delete(node->left);
    tree_delete(node->right);

    if (node->member != NULL) {
        free(node->member);
    }
    free(node);
}

/* 销毁家谱树 */
void destroy_family_tree(FamilyTree *tree) {
    if (tree == NULL) return;

    tree_delete(tree->root);
    free(tree);
}

/* 初始化家谱 - 孔子世家 */
FamilyTree* initialize_family_tree() {
    FamilyTree *tree = create_family_tree();
    if (tree == NULL) return NULL;

    /* 添加孔子 */
    FamilyMember *kongzi = create_member(
        "孔子",
        "仲尼",
        "至圣",
        "春秋",
        "鲁国司寇",
        "公元前551年孔子诞生-公元前479年逝世",
        "前551-前479",
        "儒家文化的奠基者，中国历史上最伟大的教育家和思想家",
        1
    );

    tree_insert_member(tree, kongzi, NULL);

    /* 添加孔子的子女 */
    FamilyMember *kongji = create_member(
        "孔鲤",
        "伯鱼",
        "泽",
        "春秋",
        "鲁国大夫",
        "孔子之子",
        "前533-前483",
        "孔子的儿子，传承儒家思想",
        2
    );

    TreeNode *kongzi_node = tree_search_member(tree, "孔子");
    tree_insert_member(tree, kongji, kongzi_node);

    FamilyMember *kongji_son = create_member(
        "孔伋",
        "子思",
        "述圣",
        "春秋",
        "鲁国官员",
        "孔子的孙子，著《中庸》",
        "前483-前402",
        "著名思想家，孟子的老师",
        3
    );

    TreeNode *kongji_node = tree_search_member(tree, "孔鲤");
    tree_insert_member(tree, kongji_son, kongji_node);

    /* 添加更多代次 */
    FamilyMember *kongji_grandson = create_member(
        "孔汲",
        "白",
        "成子",
        "春秋",
        "鲁国官员",
        "孔子的曾孙",
        "前470-前395",
        "继续传承儒家文化",
        4
    );

    TreeNode *kongji_son_node = tree_search_member(tree, "孔伋");
    tree_insert_member(tree, kongji_grandson, kongji_son_node);

    FamilyMember *kongji_greatgrandson = create_member(
        "孔夫",
        "君",
        "",
        "春秋",
        "官员",
        "孔子的玄孙",
        "前450-前370",
        "儒家学派的继承者",
        5
    );

    TreeNode *kongji_grandson_node = tree_search_member(tree, "孔汲");
    tree_insert_member(tree, kongji_greatgrandson, kongji_grandson_node);

    /* 添加第五代 */
    FamilyMember *fifth_gen = create_member(
        "孔睦",
        "伯玉",
        "",
        "春秋",
        "官员",
        "孔子的玄玄孙",
        "前420-前340",
        "儒家传人",
        6
    );

    TreeNode *fifth_gen_parent = tree_search_member(tree, "孔夫");
    tree_insert_member(tree, fifth_gen, fifth_gen_parent);

    /* 添加琅琊王氏分支 */
    FamilyMember *wang_xizhi = create_member(
        "王羲之",
        "逸少",
        "书圣",
        "东晋",
        "右军将军",
        "永和九年兰亭雅集-《兰亭集序》",
        "303-361",
        "书法艺术的集大成者，\"书圣\"称号，永和九年参加兰亭雅集，著《兰亭集序》",
        7
    );

    tree_insert_member(tree, wang_xizhi, NULL);

    TreeNode *wang_root = tree_search_member(tree, "王羲之");

    FamilyMember *wang_xianzhi = create_member(
        "王献之",
        "子敬",
        "右军",
        "东晋",
        "侍中",
        "继承父业，成为书法名家",
        "344-386",
        "王羲之之子，书法成就卓著，与父齐名，\"小圣贤庄\"主人公",
        8
    );

    tree_insert_member(tree, wang_xianzhi, wang_root);

    FamilyMember *wang_ningzhi = create_member(
        "王凝之",
        "幼仪",
        "",
        "东晋",
        "太常",
        "王羲之之子",
        "341-388",
        "王羲之的儿子",
        9
    );

    tree_insert_member(tree, wang_ningzhi, wang_root);

    FamilyMember *wang_xun = create_member(
        "王询",
        "伯仁",
        "",
        "东晋",
        "左卫将军",
        "王羲之之子",
        "340-385",
        "王羲之的儿子",
        10
    );

    tree_insert_member(tree, wang_xun, wang_root);

    FamilyMember *wang_huizhi = create_member(
        "王徽之",
        "子玙",
        "",
        "东晋",
        "黄门侍郎",
        "\"王子玙乘兴夜访戴安道\"故事主人公",
        "363-388",
        "名士风流，有名言\"何必要州郡，但得一山水足矣\"",
        11
    );

    tree_insert_member(tree, wang_huizhi, wang_ningzhi);

    return tree;
}
