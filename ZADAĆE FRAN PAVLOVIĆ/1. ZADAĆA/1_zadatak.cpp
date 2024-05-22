#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

int broj_u_obradi;
int zavrsi = 0;

void signal_sigusr1(int s){

   printf("\n Signal SIGUSR1 primljen, trenutni broj u obradi je %d", broj_u_obradi);

}


void signal_sigterm(int s){

   printf("\n Signal SIGTERM primljen, gasim program\n");
   
   FILE *status = fopen("status.txt", "w");
   
   fprintf(status, "%d\n", broj_u_obradi);
   fclose(status);
   
   zavrsi = 1;
}


void signal_sigint(int s){

   printf("\n Signal SIGINT primljen, gasim program\n");
   
   zavrsi = 1;
}


int main(){

   printf("PID procesa je %d\n", getpid());

   FILE *status = fopen("status.txt","r");
   FILE *obrada = fopen("obrada.txt","a+");

   fscanf(status, "%d", &broj_u_obradi);
   signal(SIGTERM, signal_sigterm);
   signal(SIGINT, signal_sigint);
   signal(SIGUSR1, signal_sigusr1);


   while(zavrsi == 0){

   status = freopen("status.txt", "w+", status);

   if(broj_u_obradi == 0){

  int obradaBr;
  fseek(obrada, 0, SEEK_SET);

  while(!feof(obrada)){

  fscanf(obrada, "%d", &obradaBr);

   }

   broj_u_obradi = round(sqrt(obradaBr));

   }

   fseek(status, 0, SEEK_SET);
   fprintf(status, "%d", 0 );
   fflush(status);
   fflush(obrada);
   sleep(3);

   fseek(status, 0, SEEK_SET);
   fseek(obrada, 0, SEEK_END);
   broj_u_obradi++;
   fprintf(status, "%d", broj_u_obradi);
   fprintf(obrada, "%d\n", broj_u_obradi*broj_u_obradi);


   fflush(status);
   fflush(obrada);
   sleep(2);

   }

   fclose(status);
   fclose(obrada);



   return 0;

}
