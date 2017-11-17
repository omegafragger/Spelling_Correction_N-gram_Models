/**Defining the data structure to hold one particular (uni/bi/tri)gram node.
The node contains the following elements:
1. Three strings as strn1, strn2, strn3 : In a trigram model there are at most three words whose trigram probability must be computed.
2. Probability: The uni/bi/tri gram probability corresponding to the set of strings given above.
NOTE: The sequence of the strings is always of the form strn1=>strn2=>strn3.
Also in case of uni/bi gram models the last strings are left as null.**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRN_LEN 1000

/**-------------------------------------------------------------------------------------------------------------------------------------**/
/*Defining the structure*/
struct word_info {
    char *strn1;//first string
    char *strn2;//second string
    char *strn3;//third string
    char *strn4;
    char *strn5;
    double prob;//holds the uni/bi/tri gram probability
};

typedef struct word_info WORD_INFO;

/**----------------------------------- Functions to access the structure ----------------------------------------------------------------**/

/*Generates a unigram node*/
WORD_INFO *new_unigram_node(char *str1, double prb) {
    WORD_INFO *res = (WORD_INFO *)malloc(sizeof(WORD_INFO));
    res->strn1 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn2 = NULL;
    res->strn3 = NULL;
    res->strn4 = NULL;
    res->strn5 = NULL;
    strcpy(res->strn1, str1);
    res->prob = prb;
    return res;
}

/*Generates a bigram node*/
WORD_INFO *new_bigram_node(char *str1, char *str2, double prb) {
    WORD_INFO *res = (WORD_INFO *)malloc(sizeof(WORD_INFO));
    res->strn1 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn2 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn3 = NULL;
    res->strn4 = NULL;
    res->strn5 = NULL;
    strcpy(res->strn1, str1);
    strcpy(res->strn2, str2);
    res->prob = prb;
    return res;
}

/*Generates a trigram node*/
WORD_INFO *new_trigram_node(char *str1, char *str2, char *str3, double prb) {
    WORD_INFO *res = (WORD_INFO *)malloc(sizeof(WORD_INFO));
    res->strn1 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn2 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn3 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn4 = NULL;
    res->strn5 = NULL;
    strcpy(res->strn1, str1);
    strcpy(res->strn2, str2);
    strcpy(res->strn3, str3);
    res->prob = prb;
    return res;
}

/*Generates a quadragram node*/
WORD_INFO *new_quadragram_node(char *str1, char *str2, char *str3, char *str4, double prb) {
    WORD_INFO *res = (WORD_INFO *)malloc(sizeof(WORD_INFO));
    res->strn1 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn2 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn3 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn4 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn5 = NULL;
    strcpy(res->strn1, str1);
    strcpy(res->strn2, str2);
    strcpy(res->strn3, str3);
    strcpy(res->strn4, str4);

    res->prob = prb;
    return res;
}

/*Generates a pentagram node*/
WORD_INFO *new_pentagram_node(char *str1, char *str2, char *str3, char *str4, char *str5, double prb) {
    WORD_INFO *res = (WORD_INFO *)malloc(sizeof(WORD_INFO));
    res->strn1 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn2 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn3 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn4 = (char *)malloc(STRN_LEN * sizeof(char));
    res->strn5 = (char *)malloc(STRN_LEN * sizeof(char));
    strcpy(res->strn1, str1);
    strcpy(res->strn2, str2);
    strcpy(res->strn3, str3);
    strcpy(res->strn4, str3);
    strcpy(res->strn5, str3);
    res->prob = prb;
    return res;
}

/**Getter and setter functions for the above data structure **/

char *get_strn1(WORD_INFO *wi) {
    if((wi != NULL) && (wi->strn1 != NULL)) {
        return wi->strn1;
    }
    else {
        return NULL;
    }
}
/*Return code - 1 => successful, 0=> failed*/
int set_strn1(WORD_INFO *wi, char *str) {
    if((wi != NULL) && (wi->strn1 != NULL) && (str != NULL)) {
        strcpy(wi->strn1, str);
        return 1;
    }
    else {
        return 0;
    }
}

char *get_strn2(WORD_INFO *wi) {
    if((wi != NULL) && (wi->strn2 != NULL)) {
        return wi->strn2;
    }
    else {
        return NULL;
    }
}
/*Return code - 1 => successful, 0=> failed*/
int set_strn2(WORD_INFO *wi, char *str) {
    if((wi != NULL) && (wi->strn2 != NULL) && (str != NULL)) {
        strcpy(wi->strn2, str);
        return 1;
    }
    else {
        return 0;
    }
}

char *get_strn3(WORD_INFO *wi) {
    if((wi != NULL) && (wi->strn3 != NULL)) {
        return wi->strn3;
    }
    else {
        return NULL;
    }
}
/*Return code - 1 => successful, 0=> failed*/
int set_strn3(WORD_INFO *wi, char *str) {
    if((wi != NULL) && (wi->strn3 != NULL) && (str != NULL)) {
        strcpy(wi->strn3, str);
        return 1;
    }
    else {
        return 0;
    }
}
char *get_strn4(WORD_INFO *wi) {
    if((wi != NULL) && (wi->strn4 != NULL)) {
        return wi->strn4;
    }
    else {
        return NULL;
    }
}

int set_strn4(WORD_INFO *wi, char *str) {
    if((wi != NULL) && (wi->strn4 != NULL) && (str != NULL)) {
        strcpy(wi->strn4, str);
        return 1;
    }
    else {
        return 0;
    }
}

double get_prob(WORD_INFO *wi) {
    if(wi != NULL) {
        return wi->prob;
    }
    else {
        return -1;
    }
}

/*Return code - 1 => successful, 0=> failed*/
int set_prob(WORD_INFO *wi, double p) {
    if(wi != NULL) {
        wi->prob = p;
        return 1;
    }
    else {
        return 0;
    }
}

/**--------------------------------------------- Code to create a 2d array of WORD_INFO structs ---------------------------------------**/

/*Function to initialize the 2-d array, Takes the number of rows and the number of columns as parameter.*/
WORD_INFO ***create_array(int rows, int cols) {
    WORD_INFO ***arr = (WORD_INFO ***)malloc(rows * sizeof(WORD_INFO **));
    int i,j;
    for(i=0;i<rows;i++) {
        arr[i] = (WORD_INFO **)malloc(cols * sizeof(WORD_INFO *));
    }
    for (i=0;i<rows;i++) {
        for(j=0;j<cols;j++) {
            arr[i][j] = NULL;
        }
    }
    return arr;
}

/*Function to get the (i,j) th element of the 2-d array
Will return NULL if the place is NULL. */
WORD_INFO *get_element(WORD_INFO ***arr, int i, int j) {
    if(arr == NULL) {
        return NULL;
    }
    return arr[i][j];//By default an entry in the array is NULL
}

/*Function to set the (i,j)th element of the 2-d array
Will overwrite the location if it is already filled.
1=> successful, 0=> failed*/
int set_element(WORD_INFO ***arr, int i, int j, WORD_INFO *target) {
    if(arr == NULL) {
        return 0;
    }
    WORD_INFO *cur = arr[i][j];
    if( cur == NULL ){
        arr[i][j] = target;
    }
    else {
        arr[i][j] = target;
        free(cur);
    }
    return 1;
}

/*Function to delete an element in the array
1=>deletion successful, 0=>failed*/
int del_element(WORD_INFO ***arr, int i, int j) {
    if(arr == NULL) {
        return 0;
    }
    if(arr[i][j] == NULL) {
        return 1;
    }
    WORD_INFO *cur = arr[i][j];
    arr[i][j] = NULL;
    free(cur);
    return 1;
}
