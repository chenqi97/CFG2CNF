#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50
#define MAX_LINE 255

typedef struct RuleRight
{
    char *rule_r;
    struct RuleRight *next_rule_r;
}RuleRightNode, *RuleRightNodePtr;

typedef struct RuleLeft
{
    char variable;
    char tempVar;
    int varFlag;
    int flag;//falg = 1 e has been deleted
    struct RuleLeft *next_rule_l;
    struct RuleRight *first_rule_r;
}RuleLeftNode, *RuleLeftNodePtr;

typedef struct CFA
{
    char *variables;
    char *terminals;
    struct RuleLeft *rule;
    char start_v;
}CFANode, *CFANodePtr;

typedef struct IntList
{
    int list[MAX_LEN];
    int length;
}IntArrayList, *IntArrayListPtr;

void init(CFANodePtr cfaNodePtr, char *variables, char *terminals, char start_v);
void addProtoCFA(RuleLeftNodePtr rule,char *production);
int addOneRule(RuleRightNodePtr rule, char *one_rule);
int removeOneRule(RuleRightNodePtr preRule, RuleRightNodePtr postRule);
int removeEmptyChars(RuleLeftNodePtr rule);
char findEmptyChar(RuleLeftNodePtr rule);
RuleLeftNodePtr findSinVar(RuleLeftNodePtr rule);
void removeOneEmptyChar(RuleLeftNodePtr rule, char variable);
void removeSinVar(RuleLeftNodePtr rule);
void removeOneSinVar(RuleLeftNodePtr rule, RuleLeftNodePtr leftRule,char variable);
void removeTerminals(CFANodePtr cfa);
void addOneVar(RuleLeftNodePtr leftRule, char newVar, char *newRule);
char *merge(RuleLeftNodePtr rule, char *replace);
void deduplicate(RuleLeftNodePtr rule, RuleLeftNodePtr endRule, char *duplicateVar, char *replace);
//tools
void printNFC(CFANodePtr cfa);
IntArrayListPtr findVariableInRule(char *rule, char variable);
char *getNewRule(char *rule, IntArrayListPtr arrayList);
void printCNF(RuleLeftNodePtr rule);
char genNewVar(char *variables);
//int isInVariables(char *variables, char variable);
//IntArrayList
void initArrayList(IntArrayListPtr arrayList);
int getListLen(IntArrayListPtr arrayList);
int getElement(IntArrayListPtr arrayList, int pos);
void addElement(IntArrayListPtr arrayList, int element);
int isInList(IntArrayListPtr arrayList, int element);