#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int hesapNo;
	double bakiye;

}Hesap;

typedef struct{

	Hesap hesap[20];
	char Ad[50];
	double tcNo;
	int hesapSayisi;
	int mTuru;
}Musteri;

typedef struct{

	Musteri musteri[1000];
	int bMusteriSayisi;
	int tMusteriSayisi;
}Banka;
FILE *pf;

void HesapNoOlustur();

int main(){
	int i;
	srand(time(NULL));
	for (i=0; i<100; i++){
		HesapNoOlustur();
	}
}
void HesapNoOlustur(){
	int temp, islem=0;
	while(islem==0){
		temp = rand()%999999999;
		if (temp>100000000){
			printf("%d\n", temp);
			islem = 1;
		}
	}
}

int HesapNoKontrol(int hesapNo){
	
}