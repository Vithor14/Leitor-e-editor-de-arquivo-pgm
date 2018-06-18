#include<stdio.h>
#include<unistd.h>

int main(){
	int altura,largura,i=0,j=0,p=0;
	int pou=1;
	printf("INSERE A POHA DA ALTURA E DA LARGURA!\n");		// ESSA LINHA JÁ É BEM SUGESTIVA
	scanf("%d %d", &altura, &largura);
	
	int matriz[altura][largura];		// DEFINE A MATRIZ DA IMAGEM SEM FILTRO
	int matrizm[altura][largura];		// DEFINE A MATRIZ QUE VAI SER UTILIZADA PARA RECEBER A IMAGEM COM O FILTRO DA MÉDIA
	int matriz0[altura+2][largura+2];	// DEFINE A MATRIZ QUE VAI SER UTILIZADA PARA SER EDITADA UTILIZANDO A MÉDIA.
	int x=0;
	int y=0;
	int f=0;
	int v=0,d=0;
	int poi=0;
	int media[3][3]={0,0,0,0,0,0,0,0,0}; // Matriz da média
	i=0;
	j=0;
	for(i=0;i<altura;i++){    // Cria uma matriz genérica para servir como exemplo             
		
		for(j=0;j<largura;j++){
			matriz[i][j]=pou;
			matrizm[i][j]=0;
			pou++;
			printf("%d  ", matriz[i][j]);
			}
		printf("\n");
		}   
	printf("\n");
	for(i=0;i<altura+2;i++){   //faz uma matriz só com zeros para receber a imagem e criar as bordas(0);
             
       for(j=0;j<largura+2;j++){
           matriz0[i][j] =0;
		   printf("%d  ", matriz0[i][j]);
       }
		printf("\n");	
	}
	i=0;
	j=0;
	printf("\n");
	for(i=1;i<=altura;i++){       //INSERE A IMAGEM NA MATRIZ ZERADA
             
       		for(j=1;j<=largura;j++){
           		matriz0[i][j] = matriz[i-1][j-1];
			printf("%d  ", matriz0[i][j]);
		}
		printf("\n");	
	}
			
	i=0;
	j=0;
	printf("\n");
	for(i=0;i<altura+2;i++){ //Printa a imagem da matriz para ver se ocorreu tudo certo com as bordas
             
       for(j=0;j<largura+2;j++){
		   printf("%d  ", matriz0[i][j]);
       }
		printf("\n");
	}
	printf("\n");

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
	for(i=0;i<altura;i++){ //Printa a matriz final                  
		
		for(j=0;j<largura;j++){
			printf("%d  ", matrizm[i][j]);
			}
		printf("\n");
		}
	return 0;
}
