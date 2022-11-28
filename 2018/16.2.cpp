#include<stdio.h>
#include<string.h>
int main()
{
	char s1[27],s2[27],ch;
	int len1 = 0,len2 = 0,g = 0;
	scanf("%s%s",s1,s2);
//	printf("%s\n%s\n",s1,s2);
	len1 = strlen(s1);
	len2 = strlen(s2);
	if((len1==1)&&(len2==1)){
		if(strcmp(s1,s2)==0) printf("true");
		else printf("true");
	}
	else{
	
    //	printf("%d %d",len1,len2);
    	for(int i = 0; i < len1-1; i++){
	    	ch = s1[0];
	    //	printf("%c\n",ch);
	    	for(int j=0;j<len1-1;j++){
	    		s1[j] = s1[j+1];
	    	}
	    	s1[len1-1] = ch;
    	//	s1[len2] = '\0';
    	//	printf("%s %s\n",s1,s2);
	    	if(strstr(s1,s2)!=NULL) g++;
	    }
    	if(g!=0) printf("true");
	    else printf("false");
    }
	return 0;
}
