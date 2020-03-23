#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int llc(char trama[], int longitud,char *resultado){
  int control=0,
      sb=0,
      x=0;
  char mensaje[15]="";
  /*Si la trama no es multiplo de 16 o tienen menos de 64 bits, no es una trama
    LLC*/
  if((longitud%16)!=0 || (longitud/16)<4){
    return 0;
  }
  control=(int) trama[16];
  if((control%2)==0){
    /*Si el control es divisible entre 2 (inicia con un 0), entonces es una
    trama de Información*/
    strcpy(mensaje,"Informacion");
  }
  else{
    /*Si no es divisible entre 2 (inicia con un 1)...*/
    sb=(control-1)/2;
    if((sb%2)==0){
      /*Si el siguiente bit es un 0, es una trama de Supervición*/
      strcpy(mensaje,"Supervicion");
    }
    else{
      /*Si es un 1, es una trama no numerada*/
      strcpy(mensaje,"No numerada");
    }
  }
  /*Guardamos nuestro mensaje en la variable resultado*/
  for(x=0;x<15;x++){
    *(resultado+x)=mensaje[x];
  }
  return 1;
}

int main(void){
  char trama[64]={
    0x00, 0xa0, 0x24, 0x14, 0x49, 0x4a, 0x00, 0xa0, 0x24, 0x14, 0x49,
    0xaa, 0x00, 0x04, 0xf0, 0xf0, 0x0a, 0x3b, 0x00, 0x00, 0xff, 0xef,
    0x16, 0x0c, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x0c, 0x0f, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
  };
  char resultado[100]="hola";
  if(llc(trama,sizeof(trama),resultado)){
    printf("%s\n",resultado);
  }
}
