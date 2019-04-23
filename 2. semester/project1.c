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
	Islem islem[100];
}Hesap;

typedef struct{
	int hesapNo;
	double tcNo;
	char Ad[50];
}transferHesap;

typedef struct{

	Hesap hesap[20];
	transferHesap tHesap[20];
	char Ad[50];
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
void Guncelle();
void VeriAl();
void HesapNoOlustur();

int main(){
	aBank.mSayisi=0;
	int sorgu;
	printf(".............aBank.............\n");
	printf("1-)	Hesap Açma\n");
	printf("2-)	Hesap Islemleri\n");
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
			int t;
			char temp;
			printf(".............aBank.............\n");
			printf("Ad Soyad: ");
			scanf("%s", aBank.musteri[aBank.mSayisi].Ad);
			t = strlen(aBank.musteri[aBank.mSayisi].Ad);
			do{
				scanf("%c", &temp);
				if (temp == ' ') temp = '-';
				aBank.musteri[aBank.mSayisi].Ad[t++] = temp;
			}while (temp != '\n');
			aBank.musteri[aBank.mSayisi].Ad[t-1] = 0;
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Tc Numaranizi Giriniz: ");
			do{
				scanf("%lf", &aBank.musteri[aBank.mSayisi].tcNo);
				if (!(aBank.musteri[aBank.mSayisi].tcNo>10000000000&&aBank.musteri[aBank.mSayisi].tcNo<99999999999)) printf("Hatali Giris!\nTekrar Deneyiniz: ");
			}while(!(aBank.musteri[aBank.mSayisi].tcNo>10000000000&&aBank.musteri[aBank.mSayisi].tcNo<99999999999));
			system("@cls||clear");
			printf(".............aBank.............\n");
			printf("Hesabiniz kuruldu\n1-)	Ana Menu\n2-)	Cikis\nSecim: ");
			do{
				scanf("%d", &sorgu);
				if(sorgu<1 || sorgu>2) {
					printf("Hatali Giris!\nTekrar Deneyiniz: ");
				}
			}while(sorgu<1 || sorgu>2);
			if (sorgu == 1){
				system("@cls||clear");
				main();
			}
			else if (sorgu == 2) exit(1);
		}break;
		case 2:{
			
		}break;
	}

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
	int i, j, mNo, kontrol;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		kontrol = fscanf(pf, " Ad Soyad: %s", aBank.musteri[mNo-1].Ad);
		fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
			fscanf(pf, " Islem Sayisi: %d", &aBank.musteri[mNo-1].hesap[i].islemSayisi);
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
		fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &aBank.musteri[mNo-1].tHesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].tHesapSayisi; i++){
			fscanf(pf, " Hesap %*d: %d", &aBank.musteri[mNo-1].tHesap[i].hesapNo);
			fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tHesap[i].tcNo);
		}
		aBank.musteri[mNo-1].mTuru = 1;
		if (kontrol == 1) aBank.mSayisi++;
	}
	fclose(pf);
	pf = fopen("ticariMusteri.txt", "r");
	while(!feof(pf)){
		fscanf(pf, " Musteri: %d", &mNo);
		kontrol = fscanf(pf, " Ad Soyad: %s", aBank.musteri[mNo-1].Ad);
		fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tcNo);
		fscanf(pf, " Hesap Sayisi: %d", &aBank.musteri[mNo-1].hesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].hesapSayisi; i++){
			fscanf(pf, " Hesap %*d : %d", &aBank.musteri[mNo-1].hesap[i].hesapNo);
			fscanf(pf, " Bakiye: %lf", &aBank.musteri[mNo-1].hesap[i].bakiye);
			fscanf(pf, " Islem Sayisi: %d", &aBank.musteri[mNo-1].hesap[i].islemSayisi);
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
		fscanf(pf, " Kayitli Havale Hesap Sayisi: %d", &aBank.musteri[mNo-1].tHesapSayisi);
		for (i=0; i<aBank.musteri[mNo-1].tHesapSayisi; i++){
			fscanf(pf, " Hesap %*d: %d", &aBank.musteri[mNo-1].tHesap[i].hesapNo);
			fscanf(pf, " Tc-No: %lf", &aBank.musteri[mNo-1].tHesap[i].tcNo);
		}
		aBank.musteri[mNo-1].mTuru = 2;
		if (kontrol == 1) aBank.mSayisi++;
	}
	fclose(pf);
	printf("%.2lf\n", aBank.musteri[3].hesap[0].islem[0].iTutar);
}

void Guncelle(){
	int i, j, k, b=0, t=0;
	FILE *pf1, *pf2;
	fclose(fopen("bireyselMusteri.txt", "w"));
	fclose(fopen("ticariMusteri.txt", "w"));
	pf1 = fopen("bireyselMusteri.txt", "a");
	pf2 = fopen("ticariMusteri.txt", "a");
	for (i=0; i<aBank.mSayisi; i++){
		if (aBank.musteri[i].mTuru == 1){
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
			b=1;
		}else if (aBank.musteri[i].mTuru == 2){
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
			t=1;
		}
	}

}