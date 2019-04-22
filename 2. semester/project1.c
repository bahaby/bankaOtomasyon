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

Banka aBank;
void Guncelle();
void VeriAl();
void HesapNoOlustur();

int main(){
	// int i;
	// srand(time(NULL));
	// for (i=0; i<100; i++){
	// 	HesapNoOlustur();
	// }
	VeriAl();
	Guncelle();
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
	aBank.mSayisi=0;
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
		}
		aBank.musteri[mNo-1].mTuru = 0;
		aBank.mSayisi++;
	}
	fclose(pf);
	pf = fopen("ticariMusteri.txt", "r");
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
		}
		aBank.musteri[mNo-1].mTuru = 1;
		aBank.mSayisi++;
	}
	fclose(pf);
	printf("%d\n", aBank.musteri[3].hesap[4].hesapNo);
}

void Guncelle(){
	int i, j, b=0, t=0;
	FILE *pf1, *pf2;
	fclose(fopen("bireyselMusteri.txt", "w"));
	fclose(fopen("ticariMusteri.txt", "w"));
	pf1 = fopen("bireyselMusteri.txt", "a");
	pf2 = fopen("ticariMusteri.txt", "a");
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].mTuru == 0){
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Musteri: %d", i+1);
			fprintf(pf1, "\n\tTc-No: %.lf", aBank.musteri[i].tcNo);
			fprintf(pf1, "\n\tHesap Sayisi: %d", aBank.musteri[i].hesapSayisi);
			for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].hesap[j].hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", aBank.musteri[i].hesap[j].bakiye);
			}
			b=1;
		}else if (aBank.musteri[i].mTuru == 1){
			if (t!=0) fprintf(pf2, "\n");
			fprintf(pf2, "Musteri: %d", i+1);
			fprintf(pf2, "\n\tTc-No: %.lf", aBank.musteri[i].tcNo);
			fprintf(pf2, "\n\tHesap Sayisi: %d", aBank.musteri[i].hesapSayisi);
			for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].hesap[j].hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", aBank.musteri[i].hesap[j].bakiye);
			}
			t=1;
		}
	}
	

}