/*$Id: b_soft_review.c,v 5.16 2013/09/26 09:43:28 sasa Exp $*/
/*10.09.2019	31.10.2014	Белых А.И. 	b_corp2.c
Формирование файла для подсистемы Клиент-банк для системы CORP2
Структура файлу ІМПОРТУ.


kb_a  12  C  МФО клієнта               Незмінне  МФО банку платника, де обліковується рахунок 2909*.
kk_a  29  C  рахунок клієнта           Незмінне  Транзитний рахунок 2909*, на який попередньо перерахована загальна сума заробітної платні на карткові рахунки 2625 співробітників.
kb_b  12  C  МФО кореспондента                   МФО банку одержувача, де обліковуються рахунки одержувачів зарплатні.
kk_b  29  C  рахунок кореспондента               Рахунок 2625* на який перераховується сума зарплатні – по кожній особі окремо.
d_k    1  N  ознака «дебет/кредит»     Незмінне  Поле ЗАВЖДИ містить значення «1».
summa 20  N  сума платежу (в копійках)  
vid    3  N  тип платежу               Незмінне  Поле ЗАВЖДИ містить значення «1».
ndoc  10  C  номер документа                      Кожний документ повинен містити унікальний номер.
i_va   4  N  код валюти платежу        Незмінне   Поле ЗАВЖДИ містить значення «980».
da     8  D  дата                                 Поточна дата.
da_doc 8  D  дата документу                       Повинна бути не більше значення поля «da», але різниця не повинна перевищувати 10 днів.
nk_a  38  C  ім'я клієнта              Незмінне   Назва транзитного рахунку 2909*. Назва рахунку береться в установі банку, де відкритий рахунок 2909*.
nk_b  38  C  ім'я кореспондента                   Прізвище, ім'я, по-батькові одержувача зарплатні.
nazn 160  C  призначення платежу                  
kod_a 14  C  ЗКПО клієнта              Незмінне  Поле містить ЗКПО банку платника.
kod_b 14  C  ЗКПО кореспондента                   Містить ідентифікаційний номер одержувача зарплатні.
 

*/   




#include <math.h>
#include <errno.h>
#include "buhg.h"
#include "dok4.h"

void b_corp2_h(const char *imaf,long kolz);

extern class REC rec;

int	b_corp2(const char *tabl,class iceb_tu_str *imafdbf_out)
{
char		imaf1[32];
FILE		*ff1;
class iceb_tu_str koment("");
char		strsql[512];
short		d,m,g;
int kom1=0;
int kolstr=0;
SQL_str row;
class SQLCURSOR cur;

sprintf(strsql,"select datd,nomd from %s where vidpl='1'",tabl);

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
  iceb_t_soob(gettext("Не найдено ни одного документа для передачи в банк!"));
  return(1);
 }

class VVOD VVOD1(2);
helstr(LINES-1,0," F10 ",gettext("выход"),NULL);

VVOD1.VVOD_SPISOK_add_MD(gettext("Введите имя файла"));
if((kom1=vvod1(imafdbf_out,32,&VVOD1,NULL,stdscr,-1,-1)) == FK10)
 return(1);
if(kom1 == ESC)
 return(1);


sprintf(imaf1,"plat_b.txt");
if((ff1 = fopen(imaf1,"w")) == NULL)
 {
  error_op_nfil(imaf1,errno,"");
  return(1);
 }

int nomstr=0;
int summa=0;
while(cur.read_cursor(&row) != 0)
 {
  rsdat(&d,&m,&g,row[0],2);

  if(readpdok(tabl,g,row[1]) != 0)
     continue;

  /*Читаем комментарий*/
  readkom(tabl,rec.dd,rec.md,rec.gd,rec.nomdk.ravno(),&koment);

  nomstr++;  

/************
kb_a  12  C  МФО клієнта               Незмінне  МФО банку платника, де обліковується рахунок 2909*.
kk_a  15  C  рахунок клієнта           Незмінне  Транзитний рахунок 2909*, на який попередньо перерахована загальна сума заробітної платні на карткові рахунки 2625 співробітників.
kb_b  12  C  МФО кореспондента                   МФО банку одержувача, де обліковуються рахунки одержувачів зарплатні.
kk_b  15  C  рахунок кореспондента               Рахунок 2625* на який перераховується сума зарплатні – по кожній особі окремо.
d_k    1  N  ознака «дебет/кредит»     Незмінне  Поле ЗАВЖДИ містить значення «1».
summa 20  N  сума платежу (в копійках)  
vid    3  N  тип платежу               Незмінне  Поле ЗАВЖДИ містить значення «1».
ndoc  10  C  номер документа                      Кожний документ повинен містити унікальний номер.
i_va   4  N  код валюти платежу        Незмінне   Поле ЗАВЖДИ містить значення «980».
da     8  D  дата                                 Поточна дата.
da_doc 8  D  дата документу                       Повинна бути не більше значення поля «da», але різниця не повинна перевищувати 10 днів.
nk_a  38  C  ім'я клієнта              Незмінне   Назва транзитного рахунку 2909*. Назва рахунку береться в установі банку, де відкритий рахунок 2909*.
nk_b  38  C  ім'я кореспондента                   Прізвище, ім'я, по-батькові одержувача зарплатні.
nazn 160  C  призначення платежу                  
kod_a 14  C  ЗКПО клієнта              Незмінне  Поле містить ЗКПО банку платника.
kod_b 14  C  ЗКПО кореспондента                   Містить ідентифікаційний номер одержувача зарплатні.
***************/
  double cel=0.;
  modf(rec.sumd*100.,&cel);
  summa=(int)cel;

  fprintf(ff1," %-12.12s%-29.29s%-12.12s%-29.29s%1.1s%20d%3.3s%-10.10s%4.4s%04d%02d%02d%04d%02d%02d%-*.*s%-*.*s%-*.*s%-14.14s%-14.14s",
  rec.mfo.ravno(),
  rec.nsh.ravno(),
  rec.mfo1.ravno(),
  rec.nsh1.ravno(),
  "1",
  summa,
  "1",
  rec.nomdk.ravno(),
  "980",
  rec.gd,rec.md,rec.dd, 
  rec.gd,rec.md,rec.dd, 
  iceb_tu_kolbait(38,rec.naior.ravno()),
  iceb_tu_kolbait(38,rec.naior.ravno()),
  rec.naior.ravno(),
  iceb_tu_kolbait(38,rec.naior1.ravno()),
  iceb_tu_kolbait(38,rec.naior1.ravno()),
  rec.naior1.ravno(),
  iceb_tu_kolbait(160,koment.ravno()),
  iceb_tu_kolbait(160,koment.ravno()),
  koment.ravno(),
  rec.kod.ravno(),
  rec.kod1.ravno());


 }
fputc(26, ff1);

fclose(ff1);

/*Перекодируем в WINDOUS кодировку*/
perecod(2,imaf1);


char imafdbf[32];

sprintf(imafdbf,"plat.txt");
b_corp2_h(imafdbf,nomstr);

/*Сливаем два файла*/
iceb_t_cat(imafdbf,imaf1);
unlink(imaf1);

return(0);
}
/******************************/
/*Создаем заголовок файла dbf*/
/******************************/
void b_corp2_f(DBASE_FIELD *f,const char *fn,char  ft,int l1,int l2,
int *header_len,int *rec_len)
{

strncpy(f->name, fn,sizeof(f->name)-1);
f->type = ft;
f->length = l1;
f->dec_point = l2;
*header_len=*header_len+sizeof(DBASE_FIELD);
*rec_len=*rec_len+l1;
}

/***********************************************/
void b_corp2_h(const char *imaf,long kolz)
{
time_t		tmm;
struct  tm      *bf;
FILE		*ff;
DBASE_HEAD  h;
int fd;
int i;
int header_len, rec_len;
#define kolpol  16
DBASE_FIELD f[kolpol];
memset(&f, '\0', sizeof(f));

if((ff = fopen(imaf,"w")) == NULL)
 {
  error_op_nfil(imaf,errno,"");
  return;
 }

memset(&h,'\0',sizeof(h));

h.version = 3;

time(&tmm);
bf=localtime(&tmm);

h.l_update[0] = bf->tm_year;       /* yymmdd for last update*/
h.l_update[1] = bf->tm_mon+1;       /* yymmdd for last update*/
h.l_update[2] = bf->tm_mday;       /* yymmdd for last update*/

h.count = kolz;              /* number of records in file*/

header_len = sizeof(h);
rec_len = 0;
int shetshik=0;
/************
kb_a  12  C  МФО клієнта               Незмінне  МФО банку платника, де обліковується рахунок 2909*.
kk_a  29  C  рахунок клієнта           Незмінне  Транзитний рахунок 2909*, на який попередньо перерахована загальна сума заробітної платні на карткові рахунки 2625 співробітників.
kb_b  12  C  МФО кореспондента                   МФО банку одержувача, де обліковуються рахунки одержувачів зарплатні.
kk_b  29  C  рахунок кореспондента               Рахунок 2625* на який перераховується сума зарплатні – по кожній особі окремо.
d_k    1  N  ознака «дебет/кредит»     Незмінне  Поле ЗАВЖДИ містить значення «1».
summa 20  N  сума платежу (в копійках)  
vid    3  N  тип платежу               Незмінне  Поле ЗАВЖДИ містить значення «1».
ndoc  10  C  номер документа                      Кожний документ повинен містити унікальний номер.
i_va   4  N  код валюти платежу        Незмінне   Поле ЗАВЖДИ містить значення «980».
da     8  D  дата                                 Поточна дата.
da_doc 8  D  дата документу                       Повинна бути не більше значення поля «da», але різниця не повинна перевищувати 10 днів.
nk_a  38  C  ім'я клієнта              Незмінне   Назва транзитного рахунку 2909*. Назва рахунку береться в установі банку, де відкритий рахунок 2909*.
nk_b  38  C  ім'я кореспондента                   Прізвище, ім'я, по-батькові одержувача зарплатні.
nazn 160  C  призначення платежу                  
kod_a 14  C  ЗКПО клієнта              Незмінне  Поле містить ЗКПО банку платника.
kod_b 14  C  ЗКПО кореспондента                   Містить ідентифікаційний номер одержувача зарплатні.
***************/
b_corp2_f(&f[shetshik++],"kb_a", 'C', 12, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"kk_a", 'C', 29, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"kb_b", 'C', 12, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"kk_b", 'C', 29, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"d_k", 'N',1, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"summa", 'N',20, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"vid", 'N', 3, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"ndoc", 'C', 10, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"i_va", 'N', 4, 0,&header_len,&rec_len);
b_corp2_f(&f[shetshik++],"da", 'D', 8, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"da_doc", 'D', 8, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"nk_a", 'C', 38, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"nk_b", 'C', 38, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"nazn", 'C', 160, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"kod_a", 'C', 14, 0, &header_len,&rec_len);
b_corp2_f(&f[shetshik++],"kod_b", 'C', 14, 0, &header_len,&rec_len);


h.header = header_len + 1;/* length of the header
                           * includes the \r at end
                           */
h.lrecl= rec_len + 1;     /* length of a record
                           * includes the delete
                           * byte
                          */
/*
 printw("h.header=%d h.lrecl=%d\n",h.header,h.lrecl);
*/


fd = fileno(ff);

if(write(fd, &h, sizeof(h)) < 0)
 {
  printw("\n%s-%s\n",__FUNCTION__,strerror(errno));
  OSTANOV();
 }

for(i=0; i < kolpol; i++) 
 {
  if(write(fd, &f[i], sizeof(DBASE_FIELD)) < 0)
   {
    printw("\n%s-%s\n",__FUNCTION__,strerror(errno));
    OSTANOV();
   }
 }
fputc('\r', ff);

fclose(ff);

}
