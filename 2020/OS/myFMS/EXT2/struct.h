#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <memory.h>
#include <time.h>
#include <malloc.h>
#include <io.h>
#include <queue>
#include <vector>
#include <stack>
using namespace std;

#define true  1
#define false 0

#define BLOCKNUM  1024 //块的数量
#define BLOCKSIZE 1024 //块的大小 B
#define INODENUM  80   //inode的数量
#define INODESIZE 64   //inode的大小 B
#define FILESIZE  8    //每个文件的最大块数

typedef struct
{
    short block_size;  //块的大小
    short block_num;   //块的数量
    short inode_num;   //inode的数量
    int free_block;    //空闲块的数量
    short free_inode;  //空闲inode的数量
    short root_dir;    //根目录的索引号
    char unused[1008]; //填充使结构体大小为1024B
}SuperBlock;

typedef struct //50B(文件名按10算）要求64B
{
    short node_num;    //inode索引号 2B
    char file_name[10]; //文件名 要求8B
    short is_dir;      //文件类型 0-文件 1-目录 2B
    short parent;      //上级目录 2B
    int length;      //文件-文件大小 目录-文件名大小 4B
    //存储文件的块的位置
    short block_num[FILESIZE] = {0};//2B
    char time[28];     //文件创建时间 28B
}INode;
//80个文件，每个文件一个inode，iNode表应该有4000B，4个block

typedef struct
{ //文件控制块
    short node_num;    //inode索引号
    char file_name[10]; //文件名
    short is_dir;      //文件类型
    char time[28];     //文件创建时间
}FCB, *PtrFCB;

typedef struct
{//活动文件
    short inode_num;//索引号
    char mode;//打开模式，有r,w
}Activefile;

void CreateSuperBlock(SuperBlock &super); //创建superblock
void InitSuperBlock(SuperBlock &super, char *content); //初始化superblock
void InitINode(INode &node, char *info); //初始化inode
void InitFileSystem(SuperBlock &super, char (&block_map)[BLOCKNUM+1], char (&inode_map)[BLOCKNUM+1],
                    INode (&inode_list)[INODENUM], char (&content)[BLOCKNUM][BLOCKSIZE+1]); //初始化文件系统

#endif // STRUCT_H
