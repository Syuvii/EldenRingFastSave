#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <string>

#define PATHLEN 256
#define BUFFSIZE 4096

bool copyFile(const char* src, const char* dst);
bool copyDir(const char* src, const char* dst);
bool deleteFile(const char* src);
bool deleteDir(const char* dir);
bool getSubFiles(const char* dir, std::vector<std::string>& subfiles);
bool getSubDirs(const char* dir, std::vector<std::string>& subdirs);


/* copy file from src to dst
    src: path/fileName
    dst: path/fileName
*/ 
bool copyFile(const char* src, const char* dst)
{
    FILE* inFile = fopen(src, "rb");
    FILE* outFile = fopen(dst, "wb");

    if (inFile == NULL || outFile == NULL)
        return 0;
    else
    {
        char* buff = (char*)malloc(sizeof(char)*BUFFSIZE);
        int cnt = 0;
        while(cnt = fread(buff, sizeof(char), BUFFSIZE, inFile))
        {
            fwrite(buff, sizeof(char), cnt, outFile);
        }
    }

    fclose(inFile);
    fclose(outFile);

    return 1;
}

/* copy direct from src to dst
    src: path/dir
    dst: path/dir
*/
bool copyDir(const char* src, const char* dst)
{
    DIR* pSrcDir;
    DIR* pDstDir;
    struct dirent* pSrcDirent;

    pSrcDir = opendir(src);
    if (pSrcDir == NULL)
    {
        return 0;
    }

    deleteDir(dst);
    mkdir(dst);

    pDstDir = opendir(dst);
    if (pDstDir == NULL)
    {
        return 0;
    }

    char* srcChild = (char*)malloc(sizeof(char)*PATHLEN);
    char* dstChild = (char*)malloc(sizeof(char)*PATHLEN);
    struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
    while((pSrcDirent = readdir(pSrcDir)) != NULL)
    {
        if (strcmp(pSrcDirent->d_name, ".")==0) continue;
        if (strcmp(pSrcDirent->d_name, "..")==0) continue;
        strcpy(srcChild, src);
        strcpy(dstChild, dst);
        strcat(srcChild, "\\");
        strcat(dstChild, "\\");
        strcat(srcChild, pSrcDirent->d_name);
        strcat(dstChild, pSrcDirent->d_name);
        stat(srcChild, buff);
        if (S_ISDIR(buff->st_mode))
        {
            if (!copyDir(srcChild, dstChild))
            {
                free(srcChild);
                free(dstChild);
                free(buff);
                closedir(pSrcDir);
                closedir(pDstDir);
                return 0;
            }
        }
        else if (S_ISREG(buff->st_mode))
        {
            if (!copyFile(srcChild, dstChild))
            {
                free(srcChild);
                free(dstChild);
                free(buff);
                closedir(pSrcDir);
                closedir(pDstDir);
                return 0;
            }
        }
    }
    free(srcChild);
    free(dstChild);
    free(buff);
    closedir(pSrcDir);
    closedir(pDstDir);
    return 1;
}

/* delete file src
    src: path/fileName
*/
bool deleteFile(const char* src)
{
    if (remove(src) == 0)
        return 1;
    else
        return 0;
}

/* delete direct dir
    dir: path/dir
*/
bool deleteDir(const char* dir)
{
    DIR* pDir;
    struct dirent* pDirent;

    pDir = opendir(dir);
    if (pDir == NULL)
        return 0;

    char* child = (char*)malloc(sizeof(char)*PATHLEN);
    struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
    while((pDirent = readdir(pDir)) != NULL)
    {
        if (strcmp(pDirent->d_name, ".")==0) continue;
        if (strcmp(pDirent->d_name, "..")==0) continue;
        strcpy(child, dir);
        strcat(child, "\\");
        strcat(child, pDirent->d_name);
        stat(child, buff);
        if (S_ISDIR(buff->st_mode))
        {
            if (!deleteDir(child))
            {
                free(child);
                free(buff);
                closedir(pDir);
                return 0;
            }
        }
        else if (S_ISREG(buff->st_mode))
        {
            if (!deleteFile(child))
            {
                free(child);
                free(buff);
                closedir(pDir);
                return 0;
            }
        }
    }
    rmdir(dir);
    free(child);
    free(buff);
    closedir(pDir);

    return 1;
}

/* get subdirs in dst
    dir: path/dir
    subfiles: vector<string>
*/
bool getSubFiles(const char* dir, std::vector<std::string>& subfiles)
{
    DIR* pDir;
    struct dirent* pDirent;

    pDir = opendir(dir);
    if (pDir == NULL)
        return 0;

    char* child = (char*)malloc(sizeof(char)*PATHLEN);
    struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
    while((pDirent = readdir(pDir)) != NULL)
    {
        if (strcmp(pDirent->d_name, ".")==0) continue;
        if (strcmp(pDirent->d_name, "..")==0) continue;
        strcpy(child, dir);
        strcat(child, "\\");
        strcat(child, pDirent->d_name);
        stat(child, buff);
        if (S_ISREG(buff->st_mode))
        {
            subfiles.emplace_back(pDirent->d_name);
        }
    }

    free(child);
    free(buff);
    closedir(pDir);

    return 1;
}

/* get subdirs in dst
    dir: path/dir
    subdirs: vector<string>
*/
bool getSubDirs(const char* dir, std::vector<std::string>& subdirs)
{
    DIR* pDir;
    struct dirent* pDirent;

    pDir = opendir(dir);
    if (pDir == NULL)
        return 0;

    char* child = (char*)malloc(sizeof(char)*PATHLEN);
    struct stat* buff = (struct stat*)malloc(sizeof(struct stat));
    while((pDirent = readdir(pDir)) != NULL)
    {
        if (strcmp(pDirent->d_name, ".")==0) continue;
        if (strcmp(pDirent->d_name, "..")==0) continue;
        strcpy(child, dir);
        strcat(child, "\\");
        strcat(child, pDirent->d_name);
        stat(child, buff);
        if (S_ISDIR(buff->st_mode))
        {
            subdirs.emplace_back(pDirent->d_name);
        }
    }

    free(child);
    free(buff);
    closedir(pDir);

    return 1;
}

#endif