#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#define SIZE 5

typedef struct
{
	char potrebitel[30];
	int num;
	int pass;
}DATA;

DATA *readInfo(char *filename,DATA *D,int *broi);
void writeInfo(char *filename,DATA *D,int *broi);
int reg(DATA *D,int *num,int *lm);
int login(DATA *D,int num);
int check(char *ime,DATA *D,int num);
int encrypt(char *pass);
int second_menu();

int main()
{
	DATA *DT;int limit=SIZE,a;
	FILE *f;
	int key,br=0,i;char filename[40];

	printf("Vavedi ime na fail s danni za potrebiteli:");
	gets(filename);
	DT=(DATA *)malloc(SIZE*sizeof(DATA));
	if(!DT)
	{
		printf("Error!\n");exit(1);
	}

	DT=readInfo(filename,DT,&br);
	for(i=0;i<br;i++)
	{
		printf("ptr:%s \n",DT[i].potrebitel);
		printf("pass:%d \n",DT[i].pass);
		printf("n:%d \n",DT[i].num);
	}
	printf("Natisnete klavish za prodyljenie!\n");getch();
	do
	{
		system("cls");
		printf("Izberete dejstvie:\n");
		printf("1 - Login\n");
		printf("2 - Registracq\n");
		printf("3 - Izhod\n");
		printf("Kod na operacia:");
		scanf("%d",&key);
		switch(key){
			case 1: if (login(DT,br))
					{printf("Uspeshno vlizane!\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					a=second_menu();}
					else {
					printf("Neuspeshno vlizane\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					}
					break;
			case 2: if (reg(DT,&br,&limit))
					{
					printf("Uspeshna registraciq!\n");
					writeInfo(filename,DT,&br);
					printf("Natisnete klavish za prodyljenie!\n");getch();
					}
					else
					{
					printf("Neuspeshna registraciq!\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					}
					break;
			case 3: writeInfo(filename,DT,&br);
					free(DT);
					printf("Pametta e osvobodena!\n");
					return 0;
			default:
				{
				printf("Nekorekten kod!\n");
				printf("Natisnete klavish za prodyljenie!\n");getch();
				}
		}
	}while(key!=3);
	return 0;
}
DATA *readInfo(char *filename,DATA *D,int *broi)
{
	DATA *D1=NULL;FILE *f;int i=0;int limit=SIZE;
	if(f=fopen(filename,"r"))
	{
		while(!feof(f))
		{
		if(i<limit)
		{
			fread(&D[i], sizeof(D[i]), 1, f );
			i++;
		}
		else
		{
			limit=limit+SIZE;
			D1=(DATA *)realloc(D,limit*sizeof(DATA));
			if(D1)
			{
				D=D1;
				D1=NULL;
				fread(&D[i], sizeof(D[i]), 1, f );
				i++;
			}
			else {printf("Niama pamet!\n");exit(1);}
		}
		*broi=i-1;
	}
	}else {
			printf("Greshka pri otvariane na fail %s!",filename);
			exit(1);
		 }
	fclose(f);
	printf("Uspeshno procheteni danni za %d potrebiteli!\n",*broi);
	printf("Natisnete klavish za prodyljenie!\n");
	getch();
	return D;
}
void writeInfo(char *filename,DATA *D,int *broi)
{
	FILE *f;int i;
	if(f=fopen(filename,"w"))
	{
		for(i=0;i<*broi;i++)
			fwrite(&D[i], sizeof(D[i]), 1, f );
	}
	else {
		printf("Greshka pri zapis na fail %s!",filename);
		exit(1);
	}
	fclose(f);
	printf("Uspeshno zapisani danni za %d potrebiteli!\n",*broi);
}
int check(char *ime,DATA *D,int num)
{ int flag=0,i;
	for(i=0;i<num;i++)
	   if(strcmp(D[i].potrebitel,ime)==0)
	     flag=1;
	if(flag)
	   return 0;
	else
	   return 1;
}
int encrypt(char *pass)
{
	char e_pass[30];int i;int sum=0;
	for(i = 0; (i < strlen(pass) && pass[i] != '\0'); i++)
        e_pass[i] = pass[i] + 3;
	for(i = 0; (i < strlen(pass) && pass[i] != '\0'); i++)
	   sum=sum+e_pass[i];
	return sum;
}
int reg(DATA *D,int *num,int *lm)
{   char un[30],ps[30];int ps1;DATA *D1=NULL;
	printf("User name:");fflush(stdin);
	gets(un);
	printf("Password:");fflush(stdin);
	gets(ps);
	ps1=encrypt(ps);
	if(*num<*lm)
		if (check(un,D,*num))
	  	{
	  		strcpy(D[*num].potrebitel,un);
	  		D[*num].pass=ps1;
	  		D[*num].num=*num+1;
	  		*num=*num+1;
	  		return 1;
	  	}
		else
	  	{
	     	printf("Tozi user veche e zapisan!\n");
	     	return 0;
      	}
    else
	{
		*lm=*lm+SIZE;
		D1=(DATA *)realloc(D,(*lm)*sizeof(DATA));
		if(!D1)
			{ printf("Error!\n");exit(1);}
		else
		{
			D=D1;
			if (check(un,D,*num))
	  		{
	  			strcpy(D[*num].potrebitel,un);
	  			D[*num].pass=ps1;
	  			D[*num].num=*num+1;
	  			*num=*num+1;
	  			return 1;
	  		}
			else
	  		{
	     		printf("Tozi user veche e zapisan!\n");
	     		return 0;
      		}

		}
	}
}
int login(DATA *D,int num)
{
	int i,flag=0;
	char un[30],ps[30];
	printf("User name:");fflush(stdin);
	gets(un);
	printf("Password:");fflush(stdin);
	gets(ps);
	for(i=0;i<num;i++)
	{
	  if((strcmp(D[i].potrebitel,un)==0)&&(D[i].pass==encrypt(ps)))
	  	flag=1;
    }
	  if(flag)
	  	return 1;
	  else
	    return 0;
}
int second_menu()
{
	int key;
	do
	{
		system("cls");
		printf("Izberete dejstvie:\n");
		printf("1 - Depozit\n");
		printf("2 - Teglene\n");
		printf("3 - Transfer\n");
		printf("4 - Obrabotka na tranzakcii\n");
		printf("5 - Logout\n");
		printf("Kod na operacia:");
		scanf("%d",&key);
		switch(key){
			case 1:
					break;
			case 2:
					break;
			case 3:
					break;
			case 4:
					break;
			case 5: printf("Vie izlqzohte ot profila si!\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					return 1;
			default:
				{
				printf("Nekorekten kod!\n");
				printf("Natisnete klavish za prodyljenie!\n");getch();
				}
		}
	}while(key!=5);
}
