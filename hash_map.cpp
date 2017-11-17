//#include<stdio.h>
#include<iostream>
#include<string>
#include <map>
#include<fstream>
#include<math.h>
#include<cstdlib>
//#include<bits/stdc++.h>

using namespace std;

map<string,float> dataset;

void initialize_map()
{
    string buffer;
    ifstream infile;
    infile.open ("four-gram.txt",ios::in);

    getline(infile,buffer);
    cout<<"Buffer : "<<buffer;
    int line=1;

    string sentence,prob;
    double probability;

    while(!infile.eof())
    {
        //cout<<"I am here"<<endl;
        getline(infile,buffer);
        line++;
        //cout<<"Buffer : "<<buffer<<endl;
        //int len=strlen(buffer);
        //sentence="";
        int i=0;
        int indx=0;
        while(buffer[i]!='\t')
        {
            prob[indx]=buffer[i];
            indx++;
            i++;
        }
        prob[indx]='\0';
        probability=atof(prob.c_str());
        //cout<<"After conversion = "<<probability<<'\t';
        indx=0;
        i++;
        int store=i;
        while(buffer[i]!='\0')
        {
            if(buffer[i]!='\t')
            {
                i++;
            }
            else
            {
                break;
            }
        }
        sentence.assign(buffer,store,(i-store));
        //cout<<endl<<"The sentence = "<<sentence<<"."<<endl;
        dataset.insert(pair<string,float>(sentence,probability));
        //if(line==10)
        //    break;
    }
    cout << "File reading complete...";
    infile.close();
    //return 1;
    /*map<string,float>::iterator itr;
    cout<<endl<<endl;
    for(itr=dataset.begin();itr!=dataset.end();itr++)
    {
        cout<<itr->first << ": " << itr->second << endl;
    }*/
}

float extract_prob(string str)
{
    map<string,float>::iterator it;
    it=dataset.find(str);
    return it->second;
}

int main(void){
    initialize_map();
}

