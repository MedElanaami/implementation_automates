#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define Source "/home/med/ILISI/compilation/implementation_automates/files/source.txt"
#define Result "/home/med/ILISI/compilation/implementation_automates/files/result.txt"

typedef enum
{
    SI,
    ALORS,
    SINON,
    NBINT,
    NBFLOAT,
    ID,
    OPRELINF,
    OPRELSUP,
    OPRELINFEGAL,
    OPRELSUPEGAL,
    OPRELEGAL,
    OPRELDIFF,
    FIN,
    ERROR
} token;

//Les automates
typedef enum {
    A,
    A0, // S || A
    A1, // S
    A2, // SI
    A3,
    A4,
    A5, // SINON
    A06,
    A6, // A
    A7,
    A8,
    A9,
    A10,//ALORS
    //OPERATEURS
    B0, // < || > || =
    B1, // <
    B2, // >
    B3, // =
    B4, // <>
    B5, // <=
    B6, // >=

    // ID
    C0, // lettre
    C1, // lettre || chiffre
    //NB
    D0, // chiffre
    D1, // chiffre
    D2, // .
    D3, // E
    D4, // chiffre
    D5, // chiffre
    D6, // + || -

}Automate;

Automate etat = A0;
Automate etatInitial = A0;
int line = 1;


void retourner(int nbrCar, FILE *file)
{
    /*
     * Parameters
        pointer: It is the pointer to a FILE object that identifies the stream.
        offset: It is the number of bytes to offset from the position
        position: It is the position from where the offset is added. Position defines the point with respect to which the file pointer needs to be moved. It has three values:
        SEEK_END: It denotes the end of the file.
        SEEK_SET: It denotes etatInitialing of the file.
        SEEK_CUR: It denotes the file pointer’s current position.
     */
    if (!feof(file)) fseek(file, -nbrCar, SEEK_CUR);
    else fseek(file, -nbrCar + 1, SEEK_CUR);
}

Automate echec(int nbrCar, FILE *file)
{
    //
    retourner(nbrCar, file);
    switch (etatInitial)
    {
        case A0:
            etatInitial = A06;
            break;
        case A06:
            etatInitial = B0;
            break;
        case B0:
            etatInitial = C0;
            break;
        case C0:
            etatInitial = D0;
            break;
        case D0:
            etatInitial = A0;
            return A;
        default: printf("\nSomething else!!");
    }
    return etatInitial;
}

token next_token(FILE *file)
{
    if (!file)
    {
        printf("\nError: file not existe!!"); exit(0);
    }
    int c;
    while (1)
    {
        c = fgetc(file);
        switch (etat)
        {
            case A:
                if (feof(file)) return FIN;
                printf("\nErreur dans ligne %d :\"%c\"", line, c);
                return ERROR;
            case A0:
                if (feof(file)) return FIN;
                while (isspace(c))
                {
                    if (c == '\n') line++;
                    c = fgetc(file);
                }
                if (c == 's') etat = A1;
                else etat = echec(1, file);
                break;
            case A1:
                if (c == 'i') etat = A2;
                else etat = echec(2, file);
                break;
            case A2:
                if (c == 'n') etat = A3;
                else if (isspace(c) || feof(file))
                {
                    retourner(1, file);
                    return SI;
                }
                else etat = echec(3, file);
                break;
            case A3:
                if (c == 'o') etat = A4;
                else etat = echec(4, file);
                break;
            case A4:
                if (c == 'n') etat = A5;
                else etat = echec(5, file);
                break;
            case A5:
                if (isspace(c) || feof(file))
                {
                    retourner(1, file);
                    return SINON;
                }
                else etat = echec(6, file);
                break;
            case A06:
                if (c == 'a') etat = A6;
                else etat = echec(1, file);
                break;
            case A6:
                if (c == 'l') etat = A7;
                else etat = echec(2, file);
                break;
            case A7:
                if (c == 'o') etat = A8;
                else etat = echec(3, file);
                break;
            case A8:
                if (c == 'r') etat = A9;
                else etat = echec(4, file);
                break;
            case A9:
                if (c == 's') etat = A10;
                else etat = echec(5, file);
                break;
            case A10:
                if (isspace(c) || feof(file))
                {
                    retourner(1, file);
                    return ALORS;
                }
                else etat = echec(6, file);
                break;
            case B0:
                if (c == '<') etat = B1;
                else if (c == '>') etat = B2;
                else if (c == '=') etat = B3;
                else etat = echec(1, file);
                break;
            case B1:
                if (c == '>') etat = B4;
                else if (c == '=') etat = B5;
                else
                {
                    retourner(1, file);
                    return OPRELINF;
                }
                break;
            case B2:
                if (c == '=') etat = B6;
                else
                {
                    retourner(1, file);
                    return OPRELSUP;
                }
                break;
            case B3:
                retourner(1, file);
                return OPRELEGAL;
            case B4:
                retourner(1, file);
                return OPRELDIFF;
            case B5:
                retourner(1, file);
                return OPRELINFEGAL;
            case B6:
                retourner(1, file);
                return OPRELSUPEGAL;
            case C0:
                if (!feof(file) && isalpha(c)) etat = C1;
                else etat = echec(1, file);
                break;
            case C1:
                if (!feof(file) && isalnum(c)) etat = C1;
                else
                {
                    retourner(1, file);
                    return ID;
                }
                break;
            case D0:
                if (isdigit(c)) etat = D1;
                else etat = echec(1, file);
                break;
            case D1:
                if (!feof(file) && isdigit(c)) etat = D1;
                else if (c == 'E' || c == 'e') etat = D3;
                else if (c == '.') etat = D2;
                else
                {
                    retourner(1, file);
                    return NBINT;
                }
                break;
            case D3:
                if (!feof(file) && isdigit(c)) etat = D5;
                else if (c == '+' || c == '-') etat = D6;
                else
                {
                    retourner(2, file);
                    return NBINT;
                }
                break;
            case D2:
                if (!feof(file) && isdigit(c)) etat = D4;
                else
                {
                    retourner(1, file);
                    return NBINT;
                }
                break;
            case D5:
                if (!feof(file) && isdigit(c)) etat = D5;
                else
                {
                    retourner(1, file);
                    return NBFLOAT;
                }
                break;
            case D6:
                if (!feof(file) && isdigit(c)) etat = D5;
                else
                {
                    retourner(3, file);
                    return NBINT;
                }
                break;
            case D4:
                if (!feof(file) && isdigit(c)) etat = D4;
                else if (c == 'E' || c == 'e') etat = D3;
                else
                {
                    retourner(1, file);
                    return NBFLOAT;
                }
                break;
            default:
                printf("\nSomething else");
        }
    }
}

void transferToken(token tk,FILE *file)
{
    if ( tk!=ERROR) {
        switch (tk) {
            case SI:
                fprintf(file, "SI ");
                break;
            case ALORS:
                fprintf(file, "ALORS \n\t"); break;
            case SINON:
                fprintf(file, "SINON \n\t"); break;
            case OPRELINF:
                fprintf(file, "OPRELINF "); break;
            case OPRELINFEGAL:
                fprintf(file, "OPRELINFEGAL "); break;
            case OPRELSUP:
                fprintf(file, "OPRELSUP "); break;
            case OPRELSUPEGAL:
                fprintf(file, "OPRELSUPEGAL "); break;
            case OPRELEGAL:
                fprintf(file, "OPRELEGAL "); break;
            case OPRELDIFF:
                fprintf(file, "OPRELDIFF "); break;
            case ID:
                fprintf(file, "ID "); break;
            case NBINT:
                fprintf(file, "NBINT "); break;
            case NBFLOAT:
                fprintf(file, "NBFLOAT "); break;

            default:
                printf("\nErreur");
                exit(0);
        }
    }
}

FILE *Open_File(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        printf("\nErreur lors de l'ouverture de fichier %s", filename);
        exit(0);
    }
    return ((FILE *)file);
}

int main()
{
    FILE *fichierSource = Open_File(Source, "r");
    FILE *ficherRes = Open_File(Result, "w");
    token tk;
    line = 1;
    while ((tk = next_token(fichierSource)) != FIN)
    {
        etatInitial = A0; //start
        etat = A0; // state
        transferToken(tk,ficherRes);
    }
    fprintf(ficherRes, "FIN");
    fclose(fichierSource);
    fclose(ficherRes);
    return 0;
}
