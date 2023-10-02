#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define ID 				256
#define KEYWORD 		257
#define INT 			258
#define STRING_LITERAL 	259
#define COMENTARIO 		260
#define ESPACO 			261

//erificar se uma palavra é uma palavra-chave
int isKeyword(char *word) {
    char *keywords[] = {
        "and", "break", "do", "else", "elseif",
        "end", "false", "for", "function", "if",
        "in", "local", "nil", "not", "or",
        "repeat", "return", "then", "true", "until", "while"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1; //é uma palavra-chave
        }
    }
    return 0; //não é uma palavra-chave
}


void lexer(char *code) {
    int state = 0; 
    char lexema[100];
    int lexemaIndex = 0;

    for (int i = 0; code[i] != '\0'; i++) {
        char c = code[i];

        switch (state) {
            case 0:
                if (isspace(c)) {
                    //espaço em branco, continue no estado 0
                    state = 0;
                } else if (c == '"') {
                    //início de um literal de string, vá para o estado 1
                    state = 1;
                    lexema[lexemaIndex++] = c;
                } else if (c == '-' && code[i + 1] == '-') {
                    //início de um comentário, vá para o estado 2
                    state = 2;
                    lexema[lexemaIndex++] = c;
                    lexema[lexemaIndex++] = code[++i];
                } else if (isalpha(c) || c == '_') {
                    //início de um identificador, vá para o estado 3
                    state = 3;
                    lexema[lexemaIndex++] = c;
                } else if (isdigit(c)) {
                    //início de um número inteiro, vá para o estado 4
                    state = 4;
                    lexema[lexemaIndex++] = c;
                } else {
                    //outros caracteres, continue no estado 0
                    state = 0;
                }
                break;

            case 1:
                //dentro de um literal de string
                lexema[lexemaIndex++] = c;
                if (c == '"') {
                    //fim do literal de string, retorne STRING_LITERAL
                    lexema[lexemaIndex] = '\0';
                    printf("<STRING_LITERAL, %s>\n", lexema);
                    lexemaIndex = 0;
                    state = 0;
                }
                break;

            case 2:
                //dentro de um comentário
                lexema[lexemaIndex++] = c;
                if (c == '\n') {
                    //fim do comentário de linha única, retorne COMENTÁRIO
                    lexema[lexemaIndex] = '\0';
                    printf("<COMENTARIO, %s>\n", lexema);
                    lexemaIndex = 0;
                    state = 0;
                }
                break;

            case 3:
                //identificador
                if (isalnum(c) || c == '_') {
                    lexema[lexemaIndex++] = c;
                } else {
                    lexema[lexemaIndex] = '\0';
                    if (isKeyword(lexema)) {
                        //é uma palavra-chave, retorne KEYWORD
                        printf("<KEYWORD, %s>\n", lexema);
                    } else {
                        //é um identificador, retorne ID
                        printf("<ID, %s>\n", lexema);
                    }
                    lexemaIndex = 0;
                    state = 0;
                    i--; //volte 1 caractere para reanalisar o caractere atual
                }
                break;

            case 4:
                //dentro de um número inteiro
                if (isdigit(c)) {
                    lexema[lexemaIndex++] = c;
                } else {
                    lexema[lexemaIndex] = '\0';
                    //é um número inteiro, retorne INT
                    printf("<INT, %s>\n", lexema);
                    lexemaIndex = 0;
                    state = 0;
                    i--; //volte 1 caractere para reanalisar o caractere atual
                }
                break;
        }
    }
}

int main() {
    FILE *file;
    char fileName[] = "programa.txt";
    char *code = NULL;
    long length;

    file = fopen(fileName, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        code = (char *)malloc(length + 1);
        if (code) {
            fread(code, 1, length, file);
            code[length] = '\0';
            fclose(file);
        }
    }

    if (code) {
        lexer(code);
        free(code);
    } else {
        printf("Falha ao abrir o arquivo.\n");
    }

    return 0;
}
