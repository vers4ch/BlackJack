//
//  main.c
//  BlackJack
//
//  Created by Данил on 10.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

// структура карты
typedef struct card{
  char* suit;
  char* name;
} One_card;

// структура колоды
typedef struct deck{
  One_card card;
  struct deck* next;
} Deck;

// структура игрока
typedef struct player{
  Deck* deck_of_player;
  int points_of_player;
} Player;

// структура дилера
typedef struct dealer{
  Deck* deck_of_dealer;
  int points_of_dealer;
} Dealer;

// создание нового узла списка
Deck* New_element(One_card a_card, Deck* next_element){
  Deck* node = (Deck*)malloc(sizeof(Deck));
  node -> card.suit = a_card.suit;
  node -> card.name = a_card.name;

  node -> next = next_element;

  return node;
}

// подсчет стоимости карты
int Cost_of_Card (One_card a_card){
  int cost = 0;
  
  if (!isdigit(atoi(a_card.name)))
        cost = atoi(a_card.name);
      if (strcmp(a_card.name, "J") == 0 || strcmp(a_card.name, "Q") == 0 || strcmp(a_card.name, "K") == 0)
        cost = 10;
      if (strcmp(a_card.name, "A") == 0)
        cost = 11;

  return cost;
}

// создание списка из узлов
Deck* Create_a_list(One_card array[], int lenth_of_deck){
  Deck *head = NULL, *node = NULL;

  for (int i = lenth_of_deck - 1; i >= 0; i--){
      node = New_element(array[i], node);
      head = node;
    }
  
  return head;
}

// вывод списка
void Print_list(Deck* head){
  Deck* ptr = head;
  
  while(ptr){
      printf("%s - %s\n", ptr -> card.suit, ptr -> card.name);
      ptr = ptr -> next;
    }
}

// добавление элемента в начало списка +
void Add_at_Start(Deck** referance_head, One_card a_card){
  Deck* New_element = (Deck*)malloc(sizeof(Deck));
  
  New_element -> card.suit = a_card.suit;
  New_element -> card.name = a_card.name;

  New_element -> next = *referance_head;
  
  *referance_head = New_element;
}

// удаление с начала +
One_card Delete_at_Start(Deck **head){
  One_card deleted_card;
  Deck* temp = (*head) -> next;
  deleted_card.suit = (*head) -> card.suit;
  deleted_card.name = (*head) -> card.name;
  
  free(*head);
  
  (*head) = temp;
  
  return deleted_card;
}

// добавление узла после индекса элемента списка
void Add_at_Position(int position, One_card a_card, Deck *head){
  int count = 0;
  while (count < position - 1 && head -> next)
    {
      head = head -> next;
      count++;
    }

  Deck* new_element = (Deck*)malloc(sizeof(Deck));
  new_element -> card.suit = a_card.suit;
  new_element -> card.name = a_card.name;

  if (head -> next)
    new_element -> next = head -> next;
  else
    new_element -> next = NULL;

  head -> next = new_element;
}

// удаление определенного узла +
One_card Delete_position(int position, Deck *head){
  Deck* head_1 = head;
  Deck* previous_element = NULL;
  One_card a_card;

  int count = 0;

  while (head_1 && count < position){
    count++;
    previous_element = head_1;
    head_1 = head_1 -> next;
  }

  if (head == head_1)
    head = head_1 -> next;
  else{
    if (previous_element)
      previous_element -> next = head_1 -> next;
  }
  
  a_card.suit = head_1 -> card.suit;
  a_card.name = head_1 -> card.name;

  free(head_1);
  return a_card;
}

// тасовка колоды
void Shuffle_a_Deck(Deck **head){
  srand(time(NULL));
  for (int i = 0; i < 96784 + rand() % (99534 - 96784 + 1); i++){
      One_card shuffle_card;
      int position_of_paste = rand() % 52;
      int position_of_delete = rand() % 52;

      if (position_of_delete == 0)
        shuffle_card = Delete_at_Start(head);
      else
        shuffle_card = Delete_position(position_of_delete, *head);

      if (position_of_paste == 0)
        Add_at_Start(head, shuffle_card);
      else
        Add_at_Position(position_of_paste, shuffle_card, *head);
    }
}

// красивый вывод карты
void Print_a_Card(One_card a_card, int is_dealer){
// проверка на карту дилера
if (is_dealer == 0){
    if (strcmp(a_card.name, "10") == 0){
    printf("    ________\n \
  |        |\n \
  | %s     |\n \
  | %s      |\n \
  |     %s |\n \
  |      %s |\n \
  |________|\n", a_card.name, a_card.suit, a_card.name, a_card.suit);
    }
    else{
    printf("    ________\n \
  |        |\n \
  | %s      |\n \
  | %s      |\n \
  |      %s |\n \
  |      %s |\n \
  |________|\n", a_card.name, a_card.suit, a_card.name, a_card.suit);
    }
  }
  else{
  printf("    ________\n \
  | ##**## |\n \
  | **##** |\n \
  | ##**## |\n \
  | **##** |\n \
  | ##**## |\n \
  |________|\n");
  }
}

// логика игры
int Logic_of_Game(Player players_points, Dealer dealers_points, int money, int bet, FILE* log){
    if (players_points.points_of_player == dealers_points.points_of_dealer){
        printf("\n\n\t\t\t  PUSH\n\v");
        fprintf(log, "\n\t\t  PUSH\n");
        return money + bet;
    }
    else if (players_points.points_of_player == 21){
        printf("\n\n\t\t\t  BLACKJACK!!!\n\v");
        fprintf(log, "\n\t\t  BLACKJACK!!!\n");
        return ceil(money + bet + (bet * (double)(3 / 2)));
    }
    else if (dealers_points.points_of_dealer > 21){
        printf("\n\n\t\t\t  You WON!!!\n\v");
        fprintf(log, "\n\t\t  You WON!!!\n");
        return money + (bet * 2);
    }
    else if (players_points.points_of_player > 21 || dealers_points.points_of_dealer == 21){
        printf("\n\n\t\t\t  You LOSE!!!\n\v");
        fprintf(log, "\n\t\t  You LOSE!!!\n");
        return money;
    }
    else if (players_points.points_of_player < dealers_points.points_of_dealer){
        printf("\n\n\t\t\t  You LOSE!!!\n\v");
        fprintf(log, "\n\t\t  You LOSE!!!\n");
        return money;
    }
    else if (dealers_points.points_of_dealer < players_points.points_of_player){
        printf("\n\n\t\t\t  You WON!!!\n\v");
        fprintf(log, "\n\t\t  You WON!!!\n");
        return money + (bet * 2);
    }
    return 0;
}

//запись в файл
void Print_in_File(Deck* deck_of_cards, int count_of_cards, FILE* log){
  One_card cards[count_of_cards];
  Deck* temp = deck_of_cards;

  for (int i = 0; i < count_of_cards; i++){
      cards[i] = temp -> card;
      fprintf(log, "%s - %s\n", cards[i].suit, cards[i].name);
      temp = deck_of_cards -> next;
    }
}

int main(){
// константы
    const int size_of_deck = 52;
  
// массив структур
  One_card deck_of_cards[size_of_deck];

// массивы мастей и значений
  char* suits[] = {"\u2660", "\u2665", "\u2663", "\u2666"};
  char* names[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

// генерация массива структур
  for (int k = 1; k < size_of_deck + 1; k++){
      deck_of_cards[k - 1].suit = suits[k % 4];
      deck_of_cards[k - 1].name = names[k % 13];
    }
  
  
  int money = 1000;
  int exit = 0;
  int bet = 0;
  FILE* open_logs = fopen("log_of_game.txt", "w");
    fprintf(open_logs, "FILE OPEN\n");
  Deck* deck_of_dealer_at_start;
  Deck* deck_of_player_at_start;
  
  while (exit == 0){
      printf("\t\t\tYOUR MONEY: %d\n", money);
      printf("What bet you want to put?(minimum bet = 1)\n");
      scanf("%d", &bet);
      int money_at_start = money;
      
      if (bet < 1){
        printf("\t\tDON'T CHEAT, SLAVE!!!\n");
        break;
      }
      
      system("clear");

      if (bet > money){
        printf("\t\tNOT ENOUGH MONEY, BITCH\n");
        break;
      }
      
      money -= bet;
      
    // создание колоды и перемешивание
      Deck* deck_in_game = Create_a_list(deck_of_cards, size_of_deck);
      Shuffle_a_Deck(&deck_in_game);
    
    // счётчик карт
      int count_of_players_cards = 0;
      int count_of_dealers_cards = 0;
    
    // рука Игрока
      Player risky_person;
      risky_person.deck_of_player = (Deck*)malloc(sizeof(Deck));
      risky_person.deck_of_player -> next = NULL;
      risky_person.points_of_player = 0;

      printf("\t\t\tYOUR MONEY: %d\n", money);
      printf("Your hand:\n");
      for (int i = 0; i < 2; i++){
          One_card temp_player = Delete_at_Start(&deck_in_game);
          
          if (strcmp(temp_player.name, "A") == 0 && risky_person.points_of_player + Cost_of_Card(temp_player) > 21)
            risky_person.points_of_player += 1;
          else
            risky_person.points_of_player += Cost_of_Card(temp_player);
          
          Add_at_Start(&risky_person.deck_of_player, temp_player);
          Print_a_Card(temp_player, 0);
          count_of_players_cards++;
        }
      printf("\nYour points is: %d\n", risky_person.points_of_player);
      deck_of_player_at_start = risky_person.deck_of_player;
      int points_of_player_at_start = risky_person.points_of_player;
      
    // рука Дилера
      Dealer eternal_winner;
      eternal_winner.deck_of_dealer = (Deck*)malloc(sizeof(Deck));
      eternal_winner.deck_of_dealer -> next = NULL;
      eternal_winner.points_of_dealer = 0;
    
      printf("\nDealer's hand:\n");
      int cost = 0;
      for (int i = 0; i < 2; i++){
          One_card temp = Delete_at_Start(&deck_in_game);
          cost = Cost_of_Card(temp);
    
          eternal_winner.points_of_dealer += cost;
          Add_at_Start(&eternal_winner.deck_of_dealer, temp);
          count_of_dealers_cards++;
        }
      Print_a_Card(eternal_winner.deck_of_dealer -> next -> card, 0);
      Print_a_Card(eternal_winner.deck_of_dealer -> card, 1);
      printf("\nDealer's points is: %d\n", eternal_winner.points_of_dealer - cost);
      int points_of_dealer_at_start = eternal_winner.points_of_dealer;
      deck_of_dealer_at_start = eternal_winner.deck_of_dealer;
      
    
    // выбор хода
      int choose;
      printf("\n1 - Stand");
      printf("\n2 - Take card\n");
      printf("3 - Double bount\n");
      scanf("%d", &choose);

      fprintf(open_logs, "==========================\n");
      fprintf(open_logs, "Your money: %d\n", money_at_start);
      fprintf(open_logs, "Your bet: %d\n", bet);
      fprintf(open_logs, "\nYour hand:\n");
      Print_in_File(deck_of_player_at_start, 2, open_logs);
      fprintf(open_logs, "Your points: %d\n", points_of_player_at_start);

      fprintf(open_logs, "\nDealer hand:\n");
      Print_in_File(deck_of_dealer_at_start, 2, open_logs);
      fprintf(open_logs, "\nDealer's points: %d\n", points_of_dealer_at_start);

      
      switch(choose){
          case 1:
            system("clear");
            printf("\t\t\t\tSTAND:\v\n");
            printf("\t\t\tYOUR MONEY: %d\n", money);
            fprintf(open_logs, "\nMOVE: STAND\n");
            
            // игрок
            printf("Your cards:\n");
            fprintf(open_logs, "\nYour cards:\n");
            for (int i = 0; i < count_of_players_cards; i++){
                One_card temp_print = Delete_at_Start(&risky_person.deck_of_player);
                Print_a_Card(temp_print, 0);
                fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                Add_at_Start(&deck_in_game, temp_print);
              }
            printf("Your points: %d\n", risky_person.points_of_player);
            fprintf(open_logs, "Your points: %d\n", risky_person.points_of_player);
            
    
            // дилер
            while (eternal_winner.points_of_dealer < 17){
                One_card temp1 = Delete_at_Start(&deck_in_game);
                int cost = Cost_of_Card(temp1);
                Add_at_Start(&eternal_winner.deck_of_dealer, temp1);
                One_card temp = eternal_winner.deck_of_dealer -> next -> card;
                
                if (eternal_winner.points_of_dealer + Cost_of_Card(temp) > 21 && strcmp(temp.name, "A") == 0)
                  eternal_winner.points_of_dealer += 1;
                else
                  eternal_winner.points_of_dealer += cost;
                
                count_of_dealers_cards++;
              }
            
            
            printf("\nDealer's cards:\n");
            fprintf(open_logs, "\nDealer's cards:\n");
            for (int i = 0; i < count_of_dealers_cards; i++){
                One_card temp_print = Delete_at_Start(&eternal_winner.deck_of_dealer);
                Print_a_Card(temp_print, 0);
                fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                Add_at_Start(&deck_in_game, temp_print);
              }
            printf("Dealer's points: %d", eternal_winner.points_of_dealer);
            fprintf(open_logs, "\nDealer's points: %d\n", eternal_winner.points_of_dealer);
    
            // Игра при не желании брать карты
            money = Logic_of_Game(risky_person, eternal_winner, money, bet, open_logs);
            printf("\t\t\tYOUR MONEY: %d\n\n", money);
            fprintf(open_logs, "\t\tYOUR MONEY: %d\n\n", money);
            
            break;
    
          case 2:
            system("clear");
            printf("\t\t\t\tTAKING CARD:\v\n");
            fprintf(open_logs, "\nMOVE: ADD CARD\n");
            int stop = 1;
    
            while (stop == 1){
                One_card temp_hit = Delete_at_Start(&deck_in_game);
                Add_at_Start(&risky_person.deck_of_player, temp_hit);
                count_of_players_cards++;
                risky_person.points_of_player = 0;
                printf("\t\t\tYOUR MONEY: %d\n", money);
                
                for (int i = 0; i < count_of_players_cards; i++){
                    One_card temp;
                    
                    if (i == 0)
                      temp = Delete_at_Start(&risky_person.deck_of_player);
                    else
                      temp = Delete_position(i, risky_person.deck_of_player);
                    
                    if (risky_person.points_of_player + Cost_of_Card(temp) > 21 && strcmp(temp.name, "A") == 0)
                      risky_person.points_of_player += 1;
                    else
                      risky_person.points_of_player += Cost_of_Card(temp);
                    Print_a_Card(temp, 0);
                    Add_at_Start(&risky_person.deck_of_player, temp);
                  }
                printf("Your points: %d\n", risky_person.points_of_player);
                
                printf("\nDealer's hand:\n");
                Print_a_Card(eternal_winner.deck_of_dealer -> next -> card, 0);
                Print_a_Card(eternal_winner.deck_of_dealer -> card, 1);
                printf("\nDealer's points is: %d\n", eternal_winner.points_of_dealer - cost);
    
                int next;
                printf("\nNext step:\n");
                printf("1 - Stand\n");
                printf("2 - Add card\n");
                scanf("%d", &next);
                
                if (next == 1){
                  stop = 0;
                  while (eternal_winner.points_of_dealer < 17){
                    One_card temp1 = Delete_at_Start(&deck_in_game);
                    int cost = Cost_of_Card(temp1);
                    Add_at_Start(&eternal_winner.deck_of_dealer, temp1);
                    One_card temp = eternal_winner.deck_of_dealer -> next -> card;
                
                    if (eternal_winner.points_of_dealer + Cost_of_Card(temp) > 21 && strcmp(temp.name, "A") == 0)
                      eternal_winner.points_of_dealer += 1;
                    else
                      eternal_winner.points_of_dealer += cost;
                    
                    count_of_dealers_cards++;
                  }
    
                  system("clear");
                  printf("\t\t\tYOUR MONEY: %d\n", money);
                  printf("Your cards:\n");
                  fprintf(open_logs, "\nYour cards:\n");
                  for (int i = 0; i < count_of_players_cards; i++){
                      One_card temp_print = Delete_at_Start(&risky_person.deck_of_player);
                      Add_at_Start(&deck_in_game, temp_print);
                      Print_a_Card(temp_print, 0);
                      fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                    }
                  printf("Your points: %d\n", risky_person.points_of_player);
                  fprintf(open_logs, "Your points: %d\n", risky_person.points_of_player);
    
                  printf("\nDealer's cards:\n");
                  fprintf(open_logs, "\nDealer's cards:\n");
                  for (int i = 0; i < count_of_dealers_cards; i++){
                      One_card temp_print = Delete_at_Start(&eternal_winner.deck_of_dealer);
                      Add_at_Start(&deck_in_game, temp_print);
                      Print_a_Card(temp_print, 0);
                      fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                    }
                  printf("Dealer's points: %d\n", eternal_winner.points_of_dealer);
                  fprintf(open_logs, "Dealer's points: %d\n", eternal_winner.points_of_dealer);
                  money = Logic_of_Game(risky_person, eternal_winner, money, bet, open_logs);
                  printf("\t\t\tYOUR MONEY: %d\n\n", money);
                  fprintf(open_logs, "\t\tYOUR MONEY: %d\n\n", money);
                }
                else{
                  stop = 1;
                  system("clear");
                  printf("\t\t\t\tTAKING CARD:\v\n");
                }
              }
            
            break;

          case 3:
            system("clear");
            printf("\t\t\t\tDOUBLE:\v\n");
            money -= bet;
            printf("\t\t\tYOUR MONEY: %d\n", money);
            fprintf(open_logs, "\nMOVE: DOUBLE BOUNT\n");

            One_card temp_add = Delete_at_Start(&deck_in_game);
            Add_at_Start(&risky_person.deck_of_player, temp_add);
            count_of_players_cards++;
            
            printf("Ваши карты:\n");
            fprintf(open_logs, "\nВаши карты:\n");
            for (int i = 0; i < count_of_players_cards; i++){
                One_card temp_print = Delete_at_Start(&risky_person.deck_of_player);
                Print_a_Card(temp_print, 0);
                fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                Add_at_Start(&deck_in_game, temp_print);
              }
            printf("Очков у Вас: %d\n", risky_person.points_of_player);
            fprintf(open_logs, "Очков у Вас: %d\n", risky_person.points_of_player);

            while (eternal_winner.points_of_dealer < 17){
                
                One_card temp1 = Delete_at_Start(&deck_in_game);
                int cost = Cost_of_Card(temp1);
                Add_at_Start(&eternal_winner.deck_of_dealer, temp1);
                One_card temp = eternal_winner.deck_of_dealer -> next -> card;
                
                if (eternal_winner.points_of_dealer + Cost_of_Card(temp) > 21 && strcmp(temp.name, "A") == 0)
                  eternal_winner.points_of_dealer += 1;
                else
                  eternal_winner.points_of_dealer += cost;
                
                count_of_dealers_cards++;
              }
    
            printf("\nКарты дилера:\n");
            fprintf(open_logs, "\nКарты дилера:\n");
            for (int i = 0; i < count_of_dealers_cards; i++){
                One_card temp_print = Delete_at_Start(&eternal_winner.deck_of_dealer);
                Print_a_Card(temp_print, 0);
                fprintf(open_logs, "%s - %s\n", temp_print.suit, temp_print.name);
                Add_at_Start(&deck_in_game, temp_print);
              }
            printf("Очки дилера: %d\n", eternal_winner.points_of_dealer);
            fprintf(open_logs,"Очки дилера: %d\n", eternal_winner.points_of_dealer);
    
            money = Logic_of_Game(risky_person, eternal_winner, money, bet, open_logs);
            money += bet;
            printf("\t\tФишек на счету: %d\n\n", money);
            fprintf(open_logs, "\t\tФишек на счету: %d\n\n", money);
        }
      
      int a;
      printf("Continue(0 - no / 1 - yes)?\n");
      scanf("%d", &a);
      if (a == 0)
        exit = 1;
      else
        exit = 0;
      system("clear");
  }
    fprintf(open_logs, "CLOSE\n");
  fclose(open_logs);
  return 0;
}
