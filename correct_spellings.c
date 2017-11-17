#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<stdbool.h>
#include "word_store.h"
#include "my_client.h"
#define MAX 1000000
#define WORDLEN 1000
#define number 5
#define model 4 //1-unigram, 2-bigram, 3-trigram
#define type 0 //0-real, 1-non-real

#define unknown -6.002618
void change() // Function to change the large caps to small caps in the input phoneme file
{
    FILE *fin=fopen("Phoneme.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open file Phoneme.txt\n");
    }
    FILE *fout=fopen("PhonemeMod.txt","w");
    char c;
    while( ( c = fgetc(fin) ) != EOF )
    {
        if(c>=65 && c<=90)
        {
            c+=32;
            fprintf(fout,"%c",c);
        }
        else
        {
            fprintf(fout,"%c",c);
        }
        printf("%c",c);
    }
    fclose(fin);
    fclose(fout);
}
void store_phoneme() // Function to remove spaces in the input file
{
    FILE *fin=fopen("PhonemeMod.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open PhonemeMod.txt\n ");
        return;
    }
    FILE *fout=fopen("Merge Phoneme.txt","w");
    char word[200];
    char phoneme[300];
    char sentence[500];
    while(!feof(fin))
    {
        fscanf(fin,"%[^\n]\n",sentence);
        int len=strlen(sentence);
        int i=0;
        int word_indx=0;
        int ph_indx=0;
        while(sentence[i]!=' ')
        {
            word[word_indx++]=sentence[i];
            i++;
        }
        word[word_indx]='\0';
        i+=2;
        while(i<len)
        {
            if(sentence[i]!=' ')
            {
                phoneme[ph_indx]=sentence[i];
                ph_indx++;
            }
            i++;
        }
        phoneme[ph_indx]='\0';
        printf("%s\n",phoneme);
        fprintf(fout,"%s %s\n",word,phoneme);
    }
    fclose(fin);
    fclose(fout);
}
int min(int a, int b, int c) //To find the minimum of 3 numbers
{
    int temp;
    if(a<b)
    {
        temp=a;
    }
    else
        temp=b;
    if(c<temp)
        temp=c;
    return(temp);
}
char **searchPh(char *search_word, char store[][100], int *indx) // Search words with same pronounciation which are not the same as the search word
{
    //printf("Searching for same phomene = %s\n",search_word);
    FILE *fin=fopen("Merge Phoneme.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open Merge Phoneme.txt\n ");
        return;
    }
    char word[200];
    char phoneme[300];
    char sentence[500];
    //Getting the phoneme of the given search word.
    while(!feof(fin))
    {
        fscanf(fin,"%s %s",&word,&phoneme);
        if(strcmp(word,search_word)==0)
        {
            break;
        }
    }
    rewind(fin);
    char prev_ph[300];
    char prev_word[200];
    strcpy(prev_word,word);
    strcpy(prev_ph,phoneme);
    printf("Pronounciation = %s\n",phoneme);
    while(!feof(fin))
    {
        fscanf(fin,"%s %s",&word,&phoneme);
        if(strcmp(prev_ph,phoneme)==0 && strcmp(word,prev_word)!=0 && !insidepunc(word))
        {
            store[*indx][0] = '\0';
            strcpy(store[*indx],word);
            *indx = *indx + 1;
            printf("Phoneme Word = %s\n",word);
            //return word;
        }
    }
    //printf("I am here too\n");
    fclose(fin);
    int i;
    /*for(i=0;i<(*indx);i++) {
        printf("Phoneme word in listL: %s\n", store[i]);
    }*/
    char **res = (char **)malloc(*indx * sizeof(char *));
    int bleh;
    for (bleh = 0; bleh < (*indx); bleh++) {
        res[bleh] = (char *)malloc(100 * sizeof(char));
        strcpy(res[bleh], store[bleh]);
    }
    /*for(bleh = 0; bleh < (*indx); bleh++) {
        printf("Phoneme word in dynamic list: %s\n", res[bleh]);
    }*/
    //return "";
    return res;
}

void print(int con[27][27], char *str) //Print the confusion matrix
{
    char filename[]="Confusion Matrix ";
    strcat(filename,str);
    strcat(filename,".txt");
    FILE *fin=fopen(filename,"w");
    if(fin==NULL)
    {
        printf("%s\n",filename);
        return;
    }
    int i,j;
    for(i=0; i<27; i++)
    {
        for(j=0; j<27; j++)
        {
            fprintf(fin,"%d ",con[i][j]);
        }
        fprintf(fin,"\n");
    }
    fclose(fin);
}
void change2smallcaps(char *str) // Function to change large caps to small caps
{
    int len=strlen(str);
    int i;
    for(i=0; i<len; i++)
    {
        if(str[i]>=65 && str[i]<=90)
        {
            str[i]+=32;
        }
    }
}
bool inRange(int i) // Check whether a value is in the range of a-z
{
    if(i>=97 && i<=122)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void editdistConfusionMat(char *s1,char *s2, int con_sub[27][27],int con_ins[27][27],int con_del[27][27])  //Function to calculate the edit distance
{
    int x=0,y=0,cost,len1,len2,i,j,store,k=0,a=97;
    int left,right,corner;
    int count=0;
    len1=strlen(s1);
    len2=strlen(s2);
    int arr[len2+1][len1+1];
    int row=0,col=0,str1,str2;
    //Initializing the row
    for(x=0; x<=len2; x++)
    {
        arr[x][0]=x;
    }
//Initializing the column
    for(y=0; y<=len1; y++)
    {
        arr[0][y]=y;
    }
    //Calculating edit distance
    for(x=1; x<=len2; x++)
    {
        for(y=1; y<=len1; y++)
        {
            if(s1[y-1]==s2[x-1])
                cost=0;
            else
                cost=1;
            arr[x][y]=min(arr[x-1][y]+1,arr[x][y-1]+1,arr[x-1][y-1]+cost);
            if(arr[x][y]==arr[x-1][y-1]+cost) //substitution
            {
                str1=s1[y-1];
                str2=s2[x-1];
                if(inRange(str1) && inRange(str2))
                {
                    for(k=1; k<=26; k++)
                    {
                        if((con_sub[0][k])==str1)
                        {
                            col=k;
                        }
                    }
                    for(k=1; k<=26; k++)
                    {
                        if((con_sub[k][0])==str2)
                        {
                            row=k;
                        }
                    }
                    con_sub[row][col]++;
                }
            }
            else if(arr[x][y]==arr[x-1][y]+1) //insertion
            {
                str1=s1[y];
                str2=s2[x];
                if(inRange(str1) && inRange(str2))
                {
                    for(k=1; k<=26; k++)
                    {
                        if((con_ins[0][k])==str1)
                        {
                            col=k;
                        }
                    }
                    for(k=1; k<=26; k++)
                    {
                        if((con_ins[k][0])==str2)
                        {
                            row=k;
                        }
                    }
                    con_ins[row][col]++;
                }
            }
            else if(arr[x][y]==arr[x][y-1]+1) //deletion
            {
                str1=s1[y];
                str2=s2[x];
                if(inRange(str1) && inRange(str2))
                {
                    for(k=1; k<=26; k++)
                    {
                        if((con_del[0][k])==str1)
                        {
                            col=k;
                        }
                    }
                    for(k=1; k<=26; k++)
                    {
                        if((con_del[k][0])==str2)
                        {
                            row=k;
                        }
                    }
                    con_del[row][col]++;
                }
            }
        }
    }
    for(x=1,y=1; x<=26,y<=26; x++,y++)
    {
        con_sub[x][y]=0;
        con_ins[x][y]=0;
        con_del[x][y]=0;
    }
//   return(arr[len2][len1]);
}
void initialize(int con[27][27])  //Function to initialize the confusion matrix
{
    int i,j,a;
    for(i=0; i<27; i++)
    {
        for(j=0; j<27; j++)
        {
            con[i][j]=0;
        }
    }
    a=97;
    for(i=1; i<=26; i++)
    {
        con[i][0]=a;
        a++;
    }
    a=97;
    for(i=1; i<=26; i++)
    {
        con[0][i]=a;
        a++;
    }
    //free(con);
}
void createConfusionMatrix(int con_sub[27][27],int con_ins[27][27],int con_del[27][27]) // Function to create the confusion matrices
{
    char *source,*target;
    char *test_word;
    test_word=(char *)malloc(sizeof(test_word));
    source=(char *)malloc(sizeof(source));
    target=(char *)malloc(sizeof(target));
    initialize(con_sub);
    initialize(con_ins);
    initialize(con_del);
    int distance;
    char str[MAX];
    int i=0,j;
    int len;
    FILE *fp;
    if((fp=fopen("abc1.txt","r+"))==NULL)
    {
        printf("Cannot open abc1.txt\n");
        return;
    }
    while(fgets(str,MAX,fp) != NULL)
    {
        char first_word[WORDLEN];
        char other_word[WORDLEN];
        for(i=0; str[i]!=':'; i++)
        {
            first_word[i] = str[i];
        }
        first_word[i] = '\0';
        change2smallcaps(first_word);
        int count = 0;
        i++;
        for(;; i++)
        {
            if(str[i] == ' ')
            {
                continue;
            }
            else if(str[i] == ',')
            {
                other_word[count] = '\0';
                change2smallcaps(other_word);
                editdistConfusionMat(first_word,other_word,con_sub,con_ins,con_del);
                count = 0;
                continue;
            }
            else if(str[i] == '\n')
            {
                other_word[count] = '\0';
                change2smallcaps(other_word);
                editdistConfusionMat(first_word,other_word,con_sub,con_ins,con_del);
                break;
            }
            else
            {
                other_word[count] = str[i];
                count++;
            }
        }
    }
    fclose(fp);
}
int insidepunc(char *buffer) // Function to check if any punctuation exists in the string
{
    int i;
    int len=strlen(buffer);
    for(i=0; i<len; i++)
    {
        if((buffer[i] < 65) || (buffer[i] > 90 && buffer[i] < 97) || (buffer[i] > 122))
            return 1;
        //if(buffer[i]=='\'' || buffer[i]=='@' || buffer[i]=='-')
        //return true;
    }
    return 0;
}
char *longword(char *str) //Function to reduce the word (eg. Ohhhhhh to Oh)
{
    char *ans=(char *)malloc(sizeof(str));
    int len=strlen(str);
    int i,dup=0,count=0,j,save;
    for(i=0; i<len;)
    {
        char c = str[i];
        ans[dup++]=c;
        count=0;
        save=i;
        while(str[i]!='\0' && c==str[save])
        {
            count++;
            save++;
        }
        if(count>2)
        {
            i+=count;
        }
        else
        {
            i++;
        }
    }
    ans[dup]='\0';
    return ans;
}
/**TODO !! If short 1 has a common starting character with short 2 then short 1 is expanded first**/
char *shrtcut(char *buffer,int *changed) // Check for shortcuts and change the "changed" flag
{
    FILE *fph=fopen("ph.txt","r");
    if(fph==NULL)
    {
        printf("Cannot open ph.txt");
        return;
    }
    FILE *fs1=fopen("shrt words1.txt","r");
    if(fs1==NULL)
    {
        printf("Cannot open shrt words1.txt");
        return;
    }
    FILE *fs2=fopen("shrt words2.txt","r");
    if(fs2==NULL)
    {
        printf("Cannot open shrt words.txt");
        return;
    }
    char shrttwo[100][20],corresshrttwo[100][20];
    char shrtone[100],corresshrtone[100][20];
    char mistake[200][50],corresmistake[200][50];
    char tempword[50];
    int indx=0;
    int i;
    while(!feof(fs1))
    {
        fscanf(fs1,"%[^\n]\n",tempword);
        shrtone[indx]=tempword[0];
        int l=0;
        for(i=2; i<=strlen(tempword); i++)
        {
            corresshrtone[indx][l++]=tempword[i];
        }
        indx++;
    }
    int shrtoneindx=indx-1;
    indx=0;
    while(!feof(fs2))
    {
        fscanf(fs2,"%s",shrttwo[indx]);
        fscanf(fs2,"%s",corresshrttwo[indx]);
        indx++;
    }
    int shrttwoindx=indx-1;
    indx=0;
    while(!feof(fph))
    {
        fscanf(fph,"%s %s",mistake[indx],corresmistake[indx]);
        indx++;
    }
    int mistakeindx=indx-1;
    int j,k,flag=0;
    char cword[500];
    for(i=0; i<mistakeindx; i++)
    {
        if(strcmp(buffer,mistake[i])==0)
        {
            strcpy(cword,corresmistake[i]);
            *changed=1;
            flag=1;
            break;
        }
    }
    if(flag!=1)
    {
        int len=strlen(buffer);
        int temp=0;
        char s1,s2;
        char c1[5];
        int flag1,flag2,add;
        for(i=0; buffer[i]!='\0'; i++)
        {
            flag1=0;
            flag2=0;
            add=0;
            s1=buffer[i];
            s2=buffer[i+1];
            for(j=0; j<shrtoneindx; j++)
            {
                if(buffer[i]==shrtone[j])
                {
                    flag1=1;
                    for(k=0; k<strlen(corresshrtone[j]); k++)
                    {
                        cword[temp]=corresshrtone[j][k];
                        temp++;
                        add=strlen(corresshrtone[j]);
                    }
                    break;
                }
            }
            c1[1]=s2;
            c1[2]='\0';
            for(j=0; j<shrttwoindx; j++)
            {
                if(strcmp(c1,shrttwo[j])==0)
                {
                    flag2=1;
                    add=0;
                    i++;
                    for(k=0; k<strlen(corresshrttwo[j]); k++)
                    {
                        cword[temp]=corresshrttwo[j][k];
                        temp++;
                        add=strlen(corresshrttwo[j]);
                    }
                    break;
                }
            }
            if(flag1!=1 && flag2!=1)
            {
                cword[temp++]=buffer[i];
            }
        }
        cword[temp]='\0';
    }
    fclose(fs1);
    fclose(fs2);
    fclose(fph);
    //printf("Changing to %s\n",cword);
    return cword;
}
bool check_existance(char *str) // Function to check whether the given word is valid or not
{
    // printf("Checking word = %s\n",str);
    FILE *fin = fopen("ugram.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open ugram.txt");
        return false;
    }
    char buffer[100];
    char probability[20];
    while(!feof(fin))
    {
        fscanf(fin,"%s\t%s\n",probability,buffer);
        if(strcmp(buffer,str)==0)
        {
            //  printf("Exiting from check existance\n");
            return true;
        }
        //  printf("%s\t",buffer);
    }
    // printf("Out of the while loop\n");
    fclose(fin);
    /*fin=fopen("all words.txt","r");
    if(fin==NULL)
    {
    printf("Cannot open all words.txt");
    return;
    }
    //char buffer[100];
    int flag=0;
    //printf("I am here again\n");
    while(!feof(fin))
    {
    fscanf(fin,"%s",buffer);
    if(strcmp(buffer,str)==0)
    {
      //  printf("Exiting from check existance\n");
        return true;
    }
    }
    fclose(fin);*/
    // printf("Exiting from check existance\n");
    return false;
}
char *concat2(char *str1,char *str2)
{
    int len1,len2;
    len1=strlen(str1);
    len2=strlen(str2);
    int i,j;
    char final_word[100];
    for(i=0; i<len1; i++)
    {
        final_word[i]=str1[i];
    }
    final_word[i++]=' ';
    for(j=0; j<len2; j++)
    {
        final_word[i]=str2[j];
        i++;
    }
    final_word[i]='\0';
    return final_word;
}
char *concat3(char *str1,char *str2,char *str3)
{
    int len1,len2,len3;
    len1=strlen(str1);
    len2=strlen(str2);
    len3=strlen(str3);
    int i,j;
    //printf("Len1 = %d\tLen2 = %d\tLen3 = %d\n",len1,len2,len3);
    char final_word[1000];
    for(i=0; i<len1; i++)
    {
        final_word[i]=str1[i];
    }
    final_word[i++]=' ';
    for(j=0; j<len2; j++)
    {
        final_word[i]=str2[j];
        i++;
    }
    final_word[i++]=' ';
    for(j=0; j<len3; j++)
    {
        final_word[i]=str3[j];
        i++;
    }
    final_word[i]='\0';
    //printf("Exiting from concat3\n");
    return final_word;
}
int edit(char *s1,char *s2) // Function to get only the edit distance
{
    int x=0,y=0,cost,len1,len2,i,j;
    len1=strlen(s1);
    len2=strlen(s2);
    int arr[len2+1][len1+1];
    int row=0,col=0,str1,str2;
    //Initializing the row
    for(x=0; x<=len2; x++)
    {
        arr[x][0]=x;
    }
//Initializing the column
    for(y=0; y<=len1; y++)
    {
        arr[0][y]=y;
    }
    //Calculating edit distance
    for(x=1; x<=len2; x++)
    {
        for(y=1; y<=len1; y++)
        {
            if(s1[y-1]==s2[x-1])
                cost=0;
            else
                cost=1;
            arr[x][y]=min(arr[x-1][y]+1,arr[x][y-1]+1,arr[x-1][y-1]+cost);
        }
    }
    return(arr[len2][len1]);
}
void suggestions(char test_word[], char **store, float *prob, int *indx, int *valid) // Function to add words in store_word with edit distance = 2
{
    FILE *fin = fopen("ugram.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open ugram.txt");
        return;
    }
    char buffer[100];
    float probability;
    while(!feof(fin))
    {
        fscanf(fin,"%f\t%s\n",&probability,buffer);
        change2smallcaps(buffer);
        if(strcmp(buffer,test_word)==0)
        {
            (*valid)=1;
        }
        int distance=edit(test_word,buffer);
        if(distance<=1 && !insidepunc(buffer))
        {
            // printf("Scanning = %s\n",buffer);

            int k=*indx;
            strcpy(store[k],buffer);
            prob[k]=pow(2.71828,probability);
            printf("Found %s within edit distance 1 of given word: %s with prob: %f\n", buffer, test_word, probability);
            //printf("Storing the prob = %f\n",prob[k]);
            (*indx)++;
            //printf("Edit distance less than 2 = %s\n",buffer);
        }
    }
    fclose(fin);
}
void trim(char *str) // Function to trim a given string
{
    int i,j=0;
    for(i=0; str[i]!='\0'; i++)
    {
        if(str[i]!=' ' && str[i] != '\t')
        {
            str[j++]=str[i];
        }
    }
    str[j]='\0';
}
void checkquadragram(char str[], double* probs, int sock)
{
    change2smallcaps(str);
    *probs = extract_prob(str,&probs,sock,4);
    printf("Probability = %lf\n",(*probs));
    //getchar();
}

void checktrigram(char str[], double* prob,int sock)
{
    change2smallcaps(str);
    *prob = extract_prob(str,&prob,sock,3);
    printf("Probability = %lf\n",(*prob));
}

void checkbigram(char str[], double* prob, int sock)
{
    change2smallcaps(str);
    *prob = extract_prob(str,&prob,sock,2);
    //printf("I am here!!\n");
    printf("Probability = %lf\n",(*prob));
}
void checkunigram(char *test_word, float *prob, int *indx, char ans[][100], char original_word[])
{
    FILE *fin=fopen("ugram.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open ugram.txt");
        return;
    }
    char buffer[100];
    float probability;
    while(!feof(fin))
    {
        fscanf(fin,"%f ",&probability);
        fscanf(fin,"%[^\n]",buffer);
        change2smallcaps(test_word);
        change2smallcaps(buffer);
        if(strcmp(test_word,buffer)==0)
        {
            prob[*indx]=probability;
            strcpy(ans[*indx],original_word);
            (*indx)++;
            //printf("%s - Unigram Match FOUND\n",original_word);
            break;
        }
    }
    //printf("Exiting from unigram with index = %d\n",*indx);
    fclose(fin);
}
bool insidePunc(char *buffer)
{
    int i;
    int len=strlen(buffer);
    for(i=0; i<len; i++)
    {
        if(buffer[i]=='\'' || buffer[i]=='@' || buffer[i]=='$' || buffer[i]=='^')
            return true;
    }
    return false;
}
void swap(float *xp, float *yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void swap_string(char **str1_ptr, char **str2_ptr)
{
    char *temp = *str1_ptr;
    *str1_ptr = *str2_ptr;
    *str2_ptr = temp;
}
void selectionSort(float arr[], int n, char **answer)
{
    int i, j, min_idx;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
            if (arr[j] > arr[min_idx])
                min_idx = j;
        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
        swap_string(&answer[min_idx],&answer[i]);
    }
}
void print_edit_distance(int row, int col, int arr[row][col])
{
    int x,y;
    for(x=0; x<row; x++)
    {
        for(y=0; y<col; y++)
        {
            printf("%d ",arr[x][y]);
        }
        printf("\n");
    }
}
// Find where what action took place and store the value from the confusion matrices in MULTIPLIER
void find_Diff(char s1[], char s2[], int con_sub[27][27],int con_ins[27][27],int con_del[27][27], float multiply[], int index)
{
    int len1=strlen(s1);
    int len2=strlen(s2);
    int x=0,y=0,cost;
    int arr[len2+1][len1+1];
    for(x=0; x<=len2; x++)
    {
        arr[x][0]=x;
    }
    for(y=0; y<=len1; y++)
    {
        arr[0][y]=y;
    }
    for(x=1; x<=len2; x++)
    {
        for(y=1; y<=len1; y++)
        {
            if(s1[y-1]==s2[x-1])
                cost=0;
            else
                cost=1;
            arr[x][y]=min(arr[x-1][y]+1,arr[x][y-1]+1,arr[x-1][y-1]+cost);
        }
    }
    //printf("%s - %s\n",s1,s2);
    //print_edit_distance(len2+1,len1+1,arr);
    //printf("Confusion Matrix Done\n");
    int i=len2;
    int j=len1;
    int left, dia, up;
    float value=0.0;
    float divisor;
    float dividend;
    while(i!=0 && j!=0)
    {
        /*left=arr[i][j-1]; insertion
        dia=arr[i-1][j-1];  substitution
        up=arr[i-1][j];   deletion*/
        dividend=0.0;
        divisor=0.0;
        // printf("i = %d\tj = %d\n",i,j);
        if(i!=0)
        {
            up=arr[i-1][j];
            //printf("I am here\n");
            dia=arr[i-1][j-1];
        }
        if(j!=0)
        {
            left=arr[i][j-1];
        }
        int minimum = min(left,up,dia);
        //printf("Min = %d\n",minimum);
        char c1,c2;
        int p,q;
        if(minimum==left)
        {
            c1=s1[j-1];
            c2=s2[i-1];
            dividend=con_ins[c1-96][c2-96];
            p=c1-96;
            for(q=1; q<27; q++)
            {
                divisor+=con_ins[p][q];
            }
            // printf("%c Insertion %c\n",s1[j-1],s2[i-1]);
            // printf("Dividend = %f\n",dividend);
            // printf("Divisor = %f\n",divisor);
            value+=dividend/divisor;
            j--;
        }
        else if(minimum==up)
        {
            c1=s1[j-1];
            c2=s2[i-1];
            dividend=con_del[c1-96][c2-96];
            p=c1-96;
            for(q=1; q<27; q++)
            {
                divisor+=con_del[p][q];
            }
            // printf("%c Deletion %c\n",s1[j-1],s2[i-1]);
            // printf("Dividend = %f\n",dividend);
            // printf("Divisor = %f\n",divisor);
            value+=dividend/divisor;
            i--;
        }
        else
        {
            c1=s1[j-1];
            c2=s2[i-1];
            dividend=con_sub[c1-96][c2-96];
            p=c1-96;
            for(q=1; q<27; q++)
            {
                divisor+=con_sub[p][q];
            }
            // printf("%c Substitution %c\n",s1[j-1],s2[i-1]);
            // printf("Dividend = %f\n",dividend);
            // printf("Divisor = %f\n",divisor);
            value+=dividend/divisor;
            i--;
            j--;
        }
    }
    multiply[index]=value;
    // printf("Value = %f\n",value);
}

float get_probability(char word[])
{
    FILE *fin=fopen("ugram.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open ugram.txt");
        return 0;
    }
    char buffer[100];
    float probability;
    while(!feof(fin))
    {
        fscanf(fin,"%f\t%s\n ",&probability, buffer);
        if(strcmp(word,buffer)==0)
        {
            return probability;
        }
    }
    //printf("Exiting from unigram with index = %d\n",*indx);
    fclose(fin);
    return 0;
}
void sorting(WORD_INFO **arr, int n)
{
    int i, j, max_idx;
    for (i = 0; i < n-1; i++)
    {
        // Find the maximum element in unsorted array
        max_idx = i;
        for (j = i+1; j < n; j++)
        {
            if (get_prob(arr[j]) > get_prob(arr[max_idx]))
            {
                max_idx = j;
            }
        }
        WORD_INFO *temp=arr[max_idx];
        arr[max_idx]=arr[i];
        arr[i]=temp;
    }
}
void find_top_suggestions(char **store_word, float *probability, int store_indx, char **sug_word, float *sug_prob)
{
    //printf("Just the start: %s\n",store_word[0]);
    int *marked = (int *)malloc(store_indx * sizeof(int));
    int i,j;
    for(i=0; i<store_indx; i++) {
        marked[i] = 0;
    }
    int cnt = 0;
    /*for(i=0;i<store_indx;i++) {
        printf("Index: %d\t Word: %s\n",i,store_word[i]);
        getchar();
    }*/
    for(i=0; i<number; i++) {
        int max_idx = 0;
        while(marked[max_idx] != 0) {
            max_idx++;
        }
        for(j=0; j<store_indx; j++) {
            if((probability[j] >= probability[max_idx]) && (marked[j] == 0)) {
                max_idx = j;
            }
        }
        sug_prob[cnt] = probability[max_idx];
        sug_word[cnt] = store_word[max_idx];
        //printf("maximum index obtained: %d\tMaximum probability: %f\tWord associated: %s\n",max_idx,sug_prob[cnt],store_word[max_idx]);
        //getchar();
        cnt++;
        marked[max_idx] = 1;
    }
}

void print_word_info_array(WORD_INFO ***arr, int word_num, FILE *fp) {
    int i, j;
    fprintf(fp, "-------------------------------------------------------------------------------------\n");
    for(i=0; i < word_num; i++) {
        for(j=0; j<number; j++) {
            WORD_INFO *temp = get_element(arr, i, j);
            fprintf(fp, "||%s %s %s %s: %f\t", get_strn1(temp), get_strn2(temp), get_strn3(temp), get_strn4(temp), get_prob(temp));
        }
        fprintf(fp, "\n");
    }
}

//A function to split a sentence into its constituent words
char **break_sentence_words(char *sentence, int *nwords) {
    int num_space = 0;
    int i;
    for(i=0; i<strlen(sentence); i++) {
        if (sentence[i] == ' ') {
            num_space++;
        }
    }
    int num_words = num_space + 1;
    char **res = (char **)malloc(num_words * sizeof(char *));
    for (i=0; i<num_words; i++) {
        res[i] = (char *)malloc(WORDLEN * sizeof(char));
        res[i][0] = '\0';
    }
    int cnt = 0, cnt2 = 0;
    for (i=0; i<strlen(sentence); i++) {
        if (sentence[i] != ' ' || sentence[i] != '\0') {
            res[cnt][cnt2] = sentence[i];
            cnt2++;
        }
        else {
            res[cnt][cnt2] = '\0';
            cnt++;
        }
    }
    *nwords = num_words;
    return res;
}

int main()
{
    static int con_del[27][27];
    static int con_ins[27][27];
    static int con_sub[27][27];
    createConfusionMatrix(con_sub,con_ins,con_del);
    //print(con_sub,"Substitution");
    //print(con_del,"Deletion");
    //print(con_ins,"Insertion");
    printf("Confusion matrices successfully created !\n");
    FILE *fin=fopen("dataset.txt","r");
    FILE *fout=fopen("output.txt","w");
    char buffer[200];
    char shrtword[50];
    char firstchange[500];
    //char store_word[1000][100];
    char **store_word = (char **)malloc(1000 * sizeof(char *));
    int i;
    for(i=0; i<1000; i++) {
        store_word[i] = (char *)malloc(100 * sizeof(char));
    }
    float probability[1000];
    buffer[0]='\0';
    int changed=0;
    char suggestedWords_Buffer[number][100];
    int itr;
    for(itr=0; itr<number; itr++)
    {
        memset(suggestedWords_Buffer[itr],'\0',100);
    }
    WORD_INFO ***arr = create_array(30,5);
    int word_no=0;
    int my_socket = connection(number);
    while(!feof(fin))
    {
        fscanf(fin,"%[^\n]\n",buffer);
        int num_words = 0;
        char **brokenwords = break_sentence_words(buffer, &num_words);
        int xz = 0;
        for (xz = 0; xz < num_words; xz++) {
            strcpy(buffer, brokenwords[xz]);
            change2smallcaps(buffer);
            printf("\nScanning = %s\n",buffer);
            int store_indx=0;
            changed=0;
            int i=0;
            int len=strlen(buffer);
            if(!insidePunc(buffer))
            {
                /**Preprocessing the word **/
                strcpy(buffer,longword(buffer)); //Remove extraneous character eg. ohhh => oh
                strcpy(firstchange,shrtcut(buffer,&changed));
                strcpy(buffer,firstchange);
                printf("After initial word normalization: %s\n", buffer);
                /**End of preprocessing **/
                // printf("I am here\n");
                if(strlen(buffer) >= 1)
                {
                    store_indx=0;
                    int valid_word=0;
                    suggestions(buffer,store_word,probability,&store_indx,&valid_word);
                    if(valid_word==1 && type==1)
                    {
                        continue;
                    }
                    printf("After suggestion index = %d\n",store_indx);
                    char phoneme_words[10][30];
                    char **phoneme_word = NULL;
                    int phoneme_indx=0;
                    float phoneme_prob;
                    /***CHECK FROM HERE --------------------------------------------------------------------------------**/
                    phoneme_word = searchPh(buffer,phoneme_words,&phoneme_indx);
                    printf("After phoneme index = %d\n",phoneme_indx);
                    for(i=0; i<phoneme_indx; i++)
                    {
                        //printf("Phoneme word found: %s\n", phoneme_word[i]);
                        if(check_existance(phoneme_word[i]) == true) {
                            phoneme_prob=get_probability(phoneme_word[i]);
                            phoneme_prob = pow(2.71828,phoneme_prob);
                            strcpy(store_word[store_indx],phoneme_word[i]);
                            probability[store_indx]=phoneme_prob+1;
                            //printf("Phoneme: %s\t Probability: %f\n", phoneme_word[i], phoneme_prob);
                            store_indx++;
                        }
                        /*else {
                            //printf("Phoneme word : %s not found!!\n", phoneme_word[i]);
                        }*/
                        // printf("Phoneme = %s(%f)\n",phoneme_word[i],phoneme_prob);
                    }

                    float *multiplier = (float *)malloc(store_indx * sizeof(float));
                    for(i=0; i<store_indx; i++)
                    {
                        find_Diff(buffer,store_word[i],con_sub,con_ins,con_del,multiplier,i);
                    }
                    /*for(i=0;i<store_indx;i++)
                    {
                        printf("%s\t%s\t%f\n",buffer,store_word[i],multiplier[i]);
                    }
                    getchar();*/
                    for(i=0; i<store_indx; i++)
                    {
                        if(probability[i] <= 1) {
                            probability[i]=probability[i]*multiplier[i];
                            //probability[i] = multiplier[i];
                        }
                        else {
                            probability[i] = probability[i] - 1;
                            float channel_prob = 0.1/phoneme_indx;
                            probability[i] *= channel_prob;
                        }
                    }

                    /***CHECK TILL HERE --------------------------------------------------------------------------------------**/
                    /*Code to identify just the top 5 (number) suggestions and send them over */
                    float *sug_prob = (float *)malloc(number * sizeof(float));
                    char **sug_word = (char **)malloc(number * sizeof(char *));
                    for(i=0; i<number; i++) {
                        sug_word[i] = NULL;
                    }
                    find_top_suggestions(store_word, probability, store_indx, sug_word, sug_prob);
                    for(i=0; i<number; i++) {
                        printf("%s\tp(x) = %f\n",sug_word[i],sug_prob[i]);
                        //getchar();
                    }
                    for(i=0; i<number; i++)
                    {
                        strcpy(suggestedWords_Buffer[i],sug_word[i]);
                        probability[i] = sug_prob[i];
                    }
                    if(valid_word==1)
                    {
                        strcpy(suggestedWords_Buffer[number-1],buffer);
                    }
                    for(i=0; i<number; i++)
                    {
                        printf("The suggested words for the word %s are : %s(%f)\n",buffer,suggestedWords_Buffer[i],probability[i]);
                        //fprintf(fout,"The suggested words for the word %s are : %s(%f)\n",buffer,suggestedWords_Buffer[i],probability[i]);
                    }
                    //getchar();
                    //printf("WORKS FINE TILL HERE !!! \n");
                    /*---------------------------- TILL 5 UNIGRAM SUGGESTIONS---------------------------*/
                    printf("\nEntering DANGER ZONE\n");
                    int prob_indx=0;
                    char addword[10000];
                    double prob,prev_prob;
                    int x,y,z;
                    int index=0;
                    WORD_INFO *temp[number * number];
                    for(x=0; x<(number*number); x++)
                    {
                        temp[x]=NULL;
                    }
                    if(word_no==0)
                    {
                        if(model==4)
                        {
                            for(x=0; x<number; x++)
                            {
                                prob=0.0;
                                strcpy(addword,concat2("<s>",suggestedWords_Buffer[x]));
                                printf("Word send for Bigram = %s\n",addword);
                                checkbigram(addword,&prob,my_socket);
                                if (prob != 0) {
                                    prob = pow(2.71828,prob);
                                }
                                /*else
                                {
                                  prob=pow(2.71828,unknown);
                                  fprintf(fout,"Smoothed prob: %f\n",prob);
                                }*/
                                //fprintf(fout,"Word send for Bigram =%s with probability =%f\n",addword,prob);
                                temp[index] = new_quadragram_node("<s>","<s>","<s>",suggestedWords_Buffer[x],prob);
                                index++;
                            }
                            //getchar();
                        }
                        if(model==3)
                        {
                            for(x=0; x<number; x++)
                            {
                                prob=0.0;
                                strcpy(addword,concat2("<s>",suggestedWords_Buffer[x]));
                                // printf("Word send for Trigram = %s\n",addword);
                                checkbigram(addword,&prob,my_socket);
                                // printf("For %s - trigram prob = %lf\n",addword,prob);
                                temp[index] = new_trigram_node("<s>","<s>",suggestedWords_Buffer[x],prob);
                                index++;
                            }
                        }
                        if(model==2)
                        {
                            for(x=0; x<number; x++)
                            {
                                prob=0.0;
                                strcpy(addword,concat2("<s>",suggestedWords_Buffer[x]));
                                checkbigram(addword,&prob,my_socket);
                                temp[index] = new_bigram_node("<s>",suggestedWords_Buffer[x],prob);
                                index++;
                            }
                        }
                        if(model==1)
                        {
                            for(x=0; x<number; x++)
                            {
                                temp[index]=new_unigram_node(suggestedWords_Buffer[x],probability[x]);
                                index++;
                            }
                        }
                        double temp_prob;
                        /*for(x=0;x<number;x++)
                        {
                        temp_prob=get_prob(temp[x]);
                        temp_prob=pow(2.71828,temp_prob);
                        temp_prob=set_prob(temp[x],temp_prob);
                        }*/
                        sorting(temp,index);
                        index=0;
                        for(x=0; x<number; x++)
                        {
                            int pi=set_element(arr,word_no,x,temp[x]);
                            WORD_INFO *n1 = get_element(arr,word_no,x);
                            //fprintf(fout,"%s %s %s %s(%lf)\n",get_strn1(n1),get_strn2(n1),get_strn3(n1),get_strn4(n1),get_prob(n1));
                        }
                    }
                    else if(word_no==1)
                    {
                        if(model==4)
                        {
                            for(y=0; y<number; y++)
                            {
                                for(x=0; x<number; x++)
                                {
                                    prob=0.0;
                                    strcpy(addword,"");
                                    strcat(addword,get_strn3(get_element(arr,word_no-1,x)));
                                    strcat(addword," ");
                                    strcat(addword,get_strn4(get_element(arr,word_no-1,x)));
                                    strcat(addword," ");
                                    strcat(addword,suggestedWords_Buffer[y]);
                                    printf("Word sending for trigram = %s\n",addword);
                                    //getchar();
                                    checktrigram(addword,&prob,my_socket);
                                    //fprintf(fout,"Word send for Trigram =%s with probability =%f\n",addword,prob);
                                    if(prob == 0) {
                                        addword[0] = '\0';
                                        strcpy(addword, "");
                                        strcat(addword, get_strn4(get_element(arr, word_no-1,x)));
                                        strcat(addword, " ");
                                        strcat(addword, suggestedWords_Buffer[y]);
                                        printf("Word sending for Bigram backoff = %s\n", addword);
                                        checkbigram(addword, &prob, my_socket);
                                        // fprintf(fout, "Word sent for Bigram backoff = %s with probability = %f\n", addword, prob);
                                        /*if(prob == 0)
                                        {
                                          prob=pow(2.71828,unknown);
                                          fprintf(fout,"Smoothed prob: %f\n",prob);
                                        }*/
                                    }
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                        prob += 0.02;
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_quadragram_node(addword,get_strn3(get_element(arr,word_no-1,x)),get_strn4(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        if(model==3)
                        {
                            for(x=0; x<number; x++)
                            {
                                for(y=0; y<number; y++)
                                {
                                    prob=0.0;
                                    strcpy(addword,concat3(get_strn2(get_element(arr,word_no-1,x)),get_strn3(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y]));
                                    checktrigram(addword,&prob,my_socket);
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_trigram_node(addword,get_strn3(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        if(model==2)
                        {
                            for(x=0; x<number; x++)
                            {
                                for(y=0; y<number; y++)
                                {
                                    prob=0.0;
                                    strcpy(addword,concat2(get_strn2(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y]));
                                    checkbigram(addword,&prob,my_socket);
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_bigram_node(addword,suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        //printf("Loop ends\n");
                        sorting(temp,index);
                        index=0;
                        for(i=0; i<number; i++)
                        {
                            int pi=set_element(arr,word_no,i,temp[i]);
                            WORD_INFO *n1 = get_element(arr,word_no,i);
                            //fprintf(fout,"%s %s %s %s(%lf)\n",get_strn1(n1),get_strn2(n1),get_strn3(n1),get_strn4(n1),get_prob(n1));
                        }
                    }
                    else
                    {
                        if(model==4)
                        {
                            for(y=0; y<number; y++)
                            {
                                for(x=0; x<number; x++)
                                {
                                    prob=0.0;
                                    strcpy(addword,"");
                                    strcat(addword,get_strn2(get_element(arr,word_no-1,x)));
                                    strcat(addword," ");
                                    strcat(addword,get_strn3(get_element(arr,word_no-1,x)));
                                    strcat(addword," ");
                                    strcat(addword,get_strn4(get_element(arr,word_no-1,x)));
                                    strcat(addword," ");
                                    strcat(addword,suggestedWords_Buffer[y]);
                                    printf("Word sending for quadragram = %s\n",addword);
                                    //getchar();
                                    //strcpy(addword,concat3(get_strn2(get_element(arr,word_no-1,x)),get_strn3(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y]));
                                    checkquadragram(addword,&prob,my_socket);
                                    //fprintf(fout,"Word send for Quadragram =%s with probability =%f\n",addword,prob);
                                    if(prob == 0) {
                                        addword[0] = '\0';
                                        strcpy(addword, "");
                                        strcat(addword, get_strn3(get_element(arr,word_no-1,x)));
                                        strcat(addword, " ");
                                        strcat(addword, get_strn4(get_element(arr,word_no-1,x)));
                                        strcat(addword, " ");
                                        strcat(addword, suggestedWords_Buffer[y]);
                                        printf("Word sending for trigram backoff = %s\n", addword);
                                        checktrigram(addword, &prob, my_socket);
                                        //fprintf(fout, "Word sent for trigram backoff = %s with probability = %f\n", addword, prob);
                                        if(prob == 0)
                                        {
                                            addword[0] = '\0';
                                            strcat(addword, get_strn4(get_element(arr,word_no-1,x)));
                                            strcat(addword, " ");
                                            strcat(addword, suggestedWords_Buffer[y]);
                                            printf("Word sending for bigram backoff = %s\n", addword);
                                            checkbigram(addword, &prob, my_socket);
                                            //fprintf(fout, "Word sent for bigram backoff = %s with probability = %f\n", addword, prob);
                                        }

                                    }
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                        prob += 0.02;
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_quadragram_node(addword,get_strn3(get_element(arr,word_no-1,x)),get_strn4(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        if(model==3)
                        {
                            for(x=0; x<number; x++)
                            {
                                for(y=0; y<number; y++)
                                {
                                    prob=0.0;
                                    strcpy(addword,concat3(get_strn2(get_element(arr,word_no-1,x)),get_strn3(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y]));
                                    checktrigram(addword,&prob,my_socket);
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_trigram_node(addword,get_strn3(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        if(model==2)
                        {
                            for(x=0; x<number; x++)
                            {
                                for(y=0; y<number; y++)
                                {
                                    prob=0.0;
                                    strcpy(addword,concat2(get_strn2(get_element(arr,word_no-1,x)),suggestedWords_Buffer[y]));
                                    checkbigram(addword,&prob,my_socket);
                                    if(prob!=0.0)
                                    {
                                        prob=pow(2.71828,prob);
                                    }
                                    prev_prob=get_prob(get_element(arr,word_no-1,x));
                                    prob+=prev_prob;
                                    addword[0]='\0';
                                    strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                                    temp[index] = new_bigram_node(addword,suggestedWords_Buffer[y],prob);
                                    index++;
                                }
                            }
                        }
                        // printf("Loop ends\n");
                        sorting(temp,index);
                        index=0;
                        for(i=0; i<number; i++)
                        {
                            int pi=set_element(arr,word_no,i,temp[i]);
                            WORD_INFO *n1 = get_element(arr,word_no,i);
                            //fprintf(fout,"%s %s %s %s(%lf)\n",get_strn1(n1),get_strn2(n1),get_strn3(n1),get_strn4(n1),get_prob(n1));
                        }
                        // getchar();
                    }
                }
                else
                {
                    WORD_INFO *temp[number];
                    double prev_prob;
                    int x;
                    char addword[600];
                    for(x=0; x<number; x++)
                    {
                        temp[x]=NULL;
                    }
                    if(word_no==0)
                    {
                        if(model==4)
                        {
                            for(x=0; x<number; x++)
                            {
                                temp[x] = new_quadragram_node("<s>","<s>","<s>",buffer,0.0);
                            }
                        }
                        if(model==3)
                        {
                            for(x=0; x<number; x++)
                            {
                                temp[x] = new_trigram_node("<s>","<s>",buffer,0.0);
                            }
                        }
                        if(model==2)
                        {
                            for(x=0; x<number; x++)
                            {
                                temp[x] = new_bigram_node("<s>",buffer,0.0);
                            }
                        }
                    }
                    else
                    {
                        for(x=0; x<number; x++)
                        {
                            strcpy(addword,concat2(get_strn1(get_element(arr,word_no-1,x)),get_strn2(get_element(arr,word_no-1,x))));
                            prev_prob=get_prob(get_element(arr,word_no-1,x));
                            if(model==4)
                            {
                                temp[x] = new_quadragram_node(addword,get_strn3(get_element(arr,word_no-1,x)),get_strn3(get_element(arr,word_no-1,x)),buffer,prev_prob);
                            }
                            if(model==3)
                            {
                                temp[x] = new_trigram_node(addword,get_strn3(get_element(arr,word_no-1,x)),buffer,prev_prob);
                            }
                            if(model==2)
                            {
                                temp[x]= new_bigram_node(addword,buffer,prev_prob);
                            }
                        }
                    }
                    for(x=0; x<number; x++)
                    {
                        int pi=set_element(arr,word_no,x,temp[x]);
                        WORD_INFO *n1 = get_element(arr,word_no,x);
                        //fprintf(fout,"%s %s %s %s(%lf) - ",get_strn1(n1),get_strn2(n1),get_strn3(n1),get_strn4(n1),get_prob(n1));
                    }
                    // printf("Leaving 2 lettered words\n");
                }
                print_word_info_array(arr, word_no, fout);
                word_no++;
            }


        }
        fprintf(fout,"Output - > %s %s %s %s",get_strn1(get_element(arr,word_no-1,0)),get_strn2(get_element(arr,word_no-1,0)),get_strn3(get_element(arr,word_no-1,0)),get_strn4(get_element(arr,word_no-1,0)));
        fprintf(fout,"---------------------------------------------------------------------------------------------------------------------------------------");
    }
    close_socket(my_socket);
    printf("DONE\n");
    fclose(fin);
    fclose(fout);
    return 0;
}