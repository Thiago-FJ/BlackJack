#include <iostream>
#include <limits>   //numeric limits
#include <locale.h> // português
#include <random>   //valores aleatórioa
#include <unistd.h> //wait

using namespace std;

// Inicializando o gerador de números aleatórios isso tem que ser aqui pq usa
// dentro das funções eu acho
random_device rd;  // Obtem um número aleatório do hardware
mt19937 gen(rd()); // Inicializa o gerador Mersenne Twister
uniform_int_distribution<> range(1, 13); // [0, 13]

int escolha = 0;
// Menu
//  Função para imprimir o logo inicial
void iLogo() {
  cout << R"(
                             =+==%@@@@@-=+=                             
                         .*:     %@@@@%     :#.                         
                       .#  +#   :%%**%@:   #*  *:                       
                      @     :-  :#@@@@#:  -:     @.                     
                    :@@@*.=..%@@@@@@@@@@@@%..=.+@@@:                    
                   .@@@@@+ %@@@@@@+  +@@@@@@% +@@@@@.                   
                   *-%@@+.@@@@@@-      +@@@@@@.=@@@=*                   
                  ::   - @@@@@@@        @@@@@@@ -   ::                  
                  *   .%@@@@@@@*        *@@@@@@#@    +                  
                  % #+.@#@@@@.            .@@@@+@. @ #                  
                  *    %@@@@%   BLACKJACK  %@@@#@    +                  
                  ::   :.@@@@.             @@@@ :   ::                  
                   *-@@@=.@@@@%          #@@@@.=@@%-*                   
                   .@@@@@+ %@@@@@@-  -@@@@@@@ +@@@@@.                   
                    :@@@*.-..%@@@@@@@@@@@@%..=.*@@@:                    
                     .@        .:#@@@@#-.        @.                     
                       :#  -%   :%%**%@:   @=  *:                       
                         .#:     %@@@@%     :#.                         
                             =+--%@@@@@--++                             
    )" << endl;
}

struct Baralho {
  struct A {
    int qnt = 4;
    int valor = 11;
    string img = "| A♠|";
  } A; // instância da struct A para permitir aninhamento de estruturas

  struct Dois {
    int qnt = 4;
    int valor = 2;
    string img = "| 2♣|";
  } Dois;

  struct Tres {
    int qnt = 4;
    int valor = 3;
    string img = "| 3♣|";
  } Tres;

  struct Quatro {
    int qnt = 4;
    int valor = 4;
    string img = "| 4♠|";
  } Quatro;

  struct Cinco {
    int qnt = 4;
    int valor = 5;
    string img = "| 5♠|";
  } Cinco;

  struct Seis {
    int qnt = 4;
    int valor = 6;
    string img = "| 6♦|";
  } Seis;

  struct Sete {
    int qnt = 4;
    int valor = 7;
    string img = "| 7♦|";
  } Sete;

  struct Oito {
    int qnt = 4;
    int valor = 8;
    string img = "| 8♥|";
  } Oito;

  struct Nove {
    int qnt = 4;
    int valor = 9;
    string img = "| 9♥|";
  } Nove;

  struct Dez {
    int qnt = 4;
    int valor = 10;
    string img = "|10♠|";
  } Dez;

  struct J {
    int qnt = 4;
    int valor = 10;
    string img = "| J♣|";
  } J;

  struct Q {
    int qnt = 4;
    int valor = 10;
    string img = "| Q♦|";
  } Q;

  struct K {
    int qnt = 4;
    int valor = 10;
    string img = "| K♥|";
  } K;

}; // Criando um baralho global pra ser usado em todas as funções
Baralho baralho; // cria o strut do baralho

struct Jogador {
  int dinheiro = 700;
  int val = 0; // valor da mão
  int aposta = 0;
  string cartas;
};
Jogador jogador;
Jogador dealer;

void aposta() {
  cout << endl
       << "--------------------------------------------------------------------"
          "--------"
       << endl;
  cout << "Banca: R$" << jogador.dinheiro << endl;
  cout << "Quanto quer apostar? ";
  cout << endl
       << "--------------------------------------------------------------------"
          "--------"
       << endl;

  cin >> jogador.aposta;
  while (cin.fail() || jogador.aposta > jogador.dinheiro ||
         jogador.aposta <= 0) {
    cin.clear(); // Clear error flags
    cin.ignore(numeric_limits<streamsize>::max(),
               '\n'); // Discard invalid input
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "Aposta inválida. Insira um valor entre 1 e " << jogador.dinheiro
         << ": ";
    cin >> jogador.aposta;
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
  }
}

bool jogo = true; // mantém o jogo rodando
bool menu(int i) {
  char iniciar = '\0';
  if (i == 1) {
    iLogo();
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "\t\t\t\t\tBem-vindo ao jogo!\n\t\t\tTente alcançar 21 pontos sem "
            "ultrapassar, derrotando o dealer e vencendo a "
            "partida!\n\t\t\tVamos começar? [S] Sim\t[N] Não\t: ";
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cin >> iniciar;
  } else if (i == 2) {
    if (jogador.dinheiro < 0) {
      cout << "Você não tem dinheiro suficiente para continuar apostando! A "
              "casa sempre vence mais sorte na próxima"
           << endl;
      exit(0);
    }
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "\nQuer tentar sua sorte de novo ou vai fugir da derrota? [S] "
            "Sim\t[N] Não\t: ";
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cin >> iniciar;
  }

  switch (toupper(iniciar)) { // Transforma para maiúsculo
  case 'S':
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "\nVamos começar então!\n";
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    sleep(1);
    baralho = Baralho(); //"repõe" as cartas no baralho
    jogador.cartas = "";
    dealer.cartas = ""; // reinicia as cartas do jogador e do dealer
    jogador.val = 0;
    dealer.val = 0; // reinicia o valor das cartas do jogador e do dealer
    cout << "\033[2J\033[1;1H";
    // system("cls");
    sleep(1);
    return true; // Jogador quer continuar
  case 'N':
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "\nAh, desistiu, hein? Parece que 21 pontos não eram tão atraentes "
            "assim! Da próxima vez, quem sabe você tenta vencer o dealer de "
            "verdade!\n";
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "Banca Final: R$" << jogador.dinheiro << "\nLucro obtido: R$"
         << jogador.dinheiro - 700 << "\n";
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    return false; // Jogador quer parar
  default:
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << "\nOpção inválida. Parece que Blackjack não é para você. Da "
            "próxima vez, quem sabe você tenta vencer o dealer de verdade!\n";
    if (jogador.dinheiro - 700 > 0) {
      cout << "Banca Final: R$" << jogador.dinheiro << "\n Prejuizo obtido: R$"
           << jogador.dinheiro - 700 << "\n";
      return false;
    } else {
      cout << "Banca Final: R$" << jogador.dinheiro << "\n Lucro obtido: R$"
           << jogador.dinheiro - 700 << "\n";
      cout << endl
           << "----------------------------------------------------------------"
              "------------"
           << endl;
      return false; // Escolha inválida, assume como "não"
    }
  }
}
// padrão de inicio de jogo
//--------------------------------------------------

int valCarta(int carta) {
  switch (carta) {
  case 1:
    return baralho.A.valor;
  case 2:
    return baralho.Dois.valor;
  case 3:
    return baralho.Tres.valor;
  case 4:
    return baralho.Quatro.valor;
  case 5:
    return baralho.Cinco.valor;
  case 6:
    return baralho.Seis.valor;
  case 7:
    return baralho.Sete.valor;
  case 8:
    return baralho.Oito.valor;
  case 9:
    return baralho.Nove.valor;
  case 10:
    return baralho.Dez.valor;
    break;
  case 11: // J
    return baralho.J.valor;
  case 12: // Q
    return baralho.Q.valor;
  case 13: // K
    return baralho.K.valor;

  default:
    cout << "Alguma coisa deu merda 1"
         << endl; // quando tiver tudo funcionando apagar isso
    break;
  }
  return 0;
} // Para pegar o valor de cada carta

string imgCarta(int carta) {
  switch (carta) {
  case 1:
    return baralho.A.img;
  case 2:
    return baralho.Dois.img;
  case 3:
    return baralho.Tres.img;
  case 4:
    return baralho.Quatro.img;
  case 5:
    return baralho.Cinco.img;
  case 6:
    return baralho.Seis.img;
  case 7:
    return baralho.Sete.img;
  case 8:
    return baralho.Oito.img;
  case 9:
    return baralho.Nove.img;
  case 10:
    return baralho.Dez.img;
    break;
  case 11: // J
    return baralho.J.img;
  case 12: // Q
    return baralho.Q.img;
  case 13: // K
    return baralho.K.img;
    break;

  default:
    cout << "Alguma coisa deu merda 2"
         << endl; // quando tiver tudo funcionando apagar isso
    break;
  }
  return 0;
} // Função para o símbolo de cada carta e armazenar num vetor

int attCarta(int carta) {
  switch (carta) {
  case 1:
    baralho.A.qnt -= 1;
    break;
  case 2:
    baralho.Dois.qnt -= 1;
    break;
  case 3:
    baralho.Tres.qnt -= 1;
    break;
  case 4:
    baralho.Quatro.qnt -= 1;
    break;
  case 5:
    baralho.Cinco.qnt -= 1;
    break;
  case 6:
    baralho.Seis.qnt -= 1;
    break;
  case 7:
    baralho.Sete.qnt -= 1;
    break;
  case 8:
    baralho.Oito.qnt -= 1;
    break;
  case 9:
    baralho.Nove.qnt -= 1;
    break;
  case 10:
    baralho.Dez.qnt -= 1;
    break;
  case 11: // J
    baralho.J.qnt -= 1;
    break;
  case 12: // Q
    baralho.Q.qnt -= 1;
    break;
  case 13: // K
    baralho.K.qnt -= 1;
    break;
  default:
    cout << "Erro: valor inválido de carta (" << carta << ")\n";
    return -1; // Retorna um erro para indicar que algo deu errado
  }
  return 0;
} // Função para atualizar a quantidade de cada carta no baralho

void fJogador() {

  int reg;
  // Jogador pega duas cartas iniciais
  for (int i = 0; i < 2; i++) {
    reg = range(gen);
    jogador.val += valCarta(reg);
    jogador.cartas = jogador.cartas + " " + imgCarta(reg);
    attCarta(reg);
  }

  cout << "Mão do jogador: " << jogador.cartas << "\nValor: " << jogador.val
       << "\n";
}

void fDealer() {
  int reg;
  // Dealer pega duas cartas iniciais
  for (int i = 0; i < 2; i++) {
    reg = range(gen);
    dealer.val += valCarta(reg);
    dealer.cartas = dealer.cartas + " " + imgCarta(reg);
    attCarta(reg);
  }
  cout << "Mão do dealer ";
  for (int i = 0; i < 8; i++) {
    cout << dealer.cartas[i];
  }
  cout << " |###|\n";
}

void jDealer() {
  int reg;
  // Checa por um possível blackjack
  if (dealer.val == 21) {
    if (jogador.val == 21) {
      cout << "\nEmpate!\nIsso foi apenas uma jogada de sorte! O valor da "
              "aposta foi devolvido ao jogador.\n";
    } else {
      cout << "Blackjack do dealer! Você perdeu.\nA casa sempre vence!\n";
    }
    if (!menu(2)) { // Chama o menu para perguntar se o jogador quer continuar
      jogo = false;
    }
    return; // Para de executar jDealer
  }

  // Sequência de jogadas do dealer
  while (dealer.val < 21) {
    if (dealer.val >= 17) { // Dealer para no soft 17+
      cout << "Mão do dealer: " << dealer.cartas << " valor: " << dealer.val
           << endl;
      return;
    } else { // Dealer compra mais uma carta
      reg = range(gen);
      dealer.val += valCarta(reg);
      dealer.cartas = dealer.cartas + " " + imgCarta(reg);
      attCarta(reg);
      cout << endl;
    }
  }
}

void jJogador() {
  escolha = 0;
  int reg;
  int dd = 0;
  while (jogador.val < 21) {
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;
    cout << endl << "Suas opções: " << endl;
    cout << "1 - Pedir carta || Jogador compra uma carta, ganho 2x" << endl;
    cout << "2 - Parar || Jogador passa a vez para o dealer" << endl;
    cout << "3 - Double Down || Jogador compra uma carta e para "
            "automaticamente, ganho 4x"
         << endl;
    cout << "4 - Fugir || Jogador encerra a partida e perde metade da aposta"
         << endl;
    cout << endl
         << "------------------------------------------------------------------"
            "----------"
         << endl;

    cin >> escolha;

    switch (escolha) {
    case 1:
      dd++;
      reg = range(gen);
      jogador.val += valCarta(reg);
      jogador.cartas = jogador.cartas + " " + imgCarta(reg);
      attCarta(reg);
      cout << endl;
      cout << "Mão do jogador: " << jogador.cartas << " valor: " << jogador.val;
      break;
    case 2:
      cout << endl
           << "----------------------------------------------------------------"
              "------------"
           << endl;
      cout << endl << "Jogador não compra mais cartas." << endl;
      cout << endl
           << "----------------------------------------------------------------"
              "------------"
           << endl;
      return;

    case 3:
      dd++;
      if (dd > 1) {
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
        cout << "Opção permitida apenas na primeira rodada. Tente novamente."
             << endl;
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
      } else {
        jogador.aposta += jogador.aposta;
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
        cout << endl
             << "Jogador optou por dobrar aposta!" << endl
             << "Aposta atual: " << jogador.aposta << endl;
        reg = range(gen);
        jogador.val += valCarta(reg);
        jogador.cartas = jogador.cartas + " " + imgCarta(reg);
        attCarta(reg);
        cout << "Mão do jogador: " << jogador.cartas
             << " Valor: " << jogador.val;
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
        return;
      }
    case 4:
      cout << endl
           << "----------------------------------------------------------------"
              "------------"
           << endl;
      cout << "Você fugiu da partida, perdeu metade da aposta." << endl;
      cout << endl
           << "----------------------------------------------------------------"
              "------------"
           << endl;
      jogador.dinheiro = jogador.dinheiro - (jogador.aposta / 2);
      return;
      return;
    }
  }
}

int main() {
  setlocale(LC_ALL, "Portuguese_Brazil");

  if (!menu(1)) { // Pergunta no início do jogo
    return 0;
  }
  while (jogo) {
    aposta();   // Jogador escolhe quanto quer apostar
    fDealer();  // Dealer pega as duas primeiras cartas
    fJogador(); // Jogador pega as duas primeiras cartas

    while (true) {
      if (dealer.val == 21 || jogador.val == 21) {
        if (dealer.val == 21 && jogador.val != 21) {
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          cout << "\nDealer venceu por Blackjack. Você perdeu." << endl;
          cout << "Mão completa do dealer: " << dealer.cartas
               << " Valor: " << dealer.val << endl;
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          jogador.dinheiro -= jogador.aposta;
          break;

        }

        else if (dealer.val != 21 && jogador.val == 21) {
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          cout << "Jogador venceu com Blackjack! Aposta multiplicada por 3!"
               << endl;
          jogador.dinheiro += (jogador.aposta * 3);
          cout << "Banca: R$" << jogador.dinheiro;
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          break;
        }

        else {
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          cout << "Empate. A aposta foi retornada ao jogador." << endl;
          cout << endl
               << "------------------------------------------------------------"
                  "----------------"
               << endl;
          break;
        }
      } // verifica BlackJack

      jJogador(); // Jogador realiza suas ações
      if (jogador.val > 21) {
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
        cout << "Jogador ultrapassou 21" << endl;
        jogador.dinheiro -= jogador.aposta;
        cout << "Boa sorte na próxima vez!" << endl;
        cout << "Banca: R$" << jogador.dinheiro << endl;
        cout << endl
             << "--------------------------------------------------------------"
                "--------------"
             << endl;
        break;
      } else if (escolha == 4) {
        break;
      }

      else {
        jDealer(); // Dealer realiza suas ações
        if (dealer.val > 21) {
          cout << "Mão do dealer: " << dealer.cartas
               << "  valor: " << dealer.val;
          cout << "\nDealer ultrapassou 21" << endl;
          jogador.dinheiro += jogador.aposta;
          cout << "Sorte de principiante!" << endl;
          cout << "Banca: R$" << jogador.dinheiro << endl;
          break;
        } else if (jogador.val > dealer.val) {
          cout << "\n\nJogador venceu!" << endl;
          jogador.dinheiro += jogador.aposta;
          cout << "Banca: R$" << jogador.dinheiro << endl;
          break;
        } else if (dealer.val > jogador.val) {
          cout << "Jogador Perdeu!" << endl;
          jogador.dinheiro -= jogador.aposta;
          cout << "Banca: R$" << jogador.dinheiro << endl;
          break;
        } else {
          cout << "Um empate aconteceu! Nada muda" << endl;
          break;
        }
      }
    }
    if (!menu(2)) { // Pergunta no final da rodada se o jogador quer continuar
      break;
    }
  }
  return 0;
}