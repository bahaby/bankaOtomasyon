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
}transferHesap;

typedef struct{

	Hesap hesap[100];
	transferHesap tHesap[100];
	double tBakiye;
	char Ad[120];
	char Sifre[120];
	double tcNo;
	int hesapSayisi;
	int tHesapSayisi;
	int mTuru;//1->bireysel || 2->ticari
}Musteri;

typedef struct{
	Musteri musteri[1000];
	int mSayisi;
}Banka;

typedef struct{
	char Ad[120];
	int hesapNo;
	int iTuru;
	int mTuru;
	double Tutar;
	Tarih tarih;
}dIslem;

typedef struct{
	dIslem islem[1000];
	int islemSayisi;
}Dekont;

Banka aBank;
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
void hHesapKayit(int mS);
void hesapAc(int mS);
void hesapSil(int mS, int s);
void hesapOzeti(int mS, int hS);
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);
int hesapSec(int mS, int s);
int HesapNoOlustur();
int tcNoKontrol(double tcNo);
int hNoKontrol(int hesapNo, int n);
char *isimDuzelt(char isim[120]);
char *sifrele(char sifre[120]);
void strAl(char str[120]);

int main(){
	AnaMenu();
}

void AnaMenu(){
	VeriAl();
	int sorgu, kontrol;
	char temp[120];
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
	char temp[120];
	double dTemp;
	if (mS == -1){
		char sifre[120];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc Numaranizi Giriniz: ");
		do{
			strAl(temp);
			kontrol = sscanf(temp, "%lf", &dTemp);
			sonuc = tcNoKontrol(dTemp);
			if (!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}while(!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1 && kontrol==1));
		mS=sonuc;
		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (t!=0) printf("Hatali sifre!\n");
			printf("Sifrenizi Giriniz: ");
			strAl(sifre);
			t = strcmp(aBank.musteri[mS].Sifre, sifrele(sifre));
		}while(t!=0);
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hosgeldin, %s\n", isimDuzelt(aBank.musteri[mS].Ad));
	printf("1-)\tHesap Sec\n2-)\tHesap Ac\n3-)\tHesap Sil\n4-)\tHavale Hesabi Kaydet\n5-)\tHavale Hesabi Sil\n6-)\tRapor Al\n0-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>6 || kontrol == 0) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if (sorgu == 3 && aBank.musteri[mS].hesapSayisi == 1){
			printf("Son Hesabinizi silemezsiniz!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 5 && aBank.musteri[mS].tHesapSayisi == 0){
			printf("Kayitli hesabiniz kalmamistir!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>6 || kontrol == 0);

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
			hesapSil(mS, 1);
		}break;
		case 4:{
			hHesapKayit(mS);
		}break;
		case 5:{
			hesapSil(mS, 2);
		}break;
		case 6:{
			bankaRapor(mS);
		}break;
	}
}


void YeniMusteri(){
	int sorgu, kontrol;
	double dTemp;
	char s1[120]={}, s2[120]={}, temp[120];
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
	aBank.musteri[aBank.mSayisi].tcNo = dTemp;

	system("@cls||clear");
	printf(".............aBank.............\n");
	do{
		printf("Sifrenizi Giriniz: ");
		strAl(s1);
		printf("Tekrar Giriniz: ");
		strAl(s2);
		if (strcmp(s1, s2) != 0){
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hatali Giris!\n");
		}else if ((int)strlen(s1)<4){
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Sifreniz en az 4 haneli olmali!\n");
		}
	}while(strcmp(s1, s2) != 0 || (int)strlen(s1)<4);
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
	char temp[120];
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
	int i, j, k, iTuru, b=0, t=0;
	double gelen=0, giden=0, kar=0, kesinti;
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
			}
			fclose(pf2);
			t=1;
		}
	}

	for (i=0; i<aBank.mSayisi; i++){
		for (j=0; j<aBank.musteri[i].hesapSayisi; j++){
			for (k=0; k<aBank.musteri[i].hesap[j].islemSayisi; k++){
				iTuru = aBank.musteri[i].hesap[j].islem[k].iTuru;
				if (iTuru == 1) giden += aBank.musteri[i].hesap[j].islem[k].iTutar;
				else if (iTuru == 2) gelen += aBank.musteri[i].hesap[j].islem[k].iTutar;
				else if (iTuru == 3 && aBank.musteri[i].mTuru == 1){
					if (aBank.musteri[i].hesap[j].islem[k].iTutar < 0){
						kesinti = ((int)(-aBank.musteri[i].hesap[j].islem[k].iTutar*2)) / 100.0;
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
	fprintf(pf1, "Toplam kar:                  %.2lf", kar);
	fclose(pf1);
	
}
void hesapIslem(int mS, int hS){
	char temp[120];
	int sorgu, kontrol;
	if (hS == -1) hS = hesapSec(mS, 1);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabinizdaki bakiyeniz: %.2lf TL'dir.\n", aBank.musteri[mS].hesap[hS].bakiye);
	printf("Toplam bakiyeniz: %.2lf TL'dir.\n\n", aBank.musteri[mS].tBakiye);
	printf("1-)\tPara Cekme\n2-)\tPara Yatirma\n3-)\tHavale\n4-)\tMusteri Islemleri\n5-)\tHesap Ozeti\n0-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<0 || sorgu>5 ||kontrol == 0) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if ((sorgu == 1 || sorgu == 3) && aBank.musteri[mS].tBakiye == 0){
			printf("Bu islem icin hesaplarinizda yeterli para yok!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}
	}while(sorgu<0 || sorgu>5 || kontrol == 0);
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
		case 5:{
			hesapOzeti(mS, hS);
		}
	}
}
void paraCek(int mS, int hS){
	int i, kontrol, sorgu;
	char temp[120];
	double dTemp, limit;
	limit = (aBank.musteri[mS].mTuru == 1) ? 750 : 1500;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Cekmek istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>aBank.musteri[mS].tBakiye) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", aBank.musteri[mS].tBakiye);
		else if (dTemp>limit) printf("Para cekme limitiniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", limit);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=aBank.musteri[mS].tBakiye && dTemp<=limit && kontrol==1));
	dTemp = ((int)(dTemp*100)) / 100.0;
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
	int kontrol, sorgu;
	char temp[120];
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Yatirmak istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && kontrol==1));
	dTemp = ((int)(dTemp*100)) / 100.0;

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
	double dTemp, kesinti;
	int i, tHesapS, hhS, hmS, tHesapNo, sorgu, kontrol;
	char temp[120];
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (aBank.musteri[mS].tHesapSayisi != 0){
		printf("1-)\tKayitli Havale Hesabina Gonder\n2-)\tHesap No Girerek Gonder\nSecim: ");
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
				if (tHesapNo == aBank.musteri[mS].hesap[hS].hesapNo){
					system("@cls||clear");
					printf(".............aBank.............\n");
					printf("Islem yaptiginiz hesabi secemezsiniz...\n\n");
					printf("1-)\tGeri Don\nSecim: ");
					do{
						strAl(temp);
						kontrol = sscanf(temp, "%d", &sorgu);
						if(sorgu<0 || sorgu>1 || kontrol == 0) {
							printf("Hatali Giris!\nTekrar Deneyiniz: ");
						}
					}while(sorgu<0 || sorgu>1 || kontrol == 0);
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
	printf("Gondermek istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%lf", &dTemp);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (dTemp>aBank.musteri[mS].tBakiye && kontrol == 1) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", aBank.musteri[mS].tBakiye);
		else if (!(dTemp>0 && kontrol==1)) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && dTemp<=aBank.musteri[mS].tBakiye && kontrol==1));
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
		aBank.musteri[mS].hesap[hS].bakiye -= dTemp;
		islemKaydi(mS, hS, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -dTemp);
		kesinti = (aBank.musteri[mS].mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;

		aBank.musteri[hmS].hesap[hhS].bakiye += (dTemp - kesinti);
		islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[hS].hesapNo, dTemp - kesinti);
	}else{
		if (dTemp>aBank.musteri[mS].hesap[hS].bakiye && aBank.musteri[mS].hesap[hS].bakiye != 0){
			dTemp -= aBank.musteri[mS].hesap[hS].bakiye;
			islemKaydi(mS, hS, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -aBank.musteri[mS].hesap[hS].bakiye);
			kesinti = (aBank.musteri[mS].mTuru == 1) ? ((int)(aBank.musteri[mS].hesap[hS].bakiye*2)) / 100.0 : 0;
			
			aBank.musteri[hmS].hesap[hhS].bakiye += (aBank.musteri[mS].hesap[hS].bakiye - kesinti);
			islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[hS].hesapNo, aBank.musteri[mS].hesap[hS].bakiye - kesinti);
			aBank.musteri[mS].hesap[hS].bakiye = 0;
		}
		for (i=0; i<aBank.musteri[mS].hesapSayisi; i++){
			if (aBank.musteri[mS].hesap[i].bakiye != 0){
				if (dTemp<=aBank.musteri[mS].hesap[i].bakiye){
					aBank.musteri[mS].hesap[i].bakiye -= dTemp;
					islemKaydi(mS, i, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -dTemp);
					kesinti = (aBank.musteri[mS].mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;

					aBank.musteri[hmS].hesap[hhS].bakiye += (dTemp - kesinti);
					islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[i].hesapNo, dTemp - kesinti);

				}else if (dTemp>aBank.musteri[mS].hesap[i].bakiye){
					dTemp -= aBank.musteri[mS].hesap[i].bakiye;
					islemKaydi(mS, i, 3, aBank.musteri[hmS].hesap[hhS].hesapNo, -aBank.musteri[mS].hesap[i].bakiye);
					kesinti = (aBank.musteri[mS].mTuru == 1) ? ((int)(aBank.musteri[mS].hesap[i].bakiye*2)) / 100.0 : 0;
					
					aBank.musteri[hmS].hesap[hhS].bakiye += (aBank.musteri[mS].hesap[i].bakiye - kesinti);
					islemKaydi(hmS, hhS, 3, aBank.musteri[mS].hesap[i].hesapNo, aBank.musteri[mS].hesap[i].bakiye - kesinti);
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
	int sorgu, kontrol;
	char temp[120];
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesap numarasini giriniz: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if (!(sorgu>100000000 && sorgu<999999999 && kontrol==1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		else if (hNoKontrol(sorgu, 1) == -1) printf("Boyle bir hesap numarasi yok!\nTekrar Deneyiniz: ");
	}while(!(sorgu>100000000 && sorgu<999999999 && hNoKontrol(sorgu, 1) != -1 && kontrol==1));
	aBank.musteri[mS].tHesap[aBank.musteri[mS].tHesapSayisi].hesapNo = sorgu;
	aBank.musteri[mS].tHesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri Don\n2-)\tAna Menu\nSecim: ");
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
void hesapAc(int mS){
	int sorgu, kontrol, hS = aBank.musteri[mS].hesapSayisi;
	char temp[120];
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
void hesapSil(int mS, int s){//s 1 ise normal hesap 2 ise transfer hesap
	int i, n, sorgu, kontrol, shS;
	char temp[120];
	shS =  hesapSec(mS, s);
	n = (s==1) ? aBank.musteri[mS].hesapSayisi : aBank.musteri[mS].tHesapSayisi;
	if (s==1 && aBank.musteri[mS].hesap[shS].bakiye!=0){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesabinizda para varken hesabinizi silemezsiniz!\n1-)\tGeri Don\n2-)\tAna Menu\nSecim: ");
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
	}else{
		for (i=shS; i<n; i++){
			if (s==1){
				aBank.musteri[mS].hesap[i] = aBank.musteri[mS].hesap[i+1]; 
			}else{
				aBank.musteri[mS].tHesap[i] = aBank.musteri[mS].tHesap[i+1]; 
			}
		}
		(s==1) ? aBank.musteri[mS].hesapSayisi-- : aBank.musteri[mS].tHesapSayisi--;
		Guncelle();
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
		printf("1-)\tGeri Don\n2-)\tAna Menu\nSecim: ");
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
}
int hesapSec(int mS, int s){ //s 1 ise normal hesap 2 ise transfer hesap
	int i, sorgu, kontrol, hNo, n;
	char temp[120];
	n = (s==1) ? aBank.musteri[mS].hesapSayisi : aBank.musteri[mS].tHesapSayisi;
	if (n==1) return 0;
	else if(n==0) return -1;
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
char *sifrele(char sifre[120]){
	int i, temp = *sifre, len = strlen(sifre);
	for (i=0; i<15; i++){
		temp += ((i+1) * (*(sifre+(i%len)) + 11) + *(sifre+len-(i%len)-1) + i);
		*(sifre+i) = temp%93 + 33;
	}
	*(sifre+15) = '\0';
	return sifre;
}

int tcNoKontrol(double tcNo){
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].tcNo == tcNo) return i;
	}
	return -1;
}

int hNoKontrol(int hesapNo, int n){ // n 1 ise müsteri no, 2 ise hesap no
	int mS, hS;
	for (mS=0; mS<aBank.mSayisi; mS++){
		for (hS=0; hS<aBank.musteri[mS].hesapSayisi; hS++){
			if (aBank.musteri[mS].hesap[hS].hesapNo == hesapNo) return (n==1)?mS:hS;
		}
	}
	return -1;
}

char *isimDuzelt(char isim[120]){
	int i;
	for (i=0; i<(int)strlen(isim); i++){
		if (*(isim+i) == '-') *(isim+i) = ' ';
	}
	return isim;
}

void strAl(char str[120]){
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

void hesapOzeti(int mS, int hS){
	FILE *pf;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i, j, k, n, t1, t2, t3, iS, aralik, ihNo, imS, iA, iY, sorgu, kontrol;
	double kesinti;
	char c, temp[120];
	iS = aBank.musteri[mS].hesap[hS].islemSayisi;
	t1 = (aBank.musteri[mS].hesap[hS].islem[0].tarih.Yil - 1900) * 12 + aBank.musteri[mS].hesap[hS].islem[0].tarih.Ay;
	t2 = tm.tm_year * 12 + tm.tm_mon+1 ;
	aralik = t2-t1;
	Dekont dekont[aralik+1];
	int index[aralik+1];
	
	system("@cls||clear");
	printf(".............aBank.............\n");
	for (i=0; i<aralik+1; i++){
		*(index+i) = -1;
		dekont[i].islemSayisi = 0;
		dekont[i].islem[0].Tutar = 0;
		for (j=iS-1; j>=0; j--){
			t3 = (aBank.musteri[mS].hesap[hS].islem[j].tarih.Yil - 1900) * 12 + aBank.musteri[mS].hesap[hS].islem[j].tarih.Ay;
			if ((t2-t3) == i){
				ihNo = aBank.musteri[mS].hesap[hS].islem[j].iHesap;
				imS = hNoKontrol(ihNo, 1);
				strcpy(dekont[i].islem[dekont[i].islemSayisi].Ad, aBank.musteri[imS].Ad);
				dekont[i].islem[dekont[i].islemSayisi].hesapNo = ihNo;
				dekont[i].islem[dekont[i].islemSayisi].mTuru = aBank.musteri[mS].mTuru;
				dekont[i].islem[dekont[i].islemSayisi].iTuru = aBank.musteri[mS].hesap[hS].islem[j].iTuru;
				dekont[i].islem[dekont[i].islemSayisi].Tutar = aBank.musteri[mS].hesap[hS].islem[j].iTutar;
				dekont[i].islem[dekont[i].islemSayisi].tarih = aBank.musteri[mS].hesap[hS].islem[j].tarih;
				dekont[i].islemSayisi++;
			}
		}
	}
	k=0;
	for (i=0; i<aralik+1; i++){
		if (dekont[i].islem[0].Tutar != 0){
			iY = (t2 - i) / 12 + 1900;
			iA = (t2 - i -1) % 12 +1; 
			n = i-k+1;
			if (i==0) printf("%2d-)\t%0.2d - %0.2d\t", n, iY, iA);
			else printf("%2d-)\t%d - %0.2d/%0.2d\t", n, iY, iA, iA%12+1);
			if ((i-k)%2 == 1) printf("\n");
			*(index+i-k+1) = i;
		}else{
			k++;
		}
	}
	printf("\n\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<1 || sorgu>n || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>n || kontrol == 0);
	

	fclose(fopen("dekont.txt", "w"));
	pf = fopen("dekont.txt", "a");
	printf("\n\ndeneme %d\n", dekont[0].islem[0].iTuru);
	i = *(index + sorgu);
	for (j=dekont[i].islemSayisi-1; j>=0; j--){
		if (dekont[i].islem[j].iTuru == 1){
			fprintf(pf, "%0.2d.%0.2d.%0.4d - %0.2d:%0.2d  Para Cekme    %.2lf\n", 
				dekont[i].islem[j].tarih.Gun,
				dekont[i].islem[j].tarih.Ay,
				dekont[i].islem[j].tarih.Yil,
				dekont[i].islem[j].tarih.Saat,
				dekont[i].islem[j].tarih.Dk,
				-dekont[i].islem[j].Tutar);
		}else if (dekont[i].islem[j].iTuru == 2){
			fprintf(pf, "%0.2d.%0.2d.%0.4d - %0.2d:%0.2d  Para Yatirma  %.2lf\n", 
				dekont[i].islem[j].tarih.Gun,
				dekont[i].islem[j].tarih.Ay,
				dekont[i].islem[j].tarih.Yil,
				dekont[i].islem[j].tarih.Saat,
				dekont[i].islem[j].tarih.Dk,
				dekont[i].islem[j].Tutar);
		}else if (dekont[i].islem[j].iTuru == 3){
			if (dekont[i].islem[j].Tutar<0){
				if (dekont[i].islem[j].mTuru == 1)
					kesinti = ((int)(-dekont[i].islem[j].Tutar*2)) / 100.0;
				fprintf(pf, "%0.2d.%0.2d.%0.4d - %0.2d:%0.2d  Giden Havale  %.2lf (-%.2lf)\n", 
					dekont[i].islem[j].tarih.Gun,
					dekont[i].islem[j].tarih.Ay,
					dekont[i].islem[j].tarih.Yil,
					dekont[i].islem[j].tarih.Saat,
					dekont[i].islem[j].tarih.Dk,
					-dekont[i].islem[j].Tutar,
					kesinti);
				fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", dekont[i].islem[j].Ad, dekont[i].islem[j].hesapNo);
			}else if (dekont[i].islem[j].Tutar>0){
				fprintf(pf, "%0.2d.%0.2d.%0.4d - %0.2d:%0.2d  Gelen Havale  %.2lf\n", 
					dekont[i].islem[j].tarih.Gun,
					dekont[i].islem[j].tarih.Ay,
					dekont[i].islem[j].tarih.Yil,
					dekont[i].islem[j].tarih.Saat,
					dekont[i].islem[j].tarih.Dk,
					dekont[i].islem[j].Tutar);
				fprintf(pf, "\tGonderen kisi:    %s (%d)\n", dekont[i].islem[j].Ad, dekont[i].islem[j].hesapNo);
			}
		}
		
	}
	fclose(pf);
	pf = fopen("dekont.txt", "r");
	system("@cls||clear");
	printf(".............aBank.............\n\n");
	while ((c=fgetc(pf))!=EOF){
		printf("%c", c);
	}
	fclose(pf);
	printf("\n1-)\tGeri Don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
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
			hesapIslem(mS, hS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}

void bankaRapor(int mS){
	FILE *pf;
	int sorgu, kontrol;
	char temp[120];
	system("@cls||clear");
	printf(".............aBank.............\n\n");
	char c;
	pf = fopen("rapor.txt", "r");
	while((c = fgetc(pf)) != EOF){
		printf("%c", c);
	}
	printf("\n\n1-)\tGeri Don\n2-)\tAna Menu\nSecim: ");
	do{
		strAl(temp);
		kontrol = sscanf(temp, "%d", &sorgu);
		if(sorgu<1 || sorgu>2 || kontrol == 0) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2 || kontrol == 0);
	switch (sorgu){
		case 1:{
			MusteriIslem(mS);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
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