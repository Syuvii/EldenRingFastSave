#include <stdlib.h>
#include <stdio.h>

#define MAX 100

int main(int argc, char** argv) 
{
    char* USERPROFILE = getenv("USERPROFILE");

    char SRCDIR[MAX];
    char DSTDIR[MAX];
    sprintf(SRCDIR, "%s\\AppData\\Roaming\\EldenRing", USERPROFILE);
    sprintf(DSTDIR, "%s\\Documents\\EldenRingBackFile", USERPROFILE);

    printf("/* enter 0 to exit, 1 to save, 2 to load, 3 to delete */\n");
    printf("1.save\n2.load\n3.delete\n");


    char subDir[MAX];
    char newDir[MAX];
    char cmd[MAX];  

    while(1)
    {
        int op; scanf("%d", &op);
        if (op == 0)
        {
            return 0;
        }
        else if (op == 1)
        {
            printf("save name:");
            scanf("%s", subDir);
            sprintf(newDir, "%s\\%s", DSTDIR, subDir);
            sprintf(cmd, "md %s", newDir);
            system(cmd);
            sprintf(cmd, "xcopy %s %s /s", SRCDIR, newDir);
            system(cmd);
        }
        else if (op == 2)
        {
            printf("you can load from the saves below:\n");
            sprintf(cmd, "dir %s /a/d", DSTDIR);
            system(cmd);
            printf("load name:");
            scanf("%s", subDir);
            sprintf(newDir, "%s\\%s", DSTDIR, subDir);
            sprintf(cmd, "xcopy %s %s /s", newDir, SRCDIR);
            system(cmd);
        }
        else if (op == 3)
        {
            printf("you can delete the saves below:\n");
            sprintf(cmd, "dir %s /a/d", DSTDIR);
            system(cmd);
            printf("delete name:");
            scanf("%s", subDir);
            sprintf(newDir, "%s\\%s", DSTDIR, subDir);
            sprintf(cmd, "rd %s /s", newDir);
            system(cmd);
        }
        else
        {
            printf("Error: please enter 0 or 1 or 2 or 3\n");
        }
    }

    return 0;
}