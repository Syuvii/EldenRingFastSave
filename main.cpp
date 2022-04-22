#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <dirent.h>
#include <unistd.h>

#include "tools.h"

#define DIRLEN 256
#define BUFFSIZE 4096

/*展示提示信息*/
void showGuide()
{
    printf("/* enter 0 to exit, 1 to save, 2 to load, 3 to delete, 4 to show guide*/\n");
    printf("0.exit\n1.save\n2.load\n3.delete\n4.show guide\n");
    printf("/******************************************************/\n");
}

/*显示所有存档*/
void showSaves(char dir[])
{
    DIR* pDir = opendir(dir);
    struct dirent* pDirent;
    if (pDir == NULL)
    {
        printf("[Error]: cannot open dir: [%s]\n", dir);
        closedir(pDir);
        return;
    }
    while((pDirent = readdir(pDir)) != NULL)
    {
        if (!strcmp(pDirent->d_name, ".")) continue;
        if (!strcmp(pDirent->d_name, "..")) continue;
        struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
        char* tempDir = (char*)malloc(sizeof(char)*DIRLEN);
        sprintf(tempDir, "%s\\%s", dir, pDirent->d_name);
        stat(tempDir, buff);
        if (S_ISDIR(buff->st_mode))
        {
            printf("[%s]\n", pDirent->d_name);
        }
    }
    closedir(pDir);
}

int main(int argc, char** argv) 
{
    char* USERPROFILE = getenv("USERPROFILE");
    char* SRCDIR = (char*)malloc(sizeof(char)*DIRLEN);
    char* DSTDIR = (char*)malloc(sizeof(char)*DIRLEN);
    sprintf(SRCDIR, "%s\\AppData\\Roaming\\EldenRing", USERPROFILE);
    sprintf(DSTDIR, "%s\\Documents\\EldenRingBackFile", USERPROFILE);

    char* saveName = (char*)malloc(sizeof(char)*DIRLEN);
    char* srcNewDir = (char*)malloc(sizeof(char)*DIRLEN);
    char* srcFile = (char*)malloc(sizeof(char)*DIRLEN);
    char* dstNewDir = (char*)malloc(sizeof(char)*DIRLEN);
    char* dstFile = (char*)malloc(sizeof(char)*DIRLEN);

    DIR* pDir;
    struct dirent* pDirent;

    // 如果EldenRingBackFile目录不存在则创建
    pDir = opendir(DSTDIR);
    if (pDir == NULL) 
    {
        if (mkdir(DSTDIR) == -1)
        {
            printf("[Error] cannot create dir [EldenRingBackFile]\n");
            system("pause");
            return 0;
        }
    }
    closedir(pDir);

    // 获取所有账号
    std::vector<std::string> accounts;
    getSubDirs(SRCDIR, accounts);

    showGuide();

loop:
    int op = 0; 
    int target = 0;
    time_t modifyTime = 0;
    scanf("%d", &op);
    switch(op)
    {
    // case 0: exit
    case 0:
        system("pause");
        return 0;
    
    // case 1: save
    case 1:
        printf("save name:");
        scanf("%s", saveName);
        
        for (auto account : accounts)
        {
            sprintf(srcNewDir, "%s\\%s", SRCDIR, account.c_str());
            sprintf(dstNewDir, "%s\\%s", DSTDIR, saveName);
            mkdir(dstNewDir);
            sprintf(dstNewDir, "%s\\%s", dstNewDir, account.c_str());
            if (!copyDir(srcNewDir, dstNewDir))
            {
                printf("[Error]: cannot save [%s] for account [%s]\n", saveName, account.c_str());
                goto loop;
            }
        }
        
        printf("save succeed!\n");
        goto loop;

    // case 2: load
    case 2:
        printf("you can load from the saves below:\n");
        showSaves(DSTDIR);
        scanf("%s", saveName);

        target = 0;
        modifyTime = 0;

        for (int i = 0; i < accounts.size(); ++i)
        {
            struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
            char* temp = (char*)malloc(sizeof(char)*DIRLEN);
            sprintf(temp, "%s\\%s\\ER0000.sl2", SRCDIR, accounts[i].c_str());
            if (stat(temp, buff) == -1)
                continue;
            if (modifyTime < buff->st_mtime)
            {
                modifyTime = buff->st_mtime;
                target = i;
            }
        }

        sprintf(srcNewDir, "%s\\%s", SRCDIR, accounts[target].c_str());
        sprintf(dstNewDir, "%s\\%s\\%s", DSTDIR, saveName, accounts[target].c_str());
        if (!copyDir(dstNewDir, srcNewDir))
        {
            printf("[Error]: cannot load [%s] for account [%s]\n", saveName, accounts[target].c_str());
            goto loop;
        }

        printf("load succeed!\n");
        goto loop;

    // case 3: delete
    case 3:
        printf("you can delete the saves below:\n");
        showSaves(DSTDIR);
        scanf("%s", saveName);

        sprintf(dstNewDir, "%s\\%s", DSTDIR, saveName);
        if (!deleteDir(dstNewDir))
        {
            printf("[Error]: cannot delete [%s]\n", saveName);
            goto loop;
        }

        printf("delete succeed!\n");
        goto loop;

    // case 4: show guide
    case 4:
        showGuide();
        goto loop;

    // default: input invalid
    default:
        printf("[Warning]: please enter a number in [0,1,2,3,4]\n");
        goto loop;
    }

    return 0;
}