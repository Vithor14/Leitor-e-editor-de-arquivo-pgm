#include <stdio.h>
#include <stdlib.h>

void learquivo(FILE *fp,char *buffer);

void converteparaint(char *buffer,int *numeros);

int proxnumero(FILE *fp,char *buffer,int *numeros);

int main(){
	FILE *fp;
	char buffer[80];
	int numeros[45];
}

void learquivo(FILE *fp,char *buffer){
		int count;
		
		for(count=0;count<80;count++){
			buffer[count] = 0;
		}
		
		fgets(buffer,80,fp);
		
		for(count=0;count<80;count++){
			if(buffer[count] == '#' || buffer[count] == '\n' || buffer[count] == '\0'){
				buffer[count] = '\0';
				
			}
		}
	}

void converteparaint(char *buffer,int *numeros){
	
	int count,n;
	char straux[5];
	
	numeros[0] = 0;
	
	for(count = 0; buffer[count] != '\0' ;count++){
		if(buffer[count] >= '0' && buffer[count] <= '9'){			
			for(n = 0; buffer[count + n] >= '0' && buffer[count + n] <= '9' ;n++){
				straux[n] = buffer[count + n];
			}
			numeros[0]++;
			numeros[numeros[0]] = atoi(straux);
			count = count+n;
			
			for(n=0; n < 5 ; n++){
				straux[n] = 0;
			}
		}	
	}
//	printf("numeros na linha :%d\n",numeros[0]);
//	for(count = 1;count <= numeros[0] ; count++){printf("%d ",numeros[count]);}
//	printf("\n\n");

}

int proxnumero(FILE *fp,char *buffer,int *numeros){
	
	int temp = 0;
	int count;
	
	while(numeros[0] == 0){
		learquivo(fp,buffer);
		converteparaint(buffer,numeros);
	}
	printf("recebido:\n");
	for(count = 0;count <= numeros[0] ; count++){printf("%d ",numeros[count]);}
	printf("\n");
	
	temp = numeros[1];
	
	for(count = 1; count < numeros[0];count++){
		numeros[count] = numeros[count+1];
	}
	numeros[0]--;
	return temp;
}
