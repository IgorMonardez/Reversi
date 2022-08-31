#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<math.h>
//Igor, Fernando, Thayn�
struct jogada{
	int linha,coluna;
};

struct Posicao{
	int tabuleiro[8][8];
	int jogadorVez;
};

struct JogadaValida{
	struct jogada jog;
	struct JogadaValida *prox,*ant;
};

struct qtdPecas{
  int brancas, pretas;
};

struct qtdPecas contaPecas(struct qtdPecas Pecas, struct Posicao Tabuleiro){
  	Pecas.brancas = 0;
  	Pecas.pretas = 0;
	for(int i = 0; i<8 ; i++)
	{
    	for(int j = 0; j<8 ; j++)
		{
    		if(Tabuleiro.tabuleiro[i][j] == 1)
        		Pecas.brancas++;
      		else 
        		if(Tabuleiro.tabuleiro[i][j] == -1)
          			Pecas.pretas++;
    	}
  }
  return Pecas;
}

struct JogadaValida *Destroi(struct JogadaValida *Jogada){
	struct JogadaValida *aux = Jogada;
	Jogada->ant->prox = NULL;
	while(aux!= Jogada)
	{
		Jogada = Jogada->prox;
		free(aux);
		aux = Jogada;
	}
  	
	return NULL;
}

struct JogadaValida *CriaLista(){
	struct JogadaValida *sentinela = (struct JogadaValida*) malloc(sizeof(struct JogadaValida));
	sentinela->prox = sentinela;
	sentinela->ant = sentinela;
	return sentinela;
}

struct JogadaValida *DestroiParcial(struct JogadaValida *Jogada)
{
  	struct JogadaValida *aux = Jogada;
	Jogada->ant->prox = NULL;
	while(aux!= Jogada)
	{
		Jogada = Jogada->prox;
		free(aux);
		aux = Jogada;
	}
  	Jogada = CriaLista();
	return Jogada;
}

struct JogadaValida *CriaElemento(int linha,int coluna)
{
	struct JogadaValida *resp = (struct JogadaValida*) malloc(sizeof(struct JogadaValida));
	resp->jog.linha = linha;
	resp->jog.coluna = coluna;
	resp->prox = NULL;
	resp->ant = NULL;
	return resp;
}

void Insere(struct JogadaValida *valida, int linha, int coluna)
{
  	struct JogadaValida *novo = CriaElemento(linha,coluna);
  	novo->prox = valida;
  	novo->ant = valida->ant;
  	valida->ant->prox = novo;
  	valida->ant = novo;
}

int Busca(struct JogadaValida *Valida,int linha, int coluna){
	struct JogadaValida *aux = Valida->prox;
	while(aux != Valida)
	{
		if(aux->jog.linha == linha && aux->jog.coluna == coluna)
			return 1;
		aux = aux->prox;
	}
	return 0;
}

struct Posicao IniciaTabuleiro(){
	struct Posicao Tabuleiro;
	
	for(int i = 0;i<8;i++)
		for(int j = 0;j<8;j++)
			Tabuleiro.tabuleiro[i][j] = 0;
	
	Tabuleiro.tabuleiro[3][3] = 1;
	Tabuleiro.tabuleiro[4][4] = 1;
	Tabuleiro.tabuleiro[3][4] = -1;
	Tabuleiro.tabuleiro[4][3] = -1;
	
	return Tabuleiro;
}


//Listar com sentinela
void Listar(struct JogadaValida *valida){
	struct JogadaValida *aux = valida->prox;
	if(aux == NULL)
		return;
	while(aux != valida){
		printf("(%d,%d)",aux->jog.linha,aux->jog.coluna);
		aux = aux->prox;
	}
}



void DesenhaTabuleiro(struct Posicao Tabuleiro,struct jogada jogada){
	printf("   0   1   2   3   4   5   6   7 \n");
	int i = 0, j = 0;
	for(i =0;i<8;i++)
	{
    	printf("%d ", i);
		for(j = 0;j<8;j++)
		{
			if(j == 0)
			{
				if(jogada.linha == i && jogada.coluna == j)
					printf("%2d|",Tabuleiro.tabuleiro[i][j]);
				else
					printf("%2d ",Tabuleiro.tabuleiro[i][j]);
			}
			else
			{
				if(jogada.linha == i && jogada.coluna == j)
					printf("|%2d|",Tabuleiro.tabuleiro[i][j]);
				else		
					printf("|%2d ",Tabuleiro.tabuleiro[i][j]);
			}	
			
			
		}
		printf("|");
		printf("\n");
		j = 0;
	}
}


struct JogadaValida *DireitaEsquerda(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
  	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i][j-x] == Tabuleiro.jogadorVez*(-1))
     	 	cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	j = j-x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;
					
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *EsquerdaDireita(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
  	int x, cont = 0;
  	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i][j+x] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	j = j+x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;	
		if(Busca(aux,i,j) == 1)
			return aux;		
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *CimaBaixo(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
  	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i-x][j] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	i = i-x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;		
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *BaixoCima(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
  	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i+x][j] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
     	i = i+x;
     	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;	
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *DiagonalCimaEsquerdaDireita(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i+x][j+x] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	j = j+x;
    	i = i+x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;		
		Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *DiagonalBaixoEsquerdaDireita(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i-x][j-x] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	j = j-x;
    	i = i-x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;	
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *DiagonalCimaDireitaEsquerda(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i-x][j+x] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	j = j+x;
    	i = i-x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;		
    	Insere(aux,i,j);
  	}
  	return aux;
}

struct JogadaValida *DiagonalBaixoDireitaEsquerda(struct JogadaValida *aux,int i,int j,struct Posicao Tabuleiro){
	int x, cont = 0;
	for(x = 1; x<=j;x++){
    	if(Tabuleiro.tabuleiro[i+x][j-x] == Tabuleiro.jogadorVez*(-1))
      		cont++;
    	else
      		break;
  	}
  	if(cont>0){
    	i = i+x;
    	j = j-x;
    	if((i < 0 || i >=8) || (j<0 || j>=8) )
    		return aux;
    	if(Tabuleiro.tabuleiro[i][j] == 1 ||Tabuleiro.tabuleiro[i][j] == -1)
			return aux;
		if(Busca(aux,i,j) == 1)
			return aux;		
    	Insere(aux,i,j);
  	}
  	return aux;
}
struct JogadaValida *CalculaJogadasValidas(struct Posicao Tabuleiro){
  	struct JogadaValida *aux = CriaLista();
  	for(int i = 0;i<8;i++)
  	{
    	for(int j = 0;j<8;j++)
    	{
      		if(Tabuleiro.tabuleiro[i][j] == Tabuleiro.jogadorVez)
      		{
        		aux = DireitaEsquerda(aux,i,j,Tabuleiro);
        		aux = EsquerdaDireita(aux,i,j,Tabuleiro);
	        	aux = CimaBaixo(aux,i,j,Tabuleiro);
	        	aux = BaixoCima(aux,i,j,Tabuleiro);
	        	aux = DiagonalCimaEsquerdaDireita(aux,i,j,Tabuleiro);
	        	aux = DiagonalBaixoEsquerdaDireita(aux,i,j,Tabuleiro);
	        	aux = DiagonalCimaDireitaEsquerda(aux,i,j,Tabuleiro);
	        	aux = DiagonalBaixoDireitaEsquerda(aux,i,j,Tabuleiro);
      		}
      	}
  	}
  	if(aux->prox == aux)
  	{
  		aux = Destroi(aux);
  		return NULL;
	}
  	return aux;
}

int VerificarFim(struct Posicao x){ 
	int verificador = 0;
	 
	for(int i = 0;i<8;i++)
		for(int j = 0;j<8;j++)
			if(x.tabuleiro[i][j] == 0)
				verificador = 1;	//enquanto tiver ao menos 1 casa vazia o jogo nao para.
	struct JogadaValida *valida = CalculaJogadasValidas(x);
	if(valida == NULL && verificador != 0)
	{
		x.jogadorVez *= -1;
		valida = CalculaJogadasValidas(x);
		if(valida == NULL)
		{
			printf("Jogo acabou por falta de jogadas v�lidas.\n\a");
			verificador = 0;
		}
		
	}			
    		
	return verificador;		
}

void printPecas(struct Posicao PosAtual)
{
	struct qtdPecas Pecas = contaPecas(Pecas,PosAtual);
	printf("Pretas - %d Brancas - %d\n",Pecas.pretas,Pecas.brancas);  
}

int Erro(struct jogada Erro, struct JogadaValida *valida)
{
  	struct JogadaValida *aux = valida->prox;
  	while(aux!= valida)
  	{
    	if(aux->jog.linha == Erro.linha && aux->jog.coluna == Erro.coluna)
    		return 1;
		
    	aux = aux->prox;
  	}
  	printf("Jogada Inv�lida, tente outra \a");
  	return 0;
}

void desenhaTabuleiroJogadas(struct JogadaValida *valida,struct Posicao PosAtual,struct jogada jogada){
	int i,j;
	printf("   0   1   2   3   4   5   6   7 \n");
	for(int i = 0;i<8;i++)
	{
		printf("%d ", i);
		for(int j = 0;j<8;j++)
		{
			if(j == 0)
			{
				if(jogada.linha == i && jogada.coluna == j)
					printf("%2d|",PosAtual.tabuleiro[i][j]);
				else	
					if(Busca(valida,i,j) == 1)
						printf(" x ");
					else
						printf("%2d ",PosAtual.tabuleiro[i][j]);
			}
			else
			{
				if(jogada.linha == i && jogada.coluna == j)
					printf("|%2d|",PosAtual.tabuleiro[i][j]);
				else
					if(Busca(valida,i,j) == 1)
						printf("| x ");
					else
						printf("|%2d ",PosAtual.tabuleiro[i][j]);
			}
			
		}
		printf("|");
		printf("\n");
		j = 0;
	}
}

struct jogada EscolheJogada(struct JogadaValida *valida,struct Posicao PosAtual,struct jogada jogada){
  	struct jogada Jogada;
	desenhaTabuleiroJogadas(valida, PosAtual,jogada);
  	do 
  	{
    	printf("\nJogue: ");
	  	scanf("%d %d",&Jogada.linha, &Jogada.coluna);
  	}
 	while(Erro(Jogada,valida) == 0);
  	return Jogada;
}



int PodeJogar(struct Posicao *PosAtual, struct jogada jog, int deltaLinha, int deltaColuna)
{
    int i = jog.linha + deltaLinha;
    int j = jog.coluna + deltaColuna;

    while (i>=0 && j>=0 && i<8 && j<8 && PosAtual->tabuleiro[i][j] == -PosAtual->jogadorVez)
	{
        i += deltaLinha;
        j += deltaColuna;
    }

    if (i<0||j<0||i>=8||j>=8)
		return 0;    
    if (PosAtual->tabuleiro[i][j]==0) 
		return 0;     
		
    return 1;                              
}

void Troca(struct Posicao *PosAtual, struct jogada jog, int deltaLinha, int deltaColuna)
{
    int i = jog.linha + deltaLinha;
    int j = jog.coluna + deltaColuna;

    while (PosAtual->tabuleiro[i][j]==-PosAtual->jogadorVez){
        PosAtual->tabuleiro[i][j] = -PosAtual->tabuleiro[i][j];
        i += deltaLinha;
        j += deltaColuna;
    }
}

void ExecutaJogada(struct Posicao *PosAtual, struct jogada jog){
  	
  	if (jog.linha<0||jog.coluna<0||jog.linha>=8||jog.coluna>=8||PosAtual->tabuleiro[jog.linha][jog.coluna]!=0) 
	  	return;
	  	
    int deltaColuna,deltaLinha;
    int resp = 0;

    for (deltaLinha = -1;deltaLinha<=1;deltaLinha++)
	{
        for (deltaColuna=-1;deltaColuna<=1;deltaColuna++)
		{
            if (deltaLinha!=0||deltaColuna!=0)
			{
                if (PodeJogar(PosAtual,jog,deltaLinha,deltaColuna)>0)
				{
                    Troca(PosAtual,jog,deltaLinha,deltaColuna);
                    PosAtual->tabuleiro[jog.linha][jog.coluna] = PosAtual->jogadorVez;
                    
                }
            }
        }
    }
	PosAtual->jogadorVez *= -1;
    
}



void verificarVencedor(struct Posicao Tabuleiro){	
  	struct qtdPecas Pecas;
  	Pecas = contaPecas(Pecas,Tabuleiro);
  	if(Pecas.brancas > Pecas.pretas)
  	{
    	printf("Com %d a %d\n", Pecas.brancas, Pecas.pretas);
    	printf("O branco � o vencedor\n");
  	}
  	else
    	if(Pecas.pretas > Pecas.brancas)
    	{
      		printf("Com %d a %d\n", Pecas.pretas, Pecas.brancas);
      		printf(" O preto � o vencedor\n");
    	}
    	else 
      		if(Pecas.pretas == Pecas.brancas)
      		{
		        printf("Com %d a %d\n", Pecas.brancas,Pecas.pretas);
		        printf("A partida deu empate!\n");
      		}
}

double AvaliaPosicao(struct Posicao PosCopia)
{
  	struct qtdPecas Pecas;
  	Pecas = contaPecas(Pecas, PosCopia);
  	double valor = 0;
   	valor = (((double)Pecas.pretas)/(Pecas.brancas));
	return valor;
}


struct jogada ExecutaIA(struct Posicao PosAtual,int nivel,double alfa,double beta)
{
	struct JogadaValida *lista;
	struct JogadaValida *jogadaAux;
    struct jogada melhorJogada;
    struct jogada jogadaIA;
    double melhorValor, valorJogada;
	int podado = 0;
    
    if(nivel%2 == 0){
    	melhorValor = -INFINITY;
    }else{
    	melhorValor = +INFINITY;	
    }  	
	    
  	lista = CalculaJogadasValidas(PosAtual);
	jogadaAux = lista->prox;
	
	while (jogadaAux != lista && podado == 0){
	   	  	
		struct Posicao posCopia = PosAtual;
		    
	    ExecutaJogada(&posCopia,jogadaAux->jog);
	       
	    posCopia = PosAtual;
		  		    
		if(nivel < 5)
	    {
	      	if(nivel%2 == 0)
	      	   	jogadaIA = ExecutaIA(posCopia,nivel+1,melhorValor,beta);
	        else
	      	   	jogadaIA = ExecutaIA(posCopia, nivel+1, alfa, melhorValor);
	      	
	      	ExecutaJogada(&posCopia,jogadaIA);
	    }
	     
	    valorJogada = AvaliaPosicao(posCopia);
	        
	    
	    if((valorJogada < alfa) || (valorJogada > beta))
	    	podado = 1;
	   	    
	    if(nivel % 2 == 0)
	    { 
	        if(valorJogada >= melhorValor )
	        {
	        	melhorValor = valorJogada;
	            melhorJogada = jogadaAux->jog;
	        }
	    }
	    else 
	        if(valorJogada <= melhorValor )
	        {
	            melhorValor = valorJogada;
	            melhorJogada = jogadaAux->jog;
	        }
		jogadaAux = jogadaAux->prox; 
	}
	lista = Destroi(lista);
   	   
    return melhorJogada;
}

void SalvaJogadaBIN(FILE *arq, struct jogada *jogada)
{
	if(jogada->linha == -1 && jogada->coluna == -1)
	{
		arq = fopen("jogadas.bin","ab");
		fclose(arq);
	}
	else
	{
		arq = fopen("jogadas.bin","ab");
		fwrite(jogada,sizeof(int),2,arq);
		fclose(arq);
	}	
}



int main()
{
  	setlocale(LC_ALL, "Portuguese");
  	FILE *arq;
	struct Posicao Tabuleiro = IniciaTabuleiro();
	struct jogada Jogada;
	Jogada.linha = -1;
	Jogada.coluna = -1;
  	struct JogadaValida *valida = CriaLista();
  	
  	SalvaJogadaBIN(arq,&Jogada);
  		
	Tabuleiro.jogadorVez = -1;
	do
	{
		valida = CriaLista();
	  	system("cls");
	  	printf("Legenda: |- Jogada anterior (da IA)\n");
		printf("         x - Poss�veis jogadas - Preto (-1)\n"); 
	  	if(Tabuleiro.jogadorVez == -1)
	  	{
	  		printf("Jogador da Vez � o Preto (-1)\n");
	    	valida = CalculaJogadasValidas(Tabuleiro);
	    	if(valida == NULL)
	    	{
	    		Jogada.linha = -1;
	    		Jogada.coluna = -1;
	    	}
			else
			{
				printPecas(Tabuleiro);
				Jogada = EscolheJogada(valida,Tabuleiro,Jogada);
				valida = DestroiParcial(valida);
	   		}
        }
	    else
		{
        	printf("Jogador da vez � o branco(1)\n");
        	valida = CalculaJogadasValidas(Tabuleiro);
        	if(valida == NULL)
	    	{
	    		Jogada.linha = -1;
	    		Jogada.coluna = -1;
	    	}
	    	else
        		Jogada = ExecutaIA(Tabuleiro,0,-INFINITY,+INFINITY);
            
        }
        SalvaJogadaBIN(arq,&Jogada);
		ExecutaJogada(&Tabuleiro,Jogada);
	}
	while(VerificarFim(Tabuleiro) == 1); //verificarfim vai retornar 1 enquanto tiver posi��o com "0" ou n�o houver mais jogadas validas para ambos os jogadores
	
	DesenhaTabuleiro(Tabuleiro,Jogada);
  	verificarVencedor(Tabuleiro);
  	valida = Destroi(valida);
  	return 0;
}

