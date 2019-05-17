#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

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
	double tGelen;
	double tGiden;
	double tKar;
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

char temp[120];
//mS = stractaki müşteri indisi, hS = stracttaki hesap indisi
void AnaMenu();//ana menü
void YeniMusteri();//yeni müşteri oluşturup müşteriye 1 hesap ekler
void MusteriIslem(int mS);//musteri işlem menüsü
void Guncelle();//structta depolanan verileri müşteri dosyaları ve rapor.txt ye yazar
void VeriAl();//program başlangıcında müşteri doyalarındaki verileri structta depolar
void bankaRapor(int mS);//rapor.txt yi ekrana yazdırır--mS işlemin yapıldığı müşteriye geri dönebilmek için
void hesapIslem(int mS, int hS);//Hesap işlemleri
void paraCek(int mS, int hS);//para çekme işlemleri
void paraYatir(int mS, int hS);//para yatirma işlemleri
void havaleGonder(int mS, int hS);//havale işlemleri
void hHesapKayit(int mS, int hS, int hNo);//hNo -1 ise hesap no sorar ve kayitli hesaplara kaydeder veya hNo yu kayitli hesaplara kaydeder
void hesapAc(int mS);//müşteriye yeni hesap açar
void hesapSil(int mS, int s);//s'in degerine göre kayitli veya normal hesap siler önce silinecek hesabı seçtirir
void hesapOzeti(int mS, int hS);//aylık olarak işlem geçmişini gruplar seçilen tarihteki hesap özetini dekont.txt ye yazdırır ve ekranda gosterir
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);//işlem kaydı yapar
int hesapSec(int mS, int hS, int s);//hesap seçme menüsü seçilen hesap noyu döndürür (s'in degerine göre normal hesap veya kayitli hesap)
int NoOlustur(int n);//hesap numaralarını karşılaştırıp random ve farklı bir hesap numarası döndürür
int tcNoKontrol(double tcNo);//tc no kontrolü tc no varsa müşteri indisini yoksa -1 döndürür
int mNoKontrol(int musteriNo);//musteri no kontrolu yoksa -1 varsa indisini döndürür
int hNoKontrol(int hesapNo, int n); //n 1 ise müşteri indisi 2 ise hesap indisi dondürür hesap numarasi yoksa -1 döndürür
int thKontrol(int mS, int hNo);//transfer hesap kontrolü yoksa -1 varsa indisini döndürür
char *sifrele(char sifre[120]);//müşteri sifresini şifreler 8 haneli karakter dizisi döndürür
void strAl(char str[], int min, int max);//input alır boşlukları '-' ye çevirir min dan kısa veya max dan uzun karakter girilirse hata verir tekrar input ister
double cekilenPara(int mS);//müşterinin günlük çektiği parayı hesaplar ve döndürür
void isimDuzelt(char ad[120]);//isimlerdeki '-' leri boşluğa çevirir

int main(){
	setlocale(LC_ALL, "Turkish");
	VeriAl();
	aBank.girisYapan = -1;
	AnaMenu();
}

void VeriAl(){
	int i, j, mNo;
	FILE *pf;
	for (i=0; i<5; i++){
		pf = fopen("bireyselMusteri.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf!=NULL){
		fseek(pf, 0, SEEK_END);
		if (ftell(pf) != 0){
			fseek(pf, 0, SEEK_SET);
			while(!feof(pf)){
				fscanf(pf, " Müşteri: %d / [ %s ]", &mNo, temp);
				strcpy((aBank.musteri+mNo-1)->Sifre, temp);
				fscanf(pf, " Tc no: %lf", &(aBank.musteri+mNo-1)->tcNo);
				fscanf(pf, " Ad soyad: %s", (aBank.musteri+mNo-1)->Ad);
				fscanf(pf, " Müşteri no: %d", &(aBank.musteri+mNo-1)->musteriNo);
				fscanf(pf, " Hesap sayısı: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
				for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
					fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
					fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
					fscanf(pf, " İşlem sayısı: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
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
				fscanf(pf, " Kayıtlı havale hesap sayısı: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
				if ((aBank.musteri+mNo-1)->tHesapSayisi != 0){
					for (i=0; i<(aBank.musteri+mNo-1)->tHesapSayisi; i++){
						fscanf(pf, " Hesap %*d: %d", &((aBank.musteri+mNo-1)->tHesap+i)->hesapNo);
					}
				}
				(aBank.musteri+mNo-1)->mTuru = 1;
				aBank.mSayisi++;
			}
		}
		fclose(pf);
	}
	for (i=0; i<5; i++){
		pf = fopen("ticariMusteri.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf!=NULL){
		fseek(pf, 0, SEEK_END);
		if (ftell(pf) != 0){
			fseek(pf, 0, SEEK_SET);
			while(!feof(pf)){
				fscanf(pf, " Müşteri: %d / [ %s ]", &mNo, temp);
				strcpy((aBank.musteri+mNo-1)->Sifre, temp);
				fscanf(pf, " Tc no: %lf", &(aBank.musteri+mNo-1)->tcNo);
				fscanf(pf, " Ad soyad: %s", (aBank.musteri+mNo-1)->Ad);
				fscanf(pf, " Müşteri no: %d", &(aBank.musteri+mNo-1)->musteriNo);
				fscanf(pf, " Hesap sayısı: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
				for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
					fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
					fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
					fscanf(pf, " İşlem sayısı: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
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
				fscanf(pf, " Kayıtlı havale hesap sayısı: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
				if ((aBank.musteri+mNo-1)->tHesapSayisi != 0){
					for (i=0; i<(aBank.musteri+mNo-1)->tHesapSayisi; i++){
						fscanf(pf, " Hesap %*d: %d", &((aBank.musteri+mNo-1)->tHesap+i)->hesapNo);
					}
				}
				(aBank.musteri+mNo-1)->mTuru = 2;
				aBank.mSayisi++;
			}
		}
		fclose(pf);
	}
	for (i=0; i<5; i++){
		pf = fopen("rapor.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf != NULL){
		fscanf(pf, " aBank gelir-gider raporu...");
		fscanf(pf, " ");
		fscanf(pf, " Bankada bulunan toplam para: %*f");
		fscanf(pf, " Gelen toplam para: %lf", &aBank.tGelen);
		fscanf(pf, " Giden toplam para: %lf", &aBank.tGiden);
		fscanf(pf, " Bankanın karı: %lf", &aBank.tKar);
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
	int i, j, k, b=0, t=0;
	FILE *pf1, *pf2;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->mTuru == 1){
			if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));
			if ((pf1 = fopen("bireyselMusteri.txt", "a")) == NULL){
				printf("Dosya acma hatası!\n");
				exit(1);
			}
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Müşteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf1, "\n\tTc no: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf1, "\n\tAd soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf1, "\n\tMüşteri no: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf1, "\n\tHesap sayısı: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf1, "\n\t\tİşlem sayısı: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
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
			fprintf(pf1, "\n\tKayıtlı havale hesap sayısı: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			fclose(pf1);
			b=1;
		}else if ((aBank.musteri+i)->mTuru == 2){
			if (t==0) fclose(fopen("ticariMusteri.txt", "w"));
			if ((pf2 = fopen("ticariMusteri.txt", "a")) == NULL){
				printf("Dosya acma hatası!\n");
				exit(1);
			}
			if (t!=0) fprintf(pf2, "\n");
			fprintf(pf2, "Müşteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf2, "\n\tTc no: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf2, "\n\tAd soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf1, "\n\tMüşteri no: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf2, "\n\tHesap sayısı: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf2, "\n\t\tİşlem sayısı: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
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
			fprintf(pf2, "\n\tKayıtlı havale hesap sayısı: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			fclose(pf2);
			t=1;
		}
	}


	fclose(fopen("rapor.txt", "w"));
	if ((pf1 = fopen("rapor.txt", "a")) == NULL){
		printf("Dosya acma hatası!\n");
		exit(1);
	}
	fprintf(pf1, "aBank gelir-gider raporu...\n\n");
	fprintf(pf1, "Bankada bulunan toplam para: %.2lf\n", aBank.tGelen - aBank.tGiden + aBank.tKar);
	fprintf(pf1, "Gelen toplam para:           %.2lf\n", aBank.tGelen);
	fprintf(pf1, "Giden toplam para:           %.2lf\n", aBank.tGiden);
	fprintf(pf1, "Bankanin karı:               %.2lf", aBank.tKar);
	fclose(pf1);

	for (i=0; i<aBank.mSayisi; i++){
		(aBank.musteri+i)->tBakiye = 0;
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			(aBank.musteri+i)->tBakiye += ((aBank.musteri+i)->hesap+j)->bakiye;
		}
	}
}

void AnaMenu(){
	int sorgu, kontrol;
	char c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (aBank.girisYapan == -1){
		printf("1-)\tYeni müşteri kaydı\n");
		printf("2-)\tMüşteri girişi\n");
	}else{
		printf("1-)\tMüşteri işlemleri\n");
		printf("2-)\tMüşteri çıkışı\n");
	}
	printf("0-)\tÇıkış\n");
	printf("Seçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 2 && aBank.mSayisi==0){
			printf("Herhangi bir müşteri yok henüz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Çıkış başarılı\n\n");
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
	int sorgu, kontrol, is1, is2, i, j, k, t;
	double dTemp;
	char s1[120]={}, s2[120]={}, c;
	char alfabe[] = "abcçdefgğhıijklmnoöprsştuüvyzABCÇDEFGĞHIİJKLMNOÖPRSŞTUÜVYZ-";

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("1-)\tBireysel müşteri\n2-)\tTicari müşteri\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:
		case 2:{
			(aBank.musteri+aBank.mSayisi)->mTuru = sorgu;
		}break;
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Ad Soyad (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 5, 120);
		kontrol=1;
		t = strlen(temp);
		k=0;
        for (i=0; i<t && kontrol==1; i++){
            for (j=0; j<(int)strlen(alfabe); j++){
                if(*(temp+i) == *(alfabe+j)) {
                    kontrol = 1;
                    break;
                }else {
                    kontrol = 0;
                    continue;
                }
            }
			if (i!=0 && *(temp+i-1) == '-' && *(temp+i) == '-') kontrol=0;
			if (*(temp+i) == '-') k++;
        }
		if (k == 0) kontrol=0;
		if (t == 1 && *temp == '0') YeniMusteri();
		if (kontrol != 1) printf("Hatalı giriş!\nTekrar deneyiniz: ");
	}while(kontrol != 1);
	strcpy((aBank.musteri+aBank.mSayisi)->Ad, temp);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc numaranızı giriniz (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 11, 11);
		t = strlen(temp);
		if (t == 1 && *temp == '0') YeniMusteri();
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (kontrol != 1 || *temp=='0'){
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (tcNoKontrol(dTemp) != -1){
			printf("Bu tc numarası kayıtlı!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	(aBank.musteri+aBank.mSayisi)->tcNo = dTemp;

	system("@cls||clear");
	printf(".............aBank.............\n");
	do{
		printf("Şifrenizi giriniz (Min 4, max 8 hane. İptal etmek için 0 giriniz): ");
		strAl(s1, 1, 30);
		t = strlen(s1);
		if (t == 1 && *s1 == '0') YeniMusteri();
		sscanf(s1, "%d%c", &is1, &c);
		printf("Tekrar giriniz: ");
		strAl(s2, 1, 30);
		t = strlen(s2);
		if (t == 1 && *s2 == '0') YeniMusteri();
		kontrol = sscanf(s2, "%d%c", &is2, &c);
		if (!(strcmp(s1, s2) == 0 && is1>=0 && is1<=99999999 && kontrol == 1)){
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hatalı giriş!\n");
			kontrol = 0;
		}
	}while(kontrol != 1);
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
	printf(".............aBank.............\nHesabınız kuruldu...\n\n");
	strcpy(temp, (aBank.musteri+aBank.mSayisi-1)->Ad);
	isimDuzelt(temp);
	printf("Adınız: %s\n", temp);
	printf("Tc numaranız: %.lf\n", (aBank.musteri+aBank.mSayisi-1)->tcNo);
	printf("Müşteri numaranız: %d\n", (aBank.musteri+aBank.mSayisi-1)->musteriNo);
	printf("Hesap numaranız: %d\n", ((aBank.musteri+aBank.mSayisi-1)->hesap)->hesapNo);
	printf("Şifreniz: %s\n\n", s2);
	printf("1-)\tMüşteri işlemleri\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			MusteriIslem(aBank.girisYapan);
		}break;
	}
}

void MusteriIslem(int mS){
	int sonuc, sorgu, kontrol, t, s, iTemp;
	char c;
	double dTemp;
	if (mS == -1){
		char sifre[120];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc no veya müşteri no giriniz (İptal etmek için 0 giriniz): ");
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
			if (!(sonuc != -1 && kontrol == 1 && *temp != '0')){
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		mS=sonuc;
		s=0;
		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (s!=0) printf("Hatalı şifre!%d\n", t);
			printf("Şifrenizi giriniz (İptal etmek için 0 giriniz): ");
			strAl(sifre, 4, 8);
			t = strlen(sifre);
			if (t == 1 && *sifre == '0') AnaMenu();
			s = strcmp((aBank.musteri+mS)->Sifre, sifrele(sifre));
			
		}while(s!=0);
		aBank.girisYapan = mS;
		AnaMenu();
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	strcpy(temp, (aBank.musteri+mS)->Ad);
	isimDuzelt(temp);
	printf("Hoşgeldin, %s\n", temp);
	printf("Müşteri numaranız: %d\n", (aBank.musteri+mS)->musteriNo);
	printf("Toplam bakiyeniz: %.2lf\n\n", (aBank.musteri+mS)->tBakiye);
	printf("1-)\tHesap işlemleri\n2-)\tYeni hesap aç\n3-)\tHesap sil\n4-)\tHavale hesabı kaydet\n5-)\tHavale hesabı sil\n");
	printf("6-)\tRapor al\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>6 || kontrol != 1){
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 3 && (aBank.musteri+mS)->hesapSayisi == 1){
			printf("Son hesabınızı silemezsiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 5 && (aBank.musteri+mS)->tHesapSayisi == 0){
			printf("Kayitli hesabınız kalmamıştır!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
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
			hHesapKayit(mS, -1, -1);
		}break;
		case 5:{
			hesapSil(mS, 2);
		}break;
		case 6:{
			bankaRapor(mS);
		}break;
	}
}

void hesapIslem(int mS, int hS){
	char c;
	int sorgu, kontrol;
	double limit;
	if (hS == -1) hS = hesapSec(mS, -1, 1);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabınızdaki bakiyeniz: %.2lf TL'dir.\n\n", ((aBank.musteri+mS)->hesap+hS)->bakiye);
	printf("1-)\tPara çekme\n2-)\tPara yatırma\n3-)\tHavale\n4-)\tHesap özeti\n5-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>5 ||kontrol != 1){
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if ((sorgu == 1 || sorgu == 3) && (aBank.musteri+mS)->tBakiye == 0){
			printf("Bu işlem için hesaplarınızda yeterli para yok!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 1 && limit == 0){
			printf("Günlük para çekme limitinize ulaştınız!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 4 && ((aBank.musteri+mS)->hesap+hS)->islemSayisi==0){
			printf("Hesabınızda herhangi bir işlem yapılmadı henüz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
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
			hesapOzeti(mS, hS);
		}break;
		case 5:{
			MusteriIslem(mS);
		}break;
	}
}
void paraCek(int mS, int hS){
	int i, t, kontrol, sorgu;
	char c;
	double dTemp, limit;
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Çekmek istediğiniz tutarı giriniz (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 1, 10);
		t = strlen(temp);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (kontrol == 1) sorgu = (int)(dTemp*100);
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){
			printf("Hatalı giriş yaptınız!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu%500 != 0){
			printf("5 ve katları sadece!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (dTemp>(aBank.musteri+mS)->tBakiye){
			printf("Toplam bakiyeniz %.2lf TL'dir!\nFarklı bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
			kontrol = 0;
		}else if (dTemp>limit){
			printf("Para çekme limitiniz %.2lf TL'dir!\nFarklı bir miktar giriniz: ", limit);
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
		((aBank.musteri+mS)->hesap+hS)->bakiye -= dTemp;
		islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -dTemp);
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("İşlem başarılı...\n\n");
		printf("%d Nolu hesabınızdan %.2lf TL kesildi\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
		dTemp = 0;
	}else{
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarınızı kullanarak işlemi tamamla\n2-)\tİşlemi iptal et\nSeçim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);
		else{
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("İşlem başarılı...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				printf("%d Nolu hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
			}
			for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){
				if (((aBank.musteri+mS)->hesap+i)->bakiye != 0){
					if (dTemp<=((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						((aBank.musteri+mS)->hesap+i)->bakiye -= dTemp;
						islemKaydi(mS, i, 1, ((aBank.musteri+mS)->hesap+i)->hesapNo, -dTemp);
						printf("%d Nolu ek hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);
						dTemp = 0;
					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						dTemp -= ((aBank.musteri+mS)->hesap+i)->bakiye;
						islemKaydi(mS, i, 1, ((aBank.musteri+mS)->hesap+i)->hesapNo, -((aBank.musteri+mS)->hesap+i)->bakiye);
						printf("%d Nolu ek hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
						((aBank.musteri+mS)->hesap+i)->bakiye = 0;
					}
					if (dTemp==0) printf("\n");
				}
			}
		}
	}
	Guncelle();
	printf("1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hesapIslem(mS, hS);
		}break;
	}
}
void paraYatir(int mS, int hS){
	int kontrol, sorgu, t;
	char c;
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Yatırmak istediğiniz tutarı giriniz (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 1, 12);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		t = strlen(temp);
		if (kontrol == 1) sorgu = (int)(dTemp*100);
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || dTemp>=1000000 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){
			printf("Hatalı giriş yaptınız!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu%500 != 0){
			printf("5 ve katları sadece!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;

	((aBank.musteri+mS)->hesap+hS)->bakiye += dTemp;
	islemKaydi(mS, hS, 2, ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("İşlem başarılı...\n\n");
	printf("1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hesapIslem(mS, hS);
		}break;
	}
}
void havaleGonder(int mS, int hS){
	double dTemp, kesinti;
	int i, t, tHesapS, hhS, hmS, tHesapNo, sorgu, kontrol;
	char c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if ((aBank.musteri+mS)->tHesapSayisi != 0){
		printf("1-)\tKayıtlı havale hesabına gönder\n2-)\tHesap no girerek gönder\nSeçim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		switch (sorgu){
			case 1:{
				tHesapS = hesapSec(mS, hS, 2);
				tHesapNo = ((aBank.musteri+mS)->tHesap+tHesapS)->hesapNo;
				if (tHesapNo == ((aBank.musteri+mS)->hesap+hS)->hesapNo){
					system("@cls||clear");
					printf(".............aBank.............\n");
					printf("İşlem yaptığınız hesabı seçemezsiniz...\n\n");
					printf("1-)\tGeri dön\nSeçim: ");
					do{
						strAl(temp, 1, 1);
						kontrol = sscanf(temp, "%d%c", &sorgu, &c);
						if(sorgu<0 || sorgu>1 || kontrol != 1) {
							printf("Hatalı giriş!\nTekrar deneyiniz: ");
							kontrol = 0;
						}
					}while(kontrol != 1);
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
				printf("Hesap numarasını giriniz (İptal etmek için 0 giriniz): ");
				do{
					strAl(temp, 7, 7);
					t = strlen(temp);
					kontrol = sscanf(temp, "%d%c", &sorgu, &c);
					if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
					if (kontrol != 1) printf("Hatalı giriş!\nTekrar deneyiniz: ");
					else if (hNoKontrol(sorgu, 1) == -1){
						printf("Böyle bir hesap numarası yok!\nTekrar deneyiniz: ");
						kontrol = 0;
					}
				}while(kontrol != 1);
				tHesapNo = sorgu;
			}break;
		}
	}else{
		printf("Hesap numarasını giriniz (İptal etmek için 0 giriniz): ");
		do{
			strAl(temp, 7, 7);
			t = strlen(temp);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
			if (kontrol != 1) printf("Hatalı giriş!\nTekrar deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1){
				printf("Böyle bir hesap numarası yok!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		tHesapNo = sorgu;
	}

	hhS = hNoKontrol(tHesapNo, 2);
	hmS = hNoKontrol(tHesapNo, 1);

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Göndermek istediğiniz tutarı giriniz (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 1, 12);
		t = strlen(temp);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){
			printf("Hatalı giriş yaptınız!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (dTemp>(aBank.musteri+mS)->tBakiye && kontrol == 1){
			printf("Toplam bakiyeniz %.2lf TL'dir!\nFarklı bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("İşlem başarılı...\n\n");
		((aBank.musteri+mS)->hesap+hS)->bakiye -= dTemp;
		islemKaydi(mS, hS, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -dTemp);
		kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
		printf("%d Nolu hesabınızdan %.2lf TL kesildi\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);

		((aBank.musteri+hmS)->hesap+hhS)->bakiye += (dTemp - kesinti);
		islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp - kesinti);
		dTemp = 0;
	}else{
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarınızı kullanarak işlemi tamamla\n2-)\tİşlemi iptal et\nSeçim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);
		else{
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("İşlem başarılı...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(((aBank.musteri+mS)->hesap+hS)->bakiye*2)) / 100.0 : 0;
				printf("%d Nolu hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				
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
						printf("%d Nolu ek hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);

						((aBank.musteri+hmS)->hesap+hhS)->bakiye += (dTemp - kesinti);
						islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp - kesinti);
						dTemp = 0;

					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){
						dTemp -= ((aBank.musteri+mS)->hesap+i)->bakiye;
						islemKaydi(mS, i, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -((aBank.musteri+mS)->hesap+i)->bakiye);
						kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(((aBank.musteri+mS)->hesap+i)->bakiye*2)) / 100.0 : 0;
						printf("%d Nolu ek hesabınızdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
						
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
	printf("1-)\tHavale hesabı olarak kaydet\n2-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(thKontrol(mS, tHesapNo) != -1 && kontrol == 1 && sorgu == 1){
			printf("Bu hesap zaten ekli!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hHesapKayit(mS, hS, tHesapNo);
		}break;
		case 2:{
			hesapIslem(mS, hS);
		}break;
	}
}
void hHesapKayit(int mS, int hS, int hNo){
	int sorgu, kontrol, s=1, t;
	char c;
	if (hNo == -1){
		s=0;
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap numarasını giriniz (İptal etmek için 0 giriniz): ");
		do{
			strAl(temp, 7, 7);
			t = strlen(temp);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1 && t == 1) MusteriIslem(mS);
			if (kontrol != 1) printf("Hatalı giriş!\nTekrar deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1){
				printf("Böyle bir hesap numarası yok!\nTekrar deneyiniz: ");
				kontrol = 0;
			}else if (thKontrol(mS, sorgu) != -1){
				printf("Hesap numarası zaten kayıtlı!\nTekrar deneyiniz: ");
				kontrol = 0;
			} 
		}while(kontrol != 1);
		hNo = sorgu;
	}
	((aBank.musteri+mS)->tHesap+(aBank.musteri+mS)->tHesapSayisi)->hesapNo = hNo;
	(aBank.musteri+mS)->tHesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("İşlem başarılı...\n\n");
	printf("1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			if (s==1) hesapIslem(mS, hS);
			else MusteriIslem(mS);
		}break;
	}

}
void hesapAc(int mS){
	int sorgu, kontrol, hS = (aBank.musteri+mS)->hesapSayisi;
	char c;
	((aBank.musteri+mS)->hesap+hS)->hesapNo = NoOlustur(2);
	((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
	((aBank.musteri+mS)->hesap+hS)->islemSayisi = 0;
	(aBank.musteri+mS)->hesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabınız Acildi..\nHesap numaraniz: %d\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo);
	printf("1-)\tHesap işlemleri\n2-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hesapIslem(mS, (aBank.musteri+mS)->hesapSayisi-1);
		}break;
		case 2:{
			MusteriIslem(mS);
		}break;
	}
}
void hesapSil(int mS, int s){// s 1 ise normal hesap 2 ise kayitli hesap
	int i, n, sorgu, kontrol, shS;
	char c;
	shS =  hesapSec(mS, -1, s);
	n = (s==1) ? (aBank.musteri+mS)->hesapSayisi : (aBank.musteri+mS)->tHesapSayisi;
	if (s==1 && ((aBank.musteri+mS)->hesap+shS)->bakiye!=0){
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesabınızda para varken hesabınızı silemezsiniz!\n1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>1 || kontrol != 1) {
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		switch (sorgu){
			case 0:{
				AnaMenu();
			}break;
			case 1:{
				hesapSil(mS, 1);
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
		printf("İşlem başarılı...\n\n");
		printf("1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>1 || kontrol != 1) {
				printf("Hatalı giriş!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		switch (sorgu){
			case 0:{
				AnaMenu();
			}break;
			case 1:{
				MusteriIslem(mS);
			}break;
		}
	}
}
int hesapSec(int mS, int hS, int s){// s 1 ise normal hesap 2 ise kayitli hesap
	int i, t, sorgu, kontrol, hNo, n, tmS;
	char c;
	if (s==1){
		n = (aBank.musteri+mS)->hesapSayisi;
		if (n==1) return 0;
	}else n = (aBank.musteri+mS)->tHesapSayisi;
	if(n==0) return -1;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("İşlem yapılacak hesap no seçiniz (İptal etmek için 0 giriniz): \n\n");
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
	printf("\nSeçim: ");
	do{
		strAl(temp, 1, 2);
		t = strlen(temp);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1 && t == 1){
			if (hS == -1) MusteriIslem(mS);
			else hesapIslem(mS, hS);
		}
		if (sorgu<1 || sorgu>n || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	return sorgu-1;
}

int NoOlustur(int n){ //n 1 ise musterino 2 ise hesapno
	int No, kontrol;
	srand(time(NULL)+n);
	do{
		if (n==1) No = (rand()%900+100)*1000000 + (rand()%1000)*1000 + rand()%1000;
		else if(n==2) No = (rand()%9000+1000)*1000 + (rand()%1000);
		kontrol = (n==1)?mNoKontrol(No):hNoKontrol(No, 1);
	}while(kontrol!=-1);
	return No;
}
char *sifrele(char sifre[120]){
	int i, iTemp[8]={}, len = strlen(sifre);
	for (i=0; i<8; i++){
		*(iTemp+i) += ((i+1) * (*(sifre+(i%len)) + 11) + *(sifre+len-(i%len)-1) + i);
		*(sifre+i) = *(iTemp+i)%93 + 33;
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
int hNoKontrol(int hesapNo, int n){//n 1 ise müşteri sırası 2 ise hesap sırası
	int mS, hS;
	for (mS=0; mS<aBank.mSayisi; mS++){
		for (hS=0; hS<(aBank.musteri+mS)->hesapSayisi; hS++){
			if (((aBank.musteri+mS)->hesap+hS)->hesapNo == hesapNo) return (n==1)?mS:hS;
		}
	}
	return -1;
}
int thKontrol(int mS, int hNo){
	int i;
	for (i=0; i<(aBank.musteri+mS)->tHesapSayisi; i++){
		if (((aBank.musteri+mS)->tHesap+i)->hesapNo == hNo) return i;
	}
	return -1;
}

void strAl(char str[120], int min, int max){
	int t, i, kontrol;
	char cTemp;
	do{
		for (i=0; i<120; i++) {
			str[i] = '\0';
		}
		kontrol=1;
		scanf("%s", str);
		t = strlen(str);
		do{
			scanf("%c", &cTemp);
			if (cTemp == ' ') cTemp = '-';
			*(str + t++) = cTemp;
		}while (cTemp != '\n');
		*(str+t-1) = 0;
		t = strlen(str);
		if (*(str+t-1) == '-') kontrol=0;
		if (t==1 && *str == '0') break;
		if (t>max || t<min || kontrol != 1){
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while (kontrol != 1);
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
	char c;
	iS = ((aBank.musteri+mS)->hesap+hS)->islemSayisi;
	t1 = ((((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Yil - 1900) * 12 + (((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Ay;
	t2 = tm.tm_year * 12 + tm.tm_mon+1 ;
	aralik = t2-t1;
	int index[aralik+1];
	
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
	system("@cls||clear");
	printf(".............aBank.............\n");
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
	printf("\n\nSeçim (İptal etmek için 0 giriniz): ");
	do{
		strAl(temp, 1, 2);
		t = strlen(temp);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if(sorgu<0 || sorgu>n || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	
	fclose(fopen("dekont.txt", "w"));
	if ((pf = fopen("dekont.txt", "a")) == NULL){
		printf("Dosya açma hatası!\n");
		exit(1);
	}
	i = *(index + sorgu);
	iY = (t2 - i) / 12 + 1900;
	iA = (t2 - i -1) % 12 +1; 
	if (i==0) fprintf(pf, "(%02d - %02d)  Dönemi hesap özeti", iY, iA);
	else fprintf(pf, "(%d - %02d/%02d)  Dönemi hesap özeti", iY, iA, iA%12+1);
	fprintf(pf, "  (%d)\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo);
	for (j=(dekont+i)->islemSayisi-1; j>=0; j--){
		if (((dekont+i)->islem+j)->iTuru == 1){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para çekme    %.2lf\n", 
				((dekont+i)->islem+j)->tarih.Gun,
				((dekont+i)->islem+j)->tarih.Ay,
				((dekont+i)->islem+j)->tarih.Yil,
				((dekont+i)->islem+j)->tarih.Saat,
				((dekont+i)->islem+j)->tarih.Dk,
				-((dekont+i)->islem+j)->Tutar);
		}else if (((dekont+i)->islem+j)->iTuru == 2){
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para yatırma  %.2lf\n", 
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
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden havale  %.2lf (-%.2lf)\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar,
						kesinti);
					fprintf(pf, "\tGönderilen kişi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}else{
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden havale  %.2lf\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar);
					fprintf(pf, "\tGönderilen kişi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}
			}else if (((dekont+i)->islem+j)->Tutar>0){
				fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Gelen havale  %.2lf\n", 
					((dekont+i)->islem+j)->tarih.Gun,
					((dekont+i)->islem+j)->tarih.Ay,
					((dekont+i)->islem+j)->tarih.Yil,
					((dekont+i)->islem+j)->tarih.Saat,
					((dekont+i)->islem+j)->tarih.Dk,
					((dekont+i)->islem+j)->Tutar);
				fprintf(pf, "\tGönderen kişi:    %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
			}
		}
	}
	fclose(pf);
	if ((pf = fopen("dekont.txt", "r")) == NULL){
		printf("Dosya açma hatası!\n");
		exit(1);
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	while ((c=fgetc(pf))!=EOF){
		printf("%c", c);
	}
	fclose(pf);
	printf("\n1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			hesapOzeti(mS, hS);
		}break;
	}
}

void bankaRapor(int mS){
	FILE *pf;
	int sorgu, kontrol;
	char c;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	if ((pf = fopen("rapor.txt", "r")) == NULL){
		printf("Dosya açma hatası!\n");
		exit(1);
	}
	while((c = fgetc(pf)) != EOF){
		printf("%c", c);
	}
	printf("\n\n1-)\tGeri dön\n0-)\tAna menü\nSeçim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatalı giriş!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			AnaMenu();
		}break;
		case 1:{
			MusteriIslem(mS);
		}break;
	}
}

void islemKaydi(int mS, int hS, int iT, int iH, double iTutar){
	int iS, mTuru;
	double kesinti;
	iS = ((aBank.musteri+mS)->hesap+hS)->islemSayisi;
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

	mTuru = (aBank.musteri+mS)->mTuru;
	if (iT == 1){
		aBank.tGiden -= iTutar;
	}else if (iT == 2){
		aBank.tGelen += iTutar;
	}else if (iT == 3 && mTuru == 1){
		kesinti = ((int)(-iTutar*2)) / 100.0;
		aBank.tKar += kesinti;
	}
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