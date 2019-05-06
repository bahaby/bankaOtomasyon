#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
	int Gun;
	int Ay;
	int Yil;
	int Saat;
	int Dk;
}Tarih;

typedef struct{
	int iTuru; //1->para çekme, 2->para yatırma, 3->Havale
	int iHesap;
	double iTutar;
	Tarih *pTarih;
}Islem;

typedef struct{
	int hesapNo;
	int islemSayisi;
	double bakiye;
	Islem *pIslem[500];
}Hesap;

typedef struct{
	int hesapNo;
}transferHesap;

typedef struct{

	Hesap *pHesap[100];
	transferHesap *ptHesap[100];
	double tBakiye;
	char Ad[120];
	char Sifre[8];
	double tcNo;
	int hesapSayisi;
	int tHesapSayisi;
	int mTuru;//1->bireysel || 2->ticari
}Musteri;

typedef struct{
	Musteri *pMusteri[100];
	int mSayisi;
	int girisYapan;
}Banka;

typedef struct{
	char Ad[120];
	int hesapNo;
	int iTuru;
	int mTuru;
	double Tutar;
	Tarih *pTarih;
}dIslem;

typedef struct{
	dIslem *pIslem[500];
	int islemSayisi;
}Dekont;

Banka *paBank;
Dekont *pDekont[500];

void AnaMenu();
void YeniMusteri();
void MusteriIslem(int mS);
void Guncelle();
void VeriAl();
void bankaRapor(int mS);
void hesapIslem(int mS, int hS);
void paraCek(int mS, int hS);
void paraYatir(int mS, int hS);
void havaleGonder(int mS, int hS);
void hHesapKayit(int mS, int hNo);
void hesapAc(int mS);
void hesapSil(int mS, int s);// s 1 ise normal hesap 2 ise kayitli hesap
void hesapOzeti(int mS, int hS);
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);
int hesapSec(int mS, int hS, int s);// s 1 ise normal hesap 2 ise kayitli hesap
int HesapNoOlustur();
int tcNoKontrol(double tcNo);
int hNoKontrol(int hesapNo, int n); //n 1 ise müsteri sırası 2 ise hesap sırası
char *sifrele(char sifre[120]);
void strAl(char str[], int min, int max);
double cekilenPara(int mS);
void isimDuzelt(char ad[120]);
void pTanimla();

int main(){
	pTanimla();
	paBank->girisYapan = -1;
	AnaMenu();
}

void pTanimla(){
	int i, j, k;
	paBank = malloc(sizeof(Banka));
	for (i=0; i<100; i++){
		paBank->pMusteri[i] = malloc(sizeof(Musteri));
		pDekont[i] = malloc(sizeof(Dekont)); 
		for (j=0; j<500; j++){
			pDekont[i]->pIslem[j] = malloc(sizeof(dIslem));
			pDekont[i]->pIslem[j]->pTarih = malloc(sizeof(Tarih));
		}
		for (j=0; j<100; j++){
			paBank->pMusteri[i]->pHesap[j] = malloc(sizeof(Hesap));
			paBank->pMusteri[i]->ptHesap[j] = malloc(sizeof(transferHesap));
			for (k=0; k<500; k++){
				paBank->pMusteri[i]->pHesap[j]->pIslem[k] = malloc(sizeof(Islem));
				paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih = malloc(sizeof(Tarih));
			}
		}
	}
}

void VeriAl(){
	int i, j, mNo;
	char temp[120], c;
	paBank->mSayisi=0;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	if (pf!=NULL){
		while(!feof(pf)){
			fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
			strcpy(paBank->pMusteri[mNo-1]->Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &paBank->pMusteri[mNo-1]->tcNo);
			fscanf(pf, " Ad Soyad: %s", paBank->pMusteri[mNo-1]->Ad);
			fscanf(pf, " Hesap Sayisi: %d", &paBank->pMusteri[mNo-1]->hesapSayisi);
			for (i=0; i<paBank->pMusteri[mNo-1]->hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &paBank->pMusteri[mNo-1]->pHesap[i]->hesapNo);
				fscanf(pf, " Bakiye: %lf", &paBank->pMusteri[mNo-1]->pHesap[i]->bakiye);
				fscanf(pf, " Islem Sayisi: %d", &paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi);
				if (paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi!=0){
					for (j=0; j<paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Gun,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Ay,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Yil,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Saat,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Dk,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iTutar);
						fscanf(pf, " %d - %d", 
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iTuru,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &paBank->pMusteri[mNo-1]->tHesapSayisi);
			if (paBank->pMusteri[mNo-1]->tHesapSayisi != 0){
				for (i=0; i<paBank->pMusteri[mNo-1]->tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &paBank->pMusteri[mNo-1]->ptHesap[i]->hesapNo);
				}
			}
			paBank->pMusteri[mNo-1]->mTuru = 1;
			paBank->mSayisi++;
		}
		fclose(pf);
	}
	pf = fopen("ticariMusteri.txt", "r");
	if (pf!=NULL){
		while(!feof(pf)){
			fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
			strcpy(paBank->pMusteri[mNo-1]->Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &paBank->pMusteri[mNo-1]->tcNo);
			fscanf(pf, " Ad Soyad: %s", paBank->pMusteri[mNo-1]->Ad);
			fscanf(pf, " Hesap Sayisi: %d", &paBank->pMusteri[mNo-1]->hesapSayisi);
			for (i=0; i<paBank->pMusteri[mNo-1]->hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &paBank->pMusteri[mNo-1]->pHesap[i]->hesapNo);
				fscanf(pf, " Bakiye: %lf", &paBank->pMusteri[mNo-1]->pHesap[i]->bakiye);
				fscanf(pf, " Islem Sayisi: %d", &paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi);
				if (paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi!=0){
					for (j=0; j<paBank->pMusteri[mNo-1]->pHesap[i]->islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Gun,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Ay,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Yil,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Saat,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->pTarih->Dk,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iTutar);
						fscanf(pf, " %d - %d", 
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iTuru,
							&paBank->pMusteri[mNo-1]->pHesap[i]->pIslem[j]->iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &paBank->pMusteri[mNo-1]->tHesapSayisi);
			if (paBank->pMusteri[mNo-1]->tHesapSayisi != 0){
				for (i=0; i<paBank->pMusteri[mNo-1]->tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &paBank->pMusteri[mNo-1]->ptHesap[i]->hesapNo);
				}
			}
			paBank->pMusteri[mNo-1]->mTuru = 2;
			paBank->mSayisi++;
		}
		fclose(pf);
	}

	for (i=0; i<paBank->mSayisi; i++){
		paBank->pMusteri[i]->tBakiye = 0;
		for (j=0; j<paBank->pMusteri[i]->hesapSayisi; j++){
			paBank->pMusteri[i]->tBakiye += paBank->pMusteri[i]->pHesap[j]->bakiye;
		}
	}
}

void Guncelle(){
	int i, j, k, iTuru, b=0, t=0;
	double gelen=0, giden=0, kar=0, kesinti;
	FILE *pf1, *pf2;
	for (i=0; i<paBank->mSayisi; i++){
		if (paBank->pMusteri[i]->mTuru == 1){
			if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));
			pf1 = fopen("bireyselMusteri.txt", "a");
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Musteri: %d / [ %s ]", i+1, paBank->pMusteri[i]->Sifre);
			fprintf(pf1, "\n\tTc-No: %.lf", paBank->pMusteri[i]->tcNo);
			fprintf(pf1, "\n\tAd Soyad: %s", paBank->pMusteri[i]->Ad);
			fprintf(pf1, "\n\tHesap Sayisi: %d", paBank->pMusteri[i]->hesapSayisi);
			for (j=0; j<paBank->pMusteri[i]->hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, paBank->pMusteri[i]->pHesap[j]->hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", paBank->pMusteri[i]->pHesap[j]->bakiye);
				fprintf(pf1, "\n\t\tIslem Sayisi: %d", paBank->pMusteri[i]->pHesap[j]->islemSayisi);
				for (k=0; k<paBank->pMusteri[i]->pHesap[j]->islemSayisi; k++){
					fprintf(pf1, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Gun,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Ay,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Yil,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Saat,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Dk,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar);
					fprintf(pf1, "\n\t\t\t\t%d - %d", 
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTuru,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iHesap);
				}
			}
			fprintf(pf1, "\n\tKayitli Havale Hesap Sayisi: %d", paBank->pMusteri[i]->tHesapSayisi);
			for (j=0; j<paBank->pMusteri[i]->tHesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, paBank->pMusteri[i]->ptHesap[j]->hesapNo);
			}
			fclose(pf1);
			b=1;
		}else if (paBank->pMusteri[i]->mTuru == 2){
			if (t==0) fclose(fopen("ticariMusteri.txt", "w"));
			pf2 = fopen("ticariMusteri.txt", "a");
			if (t!=0) fprintf(pf2, "\n");
			fprintf(pf2, "Musteri: %d / [ %s ]", i+1, paBank->pMusteri[i]->Sifre);
			fprintf(pf2, "\n\tTc-No: %.lf", paBank->pMusteri[i]->tcNo);
			fprintf(pf2, "\n\tAd Soyad: %s", paBank->pMusteri[i]->Ad);
			fprintf(pf2, "\n\tHesap Sayisi: %d", paBank->pMusteri[i]->hesapSayisi);
			for (j=0; j<paBank->pMusteri[i]->hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, paBank->pMusteri[i]->pHesap[j]->hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", paBank->pMusteri[i]->pHesap[j]->bakiye);
				fprintf(pf2, "\n\t\tIslem Sayisi: %d", paBank->pMusteri[i]->pHesap[j]->islemSayisi);
				for (k=0; k<paBank->pMusteri[i]->pHesap[j]->islemSayisi; k++){
					fprintf(pf2, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Gun,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Ay,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Yil,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Saat,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->pTarih->Dk,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar);
					fprintf(pf2, "\n\t\t\t\t%d - %d", 
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTuru,
						paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iHesap);
				}
			}
			fprintf(pf2, "\n\tKayitli Havale Hesap Sayisi: %d", paBank->pMusteri[i]->tHesapSayisi);
			for (j=0; j<paBank->pMusteri[i]->tHesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, paBank->pMusteri[i]->ptHesap[j]->hesapNo);
			}
			fclose(pf2);
			t=1;
		}
	}

	for (i=0; i<paBank->mSayisi; i++){
		paBank->pMusteri[i]->tBakiye = 0;
		for (j=0; j<paBank->pMusteri[i]->hesapSayisi; j++){
			paBank->pMusteri[i]->tBakiye += paBank->pMusteri[i]->pHesap[j]->bakiye;
		}
	}
	
	for (i=0; i<paBank->mSayisi; i++){
		for (j=0; j<paBank->pMusteri[i]->hesapSayisi; j++){
			for (k=0; k<paBank->pMusteri[i]->pHesap[j]->islemSayisi; k++){
				iTuru = paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTuru;
				if (iTuru == 1) giden += paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar;
				else if (iTuru == 2) gelen += paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar;
				else if (iTuru == 3 && paBank->pMusteri[i]->mTuru == 1){
					if (paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar < 0){
						kesinti = ((int)(-paBank->pMusteri[i]->pHesap[j]->pIslem[k]->iTutar*2)) / 100.0;
						kar += kesinti;
					}
				}
			}
		}
	}
	fclose(fopen("rapor.txt", "w"));
	pf1 = fopen("rapor.txt", "a");
	fprintf(pf1, "aBank gelir-gider raporu...\n\n");
	fprintf(pf1, "Bankada bulunan toplam para: %.2lf\n", gelen + giden + kar);
	fprintf(pf1, "Gelen toplam para:           %.2lf\n", gelen);
	fprintf(pf1, "Giden toplam para:           %.2lf\n", (giden == 0)?giden:-giden);
	fprintf(pf1, "Bankanin kari:               %.2lf", kar);
	fclose(pf1);
}

void AnaMenu(){
	VeriAl();
	int sorgu, kontrol;
	char temp[120], c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (paBank->girisYapan == -1){
		printf("1-)\tYeni Musteri Kaydi\n");
		printf("2-)\tMusteri Girisi\n");
	}else{
		printf("1-)\tMusteri Islemleri\n");
		printf("2-)\tMusteri Cikisi\n");
	}
	printf("0-)\tCikis\n");
	printf("Secim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}else if(sorgu == 2 && paBank->mSayisi==0){
			printf("Herhangi bir musteri yok henuz.\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	system("@cls||clear");
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			if (paBank->girisYapan == -1){
				YeniMusteri();
			}else{
				MusteriIslem(paBank->girisYapan);
			}
		}break;
		case 2:{
			if (paBank->girisYapan == -1){
				MusteriIslem(paBank->girisYapan);
			}else{
				paBank->girisYapan = -1;
				AnaMenu();
			}
		}break;
	}
}

void YeniMusteri(){
	int sorgu, kontrol, is1, is2;
	double dTemp;
	char temp[120], s1[120]={}, s2[120]={}, c;
	printf(".............aBank.............\n");
	printf("1-)\tBireysel Musteri\n2-)\tTicari Musteri\n3-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>3 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>3 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:
		case 2:{
			paBank->pMusteri[paBank->mSayisi]->mTuru = sorgu;
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Ad Soyad: ");
	strAl(paBank->pMusteri[paBank->mSayisi]->Ad, 5, 120);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc Numaranizi Giriniz: ");
	do{
		strAl(temp, 11, 11);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (kontrol!=1 || temp[0]=='0') printf("Hatali Giris!\nTekrar Deneyiniz: ");
		else if (tcNoKontrol(dTemp) != -1) printf("Bu Tc Numarasi Kayitli!\nTekrar Deneyiniz: ");
	}while(!(tcNoKontrol(dTemp) == -1 && kontrol == 1 && temp[0] != '0'));
	paBank->pMusteri[paBank->mSayisi]->tcNo = dTemp;

	system("@cls||clear");
	printf(".............aBank.............\n");
	do{
		printf("Sifrenizi Giriniz (4 ile 8 karakter arasi): ");
		strAl(s1, 1, 30);
		sscanf(s1, "%d%c", &is1, &c);
		printf("Tekrar Giriniz: ");
		strAl(s2, 1, 30);
		kontrol = sscanf(s2, "%d%c", &is2, &c);
		if (!(strcmp(s1, s2) == 0 && is1>=1000 && is1<=99999999 && kontrol == 1)){
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hatali Giris!\n");
		}
	}while(!(strcmp(s1, s2) == 0 && is1>=1000 && is1<=99999999 && kontrol == 1));
	strcpy(paBank->pMusteri[paBank->mSayisi]->Sifre, sifrele(s1));

	paBank->pMusteri[paBank->mSayisi]->hesapSayisi = 1;
	paBank->pMusteri[paBank->mSayisi]->tHesapSayisi = 0;
	paBank->pMusteri[paBank->mSayisi]->pHesap[0]->hesapNo = HesapNoOlustur();
	paBank->pMusteri[paBank->mSayisi]->pHesap[0]->bakiye = 0;
	paBank->pMusteri[paBank->mSayisi]->pHesap[0]->islemSayisi = 0;
	paBank->girisYapan = paBank->mSayisi;
	paBank->mSayisi++;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\nHesabiniz kuruldu...\n\n");
	strcpy(temp, paBank->pMusteri[paBank->mSayisi-1]->Ad);
	isimDuzelt(temp);
	printf("Adiniz: %s\n", temp);
	printf("Tc Numaraniz: %.lf\n", paBank->pMusteri[paBank->mSayisi-1]->tcNo);
	printf("Hesap Numaraniz: %d\n", paBank->pMusteri[paBank->mSayisi-1]->pHesap[0]->hesapNo);
	printf("Sifreniz: %d\n\n", is1);
	printf("1-)\tMusteri Islemleri\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			MusteriIslem(paBank->girisYapan);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}

void MusteriIslem(int mS){
	int sonuc, sorgu, kontrol, t=0;
	char temp[120], c;
	double dTemp;
	if (mS == -1){
		char sifre[120];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc Numaranizi Giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 11, 11);
			kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
			if (dTemp == 0 && kontrol == 1) AnaMenu();
			sonuc = tcNoKontrol(dTemp);
			if (!(sonuc != -1 && kontrol == 1 && temp[0] != '0')) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}while(!(sonuc != -1 && kontrol == 1 && temp[0] != '0'));
		mS=sonuc;
		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (t!=0) printf("Hatali sifre!\n");
			printf("Sifrenizi Giriniz: ");
			strAl(sifre, 4, 8);
			t = strcmp(paBank->pMusteri[mS]->Sifre, sifrele(sifre));
		}while(t!=0);
		paBank->girisYapan = mS;
		AnaMenu();
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	strcpy(temp, paBank->pMusteri[mS]->Ad);
	isimDuzelt(temp);
	printf("Hosgeldin, %s\n", temp);
	printf("1-)\tHesap Sec\n2-)\tHesap Ac\n3-)\tHesap Sil\n4-)\tHavale Hesabi Kaydet\n5-)\tHavale Hesabi Sil\n");
	printf("6-)\tRapor Al\n7-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>7 || kontrol != 1) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if (sorgu == 3 && paBank->pMusteri[mS]->hesapSayisi == 1){
			printf("Son Hesabinizi silemezsiniz!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 5 && paBank->pMusteri[mS]->tHesapSayisi == 0){
			printf("Kayitli hesabiniz kalmamistir!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>7 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			hesapIslem(mS, -1);
		}break;
		case 2:{
			hesapAc(mS);
		}break;
		case 3:{
			hesapSil(mS, 1);
		}break;
		case 4:{
			hHesapKayit(mS, -1);
		}break;
		case 5:{
			hesapSil(mS, 2);
		}break;
		case 6:{
			bankaRapor(mS);
		}break;
		case 7:{
			AnaMenu();
		}break;
	}
}

void hesapIslem(int mS, int hS){
	char temp[120], c;
	int sorgu, kontrol;
	double limit;
	if (hS == -1) hS = hesapSec(mS, -1, 1);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabinizdaki bakiyeniz: %.2lf TL'dir.\n", paBank->pMusteri[mS]->pHesap[hS]->bakiye);
	printf("Toplam bakiyeniz: %.2lf TL'dir.\n\n", paBank->pMusteri[mS]->tBakiye);
	printf("1-)\tPara Cekme\n2-)\tPara Yatirma\n3-)\tHavale\n4-)\tHesap Ozeti\n5-)\tGeri Don\n6-)\tAna Menu\n0-)\tCikis\nSecim: ");
	limit = ((paBank->pMusteri[mS]->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>6 ||kontrol != 1) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if ((sorgu == 1 || sorgu == 3) && paBank->pMusteri[mS]->tBakiye == 0){
			printf("Bu islem icin hesaplarinizda yeterli para yok!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 1 && limit == 0){
			printf("Gunluk para cekme limitinize ulastiniz!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 4 && paBank->pMusteri[mS]->pHesap[hS]->islemSayisi==0){
			printf("Hesabinizda herhangi bir islem yapılmadi!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>6 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			paraCek(mS, hS);
		}break;
		case 2:{
			paraYatir(mS, hS);
		}break;
		case 3:{
			havaleGonder(mS, hS);
		}break;
		case 4:{
			hesapOzeti(mS, hS);
		}break;
		case 5:{
			MusteriIslem(mS);
		}break;
		case 6:{
			AnaMenu();
		}break;
	}
}
void paraCek(int mS, int hS){
	int i, kontrol, sorgu;
	char temp[120], c;
	double dTemp, limit;
	limit = ((paBank->pMusteri[mS]->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Cekmek istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 4);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>paBank->pMusteri[mS]->tBakiye) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", paBank->pMusteri[mS]->tBakiye);
		else if (dTemp>limit) printf("Para cekme limitiniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", limit);
		else if (!(dTemp>0 && kontrol == 1 && temp[0] != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=paBank->pMusteri[mS]->tBakiye && dTemp<=limit && kontrol==1 && temp[0] != '0'));
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=paBank->pMusteri[mS]->pHesap[hS]->bakiye && paBank->pMusteri[mS]->pHesap[hS]->bakiye != 0){
		paBank->pMusteri[mS]->pHesap[hS]->bakiye -= dTemp;
		islemKaydi(mS, hS, 1, paBank->pMusteri[mS]->pHesap[hS]->hesapNo, -dTemp);
		printf("%d Nolu hesabinizdan %.2lf TL kesildi\n\n", paBank->pMusteri[mS]->pHesap[hS]->hesapNo, dTemp);
		dTemp = 0;
	}else{
		if (dTemp>paBank->pMusteri[mS]->pHesap[hS]->bakiye && paBank->pMusteri[mS]->pHesap[hS]->bakiye != 0){
			dTemp -= paBank->pMusteri[mS]->pHesap[hS]->bakiye;
			islemKaydi(mS, hS, 1, paBank->pMusteri[mS]->pHesap[hS]->hesapNo, -paBank->pMusteri[mS]->pHesap[hS]->bakiye);
			printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[hS]->hesapNo, paBank->pMusteri[mS]->pHesap[hS]->bakiye);
			paBank->pMusteri[mS]->pHesap[hS]->bakiye = 0;
		}
		for (i=0; i<paBank->pMusteri[mS]->hesapSayisi; i++){
			if (paBank->pMusteri[mS]->pHesap[i]->bakiye != 0){
				if (dTemp<=paBank->pMusteri[mS]->pHesap[i]->bakiye && dTemp!=0){
					paBank->pMusteri[mS]->pHesap[i]->bakiye -= dTemp;
					islemKaydi(mS, i, 1, paBank->pMusteri[mS]->pHesap[i]->hesapNo, -dTemp);
					printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[i]->hesapNo, dTemp);
					dTemp = 0;
				}else if (dTemp>paBank->pMusteri[mS]->pHesap[i]->bakiye && dTemp!=0){
					dTemp -= paBank->pMusteri[mS]->pHesap[i]->bakiye;
					islemKaydi(mS, i, 1, paBank->pMusteri[mS]->pHesap[i]->hesapNo, -paBank->pMusteri[mS]->pHesap[i]->bakiye);
					printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[i]->hesapNo, paBank->pMusteri[mS]->pHesap[i]->bakiye);
					paBank->pMusteri[mS]->pHesap[i]->bakiye = 0;
				}
				if (dTemp==0) printf("\n");
			}
		}
	}
	Guncelle();
	printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void paraYatir(int mS, int hS){
	int kontrol, sorgu;
	char temp[120], c;
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Yatirmak istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 6);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		else if (!(dTemp>0 && kontrol == 1 && temp[0] != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && kontrol == 1 && temp[0] != '0'));
	dTemp = ((int)(dTemp*100)) / 100.0;

	paBank->pMusteri[mS]->pHesap[hS]->bakiye += dTemp;
	islemKaydi(mS, hS, 2, paBank->pMusteri[mS]->pHesap[hS]->hesapNo, dTemp);
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void havaleGonder(int mS, int hS){
	double dTemp, kesinti;
	int i, tHesapS, hhS, hmS, tHesapNo, sorgu, kontrol;
	char temp[120], c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (paBank->pMusteri[mS]->tHesapSayisi != 0){
		printf("1-)\tKayitli Havale Hesabina Gonder\n2-)\tHesap No Girerek Gonder\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<1 || sorgu>2 || kontrol != 1);
		switch (sorgu){
			case 1:{
				tHesapS = hesapSec(mS, hS, 2);
				tHesapNo = paBank->pMusteri[mS]->ptHesap[tHesapS]->hesapNo;
				if (tHesapNo == paBank->pMusteri[mS]->pHesap[hS]->hesapNo){
					system("@cls||clear");
					printf(".............aBank.............\n");
					printf("Islem yaptiginiz hesabi secemezsiniz...\n\n");
					printf("1-)\tGeri Don\nSecim: ");
					do{
						strAl(temp, 1, 1);
						kontrol = sscanf(temp, "%d%c", &sorgu, &c);
						if(sorgu<0 || sorgu>1 || kontrol != 1) {
							printf("Hatali Giris!\nTekrar Deneyiniz: ");
						}
					}while(sorgu<0 || sorgu>1 || kontrol != 1);
					switch (sorgu){
						case 1:{
							havaleGonder(mS, hS);
						}break;
					}
				}
			}break;
			case 2:{
				system("@cls||clear");
				printf(".............aBank.............\n");
				printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
				do{
					strAl(temp, 9, 9);
					kontrol = sscanf(temp, "%d%c", &sorgu, &c);
					if (sorgu == 0 && kontrol == 1) hesapIslem(mS, hS);
					if (kontrol!=1) printf("Hatali Giris!\nTekrar Deneyiniz: ");
					else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
				}while(!(hNoKontrol(sorgu, 1) != -1 && kontrol==1));
				tHesapNo = sorgu;
			}break;
		}
	}else{
		printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 9, 9);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1) hesapIslem(mS, hS);
			if (kontrol!=1) printf("Hatali Giris!\nTekrar Deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
		}while(!(hNoKontrol(sorgu, 1) != -1 && kontrol==1));
		tHesapNo = sorgu;
	}

	hhS = hNoKontrol(tHesapNo, 2);
	hmS = hNoKontrol(tHesapNo, 1);

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Gondermek istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 9);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>paBank->pMusteri[mS]->tBakiye && kontrol == 1) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", paBank->pMusteri[mS]->tBakiye);
		else if (!(dTemp>0 && kontrol == 1 && temp[0] != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=paBank->pMusteri[mS]->tBakiye && kontrol==1 && temp[0] != '0'));
	dTemp = ((int)(dTemp*100)) / 100.0;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	if (dTemp<=paBank->pMusteri[mS]->pHesap[hS]->bakiye && paBank->pMusteri[mS]->pHesap[hS]->bakiye != 0){
		paBank->pMusteri[mS]->pHesap[hS]->bakiye -= dTemp;
		islemKaydi(mS, hS, 3, paBank->pMusteri[hmS]->pHesap[hhS]->hesapNo, -dTemp);
		kesinti = (paBank->pMusteri[mS]->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
		printf("%d Nolu hesabinizdan %.2lf TL kesildi\n\n", paBank->pMusteri[mS]->pHesap[hS]->hesapNo, dTemp);

		paBank->pMusteri[hmS]->pHesap[hhS]->bakiye += (dTemp - kesinti);
		islemKaydi(hmS, hhS, 3, paBank->pMusteri[mS]->pHesap[hS]->hesapNo, dTemp - kesinti);
		dTemp = 0;
	}else{
		if (dTemp>paBank->pMusteri[mS]->pHesap[hS]->bakiye && paBank->pMusteri[mS]->pHesap[hS]->bakiye != 0){
			dTemp -= paBank->pMusteri[mS]->pHesap[hS]->bakiye;
			islemKaydi(mS, hS, 3, paBank->pMusteri[hmS]->pHesap[hhS]->hesapNo, -paBank->pMusteri[mS]->pHesap[hS]->bakiye);
			kesinti = (paBank->pMusteri[mS]->mTuru == 1) ? ((int)(paBank->pMusteri[mS]->pHesap[hS]->bakiye*2)) / 100.0 : 0;
			printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[hS]->hesapNo, paBank->pMusteri[mS]->pHesap[hS]->bakiye);
			
			paBank->pMusteri[hmS]->pHesap[hhS]->bakiye += (paBank->pMusteri[mS]->pHesap[hS]->bakiye - kesinti);
			islemKaydi(hmS, hhS, 3, paBank->pMusteri[mS]->pHesap[hS]->hesapNo, paBank->pMusteri[mS]->pHesap[hS]->bakiye - kesinti);
			paBank->pMusteri[mS]->pHesap[hS]->bakiye = 0;
		}
		for (i=0; i<paBank->pMusteri[mS]->hesapSayisi; i++){
			if (paBank->pMusteri[mS]->pHesap[i]->bakiye != 0){
				if (dTemp<=paBank->pMusteri[mS]->pHesap[i]->bakiye && dTemp!=0){
					paBank->pMusteri[mS]->pHesap[i]->bakiye -= dTemp;
					islemKaydi(mS, i, 3, paBank->pMusteri[hmS]->pHesap[hhS]->hesapNo, -dTemp);
					kesinti = (paBank->pMusteri[mS]->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
					printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[i]->hesapNo, dTemp);

					paBank->pMusteri[hmS]->pHesap[hhS]->bakiye += (dTemp - kesinti);
					islemKaydi(hmS, hhS, 3, paBank->pMusteri[mS]->pHesap[i]->hesapNo, dTemp - kesinti);
					dTemp = 0;

				}else if (dTemp>paBank->pMusteri[mS]->pHesap[i]->bakiye && dTemp!=0){
					dTemp -= paBank->pMusteri[mS]->pHesap[i]->bakiye;
					islemKaydi(mS, i, 3, paBank->pMusteri[hmS]->pHesap[hhS]->hesapNo, -paBank->pMusteri[mS]->pHesap[i]->bakiye);
					kesinti = (paBank->pMusteri[mS]->mTuru == 1) ? ((int)(paBank->pMusteri[mS]->pHesap[i]->bakiye*2)) / 100.0 : 0;
					printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", paBank->pMusteri[mS]->pHesap[i]->hesapNo, paBank->pMusteri[mS]->pHesap[i]->bakiye);
					
					paBank->pMusteri[hmS]->pHesap[hhS]->bakiye += (paBank->pMusteri[mS]->pHesap[i]->bakiye - kesinti);
					islemKaydi(hmS, hhS, 3, paBank->pMusteri[mS]->pHesap[i]->hesapNo, paBank->pMusteri[mS]->pHesap[i]->bakiye - kesinti);
					paBank->pMusteri[mS]->pHesap[i]->bakiye = 0;
				}
				if (dTemp==0) printf("\n");
			}
		}
	}
	Guncelle();
	printf("1-)\tGeri Don\n2-)\tHavale Hesabi Olarak Kaydet\n3-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>3 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>3 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			hesapIslem(mS, hS);
		}break;
		case 2:{
			hHesapKayit(mS, tHesapNo);
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
}
void hHesapKayit(int mS, int hNo){
	int sorgu, kontrol;
	char temp[120], c;
	if (hNo == -1){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 9, 9);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1) MusteriIslem(mS);
			if (kontrol!=1) printf("Hatali Giris!\nTekrar Deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
		}while(!(hNoKontrol(sorgu, 1) != -1 && kontrol==1));
		hNo = sorgu;
	}
	paBank->pMusteri[mS]->ptHesap[paBank->pMusteri[mS]->tHesapSayisi]->hesapNo = hNo;
	paBank->pMusteri[mS]->tHesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tMusteri Islemleri\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			MusteriIslem(mS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}

}
void hesapAc(int mS){
	int sorgu, kontrol, hS = paBank->pMusteri[mS]->hesapSayisi;
	char temp[120], c;
	paBank->pMusteri[mS]->pHesap[hS]->hesapNo = HesapNoOlustur();
	paBank->pMusteri[mS]->pHesap[hS]->bakiye = 0;
	paBank->pMusteri[mS]->pHesap[hS]->islemSayisi = 0;
	paBank->pMusteri[mS]->hesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabiniz Acildi..\n1-)\tGeri Don\n2-)\tHesap Islemleri\n3-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>3 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>3 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			MusteriIslem(mS);
		}break;
		case 2:{
			hesapIslem(mS, paBank->pMusteri[mS]->hesapSayisi-1);
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
}
void hesapSil(int mS, int s){//s 1 ise normal hesap 2 ise transfer hesap
	int i, n, sorgu, kontrol, shS;
	char temp[120], c;
	shS =  hesapSec(mS, -1, s);
	n = (s==1) ? paBank->pMusteri[mS]->hesapSayisi : paBank->pMusteri[mS]->tHesapSayisi;
	if (s==1 && paBank->pMusteri[mS]->pHesap[shS]->bakiye!=0){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesabinizda para varken hesabinizi silemezsiniz!\n1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>2 || kontrol != 1) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<0 || sorgu>2 || kontrol != 1);
		switch (sorgu){
			case 0:{
				printf("Cikis Basarili\n\n");
				exit(0);
			}break;
			case 1:{
				hesapSil(mS, 1);
			}break;
			case 2:{
				AnaMenu();
			}break;
		}
	}else{
		for (i=shS; i<n; i++){
			if (s==1){
				paBank->pMusteri[mS]->pHesap[i] = paBank->pMusteri[mS]->pHesap[i+1]; 
			}else{
				paBank->pMusteri[mS]->ptHesap[i] = paBank->pMusteri[mS]->ptHesap[i+1]; 
			}
		}
		(s==1) ? paBank->pMusteri[mS]->hesapSayisi-- : paBank->pMusteri[mS]->tHesapSayisi--;
		Guncelle();
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
		printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>2 || kontrol != 1) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<0 || sorgu>2 || kontrol != 1);
		switch (sorgu){
			case 0:{
				printf("Cikis Basarili\n\n");
				exit(0);
			}break;
			case 1:{
				MusteriIslem(mS);
			}break;
			case 2:{
				AnaMenu();
			}break;
		}
	}
}
int hesapSec(int mS, int hS, int s){ //s 1 ise normal hesap 2 ise transfer hesap
	int i, sorgu, kontrol, hNo, n, tmS;
	char temp[120], c;
	if (s==1){
		n = paBank->pMusteri[mS]->hesapSayisi;
		if (n==1) return 0;
	}else n = paBank->pMusteri[mS]->tHesapSayisi;
	if(n==0) return -1;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem Yapilacak Hesap No Seciniz (Iptal etmek icin 0 giriniz): \n\n");
	for (i=0; i<n; i++){
		hNo = (s==1) ? paBank->pMusteri[mS]->pHesap[i]->hesapNo : paBank->pMusteri[mS]->ptHesap[i]->hesapNo;
		if (s==1){
			printf("%d-) %d (%.2lf TL)\n", i+1, hNo, paBank->pMusteri[mS]->pHesap[i]->bakiye);
		}else{
			tmS = hNoKontrol(hNo, 1);
			strcpy(temp, paBank->pMusteri[tmS]->Ad);
			isimDuzelt(temp);
			printf("%d-) %d (%s)\n", i+1, hNo, temp);
		}
	}
	printf("\nSecim: ");
	do{
		strAl(temp, 1, 2);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1){
			if (hS == -1) MusteriIslem(mS);
			else hesapIslem(mS, hS);
		}
		if (sorgu<1 || sorgu>n || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>n || kontrol != 1);
	return sorgu-1;
}

int HesapNoOlustur(){
	int hesapNo;
	srand(time(NULL));
	do{
		hesapNo = rand()%899999999 + 100000000;	
	}while(hNoKontrol(hesapNo, 1)!=-1);
	return hesapNo;
}
char *sifrele(char sifre[8]){
	int i, temp[8]={}, len = strlen(sifre);
	for (i=0; i<8; i++){
		*(temp+i) += ((i+1) * (*(sifre+(i%len)) + 11) + *(sifre+len-(i%len)-1) + i);
		*(sifre+i) = *(temp+i)%93 + 33;
	}
	return sifre;
}

int tcNoKontrol(double tcNo){
	int i;
	for (i=0; i<paBank->mSayisi; i++){
		if (paBank->pMusteri[i]->tcNo == tcNo) return i;
	}
	return -1;
}

int hNoKontrol(int hesapNo, int n){ // n 1 ise müsteri no, 2 ise hesap no
	int mS, hS;
	for (mS=0; mS<paBank->mSayisi; mS++){
		for (hS=0; hS<paBank->pMusteri[mS]->hesapSayisi; hS++){
			if (paBank->pMusteri[mS]->pHesap[hS]->hesapNo == hesapNo) return (n==1)?mS:hS;
		}
	}
	return -1;
}

void strAl(char str[], int min, int max){
	int t;
	char cTemp;
	do{
		scanf("%s", str);
		t = strlen(str);
		do{
			scanf("%c", &cTemp);
			if (cTemp == ' ') cTemp = '-';
			*(str + t++) = cTemp;
		}while (cTemp != '\n');
		*(str+t-1) = 0;
		t = strlen(str);
		if (t==1 && str[0] == '0') break;
		if (t>max || t<min) printf("Karakter sayisi gecersiz!\nTekrar deneyiniz: ");
	}while (!(t<=max && t>=min));
}
void isimDuzelt(char ad[120]){
	int i, t;
	t = strlen(ad);
	for (i=0; i<t; i++){
		if (ad[i] == '-') ad[i] = ' ';
	}
}
void hesapOzeti(int mS, int hS){
	FILE *pf;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i, j, k, n, t1, t2, t3, iS, aralik, ihNo, imS, iA, iY, sorgu, kontrol;
	double kesinti;
	char c, temp[120];
	iS = paBank->pMusteri[mS]->pHesap[hS]->islemSayisi;
	t1 = (paBank->pMusteri[mS]->pHesap[hS]->pIslem[0]->pTarih->Yil - 1900) * 12 + paBank->pMusteri[mS]->pHesap[hS]->pIslem[0]->pTarih->Ay;
	t2 = tm.tm_year * 12 + tm.tm_mon+1 ;
	aralik = t2-t1;
	int index[aralik+1];
	
	system("@cls||clear");
	printf(".............aBank.............\n");
	for (i=0; i<aralik+1; i++){
		*(index+i) = -1;
		pDekont[i]->islemSayisi = 0;
		pDekont[i]->pIslem[0]->Tutar = 0;
		for (j=iS-1; j>=0; j--){
			t3 = (paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->pTarih->Yil - 1900) * 12 + paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->pTarih->Ay;
			if ((t2-t3) == i){
				ihNo = paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->iHesap;
				imS = hNoKontrol(ihNo, 1);
				strcpy(pDekont[i]->pIslem[pDekont[i]->islemSayisi]->Ad, paBank->pMusteri[imS]->Ad);
				isimDuzelt(pDekont[i]->pIslem[pDekont[i]->islemSayisi]->Ad);
				pDekont[i]->pIslem[pDekont[i]->islemSayisi]->hesapNo = ihNo;
				pDekont[i]->pIslem[pDekont[i]->islemSayisi]->mTuru = paBank->pMusteri[mS]->mTuru;
				pDekont[i]->pIslem[pDekont[i]->islemSayisi]->iTuru = paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->iTuru;
				pDekont[i]->pIslem[pDekont[i]->islemSayisi]->Tutar = paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->iTutar;
				pDekont[i]->pIslem[pDekont[i]->islemSayisi]->pTarih = paBank->pMusteri[mS]->pHesap[hS]->pIslem[j]->pTarih;
				pDekont[i]->islemSayisi++;
			}
		}
	}
	k=0;
	for (i=0; i<aralik+1; i++){
		if (pDekont[i]->pIslem[0]->Tutar != 0){
			iY = (t2 - i) / 12 + 1900;
			iA = (t2 - i -1) % 12 +1; 
			n = i-k+1;
			if (i==0) printf("%2d-)\t%02d - %02d\t", n, iY, iA);
			else printf("%2d-)\t%d - %02d/%02d\t", n, iY, iA, iA%12+1);
			if (n%2 == 0) printf("\n");
			*(index+n) = i;
		}else{
			k++;
		}
	}
	printf("\n\nSecim (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 2);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1) hesapIslem(mS, hS);
		if(sorgu<0 || sorgu>n || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>n || kontrol != 1);
	
	fclose(fopen("dekont.txt", "w"));
	pf = fopen("dekont.txt", "a");
	i = *(index + sorgu);
	iY = (t2 - i) / 12 + 1900;
	iA = (t2 - i -1) % 12 +1; 
	if (i==0) fprintf(pf, "(%02d - %02d)  Donemi Hesap Ozeti", iY, iA);
	else fprintf(pf, "(%d - %02d/%02d)  Donemi Hesap Ozeti", iY, iA, iA%12+1);
	fprintf(pf, "  (%d)\n\n", paBank->pMusteri[mS]->pHesap[hS]->hesapNo);
	for (j=pDekont[i]->islemSayisi-1; j>=0; j--){
		if (pDekont[i]->pIslem[j]->iTuru == 1){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para Cekme    %.2lf\n", 
				pDekont[i]->pIslem[j]->pTarih->Gun,
				pDekont[i]->pIslem[j]->pTarih->Ay,
				pDekont[i]->pIslem[j]->pTarih->Yil,
				pDekont[i]->pIslem[j]->pTarih->Saat,
				pDekont[i]->pIslem[j]->pTarih->Dk,
				-pDekont[i]->pIslem[j]->Tutar);
		}else if (pDekont[i]->pIslem[j]->iTuru == 2){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para Yatirma  %.2lf\n", 
				pDekont[i]->pIslem[j]->pTarih->Gun,
				pDekont[i]->pIslem[j]->pTarih->Ay,
				pDekont[i]->pIslem[j]->pTarih->Yil,
				pDekont[i]->pIslem[j]->pTarih->Saat,
				pDekont[i]->pIslem[j]->pTarih->Dk,
				pDekont[i]->pIslem[j]->Tutar);
		}else if (pDekont[i]->pIslem[j]->iTuru == 3){
			if (pDekont[i]->pIslem[j]->Tutar<0){
				if (pDekont[i]->pIslem[j]->mTuru == 1){
					kesinti = ((int)(-pDekont[i]->pIslem[j]->Tutar*2)) / 100.0;
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden Havale  %.2lf (-%.2lf)\n", 
						pDekont[i]->pIslem[j]->pTarih->Gun,
						pDekont[i]->pIslem[j]->pTarih->Ay,
						pDekont[i]->pIslem[j]->pTarih->Yil,
						pDekont[i]->pIslem[j]->pTarih->Saat,
						pDekont[i]->pIslem[j]->pTarih->Dk,
						-pDekont[i]->pIslem[j]->Tutar,
						kesinti);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", pDekont[i]->pIslem[j]->Ad, pDekont[i]->pIslem[j]->hesapNo);
				}else{
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden Havale  %.2lf\n", 
						pDekont[i]->pIslem[j]->pTarih->Gun,
						pDekont[i]->pIslem[j]->pTarih->Ay,
						pDekont[i]->pIslem[j]->pTarih->Yil,
						pDekont[i]->pIslem[j]->pTarih->Saat,
						pDekont[i]->pIslem[j]->pTarih->Dk,
						-pDekont[i]->pIslem[j]->Tutar);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", pDekont[i]->pIslem[j]->Ad, pDekont[i]->pIslem[j]->hesapNo);
				}
			}else if (pDekont[i]->pIslem[j]->Tutar>0){
				fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Gelen Havale  %.2lf\n", 
					pDekont[i]->pIslem[j]->pTarih->Gun,
					pDekont[i]->pIslem[j]->pTarih->Ay,
					pDekont[i]->pIslem[j]->pTarih->Yil,
					pDekont[i]->pIslem[j]->pTarih->Saat,
					pDekont[i]->pIslem[j]->pTarih->Dk,
					pDekont[i]->pIslem[j]->Tutar);
				fprintf(pf, "\tGonderen kisi:    %s (%d)\n", pDekont[i]->pIslem[j]->Ad, pDekont[i]->pIslem[j]->hesapNo);
			}
		}
		
	}
	fclose(pf);
	pf = fopen("dekont.txt", "r");
	system("@cls||clear");
	printf(".............aBank.............\n");
	while ((c=fgetc(pf))!=EOF){
		printf("%c", c);
	}
	fclose(pf);
	printf("\n1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis Basarili\n\n");
			exit(0);
		}break;
		case 1:{
			hesapOzeti(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}

void bankaRapor(int mS){
	FILE *pf;
	int sorgu, kontrol;
	char temp[120], c;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n\n");
	pf = fopen("rapor.txt", "r");
	while((c = fgetc(pf)) != EOF){
		printf("%c", c);
	}
	printf("\n\n1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis basarili\n\n");
			exit(0);
		}break;
		case 1:{
			MusteriIslem(mS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}

void islemKaydi(int mS, int hS, int iT, int iH, double iTutar){
	int iS = paBank->pMusteri[mS]->pHesap[hS]->islemSayisi;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->pTarih->Yil = tm.tm_year + 1900;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->pTarih->Ay = tm.tm_mon + 1;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->pTarih->Gun = tm.tm_mday;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->pTarih->Saat = tm.tm_hour;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->pTarih->Dk = tm.tm_min;

	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->iTuru = iT;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->iTutar = iTutar;
	paBank->pMusteri[mS]->pHesap[hS]->pIslem[iS]->iHesap = iH;
	
	paBank->pMusteri[mS]->pHesap[hS]->islemSayisi++;
}

double cekilenPara(int mS){
	int i, j, iYil, iAy, iGun, Yil, Ay, Gun;
	double limit = 0;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	Yil = tm.tm_year + 1900;
	Ay = tm.tm_mon + 1;
	Gun = tm.tm_mday;
	for (i=0; i<paBank->pMusteri[mS]->hesapSayisi; i++){
		for (j=0; j<paBank->pMusteri[mS]->pHesap[i]->islemSayisi; j++){
			iYil = paBank->pMusteri[mS]->pHesap[i]->pIslem[j]->pTarih->Yil;
			iAy = paBank->pMusteri[mS]->pHesap[i]->pIslem[j]->pTarih->Ay;
			iGun = paBank->pMusteri[mS]->pHesap[i]->pIslem[j]->pTarih->Gun;
			if (Yil == iYil && Ay == iAy && Gun == iGun && paBank->pMusteri[mS]->pHesap[i]->pIslem[j]->iTuru == 1){
				limit -= paBank->pMusteri[mS]->pHesap[i]->pIslem[j]->iTutar;
			}
		}
	}
	return limit;
}