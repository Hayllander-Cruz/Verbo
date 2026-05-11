#include <iostream>
#include <string>
#include <vector>

// Definição dos componentes da linguagem Verbo
enum Token {
    TOK_FIM = -1,
    TOK_FUNCAO = -2,      // comando 'funcao'
    TOK_DEFINIR = -3,    // comando 'definir'
    TOK_EXIBIR = -4,     // comando 'Exibir'
    TOK_IDENTIFICADOR = -5,
    TOK_NUMERO = -6
};

std::string ValorTexto;

// Função que lê o código e identifica o que cada palavra é
int proximo_token() {
    static int ultimo_caractere = ' ';

    // Pular espaços
    while (isspace(ultimo_caractere))
        ultimo_caractere = getchar();

    // Identificar palavras (funcao, definir, etc)
    if (isalpha(ultimo_caractere)) {
        ValorTexto = ultimo_caractere;
        while (isalnum((ultimo_caractere = getchar())))
            ValorTexto += ultimo_caractere;

        if (ValorTexto == "funcao") return TOK_FUNCAO;
        if (ValorTexto == "definir") return TOK_DEFINIR;
        if (ValorTexto == "Exibir") return TOK_EXIBIR;
        return TOK_IDENTIFICADOR;
    }

    if (ultimo_caractere == EOF) return TOK_FIM;

    int caractere_atual = ultimo_caractere;
    ultimo_caractere = getchar();
    return caractere_atual;
}

int main() {
    std::cout << "--- Interpretador Logico Verbo v0.1 ---" << std::endl;
    std::cout << "Escreva um comando (ex: definir nota = 10):" << std::endl;

    while (true) {
        int token = proximo_token();
        if (token == TOK_FIM) break;

        if (token == TOK_FUNCAO) std::cout << "[Token] Palavra-chave: funcao" << std::endl;
        else if (token == TOK_DEFINIR) std::cout << "[Token] Comando: definir" << std::endl;
        else if (token == TOK_EXIBIR) std::cout << "[Token] Acao: Exibir" << std::endl;
        else if (token == TOK_IDENTIFICADOR) std::cout << "[Token] Nome criado: " << ValorTexto << std::endl;
    }
    return 0;
}
