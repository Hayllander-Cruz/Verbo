#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

// --- 1. O LEXER (Scanner Profissional) ---
enum class TipoToken {
    Fim = -1,
    Funcao = -2,
    Definir = -3,
    Exibir = -4,
    Identificador = -5,
    Numero = -6,
    Simbolo = 0
};

class Lexer {
public:
    static std::string NomeIdentificador;
    static double ValorNumerico;

    static TipoToken obterToken() {
        static int ultimoCaractere = ' ';

        while (isspace(ultimoCaractere))
            ultimoCaractere = getchar();

        if (isalpha(ultimoCaractere)) {
            NomeIdentificador = ultimoCaractere;
            while (isalnum((ultimoCaractere = getchar())))
                NomeIdentificador += ultimoCaractere;

            if (NomeIdentificador == "funcao")  return TipoToken::Funcao;
            if (NomeIdentificador == "definir") return TipoToken::Definir;
            if (NomeIdentificador == "Exibir")  return TipoToken::Exibir;
            return TipoToken::Identificador;
        }

        if (isdigit(ultimoCaractere) || ultimoCaractere == '.') {
            std::string strNum;
            do {
                strNum += ultimoCaractere;
                ultimoCaractere = getchar();
            } while (isdigit(ultimoCaractere) || ultimoCaractere == '.');

            ValorNumerico = strtod(strNum.c_str(), nullptr);
            return TipoToken::Numero;
        }

        if (ultimoCaractere == EOF) return TipoToken::Fim;

        int caractereAtual = ultimoCaractere;
        ultimoCaractere = getchar();
        return (TipoToken)caractereAtual;
    }
};

std::string Lexer::NomeIdentificador;
double Lexer::ValorNumerico;

// --- 2. A AST (Árvore Sintática Abstrata) ---
// Isso permite que o compilador "entenda" a estrutura antes de gerar binário
class ExpressaoAST {
public:
    virtual ~ExpressaoAST() = default;
};

// Representa um número na Verbo
class NumeroExpressaoAST : public ExpressaoAST {
    double Val;
public:
    NumeroExpressaoAST(double Val) : Val(Val) {}
};

// --- 3. O PARSER (O Cérebro Gramatical) ---
class Parser {
    TipoToken TokenAtual;

    void proximo() { TokenAtual = Lexer::obterToken(); }

    // Log de Erro Profissional
    std::unique_ptr<ExpressaoAST> LogErro(const char* mensagem) {
        fprintf(stderr, "ERRO SINTATICO: %s\n", mensagem);
        return nullptr;
    }

public:
    void Analisar() {
        proximo();
        while (true) {
            switch (TokenAtual) {
            case TipoToken::Fim:
                return;
            case TipoToken::Definir:
                TratarDefinicao();
                break;
            default:
                proximo();
                break;
            }
        }
    }

    // Regra: definir [nome] = [valor]
    void TratarDefinicao() {
        proximo(); // pula 'definir'
        if (TokenAtual != TipoToken::Identificador) {
            LogErro("Esperado um nome após 'definir'");
            return;
        }

        std::string nomeVar = Lexer::NomeIdentificador;
        proximo(); // pula nome

        if ((char)TokenAtual != '=') {
            LogErro("Esperado '=' após o nome da variável");
            return;
        }

        proximo(); // pula '='
        if (TokenAtual != TipoToken::Numero) {
            LogErro("Esperado um valor numérico");
            return;
        }

        std::cout << ">>> SUCESSO: Variavel '" << nomeVar << "' definida com o valor " << Lexer::ValorNumerico << std::endl;
        proximo();
    }
};

// --- 4. EXECUÇÃO ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    std::cout << "--- COMPILADOR VERBO (MODO EXPERT) ---" << std::endl;
    std::cout << "Aguardando entrada logica..." << std::endl;

    Parser projetoVerbo;
    projetoVerbo.Analisar();

    return 0;
}
