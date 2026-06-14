#include "common.h"

/* 打印成员信息 */
void print_member(FamilyMember *member) {
    if (member == NULL) return;

    printf("┌──────────────────────────────────────┐\n");
    printf("│ 姓名：%-30s │\n", member->name);
    printf("├──────────────────────────────────────┤\n");
    printf("│ 字号：%-30s │\n", member->courtesy_name);
    printf("│ 谥号：%-30s │\n", member->posthumous_name);
    printf("│ 朝代：%-30s │\n", member->dynasty);
    printf("│ 官职：%-30s │\n", member->position);
    printf("│ 生卒：%-30s │\n", member->birth_death);
    printf("├──────────────────────────────────────┤\n");
    printf("│ 历史坐标：\n│ %s\n", member->coordinates);
    printf("├──────────────────────────────────────┤\n");
    printf("│ 主要成就：\n│ %s\n", member->achievement);
    printf("└──────────────────────────────────────┘\n");
}

/* 创建家族成员 */
FamilyMember* create_member(const char *name, const char *courtesy_name,
                            const char *posthumous_name, const char *dynasty,
                            const char *position, const char *coordinates,
                            const char *birth_death, const char *achievement,
                            int member_id) {
    FamilyMember *member = (FamilyMember*)malloc(sizeof(FamilyMember));
    if (member == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }

    strncpy(member->name, name, MAX_NAME_LEN - 1);
    member->name[MAX_NAME_LEN - 1] = '\0';

    strncpy(member->courtesy_name, courtesy_name, MAX_COURTESY_NAME_LEN - 1);
    member->courtesy_name[MAX_COURTESY_NAME_LEN - 1] = '\0';

    strncpy(member->posthumous_name, posthumous_name, MAX_POSTHUMOUS_NAME_LEN - 1);
    member->posthumous_name[MAX_POSTHUMOUS_NAME_LEN - 1] = '\0';

    strncpy(member->dynasty, dynasty, MAX_DYNASTY_LEN - 1);
    member->dynasty[MAX_DYNASTY_LEN - 1] = '\0';

    strncpy(member->position, position, MAX_POSITION_LEN - 1);
    member->position[MAX_POSITION_LEN - 1] = '\0';

    strncpy(member->coordinates, coordinates, MAX_COORDINATES_LEN - 1);
    member->coordinates[MAX_COORDINATES_LEN - 1] = '\0';

    strncpy(member->birth_death, birth_death, MAX_BIRTH_DEATH_LEN - 1);
    member->birth_death[MAX_BIRTH_DEATH_LEN - 1] = '\0';

    strncpy(member->achievement, achievement, MAX_ACHIEVEMENT_LEN - 1);
    member->achievement[MAX_ACHIEVEMENT_LEN - 1] = '\0';

    member->member_id = member_id;

    return member;
}

/* 比较两个成员的姓名 */
int compare_members(const FamilyMember *m1, const FamilyMember *m2) {
    if (m1 == NULL || m2 == NULL) return 0;
    return strcmp(m1->name, m2->name);
}
