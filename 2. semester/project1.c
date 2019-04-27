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
	Tarih tarih;
}Islem;

typedef struct{
	int hesapNo;
	int islemSayisi;
	double bakiye;
	Islem islem[1000];
}Hesap;

typedef struct{
	int hesapNo;
	double tcNo;
}transferHesap;

typedef struct{

	Hesap hesap[100];
	transferHesap tHesap[100];
	double tBakiye;
	char Ad[50];
	char Sifre[50];
	double tcNo;
	int hesapSayisi;
	int tHesapSayisi;
	int mTuru;//1->bireysel || 2->ticari
}Musteri;

typedef struct{
	Musteri musteri[1000];
	int mSayisi;
}Banka;

Banka aBank;
void AnaMenu();
void YeniMusteri();
void MusteriIslem(int mS);
void Guncelle();
void VeriAl();
void hesapIslem(int mS, int hS);
void paraCek(int mS, int hS);
void paraYatir(int mS, int hS);
void havaleGonder(int mS, int hS);
void hHesapKayit(int mS);
void hesapAc(int mS);
void hesapSil(int mS);
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);
int hesapSec(int mS, int s);
int HesapNoOlustur();
int tcNoKontrol(double tcNo);
int hNoKontrol(int hesapNo, int n);
char *isimDuzelt(char isim[50]);
char *sifrele(char sifre[50]);
void strAl(char str[50]);

int main(){
	AnaMenu();
}

void AnaMenu(){
	VeriAl();
	int sorgu, kontrol;
	char temp[50];
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("1-)\tYeni Musteri Kaydi\n");
	printf("2-)\tMusteri Islemleri\n");
	printf("0-)\tCikis\n");
	printf("Secim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol == 0);
	system("@cls||clear");
	switch (sorgu){
		case 0: exit(1); break;
		case 1:{
			YeniMusteri();
		}break;
		case 2:{
			MusteriIslem(-1);
		}break;
	}
}

void MusteriIslem(int mS){
	int sonuc, sorgu, kontrol, t=0;
	char temp[50];
	double dTemp;
	if (mS == -1){
		char sifre[50];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc Numaranizi Giriniz: ");
		do{
			strAl(temp);
			kontrol = sscanf(temp, "%lf", &dTemp);
			sonuc = tcNoKontrol(dTemp);
			if (!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}while(!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1 && kontrol==1));

		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (t!=0) printf("Hatali sifre!\n");
			printf("Şifrenizi Giriniz: ");
			strAl(sifre);
			t = strcmp(aBank.musteri[sonuc].Sifre, sifrele(sifre));
		}while(t!=0);
		mS=sonuc;
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hosgeldin, %s\n", isimDuzelt(aBank.musteri[mS].Ad));
	printf("1-)\tHesap Sec\n2-)\tHesap Aç\n3-)\tHesap Sil\n4-)\tHavale Hesabi Kaydet\n0-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>4 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>4 || kontrol == 0);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hesapIslem(mS, -1);
		}break;
		case 2:{
			hesapAc(mS);
		}break;
		case 3:{
			hesapSil(mS);
		}break;
		case 4:{
			hHesapKayit(mS);
		}break;
	}
}


void YeniMusteri(){
	int sorgu, kontrol;
	double dTemp;
	char s1[50], s2[50], temp[50];
	printf(".............aBank.............\n");
	printf("1-)\tBireysel Musteri\n");
	printf("2-)\tTicari Musteri\n");
	printf("0-)\tAna Menu\n");
	printf("Secim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol == 0);
	if (sorgu == 0) AnaMenu();
	aBank.musteri[aBank.mSayisi].mTuru = sorgu;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Ad Soyad: ");
	strAl(aBank.musteri[aBank.mSayisi].Ad);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc Numaranizi Giriniz: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (!(dTemp>10000000000 && dTemp<99999999999 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		else if (tcNoKontrol(dTemp) != -1) printf("Bu Tc Numarasi Kayitli!\nTekrar Deneyiniz: ");
	}while(!(dTemp>10000000000&&dTemp<99999999999&&tcNoKontrol(dTemp) == -1 && kontrol==1));


	system("@cls||clear");
	printf(".............aBank.............\n");
	do{
		if (strcmp(s1,s2) != 0){
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hatali Giris!\n");
		}
		printf("Sifrenizi Giriniz: ");
		strAl(s1);
		printf("Tekrar Giriniz: ");
		strAl(s2);
	}while(strcmp(s1,s2) != 0);
	strcpy(aBank.musteri[aBank.mSayisi].Sifre, sifrele(s1));

	aBank.musteri[aBank.mSayisi].hesapSayisi = 1;
	aBank.musteri[aBank.mSayisi].tHesapSayisi = 0;
	aBank.musteri[aBank.mSayisi].hesap[0].hesapNo = HesapNoOlustur();
	aBank.musteri[aBank.mSayisi].hesap[0].bakiye = 0;
	aBank.musteri[aBank.mSayisi].hesap[0].islemSayisi = 0;
	aBank.mSayisi++;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabiniz kuruldu...\n\n");
	printf("Adiniz: %s\n", isimDuzelt(aBank.musteri[aBank.mSayisi-1].Ad));
	printf("Tc Numaraniz: %.lf\n", aBank.musteri[aBank.mSayisi-1].tcNo);
	printf("Hesap Numaraniz: %d\n", aBank.musteri[aBank.mSayisi-1].hesap[0].hesapNo);
	printf("Sifreniz: %s\n\n", s2);
	printf("1-)\tAna Menu\n2-)\tCikis\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<1 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2 || kontrol == 0);
	if (sorgu == 1){
		AnaMenu();
	}
	else if (sorgu == 2) exit(1);
}
void VeriAl(){
	int i, j, mNo;
	char temp[50];
	aBank.mSayisi=0;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	if (pf!=NULL){
		while(!feof(pf)){
			fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
			strcpy(aBank.musteri[mNo-1].Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
			fscanf(pf, " Ad Soyad: %s", aBank.musteri[mNo-1].Ad);
			fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
			for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
				fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
				fscanf(pf, " Islem Sayisi: %d", &aBank.musteri[mNo-1].hesap[i].islemSayisi);
				if (aBank.musteri[mNo-1].hesap[i].islemSayisi!=0){
					for (j=0; j<aBank.musteri[mNo-1].hesap[i].islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Gun,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Ay,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Yil,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Saat,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Dk,
							&aBank.musteri[mNo-1].hesap[i].islem[j].iTutar);
						fscanf(pf, " %d - %d", 
							&aBank.musteri[mNo-1].hesap[i].islem[j].iTuru,
							&aBank.musteri[mNo-1].hesap[i].islem[j].iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &aBank.musteri[mNo-1].tHesapSayisi);
			if (aBank.musteri[mNo-1].tHesapSayisi != 0){
				for (i=0; i<aBank.musteri[mNo-1].tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &aBank.musteri[mNo-1].tHesap[i].hesapNo);
					fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tHesap[i].tcNo);
				}
			}
			aBank.musteri[mNo-1].mTuru = 1;
			aBank.mSayisi++;
		}
		fclose(pf);
	}
	pf = fopen("ticariMusteri.txt", "r");
	if (pf!=NULL){
		while(!feof(pf)){
			fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
			strcpy(aBank.musteri[mNo-1].Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
			fscanf(pf, " Ad Soyad: %s", aBank.musteri[mNo-1].Ad);
			fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
			for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
				fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
				fscanf(pf, " Islem Sayisi: %d", &aBank.musteri[mNo-1].hesap[i].islemSayisi);
				if (aBank.musteri[mNo-1].hesap[i].islemSayisi!=0){
					for (j=0; j<aBank.musteri[mNo-1].hesap[i].islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Gun,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Ay,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Yil,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Saat,
							&aBank.musteri[mNo-1].hesap[i].islem[j].tarih.Dk,
							&aBank.musteri[mNo-1].hesap[i].islem[j].iTutar);
						fscanf(pf, " %d - %d", 
							&aBank.musteri[mNo-1].hesap[i].islem[j].iTuru,
							&aBank.musteri[mNo-1].hesap[i].islem[j].iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &aBank.musteri[mNo-1].tHesapSayisi);
			if (aBank.musteri[mNo-1].tHesapSayisi != 0){
				for (i=0; i<aBank.musteri[mNo-1].tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &aBank.musteri[mNo-1].tHesap[i].hesapNo);
					fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tHesap[i].tcNo);
				}
			}
			aBank.musteri[mNo-1].mTuru = 2;
			aBank.mSayisi++;
		}
		fclose(pf);
	}
	for (i=0; i<aBank.mSayisi; i++){
		aBank.musteri[i].tBakiye = 0;
		for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
			aBank.musteri[i].tBakiye += aBank.musteri[i].hesap[j].bakiye;
		}
	}
}

void Guncelle(){
	int i, j, k, b=0, t=0;
	FILE *pf1, *pf2;
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].mTuru == 1){
			if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));
			pf1 = fopen("bireyselMusteri.txt", "a");
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Musteri: %d / [ %s ]", i+1, aBank.musteri[i].Sifre);
			fprintf(pf1, "\n\tTc-No: %.lf", aBank.musteri[i].tcNo);
			fprintf(pf1, "\n\tAd Soyad: %s", aBank.musteri[i].Ad);
			fprintf(pf1, "\n\tHesap Sayisi: %d", aBank.musteri[i].hesapSayisi);
			for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].hesap[j].hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", aBank.musteri[i].hesap[j].bakiye);
				fprintf(pf1, "\n\t\tIslem Sayisi: %d", aBank.musteri[i].hesap[j].islemSayisi);
				for (k=0; k<aBank.musteri[i].hesap[j].islemSayisi; k++){
					fprintf(pf1, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						aBank.musteri[i].hesap[j].islem[k].tarih.Gun,
						aBank.musteri[i].hesap[j].islem[k].tarih.Ay,
						aBank.musteri[i].hesap[j].islem[k].tarih.Yil,
						aBank.musteri[i].hesap[j].islem[k].tarih.Saat,
						aBank.musteri[i].hesap[j].islem[k].tarih.Dk,
						aBank.musteri[i].hesap[j].islem[k].iTutar);
					fprintf(pf1, "\n\t\t\t\t%d - %d", 
						aBank.musteri[i].hesap[j].islem[k].iTuru,
						aBank.musteri[i].hesap[j].islem[k].iHesap);
				}
			}
			fprintf(pf1, "\n\tKayitli Havale Hesap Sayisi: %d", aBank.musteri[i].tHesapSayisi);
			for (j=0; j<aBank.musteri[i].tHesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].tHesap[j].hesapNo);
				fprintf(pf1, "\n\t\tTc-No: %.lf", aBank.musteri[i].tHesap[j].tcNo);
			}
			fclose(pf1);
			b=1;
		}else if (aBank.musteri[i].mTuru == 2){
			if (t==0) fclose(fopen("ticariMusteri.txt", "w"));
			pf2 = fopen("ticariMusteri.txt", "a");
			if (t!=0) fprintf(pf2, "\n");
			fprintf(pf2, "Musteri: %d / [ %s ]", i+1, aBank.musteri[i].Sifre);
			fprintf(pf2, "\n\tTc-No: %.lf", aBank.musteri[i].tcNo);
			fprintf(pf2, "\n\tAd Soyad: %s", aBank.musteri[i].Ad);
			fprintf(pf2, "\n\tHesap Sayisi: %d", aBank.musteri[i].hesapSayisi);
			for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].hesap[j].hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", aBank.musteri[i].hesap[j].bakiye);
				fprintf(pf2, "\n\t\tIslem Sayisi: %d", aBank.musteri[i].hesap[j].islemSayisi);
				for (k=0; k<aBank.musteri[i].hesap[j].islemSayisi; k++){
					fprintf(pf2, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						aBank.musteri[i].hesap[j].islem[k].tarih.Gun,
						aBank.musteri[i].hesap[j].islem[k].tarih.Ay,
						aBank.musteri[i].hesap[j].islem[k].tarih.Yil,
						aBank.musteri[i].hesap[j].islem[k].tarih.Saat,
						aBank.musteri[i].hesap[j].islem[k].tarih.Dk,
						aBank.musteri[i].hesap[j].islem[k].iTutar);
					fprintf(pf2, "\n\t\t\t\t%d - %d", 
						aBank.musteri[i].hesap[j].islem[k].iTuru,
						aBank.musteri[i].hesap[j].islem[k].iHesap);
				}
			}
			fprintf(pf2, "\n\tKayitli Havale Hesap Sayisi: %d", aBank.musteri[i].tHesapSayisi);
			for (j=0; j<aBank.musteri[i].tHesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, aBank.musteri[i].tHesap[j].hesapNo);
				fprintf(pf2, "\n\t\tTc-No: %.lf", aBank.musteri[i].tHesap[j].tcNo);
			}
			fclose(pf2);
			t=1;
		}
	}

}
void hesapIslem(int mS, int hS){
	char temp[50];
	int sorgu, kontrol;
	if (hS == -1) hS = hesapSec(mS, 1);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabinizdaki bakiyeniz: %.2lf TL'dir.\n", aBank.musteri[mS].hesap[hS].bakiye);
	printf("Toplam bakiyeniz: %.2lf TL'dir.\n\n", aBank.musteri[mS].tBakiye);
	printf("1-)\tPara Cekme\n2-)\tPara Yatırma\n3-)\tHavale\n4-)\tMusteri Islemleri\n0-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>4 ||kontrol == 0) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if ((sorgu == 1 || sorgu == 3) && aBank.musteri[mS].tBakiye == 0){
			printf("Bu islem icin hesaplarinizda yeterli para yok!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>4 || kontrol == 0);
	switch (sorgu){
		case 0:{
			AnaMenu();
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
			MusteriIslem(mS);
		}
	}
}
void paraCek(int mS, int hS){
	int i, kontrol, sorgu;
	char temp[50];
	double dTemp, limit;
	limit = (aBank.musteri[mS].mTuru == 1) ? 750 : 1500;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Cekmek istediğiniz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>aBank.musteri[mS].tBakiye) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", aBank.musteri[mS].tBakiye);
		else if (dTemp>limit) printf("Para cekme limitiniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", limit);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=aBank.musteri[mS].tBakiye && dTemp<=limit && kontrol==1));
	if (dTemp<=aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
		aBank.musteri[mS].hesap[hS].bakiye -= dTemp;
		islemKaydi(mS, hS, 1, aBank.musteri[mS].hesap[hS].hesapNo, -dTemp);
	}else{
		if (dTemp>aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
			dTemp -= aBank.musteri[mS].hesap[hS].bakiye;
			islemKaydi(mS, hS, 1, aBank.musteri[mS].hesap[hS].hesapNo, -aBank.musteri[mS].hesap[hS].bakiye);
			aBank.musteri[mS].hesap[hS].bakiye = 0;
		}
		for (i=0; i<aBank.musteri[mS].hesapSayisi; i++){
			if (aBank.musteri[mS].hesap[i].bakiye != 0){
				if (dTemp<=aBank.musteri[mS].hesap[i].bakiye){
					aBank.musteri[mS].hesap[i].bakiye -= dTemp;
					islemKaydi(mS, i, 1, aBank.musteri[mS].hesap[i].hesapNo, -dTemp);
				}else if (dTemp>aBank.musteri[mS].hesap[i].bakiye){
					dTemp -= aBank.musteri[mS].hesap[i].bakiye;
					islemKaydi(mS, i, 1, aBank.musteri[mS].hesap[i].hesapNo, -aBank.musteri[mS].hesap[i].bakiye);
					aBank.musteri[mS].hesap[i].bakiye = 0;
				}
			}
		}
	}
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol == 0);
	switch (sorgu){
		case 0:{
			exit(1);
		}
		case 1:{
			VeriAl();
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void paraYatir(int mS, int hS){
	int i, kontrol, sorgu;
	char temp[50];
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Yatirmak istediğiniz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && kontrol==1));

	aBank.musteri[mS].hesap[hS].bakiye += dTemp;
	islemKaydi(mS, hS, 2, aBank.musteri[mS].hesap[hS].hesapNo, dTemp);
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol == 0);
	switch (sorgu){
		case 0:{
			exit(1);
		}
		case 1:{
			VeriAl();
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void havaleGonder(int mS, int hS){
	double dTemp;
	int i, tHesapS, hhS, hmS, tHesapNo, sorgu, kontrol;
	char temp[50];
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (aBank.musteri[mS].tHesapSayisi != 0){
		printf("1-)\tKayitli Hesabina Gonder\n2-)\tHesap No Girerek Gonder\nSecim: ");
		do{
			strAl(temp);
			kontrol = sscanf(temp, "%d", &sorgu);
			if(sorgu<1 || sorgu>2 || kontrol == 0) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<1 || sorgu>2 || kontrol == 0);
		switch (sorgu){
			case 1:{
				tHesapS = hesapSec(mS, 2);
				tHesapNo = aBank.musteri[mS].tHesap[tHesapS].hesapNo;
			}break;
			case 2:{
				system("@cls||clear");
				printf(".............aBank.............\n");
				printf("Hesap numarasini giriniz: ");
				do{
					strAl(temp);
					kontrol = sscanf(temp, "%d", &sorgu);
					if (!(sorgu>100000000 && sorgu<999999999 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
					else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
				}while(!(sorgu>100000000 && sorgu<999999999 && hNoKontrol(sorgu, 1) != -1 && kontrol==1));
				tHesapNo = sorgu;
			}break;
		}
	}else{
		printf("Hesap numarasini giriniz: ");
		do{
			strAl(temp);
			kontrol = sscanf(temp, "%d", &sorgu);
			if (!(sorgu>100000000 && sorgu<999999999 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
		}while(!(sorgu>100000000 && sorgu<999999999 && hNoKontrol(sorgu, 1) != -1 && kontrol==1));
		tHesapNo = sorgu;
	}

	hhS = hNoKontrol(tHesapNo, 2);
	hmS = hNoKontrol(tHesapNo, 1);

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Gondermek istediğiniz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>aBank.musteri[mS].tBakiye) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", aBank.musteri[mS].tBakiye);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=aBank.musteri[mS].tBakiye && kontrol==1));
	if (dTemp<=aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
		aBank.musteri[mS].hesap[hS].bakiye -= dTemp;
		islemKaydi(mS, hS, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -dTemp);

		aBank.musteri[hmS].hesap[hhS].bakiye += dTemp;
		islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[hS].hesapNo, dTemp);
	}else{
		if (dTemp>aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
			dTemp -= aBank.musteri[mS].hesap[hS].bakiye;
			islemKaydi(mS, hS, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -aBank.musteri[mS].hesap[hS].bakiye);
			
			aBank.musteri[hmS].hesap[hhS].bakiye += aBank.musteri[mS].hesap[hS].bakiye;
			islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[hS].hesapNo, aBank.musteri[mS].hesap[hS].bakiye);
			aBank.musteri[mS].hesap[hS].bakiye = 0;
		}
		for (i=0; i<aBank.musteri[mS].hesapSayisi; i++){
			if (aBank.musteri[mS].hesap[i].bakiye != 0){
				if (dTemp<=aBank.musteri[mS].hesap[i].bakiye){
					aBank.musteri[mS].hesap[i].bakiye -= dTemp;
					islemKaydi(mS, i, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -dTemp);

					aBank.musteri[hmS].hesap[hhS].bakiye += dTemp;
					islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[i].hesapNo, dTemp);

				}else if (dTemp>aBank.musteri[mS].hesap[i].bakiye){
					dTemp -= aBank.musteri[mS].hesap[i].bakiye;
					islemKaydi(mS, i, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -aBank.musteri[mS].hesap[i].bakiye);
					
					aBank.musteri[hmS].hesap[hhS].bakiye += aBank.musteri[mS].hesap[i].bakiye;
					islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[i].hesapNo, aBank.musteri[mS].hesap[i].bakiye);
					aBank.musteri[mS].hesap[i].bakiye = 0;
				}
			}
		}
	}
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<0 || sorgu>2 || kontrol == 0);
	switch (sorgu){
		case 0:{
			exit(1);
		}
		case 1:{
			VeriAl();
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void hHesapKayit(int mS){

}
void hesapAc(int mS){
	int sorgu, kontrol, hS = aBank.musteri[mS].hesapSayisi;
	char temp[50];
	aBank.musteri[mS].hesap[hS].hesapNo = HesapNoOlustur();
	aBank.musteri[mS].hesap[hS].bakiye = 0;
	aBank.musteri[mS].hesap[hS].islemSayisi = 0;
	aBank.musteri[mS].hesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabiniz Acildi..\n1-)\tMusteri Islemleri\n2-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<1 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2 || kontrol == 0);
	switch (sorgu){
		case 1:{
			VeriAl();
			MusteriIslem(mS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}
void hesapSil(int mS){

}
int hesapSec(int mS, int s){ //s 1 ise normal hesap 2 ise transfer hesap
	int i, sorgu, kontrol, hNo, n = aBank.musteri[mS].hesapSayisi;
	char temp[50];
	n = (s==1) ? aBank.musteri[mS].hesapSayisi : aBank.musteri[mS].tHesapSayisi;
	if (n==1) return 0;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem Yapilacak Hesap Numarasini Seciniz...\n\n");
	for (i=0; i<n; i++){
		hNo = (s==1) ? aBank.musteri[mS].hesap[i].hesapNo : aBank.musteri[mS].tHesap[i].hesapNo;
		printf("%d-) %d\t",i+1, hNo);
		if (i%3 == 2) printf("\n");
	}
	if (i%3 != 0) printf("\n");
	printf("\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<1 || sorgu>n || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>n || kontrol == 0);
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
char *sifrele(char sifre[50]){
	int i, temp = *sifre, len = strlen(sifre);
	for (i=0; i<len; i++){
		temp += ((i+1) * (*(sifre+i) + 11) + *(sifre+len-i-1) + i);
		*(sifre+i) = temp%21 + 33;
	}
	return sifre;
}

int tcNoKontrol(double tcNo){
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].tcNo == tcNo) return i;
	}
	return -1;
}

int hNoKontrol(int hesapNo, int n){
	int mS, hS;
	for (mS=0; mS<aBank.mSayisi; mS++){
		for (hS=0; hS<aBank.musteri[mS].hesapSayisi; hS++){
			if (aBank.musteri[mS].hesap[hS].hesapNo == hesapNo) return (n==1)?mS:hS;
		}
	}
	return -1;
}

char *isimDuzelt(char isim[50]){
	int i;
	for (i=0; i<(int)strlen(isim); i++){
		if (*(isim+i) == '-') *(isim+i) = ' ';
	}
	return isim;
}

void strAl(char str[50]){
	int t;
	char cTemp;
	scanf("%s", str);
	t = strlen(str);
	do{
		scanf("%c", &cTemp);
		if (cTemp == ' ') cTemp = '-';
		*(str + t++) = cTemp;
	}while (cTemp != '\n');
	*(str+t-1) = 0;
}

void islemKaydi(int mS, int hS, int iT, int iH, double iTutar){
	int iS = aBank.musteri[mS].hesap[hS].islemSayisi;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	aBank.musteri[mS].hesap[hS].islem[iS].tarih.Yil = tm.tm_year + 1900;
	aBank.musteri[mS].hesap[hS].islem[iS].tarih.Ay = tm.tm_mon + 1;
	aBank.musteri[mS].hesap[hS].islem[iS].tarih.Gun = tm.tm_mday;
	aBank.musteri[mS].hesap[hS].islem[iS].tarih.Saat = tm.tm_hour;
	aBank.musteri[mS].hesap[hS].islem[iS].tarih.Dk = tm.tm_min;

	aBank.musteri[mS].hesap[hS].islem[iS].iTuru = iT;
	aBank.musteri[mS].hesap[hS].islem[iS].iTutar = iTutar;
	aBank.musteri[mS].hesap[hS].islem[iS].iHesap = iH;
	
	aBank.musteri[mS].hesap[hS].islemSayisi++;
}

//--Havale menüsünde kayıtlı havale hesabı varsa kayıtlı havale hesaplarına
//yollama seçeneği ve hesap no ile gönderme seçenekleri gelecek yoksa direk 
//hesap no ile gönderme ekranı gelecek
//--Hesap silmede struct elemanı eşitleme kullanılacak
//--Güncelle fonksiyonu geliştirilip rapor yazdırmada yaptırılacak
//--veriAl fonksiyonu geliştirilip rapor okumasıda yaptırılacak
