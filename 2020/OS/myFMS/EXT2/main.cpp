#include <function.h>
//final
int main()
{
    FILE *fp=fopen("D:\\1mycode\\impt.txt","w");
    if(fp==NULL){
        return 0;
    }
    for(int i=0;i<100;i++){
        for(int j=0; j<50; j++){
            fprintf(fp,"%d ",j);
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    printf("File System start.\n");
    Init();

    char operation[10];
    while (true)
    {
        printf("user@localhost >>> ");
        scanf("%s", operation);
        if (strcmp(operation, "help") == 0)
            my_help();
        else if (strcmp(operation, "read") == 0){
            printf("according active file list, input fd\n");
            getchar();
            printf("show active file list? input y/n\n");
            char ans;
            scanf("%c",&ans);
            if(ans=='y'){
                showActvFile();
            }
            short fd;
            getchar();
            printf("input fd\n");
            scanf("%hd",&fd);
            my_read(fd);
        }
        else if (strcmp(operation, "write") == 0){
            printf("according active file list, input fd\n");
            getchar();
            printf("show active file list? input y/n\n");
            char ans;
            scanf("%c",&ans);
            if(ans == 'y'){
                showActvFile();
            }
            short fd;
            getchar();
            printf("input fd\n");            
            scanf("%hd",&fd);
            my_write(fd);
        }
        else if (strcmp(operation, "open") == 0){
            printf("input filename or path\n");
            char pathOname[50];
            scanf("%s",pathOname);
            getchar();
            printf("what mode? input 'r' or 'w'\n");
            char mode;
            scanf("%c",&mode);
            my_open(pathOname,mode);
        }
        else if (strcmp(operation, "close") == 0){
            printf("according active file list, input fd\n");
            printf("show active file list? input y/n\n");
            getchar();
            char ans;
            scanf("%c",&ans);
            if(ans=='y'){
                showActvFile();
            }
            printf("input fd\n");
            short fd;
            scanf("%hd",&fd);
            my_close(fd);
        }
        else if (strcmp(operation, "ls") == 0)
            my_ls();
        else if (strcmp(operation, "cd") == 0)
            my_cd();
        else if (strcmp(operation, "cd..") == 0)
            my_cdback();
        else if (strcmp(operation, "mkdir") == 0)
            my_create(1);
        else if (strcmp(operation, "rmdir") == 0)
            my_remove(1);
        else if (strcmp(operation, "create") == 0)
            my_create(0);
        else if (strcmp(operation, "remove") == 0)
            my_remove(0);
        else if (strcmp(operation, "export") == 0)
            my_export();
        else if (strcmp(operation, "import") == 0)
            my_import();
        else if (strcmp(operation, "quit") == 0)
        {
            my_quit();
            break;
        }
        else if (strcmp(operation, "super") == 0)
            show_super();
        else if (strcmp(operation, "crtpth") == 0)
            show_curpath();
        else if (strcmp(operation, "actfls") == 0)
            showActvFile();
        else if (strcmp(operation, "inode") == 0)
            showINode();
        else
            printf("Wrong operation!\n\n");
    }
    printf("File System closed.\n");

    return 0;
}
