#include <stdio.h>

int main(){
  printf("ml' nob:\n");
  int num;
  char symb;
  if(scanf("%d%c", &num, &symb) != 2 || num<1 || num>5 || symb != '\n'){
    printf("luj\n");
    return 0;
  }
  printf("Qapla'\n");
  switch (num){
  case 1:
      printf("noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
      break;
  case 2:
      printf("Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
      break;
  case 3:
      printf("qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
      break;
  case 4:
      printf("Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
      break;
  case 5:
      printf("leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
      break;
  }
  return 0;
}
