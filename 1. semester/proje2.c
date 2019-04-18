#include <stdio.h>
#include <stdlib.h>

void sirala(int x[], int y[], char a[], int n);
void f_sifir(float dizi[], int n);
void i_sifir(int dizi[], int n);
void c_kopya(char asil[], char kopya[], int n);
void f_kopya(float asil[], float kopya[], int n);
void i_kopya(int asil[], int kopya[], int n);
void yuzde(float yuzde[], int parti_oy[], int toplam_oy, int n);

int main(){
	int p_sayi, i, j, b, b_oy, il_plaka, m_kon;
	char a[26]="ABCDEFGHIJKLMNOPQRSTUVXYZ";
	
	printf("Secime katilacak parti sayisini giriniz: ");
	do{
		scanf("%d", &p_sayi);
		if (!(p_sayi>1&&p_sayi<=25)) printf("Hatali Giris Tekrar Deneyin: \a");
	}while(!(p_sayi>1&&p_sayi<=25));

	int top_il_oy=0, top_kon=0, il_sayi=0, top_oy[p_sayi], top_mv[p_sayi], top_il_1[p_sayi], top_mv_0[p_sayi];
	float top_yuz[p_sayi];

	f_sifir(top_yuz, p_sayi);
	i_sifir(top_oy, p_sayi);
	i_sifir(top_mv, p_sayi);
	i_sifir(top_il_1, p_sayi);
	i_sifir(top_mv_0, p_sayi);

	do{
		float temp_yuz[p_sayi];
		char temp_a[p_sayi];
		int il_oy=0, oy[p_sayi], m_sayi[p_sayi], temp_oy[p_sayi], temp_m_sayi[p_sayi];
		printf("Ilin plaka kodunu giriniz");
		if(il_sayi!=0) printf(" (Sonlandirmak icin 0 giriniz)");
		printf(": ");
		do{
			scanf("%d", &il_plaka);
			if (!(il_plaka>=0&&il_plaka<=81)) printf("Hatali Giris Tekrar Deneyin: \a");
		}while(!(il_plaka>=0&&il_plaka<=81));

		if(il_plaka==0) break;

		printf("Ilin milletvekili kontejanini giriniz: ");
		do{
			scanf("%d", &m_kon);
			if (!(m_kon>0&&m_kon<=50)) printf("Hatali Giris Tekrar Deneyin: \a");
		}while(!(m_kon>0&&m_kon<=50));

		for(i=0; i<p_sayi; i++){
			printf("%c Partisi'nin oy sayisini giriniz: ", a[i]);
			do{
				scanf("%d", &oy[i]);
				if (!(oy[i]>=0&&oy[i]<100000000)) printf("Hatali Giris Tekrar Deneyin: \a");
			}while(!(oy[i]>=0&&oy[i]<100000000));
			il_oy+=oy[i];
		}

		i_kopya(oy, temp_oy, p_sayi);
		i_sifir(m_sayi, p_sayi);

		//milletvekili hesabi
		j=m_kon;
		b_oy=0;
		while(j>0){
			for(i=0; i<p_sayi; i++){
				if (temp_oy[i]>b_oy){
					b_oy=temp_oy[i];
					b=i;
				}
			}
			temp_oy[b]=temp_oy[b]/2;
			b_oy=temp_oy[b];
			m_sayi[b]+=1;
			j--;
		}

		b_oy=0;
		for (i=0; i<p_sayi; i++){
			//turkiye geneli toplam oy millet vekili ve yüzdeleri toplamak icın
			top_oy[i]+=oy[i];
			top_mv[i]+=m_sayi[i];
			//en büyük oy alanı bulmak için
			if(oy[i]>b_oy){
				b_oy=oy[i];
				b=i;
			}
			//millet vekili olmayan partiler için
			if(m_sayi[i]==0) top_mv_0[i]+=1;
		}
		top_il_1[b]+=1;

		i_kopya(oy, temp_oy, p_sayi);
		i_kopya(m_sayi, temp_m_sayi, p_sayi);
		c_kopya(a, temp_a, p_sayi);

		sirala(temp_oy, temp_m_sayi, temp_a, p_sayi);

		printf("\nIl Plaka Kodu: %d\n", il_plaka);
		printf("Milletvekili Kontejani: %d\n", m_kon);
		printf("Gecerli Oy Sayisi: %d\n\n", il_oy);
		printf("\t\t Oy Say  Oy Yuzde  MV Say\n");
		printf("\t\t ------  --------  ------\n");

		yuzde(temp_yuz, temp_oy, il_oy, p_sayi);
		for (i=0; i<p_sayi; i++){
			printf("%c Partisi  %10d   %6.2f     %3d\n", temp_a[i], temp_oy[i], temp_yuz[i], temp_m_sayi[i]);
		}
		printf("\n\n");
		top_il_oy+=il_oy;
		top_kon+=m_kon;
		il_sayi+=1;
		
	}while(il_sayi<10);
	printf("\nTurkiye Geneli\n");
	printf("Milletvekili Kontejani: %d\n", top_kon);
	printf("Gecerli Oy Sayisi: %d\n\n", top_il_oy);
	printf("\t\t Oy Say  Oy Yuzde  MV Say  MV Yuzde  Il 1. lik Say  Il 0 MV Say\n");
	printf("\t\t ------  --------  ------  --------  -------------  -----------\n");
	yuzde(top_yuz, top_oy, top_il_oy, p_sayi);
	for (i=0; i<p_sayi; i++){
		printf("%c Partisi   %10d   %6.2f     %3d    %6.2f %10d    %10d \n", a[i], top_oy[i], top_yuz[i], top_mv[i], (float)(top_mv[i]*100)/top_kon, top_il_1[i], top_mv_0[i]);
	}

	sirala(top_oy, top_mv, a, p_sayi);
	sirala(top_mv, top_oy, a, p_sayi);
	printf("\nIktidar Partisi: %c Partisi\n", a[0]);
	printf("Ana Muhalefet Partisi: %c Partisi\n", a[1]);
	
}

void sirala(int x[], int y[], char a[], int n){
	int i, j, temp;
	char temp_2;
	for (i=0; i<n-1; i++){
		for(j=0; j<n-i-1; j++)
		{
			if(x[j+1]>x[j])
			{
				temp=x[j+1];
				x[j+1]=x[j];
				x[j]=temp;

				temp_2=a[j+1];
				a[j+1]=a[j];
				a[j]=temp_2;

				temp=y[j+1];
				y[j+1]=y[j];
				y[j]=temp;
			}
		}
	}
}

void f_sifir(float dizi[], int n){
	int i;
	for (i=0; i<n; i++){
		dizi[i]=0;
	}
}

void i_sifir(int dizi[], int n){
	int i;
	for (i=0; i<n; i++){
		dizi[i]=0;
	}
}

void c_kopya(char asil[], char kopya[], int n){
	int i;
	for (i=0; i<n; i++){
		kopya[i]=asil[i];
	}
}

void f_kopya(float asil[], float kopya[], int n){
	int i;
	for (i=0; i<n; i++){
		kopya[i]=asil[i];
	}
}

void i_kopya(int asil[], int kopya[], int n){
	int i;
	for (i=0; i<n; i++){
		kopya[i]=asil[i];
	}
}

void yuzde(float yuzde[], int parti_oy[], int toplam_oy, int n){
	int i;
	for (i=0; i<n; i++){
		yuzde[i]=((float)parti_oy[i]/toplam_oy)*100;
	}
}