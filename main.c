#include "cfgtocnf.h"

int main()
{
    char start_v;
    char variables[MAX_LEN];
    char terminals[MAX_LEN];
    char production[MAX_LEN];
    CFANodePtr cfa = (CFANodePtr)malloc(sizeof(CFANode));


    FILE *fp = fopen("cfg.txt","r");
    if(fp != NULL)
    {
        start_v = fgetc(fp);
        fgetc(fp);
        fgets(variables, MAX_LINE, fp);
        fgets(terminals, MAX_LINE, fp);
        variables[strlen(variables)-1] = '\0';
        terminals[strlen(terminals)-1] = '\0';
        init(cfa,variables,terminals,start_v);
        while(fgets(production, MAX_LINE, fp))
        {
            production[strlen(production)-1] = '\0';
            addProtoCFA(cfa->rule,production);
        }
        fclose(fp);
    }else
    {
        printf("Read file error!");
    }
    printf("================CFG==================\n");
    printCNF(cfa->rule);
    printf("================CNF==================\n");
    removeEmptyChars(cfa->rule);
    removeSinVar(cfa->rule);
    removeTerminals(cfa);
    printCNF(cfa->rule);
    
    free(cfa);
    return 0;
}