#include<stdio.h>
#include<string.h>

int main()
{
    FILE *fin=fopen("New Bigram.txt","r");
    if(fin==NULL)
    {
        printf("Cannot open input file\n");
        return 0;
    }

    long long int divisor=225955438824;
    long int buffer=0;
    char str1[50];
    char str2[50];
   /* while(!feof(fin))
    {
        fscanf(fin,"%s %s %lli",str1,str2,&buffer);
        //printf("%lli\t",buffer);
        //getchar();
        divisor+=buffer;
        printf("%lli\n",divisor);
    }
    getchar();*/
    rewind(fin);
    FILE *fout=fopen("Modified Bigram.txt","w");
    if(fout==NULL)
    {
        printf("Cannot open output file\n");
        return 0;
    }
    while(!feof(fin))
    {
        fscanf(fin,"%s %s %li",str1,str2,&buffer);
        printf("%li\t",buffer);
        fprintf(fout,"%s %s %lf\n",str1,str2,(double)buffer/divisor);
        printf("%s %s %lf\n",str1,str2,(double)buffer/divisor);
        //getchar();
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
