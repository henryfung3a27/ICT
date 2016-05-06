#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mysql/mysql.h>
#include <conio.h>

#define TABLE "testc"

MYSQL *conn;
MYSQL_ROW row;
MYSQL_RES *result;
unsigned int num_fields;
unsigned int num_rows;


int open_db() {
	char host[30] = "localhost";
	char user[30] = "root";
	char pass[30] = "12345";
	char db[30] = "SBA";
	
	conn = mysql_init(0);
	if( conn )
		printf("Connected %s successfully.\n", TABLE);
	else {
		printf("Fail to connect.\n");
		printf("%s\n", mysql_error(conn));
		exit(2);
	}
	
	if ( mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0))
		printf("Connection established, \n");
	else {
		printf("Fail to establish connection: %s\n", mysql_error(conn));
		exit(3);
	}
	
	printf("with connection character set: %s\n", mysql_character_set_name(conn));
}

int close_db() {
	mysql_close(conn);
}

int select_all_fields_for_all_records() {    
    char sql[120] = "SELECT * FROM testc";
	 
    char a1[2];
	 char a2[2];
	 char a3[2];
	 char a4[2];
	 char a5[2];
	 char a6[2];
	 char a7[2];
	 char a8[2];
	 char a9[2];
	 char wingame[2];
	 /*
	 int a1;
    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
    int a8;
    int a9;
    int wingame;
    */

    if (mysql_query(conn, sql)) {
        printf( "Fail to query student: %s\n", mysql_error(conn));
        return(0);
    }    
    else {
        result = mysql_store_result(conn);    
        if (result)  // if there are rows
        {
            num_fields = mysql_num_fields(result);
            while ((row = mysql_fetch_row(result)) > 0) {
                 strcpy( a1, row[0] );
                 strcpy( a2, row[1] );
                 strcpy( a3, row[2] );
                 strcpy( a4, row[3] );
                 strcpy( a5, row[4] );
                 strcpy( a6, row[5] );
                 strcpy( a7, row[6] );
                 strcpy( a8, row[7] );
                 strcpy( a9, row[8] );
                 strcpy( wingame, row[9] );
					  printf( "%s %s %s %s %s %s %s %s %s %s\n"\
					  				, a1, a2, a3, a4, a5, a6, a7, a8, a9, wingame);
					  /*
                 a1 = row[0];
                 a2 = row[1];
                 a3 = row[2];
                 a4 = row[3];
                 a5 = row[4];
                 a6 = row[5];
                 a7 = row[6];
                 a8 = row[7];
                 a9 = row[8];
                 wingame = row[9];
                 printf( "# %d # %d # %d # %d # %d # %d # %d # %d # %d # %d\n"\
					  				, a1, a2, a3, a4, a5, a6, a7, a8, a9, wingame);
                 printf( "# %d # %d # %d # %d # %d # %d # %d # %d # %d # %d\n"\
					  				, row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]);
                 printf( "| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n"\
					  				, row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]);
					  */
            }            
            mysql_free_result(result);
        }
    }
    return(0);
}    

main() {
	open_db();
	select_all_fields_for_all_records();
	close_db();
}
