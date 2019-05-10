#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ISLEM 500
#define MAX_HESAP 100
#define MAX_MUSTERI 500
#define MAX_DEKONT 500

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
	Islem islem[MAX_ISLEM];
}Hesap;

typedef struct{
	int hesapNo;
}transferHesap;//test

typedef struct{

	Hesap hesap[MAX_HESAP];
	transferHesap tHesap[MAX_HESAP];
	double tBakiye;
	char Ad[120];
	char Sifre[120];
	double tcNo;
	int hesapSayisi;
	int tHesapSayisi;
	int musteriNo;
	int mTuru;//1->bireysel || 2->ticari
}Musteri;

typedef struct{
	Musteri musteri[MAX_MUSTERI];
	int mSayisi;
	int girisYapan;
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
	dIslem islem[MAX_ISLEM];
	int islemSayisi;
}Dekont;

Banka aBank;
Dekont dekont[MAX_DEKONT];
//mS = stractaki müsteri indisi, hS = stracttaki hesap indisi
void AnaMenu();//ana menü
void YeniMusteri();//yeni müsteri oluşturup müsteriye 1 hesap ekler
void MusteriIslem(int mS);//musteri işlem menüsü
void Guncelle();//structta depolanan verileri müsteri dosyaları ve rapor.txt ye yazar
void VeriAl();//program başlangıcında müsteri doyalarındaki verileri structta depolar
void bankaRapor(int mS);//rapor.txt yi ekrana yazdırır--mS işlemin yapıldığı müsteriye geri dönebilmek için
void hesapIslem(int mS, int hS);//Hesap işlemleri
void paraCek(int mS, int hS);//para çekme işlemleri
void paraYatir(int mS, int hS);//para yatirma işlemleri
void havaleGonder(int mS, int hS);//havale işlemleri
void hHesapKayit(int mS, int hS, int hNo);//hNo -1 ise hesap no sorar ve kayitli hesaplara kaydeder veya hNo yu kayitli hesaplara kaydeder
void hesapAc(int mS);//müsteriye yeni hesap açar
void hesapSil(int mS, int s);//s'in degerine göre kayitli veya normal hesap siler önce silinecek hesabı seçtirir
void hesapOzeti(int mS, int hS);//aylık olarak işlem geçmişini gruplar seçilen tarihteki hesap özetini dekont.txt ye yazdırır ve ekranda gosterir
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);//işlem kaydı yapar
int hesapSec(int mS, int hS, int s);//hesap seçme menüsü seçilen hesap noyu döndürür (s'in degerine göre normal hesap veya kayitli hesap)
int NoOlustur(int n);//hesap numaralarını karşılaştırıp random ve farklı bir hesap numarası döndürür
int tcNoKontrol(double tcNo);//tc no kontrolü tc no varsa müsteri indisini yoksa -1 döndürür
int mNoKontrol(int musteriNo);
int hNoKontrol(int hesapNo, int n); //n 1 ise müsteri indisi 2 ise hesap indisi dondürür hesap numarasi yoksa -1 döndürür
char *sifrele(char sifre[120]);//müsteri sifresini şifreler 8 haneli karakter dizisi döndürür
void strAl(char str[], int min, int max);//input alır boşlukları '-' ye çevirir min dan kısa veya max dan uzun karakter girilirse hata verir tekrar input ister
double cekilenPara(int mS);//müsterinin günlük çektiği parayı hesaplar ve döndürür
void isimDuzelt(char ad[120]);//isimlerdeki '-' leri boşluğa çevirir

int main(){
	aBank.girisYapan = -1;
	AnaMenu();
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
			strcpy((aBank.musteri+mNo-1)->Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &(aBank.musteri+mNo-1)->tcNo);
			fscanf(pf, " Ad Soyad: %s", (aBank.musteri+mNo-1)->Ad);
			fscanf(pf, " Musteri No: %d", &(aBank.musteri+mNo-1)->musteriNo);
			fscanf(pf, " Hesap Sayisi: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
			for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
				fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
				fscanf(pf, " Islem Sayisi: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
				if (((aBank.musteri+mNo-1)->hesap+i)->islemSayisi!=0){
					for (j=0; j<((aBank.musteri+mNo-1)->hesap+i)->islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Gun,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Ay,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Yil,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Saat,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Dk,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iTutar);
						fscanf(pf, " %d - %d", 
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iTuru,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
			if ((aBank.musteri+mNo-1)->tHesapSayisi != 0){
				for (i=0; i<(aBank.musteri+mNo-1)->tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &((aBank.musteri+mNo-1)->tHesap+i)->hesapNo);
				}
			}
			(aBank.musteri+mNo-1)->mTuru = 1;
			aBank.mSayisi++;
		}
		fclose(pf);
	}
	pf = fopen("ticariMusteri.txt", "r");
	if (pf!=NULL){
		while(!feof(pf)){
			fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
			strcpy((aBank.musteri+mNo-1)->Sifre, temp);
			fscanf(pf, " Tc-No: %lf", &(aBank.musteri+mNo-1)->tcNo);
			fscanf(pf, " Ad Soyad: %s", (aBank.musteri+mNo-1)->Ad);
			fscanf(pf, " Musteri No: %d", &(aBank.musteri+mNo-1)->musteriNo);
			fscanf(pf, " Hesap Sayisi: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
			for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
				fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
				fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
				fscanf(pf, " Islem Sayisi: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
				if (((aBank.musteri+mNo-1)->hesap+i)->islemSayisi!=0){
					for (j=0; j<((aBank.musteri+mNo-1)->hesap+i)->islemSayisi; j++){
						fscanf(pf, " %d.%d.%d - %d:%d / %lf", 
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Gun,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Ay,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Yil,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Saat,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->tarih.Dk,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iTutar);
						fscanf(pf, " %d - %d", 
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iTuru,
							&(((aBank.musteri+mNo-1)->hesap+i)->islem+j)->iHesap);
					}
				}
			}
			fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
			if ((aBank.musteri+mNo-1)->tHesapSayisi != 0){
				for (i=0; i<(aBank.musteri+mNo-1)->tHesapSayisi; i++){
					fscanf(pf, " Hesap %*d: %d", &((aBank.musteri+mNo-1)->tHesap+i)->hesapNo);
				}
			}
			(aBank.musteri+mNo-1)->mTuru = 2;
			aBank.mSayisi++;
		}
		fclose(pf);
	}

	for (i=0; i<aBank.mSayisi; i++){
		(aBank.musteri+i)->tBakiye = 0;
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			(aBank.musteri+i)->tBakiye += ((aBank.musteri+i)->hesap+j)->bakiye;
		}
	}
}

void Guncelle(){
	int i, j, k, iTuru, b=0, t=0;
	double gelen=0, giden=0, kar=0, kesinti;
	FILE *pf1, *pf2;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->mTuru == 1){
			if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));
			pf1 = fopen("bireyselMusteri.txt", "a");
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Musteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf1, "\n\tTc-No: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf1, "\n\tAd Soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf1, "\n\tMusteri No: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf1, "\n\tHesap Sayisi: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf1, "\n\t\tIslem Sayisi: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
				for (k=0; k<((aBank.musteri+i)->hesap+j)->islemSayisi; k++){
					fprintf(pf1, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Gun,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Ay,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Yil,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Saat,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Dk,
						(((aBank.musteri+i)->hesap+j)->islem+k)->iTutar);
					fprintf(pf1, "\n\t\t\t\t%d - %d", 
						(((aBank.musteri+i)->hesap+j)->islem+k)->iTuru,
						(((aBank.musteri+i)->hesap+j)->islem+k)->iHesap);
				}
			}
			fprintf(pf1, "\n\tKayitli Havale Hesap Sayisi: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			fclose(pf1);
			b=1;
		}else if ((aBank.musteri+i)->mTuru == 2){
			if (t==0) fclose(fopen("ticariMusteri.txt", "w"));
			pf2 = fopen("ticariMusteri.txt", "a");
			if (t!=0) fprintf(pf2, "\n");
			fprintf(pf2, "Musteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf2, "\n\tTc-No: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf2, "\n\tAd Soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf1, "\n\tMusteri No: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf2, "\n\tHesap Sayisi: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf2, "\n\t\tIslem Sayisi: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
				for (k=0; k<((aBank.musteri+i)->hesap+j)->islemSayisi; k++){
					fprintf(pf2, "\n\t\t\t%d.%d.%d - %d:%d / %.2lf", 
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Gun,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Ay,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Yil,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Saat,
						(((aBank.musteri+i)->hesap+j)->islem+k)->tarih.Dk,
						(((aBank.musteri+i)->hesap+j)->islem+k)->iTutar);
					fprintf(pf2, "\n\t\t\t\t%d - %d", 
						(((aBank.musteri+i)->hesap+j)->islem+k)->iTuru,
						(((aBank.musteri+i)->hesap+j)->islem+k)->iHesap);
				}
			}
			fprintf(pf2, "\n\tKayitli Havale Hesap Sayisi: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			fclose(pf2);
			t=1;
		}
	}

	for (i=0; i<aBank.mSayisi; i++){
		(aBank.musteri+i)->tBakiye = 0;
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			(aBank.musteri+i)->tBakiye += ((aBank.musteri+i)->hesap+j)->bakiye;
		}
	}
	
	for (i=0; i<aBank.mSayisi; i++){
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			for (k=0; k<((aBank.musteri+i)->hesap+j)->islemSayisi; k++){
				iTuru = (((aBank.musteri+i)->hesap+j)->islem+k)->iTuru;
				if (iTuru == 1) giden += (((aBank.musteri+i)->hesap+j)->islem+k)->iTutar;
				else if (iTuru == 2) gelen += (((aBank.musteri+i)->hesap+j)->islem+k)->iTutar;
				else if (iTuru == 3 && (aBank.musteri+i)->mTuru == 1){
					if ((((aBank.musteri+i)->hesap+j)->islem+k)->iTutar < 0){
						kesinti = ((int)(-(((aBank.musteri+i)->hesap+j)->islem+k)->iTutar*2)) / 100.0;
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
	if (aBank.girisYapan == -1){
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
		}else if(sorgu == 2 && aBank.mSayisi==0){
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
			if (aBank.girisYapan == -1){
				YeniMusteri();
			}else{
				MusteriIslem(aBank.girisYapan);
			}
		}break;
		case 2:{
			if (aBank.girisYapan == -1){
				MusteriIslem(aBank.girisYapan);
			}else{
				aBank.girisYapan = -1;
				AnaMenu();
			}
		}break;
	}
}

void YeniMusteri(){
	int sorgu, kontrol, is1, is2, i;
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
			(aBank.musteri+aBank.mSayisi)->mTuru = sorgu;
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Ad Soyad: ");
	do{
		strAl(temp, 5, 120);
		kontrol=1;
		for (i=0; i<(int)strlen(temp); i++){
			if (!((*(temp+i) >= 'a' && *(temp+i) <= 'z') || (*(temp+i) >= 'A' && *(temp+i) <= 'Z') || *(temp+i) == '-')) kontrol=0;
		}
		if (kontrol!=1) printf("Hatali Giris!\nTekrar Deneyiniz: ");
	}while(kontrol!=1);
	strcpy((aBank.musteri+aBank.mSayisi)->Ad, temp);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc Numaranizi Giriniz: ");
	do{
		strAl(temp, 11, 11);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (kontrol!=1 || *temp=='0') printf("Hatali Giris!\nTekrar Deneyiniz: ");
		else if (tcNoKontrol(dTemp) != -1) printf("Bu Tc Numarasi Kayitli!\nTekrar Deneyiniz: ");
	}while(!(tcNoKontrol(dTemp) == -1 && kontrol == 1 && *temp != '0'));
	(aBank.musteri+aBank.mSayisi)->tcNo = dTemp;

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
	strcpy((aBank.musteri+aBank.mSayisi)->Sifre, sifrele(s1));

	(aBank.musteri+aBank.mSayisi)->hesapSayisi = 1;
	(aBank.musteri+aBank.mSayisi)->tHesapSayisi = 0;
	(aBank.musteri+aBank.mSayisi)->musteriNo = NoOlustur(1);
	((aBank.musteri+aBank.mSayisi)->hesap)->hesapNo = NoOlustur(2);
	((aBank.musteri+aBank.mSayisi)->hesap)->bakiye = 0;
	((aBank.musteri+aBank.mSayisi)->hesap)->islemSayisi = 0;
	aBank.girisYapan = aBank.mSayisi;
	aBank.mSayisi++;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\nHesabiniz kuruldu...\n\n");
	strcpy(temp, (aBank.musteri+aBank.mSayisi-1)->Ad);
	isimDuzelt(temp);
	printf("Adiniz: %s\n", temp);
	printf("Tc Numaraniz: %.lf\n", (aBank.musteri+aBank.mSayisi-1)->tcNo);
	printf("Musteri Numaraniz: %d\n", (aBank.musteri+aBank.mSayisi-1)->musteriNo);
	printf("Hesap Numaraniz: %d\n", ((aBank.musteri+aBank.mSayisi-1)->hesap)->hesapNo);
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
			MusteriIslem(aBank.girisYapan);
		}break;
		case 2:{
			AnaMenu();
		}break;
	}
}

void MusteriIslem(int mS){
	int sonuc, sorgu, kontrol, t, iTemp;
	char temp[120], c;
	double dTemp;
	if (mS == -1){
		char sifre[120];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc No veya Musteri No Giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 1, 11);
			t = strlen(temp);
			if(t==1 && *temp == '0'){
				AnaMenu();
			}else if(t==9){
				kontrol = sscanf(temp, "%d%c", &iTemp, &c);
				sonuc = mNoKontrol(iTemp);
			}else if(t==11){
				kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
				sonuc = tcNoKontrol(dTemp);
			}else kontrol = 0;
			if (!(sonuc != -1 && kontrol == 1 && *temp != '0')) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}while(!(sonuc != -1 && kontrol == 1 && *temp != '0'));
		mS=sonuc;
		t=0;
		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (t!=0) printf("Hatali sifre!\n");
			printf("Sifrenizi Giriniz: ");
			strAl(sifre, 4, 8);
			t = strcmp((aBank.musteri+mS)->Sifre, sifrele(sifre));
		}while(t!=0);
		aBank.girisYapan = mS;
		AnaMenu();
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	strcpy(temp, (aBank.musteri+mS)->Ad);
	isimDuzelt(temp);
	printf("Hosgeldin, %s\n", temp);
	printf("Musteri Numaraniz: %d\n", (aBank.musteri+mS)->musteriNo);
	printf("Toplam Bakiyeniz: %.2lf\n\n", (aBank.musteri+mS)->tBakiye);
	printf("1-)\tHesap Sec\n2-)\tHesap Ac\n3-)\tHesap Sil\n4-)\tHavale Hesabi Kaydet\n5-)\tHavale Hesabi Sil\n");
	printf("6-)\tRapor Al\n7-)\tAna Menu\n0-)\tCikis\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>7 || kontrol != 1) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if (sorgu == 3 && (aBank.musteri+mS)->hesapSayisi == 1){
			printf("Son Hesabinizi silemezsiniz!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 5 && (aBank.musteri+mS)->tHesapSayisi == 0){
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
			hHesapKayit(mS, -1, -1);
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
	printf("Hesabinizdaki bakiyeniz: %.2lf TL'dir.\n\n", ((aBank.musteri+mS)->hesap+hS)->bakiye);
	printf("1-)\tPara Cekme\n2-)\tPara Yatirma\n3-)\tHavale\n4-)\tHesap Ozeti\n5-)\tGeri Don\n6-)\tAna Menu\n0-)\tCikis\nSecim: ");
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>6 ||kontrol != 1) printf("Hatali giris!\nTekrar Deneyiniz: ");
		else if ((sorgu == 1 || sorgu == 3) && (aBank.musteri+mS)->tBakiye == 0){
			printf("Bu islem icin hesaplarinizda yeterli para yok!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 1 && limit == 0){
			printf("Gunluk para cekme limitinize ulastiniz!\nTekrar Deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 4 && ((aBank.musteri+mS)->hesap+hS)->islemSayisi==0){
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
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Cekmek istediginiz tutari giriniz(Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 4);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1) hesapIslem(mS, hS);
		if (!(dTemp>0 && kontrol == 1 && *temp != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
		else if (dTemp>(aBank.musteri+mS)->tBakiye) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
		else if (dTemp>limit) printf("Para cekme limitiniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", limit);
	}while(!(dTemp>0 && dTemp<=(aBank.musteri+mS)->tBakiye && dTemp<=limit && kontrol==1 && *temp != '0'));
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
		((aBank.musteri+mS)->hesap+hS)->bakiye -= dTemp;
		islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -dTemp);
		printf("%d Nolu hesabinizdan %.2lf TL kesildi\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
		dTemp = 0;
	}else{
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarinizi kullanarak islemi tamamla\n2-)\tIslemi iptal et\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<1 || sorgu>2 || kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);
		else{
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
			}
			for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){
				if (((aBank.musteri+mS)->hesap+i)->bakiye != 0){
					if (dTemp<=((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						((aBank.musteri+mS)->hesap+i)->bakiye -= dTemp;
						islemKaydi(mS, i, 1, ((aBank.musteri+mS)->hesap+i)->hesapNo, -dTemp);
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);
						dTemp = 0;
					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						dTemp -= ((aBank.musteri+mS)->hesap+i)->bakiye;
						islemKaydi(mS, i, 1, ((aBank.musteri+mS)->hesap+i)->hesapNo, -((aBank.musteri+mS)->hesap+i)->bakiye);
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
						((aBank.musteri+mS)->hesap+i)->bakiye = 0;
					}
					if (dTemp==0) printf("\n");
				}
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
		else if (!(dTemp>0 && kontrol == 1 && *temp != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
	}while(!(dTemp>0 && kontrol == 1 && *temp != '0'));
	dTemp = ((int)(dTemp*100)) / 100.0;

	((aBank.musteri+mS)->hesap+hS)->bakiye += dTemp;
	islemKaydi(mS, hS, 2, ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
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
	if ((aBank.musteri+mS)->tHesapSayisi != 0){
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
				tHesapNo = ((aBank.musteri+mS)->tHesap+tHesapS)->hesapNo;
				if (tHesapNo == ((aBank.musteri+mS)->hesap+hS)->hesapNo){
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
		if (!(dTemp>0 && kontrol == 1 && *temp != '0')) printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
		else if (dTemp>(aBank.musteri+mS)->tBakiye && kontrol == 1) printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
	}while(!(dTemp>0 && dTemp<=(aBank.musteri+mS)->tBakiye && kontrol==1 && *temp != '0'));
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
		((aBank.musteri+mS)->hesap+hS)->bakiye -= dTemp;
		islemKaydi(mS, hS, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -dTemp);
		kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
		printf("%d Nolu hesabinizdan %.2lf TL kesildi\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);

		((aBank.musteri+hmS)->hesap+hhS)->bakiye += (dTemp - kesinti);
		islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp - kesinti);
		dTemp = 0;
	}else{
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarinizi kullanarak islemi tamamla\n2-)\tIslemi iptal et\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}
		}while(sorgu<1 || sorgu>2 || kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);
		else{
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Islem basarili...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(((aBank.musteri+mS)->hesap+hS)->bakiye*2)) / 100.0 : 0;
				printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				
				((aBank.musteri+hmS)->hesap+hhS)->bakiye += (((aBank.musteri+mS)->hesap+hS)->bakiye - kesinti);
				islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye - kesinti);
				((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
			}
			for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){
				if (((aBank.musteri+mS)->hesap+i)->bakiye != 0){
					if (dTemp<=((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						((aBank.musteri+mS)->hesap+i)->bakiye -= dTemp;
						islemKaydi(mS, i, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -dTemp);
						kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);

						((aBank.musteri+hmS)->hesap+hhS)->bakiye += (dTemp - kesinti);
						islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp - kesinti);
						dTemp = 0;

					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						dTemp -= ((aBank.musteri+mS)->hesap+i)->bakiye;
						islemKaydi(mS, i, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -((aBank.musteri+mS)->hesap+i)->bakiye);
						kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(((aBank.musteri+mS)->hesap+i)->bakiye*2)) / 100.0 : 0;
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
						
						((aBank.musteri+hmS)->hesap+hhS)->bakiye += (((aBank.musteri+mS)->hesap+i)->bakiye - kesinti);
						islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+i)->hesapNo, ((aBank.musteri+mS)->hesap+i)->bakiye - kesinti);
						((aBank.musteri+mS)->hesap+i)->bakiye = 0;
					}
					if (dTemp==0) printf("\n");
				}
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
			hHesapKayit(mS, hS, tHesapNo);
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
}
void hHesapKayit(int mS, int hS, int hNo){
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
	((aBank.musteri+mS)->tHesap+(aBank.musteri+mS)->tHesapSayisi)->hesapNo = hNo;
	(aBank.musteri+mS)->tHesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri don\n2-)\tAna Menu\n0-)\tCikis\nSecim: ");
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
void hesapAc(int mS){
	int sorgu, kontrol, hS = (aBank.musteri+mS)->hesapSayisi;
	char temp[120], c;
	((aBank.musteri+mS)->hesap+hS)->hesapNo = NoOlustur(2);
	((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
	((aBank.musteri+mS)->hesap+hS)->islemSayisi = 0;
	(aBank.musteri+mS)->hesapSayisi++;
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
			hesapIslem(mS, (aBank.musteri+mS)->hesapSayisi-1);
		}break;
		case 3:{
			AnaMenu();
		}break;
	}
}
void hesapSil(int mS, int s){// s 1 ise normal hesap 2 ise kayitli hesap
	int i, n, sorgu, kontrol, shS;
	char temp[120], c;
	shS =  hesapSec(mS, -1, s);
	n = (s==1) ? (aBank.musteri+mS)->hesapSayisi : (aBank.musteri+mS)->tHesapSayisi;
	if (s==1 && ((aBank.musteri+mS)->hesap+shS)->bakiye!=0){
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
				(aBank.musteri+mS)->hesap[i] = (aBank.musteri+mS)->hesap[i+1]; 
			}else{
				(aBank.musteri+mS)->tHesap[i] = (aBank.musteri+mS)->tHesap[i+1]; 
			}
		}
		(s==1) ? (aBank.musteri+mS)->hesapSayisi-- : (aBank.musteri+mS)->tHesapSayisi--;
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
int hesapSec(int mS, int hS, int s){// s 1 ise normal hesap 2 ise kayitli hesap
	int i, sorgu, kontrol, hNo, n, tmS;
	char temp[120], c;
	if (s==1){
		n = (aBank.musteri+mS)->hesapSayisi;
		if (n==1) return 0;
	}else n = (aBank.musteri+mS)->tHesapSayisi;
	if(n==0) return -1;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem Yapilacak Hesap No Seciniz (Iptal etmek icin 0 giriniz): \n\n");
	for (i=0; i<n; i++){
		hNo = (s==1) ? ((aBank.musteri+mS)->hesap+i)->hesapNo : ((aBank.musteri+mS)->tHesap+i)->hesapNo;
		if (s==1){
			printf("%d-) %d (%.2lf TL)\n", i+1, hNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
		}else{
			tmS = hNoKontrol(hNo, 1);
			strcpy(temp, (aBank.musteri+tmS)->Ad);
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

int NoOlustur(int n){ //n 1 ise musterino 2 ise hesapno
	int No, kontrol;
	srand(time(NULL)+n);
	do{
		No = (rand()%900+100)*1000000 + (rand()%1000)*1000 + rand()%1000;//windowsta düzgün çalışsın diye
		kontrol = (n==1)?mNoKontrol(No):hNoKontrol(No, 1);
	}while(kontrol!=-1);
	return No;
}
char *sifrele(char sifre[120]){
	int i, temp[8]={}, len = strlen(sifre);
	for (i=0; i<8; i++){
		*(temp+i) += ((i+1) * (*(sifre+(i%len)) + 11) + *(sifre+len-(i%len)-1) + i);
		*(sifre+i) = *(temp+i)%93 + 33;
	}
	*(sifre+8) = '\0';
	return sifre;
}

int tcNoKontrol(double tcNo){
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->tcNo == tcNo) return i;
	}
	return -1;
}
int mNoKontrol(int musteriNo){
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->musteriNo == musteriNo) return i;
	}
	return -1;
}
int hNoKontrol(int hesapNo, int n){//n 1 ise müsteri sırası 2 ise hesap sırası
	int mS, hS;
	for (mS=0; mS<aBank.mSayisi; mS++){
		for (hS=0; hS<(aBank.musteri+mS)->hesapSayisi; hS++){
			if (((aBank.musteri+mS)->hesap+hS)->hesapNo == hesapNo) return (n==1)?mS:hS;
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
		if (t==1 && *str == '0') break;
		if (t>max || t<min) printf("Hatali giris!\nTekrar deneyiniz: ");
	}while (!(t<=max && t>=min));
}
void isimDuzelt(char ad[120]){
	int i, t;
	t = strlen(ad);
	for (i=0; i<t; i++){
		if (*(ad+i) == '-') *(ad+i) = ' ';
	}
}
void hesapOzeti(int mS, int hS){
	FILE *pf;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int i, j, k, n, t1, t2, t3, iS, aralik, ihNo, imS, iA, iY, sorgu, kontrol;
	double kesinti;
	char c, temp[120];
	iS = ((aBank.musteri+mS)->hesap+hS)->islemSayisi;
	t1 = ((((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Yil - 1900) * 12 + (((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Ay;
	t2 = tm.tm_year * 12 + tm.tm_mon+1 ;
	aralik = t2-t1;
	int index[aralik+1];
	
	system("@cls||clear");
	printf(".............aBank.............\n");
	for (i=0; i<aralik+1; i++){
		*(index+i) = -1;
		(dekont+i)->islemSayisi = 0;
		((dekont+i)->islem)->Tutar = 0;
		for (j=iS-1; j>=0; j--){
			t3 = ((((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih.Yil - 1900) * 12 + (((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih.Ay;
			if ((t2-t3) == i){
				ihNo = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iHesap;
				imS = hNoKontrol(ihNo, 1);
				strcpy(((dekont+i)->islem+(dekont+i)->islemSayisi)->Ad, (aBank.musteri+imS)->Ad);
				isimDuzelt(((dekont+i)->islem+(dekont+i)->islemSayisi)->Ad);
				((dekont+i)->islem+(dekont+i)->islemSayisi)->hesapNo = ihNo;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->mTuru = (aBank.musteri+mS)->mTuru;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->iTuru = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iTuru;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->Tutar = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iTutar;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->tarih = (((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih;
				(dekont+i)->islemSayisi++;
			}
		}
	}
	k=0;
	for (i=0; i<aralik+1; i++){
		if (((dekont+i)->islem)->Tutar != 0){
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
	fprintf(pf, "  (%d)\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo);
	for (j=(dekont+i)->islemSayisi-1; j>=0; j--){
		if (((dekont+i)->islem+j)->iTuru == 1){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para Cekme    %.2lf\n", 
				((dekont+i)->islem+j)->tarih.Gun,
				((dekont+i)->islem+j)->tarih.Ay,
				((dekont+i)->islem+j)->tarih.Yil,
				((dekont+i)->islem+j)->tarih.Saat,
				((dekont+i)->islem+j)->tarih.Dk,
				-((dekont+i)->islem+j)->Tutar);
		}else if (((dekont+i)->islem+j)->iTuru == 2){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para Yatirma  %.2lf\n", 
				((dekont+i)->islem+j)->tarih.Gun,
				((dekont+i)->islem+j)->tarih.Ay,
				((dekont+i)->islem+j)->tarih.Yil,
				((dekont+i)->islem+j)->tarih.Saat,
				((dekont+i)->islem+j)->tarih.Dk,
				((dekont+i)->islem+j)->Tutar);
		}else if (((dekont+i)->islem+j)->iTuru == 3){
			if (((dekont+i)->islem+j)->Tutar<0){
				if (((dekont+i)->islem+j)->mTuru == 1){
					kesinti = ((int)(-((dekont+i)->islem+j)->Tutar*2)) / 100.0;
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden Havale  %.2lf (-%.2lf)\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar,
						kesinti);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}else{
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden Havale  %.2lf\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}
			}else if (((dekont+i)->islem+j)->Tutar>0){
				fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Gelen Havale  %.2lf\n", 
					((dekont+i)->islem+j)->tarih.Gun,
					((dekont+i)->islem+j)->tarih.Ay,
					((dekont+i)->islem+j)->tarih.Yil,
					((dekont+i)->islem+j)->tarih.Saat,
					((dekont+i)->islem+j)->tarih.Dk,
					((dekont+i)->islem+j)->Tutar);
				fprintf(pf, "\tGonderen kisi:    %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
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
	int iS = ((aBank.musteri+mS)->hesap+hS)->islemSayisi;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->tarih.Yil = tm.tm_year + 1900;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->tarih.Ay = tm.tm_mon + 1;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->tarih.Gun = tm.tm_mday;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->tarih.Saat = tm.tm_hour;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->tarih.Dk = tm.tm_min;

	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->iTuru = iT;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->iTutar = iTutar;
	(((aBank.musteri+mS)->hesap+hS)->islem+iS)->iHesap = iH;
	
	((aBank.musteri+mS)->hesap+hS)->islemSayisi++;
}

double cekilenPara(int mS){
	int i, j, iYil, iAy, iGun, Yil, Ay, Gun;
	double limit = 0;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	Yil = tm.tm_year + 1900;
	Ay = tm.tm_mon + 1;
	Gun = tm.tm_mday;
	for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){
		for (j=0; j<((aBank.musteri+mS)->hesap+i)->islemSayisi; j++){
			iYil = (((aBank.musteri+mS)->hesap+i)->islem+j)->tarih.Yil;
			iAy = (((aBank.musteri+mS)->hesap+i)->islem+j)->tarih.Ay;
			iGun = (((aBank.musteri+mS)->hesap+i)->islem+j)->tarih.Gun;
			if (Yil == iYil && Ay == iAy && Gun == iGun && (((aBank.musteri+mS)->hesap+i)->islem+j)->iTuru == 1){
				limit -= (((aBank.musteri+mS)->hesap+i)->islem+j)->iTutar;
			}
		}
	}
	return limit;
}