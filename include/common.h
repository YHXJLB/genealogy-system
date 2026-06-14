#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_COURTESY_NAME_LEN 50
#define MAX_POSTHUMOUS_NAME_LEN 50
#define MAX_DYNASTY_LEN 30
#define MAX_POSITION_LEN 50
#define MAX_COORDINATES_LEN 100
#define MAX_BIRTH_DEATH_LEN 30
#define MAX_ACHIEVEMENT_LEN 200
#define MAX_CHILDREN 10

/* 家族成员的基本信息 */
typedef struct {
    char name[MAX_NAME_LEN];                  /* 姓名 */
    char courtesy_name[MAX_COURTESY_NAME_LEN]; /* 字号 */
    char posthumous_name[MAX_POSTHUMOUS_NAME_LEN]; /* 谥号 */
    char dynasty[MAX_DYNASTY_LEN];            /* 朝代 */
    char position[MAX_POSITION_LEN];          /* 官职 */
    char coordinates[MAX_COORDINATES_LEN];    /* 历史坐标 */
    char birth_death[MAX_BIRTH_DEATH_LEN];    /* 生卒 */
    char achievement[MAX_ACHIEVEMENT_LEN];    /* 成就 */
    int member_id;                            /* 成员编号 */
} FamilyMember;

/* 打印成员信息 */
void print_member(FamilyMember *member);

/* 创建家族成员 */
FamilyMember* create_member(const char *name, const char *courtesy_name,
                            const char *posthumous_name, const char *dynasty,
                            const char *position, const char *coordinates,
                            const char *birth_death, const char *achievement,
                            int member_id);

/* 比较两个成员的姓名 */
int compare_members(const FamilyMember *m1, const FamilyMember *m2);

#endif
