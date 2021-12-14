#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <iostream>

using namespace std;

#define TAM 45

float peso[TAM] = {23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 38, 63, 85, 89, 82};
float valor[TAM] = {92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 92, 57, 49, 68, 60, 43, 67, 84, 87, 72, 23, 31, 29, 44, 53, 38, 63, 85, 89, 82, 38, 63, 85, 89, 82};
float vprop[TAM];

float capacidade = 800;
double node_count = 0;

struct nodo
{
   int solucao;
   int nivel;
   float upbound;
   struct nodo *prox;

   //functor para comparação na pq
   bool operator<(const nodo &rhs) const
   {
      return this->upbound < rhs.upbound;
   }
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

void insere(priority_queue<tnodo> &queue, long long solucao, int nivel)
{
   node_count++;
   tnodo node_aux;
   node_aux.solucao = solucao;
   node_aux.upbound = vsol(solucao) + (capacidade - psol(solucao)) * max(vprop, nivel);
   node_aux.nivel = nivel;
   // node_aux.prox = NULL;
   // insere novo nodo na fila de prioridade e reordena os valores
   queue.push(node_aux);
}

//#######################################################
// Retira pode ser substituida por priority_queue::pop()
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
//#######################################################

int main()
{

   priority_queue<tnodo> queue;
   tnodo node;

   int i;
   for (i = 0; i < TAM; i++)
      vprop[i] = valor[i] / peso[i];

   insere(queue, 0, -1);
   int nivel;
   long long solucao;
   float upbound;
   double best_value = 0;
   int corte = 0;

   clock_t start, end;

   start = clock();

   // retira(&solucao, &nivel, &upbound) != 0 substituida por priority_queue::pop()
   while (queue.empty() == false)
   {
      node = queue.top();
      queue.pop();
      if (node.upbound < best_value)
      {
         corte++;
         continue;
      }
      if (vsol(node.solucao) > best_value)
      {
         best_value = vsol(node.solucao);
         cout << "Novo valor otimo: " << best_value << endl;
      }
      if (node.nivel == TAM - 1)
         continue;

      int novasolucao = node.solucao + (1 << (node.nivel + 1));
      if (psol(novasolucao) <= capacidade)
      {
         // printf("Inseri solucao %x nivel %d\n", novasolucao, node.nivel + 1);
         insere(queue, novasolucao, node.nivel + 1);
      }
      // printf("Inseri solucao %x nivel %d\n", node.solucao, node.nivel + 1);
      insere(queue, node.solucao, node.nivel + 1);
   }

   end = clock();
   double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

   cout << "Valor ótimo final: " << fixed
        << best_value << "\n"
        << "Tamanho fila de prioridade: " << fixed
        << node_count << "\n"
        << "Nodos cortados: " << fixed
        << corte << "\n"
        << "Tempo de execução: " << fixed
        << time_taken << " segundos" << endl;
}
