#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	float ay_br_as_uc = 1777.50, ay_br_uc, ay_tp_br_uc, ge_vergi, ge_ve_or, ay_net_uc, i;
	int ba_co_sayi, ba_co_sayi_6, en_oran, en_derece, en_ve_in, es_yr_oden, co_yr_oden, t;
	int b_200, b_100, b_50, b_20, b_10, b_5, m_100, m_50, m_25, m_10, m_5, m_1;
	char md_durum, es_cl_durum, en_durum, devam, temp, ad[50];
	double tc_no;

	float tum_ay_net_uc=0, tum_ge_vergi=0, tum_ay_tp_br_uc=0, ba_co_ort;
	int cal_sayi=0, cal_sayi_2000=0, ge_ve_15=0, ge_ve_20=0, ge_ve_27=0, ge_ve_35=0;
	int tb_200=0, tb_100=0, tb_50=0, tb_20=0, tb_10=0, tb_5=0, tm_100=0, tm_50=0, tm_25=0, tm_10=0, tm_5=0, tm_1=0;
	int evli_sayi=0, es_ca_sayi=0, co_ba_sayi=0, co_ba_sayi_3=0, tum_co_sayi=0, en_sayi=0;
	float max_br_br=0, max_br_ve=0, max_br_ne=0, max_ne_br=0, max_ne_ve=0, max_ne_ne=0;
	char max_br_ad[50], max_ne_ad[50];
	double max_br_tc, max_ne_tc;
	printf("*****Maas Hesaplama Programi*****\n");
	do
	{
		ba_co_sayi=0;
		ba_co_sayi_6=0;
		en_ve_in=0;
		es_yr_oden=0;
		co_yr_oden=0;
		b_200=0;
		b_100=0;
		b_50=0;
		b_20=0;
		b_10=0;
		b_5=0;
		m_100=0;
		m_50=0;
		m_25=0;
		m_10=0;
		m_5=0;
		m_1=0;

		printf("TC Kimlik Numarasi Giriniz: ");
		do{
			scanf("%lf", &tc_no);
			if (!(tc_no>10000000000&&tc_no<99999999999)) printf("Hatali Giris Tekrar Deneyin: \a");
		}while(!(tc_no>10000000000&&tc_no<99999999999));
		printf("Ad ve Soyad Giriniz: ");

		scanf("%s", ad);
		t = strlen(ad);
		do
		{
			scanf("%c", &temp);
			ad[t++] = temp;
		} while (temp != '\n');
		ad[t - 1] = 0; 

		printf("Aylik Brut Ucretini Giriniz (Minimum %.2f TL): ", ay_br_as_uc);
		do{
			scanf("%f", &ay_br_uc);
			if (ay_br_uc<ay_br_as_uc) printf("Hatali Giris Tekrar Deneyin: \a");
		}while(ay_br_uc<ay_br_as_uc);

		printf("Medeni Durumu Giriniz (e/b): ");
		do{
			scanf("%s",&md_durum);
			if (md_durum!='E'&&md_durum!='e'&&md_durum!='B'&&md_durum!='b') printf("Hatali Giris Tekrar Deneyin: \a");
		}while(md_durum!='E'&&md_durum!='e'&&md_durum!='B'&&md_durum!='b');
		
		if (md_durum=='E'||md_durum=='e')
		{
			printf("Esinin Calisma Durumunu Giriniz (e/h): ");
			do{
				scanf("%s", &es_cl_durum);
				if (es_cl_durum!='E'&&es_cl_durum!='e'&&es_cl_durum!='H'&&es_cl_durum!='h') printf("Hatali Giris Tekrar Deneyin: \a");
			}while(es_cl_durum!='E'&&es_cl_durum!='e'&&es_cl_durum!='H'&&es_cl_durum!='h');
			evli_sayi+=1;
			es_yr_oden=220;
			if(es_cl_durum=='E'||es_cl_durum=='e'){
				es_ca_sayi+=1;
				es_yr_oden=0;
			}
		}

		printf("Bakmakla Yukumlu Oldugu Cocuk Sayisini Giriniz: ");
		do{
			scanf("%d", &ba_co_sayi);
			if (ba_co_sayi<0) printf("Hatali Giris Tekrar Deneyin: \a");
		}while(ba_co_sayi<0);

		if (ba_co_sayi>0)
		{
			printf("Yasi 6'dan Buyuk Olan Cocuk Sayisini Giriniz: ");
			do{
				scanf("%d", &ba_co_sayi_6);
				if (ba_co_sayi_6<0||ba_co_sayi_6>ba_co_sayi) printf("Hatali Giris Tekrar Deneyin: \a");
			}while(ba_co_sayi_6<0||ba_co_sayi_6>ba_co_sayi);
			co_ba_sayi+=1;
			tum_co_sayi+=ba_co_sayi;
			if (ba_co_sayi>3) co_ba_sayi_3+=1;
		}

		printf("Engelli Durumunu Giriniz (e/h): ");
		do{
			scanf("%s", &en_durum);
			if (en_durum!='E'&&en_durum!='e'&&en_durum!='H'&&en_durum!='h') printf("Hatali Giris Tekrar Deneyin: \a");
		}while(en_durum!='E'&&en_durum!='e'&&en_durum!='H'&&en_durum!='h');
		
		if (en_durum=='E'||en_durum=='e')
		{
			printf("Engellilik Oranini Giriniz (1-100): ");
			do{
				scanf("%d", &en_oran);
				if (en_oran<1||en_oran>100) printf("Hatali Giris Tekrar Deneyin: \a");
			}while(en_oran<1||en_oran>100);
		}

		printf("\nTc No: %.0lf\nIsim Soyisim: %s\n", tc_no, ad);
		printf("Aylik Brut Ucret: %.2f TL\n",ay_br_uc);
		printf("Es Icin Aile Yardim Odenegi: %d TL\n", es_yr_oden);

		if (ba_co_sayi>0)
		{
			co_yr_oden=(ba_co_sayi - ba_co_sayi_6)*25+ba_co_sayi_6*45;
		}
		printf("Cocuk Icin Aile Yardim Odenegi: %d TL\n", co_yr_oden);

		ay_tp_br_uc=ay_br_uc+es_yr_oden+co_yr_oden;
		printf("Aylik Toplam Brut Ucret: %.2f TL\n", ay_tp_br_uc);
		
		if (ay_tp_br_uc<2000)
		{
			ge_ve_or=0.15;
			ge_ve_15+=1;
		}else if(ay_tp_br_uc<5000)
		{
			ge_ve_or=0.20;
			ge_ve_20+=1;
		}else if(ay_tp_br_uc<10000)
		{
			ge_ve_or=0.27;
			ge_ve_27+=1;
		}else
		{
			ge_ve_or=0.35;
			ge_ve_35+=1;
		}

		if(en_oran>=40 && en_oran<60) 
		{
			en_ve_in=210;
			en_derece=3;
			en_sayi+=1;
		}
		else if(en_oran>=60 && en_oran<80)
		{
			en_ve_in=470;
			en_derece=2;
			en_sayi+=1;
		}
		else if(en_oran>=80 && en_oran<=100)
		{
			en_ve_in=900;
			en_derece=1;
			en_sayi+=1;
		}

		ge_vergi=(ay_tp_br_uc - en_ve_in)*ge_ve_or;
		printf("Gelir Vergisi Kesintisi: %.2f\n", ge_vergi);

		if (en_ve_in!=0) printf("Engel Derecesi: %d\n", en_derece);

		ay_net_uc=ay_tp_br_uc - ge_vergi;

		i=ay_net_uc;
		while(i>=200){ i-=200; b_200+=1; }
		while(i>=100){ i-=100; b_100+=1; }
		while(i>=50){ i-=50; b_50+=1; }
		while(i>=20){ i-=20; b_20+=1; }
		while(i>=10){ i-=10; b_10+=1; }
		while(i>=5){ i-=5; b_5+=1; }
		while(i>=1){ i-=1; m_100+=1; }
		while(i>=0.5){ i-=0.5; m_50+=1; }
		while(i>=0.25){ i-=0.25; m_25+=1; }
		while(i>=0.1){ i-=0.1; m_10+=1; }
		while(i>=0.05){ i-=0.05; m_5+=1; }
		while(i>=0.01){ i-=0.01; m_1+=1; }
		ay_net_uc=ay_net_uc-i;

		printf("Aylik Net Ucret: %.2f\n", ay_net_uc);

		if(b_200!=0) printf("%d Adet 200 TL", b_200);
		if(b_100!=0) printf(",\n%d Adet 100 TL", b_100);
		if(b_50!=0) printf(",\n%d Adet 50 TL", b_50);
		if(b_20!=0) printf(",\n%d Adet 20 TL", b_20);
		if(b_10!=0) printf(",\n%d Adet 10 TL", b_10);
		if(b_5!=0) printf(",\n%d Adet 5 TL", b_5);
		if(m_100!=0) printf(",\n%d Adet 1 TL", m_100);
		if(m_50!=0) printf(",\n%d Adet 50 kurus", m_50);
		if(m_25!=0) printf(",\n%d Adet 25 kurus", m_25);
		if(m_10!=0) printf(",\n%d Adet 10 kurus", m_10);
		if(m_5!=0) printf(",\n%d Adet 5 kurus", m_5);
		if(m_1!=0) printf(",\n%d Adet 1 kurus", m_1);


		tum_ay_tp_br_uc+=ay_tp_br_uc;
		tum_ay_net_uc+=ay_net_uc;
		tum_ge_vergi+=ge_vergi;

		cal_sayi+=1;
		if(ay_net_uc<2000) cal_sayi_2000+=1;

		tb_200+=b_200;
		tb_100+=b_100;
		tb_50+=b_50;
		tb_20+=b_20;
		tb_10+=b_10;
		tb_5+=b_5;
		tm_100+=m_100;
		tm_50+=m_50;
		tm_25+=m_25;
		tm_10+=m_10;
		tm_5+=m_5;
		tm_1+=m_1;

		if(max_br_br<ay_tp_br_uc)
		{
			max_br_br=ay_tp_br_uc;
			max_br_ne=ay_net_uc;
			max_br_ve=ge_vergi;
			strcpy(max_br_ad, ad);
			max_br_tc=tc_no;
		}

		if(max_ne_ne<ay_net_uc)
		{
			max_ne_br=ay_tp_br_uc;
			max_ne_ne=ay_net_uc;
			max_ne_ve=ge_vergi;
			strcpy(max_ne_ad, ad);
			max_ne_tc=tc_no;
		}

		printf("\n\nBaska Calisan Var mi?(e/h): ");
		do{
			scanf("%s",&devam);
			if (devam!='E'&&devam!='e'&&devam!='H'&&devam!='h') printf("Hatali Giris Tekrar Deneyin: \a");
		}while(devam!='E'&&devam!='e'&&devam!='H'&&devam!='h');
	}while(devam=='E'||devam=='e');

	printf("\nToplam Odenen Ucret: %.2f Toplam Odenen Vergi: %.2f\n", tum_ay_net_uc, tum_ge_vergi);

	printf("Brut Ucretlerinin Ortalamasi: %.2f Net Ucretlerinin Ortalamasi: %.2f\n\n", tum_ay_tp_br_uc/cal_sayi, tum_ay_net_uc/cal_sayi);

	if(tb_200!=0) printf("%d Adet 200 TL", tb_200);
	if(tb_100!=0) printf(",\n%d Adet 100 TL", tb_100);
	if(tb_50!=0) printf(",\n%d Adet 50 TL", tb_50);
	if(tb_20!=0) printf(",\n%d Adet 20 TL", tb_20);
	if(tb_10!=0) printf(",\n%d Adet 10 TL", tb_10);
	if(tb_5!=0) printf(",\n%d Adet 5 TL", tb_5);
	if(tm_100!=0) printf(",\n%d Adet 1 TL", tm_100);
	if(tm_50!=0) printf(",\n%d Adet 50 kurus", tm_50);
	if(tm_25!=0) printf(",\n%d Adet 25 kurus", tm_25);
	if(tm_10!=0) printf(",\n%d Adet 10 kurus", tm_10);
	if(tm_5!=0) printf(",\n%d Adet 5 kurus", tm_5);
	if(tm_1!=0) printf(",\n%d Adet 1 kurus", tm_1);

	printf("\n\n2000 TL'den Az Net Ucret Alan Sayisi: %d\n\n", cal_sayi_2000);

	printf("15 Vergi Veren Calisan Sayisi: %d Yuzdesi %%%.2f\n", ge_ve_15, (float)(ge_ve_15*100)/cal_sayi);
	printf("20 Vergi Veren Calisan Sayisi: %d Yuzdesi %%%.2f\n", ge_ve_20, (float)(ge_ve_20*100)/cal_sayi);
	printf("27 Vergi Veren Calisan Sayisi: %d Yuzdesi %%%.2f\n", ge_ve_27, (float)(ge_ve_27*100)/cal_sayi);
	printf("35 Vergi Veren Calisan Sayisi: %d Yuzdesi %%%.2f\n", ge_ve_35, (float)(ge_ve_35*100)/cal_sayi);

	printf("\nAylik Toplam Brut Ucreti En Cok Olan Calisanin...\n");
	printf("Tc Nosu: %.0lf\n", max_br_tc);
	printf("Ad Soyadi: %s\n", max_br_ad);
	printf("Aylik Toplam Brut Ucreti: %.2f\n", max_br_br);
	printf("Gelir Vergisi Kesintisi: %.2f\n", max_br_ve);
	printf("Aylik Net Ucreti: %.2f\n", max_br_ne);

	printf("\nAylik Net Ucreti En Cok Olan Calisanin...\n");
	printf("Tc Nosu: %.0lf\n", max_ne_tc);
	printf("Ad Soyadi: %s\n", max_ne_ad);
	printf("Aylik Toplam Brut Ucreti: %.2f\n", max_ne_br);
	printf("Gelir Vergisi Kesintisi: %.2f\n", max_ne_ve);
	printf("Aylik Net Ucreti: %.2f\n\n", max_ne_ne);

	printf("Evli Calisan Yuzdesi: %%%.2f Bekar Calisan Yuzdesi: %%%.2f\n", (float)(evli_sayi*100)/cal_sayi, (float)((cal_sayi-evli_sayi)*100)/cal_sayi);

	if (evli_sayi==0) evli_sayi=1;
	printf("Evli Olanlarin Icinde Esleri Calisanlarin Yuzdesi: %%%.2f\n", (float)(es_ca_sayi*100)/evli_sayi);

	if (co_ba_sayi==0) co_ba_sayi=1;
	ba_co_ort=(float)tum_co_sayi/co_ba_sayi;
	printf("Bakmakla Yukumlu Olunan Cocuk Sayisi Ortalamasi: %.2f\n", ba_co_ort);

	printf("Cocuk Sayisi 3'ten Fazla Olan Calisan Sayisi: %d\n", co_ba_sayi_3);

	printf("Engelli Calisan Sayisi: %d Engelli Calisan Ortalamasi: %%%.2f\n", en_sayi, (float)(en_sayi*100)/cal_sayi);


	return 0;
}
