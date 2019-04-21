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
	int mTuru;
}Musteri;

typedef struct{

	Musteri musteri[1000];
	int mSayisi;
}Banka;

Banka baHa;

void VeriAl();
void HesapNoOlustur();

int main(){
	// int i;
	// srand(time(NULL));
	// for (i=0; i<100; i++){
	// 	HesapNoOlustur();
	// }
	VeriAl();
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
	
}

void VeriAl(){
	int t, i, mNo, hSayi, hNo;
	double b;
	FILE *pf;
	pf = fopen("bireyselMusteri.txt", "r");
	char temp[200], temp2[100];
	while(fgets(temp, 200, pf)!=NULL){
		if (sscanf(temp, "%s %d", temp2, &baHa.mSayisi)&&strstr(temp2, "Musteri:")){
			printf("%d\n", baHa.mSayisi);
		}
		if (strstr(temp, "Ad-Soyad: ")){
			for (i=14; i<strlen(temp); i++){
				baHa.musteri[baHa.mSayisi-1].Ad[i-14] = *(temp+i);
			}
		}
		if (sscanf(temp, "%s %lf", temp2, &baHa.musteri[baHa.mSayisi-1].tcNo)&&strstr(temp2, "Tc-No:")){
			printf("%.lf\n", baHa.musteri[baHa.mSayisi-1].tcNo);
		}
		if (strstr(temp, "Musteri-Turu: ")){
			if (strstr(temp, "Bireysel")){
				printf("bireysel\n");
			}else if(strstr(temp, "Ticari")){
				printf("ticari\n");
			}
		}
		if (sscanf(temp, "%s %d", temp2, &hSayi)&&strstr(temp2, "Hesap:")){
			printf("%d\n", hSayi);
		}
		if (sscanf(temp, "%s %d", temp2, &hNo)&&strstr(temp2, "Hesap-No:")){
			printf("%d\n", hNo);
		}
		if (sscanf(temp, "%s %lf", temp2, &b)&&strstr(temp2, "Bakiye:")){
			printf("%.2lf\n", b);
		}
	}
}

void Guncelle(){

}