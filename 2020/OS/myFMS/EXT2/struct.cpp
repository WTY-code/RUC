#include "struct.h"

//char content[BLOCKNUM][BLOCKSIZE+1]; //存储从文件系统存储文件中读取的信息

void CreateSuperBlock(SuperBlock &super)
{ //创建SuperBlock
    super.block_num = BLOCKNUM;
    super.inode_num = INODENUM;
    super.block_size = BLOCKSIZE;
    super.free_block = BLOCKSIZE;//应该是BLOCKNUM
    super.free_inode = INODENUM;
    super.root_dir = 0; //根目录默认为第0个inode
    memset(super.unused, '0', sizeof(super.unused));
}

short str2short(char a, char b)
{ //字符串转2位整数
    if (a == '-')
        return -1;
    return 10*(short(a-'0'))+(short(b-'0'));
}

short str2short3(char a, char b, char c)
{ //字符串转3位整数
    return 100*(short(a-'0'))+10*(short(b-'0'))+(short(c-'0'));
}

int str2int(char a, char b, char c, char d)
{ //字符串转数字
    return 1000*(int(a-'0'))+100*(int(b-'0'))+10*(int(c-'0'))+(int(d-'0'));
}

void InitSuperBlock(SuperBlock &super, char *content)
{ //初始化superblock
    super.block_size = BLOCKSIZE;
    super.block_num = BLOCKNUM;
    super.inode_num = INODENUM;
    super.free_block = str2int(content[6], content[7], content[8], content[9]);
    super.free_inode = str2short(content[10], content[11]);
    super.root_dir = str2short(content[12], content[13]);
    memset(super.unused, '0', sizeof(super.unused));
}

void InitINode(INode &node, char *info)
{ //初始化一个inode
    node.node_num = str2short(info[0], info[1]);
    memset(node.file_name, '\0', sizeof(node.file_name));
    for (int i=2; i<10; ++i)
    {
        node.file_name[i-2] = info[i];
    }
    node.is_dir = str2short(info[12], info[13]);
    node.parent = str2short(info[14], info[15]);
    for (int i=20; i<44; ++i)
    {
        node.block_num[i/2-9] = str2short3(info[i], info[i+1], info[i+2]);
    }
    memset(node.time, '\0', sizeof(node.time));
    for (int i=44; i<64; ++i)
    {
        node.time[i-44] = info[i];
    }
    //计算node所指文件的大小
    if (node.is_dir == 1)
        node.length = strlen(node.file_name);
    else
        node.length = str2int(info[16], info[17], info[18], info[19]);
}

void InitFileSystem(SuperBlock &super, char (&block_map)[BLOCKNUM+1], char (&inode_map)[BLOCKNUM+1],
                    INode (&inode_list)[INODENUM], char (&content)[BLOCKNUM][BLOCKSIZE+1])
{ //初始化文件系统
    char fs_file[] = {"file_system.txt"}; //存储文件系统内容的文件

    if (_access(fs_file, 0) == -1)
    { //文件系统不存在
        printf("File System does not exist, creating...\n");
        CreateSuperBlock(super);
        for (int i=0; i<BLOCKNUM; ++i)
        { //初始化content
            memset(content[i], '0', sizeof(content[i]));
        }
        FILE *f = fopen(fs_file, "w");
        for (int i=0; i<BLOCKNUM; ++i)
        { //初始化存储文件
            fwrite(content[i], sizeof(char), BLOCKSIZE, f);
        }
        fclose(f);
        memset(block_map, '0', BLOCKNUM);
        memset(inode_map, '0', BLOCKNUM);
        inode_map[0] = '1'; //根目录占用
        for (int i=0; i<8; ++i)
        { //前8块被占用
            block_map[i] = '1';
        }
        //初始化inode列表
        for (int i=0; i<INODENUM; ++i)
        {
            inode_list[i].parent = -1;
            memset(inode_list[i].block_num, 0, sizeof(inode_list[i].block_num));
        }
        //根目录初始化
        strcpy(inode_list[0].file_name, "root");
        strcpy(inode_list[0].time, "0000-00-00 00:00");
        printf("File System create finished.\n");
    }
    else
    { //文件系统存在 读入数据
        printf("File System exists, data is loading...\n");
        FILE *f = fopen(fs_file, "r");
        for (int i=0; i<BLOCKNUM; ++i)
        {
            fread(content[i], sizeof(char), BLOCKSIZE, f);
        }
        fclose(f);
        //读入SuperBlock
        InitSuperBlock(super, content[0]);
        //读入block位图
        strcpy(block_map, content[1]);
        for (int i=0; i<8; ++i)
        { //前8块被占用
            block_map[i] = '1';
        }
        //读入inode位图
        strcpy(inode_map, content[2]);
        inode_map[0] = '1'; //根目录占用
        //读入inode
        int ind = 0;
        char info[65]; //64
        for (int i=3; i<8; ++i)
        { //3-7块为inode块
            for (int j=0; j<16; ++j)
            { //每个块16个inode
                for (int k=j*64, p=0; k<(j+1)*64; ++k, ++p)
                {
                    info[p] = content[i][k];
                }
                InitINode(inode_list[ind], info);
                ++ind;
            }
        }
        //修改inode的parent值
        for (int i=0; i<INODENUM; ++i)
        {
            if (inode_map[i] == '0')
                inode_list[i].parent = -1;
        }
        //根目录初始化
        strcpy(inode_list[0].file_name, "root");
        strcpy(inode_list[0].time, "0000-00-00 00:00");
        printf("data loading finished.\n");
    }
}
