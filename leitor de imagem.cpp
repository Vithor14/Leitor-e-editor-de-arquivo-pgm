#include <stdio.h>
#include <stdlib.h>

int linhas,colunas;

void learquivo(FILE *fp,char *buffer);

void converteparaint(char *buffer,int *numeros);

int proxnumero(FILE *fp,char *buffer,int *numeros);

int **AlocMatriz(int m, int n);

int **LibMatriz(int m, int n, int **v);

void nomearquivo(char *arquivofinal,char *arquivo);

void media3x3(int altura, int largura, int **matriz, int **matrizm);

int** laplace(int **matrizArquivo,int **matrizfinal);

int media5x5(int altura, int largura, int **matriz, int **matrizm); 

int main(){
	
	//arquivo e matriz com imagem original e imagem final
	FILE *fp;
	int **imagem;
	int **imagemfim;
	
	//altura,largura,tons,nome do arquivo a ser lido, largura eh a qualtidade de colunas que a imagem tem, e altura a quantidade de linhas
	int largura;
	int altura;
	int tons;
	char arquivo[80] = {'t','e','s','t','e','.','p','g','m',0};
	char arquivofinal[80];
	
	//variaveis de controle de leitura: buffer e ponteiro pra vetor dos numeros, alem das variaveis i e j para colocar os dados na matriz
	char buffer[80];
	int numeros[45] = {0,0,0,0,0,0};
	int i,j;
	
	//variavel para escolher o modo de edição e contador para varios propositos
	char modo;
	int count;
	
	
	printf("digite o nome do arquivo original\n");
	gets(arquivo);
	
	fp = fopen(arquivo, "r");	
	if (fp == NULL) {
        printf("Erro de escrita de arquivo\n");
        exit(1);
    }


	learquivo(fp,buffer);
	if( !(buffer[0] == 'P' && buffer[1] == '2') ){
		printf("formato de arquivo incorreto\n");
		fclose(fp);
		exit(1);
	}
	
	largura = proxnumero(fp,buffer,numeros);
	altura = proxnumero(fp,buffer,numeros);
	tons = proxnumero(fp,buffer,numeros);
	linhas = altura;
	colunas = largura;
	
	
	printf("\nSua imagem tem tamanho %d*%d, e %d tons de cinza\n",largura,altura,tons);
	
	imagem = AlocMatriz(altura,largura);
	imagemfim = AlocMatriz(altura,largura);
	
	for(i = 0; i < altura; i++){
		for(j = 0; j < largura;j++){
			imagem[i][j] = proxnumero(fp,buffer,numeros);
		}
	}
	
	printf("imagem carregada\n\n");
	fclose(fp);
	
//	mostra toda a imagem
//	for(i = 0; i < altura; i++){
//		for(j = 0; j < largura;j++){
//			printf("%2d ",imagem[i][j]);
//		}
//		printf("\n");
//	}
	
	printf("\n");
	printf("o que deseja fazer%c\n\n",63);
	printf("1 - Mascara laplaciana 3x3\n");
	printf("2 - Mascara 5x5\n");
	printf("3 - Media 3x3\n");
	printf("4 - Media 5x5\n");
	printf("5 - Media 7x7\n");
	printf("9 - Copiar o arquivo\n");
	printf("0 - Sair\n");
	
	scanf("%c",&modo);
	getchar();
	if(modo == '1'){
		laplace(imagem,imagemfim);
	}
	else if(modo == '2'){
		
	}
	else if(modo == '3'){
		media3x3(altura,largura,imagem,imagemfim);
	}
	else if(modo == '4'){
		media5x5(altura,largura,imagem,imagemfim);
	}
	else if(modo == '5'){
		
	}
	else if(modo == '9'){
		for(i = 0; i < altura; i++){
			for(j = 0; j < largura;j++){
				imagemfim[i][j] = imagem[i][j];
			}
		}
	}	
	else if(modo == '0'){
		printf("Nada foi feito\nSaindo...\n");
		LibMatriz(altura,largura,imagem);
		exit(1);
	}
	
	for(i = 0; i < altura; i++){
		for(j = 0; j < largura;j++){
			if(imagemfim[i][j] > tons){
				imagemfim[i][j] = tons;
			}
			else if(imagemfim[i][j] < 0){
				imagemfim[i][j] = 0;
			}
		}
	}
	
//	printa resultado da funcaoo
	for(i = 0; i < altura; i++){
		for(j = 0; j < largura;j++){
			printf("%2d ",imagemfim[i][j]);
		}
		printf("\n");
	}
	
	nomearquivo(arquivofinal,arquivo);	
	fp = fopen(arquivofinal, "w");
    if (fp == NULL) {
        printf("Erro de escrita de arquivo\n");
        exit(1);
    }
	
	fprintf(fp,"P2\n");
	fprintf(fp,"%d %d\n", largura, altura);
	fprintf(fp,"%d\n", tons);
	
	for (i = 0,count = 0; i < altura; i++) {
	    for (j = 0; j < largura; j++,count++) {
	        fprintf(fp,"%3.d ", imagemfim[i][j]);
	    	if(count % 17 == 0){
	           	fprintf(fp,"\n");
	        }
	    }
	}
	
	
	
	LibMatriz(altura,largura,imagem);
	fclose(fp);
	return 0;
}

void learquivo(FILE *fp,char *buffer){
		
		//a função deste codigo é ler 1 linha do arquivo de imagem, e remover, se houver,os comentarios trocando o inicializador deles por "\0" e passando essa linha pro vetor "buffer"
		int count;
		
		//zerando o buffer pra não dar merda
		for(count=0;count<80;count++){
			buffer[count] = 0;
		}
		
		fgets(buffer,80,fp);
		
		//o # é o simbolo que se tem antes de algum comentario na image, e eu troco o "\n" por "\0" para garantir que o fim da linha é o fim da string 
		for(count=0;count<80;count++){
			if(buffer[count] == '#' || buffer[count] == '\n' || buffer[count] == '\0'){
				buffer[count] = '\0';
				break;
			}
		}
	}

void converteparaint(char *buffer,int *numeros){
	//essa função tem a utilidade de pegar a linha só composta de numeros da função "learquivo" e os converter para inteiros
	int count,n;
	char straux[5];
	
	//numeros[0] é basicamente a quantidade de numeros que ainda tem no vetor, isso é usado para fazer uma pilha,boto no mesmo vetor e para economizar passagem de valores
	numeros[0] = 0;
	
	//aqui count vai passando pelo vetor procurando um numero "buffer[count] >= '0' && buffer[count] <= '9'" e se o encontra inicia um segundo for
	//esse segundo for começa em n=0, e vai caminhando apos esse numero ja encontrado, ate achar o fim dele,a cada passo copia um char da string pra uma string auxiliar "straux" 
	//e quando encontra o fim do numero aumenta o contador numeros[0], marcando que encontrou mais um numero nessa linha
	//e então converte essa string auxiliar, usando o atoi, para um numero inteiro, e insere esse numero na posição numeros[0](que é a quantidade de numeros,indice do ultimo) 
	for(count = 0; buffer[count] != '\0' ;count++){
		if(buffer[count] >= '0' && buffer[count] <= '9'){			
			for(n = 0; buffer[count + n] >= '0' && buffer[count + n] <= '9' ;n++){
				straux[n] = buffer[count + n];
			}
			numeros[0]++;
			numeros[numeros[0]] = atoi(straux);
			count = count+n;
			
			//zera a string auxiliar, evita tretas onde se le um numero com menos caracteres que o anterior ex: le 11,converte pra int corretamente então le 5, converte 51(devia ser 5)
			for(n=0; n < 5 ; n++){
				straux[n] = 0;
			}
		}	
	}
	//como o vetor numeros foi passado por ponteiros não precisa de retorno
	
	
//	printa os numeros convertidos na execução atual, para debug
//	printf("numeros na linha :%d\n",numeros[0]);
//	for(count = 1;count <= numeros[0] ; count++){printf("%d ",numeros[count]);}
//	printf("\n\n");

}

int proxnumero(FILE *fp,char *buffer,int *numeros){
	//função que organiza o vetor numeros como pilhar, primeiro verificando se ha mais de 1 numero na pilha, se não houver, ele pede mais
	//se tiver numeros ele coloca o primeiro valor em uma variavel auxiliar, move tudo 1 indice pra cima, e retorna a variavel auxiliar
	int temp = 0;
	int count;
	
	//o while é necessario para caso haja multiplas linhas de numeros, com comentarios por exemplo
	while(numeros[0] == 0){
		learquivo(fp,buffer);
		converteparaint(buffer,numeros);
	}
	
	//codigo debug que printa todo o vetor numeros, se o vetor numeros não estiver zerado na primeira execução, eh possivel descobrir aqui se deu merda
//	printf("recebido:\n");
//	for(count = 0;count <= numeros[0] ; count++){printf("%d ",numeros[count]);}
//	printf("\n");
	
	//tira do topo da pilha e bota numa var temporaria
	temp = numeros[1];
	//move toda a pilha pra cima
	for(count = 1; count < numeros[0];count++){
		numeros[count] = numeros[count+1];
	}
	//diminui a quantidade de numeros na pilha
	numeros[0]--;
	//retorna o numero do topo
	return temp;
}

int **AlocMatriz (int m, int n){
    int **v; 
    int i;
      if (m < 1 || n < 1) { 
        printf ("Erro: Parametro invalido\n");
           return (NULL);
    }
 
    v = (int **) calloc (m, sizeof(int *));

    if (v == NULL) {
        printf ("Erro: Memoria Insuficiente");
        return (NULL);
    }
 
    for ( i = 0; i < m; i++ ) {
        v[i] = (int*) calloc (n, sizeof(int));
    
           if (v[i] == NULL) {
               printf ("Erro: Memoria Insuficiente");
            return (NULL);
        }
    }
      return (v); 
}

int **LibMatriz (int m, int n, int **v){
    int  i; 
    if (v == NULL){
    return (NULL);
}
    if (m < 1 || n < 1) { 
    printf ("** Erro: Parametro invalido **\n");
    return (v);
    }
      for (i=0; i<m; i++){
        free (v[i]); 
        }
      free (v);      
      return (NULL);
}

void nomearquivo(char *arquivofinal,char *arquivo){
	int ok = 0,count;
	char op;
	
	while(ok == 0){
		printf("digite o nome do arquivo que deseja salvar\n");
		
		gets(arquivofinal);
		
		for(count = 0;arquivofinal[count] != 0 && arquivo[count] != 0;count++){
			if(arquivofinal[count] != arquivo[count]||arquivofinal[count+1] != arquivo[count+1]){
				ok = 1;
				break;
			}
			if(arquivofinal[count + 1] == 0 && arquivo[count + 1] == 0){
				printf("deseja sobrescrever o arquivo original?\nS ou N\n");
				scanf("%c",&op);
				getchar();
				if(op == 'S' || op == 's'){
					ok = 1;
					break;
				}
				else{
					ok = 0;
					break;
				}
			}
				
				
		}
		
	}
}

void media3x3(int altura, int largura, int **matriz, int **matrizm){
	int i=0,j=0;

	int matriz0[altura+2][largura+2];	// DEFINE A MATRIZ QUE VAI SER UTILIZADA PARA SER EDITADA UTILIZANDO A MÉDIA.
	int f=0;
	int v=0,d=0;
	int pou=0;
	int media[3][3]={0,0,0,0,0,0,0,0,0}; // Matriz da média
	i=0;
	j=0;
	
	for(i=0;i<altura+2;i++){   //faz uma matriz só com zeros para receber a imagem e criar as bordas(0);
             
       for(j=0;j<largura+2;j++){
           matriz0[i][j] =0;
//		   printf("%d  ", matriz0[i][j]);
       }
//		printf("\n");	
	}
	i=0;
	j=0;
//	printf("\n");
	for(i=1;i<=altura;i++){       //INSERE A IMAGEM NA MATRIZ ZERADA
             
       		for(j=1;j<=largura;j++){
           		matriz0[i][j] = matriz[i-1][j-1];
//				printf("%d  ", matriz0[i][j]);
			}
//		printf("\n");	
	}
			
	i=0;
	j=0;
//	printf("\n");
//	for(i=0;i<altura+2;i++){ //Printa a imagem da matriz para ver se ocorreu tudo certo com as bordas
//             
//       for(j=0;j<largura+2;j++){
//		   printf("%2d ", matriz0[i][j]);
//       }
//		printf("\n");
//	}
//	printf("\n");

	i=1;
	j=1;
	for(i=1,v=0;i<=altura;i++,v++){ // calcula a média  usando a matriz zerada               
		
		for(j=1,d=0;j<=largura;j++,d++){

			 			media[0][0]=matriz0[i-1][j-1];
			 			f+=media[0][0];
			 			media[0][1]=matriz0[i-1][j];
			 			f+=media[0][1];
			 			media[0][2]=matriz0[i][j+1];
			 			f+=media[0][2];
			 			media[1][0]=matriz0[i][j-1];
			 			f+=media[1][0];
			 			media[1][1]=matriz0[i][j];
			 			f+=media[1][1];
			 			media[1][2]=matriz0[i-1][j+1];
			 			f+=media[1][2];
			 			media[2][0]=matriz0[i+1][j-1];
			 			f+=media[2][0];
			 			media[2][1]=matriz0[i+1][j];  
			 			f+=media[2][1];  	
			       		media[2][2]=matriz0[i+1][j+1];
			       		f+=media[2][2];
                		pou=f/9;
                		matrizm[v][d]=pou; //Insere a média na matriz final
                		pou=0;
                		f=0;
		}
	}
        i=0;
	j=0;
//	for(i=0;i<altura;i++){ //Printa a matriz final                  
//		
//		for(j=0;j<largura;j++){
//			printf("%d  ", matrizm[i][j]);
//			}
//		printf("\n");
//		}
	
}

int** laplace(int **matrizArquivo,int **matrizfinal) {
    int laPlace[3][3] = {0,-1,0,
                     -1,4,-1,
                      0,-1,0};
    int **matriz;
    int s, t,i,j;
    int count1, count2;

    matriz = AlocMatriz(colunas + 2,linhas + 2);
    
    for (i = linhas + 2; i < linhas; i++) {
        for (j = colunas + 2; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }


    for (int i = 1; i < linhas + 1; i++) {
        for (int j = 1; j < colunas + 1; j++) {
            matriz[i][j] = matrizArquivo[i - 1][j - 1];
        }
    }
    for (int i = 1; i < linhas+1; i++) {
        for (int j = 1; j < colunas+1; j++) {

            int soma = 0;
            for (count1 = 0, s = i - 1; s < i + 2; s++,count1++) {
                for (count2 = 0, t = j - 1; t < j + 2; t++,count2++) {
                    soma += matriz[s][t] * laPlace[count1][count2];
                }
            }
//            if (soma > 255)
//                soma = 255;
//            else if (soma < 0)
//                soma = 0;

            matrizfinal[i - 1][j - 1] = soma;
        }
    }
    return matrizfinal;
}

int media5x5(int altura, int largura, int **matriz, int **matrizm){

	int i=0,j=0;

	int matriz0[altura+4][largura+4];	// DEFINE A MATRIZ QUE VAI SER UTILIZADA PARA SER EDITADA UTILIZANDO A MÉDIA.
	int f=0;
	int v=0,d=0;
	int pou=0;
	int media[5][5]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Matriz da média
	i=0;
	j=0;
	
	for(i=0;i<altura+4;i++){   //faz uma matriz só com zeros para receber a imagem e criar as bordas(0);
             
       for(j=0;j<largura+4;j++){
           matriz0[i][j] =0;
//		   printf("%d  ", matriz0[i][j]);
       }
//		printf("\n");	
	}
	i=0;
	j=0;
//	printf("\n");
	for(i=2;i<=altura+1;i++){       //INSERE A IMAGEM NA MATRIZ ZERADA
             
       		for(j=2;j<=largura+1;j++){
           		matriz0[i][j] = matriz[i-2][j-2];
//			printf("%d  ", matriz0[i][j]);
		}
//		printf("\n");	
	}
			
	i=0;
	j=0;
//	printf("\n");
//	for(i=0;i<altura+4;i++){ //Printa a imagem da matriz para ver se ocorreu tudo certo com as bordas
//             
//       for(j=0;j<largura+4;j++){
//		   printf("%d  ", matriz0[i][j]);
//       }
//		printf("\n");
//	}
//	printf("\n");

	i=1;
	j=1;
	for(i=2,v=0;i<=altura+1;i++,v++){ // calcula a média  usando a matriz zerada               
		
		for(j=2,d=0;j<=largura+1;j++,d++){

			 			media[0][0]=matriz0[i-2][j-2];
			 			f+=media[0][0];
			 			media[0][1]=matriz0[i-2][j-1];
			 			f+=media[0][1];
			 			media[0][2]=matriz0[i-2][j];
			 			f+=media[0][2];
			 			media[0][3]=matriz0[i-2][j+1];
			 			f+=media[0][3];
			 			media[0][4]=matriz0[i-2][j+2];
			 			f+=media[0][4];
			 			media[1][0]=matriz0[i-1][j-2];
			 			f+=media[1][0];
			 			media[1][1]=matriz0[i-1][j-1];
			 			f+=media[1][1];
			 			media[1][2]=matriz0[i-1][j];
			 			f+=media[1][2];
			 			media[1][3]=matriz0[i-1][j+1];
			 			f+=media[1][3];
			 			media[1][4]=matriz0[i-1][j+2];
			 			f+=media[1][4];
			 			media[2][0]=matriz0[i][j-2];
			 			f+=media[2][0];
			 			media[2][1]=matriz0[i][j-1];
			 			f+=media[2][1];
			 			media[2][2]=matriz0[i][j];
			 			f+=media[2][2];
			 			media[2][3]=matriz0[i][j+1];
			 			f+=media[2][3];
			 			media[2][4]=matriz0[i][j+2];
			 			f+=media[2][4];
			 			media[3][0]=matriz0[i+1][j-2];
			 			f+=media[3][0];
			 			media[3][1]=matriz0[i+1][j-1];
			 			f+=media[3][1];
			 			media[3][2]=matriz0[i+1][j];
			 			f+=media[3][2];
			 			media[3][3]=matriz0[i+1][j+1];
			 			f+=media[3][3];
			 			media[3][4]=matriz0[i+1][j+2];
			 			f+=media[3][4];
			 			media[4][0]=matriz0[i+2][j-2];
			 			f+=media[4][0];
			 			media[4][1]=matriz0[i+2][j-1];
			 			f+=media[4][1];
			 			media[4][2]=matriz0[i+2][j];
			 			f+=media[4][2];
			 			media[4][3]=matriz0[i+2][j+1];
			 			f+=media[4][3];
			 			media[4][4]=matriz0[i+2][j+2];
			 			f+=media[4][4];
			 			
                		pou=f/25;
                		matrizm[v][d]=pou; //Insere a média na matriz final
                		pou=0;
                		f=0;
		}
	}
        i=0;
	j=0;
//	for(i=0;i<altura;i++){ //Printa a matriz final                  
//		
//		for(j=0;j<largura;j++){
//			printf("%d  ", matrizm[i][j]);
//			}
//		printf("\n");
//		}
	return 0;
}
