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
	float balance;
}DATA;

typedef struct transaction {
    int type;
    float amount;
    int account_num;
    struct transaction* next;
} trs;

DATA *readInfo(char *filename,DATA *D,int *broi);
void writeInfo(char *filename,DATA *D,int *broi);
int reg(DATA *D,int *num,int *lm);
int login(DATA *D,int num);
int check(char *ime,DATA *D,int num);
int encrypt(char *pass);
void second_menu(DATA *D, int num);
void withdraw(DATA *D, int num);
void deposit(DATA *D, int num);
void transfer(DATA *D, int num);

int main()
{
	DATA *DT;int limit=SIZE,a;
	FILE *f;
	int key,br=0,i,numb;
	char filename[40];

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
		printf("balance:%.2f \n",DT[i].balance);
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
			case 1: numb=login(DT,br);
                    if(numb>=0){
                    printf("Uspeshno vlizane!\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					second_menu(DT,numb);
					}
					else{
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
	  		D[*num].balance=0;
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
	int i;
	char un[30],ps[30];
	printf("User name:");fflush(stdin);
	gets(un);
	printf("Password:");fflush(stdin);
	gets(ps);
	for(i=0;i<num;i++)
	{
	  if((strcmp(D[i].potrebitel,un)==0)&&(D[i].pass==encrypt(ps)))
	  	return i;
    }
	  	return -1;
}

void withdraw(DATA *D,int num)
 {
    float amount;
    printf("Vuvedete suma za teglene: ");
    scanf("%f",&amount);
    if(D[num].balance>=amount){
        D[num].balance=D[num].balance - amount;
        printf("Uspeshno teglene. Balance: %.2f\n", D[num].balance);
        return;
    }else{
        printf("Nedostatuchna nalichnost. Tegleneto e otkazano.\n");
        return;
    }
    printf("Tozi user ne beshe nameren. Tegleneto e otkazano.\n");
}

void deposit(DATA *D, int num)
 {
    float amount=20.23;
    printf("Vuvedete suma za deposit: ");
    scanf("%f",&amount);
    D[num].balance=D[num].balance + amount;
    printf("Uspeshen deposit. Balance: %.2f \n", D[num].balance);
    return;
    printf("Tozi user ne beshe nameren. Deposita e otkazan.\n");
}
void transfer(DATA *D, int num)
 {
    int pnum;
    float amount;
    printf("Vuvedete nomer na smetka na poluchatel: ");//trqbva proverka za poluchatel
    scanf("%d",&pnum);
    //if(pnum>){
        printf("Vuvedete suma za transfer: ");
        scanf("%f",&amount);
        if(D[num].balance<amount){
            printf("Nedostatuchna nalichnost. Transferat e otkazan.\n");
            return;
        }
        D[num].balance=D[num].balance - amount;
        D[pnum].balance=D[pnum].balance + amount;
        printf("Uspeshen transfer. Balance: %.2f\n", D[num].balance);
    /*}else{
        printf("Tozi user ne beshe nameren. Transferat e otkazan.\n");
    }*/
    return;
}

void second_menu(DATA *D, int num)
{
	int key;
	do
	{
		system("cls");
		printf("Izberete dejstvie:\n");
		printf("1 - Teglene\n");
		printf("2 - Deposit\n");
		printf("3 - Transfer\n");
		printf("4 - Obrabotka na tranzakcii\n");
		printf("5 - Logout\n");
		printf("Kod na operacia:");
		scanf("%d",&key);
		switch(key){
			case 1:
                   withdraw(D,num);
					break;
			case 2:
                    deposit(D,num);
					break;
			case 3:
                    transfer(D,num);
					break;
			case 4:
					break;
			case 5: printf("Vie izlqzohte ot profila si!\n");
					printf("Natisnete klavish za prodyljenie!\n");getch();
					return 1;
			default:
				{
				printf("Nekorekten izbor!\n");
				printf("Natisnete klavish za prodyljenie!\n");getch();
				}
		}
	}while(key!=5);
}
