#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int llc(char trama[], int longitud, char *resultado){
  int control=0,
      sb=0,
      bits1[7],
      bits2[7],
      x=0,
      y=0,
      s=0,
      r=0,
      p=1;
  char mensaje[50]="",
       Ns[3]="",
       Nr[3]="";
  /*Si la trama no es multiplo de 16 o tienen menos de 64 bits, no es una trama
  LLC*/
  if((longitud%16)!=0 || (longitud/16)<4){
    return 0;
  }
  control=(int) trama[16];
  if((control%2)==0){
    /*Si el control es divisible entre 2 (inicia con un 0), entonces es una
    trama de Información*/
    strcpy(mensaje,"I, N(S)=");
    sb=control/2;
    if(trama[12]==0x00 && trama[13]<=0x03){
      /*Si el tamaño de la trama es igual o menor a 3 entonces el control es de
      1 byte*/
      /*Es siguiente for guarda los 3 siguientes bits que representan N(S)*/
      for(x=0;x<3;x++){
        if((sb%2)==0){
          bits1[x]=0;
          sb=sb/2;
        }
        else{
          bits1[x]=1;
          sb=(sb-1)/2;
        }
      }
      /*El siguiente if nos permite ignorar el valor de p/f debido a que no lo
      ocuparemos aun*/
      if((sb%2)==0){
        sb=sb/2;
      }
      else{
        sb=(sb-1)/2;
      }
      /*Es siguiente for guarda los 3 siguientes bits que representan N(R)*/
      for(x=0;x<3;x++){
        if((sb%2)==0){
          bits2[x]=0;
          sb=sb/2;
        }
        else{
          bits2[x]=1;
          sb=(sb-1)/2;
        }
      }
    }
    else{
      /*Si el tamaño de la trama es mayor a 3, entonces el control es de 2 bytes
      */
      /*Es siguiente for guarda los 7 siguientes bits que representan N(S)*/
      for(x=0;x<7;x++){
        if((sb%2)==0){
          bits1[x]=0;
          sb=sb/2;
        }
        else{
          bits1[x]=1;
          sb=(sb-1)/2;
        }
      }
      /*Cargamos el segundo bit del control*/
      control=(int) trama[17];
      /*El siguiente if nos permite ignorar el valor de p/f debido a que no lo
      ocuparemos aun*/
      if((control%2)==0){
        sb=control/2;
      }
      else{
        sb=(control-1)/2;
      }
      /*Es siguiente for guarda los 7 siguientes bits que representan N(R)*/
      for(x=0;x<7;x++){
        if((sb%2)==0){
          bits2[x]=0;
          sb=sb/2;
        }
        else{
          bits2[x]=1;
          sb=(sb-1)/2;
        }
      }
    }
    /*Los siguientes 2 for nos permiten tranformar los valores de N(S) y N(R) de
     binario a decimal*/
    for(x=0;x<7;x++){
      if(bits1[x]==1){
        for(y=0;y<x;y++){
          p*=2;
        }
        s+=p;
        p=1;
      }
    }
    for(x=0;x<7;x++){
      if(bits2[x]==1){
        for(y=0;y<x;y++){
          p*=2;
        }
        r+=p;
        p=1;
      }
    }
    /*Pasamos los valores de N(S) y N(R) a valores char[]*/
    sprintf(Ns,"%d",s);
    sprintf(Nr,"%d",r);
    /*Juntamos toda la información en un unico mensaje*/
    strcat(mensaje,Ns);
    strcat(mensaje,", N(R)=");
    strcat(mensaje,Nr);
  }
  else{
    /*Si no es divisible entre 2 (inicia con un 1)...*/
    sb=(control-1)/2;
    if((sb%2)==0){
      /*Si el siguiente bit es un 0, es una trama de Supervición*/
      sb=sb/2;
      if((sb%2)==0){
        sb=sb/2;
        if((sb%2)==0){
          /*Si los siguientes 2 bits son 00 es una trama tipo RR*/
          strcpy(mensaje,"RR, N(R)=");
        }
        else{
          /*Si los siguientes 2 bits son 01 es una trama tipo REJ*/
          strcpy(mensaje,"REJ, N(R)=");
        }
      }
      else{
        sb=(sb-1)/2;
        if((sb%2)==0){
          /*Si los siguiente 2 bits son 10 es una trama tipo RNR*/
          strcpy(mensaje,"RNR, N(R)=");
        }
        else{
          /*Si los siguiente 2 bits son 11 es una trama tipo SREJ*/
          strcpy(mensaje,"SREJ, N(R)=");
        }
      }
      if(trama[12]==0x00 && trama[13]<=0x03){
        /*Si el tamaño de la trama es igual o menor a 3 entonces el control es
        de 1 byte*/
        /*El siguiente if nos permite ignorar el valor de p/f debido a que no lo
        ocuparemos aun*/
        if((sb%2)==0){
          sb=sb/2;
        }
        else{
          sb=(sb-1)/2;
        }
        /*Es siguiente for guarda los 3 siguientes bits que representan N(R)*/
        for(x=0;x<3;x++){
          if((sb%2)==0){
            bits2[x]=0;
            sb=sb/2;
          }
          else{
            bits2[x]=1;
            sb=(sb-1)/2;
          }
        }
      }
      else{
        /*Si el tamaño de la trama es mayor a 3, entonces el control es de 2
        bytes*/
        /*Cargamos el segundo bit del control*/
        control=(int) trama[17];
        /*El siguiente if nos permite ignorar el valor de p/f debido a que no lo
        ocuparemos aun*/
        if((control%2)==0){
          sb=control/2;
        }
        else{
          sb=(control-1)/2;
        }
        /*Es siguiente for guarda los 7 siguientes bits que representan N(R)*/
        for(x=0;x<7;x++){
          if((sb%2)==0){
            bits2[x]=0;
            sb=sb/2;
          }
          else{
            bits2[x]=1;
            sb=(sb-1)/2;
          }
        }
      }
      /*El siguiente for nos permite tranformar el valor N(R) de binario a
      decimal*/
      for(x=0;x<7;x++){
        if(bits2[x]==1){
          for(y=0;y<x;y++){
            p*=2;
          }
          r+=p;
          p=1;
        }
      }
      /*Pasamos el valor N(R) a valor char[]*/
      sprintf(Nr,"%d",r);
      /*Juntamos toda la información en un unico mensaje*/
      strcat(mensaje,Nr);
    }
    else{
      /*Si los primeros 2 bits son 11 es una trama no numerada*/
      sb=(sb-1)/2;
      if((sb%2)==1){
        sb=(sb-1)/2;
        if((sb%2)==1){
          sb=(sb-1)/2;
          /*El siguiente if nos permite ignorar el valor de p/f debido a que no
          lo ocuparemos aun*/
          if((sb%2)==0){
            sb=sb/2;
          }
          else{
            sb=(sb-1)/2;
          }
          if((sb%2)==1){
            sb=(sb-1)/2;
            if((sb%2)==1){
              sb=(sb-1)/2;
              if((sb%2)==0){
                /*Si los siguientes bits son 11110 (ignorando p/f) entonces es
                una trama tipo SABME*/
                strcpy(mensaje,"SABME");
              }
            }
          }
        }
      }
      else{
        sb=sb/2;
        if((sb%2)==0){
          sb=sb/2;
          /*El siguiente if nos permite ignorar el valor de p/f debido a que no
          lo ocuparemos aun*/
          if((sb%2)==0){
            sb=sb/2;
          }
          else{
            sb=(sb-1)/2;
          }
          if((sb%2)==1){
            sb=(sb-1)/2;
            if((sb%2)==1){
              sb=(sb-1)/2;
              if((sb%2)==0){
                /*Si los siguientes bits son 00110 (ignorando p/f) entonces es
                una trama tipo UA*/
                strcpy(mensaje,"UA");
              }
            }
          }
          else{
            sb=sb/2;
            if((sb%2)==1){
              sb=(sb-1)/2;
              if((sb%2)==0){
                /*Si los siguientes bits son 00010 (ignorando p/f) entonces es
                una trama tipo DISC*/
                strcpy(mensaje,"DISC");
              }
            }
          }
        }
      }
    }
  }
  /*Guardamos nuestro mensaje en la variable resultado*/
  for(x=0;x<50;x++){
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
  char resultado[100]="";
  if(llc(trama,sizeof(trama),resultado)){
    printf("%s\n",resultado);
  }
}
