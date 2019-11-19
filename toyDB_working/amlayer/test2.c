/* test2.c: test insert and scan. But this time, forcing
splitting of pages of your B+ tree */
#include <stdio.h>
#include "am.h"
#include "pf.h"
#include "testam.h"

#define STRING_SIZE 250	/* string */
#define MAXRECS	100	/* max # of records to insert */
#define FNAME_LENGTH 80	/* file name size */

main()
{
int fd;	/* file descriptor for the index */
char fname[FNAME_LENGTH];	/* file name */
char buf[STRING_SIZE];	/* buf to store data to be inserted into index */
int recnum;	/* record number */
int sd;	/* scan descriptor */
int numrec;	/* # of records retrieved in s scan */

	/* init */
	printf("initializing\n");
	PF_Init();

	/* create index */
	printf("creating index\n");
	AM_CreateIndex(RELNAME,0,CHAR_TYPE,STRING_SIZE);

	/* open the index */
	printf("opening index\n");
	sprintf(fname,"%s.0",RELNAME);
	fd = PF_OpenFile(fname);

	/* insert into index on character */
	printf("inserting into index on char\n");
	for (recnum=0; recnum < MAXRECS; recnum++){
		sprintf(buf,"%d",recnum);
		padstring(buf,STRING_SIZE);
		AM_InsertEntry(fd,CHAR_TYPE,STRING_SIZE,buf,IntToRecId(recnum));
	}

	printf("opening index scan on char\n");
	sd = AM_OpenIndexScan(fd,CHAR_TYPE,STRING_SIZE,EQ_OP,NULL);
	printf("retrieving recid's from scan descriptor %d\n",sd);
	numrec = 0;
	while((recnum=RecIdToInt(AM_FindNextEntry(sd)))>= 0){
		printf("%d\n",recnum);
		numrec++;
	}
	printf("retrieved %d records\n",numrec);

	/* destroy everything */
	printf("closing down\n");
	AM_CloseIndexScan(sd);
	PF_CloseFile(fd);
	AM_DestroyIndex(RELNAME,0);

	printf("test2 done!\n");
	Buf_getstats();
}
