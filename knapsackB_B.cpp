#include <stdio.h>
#include <stdlib.h>

#define TAM 45

float peso[TAM] = {23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 38, 63, 85, 89, 82};
float valor[TAM] = {92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 38, 63, 85, 89, 82};
float vprop[TAM];

float capacidade = 800;

struct nodo
{
   int solucao;
   int nivel;
   float upbound;
   struct nodo *prox;
};
typedef struct nodo tnodo;
tnodo *inic = NULL, *fim = NULL;

float vsol(long long solucao)
{
   int i;
   float vaux = 0;
   for (i = 0; i < TAM; i++)
   {
      if (solucao % 2 == 1)
         vaux += valor[i];
      solucao /= 2;
   }
   return vaux;
}

float psol(long long solucao)
{
   int i;
   float paux = 0;
   for (i = 0; i < TAM; i++)
   {
      if (solucao % 2 == 1)
         paux += peso[i];
      solucao /= 2;
   }
   return paux;
}

float max(float vprop[TAM], int nivel)
{
   int i;
   float maux = vprop[nivel + 1];
   for (i = nivel + 2; i < TAM; i++)
      if (vprop[i] > maux)
         maux = vprop[i];
   return maux;
}

int cont = 0;

void insere(long long solucao, int nivel)
{
   cont++;
   tnodo *ptaux = (tnodo *)malloc(sizeof(tnodo));
   ptaux->solucao = solucao;
   ptaux->upbound = vsol(solucao) + (capacidade - psol(solucao)) * max(vprop, nivel);
   ptaux->nivel = nivel;
   ptaux->prox = NULL;
   if (inic == NULL)
      inic = fim = ptaux;
   else
   {
      fim->prox = ptaux;
      fim = ptaux;
   }
}

int retira(long long *solucao, int *nivel, float *upbound)
{
   if (inic == NULL)
      return 0;
   *solucao = inic->solucao;
   *nivel = inic->nivel;
   *upbound = inic->upbound;
   tnodo *ptaux = inic->prox;
   free(inic);
   inic = ptaux;
   return 1;
}

int main()
{
   int i;
   for (i = 0; i < TAM; i++)
      vprop[i] = valor[i] / peso[i];
   insere(0, -1);
   int nivel;
   long long solucao;
   float upbound;
   float valotimo = 0;
   int corte = 0;
   while (retira(&solucao, &nivel, &upbound) != 0)
   {
      //printf("Retirei solucao %x nivel %d upbound=%f\n",solucao,nivel,upbound);
      if (upbound < valotimo)
      {
         corte++;
         continue;
      }
      if (vsol(solucao) > valotimo)
      {
         valotimo = vsol(solucao);
         //      printf("Novo valor otimo=%f\n",valotimo);
      }
      if (nivel == TAM - 1)
         continue;
      int novasolucao = solucao + (1 << (nivel + 1));
      if (psol(novasolucao) <= capacidade)
      {
         //      printf("Inseri solucao %x nivel %d\n",novasolucao,nivel+1);
         insere(novasolucao, nivel + 1);
      }
      //   printf("Inseri solucao %x nivel %d\n",solucao,nivel+1);
      insere(solucao, nivel + 1);
   }
   printf("Valor �timo final=%f cont=%d corte=%d\n", valotimo, cont, corte);
   system("pause");
}
