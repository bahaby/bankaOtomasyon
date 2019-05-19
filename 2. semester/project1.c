#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ISLEM 500//dizi büyüklüklerinikolay değiştirebilmek için
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
	int iTuru; //1->para cekme, 2->para yatirma, 3->Havale
	int iHesap;
	double iTutar;
	Tarih tarih;
}Islem;

typedef struct{
	int hesapNo;
	int islemSayisi;//hesaptaki toplam işlem sayısı
	double bakiye;
	Islem islem[MAX_ISLEM];
}Hesap;

typedef struct{
	int hesapNo;
}transferHesap;//havale hesabları için oluşturulan struct

typedef struct{

	Hesap hesap[MAX_HESAP];
	transferHesap tHesap[MAX_HESAP];
	double tBakiye;
	char Ad[120];
	char Sifre[120];
	double tcNo;
	int hesapSayisi;//müşterinin toplam hesap sayısı
	int tHesapSayisi;//havale hesabı sayısı
	int musteriNo;
	int mTuru;//1->bireysel || 2->ticari
}Musteri;

typedef struct{
	Musteri musteri[MAX_MUSTERI];
	int mSayisi;//toplam müşteri sayısı
	int girisYapan;//giriş yapmış olan müşterinin indisi -1 ise giriş yapan yok
	double tGelen;//bankaya giren toplam para
	double tGiden;//bankadan çıkan toplam para
	double tKar;//bankanın toplam karı
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
}Dekont;//yapılan işlemlerin aylık olarak düzenlenip saklandığı struct yapısı(dekont[0] son işlemin yapıldığı ay)

Banka aBank;
Dekont dekont[MAX_DEKONT];

char temp[120];
//mS = stractaki musteri indisi, hS = stracttaki hesap indisi
void AnaMenu();//ana menu
void YeniMusteri();//yeni musteri olusturup musteriye 1 hesap ekler
void MusteriIslem(int mS);//musteri islem menusu
void Guncelle();//structta depolanan verileri musteri dosyalari ve rapor.txt ye yazar
void VeriAl();//program baslangicinda musteri doyalarindaki verileri structta depolar
void bankaRapor(int mS);//rapor.txt yi ekrana yazdirir--mS islemin yapildigi musteriye geri donebilmek icin
void hesapIslem(int mS, int hS);//Hesap islemleri
void paraCek(int mS, int hS);//para cekme islemleri
void paraYatir(int mS, int hS);//para yatirma islemleri
void havaleGonder(int mS, int hS);//havale islemleri
void hHesapKayit(int mS, int hS, int hNo);//hNo -1 ise hesap no sorar ve kayitli hesaplara kaydeder veya hNo yu kayitli hesaplara kaydeder
void hesapAc(int mS);//musteriye yeni hesap acar
void hesapSil(int mS, int s);//s'in degerine gore kayitli veya normal hesap siler once silinecek hesabi sectirir
void musteriSil(int mS);
void hesapOzeti(int mS, int hS);//aylik olarak islem gecmisini gruplar secilen tarihteki hesap ozetini dekont.txt ye yazdirir ve ekranda gosterir
void islemKaydi(int mS, int hS, int iT, int iH, double iTutar);//islem kaydi yapar
int hesapSec(int mS, int hS, int s);//hesap secme menusu secilen hesap noyu dondurur (s'in degerine gore normal hesap veya kayitli hesap)
int NoOlustur(int n);//hesap numaralarini karsilastirip random ve farkli bir hesap numarasi dondurur
int tcNoKontrol(double tcNo);//tc no kontrolu tc no varsa musteri indisini yoksa -1 dondurur
int mNoKontrol(int musteriNo);//musteri no kontrolu yoksa -1 varsa indisini dondurur
int hNoKontrol(int hesapNo, int n); //n 1 ise musteri indisi 2 ise hesap indisi dondurur hesap numarasi yoksa -1 dondurur
int thKontrol(int mS, int hNo);//transfer hesap kontrolu yoksa -1 varsa indisini dondurur
char *sifrele(char sifre[120]);//musteri sifresini sifreler 8 haneli karakter dizisi dondurur
void strAl(char str[], int min, int max);//input alir bosluklari '-' ye cevirir min dan kisa veya max dan uzun karakter girilirse hata verir tekrar input ister
double cekilenPara(int mS);//musterinin gunluk cektigi parayi hesaplar ve dondurur
void isimDuzelt(char ad[120]);//isimlerdeki '-' leri bosluga cevirir

int main(){
	VeriAl();
	aBank.girisYapan = -1;
	AnaMenu();
}

void VeriAl(){//dosyaya yazdırılan verileri struct yapısına yükler
	int i, j, mNo;//mNo -> Dosyadaki musteri sırası
	FILE *pf;
	for (i=0; i<5; i++){//okuma başarısız olma ihtimali için 5 kere deneme
		pf = fopen("bireyselMusteri.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf!=NULL){
		fseek(pf, 0, SEEK_END);
		if (ftell(pf) != 0){//dosya boşmu değilmi kontrolü
			fseek(pf, 0, SEEK_SET);
			while(!feof(pf)){
				fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
				strcpy((aBank.musteri+mNo-1)->Sifre, temp);
				fscanf(pf, " Tc no: %lf", &(aBank.musteri+mNo-1)->tcNo);
				fscanf(pf, " Ad soyad: %s", (aBank.musteri+mNo-1)->Ad);
				fscanf(pf, " Musteri no: %d", &(aBank.musteri+mNo-1)->musteriNo);
				fscanf(pf, " Hesap sayisi: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
				for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
					fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
					fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
					fscanf(pf, " Islem sayisi: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
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
				fscanf(pf, " Kayitli havale hesap sayisi: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
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
	for (i=0; i<5; i++){//okuma başarısız olma ihtimali için 5 kere deneme
		pf = fopen("ticariMusteri.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf!=NULL){
		fseek(pf, 0, SEEK_END);
		if (ftell(pf) != 0){//dosya boşmu değilmi kontrolü
			fseek(pf, 0, SEEK_SET);
			while(!feof(pf)){
				fscanf(pf, " Musteri: %d / [ %s ]", &mNo, temp);
				strcpy((aBank.musteri+mNo-1)->Sifre, temp);
				fscanf(pf, " Tc no: %lf", &(aBank.musteri+mNo-1)->tcNo);
				fscanf(pf, " Ad soyad: %s", (aBank.musteri+mNo-1)->Ad);
				fscanf(pf, " Musteri no: %d", &(aBank.musteri+mNo-1)->musteriNo);
				fscanf(pf, " Hesap sayisi: %d", &(aBank.musteri+mNo-1)->hesapSayisi);
				for (i=0; i<(aBank.musteri+mNo-1)->hesapSayisi; i++){
					fscanf(pf, " Hesap %*d : %d", &((aBank.musteri+mNo-1)->hesap+i)->hesapNo);
					fscanf(pf, " Bakiye: %lf", &((aBank.musteri+mNo-1)->hesap+i)->bakiye);
					fscanf(pf, " Islem sayisi: %d", &((aBank.musteri+mNo-1)->hesap+i)->islemSayisi);
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
				fscanf(pf, " Kayitli havale hesap sayisi: %d", &(aBank.musteri+mNo-1)->tHesapSayisi);
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
	for (i=0; i<5; i++){//okuma başarısız olma ihtimali için 5 kere deneme
		pf = fopen("rapor.txt", "r");
		if(pf!=NULL) break;
	}
	if (pf != NULL){
		fscanf(pf, " aBank gelir-gider raporu...");
		fscanf(pf, " ");
		fscanf(pf, " Bankada bulunan toplam para: %*f");
		fscanf(pf, " Gelen toplam para: %lf", &aBank.tGelen);
		fscanf(pf, " Giden toplam para: %lf", &aBank.tGiden);
		fscanf(pf, " Bankanin kari: %lf", &aBank.tKar);
		fclose(pf);
	}

	for (i=0; i<aBank.mSayisi; i++){//müşterinin bütün hesaplarındaki paraları toplayıp struct elemanına atar
		(aBank.musteri+i)->tBakiye = 0;
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			(aBank.musteri+i)->tBakiye += ((aBank.musteri+i)->hesap+j)->bakiye;
		}
	}
}

void Guncelle(){//struct yapısında olan verileri dosyaya yazdırır
	int i, j, k, b=0, t=0;
	FILE *pf1, *pf2;
	if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));//döngüde sadece ilk okuma için dosyayı silip tekrar açsın diye
	if (t==0) fclose(fopen("ticariMusteri.txt", "w"));//döngüde sadece ilk okuma için dosyayı silip tekrar açsın diye
	if ((pf1 = fopen("bireyselMusteri.txt", "a")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	if ((pf2 = fopen("ticariMusteri.txt", "a")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->mTuru == 1){
			if (b!=0) fprintf(pf1, "\n");//ilk okuma dışında döngünün başında alt satıra inmek için
			fprintf(pf1, "Musteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf1, "\n\tTc no: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf1, "\n\tAd soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf1, "\n\tMusteri no: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf1, "\n\tHesap sayisi: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf1, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf1, "\n\t\tIslem sayisi: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
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
			fprintf(pf1, "\n\tKayitli havale hesap sayisi: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf1, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			b=1;
		}else if ((aBank.musteri+i)->mTuru == 2){
			if (t!=0) fprintf(pf2, "\n");//ilk okuma dışında döngünün başında alt satıra inmek için
			fprintf(pf2, "Musteri: %d / [ %s ]", i+1, (aBank.musteri+i)->Sifre);
			fprintf(pf2, "\n\tTc no: %.lf", (aBank.musteri+i)->tcNo);
			fprintf(pf2, "\n\tAd soyad: %s", (aBank.musteri+i)->Ad);
			fprintf(pf2, "\n\tMusteri no: %d", (aBank.musteri+i)->musteriNo);
			fprintf(pf2, "\n\tHesap sayisi: %d", (aBank.musteri+i)->hesapSayisi);
			for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->hesap+j)->hesapNo);
				fprintf(pf2, "\n\t\tBakiye: %.2lf", ((aBank.musteri+i)->hesap+j)->bakiye);
				fprintf(pf2, "\n\t\tIslem sayisi: %d", ((aBank.musteri+i)->hesap+j)->islemSayisi);
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
			fprintf(pf2, "\n\tKayitli havale hesap sayisi: %d", (aBank.musteri+i)->tHesapSayisi);
			for (j=0; j<(aBank.musteri+i)->tHesapSayisi; j++){
				fprintf(pf2, "\n\t\tHesap %d: %d", j+1, ((aBank.musteri+i)->tHesap+j)->hesapNo);
			}
			t=1;
		}
	}
	fclose(pf1);
	fclose(pf2);


	fclose(fopen("rapor.txt", "w"));
	if ((pf1 = fopen("rapor.txt", "a")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	fprintf(pf1, "aBank gelir-gider raporu...\n\n");
	fprintf(pf1, "Bankada bulunan toplam para: %.2lf\n", aBank.tGelen - aBank.tGiden + aBank.tKar);
	fprintf(pf1, "Gelen toplam para:           %.2lf\n", aBank.tGelen);
	fprintf(pf1, "Giden toplam para:           %.2lf\n", aBank.tGiden);
	fprintf(pf1, "Bankanin kari:               %.2lf", aBank.tKar);
	fclose(pf1);

	for (i=0; i<aBank.mSayisi; i++){//müşterinin bütün hesaplarındaki paraları toplayıp struct elemanına atar
		(aBank.musteri+i)->tBakiye = 0;
		for (j=0; j<(aBank.musteri+i)->hesapSayisi; j++){
			(aBank.musteri+i)->tBakiye += ((aBank.musteri+i)->hesap+j)->bakiye;
		}
	}
}

void AnaMenu(){
	int sorgu, kontrol, n;
	char c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if (aBank.girisYapan == -1){//giriş yapan yoksa
		printf("1-)\tYeni musteri kaydi\n");
		printf("2-)\tMusteri girisi\n");
		n=2;
	}else{//giriş yapan varsa
		printf("1-)\tMusteri islemleri\n");
		printf("2-)\tMusteri kaydini sil\n");
		printf("3-)\tMusteri cikisi\n");
		n=3;
	}
	printf("0-)\tCikis\n");
	printf("Secim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>n || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 2 && aBank.mSayisi==0){
			printf("Herhangi bir musteri yok henuz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 2 && (aBank.musteri+aBank.girisYapan)->tBakiye != 0 && aBank.girisYapan != -1){
			printf("Hesaplarinizda para varken kaydinizi silemezsiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 0:{
			printf("Cikis basarili\n\n");
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
				musteriSil(aBank.girisYapan);
			}
		}break;
		case 3:{
			if (aBank.girisYapan != -1){
				aBank.girisYapan = -1;
				AnaMenu();
			}
		}break;
	}
}

void YeniMusteri(){//yeni müşteri kaydı
	int sorgu, kontrol, is1, is2, i, j, k, t;
	double dTemp;
	char s1[120]={}, s2[120]={}, c;//s1 s2 girilen şifrelerin atandığı değişkenler
	char alfabe[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
	//char alfabe[] = "abcdefghijklmnoprstuvyzABCDEFGHIJKLMNOPRSTUVYZ-";
	int lenA = strlen(alfabe);

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("1-)\tBireysel musteri\n2-)\tTicari musteri\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
	printf("Ad soyad (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 5, 120);
		kontrol=1;//döngüden çıkılsınmı çıkılmasınmı kontrolü için
		t = strlen(temp);
		k=0;//kullanılan (-) boşluk sayısnın atanacağı değişken
		for (i=0; i<t && kontrol==1; i++){//alfabe kontrolü ve bütün harfleri küçültme için
			for (j=0; j<lenA; j++){//alfabe kontrolü
				if(*(temp+i) == *(alfabe+j)) {
					kontrol=1;
					break;
				}else kontrol=0;
			}
			for (j=0; j<lenA/2; j++){//bütün büyük harfleri küçültme
				if (*(temp+i) == *(alfabe+j+lenA/2)) *(temp+i) = *(alfabe+j);
			}
			if (i!=0 && *(temp+i-1) == '-' && *(temp+i) == '-') kontrol=0;//2 boşluk yan yana varsa hata versin diye
			if (*(temp+i) == '-') k++;//boşluk hiç yoksa hata versin diye
		}
		for (i=0; i<t && kontrol == 1; i++){//kullanıcı adının ilk harflerini büyütmek için
			for (j=0; j<lenA/2; j++){
				if (i==0 && *(temp+i) == *(alfabe+j)){//ilk harf için
					*(temp+i) = *(alfabe+j+lenA/2);
				}else if (*(temp+i) == '-' && *(temp+i+1) == *(alfabe+j)){//boşluk sonrası harfler için
					*(temp+i+1) = *(alfabe+j+lenA/2);
					i++;
				}
			}
		}
		if (k == 0) kontrol=0;//boşluk yoksa hata
		if (t == 1 && *temp == '0') YeniMusteri();//işlem iptali için
		if (kontrol != 1) printf("Hatali giris!\nTekrar deneyiniz: ");
	}while(kontrol != 1);
	strcpy((aBank.musteri+aBank.mSayisi)->Ad, temp);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc numaranizi giriniz (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 11, 11);
		t = strlen(temp);
		if (t == 1 && *temp == '0') YeniMusteri();//işlem iptali için
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);//doğru input alabilmek için
		if (kontrol != 1){
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (tcNoKontrol(dTemp) != -1){//Tc no kayıtlımı kontrolü
			printf("Bu tc numarasi kayitli!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	(aBank.musteri+aBank.mSayisi)->tcNo = dTemp;

	system("@cls||clear");
	printf(".............aBank.............\n");
	do{
		printf("Sifrenizi giriniz (Min 4, max 8 hane. Iptal etmek icin 0 giriniz): ");
		strAl(s1, 1, 30);
		t = strlen(s1);
		if (t == 1 && *s1 == '0') YeniMusteri();
		sscanf(s1, "%d%c", &is1, &c);
		printf("Tekrar giriniz: ");
		strAl(s2, 1, 30);
		t = strlen(s2);
		if (t == 1 && *s2 == '0') YeniMusteri();
		kontrol = sscanf(s2, "%d%c", &is2, &c);
		if (!(strcmp(s1, s2) == 0 && is1>=0 && is1<=99999999 && kontrol == 1)){//şifreler aynımı kontrolü
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hatali giris!\n");
			kontrol = 0;
		}
	}while(kontrol != 1);
	strcpy((aBank.musteri+aBank.mSayisi)->Sifre, sifrele(s1));//kullanıcı şifresini şifreleyerek structa kaydeder

	(aBank.musteri+aBank.mSayisi)->hesapSayisi = 1;
	(aBank.musteri+aBank.mSayisi)->tHesapSayisi = 0;
	(aBank.musteri+aBank.mSayisi)->musteriNo = NoOlustur(1);//9 habeli rasgele rakam oluşturur
	((aBank.musteri+aBank.mSayisi)->hesap)->hesapNo = NoOlustur(2);//7 haneli rasgele rakam oluşturur (ilk hesap)
	((aBank.musteri+aBank.mSayisi)->hesap)->bakiye = 0;
	((aBank.musteri+aBank.mSayisi)->hesap)->islemSayisi = 0;
	aBank.girisYapan = aBank.mSayisi;
	aBank.mSayisi++;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\nHesabiniz kuruldu...\n\n");
	strcpy(temp, (aBank.musteri+aBank.mSayisi-1)->Ad);
	isimDuzelt(temp);//ekrana yazdırma işlemi için (-) leri boşluğa çevirir
	printf("Adiniz: %s\n", temp);
	printf("Tc numaraniz: %.lf\n", (aBank.musteri+aBank.mSayisi-1)->tcNo);
	printf("Musteri numaraniz: %d\n", (aBank.musteri+aBank.mSayisi-1)->musteriNo);
	printf("Hesap numaraniz: %d\n", ((aBank.musteri+aBank.mSayisi-1)->hesap)->hesapNo);
	printf("Sifreniz: %s\n\n", s2);
	printf("1-)\tMusteri islemleri\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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

void MusteriIslem(int mS){//structtaki indisi mS olan müşteri işlemleri için
	int sonuc, sorgu, kontrol, t, s, iTemp;
	char c;
	double dTemp;
	if (mS == -1){// mS -1 ise kulllanıcı girişi yapılır
		char sifre[120];
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Tc no veya musteri no giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 1, 11);
			t = strlen(temp);
			if(t==1 && *temp == '0'){
				AnaMenu();
			}else if(t==9){//müşteri no girildi ise
				kontrol = sscanf(temp, "%d%c", &iTemp, &c);
				sonuc = mNoKontrol(iTemp);
			}else if(t==11){//tc no girildi ise
				kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
				sonuc = tcNoKontrol(dTemp);
			}else kontrol = 0;
			if (!(sonuc != -1 && kontrol == 1 && *temp != '0')){
				printf("Hatali giris!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		mS=sonuc;//giriş yapan müşterinin structtaki indisi
		s=0;//şifre kontolunde ilk başta hata vermesin diye
		do{
			system("@cls||clear");
			printf(".............aBank.............\n");
			if (s!=0) printf("Hatali sifre!%d\n", t);
			printf("Sifrenizi giriniz (Iptal etmek icin 0 giriniz): ");
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
	printf("Hosgeldin, %s\n", temp);
	printf("Musteri numaraniz: %d\n", (aBank.musteri+mS)->musteriNo);
	printf("Toplam bakiyeniz: %.2lf\n\n", (aBank.musteri+mS)->tBakiye);
	printf("1-)\tHesap islemleri\n2-)\tYeni hesap ac\n3-)\tHesap sil\n4-)\tHavale hesabi kaydet\n5-)\tHavale hesabi sil\n");
	printf("6-)\tRapor al\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>6 || kontrol != 1){
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 3 && (aBank.musteri+mS)->hesapSayisi == 1){//1 hesap kaldı ise hata 
			printf("Son hesabinizi silemezsiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if(sorgu == 5 && (aBank.musteri+mS)->tHesapSayisi == 0){//kayıtlı hesap yoksa hata
			printf("Kayitli hesabiniz kalmamistir!\nTekrar deneyiniz: ");
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

void hesapIslem(int mS, int hS){//mS indisli müşterinin hS indisli hesap işlemleri
	char c;
	int sorgu, kontrol;
	double limit;
	if (hS == -1) hS = hesapSec(mS, -1, 1);//hS -1 ise hesapno seçimi ekranı gelir ve seçilen hesabın indisini hS ye atanır
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabinizdaki bakiyeniz: %.2lf TL'dir.\n\n", ((aBank.musteri+mS)->hesap+hS)->bakiye);
	printf("1-)\tPara cekme\n2-)\tPara yatirma\n3-)\tHavale\n4-)\tHesap ozeti\n5-)\tGeri don\n0-)\tAna menu\nSecim: ");
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);//müşterinin kalan günlük limit hesabı
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>5 ||kontrol != 1){
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if ((sorgu == 1 || sorgu == 3) && (aBank.musteri+mS)->tBakiye == 0){//müşterinin parası yoksa hata
			printf("Bu islem icin hesaplarinizda yeterli para yok!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 1 && limit == 0){//günlük limit yetersiz ise hata
			printf("Gunluk para cekme limitinize ulastiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu == 4 && ((aBank.musteri+mS)->hesap+hS)->islemSayisi==0){//henüz işlem yapılmadı ise hata
			printf("Hesabinizda herhangi bir islem yapilmadi henuz!\nTekrar deneyiniz: ");
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
void paraCek(int mS, int hS){//mS indisli müşterinin hS indisli hesabından para çekme işlemi
	int i, t, kontrol, sorgu;
	char c;
	double dTemp, limit;
	limit = (((aBank.musteri+mS)->mTuru == 1) ? 750 : 1500) - cekilenPara(mS);
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Cekmek istediginiz tutari giriniz (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 10);
		t = strlen(temp);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (kontrol == 1) sorgu = (int)(dTemp*100);//küsürat sorunu için (5 ve katları)
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){//. ile başlıyor ise veya 0 dan sonra . gelmiyor ise hata
			printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu%500 != 0){//5 ve katları değilse hata
			printf("5 ve katlari sadece!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (dTemp>(aBank.musteri+mS)->tBakiye){//müşterinin yeterli parası yoksa hata
			printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
			kontrol = 0;
		}else if (dTemp>limit){//müşterinin yeterli limiti yoksa hata
			printf("Para cekme limitiniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", limit);
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;//küsüratın 2. hanesinden sonrasını silmek için
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){//hesapta yeterli para varsa bu sorgu
		((aBank.musteri+mS)->hesap+hS)->bakiye -= dTemp;
		islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -dTemp);
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
		printf("%d Nolu hesabinizdan %.2lf TL kesildi\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
		dTemp = 0;
	}else{//hesapta yeterli para yoksa..
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarinizi kullanarak islemi tamamla\n2-)\tIslemi iptal et\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali giris!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);//işlem iptali için
		else{//işlem iptal edilmedi ise
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Islem basarili...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){//işlemin yapıldığı hesaptaki bütün para çekilir
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 1, ((aBank.musteri+mS)->hesap+hS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
			}
			for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){//diğer hesaplarda işlem yapılır yeterli para kesilene kadar
				if (((aBank.musteri+mS)->hesap+i)->bakiye != 0){//bakiye 0 ise işlem yapmasın diye
					if (dTemp<=((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){//kesilecek para hesaptaki paradan az ise
						((aBank.musteri+mS)->hesap+i)->bakiye -= dTemp;
						islemKaydi(mS, i, 1, ((aBank.musteri+mS)->hesap+i)->hesapNo, -dTemp);
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);
						dTemp = 0;
					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){//kesilecek para hesaptaki paradan fazla ise
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
	printf("1-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
void paraYatir(int mS, int hS){//mS indisli müşterinin hS indisli hesabına para yatırma işlemi
	int kontrol, sorgu, t;
	char c;
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Yatirmak istediginiz tutari giriniz (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 12);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		t = strlen(temp);
		if (kontrol == 1) sorgu = (int)(dTemp*100);//küsürat sorunu için (5tl katı)
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || dTemp>=1000000 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){//. ve 0. kontolü
			printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (sorgu%500 != 0){//5tl katı kontrolü
			printf("5 ve katlari sadece!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;

	((aBank.musteri+mS)->hesap+hS)->bakiye += dTemp;
	islemKaydi(mS, hS, 2, ((aBank.musteri+mS)->hesap+hS)->hesapNo, dTemp);
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
void havaleGonder(int mS, int hS){//mS indisli müşterinin hS indisli hesabının havale işlemi
	double dTemp, kesinti;
	int i, t, tHesapS, hhS, hmS, tHesapNo, sorgu, kontrol;
	char c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	if ((aBank.musteri+mS)->tHesapSayisi != 0){//kayıtlı havale hesabı varsa 
		printf("1-)\tKayitli havale hesabina gonder\n2-)\tHesap no girerek gonder\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali giris!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		switch (sorgu){
			case 1:{
				tHesapS = hesapSec(mS, hS, 2);//kayıtlı havale hesapları arasından seçim yaptırılır ve seçilen hesabın indisi tHesapS ye atanır
				tHesapNo = ((aBank.musteri+mS)->tHesap+tHesapS)->hesapNo;//indisten hesap no elde edilir
				if (tHesapNo == ((aBank.musteri+mS)->hesap+hS)->hesapNo){
					system("@cls||clear");
					printf(".............aBank.............\n");
					printf("Islem yaptiginiz hesabi secemezsiniz...\n\n");
					printf("1-)\tGeri don\nSecim: ");
					do{
						strAl(temp, 1, 1);
						kontrol = sscanf(temp, "%d%c", &sorgu, &c);
						if(sorgu<0 || sorgu>1 || kontrol != 1) {
							printf("Hatali giris!\nTekrar deneyiniz: ");
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
				printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
				do{
					strAl(temp, 7, 7);
					t = strlen(temp);
					kontrol = sscanf(temp, "%d%c", &sorgu, &c);
					if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
					if (kontrol != 1) printf("Hatali giris!\nTekrar deneyiniz: ");
					else if (hNoKontrol(sorgu, 1) == -1){
						printf("Boyle bir hesap numarasi yok!\nTekrar deneyiniz: ");
						kontrol = 0;
					}
				}while(kontrol != 1);
				tHesapNo = sorgu;
			}break;
		}
	}else{
		printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 7, 7);
			t = strlen(temp);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
			if (kontrol != 1) printf("Hatali giris!\nTekrar deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1){
				printf("Boyle bir hesap numarasi yok!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		tHesapNo = sorgu;
	}

	hhS = hNoKontrol(tHesapNo, 2);//havalenin yapılacağı hesabın indisi
	hmS = hNoKontrol(tHesapNo, 1);//havalenin yapılacağı hesabın sahibinin müşteri indisi

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Gondermek istediginiz tutari giriniz (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 12);
		t = strlen(temp);
		kontrol = sscanf(temp, "%lf%c", &dTemp, &c);
		if (dTemp == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if (dTemp<=0 || kontrol != 1 || *temp == '.' || (*temp == '0' && *(temp+1) != '.')){
			printf("Hatali giris yaptiniz!\nTekrar deneyiniz: ");
			kontrol = 0;
		}else if (dTemp>(aBank.musteri+mS)->tBakiye && kontrol == 1){//müşterinin yeterli parası yoksa hata
			printf("Toplam bakiyeniz %.2lf TL'dir!\nFarkli bir miktar giriniz: ", (aBank.musteri+mS)->tBakiye);
			kontrol = 0;
		}
	}while(kontrol != 1);
	dTemp = ((int)(dTemp*100)) / 100.0;
	if (dTemp<=((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){//hesapta yeterli para varsa bu sorgu
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
	}else{//hesapta yeterli para yoksa..
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap bakiyeniz yetersiz!\n");
		printf("1-)\tEk hesaplarinizi kullanarak islemi tamamla\n2-)\tIslemi iptal et\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<1 || sorgu>2 || kontrol != 1) {
				printf("Hatali giris!\nTekrar deneyiniz: ");
				kontrol = 0;
			}
		}while(kontrol != 1);
		if (sorgu == 2) hesapIslem(mS, hS);
		else{//işlem iptal edilmedi ise
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Islem basarili...\n\n");
			if (dTemp>((aBank.musteri+mS)->hesap+hS)->bakiye && ((aBank.musteri+mS)->hesap+hS)->bakiye != 0){//işlemin yapıldığı hesaptaki bütün para çekilir
				dTemp -= ((aBank.musteri+mS)->hesap+hS)->bakiye;
				islemKaydi(mS, hS, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -((aBank.musteri+mS)->hesap+hS)->bakiye);
				kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(((aBank.musteri+mS)->hesap+hS)->bakiye*2)) / 100.0 : 0;
				printf("%d Nolu hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye);
				
				((aBank.musteri+hmS)->hesap+hhS)->bakiye += (((aBank.musteri+mS)->hesap+hS)->bakiye - kesinti);
				islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+hS)->hesapNo, ((aBank.musteri+mS)->hesap+hS)->bakiye - kesinti);
				((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
			}
			for (i=0; i<(aBank.musteri+mS)->hesapSayisi; i++){//diğer hesaplarda işlem yapılır yeterli para kesilene kadar
				if (((aBank.musteri+mS)->hesap+i)->bakiye != 0){//bakiye 0 ise işlem yapmasın diye
					if (dTemp<=((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){//kesilecek para hesaptaki paradan az ise
						((aBank.musteri+mS)->hesap+i)->bakiye -= dTemp;
						islemKaydi(mS, i, 3, ((aBank.musteri+hmS)->hesap+hhS)->hesapNo, -dTemp);
						kesinti = ((aBank.musteri+mS)->mTuru == 1) ? ((int)(dTemp*2)) / 100.0 : 0;
						printf("%d Nolu ek hesabinizdan %.2lf TL kesildi\n", ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp);

						((aBank.musteri+hmS)->hesap+hhS)->bakiye += (dTemp - kesinti);
						islemKaydi(hmS, hhS, 3, ((aBank.musteri+mS)->hesap+i)->hesapNo, dTemp - kesinti);
						dTemp = 0;
					}else if (dTemp>((aBank.musteri+mS)->hesap+i)->bakiye && dTemp!=0){//kesilecek para hesaptaki paradan fazla ise
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
	printf("1-)\tHavale hesabi olarak kaydet\n2-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
void hHesapKayit(int mS, int hS, int hNo){//mS indisli müşterinin hNo hesap nosunu kayıt işlemi (hS sadece geri dön işlemi için)
	int sorgu, kontrol, s=1, t;
	char c;
	if (hNo == -1){//hNo -1 ise kaydedilecek hesap no sorulur
		s=0;
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesap numarasini giriniz (Iptal etmek icin 0 giriniz): ");
		do{
			strAl(temp, 7, 7);
			t = strlen(temp);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if (sorgu == 0 && kontrol == 1 && t == 1) MusteriIslem(mS);
			if (kontrol != 1) printf("Hatali giris!\nTekrar deneyiniz: ");
			else if (hNoKontrol(sorgu, 1) == -1){//hesap no varmı kontrolü
				printf("Boyle bir hesap numarasi yok!\nTekrar deneyiniz: ");
				kontrol = 0;
			}else if (thKontrol(mS, sorgu) != -1){//hesap zaten kayıtlı mı kontrolü
				printf("Hesap numarasi zaten kayitli!\nTekrar deneyiniz: ");
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
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
void hesapAc(int mS){//mS indisli müşterinin hesap açma işlemi
	int sorgu, kontrol, hS = (aBank.musteri+mS)->hesapSayisi;
	char c;
	((aBank.musteri+mS)->hesap+hS)->hesapNo = NoOlustur(2);//7 haneli rasgele rakam oluşturur
	((aBank.musteri+mS)->hesap+hS)->bakiye = 0;
	((aBank.musteri+mS)->hesap+hS)->islemSayisi = 0;
	(aBank.musteri+mS)->hesapSayisi++;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hesabiniz Acildi..\nHesap numaraniz: %d\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo);
	printf("1-)\tHesap islemleri\n2-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>2 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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
void hesapSil(int mS, int s){//mS indisli müşterinin hesap silme işlemi
	int i, n, sorgu, kontrol, shS;
	char c;
	shS =  hesapSec(mS, -1, s);// s 1 ise normal hesap 2 ise havale hesap seçimi yapılır seçilen hesabın indisi shS ye atanır
	n = (s==1) ? (aBank.musteri+mS)->hesapSayisi : (aBank.musteri+mS)->tHesapSayisi;//s 1 ise normal hesap 2 ise havale hesap sayısı alınır
	if (s==1 && ((aBank.musteri+mS)->hesap+shS)->bakiye!=0){//normal hesap silme işlemi
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Hesabinizda para varken hesabinizi silemezsiniz!\n1-)\tGeri don\n0-)\tAna menu\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>1 || kontrol != 1) {
				printf("Hatali giris!\nTekrar deneyiniz: ");
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
	}else{//havale hesap silme işlemi
		for (i=shS; i<n; i++){//struct eşitleme kullanılarak silme işlemi yapılır
			if (s==1){//normal hesap ise
				*((aBank.musteri+mS)->hesap+i) = *((aBank.musteri+mS)->hesap+i+1); 
			}else{//havale hesabı ise
				*((aBank.musteri+mS)->tHesap+i) = *((aBank.musteri+mS)->tHesap+i+1); 
			}
		}
		(s==1) ? (aBank.musteri+mS)->hesapSayisi-- : (aBank.musteri+mS)->tHesapSayisi--;
		Guncelle();
		system("@cls||clear");
		printf(".............aBank.............\n");
		printf("Islem basarili...\n\n");
		printf("1-)\tGeri don\n0-)\tAna menu\nSecim: ");
		do{
			strAl(temp, 1, 1);
			kontrol = sscanf(temp, "%d%c", &sorgu, &c);
			if(sorgu<0 || sorgu>1 || kontrol != 1) {
				printf("Hatali giris!\nTekrar deneyiniz: ");
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
void musteriSil(int mS){//müsteri kaydi siler
	int i, kontrol, sorgu;
	char c;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Musteri kaydinizi silmek istediginize eminmisiz?\n\n1-)\tMusteri kaydi sil\n2-)\tGeri don\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<1 || sorgu>2 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	if (sorgu == 2) AnaMenu();
	for(i=mS; i<aBank.mSayisi; i++){//struct eşitleme kullanılarak silinecek hesaptan başlanarak müşterilerin indisleri 1 kaydırılır
		*(aBank.musteri+i) = *(aBank.musteri+i+1);
	}
	aBank.mSayisi--;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem basarili...\n\n");
	printf("1-)\tGeri don\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<1 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	switch (sorgu){
		case 1:{
			aBank.girisYapan = -1;
			AnaMenu();
		}break;
	}
}
int hesapSec(int mS, int hS, int s){//mS indisli müşterinin hesap seçme işlemi (hS geri dön menüsü için
	int i, t, sorgu, kontrol, hNo, n, tmS;
	char c;
	if (s==1){//s 1 ise normal hesap
		n = (aBank.musteri+mS)->hesapSayisi;
		if (n==1) return 0;
	}else n = (aBank.musteri+mS)->tHesapSayisi; //s 1 değilse havale hesap 
	if(n==0) return -1;//hiç hesap yoksa -1 döner
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Islem yapilacak hesap no seciniz (Iptal etmek icin 0 giriniz): \n\n");
	for (i=0; i<n; i++){
		hNo = (s==1) ? ((aBank.musteri+mS)->hesap+i)->hesapNo : ((aBank.musteri+mS)->tHesap+i)->hesapNo;
		if (s==1){//s 1 ise normal hesapları listeler
			printf("%d-) %d (%.2lf TL)\n", i+1, hNo, ((aBank.musteri+mS)->hesap+i)->bakiye);
		}else{//s 1 değilse havale hesaplarını listeler
			tmS = hNoKontrol(hNo, 1);
			strcpy(temp, (aBank.musteri+tmS)->Ad);
			isimDuzelt(temp);
			printf("%d-) %d (%s)\n", i+1, hNo, temp);
		}
	}
	printf("\nSecim: ");
	do{
		strAl(temp, 1, 2);
		t = strlen(temp);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1 && t == 1){
			if (hS == -1) MusteriIslem(mS);
			else hesapIslem(mS, hS);
		}
		if (sorgu<1 || sorgu>n || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	return sorgu-1;//seçilen hesabın indisini döndürür
}

int NoOlustur(int n){ //n 1 ise musterino 2 ise hesapno oluşturur
	int No, kontrol;
	srand(time(NULL)+n);
	do{
		if (n==1) No = (rand()%900+100)*1000000 + (rand()%1000)*1000 + rand()%1000;
		else if(n==2) No = (rand()%9000+1000)*1000 + (rand()%1000);
		kontrol = (n==1)?mNoKontrol(No):hNoKontrol(No, 1);
	}while(kontrol!=-1);
	return No;
}
char *sifrele(char sifre[120]){//kullanıcı şifresini şifreler dosyadan şifre anlaşılmasın diye
	int i, iTemp[8]={}, len = strlen(sifre);
	for (i=0; i<8; i++){
		*(iTemp+i) += ((i+1) * (*(sifre+(i%len)) + 11) + *(sifre+len-(i%len)-1) + i);
		*(sifre+i) = *(iTemp+i)%93 + 33;
	}
	*(sifre+8) = '\0';
	return sifre;
}

int tcNoKontrol(double tcNo){//tcNo kontrolü
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->tcNo == tcNo) return i;//tcNo kayıtlı ise müşteri indisini döndürür
	}
	return -1;//kayıtlı değilse -1 döndürür
}
int mNoKontrol(int musteriNo){//müşteri no kontrolü
	int i;
	for (i=0; i<aBank.mSayisi; i++){
		if ((aBank.musteri+i)->musteriNo == musteriNo) return i;//müşteri no varsa müşteri indisini döndürür
	}
	return -1;//yoksa -1 döndürür
}
int hNoKontrol(int hesapNo, int n){//n 1 ise musteri sirasi 2 ise hesap sirasi
	int mS, hS;
	for (mS=0; mS<aBank.mSayisi; mS++){
		for (hS=0; hS<(aBank.musteri+mS)->hesapSayisi; hS++){
			if (((aBank.musteri+mS)->hesap+hS)->hesapNo == hesapNo) return (n==1)?mS:hS;//n 1 ise musteri sirasi 2 ise hesap sirasini döndürür
		}
	}
	return -1;//hesap no yoksa -1 döndürür
}
int thKontrol(int mS, int hNo){//mS indisli müşterinin havale hesap kontrolü
	int i;
	for (i=0; i<(aBank.musteri+mS)->tHesapSayisi; i++){
		if (((aBank.musteri+mS)->tHesap+i)->hesapNo == hNo) return i;
	}
	return -1;
}

void strAl(char str[120], int min, int max){//string almak için (min minimum karakter sayısı, max maximum karakter sayısı)
	int t, i, kontrol;
	char cTemp;
	do{
		for (i=0; i<120; i++) {//bazı hatalar için string sıfırlanır
			str[i] = '\0';
		}
		kontrol=1;
		scanf("%s", str);
		t = strlen(str);
		do{
			scanf("%c", &cTemp);
			if (cTemp == ' ') cTemp = '-';//boşlukları (-) ye çevirir
			*(str + t++) = cTemp;
		}while (cTemp != '\n');
		*(str+t-1) = 0;//string sonunu belirtir
		t = strlen(str);
		if (*(str+t-1) == '-') kontrol=0;//son karakter boşluk ise hata verir
		if (t==1 && *str == '0') break;//iptal işlemleri için döngüden çıkar
		if (t>max || t<min || kontrol != 1){//min max kontrolü
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while (kontrol != 1);
}
void isimDuzelt(char ad[120]){//(-) leri boşluğa çevirir
	int i, t;
	t = strlen(ad);
	for (i=0; i<t; i++){
		if (*(ad+i) == '-') *(ad+i) = ' ';
	}
}
void hesapOzeti(int mS, int hS){//mS indisli müşterinin hS indisli hesabının hesap özeti işlemleri
	FILE *pf;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);//güncel zaman alınır
	int i, j, k, n, t1, t2, t3, iS, aralik, ihNo, imS, iA, iY, sorgu, kontrol;
	double kesinti;
	char c;
	iS = ((aBank.musteri+mS)->hesap+hS)->islemSayisi;
	t1 = ((((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Yil - 1900) * 12 + (((aBank.musteri+mS)->hesap+hS)->islem)->tarih.Ay;//toplam ay sayısı ilk işlem için (aralık hesabı için)
	t2 = tm.tm_year * 12 + tm.tm_mon+1 ;//güncel tarih toplam ay sayısı (aralık hesabı için)
	aralik = t2-t1;//ilk işlem ile güncel tarih arasında kaç ay olduğunu bulmak için
	int index[aralik+1];//menüdeki indislere işlemin ay sırasını ekler (işlem yapılan ayların indislerini kaydetmek için)
	
	for (i=0; i<aralik+1; i++){//bütün işlem kayıtlarını aylara ayırmak için (hiç işlem yapılmayan aylarında indisi olur)
		*(index+i) = -1;//sıfırlama işlemi
		(dekont+i)->islemSayisi = 0;
		((dekont+i)->islem)->Tutar = 0;
		for (j=iS-1; j>=0; j--){//ay aralığındaki işlemleri listeler
			t3 = ((((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih.Yil - 1900) * 12 + (((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih.Ay;//işlemin yapıldıgı tarihteki toplam ay sayısı
			if ((t2-t3) == i){//ay aralığı ay farkına eşitse işlem yapar 
				ihNo = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iHesap;//işlemin yapıldığı hesap noyu alır
				imS = hNoKontrol(ihNo, 1);//işlemin yapıldıgı müşteri indisini alır
				strcpy(((dekont+i)->islem+(dekont+i)->islemSayisi)->Ad, (aBank.musteri+imS)->Ad);//işlemin yapıldığı kişinin adını kaydeder(Havale işlem kaydı için)
				isimDuzelt(((dekont+i)->islem+(dekont+i)->islemSayisi)->Ad);
				((dekont+i)->islem+(dekont+i)->islemSayisi)->hesapNo = ihNo;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->mTuru = (aBank.musteri+mS)->mTuru;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->iTuru = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iTuru;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->Tutar = (((aBank.musteri+mS)->hesap+hS)->islem+j)->iTutar;
				((dekont+i)->islem+(dekont+i)->islemSayisi)->tarih = (((aBank.musteri+mS)->hesap+hS)->islem+j)->tarih;
				(dekont+i)->islemSayisi++;//her işlemde işlem sayısını 1 arttırır
			}
		}
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	k=0;
	for (i=0; i<aralik+1; i++){
		if (((dekont+i)->islem)->Tutar != 0){//ay indisinde işlem yapıldı ise
			iY = (t2 - i) / 12 + 1900;
			iA = (t2 - i -1) % 12 +1; 
			n = i-k+1;//menüdeki indis numarası hesaplanır
			if (i==0) printf("%2d-)\t%02d - %02d\t", n, iY, iA);
			else printf("%2d-)\t%d - %02d/%02d\t", n, iY, iA, iA%12+1);
			if (n%2 == 0) printf("\n");
			*(index+n) = i;//menüdeki indis sırasından dekont structındaki ay indisini elde edebilmek için
		}else{//ay indisinde işlem yapılmadı ise k arttırılır (menüde gösterilen tarih aralıkları sıralı olsun diye)
			k++;
		}
	}
	printf("\n\nSecim (Iptal etmek icin 0 giriniz): ");
	do{
		strAl(temp, 1, 2);
		t = strlen(temp);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if (sorgu == 0 && kontrol == 1 && t == 1) hesapIslem(mS, hS);
		if(sorgu<0 || sorgu>n || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
			kontrol = 0;
		}
	}while(kontrol != 1);
	
	fclose(fopen("dekont.txt", "w"));
	if ((pf = fopen("dekont.txt", "a")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	i = *(index + sorgu);//ay indisi elde edilir
	iY = (t2 - i) / 12 + 1900;
	iA = (t2 - i -1) % 12 +1; 
	if (i==0) fprintf(pf, "(%02d - %02d)  Donemi hesap ozeti", iY, iA);
	else fprintf(pf, "(%d - %02d/%02d)  Donemi hesap ozeti", iY, iA, iA%12+1);
	fprintf(pf, "  (%d)\n\n", ((aBank.musteri+mS)->hesap+hS)->hesapNo);
	for (j=(dekont+i)->islemSayisi-1; j>=0; j--){//ilk başa en yeni işlem gelsin diye tersten listelendi(dekont yazdırma işlemi)
		if (((dekont+i)->islem+j)->iTuru == 1){//para çekme işlemi ise
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para cekme    %.2lf\n", 
				((dekont+i)->islem+j)->tarih.Gun,
				((dekont+i)->islem+j)->tarih.Ay,
				((dekont+i)->islem+j)->tarih.Yil,
				((dekont+i)->islem+j)->tarih.Saat,
				((dekont+i)->islem+j)->tarih.Dk,
				-((dekont+i)->islem+j)->Tutar);
		}else if (((dekont+i)->islem+j)->iTuru == 2){//para yatırma işlemi ise
			fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Para yatirma  %.2lf\n", 
				((dekont+i)->islem+j)->tarih.Gun,
				((dekont+i)->islem+j)->tarih.Ay,
				((dekont+i)->islem+j)->tarih.Yil,
				((dekont+i)->islem+j)->tarih.Saat,
				((dekont+i)->islem+j)->tarih.Dk,
				((dekont+i)->islem+j)->Tutar);
		}else if (((dekont+i)->islem+j)->iTuru == 3){//havale işlemi ise
			if (((dekont+i)->islem+j)->Tutar<0){//havale göderildi ise
				if (((dekont+i)->islem+j)->mTuru == 1){//bireysel müşteri ise
					kesinti = ((int)(-((dekont+i)->islem+j)->Tutar*2)) / 100.0;
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden havale  %.2lf (-%.2lf)\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar,
						kesinti);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}else{//ticari müşteri ise
					fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Giden havale  %.2lf\n", 
						((dekont+i)->islem+j)->tarih.Gun,
						((dekont+i)->islem+j)->tarih.Ay,
						((dekont+i)->islem+j)->tarih.Yil,
						((dekont+i)->islem+j)->tarih.Saat,
						((dekont+i)->islem+j)->tarih.Dk,
						-((dekont+i)->islem+j)->Tutar);
					fprintf(pf, "\tGonderilen kisi:  %s (%d)\n", ((dekont+i)->islem+j)->Ad, ((dekont+i)->islem+j)->hesapNo);
				}
			}else if (((dekont+i)->islem+j)->Tutar>0){//havale alındı ise
				fprintf(pf, "%02d.%02d.%04d - %02d:%02d  Gelen havale  %.2lf\n", 
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
	if ((pf = fopen("dekont.txt", "r")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	system("@cls||clear");
	printf(".............aBank.............\n");
	while ((c=fgetc(pf))!=EOF){//işlem kaydı dekont.txt den alınır ve yazdırılır
		printf("%c", c);
	}
	fclose(pf);
	printf("\n1-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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

void bankaRapor(int mS){//gelir gider raporu goruntülemek içim
	FILE *pf;
	int sorgu, kontrol;
	char c;
	Guncelle();
	system("@cls||clear");
	printf(".............aBank.............\n");
	if ((pf = fopen("rapor.txt", "r")) == NULL){
		printf("Dosya acma hatasi!\n");
		exit(1);
	}
	while((c = fgetc(pf)) != EOF){
		printf("%c", c);
	}
	printf("\n\n1-)\tGeri don\n0-)\tAna menu\nSecim: ");
	do{
		strAl(temp, 1, 1);
		kontrol = sscanf(temp, "%d%c", &sorgu, &c);
		if(sorgu<0 || sorgu>1 || kontrol != 1) {
			printf("Hatali giris!\nTekrar deneyiniz: ");
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

void islemKaydi(int mS, int hS, int iT, int iH, double iTutar){//iT -> işlem türü, iH -> işlemin yapıldıgı hesapno, iTutar ->işlem tutarı
	int iS, mTuru;
	double kesinti = 0;
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
	if (iT == 1){//işlem tutarları gelir gider raporu için structa kaydedilir
		aBank.tGiden -= iTutar;
	}else if (iT == 2){
		aBank.tGelen += iTutar;
	}else if (iT == 3 && mTuru == 1 && iTutar<0){
		kesinti = ((int)(-iTutar*2)) / 100.0;
		aBank.tKar += kesinti;
	}
}

double cekilenPara(int mS){//mS indisli müşterinin gün içinde çektiği para miktarını hesaplar
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