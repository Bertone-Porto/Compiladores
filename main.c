#include <stdio.h>
#include <stdlib.h> // Para usar a função malloc

// NOME TOKENS
#define IF 256
#define THEN 257
#define ELSE 258
#define RELOP 259
#define ID 260
#define NUM 261

// ATRIBUTOS
#define LT 262
#define LE 263
#define EQ 264
#define NE 265
#define GT 266
#define GE 267

struct Token {
    int nome_token;
    int atributo;
};

int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;

char *readFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    int n = 0;
    int c;

    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    code = (char *)malloc(f_size + 1); // Alocação dinâmica em C

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }
    code[n] = '\0';
    fclose(file); // Feche o arquivo após a leitura
    return code;
}

int falhar() {
    switch (estado) {
    case 0: partida = 9; break;
    case 9: partida = 12; break;
    case 12: partida = 20; break;
    case 20: partida = 25; break;
    case 25:
        // retornar mensagem de erro
        printf("Erro encontrado no código\n");
        cont_sim_lido++;
        break;
    default:
        printf("Erro do compilador\n");
    }
    return partida;
}

struct Token proximo_token() {
    struct Token token;
    char c;

    while (code[cont_sim_lido] != '\0') {
        switch (estado) {
        case 0:
            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
                estado = 0;
                cont_sim_lido++;
            } else if (c == '<') estado = 1;
            else if (c == '=') estado = 5;
            else if (c == '>') estado = 6;
            else {
                estado = falhar();
            }
            break;

        case 1:
            cont_sim_lido++;
            c = code[cont_sim_lido];

            if ((c == ' ') || (c == '\n')) {
                cont_sim_lido++;
                c = code[cont_sim_lido];
            }

            if (c == '=') estado = 2;
            else if (c == '>') estado = 3;
            else estado = 4;
            break;

        case 2:
            cont_sim_lido++;
            printf("<relop, LE>\n");
            token.nome_token = RELOP;
            token.atributo = LE;
            estado = 0;
            return token;
            break;

        case 3:
            cont_sim_lido++;
            printf("<relop, NE>\n");
            token.nome_token = RELOP;
            token.atributo = NE;
            estado = 0;
            return token;
            break;

        case 4:
            //cont_sim_lido++;
            printf("<relop, LT>\n");
            token.nome_token = RELOP;
            token.atributo = LT;
            estado = 0;
            return token;
            break;

        case 5:
            cont_sim_lido++;
            printf("<relop, EQ>\n");
            token.nome_token = RELOP;
            token.atributo = EQ;
            estado = 0;
            return token;
            break;

        case 6:
            cont_sim_lido++;
            c = code[cont_sim_lido];

            if ((c == ' ') || (c == '\n')) {
                cont_sim_lido++;
                c = code[cont_sim_lido];
            }

            if (c == '=') estado = 7;
            else estado = 8;
            break;

        case 7:
            cont_sim_lido++;
            printf("<relop, GE>\n");
            token.nome_token = RELOP;
            token.atributo = GE;
            estado = 0;
            return token;
            break;

        case 8:
            //cont_sim_lido++;
            printf("<relop, GT>\n");
            token.nome_token = RELOP;
            token.atributo = GT;
            // ATENÇÃO - CORREÇÃO: foi acrescentado o comando "estado = 0;"
            estado = 0;
            return token;
            break;

        case 9:
            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
                estado = 0;
                cont_sim_lido++;
            } else {
				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
					estado = 10; // Ir para o estado de reconhecimento de identificadores
				}
                /*implementar ações referentes aos estado */
                estado = falhar();
            }

            break;

        /*implementar ações para os estados 10, 11, 12*/
		case 10:
		// Reconhecimento de identificadores
		while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
		    // Continue lendo os caracteres do identificador
		    // Atualize o token conforme necessário
		    // Atualize o estado se necessário
		    // ...
		}
        case 12:
            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
                estado = 0;
                cont_sim_lido++;
            } else {
                /*implementar ações referentes aos estado */
                estado = falhar();
            }
            break;

        /*implementar ações para os estados 13-19*/

        case 20:
            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
                estado = 0;
                cont_sim_lido++;
            } else {
                /*implementar ações referentes aos estado */
                estado = falhar();
            }
            break;

        /*implementar ações para os estados 21-24*/

        case 25:
            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
                estado = 0;
                cont_sim_lido++;
            } else {
                /*implementar ações referentes aos estado */
                estado = falhar();
                token.nome_token = -1;
                token.atributo = -1;
                return token;
            }
            break;
        }
    }
    token.nome_token = EOF;
    token.atributo = -1;
    return token;
}

int main() {
    struct Token token;
    code = readFile("programa.txt");
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    token = proximo_token();
    //...

    free(code); // Liberar a memória alocada
    return 0;
}