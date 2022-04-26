//
// Created by Matias Freitas Guimarães on 26/04/2022.
//

#ifndef SOWORK_PHRASES_H
#define SOWORK_PHRASES_H

/**
 * função para ler o ficheiro de texto para uma matriz
 * uma linha para cada frase
 */
char[][] readFile(FILE* file);
/**
 * função main para correr o comando
 *
 * @param argc número de argumentos fornecidos
 * @param argv array de chars com os argumentos fornecidos
 * @return sinal de fim de execução
 */
int main(int argc, char* argv[]);

#endif //SOWORK_PHRASES_H
