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
	int mTuru;//0->bireysel || 1->ticari
}Musteri;

typedef struct{

	Musteri musteri[1000];
	int mSayisi;
}Banka;

Banka baHa;
void Guncelle();
void VeriAl();
void HesapNoOlustur();

int main(){
	// int i;
	// srand(time(NULL));
	// for (i=0; i<100; i++){
	// 	HesapNoOlustur();
	// }
	//Guncelle();
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
	return true;
}

void VeriAl(){
	int i, mNo;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	baHa.mSayisi=0;
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		fscanf(pf, " Tc-No: %lf", &baHa.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &baHa.musteri[mNo-1].hesapSayisi);
		for (i=0; i<baHa.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &baHa.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &baHa.musteri[mNo-1].hesap[i].bakiye);
		}
		baHa.musteri[mNo-1].mTuru = 0;
		baHa.mSayisi++;
	}
	fclose(pf);
	pf = fopen("ticariMusteri.txt", "r");
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		fscanf(pf, " Tc-No: %lf", &baHa.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &baHa.musteri[mNo-1].hesapSayisi);
		for (i=0; i<baHa.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &baHa.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &baHa.musteri[mNo-1].hesap[i].bakiye);
		}
		baHa.musteri[mNo-1].mTuru = 1;
		baHa.mSayisi++;
	}
	fclose(pf);
	printf("%d\n", baHa.musteri[3].hesap[4].hesapNo);
}

void Guncelle(){
	int i;
	FILE *pf1, *pf2;
	fclose(fopen("bireyselMusteri.txt", "w"));
	fclose(fopen("ticariMusteri.txt", "w"));
	pf1 = fopen("bireyselMusteri.txt", "a");
	pf2 = fopen("ticariMusteri.txt", "a");
	for (i=0; i<baHa.mSayisi; i++){
		if (baHa.musteri[i].mTuru == 0){
			//bireysel
		}else if (baHa.musteri[i].mTuru == 1){
			//ticari
		}
	}
	

}