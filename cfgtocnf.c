#include "cfgtocnf.h"

void init(CFANodePtr cfaNodePtr, char *variables, char *terminals, char start_v)
{
    cfaNodePtr->variables = (char*)malloc(sizeof(char)*MAX_LEN);
    cfaNodePtr->terminals = (char*)malloc(sizeof(char)*strlen(terminals));
    cfaNodePtr->variables = variables;
    cfaNodePtr->terminals = terminals;
    cfaNodePtr->start_v = start_v;

    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    cfaNodePtr->rule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    tempRule = cfaNodePtr->rule;
    tempRule->variable = start_v;
    tempRule->deletedPos = 0;
    tempRule->flag = 0;
    tempRule->first_rule_r = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    tempRule->first_rule_r->rule_r = (char*)malloc(sizeof(char)*2);
    tempRule->first_rule_r->rule_r[0] = variables[0];
    tempRule->first_rule_r->rule_r[1] = '\0';
    tempRule->first_rule_r->next_rule_r = NULL;
    tempRule->next_rule_l = NULL;

    int i = 0;
    while(variables[i] != '\0')
    {
        tempRule->next_rule_l = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
        tempRule->next_rule_l->variable = variables[i];
        tempRule->tempVar = ' ';
        tempRule->deletedPos = 0;
        tempRule->flag = 0;
        tempRule->next_rule_l->first_rule_r = NULL;
        tempRule->next_rule_l->next_rule_l = NULL;

        tempRule = tempRule->next_rule_l;
        i++;
    }
}

void addProtoCFA(RuleLeftNodePtr rule, char *production)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr newRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    char variable = production[0];
    newRightRule->rule_r = (char*)malloc(sizeof(char)*(strlen(production)-3));
    strcpy(newRightRule->rule_r,production+3);
    newRightRule->next_rule_r = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    newRightRule->next_rule_r = NULL;
    tempRule = rule;
    while(tempRule)
    {
        if(tempRule->variable == variable)
        {
            tempRightRule = tempRule->first_rule_r;
            if(tempRightRule)
            {
                while(tempRightRule->next_rule_r != NULL)
                {
                    tempRightRule = tempRightRule->next_rule_r;
                }
                tempRightRule->next_rule_r = newRightRule;
            }
            else
            {
                tempRule->first_rule_r = newRightRule;
            }
        }
        tempRule = tempRule->next_rule_l;
    }
}

int addOneRule(RuleRightNodePtr rule, char *one_rule)
{
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr newRightNode;

    tempRightRule = rule;
    while(tempRightRule->next_rule_r != NULL && strcmp(tempRightRule->rule_r, one_rule))
        tempRightRule = tempRightRule->next_rule_r;
    if(!strcmp(tempRightRule->rule_r, one_rule))
        return 0;
    newRightNode = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    newRightNode->rule_r = (char*)malloc(sizeof(char)*strlen(one_rule));
    strcpy(newRightNode->rule_r, one_rule);
    newRightNode->next_rule_r = NULL;
    tempRightRule->next_rule_r = newRightNode;

    //free(tempRightRule);
    return 1;
}

int removeOneRule(RuleRightNodePtr preRule, RuleRightNodePtr postRule)
{
    //RuleRightNodePtr tempRight = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    preRule->next_rule_r = postRule->next_rule_r;
    //tempRight = postRule;
    postRule = postRule->next_rule_r;
    //free(tempRight);
}

char findEmptyChar(RuleLeftNodePtr rule)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr tempPreRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    tempRule = rule;
    
    while(tempRule != NULL)
    {
        tempRightRule = tempRule->first_rule_r;
        tempPreRule = NULL;
        while(tempRightRule != NULL)
        {
            // printf("%s ",tempRightRule->rule_r);
            if(tempRightRule->rule_r[0] == 'e')
            {
                // printf("char:%c",tempRule->variable);
                tempRule->flag = 1;
                if(!tempPreRule)
                {
                    tempRule->first_rule_r = tempRightRule->next_rule_r;
                    tempRightRule = NULL;
                }else
                {
                    tempPreRule->next_rule_r = tempRightRule->next_rule_r;
                    tempRightRule = tempRightRule->next_rule_r;
                }
                // printf("%c",tempRule->variable);
                return tempRule->variable;
            }
            else
            {
                tempPreRule = tempRightRule;
                tempRightRule = tempRightRule->next_rule_r;
            }
        }
        tempRule = tempRule->next_rule_l;
    }
    // free(tempRule);
    // free(tempRightRule);
    // free(tempPreRule);
    return '#';
}

RuleLeftNodePtr findSinVar(RuleLeftNodePtr rule)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr tempPreRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    tempRule = rule;
    
    while(tempRule != NULL)
    {
        tempRightRule = tempRule->first_rule_r;
        tempPreRule = NULL;
        while(tempRightRule != NULL)
        {
            if(strlen(tempRightRule->rule_r) == 1 && tempRightRule->rule_r[0] >= 65 
            && tempRightRule->rule_r[0] <= 90)
            {
                tempRule->tempVar = tempRightRule->rule_r[0];
                if(tempRule->deletedPos < 50)
                    tempRule->deletedVar[tempRule->deletedPos++] = tempRule->tempVar;
                if(!tempPreRule)
                {
                    tempRule->first_rule_r = tempRightRule->next_rule_r;
                    tempRightRule = tempRightRule->next_rule_r;
                }else
                {
                    tempPreRule->next_rule_r = tempRightRule->next_rule_r;
                    tempRightRule = tempRightRule->next_rule_r;
                }
                // printf("%c",tempRule->variable);
                return tempRule;
            }
            else
            {
                tempPreRule = tempRightRule;
                tempRightRule = tempRightRule->next_rule_r;
            }
        }
        tempRule = tempRule->next_rule_l;
    }
    return NULL;
}

void removeOneEmptyChar(RuleLeftNodePtr rule, char variable)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr tempLastRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    IntArrayListPtr arrayList = (IntArrayListPtr)malloc(sizeof(IntArrayList));
    IntArrayListPtr tempList = (IntArrayListPtr)malloc(sizeof(IntArrayList));
    int count = 1;
    initArrayList(arrayList);
    tempRule = rule;
    while(tempRule != NULL)
    {
        tempRightRule = tempRule->first_rule_r;
        tempLastRule = tempRule->first_rule_r;
        while(tempLastRule->next_rule_r != NULL)
            tempLastRule = tempLastRule->next_rule_r;
        while(tempRightRule != tempLastRule->next_rule_r)
        {
            arrayList = findVariableInRule(tempRightRule->rule_r, variable);
            int length = getListLen(arrayList);
            if(length)
            {
                count = 1;
                while(count < (1<<length))
                {
                    initArrayList(tempList);
                    for(int i = 0; i < length; i++)
                    {
                        if(count & (1<<i))
                            addElement(tempList, getElement(arrayList, i));
                    }
                    //printf("tempList:%d",tempList->length);
                    if(!tempRule->flag||strcmp(getNewRule(tempRightRule->rule_r, tempList),"e"))
                        addOneRule(tempRightRule, getNewRule(tempRightRule->rule_r, tempList));
                    // printf("str:%s\n",getNewRule(tempRightRule->rule_r, tempList));
                    count ++;
                }
            }
            tempRightRule = tempRightRule->next_rule_r;
        }
        tempRule = tempRule->next_rule_l;
    }
    
    free(arrayList);
    free(tempList);
}

int removeEmptyChars(RuleLeftNodePtr rule)
{
    char variable = findEmptyChar(rule);
    if(variable == '#')
        return 1;
    else
    {
        removeOneEmptyChar(rule, variable);
        removeEmptyChars(rule);
    }
}

void removeSinVar(RuleLeftNodePtr rule)
{
    RuleLeftNodePtr tempLeft = (RuleLeftNodePtr)malloc(sizeof(RuleRightNode));
    tempLeft = findSinVar(rule);
    if(!tempLeft)
        return;
    else
    {
        // printCNF(rule);
        removeOneSinVar(rule,tempLeft,tempLeft->tempVar);
        removeSinVar(rule);
    }
}

void removeOneSinVar(RuleLeftNodePtr rule, RuleLeftNodePtr leftRule, char variable)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr tempRR= (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleRightNodePtr newRightNode = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    tempRule = rule;
    tempRR = leftRule->first_rule_r;
    while(tempRule->variable != variable && tempRule)
        tempRule = tempRule->next_rule_l;
    tempRightRule = tempRule->first_rule_r;
    if(!tempRR)
    {
        newRightNode->rule_r = (char*)malloc(sizeof(char)*strlen(tempRightRule->rule_r));
        strcpy(newRightNode->rule_r, tempRightRule->rule_r);
        newRightNode->next_rule_r = NULL;
        leftRule->first_rule_r = newRightNode;

        tempRightRule = tempRightRule->next_rule_r;
        tempRR = newRightNode;
    }
    while(tempRightRule)
    {
        if(strlen(tempRightRule->rule_r) == 1)
        {
            int i;
            for(i = 0; i < leftRule->deletedPos; i++)
            {
                if(tempRightRule->rule_r[0] == leftRule->deletedVar[i])
                    break;
            }
            if(i == leftRule->deletedPos)
                addOneRule(tempRR, tempRightRule->rule_r);
        }else
            addOneRule(tempRR, tempRightRule->rule_r);
        tempRightRule = tempRightRule->next_rule_r;
    }
}

void removeTerminals(CFANodePtr cfa)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleLeftNodePtr curEndRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    char duplicateVar[MAX_LEN];
    char replace[MAX_LEN];
    curEndRule = cfa->rule;
    while(curEndRule->next_rule_l != NULL)
        curEndRule = curEndRule->next_rule_l;
    tempRule = cfa->rule;
    while(tempRule)
    {
        tempRightRule = tempRule->first_rule_r;
        while(tempRightRule)
        {
            if(strlen(tempRightRule->rule_r)>=3)
            {
                int len = strlen(tempRightRule->rule_r);
                char newVar = genNewVar(cfa->variables);
                cfa->variables[strlen(cfa->variables)] = newVar;
                cfa->variables[strlen(cfa->variables) + 1] = '\0';

                char *tempRuleStr = (char *)malloc(sizeof(char)*3);
                char *newRule = (char *)malloc(sizeof(char)*(len));
                
                *tempRuleStr = newVar;
                *(tempRuleStr + 1) = tempRightRule->rule_r[len - 1];
                *(tempRuleStr + 2) = '\0';
                tempRightRule->rule_r[len - 1] = '\0';
                strcpy(newRule, tempRightRule->rule_r);
                strcpy(tempRightRule->rule_r,tempRuleStr);
                free(tempRuleStr);

                addOneVar(tempRule, newVar, newRule);

                if(tempRightRule->rule_r[1] > 90)
                {
                    char new_var = genNewVar(cfa->variables);
                    cfa->variables[strlen(cfa->variables)] = newVar;
                    cfa->variables[strlen(cfa->variables) + 1] = '\0';

                    char *new_rule = (char *)malloc(sizeof(char) * 2);
                    *new_rule = tempRightRule->rule_r[1];
                    *(new_rule + 1) = '\0';
                    tempRightRule->rule_r[1] = new_var;

                    addOneVar(tempRule, new_var, new_rule);
                }
                
            }else if(strlen(tempRightRule->rule_r) == 2)
            {
                if(tempRightRule->rule_r[0] > 90)
                {
                    char newVar = genNewVar(cfa->variables);
                    cfa->variables[strlen(cfa->variables)] = newVar;
                    cfa->variables[strlen(cfa->variables) + 1] = '\0';

                    char *newRule = (char *)malloc(sizeof(char) * 2);
                    *newRule = tempRightRule->rule_r[0];
                    *(newRule + 1) = '\0';
                    tempRightRule->rule_r[0] = newVar;

                    addOneVar(tempRule, newVar, newRule);
                }
                if(tempRightRule->rule_r[1] > 90)
                {
                    char newVar = genNewVar(cfa->variables);
                    cfa->variables[strlen(cfa->variables)] = newVar;
                    cfa->variables[strlen(cfa->variables) + 1] = '\0';

                    char *newRule = (char *)malloc(sizeof(char) * 2);
                    *newRule = tempRightRule->rule_r[1];
                    *(newRule + 1) = '\0';
                    tempRightRule->rule_r[1] = newVar;

                    addOneVar(tempRule, newVar, newRule);
                }
            }
            tempRightRule = tempRightRule->next_rule_r;
        }
        tempRule = tempRule->next_rule_l;
    }
    strcpy(duplicateVar,merge(curEndRule, replace));
    deduplicate(cfa->rule, curEndRule, duplicateVar, replace);
}

void addOneVar(RuleLeftNodePtr leftRule, char newVar, char *newRule)
{
    RuleLeftNodePtr newLeftNode = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr newRightNode = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));
    RuleLeftNodePtr tempLeftNode = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    
    tempLeftNode = leftRule;

    newRightNode->rule_r = newRule;
    newRightNode->next_rule_r = NULL;
    
    newLeftNode->variable = newVar;
    newLeftNode->tempVar = '$';
    newLeftNode->deletedPos = 0;
    newLeftNode->flag = 0;
    newLeftNode->next_rule_l = NULL;
    newLeftNode->first_rule_r = newRightNode;

    while(tempLeftNode->next_rule_l != NULL)
        tempLeftNode = tempLeftNode->next_rule_l;
    tempLeftNode->next_rule_l = newLeftNode;
}

char *merge(RuleLeftNodePtr rule, char *replace)
{
    RuleLeftNodePtr tempRule_i = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleLeftNodePtr tempRule_j = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleLeftNodePtr tempRule_pre_j = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    char temp_p[MAX_LEN];
    char temp_q[MAX_LEN];
    char *redundant = (char *)malloc(sizeof(char)*MAX_LEN);
    int count = 0;
    tempRule_i = rule->next_rule_l;
    while(tempRule_i != NULL)
    {
        tempRule_pre_j = tempRule_i;
        tempRule_j = tempRule_i->next_rule_l;
        strcpy(temp_p,tempRule_i->first_rule_r->rule_r);
        while(tempRule_j != NULL)
        {
            strcpy(temp_q,tempRule_j->first_rule_r->rule_r);
            if(!strcmp(temp_p,temp_q))
            {
                replace[count] = tempRule_i->variable;
                redundant[count++] = tempRule_j->variable;
                tempRule_pre_j->next_rule_l = tempRule_j->next_rule_l;
                tempRule_j = tempRule_j->next_rule_l;
            }else
            {
                tempRule_pre_j = tempRule_j;
                tempRule_j = tempRule_j->next_rule_l;    
            }
        }
        tempRule_i = tempRule_i->next_rule_l;
    }
    redundant[count] = '\0';
    replace[count] = '\0';
    return redundant;
}

void deduplicate(RuleLeftNodePtr rule, RuleLeftNodePtr endRule, char *duplicateVar, char *replace)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));

    tempRule = rule;
    while(tempRule != endRule->next_rule_l)
    {
        tempRightRule = tempRule->first_rule_r;
        while(tempRightRule)
        {
            int i = 0;
            while(tempRightRule->rule_r[i] != '\0')
            {
                for(int j = 0; j < strlen(duplicateVar); j++)
                {
                    if(duplicateVar[j] == tempRightRule->rule_r[i])
                    {
                        tempRightRule->rule_r[i] = replace[j];
                        break;
                    }
                }
                i++;
            }
            tempRightRule = tempRightRule->next_rule_r;
        }
        tempRule = tempRule->next_rule_l;
    }
}

//tools
IntArrayListPtr findVariableInRule(char *rule, char variable)
{
    IntArrayListPtr arrayList = (IntArrayListPtr)malloc(sizeof(IntArrayList));
    initArrayList(arrayList);
    int i = 0;
    while(rule[i] != '\0')
    {
        if(rule[i] == variable)
            addElement(arrayList, i);
        i++;
    }
    return arrayList;
}

char *getNewRule(char *rule, IntArrayListPtr arrayList)
{
    char *newRule;
    int len = getListLen(arrayList);
    if(strlen(rule) - len == 0){
        newRule = (char*)malloc(sizeof(char)*2);
        *newRule = 'e';
        *(newRule + 1) = '\0';
    }else{
        newRule = (char*)malloc(sizeof(char)*(strlen(rule)-len));
        int i = 0, count = 0;
        while(rule[i] != '\0')
        {
            if(!isInList(arrayList, i))
            {
                newRule[count] = rule[i];
                count++;
            }
            i++;
        }
        newRule[count] = '\0';
    }
    return newRule;
}

void printCNF(RuleLeftNodePtr rule)
{
    RuleLeftNodePtr tempRule = (RuleLeftNodePtr)malloc(sizeof(RuleLeftNode));
    RuleRightNodePtr tempRightRule = (RuleRightNodePtr)malloc(sizeof(RuleRightNode));

    tempRule = rule;
    while(tempRule)
    {
        printf("%c->",tempRule->variable);
        tempRightRule = tempRule->first_rule_r;
        while(tempRightRule)
        {
            //printf("%d",tempRightRule->rule_r[0]);
            if(tempRightRule->rule_r[0] == 'e')
                printf("e");
            else
                printf("%s",tempRightRule->rule_r);
            if(tempRightRule->next_rule_r)
                printf("|");
            tempRightRule = tempRightRule->next_rule_r;
        }
        tempRule = tempRule->next_rule_l;
        printf("\n");
    }
}

char genNewVar(char *variables)
{
    int len = strlen(variables);
    int count = 65;
    for(;count <= 90; count++)
    {
        int i;
        for(i = 0; i < len; i++)
        {
            if(count == variables[i])
                break;
        }
        if(i == len)
            return count;
    }
}
// int isInVariables(char *variables, char variable)
// {
//     int i = 0;
//     while(variables[i] != '\0')
//     {
//         if(variables[i] == variable)
//             return 1;
//     }
// }

//IntArrayList
int getListLen(IntArrayListPtr arrayList)
{
    return arrayList->length;
}

int getElement(IntArrayListPtr arrayList, int pos)
{
    return arrayList->list[pos];
}

void addElement(IntArrayListPtr arrayList, int element)
{
    arrayList->list[arrayList->length] = element;
    arrayList->length++;
}

void initArrayList(IntArrayListPtr arrayList)
{
    arrayList->length = 0;
}

int isInList(IntArrayListPtr arrayList,int element)
{
    for(int i = 0; i < arrayList->length; i++)
    {
        if(element == getElement(arrayList, i))
            return 1;
    }
    return 0;
}
