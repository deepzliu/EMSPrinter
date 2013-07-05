// SQLiteTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	if( argc!=2 ){
		fprintf(stderr, "Usage: %s DATABASE\n", argv[0]);
		return(1);
	}

	

	rc = sqlite3_open(argv[1], &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	//char pkey[] = "abc";
	//sqlite3_key(db, pkey, 3);
	char sql[256];// = "select * from T1";
	while(1){
		printf("# Input SQL: ");
		gets(sql);
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %d - %s\n", rc, zErrMsg);
			if(memcmp(zErrMsg, "no such table", strlen("no such table")) == 0){
				printf("需要建表\n");
			}
			sqlite3_free(zErrMsg);
		}
	}


	sqlite3_close(db);
	return 0;
}

