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
void HesapIslem();
void Guncelle();
void VeriAl();
int HesapNoOlustur();
int tcNoKontrol(double tcNo);
int hNoKontrol(int hesapNo, int n);
char *isimDuzelt(char isim[50]);
char *sifrele(char sifre[50]);

int main(){
	VeriAl();
	//AnaMenu();
}

void AnaMenu(){
	VeriAl();
	int sorgu;
	printf(".............aBank.............\n");
	printf("1-)\tHesap Acma\n");
	printf("2-)\tHesap Islemleri\n");
	printf("Secim: ");
	do{
		scanf("%d", &sorgu);
		if(sorgu<1 || sorgu>2) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2);
	system("@cls||clear");
	switch (sorgu){
		case 1:{
			YeniMusteri();
		}break;
		case 2:{
			HesapIslem();
		}break;
	}
}

void HesapIslem(){
	int sonuc, sorgu;
	double dTemp;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc Numaranizi Giriniz: ");
	do{
		scanf("%lf", &dTemp);
		sonuc = tcNoKontrol(dTemp);
		if (!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
	}while(!(dTemp>10000000000 && dTemp<99999999999 && sonuc != -1));
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Hosgeldin, %s\n", isimDuzelt(aBank.musteri[sonuc].Ad));
	printf("1-)\tPara Cekme\n2-)\tPara Yatırma\n3-)\tHavale Hesabi Ekle\n4-)\tHavale Gonder\nSecim: ");
	do{
		scanf("%d", &sorgu);
		if(sorgu<1 || sorgu>4) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>4);
	printf("%d\n", sorgu);
}


void YeniMusteri(){
	int sorgu, t;
	double dTemp;
	char cTemp;
	printf(".............aBank.............\n");
	printf("1-)\tBireysel Musteri\n");
	printf("2-)\tTicari Musteri\n");
	printf("Secim: ");
	do{
		scanf("%d", &sorgu);
		if(sorgu<1 || sorgu>2) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2);
	aBank.musteri[aBank.mSayisi].mTuru = sorgu;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Ad Soyad: ");
	scanf("%s", aBank.musteri[aBank.mSayisi].Ad);
	t = strlen(aBank.musteri[aBank.mSayisi].Ad);
	do{
		scanf("%c", &cTemp);
		if (cTemp == ' ') cTemp = '-';
		aBank.musteri[aBank.mSayisi].Ad[t++] = cTemp;
	}while (cTemp != '\n');
	aBank.musteri[aBank.mSayisi].Ad[t-1] = 0;
	system("@cls||clear");
	printf(".............aBank.............\n");
	printf("Tc Numaranizi Giriniz: ");
	do{
		scanf("%lf", &dTemp);
		if (!(dTemp>10000000000&&dTemp<99999999999)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
		else if (tcNoKontrol(dTemp) != -1) printf("Bu Tc Numarasi Kayitli!\nTekrar Deneyiniz: ");
	}while(!(dTemp>10000000000&&dTemp<99999999999&&tcNoKontrol(dTemp) == -1));
	aBank.musteri[aBank.mSayisi].tcNo = dTemp;

	aBank.musteri[aBank.mSayisi].hesapSayisi = 1;
	aBank.musteri[aBank.mSayisi].tHesapSayisi = 0;
	aBank.musteri[aBank.mSayisi].hesap[0].hesapNo = HesapNoOlustur();
	aBank.musteri[aBank.mSayisi].hesap[0].bakiye = 0;
	aBank.musteri[aBank.mSayisi].hesap[0].islemSayisi = 0;
	aBank.mSayisi++;
	Guncelle();

	system("@cls||clear");
	printf(".............aBank.............\n\n");
	printf("Hesabiniz kuruldu...\n");
	printf("Adiniz: %s\n", isimDuzelt(aBank.musteri[aBank.mSayisi-1].Ad));
	printf("Tc Numaraniz: %.lf\n", aBank.musteri[aBank.mSayisi-1].tcNo);
	printf("Hesap Numaraniz: %d\n\n", aBank.musteri[aBank.mSayisi-1].hesap[0].hesapNo);
	printf("1-)\tAna Menu\n2-)\tCikis\nSecim: ");
	do{
		scanf("%d", &sorgu);
		if(sorgu<1 || sorgu>2) {
			printf("Hatali Giris!\nTekrar Deneyiniz: ");
		}
	}while(sorgu<1 || sorgu>2);
	if (sorgu == 1){
		system("@cls||clear");
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
			printf("%s\n", aBank.musteri[mNo-1].Sifre);
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
}

void Guncelle(){
	int i, j, k, b=0, t=0;
	FILE *pf1, *pf2;
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].mTuru == 1){
			if (b==0) fclose(fopen("bireyselMusteri.txt", "w"));
			pf1 = fopen("bireyselMusteri.txt", "a");
			if (b!=0) fprintf(pf1, "\n");
			fprintf(pf1, "Musteri: %d", i+1);
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
			fprintf(pf2, "Musteri: %d", i+1);
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

int HesapNoOlustur(){
	int hesapNo;
	srand(time(NULL));
	do{
		hesapNo = rand()%899999999 + 100000000;
		
	}while(hNoKontrol(hesapNo, 1)!=-1);
	return hesapNo;
}
char *sifrele(char sifre[50]){
	int i;
	char secret[8] = { 22, 53, 44, 71, 66, 95, 39, 122};
	for(i = 0; i < (int)strlen(sifre); i++)
		sifre[i] ^= secret[i%8];
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
		if (isim[i] == '-') isim[i] = ' ';
	}
	return isim;
}

void tarihEkle(int mSira, int hSira){

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	//düzenlenecek
}