#ifndef FUNCTION_H
#define FUNCTION_H

#include <struct.h>

#define BUFFERSIZE 1024 //缓冲区大小 B 按要求修改为1KB

void Init(); //初始化

void my_help(); //显示命令目录
void my_read(short fd); //读文件中内容
void my_write(short fd); //写内容到文件
short my_open(char* pathOname, char mode); //打开文件
void my_close(short fd);//关闭文件
void my_ls(); //显示当前目录的文件信息
void my_cd(); //进入某一目录
void my_cdback(); //返回上级目录
//void my_rmdir(); //删除目录
void my_create(short isDir); //新建一个文件或目录
void my_create2(char name[10]);//创建一个给定文件名的文件
void my_remove(short isDir); //删除一个文件
void my_quit(); //关闭文件系统
void my_import(); //从外部导入文件
void my_export(); //将文件导出到外部
void show_super(); //显示superblock中的内容
void show_curpath(); //显示当前目录信息
void GETBLK(char* buf, short blk_no);//将指定block中的内容读到缓冲区中
void PUTBLK(char* buf, short blk_no);//将缓冲去的内容写到指定block中
bool isPath(char* pathOname);//输入字符串判断是不是路径
short findCurPath(char* path);//根据输入的路径返回当前文件的（路径）的inode索引值
void addActvFile(short fd, char mode);//将新打开的文件加入活动文件表
void delActvFile(short fd);//文件关闭时删除活动文件表
void showActvFile();//打印活动文件表
void getFilenameFromPath(char path[50], char name[10]);//从输入的绝对路径中解析出文件名
void showINode();//显示所有已占用inode的情况

#endif // FUNCTION_H
