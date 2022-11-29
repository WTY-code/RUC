#include "function.h"

short cur_path = 0; //当前所在目录的索引
char buffer[BUFFERSIZE+1]; //临时缓冲区
char content[BLOCKNUM][BLOCKSIZE+1]; //存储从文件系统存储文件中读取的信息
char BlockBitMap[BLOCKNUM+1]; //块的位图
char INodeBitMap[BLOCKNUM+1]; //inode的位图
INode inode_list[INODENUM]; //inode数组
vector<Activefile> actv_list;//活动文件表（记录哪些文件被打开了，什么模式）
char fs_file[] = {"file_system.txt"}; //存储文件系统内容的文件
SuperBlock super;

using namespace std;

//初始化文件系统
void Init()
{
    InitFileSystem(super, BlockBitMap, INodeBitMap, inode_list, content);
}

//显示superblock的情况：空闲的inode数和block数
void show_super()
{
    printf("free inode: %hd\n", super.free_inode);
    printf("free block: %d\n\n", super.free_block);
}

//显示当前路径，如root/d0/d1/filename
void show_curpath()
{
    short path = cur_path;
    stack<char*> S;
    char filename[10][10];//预设至多有10层文件
    int i=0;
    while(1){//通过当前inode的parent往上一直找到根目录，存到栈中
        strcpy(filename[i], inode_list[path].file_name);
        S.push(filename[i]);
        i++;
        if(inode_list[path].node_num==0) break;
        path = inode_list[path].parent;
    }
    while(1){//将栈中的文件名弹出，将从最深层目录到root调转为root到当前目录
        printf("%s",S.top());
        //printf("/");
        S.pop();
        if(S.empty()){
            printf("\n");
            return;
        }
        else{
            printf("/");
        }
    }
}

//显示操作列表
void my_help()
{
    printf("ls:     show the information of files in current director..\n");
    printf("read:   read the content of a existed file.\n");
    printf("write:  write content to an existed file.\n");
    printf("open:   open an existed file for operating.\n");
    printf("close:  close an existed file for operating.\n");
    printf("cd:     enter an existed director.\n");
    printf("cd..:   back to parent director.\n");
    printf("mkdir:  create a new director in current director.\n");
    printf("rmdir:  delete an existed director in current director.\n");
    printf("create: create a new file.\n");
    printf("remove: delete an existed file.\n");
    printf("import: copy content outside to a file inside.\n");
    printf("export: copy content inside to a file outside.\n");
    printf("super:  show some system information.\n");
    printf("crtpth: show the information of current director.\n");    
    printf("actfls: show active file list\n");
    printf("inode:  show info of inode\n");
    printf("quit:   close File System.\n\n");
}

//2位整数转字符串
void short2str(short num, char &a, char &b)
{
    if (num < 0)
    {
        a = '-';
        b = '1';
    }
    b = num % 10 + '0';
    num /= 10;
    a = num + '0';
}

//3位整数转字符串
void short2str3(short num, char &a, char &b, char &c)
{
    c = num % 10 + '0';
    num /= 10;
    b = num % 10 + '0';
    num /= 10;
    a = num + '0';
}

//4位整数转字符串
void int2str(int num, char &a, char &b, char &c, char &d)
{
    d = num % 10 + '0';
    num /= 10;
    c = num % 10 + '0';
    num /= 10;
    b = num % 10 + '0';
    num /= 10;
    a = num + '0';
}

//关闭文件系统 并将数据进行保存
void my_quit()
{
    //保存superblock
    int2str(super.free_block, content[0][6], content[0][7], content[0][8], content[0][9]);
    short2str(super.free_inode, content[0][10], content[0][11]);
    short2str(super.root_dir, content[0][12], content[0][13]);
    //保存block位图
    strcpy(content[1], BlockBitMap);
    //保存inode位图
    strcpy(content[2], INodeBitMap);
    //保存inode数组
    int ind = 0;
    for (int i=3; i<8; ++i)//5个存inode_list的块
    {
        for (int j=0; j<16; ++j)//每个块16个inode
        {
            int k = j * 64;
            short2str(inode_list[ind].node_num, content[i][k], content[i][k+1]);
            for (int p=k+2, q=0; p<k+10; ++p, ++q)
            {
                content[i][p] = inode_list[ind].file_name[q];
            }
            content[i][k+10] = '0'; content[i][k+11] = '0';
            short2str(inode_list[ind].is_dir, content[i][k+12], content[i][k+13]);
            short2str(inode_list[ind].parent, content[i][k+14], content[i][k+15]);
            int2str(inode_list[ind].length, content[i][k+16], content[i][k+17], content[i][k+18], content[i][k+19]);
            for (int p=0; p<FILESIZE; ++p)
            {
                short2str3(inode_list[ind].block_num[p], content[i][k+20+p*3], content[i][k+21+p*3], content[i][k+22+p*3]);
            }
            for (int p=0; p<20; ++p)
            {
                content[i][k+44+p] = inode_list[ind].time[p];
            }
            ind++;
        }
    }
    //保存所有信息
    FILE *f = fopen(fs_file, "w");
    if (f == NULL)
    {
        printf("file open failed when close system.\n");
        return;
    }
    for (int i=0; i<BLOCKNUM; ++i)
    {
        fwrite(content[i], sizeof(char), BLOCKSIZE, f);
    }
    fclose(f);
    printf("save information successfully.\n\n");
}

//操作“磁盘”函数，将第blk_no块磁盘的内容读到缓冲区中
void GETBLK(char* buf, short blk_no)
{
    strcpy(buf,content[blk_no]);
}

//操作“磁盘”函数，将缓冲区中的内容读到第blk_no块磁盘
void PUTBLK(char* buf, short blk_no)
{
    strcpy(content[blk_no],buf);
}

//根据是否有‘/’判断输入的字符串是路径还是文件名
bool isPath(char* pathOname)
{
    for(int i=0; i<strlen(pathOname); i++){
        if(pathOname[i]=='/') return true;
    }
    return false;
}

//根据输入的路径（字符串）找到iNode索引值
short findCurPath(char* path){
    short tmp_curpath = 0;
    queue<char*> Q;
    char*p;
    p=strtok(path,"/");
    while(p){
        Q.push(p);
        p=strtok(NULL,"/");
    }//将以‘/’间隔的路径分隔出来
    char* s;
    while (!Q.empty()) {
        s = Q.front();
        printf("%s\n",s);
        //检测当前目录下该文件是否存在
        for (int i=0; i<INODENUM; ++i)
        {
            if (inode_list[i].parent == tmp_curpath)
            {
                if (strcmp(inode_list[i].file_name, s) == 0)
                {
                    tmp_curpath = i;//当前目录设为队列中弹出的目录
                    break;
                }
            }
        }
        Q.pop();
    }
    return tmp_curpath;
}

//将文件描述符(inode索引值)为fd的文件按模式mode('r'或'w')加入活动文件表中
void addActvFile(short fd, char mode)
{
    Activefile tmp;
    tmp.inode_num = fd;
    tmp.mode = mode;
    actv_list.push_back(tmp);
    return;
}

//从活动文件表中删除描述符为fd的文件
void delActvFile(short fd)
{
    vector<Activefile>::iterator it;
    for(it=actv_list.begin();it!=actv_list.end();){
        if(it->inode_num == fd){
            it = actv_list.erase(it);
        }
        else it++;
    }
}

//显示当前活动文件表
void showActvFile()
{
    if(actv_list.size()==0){
        printf("active file list is empty\n");
        return;
    }

    for(int i=0; i<actv_list.size(); i++){
        printf("inode index(fd): %d ",actv_list.at(i).inode_num);
        printf("name: %s ",inode_list[actv_list.at(i).inode_num].file_name);
        printf("open mode: %c\n",actv_list.at(i).mode);
    }
    return;
}

//按模式打开指定文件，输入可以是文件名，也可以是绝对路径（要求路径终点必须是文件，不能是文件夹）
short my_open(char* pathOname, char mode)
{
    //如果输入文件名就在当前文件夹下查找，如果是绝对路径按路径寻找
    short fd;//文件描述符，实际是文件的inode索引值
    if(isPath(pathOname)){//如果输入的是绝对路径
        //printf("you input a path\n");//debug
        fd = findCurPath(pathOname);//找到输入路径所指文件的inode索引值作为文件描述符
        if(fd == 0){//在findCurPath函数中fd的初始值为0，如果没变说明没有找到对应文件
            printf("ERROR: can not find file!\n");
            return short(-1);
        }
        addActvFile(fd,mode);//加入活动文件表
        //printf("DEBUG----show actv_list:\n");//debug
        //showActvFile();//debug
        printf("open file %s successfully\n",pathOname);
        printf("fd is %hd\n",fd);
        return fd;
        //printf("if you want to read or write something, please create a file first\n");
    }
    else{//直接输入文件名
        //检测当前目录下该文件是否存在
        for (int i=0; i<INODENUM; ++i)
        {
            if (inode_list[i].parent == cur_path && inode_list[i].is_dir == 0)
            {
                if (strcmp(inode_list[i].file_name, pathOname) == 0)
                {
                    addActvFile(inode_list[i].node_num,mode);
                    //printf("DEBUG----show actv_list:\n");//debug
                    //showActvFile();//debug
                    printf("open file %s successfully\n",pathOname);
                    printf("fd is %hd\n",inode_list[i].node_num);
                    return inode_list[i].node_num;
                }
            }
        }
        printf("file %s doesn't existed in current directory, please input again.\n");
        return short(-1);
    }
}

//关闭文件
void my_close(short fd)
{
    if(actv_list.capacity()==0){
        printf("active file list is empty\n");
        return;
    }
    delActvFile(fd);//将文件从活动文件表中删除
}

//将磁盘（content）中文件内容读入内存（buffer）
void my_read(short fd)
{
    for(int index=0; index<actv_list.capacity(); index++){
        if(actv_list.at(index).inode_num == fd && actv_list.at(index).mode != 'N'){//寻找要读的文件
            for(int i=0; inode_list[fd].block_num[i] != 0; i++){
                GETBLK(buffer,inode_list[fd].block_num[i]);
                printf("buffer%d: %s\n",i,buffer);
            }
            return;
        }
    }
    printf("don't have authority to read or fd error\n");
    return;
}

//文件写函数，参数fd为文件描述符（实际为文件inode索引值），输入以‘#’作为结束标志
void my_write(short fd)
{
    bool flag = false;//是否找到文件
    for(int index=0; index<actv_list.capacity(); index++){
        if(actv_list.at(index).inode_num == fd && actv_list.at(index).mode == 'w'){
            flag = true;
            //清理原来的文件内容
            int i=0;
            while(inode_list[fd].block_num[i]!=0){
                BlockBitMap[inode_list[fd].block_num[i]] = '0';
                i++;
            }
            //如果没有空闲块不能写
            if(super.free_block == 0){
                printf("block full, can't write anything\n");
                return;
            }
            printf("input less than 1024 bytes one time, input '#' to finish\n");
            int blknum = 0;//该文件占用block数
            int idx = 0;//该文件inode.block_num数组下标
            while(blknum<8){//最大块数8块
                for (int i=0; i<BLOCKNUM; ++i)
                {
                    if (BlockBitMap[i] == '0')
                    {
                        inode_list[fd].block_num[idx] = short(i);
                        BlockBitMap[i] = '1';
                        //blkidx++;
                        //inode_list[fd].length++;
                        //printf("find a free block %d\n",i);//debug
                        break;
                    }
                }
                getchar();
                int i=0;
                char tmp;
                tmp = getchar();
                while(1){
                    buffer[i] = tmp;
                    tmp = getchar();
                    i++;
                    if(i==1024){//如果0-1023有字符，buffer已满
                        buffer[i]='\0';
                        PUTBLK(buffer,inode_list[fd].block_num[idx]);
                        inode_list[fd].length += 1024;
                        idx++;
                        blknum++;//先占block后++
                        if(blknum==8){//当占满8块时，达到最大文件长度
                            printf("this file takes up 8 blocks, can't write anymore\n");
                            return;
                        }
                        break;
                    }
                    if(tmp=='#'){//如果接收到结束字符'#'
                        buffer[i]='\0';
                        PUTBLK(buffer,inode_list[fd].block_num[idx]);
                        inode_list[fd].length += strlen(buffer);
                        idx++;
                        blknum++;
                        return;
                    }
                }
            }

            //return;
        }
    }
    if(!flag)
        printf("file %s isn't in active file list or doesn't exist.\n", inode_list[fd].file_name);
}

//当前目录下的文件、目录列表
void my_ls()
{
    for (int i=1; i<INODENUM; ++i)
    { //寻找当前目录下的inode
        if (inode_list[i].parent == cur_path && INodeBitMap[i] == '1')
        {
            printf("index:       %hd\n", inode_list[i].node_num);
            printf("name:        %s\n", inode_list[i].file_name);
            printf("type:        %s\n", (inode_list[i].is_dir == 0) ? "file" : "director");
            printf("belong to:   %s\n", inode_list[inode_list[i].parent].file_name);
            printf("size:        %d\n", inode_list[i].length);
            printf("create time: %s\n\n", inode_list[i].time);
        }
    }
    printf("all information.\n\n");
}

//进入下级目录，需输入要进入目录的inode索引值
void my_cd()
{
    printf("index of current director: index %hd\n", cur_path);
    short ind;
    printf("input the index of director >>> ");
    scanf("%hd", &ind);
    if (INodeBitMap[ind] == '1' && inode_list[ind].parent == cur_path)
    {
        printf("director change successfully.\n");
        cur_path = ind;
        printf("current director: index %hd\n\n", cur_path);
    }
    else
    {
        printf("wrong index.\n\n");
    }
}

//返回上级目录
void my_cdback()
{
    printf("index of current director is %hd\n", cur_path);
    if (cur_path == 0)
    {
        printf("current director is root director.\n\n");
        return;
    }
    cur_path = inode_list[cur_path].parent;
    printf("index of current director is %hd\n\n", cur_path);
}

//获取当前时间
void get_time(char (&now)[28])
{
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    int2str(ptminfo->tm_year+1900, now[0], now[1], now[2], now[3]);
    now[4] = '-';
    short2str(short(ptminfo->tm_mon+1), now[5], now[6]);
    now[7] = '-';
    short2str(short(ptminfo->tm_mday), now[8], now[9]);
    now[10] = ' ';
    short2str(short(ptminfo->tm_hour), now[11], now[12]);
    now[13] = ':';
    short2str(short(ptminfo->tm_min), now[14], now[15]);
}

//新建一个文件或目录，参数isDir=0表示文件，1表示目录
void my_create(short isDir)
{
    char name[10];//目录名
    printf("director/file name should not be over 8 Bytes.\n");
    printf("input director/file name >>> ");
    scanf("%s", name);
    if (strlen(name) > 8)
    {
        printf("the name is too long.\n\n");
        return;
    }
    //检测当前目录下该文件是否存在
    for (int i=0; i<INODENUM; ++i)
    {
        if (inode_list[i].parent == cur_path && isDir == inode_list[i].is_dir)
        {
            if (strcmp(inode_list[i].file_name, name) == 0)
            {
                printf("the file name has existed.\n\n");
                return;
            }
        }
    }
    //寻找当前空余的inode并建立目录
    int ind = -1;
    for (int i=1; i<INODENUM; ++i)
    {
        if (INodeBitMap[i] == '0')
        {
            INodeBitMap[i] = '1';
            ind = i;
            break;
        }
    }
    if (ind == -1)
    { //没找到空余inode
        printf("there is no free memory to create.\n\n");
        return;
    }
    inode_list[ind].node_num = ind;
    strcpy(inode_list[ind].file_name, name);
    inode_list[ind].is_dir = isDir;
    inode_list[ind].parent = cur_path;
    if (isDir == 0)
        inode_list[ind].length = 0;
    else
        inode_list[ind].length = strlen(name);
    memset(inode_list[ind].time, '\0', sizeof(inode_list[ind].time));
    get_time(inode_list[ind].time);
    //修改superblock
    super.free_block -= 1;
    super.free_inode -= 1;
    printf("create successfully.\n\n");
}

//my_import()的辅助函数，与my_create类似：新建一个给定名字的文件
void my_create2(char name[10])
{
    //检测当前目录下该文件是否存在
    for (int i=0; i<INODENUM; ++i)
    {
        if (inode_list[i].parent == cur_path && inode_list[i].is_dir == 0)
        {
            if (strcmp(inode_list[i].file_name, name) == 0)
            {
                printf("the file name has existed.\n\n");
                return;
            }
        }
    }
    //寻找当前空余的inode并建立目录
    int ind = -1;
    for (int i=1; i<INODENUM; ++i)
    {
        if (INodeBitMap[i] == '0')
        {
            INodeBitMap[i] = '1';
            ind = i;
            break;
        }
    }
    if (ind == -1)
    { //没找到空余inode
        printf("there is no free memory to create.\n\n");
        return;
    }
    inode_list[ind].node_num = ind;
    strcpy(inode_list[ind].file_name, name);
    inode_list[ind].is_dir = 0;
    inode_list[ind].parent = cur_path;
    inode_list[ind].length = 0;
    memset(inode_list[ind].time, '\0', sizeof(inode_list[ind].time));
    get_time(inode_list[ind].time);
    //寻找空余的数据块
    /*for (int i=0; i<BLOCKNUM; ++i)
    {
        if (BlockBitMap[i] == '0')
        {
            inode_list[ind].block_num[0] = short(i);
            break;
        }
    }*/
    //修改superblock
    super.free_block -= 1;
    super.free_inode -= 1;
    printf("create successfully.\n\n");
}

//删除一个文件或目录
void my_remove(short isDir)
{
    short ind = -1;
    char name[10];
    printf("input the name of director/file >>> ");
    scanf("%s", name);
    //寻找该命名的文件
    for (int i=0; i<INODENUM; ++i)
    {
        if (inode_list[i].parent == cur_path && isDir == inode_list[i].is_dir)
        {
            if (strcmp(inode_list[i].file_name, name) == 0)
            {
                ind = short(i);
                break;
            }
        }
    }
    if (ind == -1)
    {
        printf("the director/file is not existed in current director.\n\n");
        return;
    }
    //检测文件类型
    if (isDir != inode_list[ind].is_dir)
    {
        printf("this file is not %s.\n\n", (isDir == 0) ? "file" : "director");
        return;
    }
    //检测是否在当前目录下
    if (inode_list[ind].parent != cur_path)
    {
        printf("the director/file is not in current director.\n\n");
        return;
    }
    //删除内容
    int k = 0; //记录有inode多少个数据块未分配
    for (int i=0; i<8; ++i)
    {
        if (inode_list[ind].block_num[i] != 0)
        {
            ++k;
            BlockBitMap[inode_list[ind].block_num[i]] = '0';
            inode_list[ind].block_num[i] = 0;
        }
    }
    INodeBitMap[ind] = '0';
    //修改superblock
    super.free_block += k;
    super.free_inode += 1;
    printf("remove successfully.\n\n");
}

//将外部文件导入当前目录下(过程中要再创建用于保存导入内容的文件)，需输入外部文件的路径和创建的文件名
void my_import()
{
    if(super.free_block == 0){
        printf("block full, can't write anything\n");
        return;
    }
    printf("input path of source file you want to import\n");
    char srcpath[100];//导入文件绝对路径
    scanf("%s",srcpath);
    char srcname[10];
    //getFilenameFromPath(srcpath,srcname);
    printf("create a file to save content imported, input file name\n");
    scanf("%s",srcname);
    my_create2(srcname);//在当前目录下创建一个与导入文件名字相同的文件
    short fd = my_open(srcname,'w');//打开当前目录下创建的与导入文件同名的文件
    FILE *f = fopen(srcpath, "r");
    if (f == NULL)
    {
        printf("file open failed.\n\n");
        return;
    }
    //按字符从文件读入缓冲区，每1024个字符写一个block并把缓冲区清零
    int blknum = 0;//该文件占用block数
    int idx = 0;//该文件inode.block_num数组下标
    while(blknum<8){//最大块数8块
        for (int i=0; i<BLOCKNUM; ++i)
        {
            if (BlockBitMap[i] == '0')
            {
                inode_list[fd].block_num[idx] = short(i);
                BlockBitMap[i] = '1';
                inode_list[fd].length++;
                //printf("find a free block %d\n",i);//debug
                break;
            }
        }
        int i=0;
        char tmp;
        tmp = fgetc(f);
        while(1){
            buffer[i] = tmp;
            tmp = fgetc(f);
            i++;
            if(i==1024){//如果0-1023有字符，buffer已满
                buffer[i]='\0';
                PUTBLK(buffer,inode_list[fd].block_num[idx]);
                idx++;
                blknum++;//先占block后++
                if(blknum==8){//当占满8块时，达到最大文件长度
                    printf("this file takes up 8 blocks, can't write anymore\n");
                    fclose(f);
                    return;
                }
                break;
            }
            if(tmp==EOF){//如果接收到文件结束符
                buffer[i]='\0';
                PUTBLK(buffer,inode_list[fd].block_num[idx]);
                idx++;
                blknum++;
                fclose(f);
                return;
            }
        }
    }
}

//将内容导出到外部文件
void my_export()
{
    short ind = -1;
    char file[10];
    printf("input the name of file you want to export >>> ");
    scanf("%s", file);
    //检查该名称的文件是否在当前目录
    for (int i=0; i<INODENUM; ++i)
    {
        if (inode_list[i].parent == cur_path)
        {
            if (strcmp(inode_list[i].file_name, file) == 0)
            {
                ind = short(i);
                break;
            }
        }
    }
    if (ind == -1)
    {
        printf("the director/file is not existed in current director.\n\n");
        return;
    }
    //检查文件类型
    if (inode_list[ind].is_dir == 1)
    {
        printf("this is a director.\n\n");
        return;
    }
    //输入导出的文件名
    char name[15];
    printf("input the file name you want to save the content\n");
    printf("the size should not be over 10 bytes >>> ");
    scanf("%s", name);
    if (strlen(name) > 10)
    {
        printf("file name is too long.\n\n");
        return;
    }
    printf("the file type is: txt\n");
    strcat(name, ".txt");
    //导出
    int p = 0, q = 0;
    for (int i=FILESIZE-1; i>=0; i--)
    { //寻找文件的最后一个数据块
        if (inode_list[ind].block_num[i] != 0)
        {
            p = i;
            break;
        }
    }
    short k = inode_list[ind].block_num[p];
    for (int i=BLOCKSIZE-1; i>=0; i--)
    { //寻找最后一个数据块的最后一个字符的位置
        if (content[k][i] != '0')
        {
            q = i + 1;
            break;
        }
    }
    FILE *f = fopen(name, "w");
    if (f == NULL)
    {
        printf("file open failed.\n\n");
        return;
    }
    for (int i=0; i<p; ++i)
    { //写入数据
        fwrite(content[inode_list[ind].block_num[i]], sizeof(char), BLOCKSIZE, f);
    }
    fwrite(content[k], sizeof(char), q, f);
    fclose(f);
    printf("file export successfully.\n\n");
}

//显示已占用inode情况1
void showINode()
{
    for(int i=0; i<INODENUM; i++){
        if(INodeBitMap[i] != '0'){
            printf("inode index: %d type: %hd name: %s parent: %hd length: %d time: %s\n",
                   inode_list[i].node_num,inode_list[i].is_dir, inode_list[i].file_name,
                   inode_list[i].parent,inode_list[i].length,inode_list[i].time);
        }
    }
    printf("\n");
}
