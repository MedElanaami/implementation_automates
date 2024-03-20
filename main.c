#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//prototype des fonctions
#include "file.h"
// Déclaration des états
 enum etat {
    /* Mot clef */
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,

    /* OPREL */
    B0,
    B1,
    B2,
    B3,


    /* ID */
    C0,
    C1,

    /* NB */
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,

    FIN
};

int line=1;

/* Fonction ECHEC */
void echec(enum etat * s,int c) {
    if(isalpha(c)) {
        *s = C0;
    }
    else if(isdigit(c)) {
        *s = D0;
    }
    else if(c == '<' || c == '>' || c == '=') {
        *s = B0;
    }
    else if(c == ' ' || c == '\t' || c == '\n') {
        *s = FIN;
    }
    else {
        printf("Erreur lexicale ligne %d\n", line);
        exit(1);
    }
}
/* Fonction step */
void step(enum etat *s, int c, FILE *file,FILE * fileResult) {
    switch (*s) {
        case A0:
            if (c == 's' || c == 'S') {
                *s = A1;
            }
            else if (c == 'a' || c == 'A') {
                *s = A6;
            }
            else {
                echec(s, c);
            }
            break;
        case A1:
            if (c== 'i' || c == 'I') {
                *s = A2;
            }
            else {
                 echec(s, c);
            }
            break;
        case A2:
            if (c == 'n' || c == 'N') {
                *s = A3;
            }
            else if(c == ' ' || c == '\t' ){
                fprintf(fileResult, " si");
                echec(s, c);
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case A3:
            if(c == 'o' || c == 'O'){
                *s = A4;
            }
            else {
                 echec(s, c);
            }
            break;
        case A4:
            if(c == 'n' || c == 'N'){
                *s = A5;
            }
            else {
                 echec(s, c);
            }
            break;
        case A5:
            if(c == ' ' || c == '\t' || c == '\n'){

                fprintf(fileResult, " sinon");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case A6:
            if(c == 'l' || c == 'L'){
                *s = A7;
            }
            else {
                 echec(s, c);
            }
            break;
        case A7:
            if(c == 'o' || c == 'O'){
                *s = A8;
            }
            else {
                 echec(s, c);
            }
            break;
        case A8:
            if(c == 'r' || c == 'R'){
                *s = A9;
            }
            else {
                 echec(s, c);
            }
            break;
        case A9:
            if(c == 's' || c == 'S'){
                *s = A10;
            }
            else {
                 echec(s, c);
            }
            break;
        case A10:
            if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " alors");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;

        case B0:
            if(c == '<'){
                *s = B1;
            }
            else if(c == '>'){
                *s = B2;
            }
            else if(c == '='){
                *s = B3;
            }
            else {
                 echec(s, c);
            }
            break;
        case B1:
            if(c == '=' || c == '>'){
                *s = B3;
            }
            else {
                 echec(s, c);
            }
            break;
        case B2:
            if(c == '='){
                *s = B3;
            }
            else {
                 echec(s, c);
            }
            break;
        case B3:
            if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " OPREL");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;

        case C0:
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                *s = C1;
            }
            else {
                 echec(s, c);
            }
            break;
        case C1:
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
                *s = C1;
            }
            else if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " ID");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;

        case D0:
            if(c >= '0' && c <= '9'){
                *s = D1;
            }
            else {
                 echec(s, c);
            }
            break;
        case D1:
            if(c >= '0' && c <= '9'){
                *s = D1;
            }
            else if(c == '.'){
                *s = D2;
            }
            else if(c == 'E' || c == 'e'){
                *s = D3;
            }
            else if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " NB");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case D2:
            if(c >= '0' && c <= '9'){
                *s = D4;
            }
            else {
                 echec(s, c);
            }
            break;
        case D3:
            if(c == '+' || c == '-'){
                *s = D6;
            }
            else if(c >= '0' && c <= '9'){
                *s = D5;
            }
            else {
                 echec(s, c);
            }
            break;
        case D4:
            if(c >= '0' && c <= '9'){
                *s = D4;
            }
            else if(c == 'E' || c == 'e'){
                *s = D3;
            }
            else if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " NB");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case D5:
            if(c >= '0' && c <= '9'){
                *s = D5;
            }
            else if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " NB");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case D6:
            if(c >= '0' && c <= '9'){
                *s = D5;
            }
            else if(c == ' ' || c == '\t' || c == '\n'){
                fprintf(fileResult, " NB");
                line++;
            }
            else {
                 echec(s, c);
            }
            break;
        case FIN:
            break;
    }
}


int main() {

    FILE * fileSource= fopen("/home/med/ILISI/compilation/implementation_automates/source.txt", "r");
    if (fileSource == NULL) {
        printf("Impossible d'ouvrir le fichier source.txt\n");
        return 1;
    }
    FILE * fileResult= fopen("/home/med/ILISI/compilation/implementation_automates/result.txt", "w");
    if (fileResult == NULL) {
        printf("Impossible d'ouvrir le fichier result.txt\n");
        return 1;
    }
    int carr;
    enum etat s = A0;
    while ((carr= fgetc(fileSource)) != EOF){
//        carr= fgetc(fileSource);
        step(&s, carr, fileSource,fileResult);
    }
    fclose(fileSource);
    fclose(fileResult);

    return 0;
}
