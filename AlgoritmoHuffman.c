/*
	Desenvolva um algoritmo para ler uma expressão informada pelo usuário e gerar a sequência de bits que a representa através da técnica de compressão de dados de Huffman. Para tal, considere o seguinte:
	1. criar uma lista encadeada que armazena o caractere e sua frequência na expressão.
	2. Ordene a lista de forma crescente ou decrescente a partir da frequência do caractere.
	3. Crie os nós da árvore de Huffman a partir dos dados da lista.
	4. Gere a sequência de bits que representa a expressão e certifique-se que é correta utilizando o processo de descompactação.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Letras{
	char letra[3];
	int frequencia;
	struct Letras *prox;
}Letras;

typedef struct arvore{
	char info[3];
	char bits[20];
	struct arvore *esq;
	struct arvore *dir;
}arv;

void descomprimir(char palavraCompacta[100], arv *r){  // recebe a sequencia de bits das letras da palavra digitada e descomprimi
	arv *p = r;
	int i;
	for(i=1; i<strlen(palavraCompacta); i++){  // analisa bit a bit
		if(palavraCompacta[i] == '0')  // se for 0
			p = p->esq;  // percorre esquerda
		else
			p = p->dir;  // se nao percorre direita
		if(p->info[0] != '=' && p->info[0] != ':'){  // se encontra uma letra (= e : sao somas)
			printf(" %c ", p->info[0]);  // imprimi a letra
			p = r;  // e volta pro inicio da arvore
		}
	}
}

int compactar(arv *r, char x, char palavraCompacta[100]){  // compacta a palavra com a sua sequencia de bits
    if(r == NULL)
         return 0;
    else{
             if(x == r->info[0]){  // se a letra da sequencia for encontrada na arvore
                 strcat(palavraCompacta, r->bits);  // salva o bits da letra na variavel
                 return 1;    
             }
             else if(compactar(r->esq, x, palavraCompacta) == 1) // percorre esquerda
                 return 1;
             else
                 return compactar(r->dir, x, palavraCompacta);  // percorre direita
         }
}

void sequenciaBits(arv *r){  // gera a sequencia de bits parando nos no somas e baixando seus bits para seus filhos.
     if(r != NULL){
    	if(r->info[0] == '=' && r->bits[0] != -1){  // para somente nas somas
             char aux[20];  
             strcpy(aux, r->bits);  // auxiliar recebe o numero de bits da soma
             strcat(r->bits, r->dir->bits);  // soma recebe os bits do no da direita
             strcpy(r->dir->bits, r->bits);  // no da direita armeza o caminho do no pai ate ele
             strcat(aux, r->esq->bits);  // realiza o mesmo procedimento pro no da esquerda
             strcpy(r->esq->bits, aux);       
             r->bits[0] = -1;  // quando um no soma baixa seus bits pros nos da esquerda e direita, recebe -1 nos bits
        }  // ou seja, ele nao precisa mais ser verificado
        sequenciaBits(r->esq);
        sequenciaBits(r->dir);
    } 
}

void imprimeArv(arv *r, char opcao){  // imprimi a arvore e a sequencia de bits pra cada letra
    if(r != NULL){
    	if(r->info[0] != '=' && r->info[0] != ':'){  // pula as somas e so imprimi nos folhas
                if(opcao == 'l')
                      printf(" %c \t", r->info[0]);
                else if(opcao == 'f')
                      printf(" %s \t", r->bits);
         }
         imprimeArv(r->esq, opcao);
         imprimeArv(r->dir, opcao);
    } 
}

arv *criaArvore(arv *r, Letras *l){
	arv *novo = (arv*) malloc(sizeof(arv));  // cria o novo no pai
	Letras *p = l;  // encontra as duas ultimas letras da lista simples
	while(p->prox->prox != NULL)
		p = p->prox;
	int soma;
	char aux[5];
	soma = p->frequencia + p->prox->frequencia;  // soma suas frequencias
	sprintf(aux, "%i", soma);  // transforma inteiro em string
	strcpy(novo->info, "=");  // info = "="; somas sempre inicial por =
	strcat(novo->info, aux);  // info = "= valorSoma";
	strcpy(novo->bits, "1");  // nos pai sempre recebem 1 nos bits
	if(p->letra[0] != '=' && p->prox->letra[0] != '='){  // os dois ultimos são somas.
		arv *e = (arv*) malloc(sizeof(arv));  // cria folha esquerda
		arv *d = (arv*) malloc(sizeof(arv));  // cria folha esqueda
        arv *ultimo = (arv*) malloc(sizeof(arv));  // guarda o endereco do pai na raiz da arvore
        ultimo->esq = novo;  // o endereco do pai fica no campo esq
        ultimo->dir = r;
        r = ultimo;  // raiz da arvore aponta pro novo pai
		e->info[0] = p->letra[0];  // penultimo da lista simples vai para folha esquerda
		e->dir = NULL;
		e->esq = NULL;
		strcpy(e->bits, "0");  // folha esquerda recebe 0
		d->info[0] = p->prox->letra[0];  // folha direita recebe o ultimo da lista simples
		d->dir = NULL;
		d->esq = NULL;
		strcpy(d->bits, "1");  // folha direita recebe 1
		novo->esq = e;  // liga os folha com o pai
		novo->dir = d;
	}
	else if(p->letra[0] == '=' && p->prox->letra[0] == '='){  // se os dois ultimos sao somas
        novo->esq = r->esq;  // lado esquerdo do novo pai recebe o antigo pai, guardado no raiz
        novo->dir = r->dir->esq;  // lado direito recebe outro pai guardado no raiz
        arv *remove = r;  // remove os pais guardados na raiz
        arv *aux = r->dir;
        free(remove);  // atualizando o novo
        r = aux;
        if(aux->dir != NULL){  // se ainda tiver outro no pai
             arv *p = r;
             while(p->dir != NULL)  // aloca o novo pai depois dele
                 p = p->dir;
             r = aux->dir;
             p->dir = aux;
             aux->dir = NULL;
             aux->esq = novo;   
        }
        strcpy(novo->dir->bits, "1");  // no pai da direita recebe 1
        strcpy(novo->esq->bits, "0");  // no pai da esquerda recebe 0
    }
    else{  // se os dois ultimos for soma e letra
        arv *e = (arv*) malloc(sizeof(arv));  // aloca a letra na esquerda do pai
        novo->esq = e; 
        if(p->letra[0] != '=')  // se o penultimo for letra
            e->info[0] = p->letra[0];  
        else  // se nao o ultimo eh
            e->info[0] = p->prox->letra[0];
        e->dir = NULL;
        e->esq = NULL;
        strcpy(e->bits, "0");  // folha esquerda recebe 0
        novo->dir = r->esq;  // novo pai da direita aponta para pai guardado na raiz
        r->esq = novo;  // atualiza a raiz apontando para o novo pai
        strcpy(novo->dir->bits, "1");  // pai da direita recebe 1
    }
    strcpy(p->letra, novo->info);  // atualiza a lista simples 
	p->frequencia = soma;  // alocando a soma na lista
    free(p->prox);  // e apagando a ultima letra
	p->prox = NULL;
	return r;
}

void *retiraLetrasRepetidas(Letras *l){
     Letras *p, *k, *remove, *ant;
     int aux;
     for(p = l; p != NULL; p = p->prox){
           if(p->frequencia > 1){  // so verifica letras que tem mais de uma ocorrencia
               aux = p->frequencia - 1; 
               k = p->prox;
               while(aux > 0){  // vai atras do repetidos
                    if(p->letra[0] == k->letra[0]){  // encontro elementos repetidos
                         ant = p;
                         while(ant->prox != k)
                              ant = ant->prox;
                         remove = k;
                         if(p->prox == k){  // vizinhos                            
                              k = k->prox;
                              p->prox = k;
                         }
                         else if(k->prox == NULL){ // igual no final                             
                              ant->prox = NULL;
                              k = NULL;
                         }
                         else{  // os dois no meio
                              k = k->prox;
                              ant->prox = k;
                         }
                         free(remove);
                         aux--;
                    }
                    else
                        k = k->prox;
               }       
           }
     }
}

Letras *ordenaFrequencia(Letras *l){  // ordenacao por selecao
	Letras *i, *j, *aux, *ant, *maior, *pi, *pm;
	for(i = l; i != NULL; i = i->prox){
		maior = i;
		// encontra o maior
		for(j = i->prox; j != NULL; j = j->prox){
			if(j->frequencia > maior->frequencia)
                maior = j;
		}
		if(maior != i){
			if(i == l){   // i na primeira posicao				
				ant = l;
				while(ant->prox != maior)
					ant = ant->prox;
				if(maior == i->prox){   // vizinhos
					i->prox = maior->prox;
					maior->prox = i;
				}
				else if(maior->prox == NULL){   // fim
					maior->prox = i->prox;
					ant->prox = i;
					i->prox = NULL;
				}
				else{   // meio
					aux = maior->prox;		
					maior->prox = i->prox;
					i->prox = aux;		
					ant->prox = i;	
				}
				l = maior;
			}
			else{   // i no meio
				if(i->prox == maior){   // vizinhos
					aux = maior->prox;
					ant = l;
					while(ant->prox != i)
						ant = ant->prox;
					maior->prox = i;
					i->prox = aux;
					ant->prox = maior;
				}
				else if(maior->prox == NULL){   // se o menor for o ultimo
					ant = l;   // anterior do menor
					while(ant->prox != maior)
						ant = ant->prox;
					aux = l;   // anterior do i
					while(aux->prox != i)
						aux = aux->prox;	
					maior->prox = i->prox;
					aux->prox = maior;
					ant->prox = i;
					i->prox = NULL;
				}
				else{   // se o menor e o i estiver no meio
					ant = l;
					while(ant->prox != maior)
						ant = ant->prox;   // anterior ao menor
					aux = l;   
					while(aux->prox != i)
						aux = aux->prox;   // anterior ao i 
					pi = i->prox;   // posterior ao i
					pm = maior->prox;   // posterior ao menor
					i->prox = pm;  
					maior->prox = pi;
					aux->prox = maior;
					ant->prox = i;
				}
			}
			i = maior; 
		}
	}
	return l;
}

void imprimir(Letras *l, char opcao){  // imprimi a lista simples encadeada
     Letras *p;
     if(opcao == 'l')
          printf(" Letras: \t");
     else if(opcao == 'f')
          printf(" Frequencia: \t");
     for(p=l; p != NULL; p = p->prox){
         if(opcao == 'l')
              printf(" %c ", p->letra[0]);
         else if(opcao == 'f')
              printf(" %d ", p->frequencia);
     }
     printf("\n");
}

Letras *inserir(Letras *l, int i, char palavra[50]){  // lista simples encadeada, com a letras e sua frequencia
   Letras *novo = (Letras*) malloc (sizeof(Letras));
   novo->letra[0] = palavra[i];
   novo->frequencia = descobriFrequencia(palavra, strlen(palavra), i);  // encontra a frequencia de cada letra
   novo->prox = l;
   return novo;
}

int descobriFrequencia(char palavra[50], int tamanhoPalavra, int letra){
	int i, contador = 0;
	for(i=0; i<tamanhoPalavra; i++){  // percorre todas as letras
		if(palavra[letra] == palavra[i]) // encontra a letra procurada
			contador++;  
	}
	return contador;
}

main(){
	char palavra[50];
	printf("\n Digite um texto: ");
	fflush(stdin);
	gets(palavra);
	Letras *l = NULL;
	int i;
	for(i = strlen(palavra)-1; i >= 0; i--)  // inseri as letras de tras pra frente
		l = inserir(l, i, palavra);
	printf("\n\n");
	imprimir(l, 'l');  // imprimi letras
    imprimir(l, 'f');  // imprimi frequencia
	printf("\n\n");
	l = ordenaFrequencia(l);  // ordena as letras com frequencia decrescente
	retiraLetrasRepetidas(l);  // retira as letras repetidas
	imprimir(l, 'l');  // imprimi as letras
    imprimir(l, 'f');  // imprimi as frequencias
    arv *r = NULL;  // cria a arvore 
    while(l->prox->prox != NULL){  // inseri elementos até sobrar só duas letras na lista simples
        r = criaArvore(r, l);  // inseri ...
        l = ordenaFrequencia(l);  // ordena ...
        printf("\n\n"); // mostra ...
	    imprimir(l, 'l');
        imprimir(l, 'f');
    }  // apos inserir todas as letras na arvore e so restar duas
    arv *novo = (arv*) malloc(sizeof(arv));  // ligar a raiz nos dois pais restante, nesse momento a raiz so guarda os ultimos pais
    strcpy(novo->bits, "1");  // no pai recebe 1
    if(r->dir == NULL){  // se so tem um pai na raiz
        novo->dir = r->esq;   // novo pai aponta para pai guardado na raiz
        arv *e = (arv*) malloc(sizeof(arv));  // liga o pai com a letra restante
        if(l->letra[0] != '=')  // verifica se a letra eh a primeira
            e->info[0] = l->letra[0];
        else   // ou a ultima da lista simples
            e->info[0] = l->prox->letra[0];
        novo->esq = e;
        e->dir = NULL;
        e->esq = NULL;
        strcpy(e->bits, "0");  // folha esquerda recebe 0
        strcpy(novo->dir->bits, "1");  // pai guardado na raiz recebe 1
    }
    else{  // raiz possui dois pais guardados (duas somas)
        novo->esq = r->esq;  // novo pai liga com a primeira soma guardada na raiz
        strcpy(novo->esq->bits, "0");  // esquerda recebe 0
        novo->dir = r->dir->esq;  // novo pai liga com a segunda soma guardada na raiz
        strcpy(novo->dir->bits, "1");  // direita recebe 1
    }
    strcpy(novo->info, ":");  // primeiro no pai da raiz recebe : 
    r = novo;  // arvore eh criada com todos os nos pais e folhas interligados.
    printf("\n\n\n");
    printf(" ARVORE: \t");
    sequenciaBits(r);  // gera a sequencia dos bits
    imprimeArv(r, 'l');
    printf("\n\t\t");
    imprimeArv(r, 'f');  // imprimi a arvore com a sequencia de bits pra cada letra
    char palavraCompacta[100] = " ";  // palavra que vai recebe os bits de toda a palavra
    int ret;
    for(i = 0; i < strlen(palavra); i++)  // compacta a palavra digitada
          ret = compactar(r, palavra[i], palavraCompacta);  // gerando a sequencia de bits de cada letra
	printf("\n\n\n");
	printf(" COMPACTACAO: \t");
	puts(palavraCompacta);  // mostra os bits
	printf("\n\n\n");
	printf(" DESCOMPACTAR: \t");
	descomprimir(palavraCompacta, r);  // descompactar imprimindo a palavra
	printf("\n\n\n");
	system("pause");
}

