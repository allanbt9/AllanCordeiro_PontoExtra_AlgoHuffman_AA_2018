#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
 

typedef struct SIMBOLO *TSimbolo;
typedef struct CODIGO  *TCodigo;
 
struct SIMBOLO
{
   int simbolo;
   float probabilidade;
   TSimbolo rlink;
   TSimbolo llink;
   TSimbolo mother;
   TSimbolo lista;
};
 
struct CODIGO
{
   TSimbolo endereco;
   int      simbolo;
   float    probabilidade;
   char     codigo[20];
   TCodigo  link;
};
 
//----------------------------------------------------------------------------
 
void     huffman(void);
TSimbolo inserir_simbolo(TSimbolo, int, float);
void     listar_lista(TSimbolo);
int      contar_lista(TSimbolo);
TSimbolo agrupar_ultimos(TSimbolo);
TSimbolo ordenar_lista(TSimbolo, TSimbolo);
TSimbolo agrupar_ultimos2(TSimbolo);
TCodigo  armazenar_simbolos(TSimbolo);
void     gerar_simbolos(TCodigo);
void     exibir_resultado(TCodigo);
void     inverter_codigos(TCodigo);
void     inverter_string(char*);
void     ordenar_codigos(TCodigo);

 
//----------------------------------------------------------------------------
 
void huffman(void)
{
   TSimbolo arvore = NULL; // Originalmente uma lista, depois uma árvore
   TSimbolo ultimo;
   TCodigo  codigos = NULL; // Irá armazenar o resultado final
   float prob_total = 0.0, prob;
   int   contador = 0;
 
   system("cls");
   printf("\tIMPLEMENTACAO DO ALGORITMO DE HUFFMAN\n\n");
 
   do
   {
      printf(" Probabilidade de S%d: ", contador);
      scanf("%f", &prob);
      arvore = inserir_simbolo(arvore, contador, prob);
      contador++;
      prob_total += prob;
   }while(prob_total < 1.0);
 
   codigos = armazenar_simbolos(arvore);
 
   while(contar_lista(arvore) > 2)
   {
      ultimo = agrupar_ultimos(arvore);
      arvore = ordenar_lista(arvore, ultimo);
   }
 
   arvore = agrupar_ultimos2(arvore);
   gerar_simbolos(codigos);
   inverter_codigos(codigos);
   ordenar_codigos(codigos);
   exibir_resultado(codigos);   
}
 
//----------------------------------------------------------------------------
 
TSimbolo inserir_simbolo(TSimbolo lista, int simbolo, float probabilidade)
{
   TSimbolo novo = (TSimbolo) malloc(sizeof(struct SIMBOLO));
   novo->simbolo = simbolo;
   novo->probabilidade = probabilidade;
   novo->rlink = NULL;
   novo->llink = NULL;
   novo->mother = NULL;
 
   if(lista == NULL)
   {
      lista = novo;
      novo->lista = NULL;
   }
   else
   {
      if(lista->probabilidade < novo->probabilidade)
      {
         novo->lista = lista;
         lista = novo;
      }
      else
      {
         TSimbolo aux1 = lista, aux2 = lista->lista;
         int      alocado = 0;
         while(aux2 != NULL)
         {
            if(aux2->probabilidade < novo->probabilidade)
            {
               novo->lista = aux2;
               aux1->lista = novo;
               alocado = 1;
               break;
            }
            aux1 = aux2;
            aux2 = aux2->lista;
         }
 
         if(!alocado)
         {
            aux1->lista = novo;
            novo->lista = NULL;
         }
      }
   }
 
   return lista;
}
 
//----------------------------------------------------------------------------
 
void listar_lista(TSimbolo lista)
{
   while(lista != NULL)
   {
      printf("\n S%d: %.2f", lista->simbolo, lista->probabilidade);
      lista = lista->lista;
   }
 
   getch();
}
 
//----------------------------------------------------------------------------
 
int contar_lista(TSimbolo lista)
{
   int contador = 0;
 
   while(lista != NULL)
   {
      contador++;
      lista = lista->lista;
   }
 
   return contador;
}
 
//----------------------------------------------------------------------------
 
TSimbolo agrupar_ultimos(TSimbolo lista)
{
   TSimbolo agrup = (TSimbolo) malloc(sizeof(struct SIMBOLO));
 
   while(lista->lista->lista->lista != NULL)
      lista = lista->lista;
 
   agrup->simbolo = -1;
   agrup->probabilidade = lista->lista->probabilidade + lista->lista->lista->probabilidade;
   agrup->lista = NULL;
   agrup->rlink = lista->lista->lista;
   agrup->llink = lista->lista;
   lista->lista->lista->mother = agrup;
   lista->lista->mother = agrup;
   lista->lista = NULL;
 
   return agrup;
}
 
//----------------------------------------------------------------------------
 
TSimbolo ordenar_lista(TSimbolo lista, TSimbolo novo)
{
   if(lista->probabilidade <= novo->probabilidade)
   {
      novo->lista = lista;
      lista = novo;
   }
   else
   {
      TSimbolo aux1 = lista, aux2 = lista->lista;
      int      alocado = 0;
      while(aux2 != NULL)
      {
         if(aux2->probabilidade <= novo->probabilidade)
         {
            novo->lista = aux2;
            aux1->lista = novo;
            alocado = 1;
            break;
         }
         aux1 = aux2;
         aux2 = aux2->lista;
      }
 
      if(!alocado)
      {
         aux1->lista = novo;
         novo->lista = NULL;
      }
   }
 
   return lista;
}
 
//----------------------------------------------------------------------------
 
TSimbolo agrupar_ultimos2(TSimbolo lista)
{
   TSimbolo agrup = (TSimbolo) malloc(sizeof(struct SIMBOLO));
   agrup->lista = NULL;
   agrup->simbolo = -1;
   agrup->probabilidade = lista->probabilidade + lista->lista->probabilidade;
   agrup->rlink = lista->lista;
   agrup->llink = lista;
   lista->mother = agrup;
   lista->lista->mother = agrup;
   lista = agrup;
   lista->mother = NULL;
 
   return agrup;
}
 
//----------------------------------------------------------------------------
 
TCodigo armazenar_simbolos(TSimbolo lista)
{
   TCodigo codigos = NULL, item;
 
   while(lista != NULL)
   {
      item = (TCodigo) malloc(sizeof(struct CODIGO));
      item->endereco = lista;
      item->simbolo = lista->simbolo;
      item->probabilidade = lista->probabilidade;
      item->link = codigos;
      codigos = item;
      lista = lista->lista;
   }
 
   return codigos;
}
 
//----------------------------------------------------------------------------
 
void gerar_simbolos(TCodigo lista)
{
   TSimbolo aux1, aux2;
   int      indice = 0;
 
   while(lista != NULL)
   {
      aux1 = lista->endereco;
      aux2 = aux1->mother;
      while(aux2 != NULL)
      {
         if(aux1 == aux2->llink)
            lista->codigo[indice] = '0';
         else if(aux1 == aux2->rlink)
            lista->codigo[indice] = '1';
         aux1 = aux2;
         aux2 = aux2->mother;
         indice++;
      }
      lista->codigo[indice] = '\0';
      lista = lista->link;
      indice = 0;
   }
}
 
//----------------------------------------------------------------------------
 
void exibir_resultado(TCodigo lista)
{
   system("cls");
   printf("\t IMPLEMENTACAO DO ALGORITMO DE HUFFMAN\n\n");
 
   while(lista != NULL)
   {
      printf(" S%d - Prob: %.2f - %s\n", lista->simbolo, lista->probabilidade,
                                         lista->codigo);
      lista = lista->link;
   }
 
   printf("\n Faltando ordenar os simbolos. . .");
 
   getch();
}
 
//----------------------------------------------------------------------------
 
void inverter_codigos(TCodigo lista)
{
   while(lista != NULL)
   {
      inverter_string(lista->codigo);
      lista = lista->link;
   }
}
 
//----------------------------------------------------------------------------
 
void inverter_string(char *string)
{
   int  i, j;
   char temp[20];
 
   for(i = 0, j = strlen(string) - 1; j >= 0; ++i, --j)
      temp[i] = string[j];
 
   temp[i] = '\0';
   strcpy(string, temp);
}
 
//----------------------------------------------------------------------------
 
void ordenar_codigos(TCodigo lista)
{
   TCodigo  nav;
   TSimbolo auxE;
   char     auxC[20];
   int      auxS, chave;
   float    auxP;
 
    
   do
   {
      chave = 0;
      nav = lista;
      while(nav->link != NULL)
      {
         if(nav->simbolo > nav->link->simbolo)
         {
            auxE = nav->endereco;
            strcpy(auxC, nav->codigo);
            auxS = nav->simbolo;
            auxP = nav->probabilidade;
             
            nav->endereco = nav->link->endereco;
            strcpy(nav->codigo, nav->link->codigo);
            nav->simbolo = nav->link->simbolo;
            nav->probabilidade = nav->link->probabilidade;
             
            nav->link->endereco = auxE;
            strcpy(nav->link->codigo, auxC);
            nav->link->simbolo = auxS;
            nav->link->probabilidade = auxP;
             
            chave = 1;            
         }         
         nav = nav->link;
      }
   }while(chave);   
}
 


 
//----------------------------------------------------------------------------
 
int main(void)
{
   int opcao;
 
   do
   {
      system("cls");
      printf("\tIMPLEMENTACAO DO ALGORITMO DE HUFFMAN\n\n");
      printf(" 1. Inserir Probabilidades dos Simbolos\n");
      printf(" 2. Encerrar Aplicativo\n\n");
      printf(" Opcao: ");
      scanf("%d", &opcao);
 
      switch(opcao)
      {
         case 1 : huffman();
                  break;
         case 2 : printf("\n Aplicativo finalizado com sucesso. . .");
                  Sleep(2000);
                  break;
         default: printf("\n Opcao invalida. . .");
                  Sleep(2000);
      }
   }while(opcao != 2);
 
   return 0;
}
