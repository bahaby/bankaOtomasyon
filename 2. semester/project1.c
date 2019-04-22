#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

Banka baHa;

void VeriAl();
void HesapNoOlustur();

int main(){
	// int i;
	// srand(time(NULL));
	// for (i=0; i<100; i++){
	// 	HesapNoOlustur();
	// }
	VeriAl();
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

void VeriAl(){
	int t, i, mNo, hSayi, hNo;
	double b;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	char temp[200], temp2[100];
	baHa.bMusteriSayisi=0;
	baHa.tMusteriSayisi=0;
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &t);
		fscanf(pf, " Tc-No: %lf", &baHa.musteri[baHa.bMusteriSayisi].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &hSayi);
		for (i=0; i<hSayi; i++){
			fscanf(pf, " Hesap: %d", &hNo);
			fscanf(pf, " Hesap No: %d", &baHa.musteri[baHa.bMusteriSayisi].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &baHa.musteri[baHa.bMusteriSayisi].hesap[i].bakiye);
		}
		baHa.bMusteriSayisi++;
	}
	printf("%d\n", baHa.musteri[2].hesap[1].hesapNo);
}

void Guncelle(){

}