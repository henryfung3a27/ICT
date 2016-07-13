#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mysql/mysql.h>
#include <time.h>

#define FILE "Original.exe"		// <<-----------------------------------------
#define TABLE "sba_record"

int m[9] = {0,0,0,0,0,0,0,0,0};
int move[9] = {0,0,0,0,0,0,0,0,0};		// move[0] = 1st button tapped
int button;
int game_result = 0;		// 0 = not yet finish, 1 = former play win, 2 = latter play win, 3 = draw
int turn = 1;			// increment every turn
int skip = 0;
int round_num = 1;	// increment every new round   odd = player first, even = cpu first

MYSQL *conn;
MYSQL_ROW row;
MYSQL_RES *result;
unsigned int num_fields;
unsigned int num_rows;


void output_header() {
	printf("Content-Type:text/html;charset=iso-8859-1\n\n");
}

void receive_input() {
	char *data;
	data = getenv("QUERY_STRING");
	if (game_result == 0)
		sscanf(data, "\
								m1=%d&m2=%d&m3=%d&m4=%d&m5=%d&m6=%d&m7=%d&m8=%d&m9=%d\
								&turn=%d&round_num=%d&game_result=%d&Button%d\
							 ", \
							 &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8],\
							 &turn, &round_num, &game_result, &button);
	else if (game_result != 0)
		sscanf(data, "\
								m1=%d&m2=%d&m3=%d&m4=%d&m5=%d&m6=%d&m7=%d&m8=%d&m9=%d\
								&turn=%d&round_num=%d&game_result=%d&Button%d\
							 ");
}

void board() {
	int i, j;
	printf("<form action=\"%s\">", FILE);
	if (game_result == 0) {
		for (j = 0; j < 9; j++) {
			printf("  <input type=\"hidden\" name=\"m%d\" value=\"%d\">", j+1, m[j]);
		}
		printf("  <input type=\"hidden\" name=\"turn\" value=\"%d\">", turn);
		printf("  <input type=\"hidden\" name=\"round_num\" value=\"%d\">", round_num);
		printf("  <input type=\"hidden\" name=\"game_result\" value=\"%d\">", game_result);
	}
	else {
		for (j = 0; j < 9; j++) {
			printf("  <input type=\"hidden\" name=\"m%d\" value=\"%d\">", j+1, 0);
		}
		printf("  <input type=\"hidden\" name=\"turn\" value=\"%d\">", 1);
		printf("  <input type=\"hidden\" name=\"round_num\" value=\"%d\">", round_num+1);
		printf("  <input type=\"hidden\" name=\"game_result\" value=\"%d\">", 0);
	}
	for (i = 0; i < 9; i++) {
    	printf("  <input type=\"submit\" name=\"Button%d\"", i+1);
    	if (m[i] == 0)
			printf("value=\" \"");
		else if (m[i] % 2 == 1)
			printf("value=\"O\"");
		else if (m[i] % 2 == 0)
			printf("value=\"X\"");
		else
			printf ("ERROR");
		if (game_result != 0)
			printf("disabled=\"disabled\"");
		printf("style=\"height:70px; width:70px\">");
    	if (i == 2 || i == 5) {
    		printf("<br>");
    	}
   }
	if (game_result != 0) {
		printf("<br><br>");
   	printf("  <input type=\"submit\" value=\"Reset!!!\" style=\"height: 60px; width: 150px\"");
   }
}

void player_move() {
	// 0 = ' '	 1 = 'O'	 2 = 'X'
	if (m[button-1] == 0) {
	   m[button-1] = turn;
	   turn++;
	}
   else
      skip = 1;
}

void random_move() {
	srand(time(NULL));
	int r;
	do {
		r = rand() % 9;
	} while (m[r] != 0);
	m[r] = turn;
	turn++;
}

int odd_even(int a) {
	if (a == 0)
		return 0;
	else if (a % 2 == 1)		// odd
		return 1;
	else if (a % 2 == 0)		// even
		return 2;
}

void status() {
	int a = 0,b = 0;
	int i, j, k;
	int draw = 1;
	// horizontal lines
	for (i = 0; i < 3; i++) {
		if (odd_even(m[a]) == odd_even(m[a+1]) && odd_even(m[a]) == odd_even(m[a+2]) && m[a] != 0) {
			if (odd_even(m[a]) == 1) {
				game_result = 1;
			}
			else if (odd_even(m[a]) == 2) {
				game_result = 2;
			}
		}
		a += 3;
	}
	// vertical lines
	for (j = 0; j < 3; j++) {
		if (odd_even(m[j]) == odd_even(m[j+3]) && odd_even(m[j]) == odd_even(m[j+6]) && m[j] != 0) {
			if (odd_even(m[j]) == 1) {
				game_result = 1;
			}
			else if (odd_even(m[j]) == 2) {
				game_result = 2;
			}
		}
	}
	// left diagonal
	if (odd_even(m[0]) == odd_even(m[4]) && odd_even(m[0]) == odd_even(m[8]) && m[0] != 0)
		if (odd_even(m[0]) == 1) {
			game_result = 1;
		}
		else if (odd_even(m[0]) == 2) {
			game_result = 2;
		}
	// right diagonal
	if (odd_even(m[2]) == odd_even(m[4]) && odd_even(m[2]) == odd_even(m[6]) && m[2] != 0)
		if (odd_even(m[2]) == 1) {
			game_result = 1;
		}
		else if (odd_even(m[2]) == 2) {
			game_result = 2;
		}
	// draw	
	for (k = 0; k < 9; k++) {
		if (m[k] == 0)
			draw = 0;
	}
	
	if (odd_even(round_num) == 1)
		if (game_result == 1) {
			printf("<h2>You Win!</h2>");
		}
		else if (game_result == 2) {
			printf("<h2>Computer Win!</h2>");
		}
	if (odd_even(round_num) == 2)
		if (game_result == 1) {
			printf("<h2>Computer Win!</h2>");
		}
		else if (game_result == 2) {
			printf("<h2>You Win!</h2>");
		}
	if (draw == 1 && game_result == 0) {
		printf("<h2>Draw</h2>");
		game_result = 3;
	}
}

void move_sequence() {
	int i, j;
    for (i = 1; i <= turn; i++) {
   	for(j = 0; j < 9; j++) {
  			if (m[j] == i) {
  				move[i-1] = j+1;
  				break;
   		}
		}
	}
}

int open_db() {
	char host[30] = "localhost";
	char user[30] = "root";
	char pass[30] = "12345";
	char db[30] = "SBA";
	
	conn = mysql_init(0);
	if( conn )
		printf("Connected successfully.\n");
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

int insert_record() {
	char sql[120];
	int rows_affected;
	sprintf( sql, "INSERT INTO %s VALUES ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"\
						, TABLE, move[0], move[1], move[2], move[3], move[4], move[5], move[6], move[7], move[8], game_result);
	
	if (mysql_query(conn, sql)) {
        printf( "Fail to insert record: %s\n", mysql_error(conn));
        return(0);
    }    
    else {
        rows_affected = mysql_affected_rows(conn);
        if (rows_affected > 0) 
           return(1);           // record inserted successfully
        else    
           return(0);           // record not inserted (may be because of duplicated values in the key
    }    
}

int select_records(){
	char sql[500];
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
	char temp[300] = "";
	char itoa_move[2];
	char itoa_loop[2];
	int i;
	
	//sprintf( sql, "SELECT * FROM %s where wingame = %d", TABLE, odd_even(round_num) == 1? 2:1);
	for (i = 0; i < 9; i++) {
		if (move[i] != 0) {
			sprintf(itoa_loop, "%d", i+1);
			sprintf(itoa_move, "%d", move[i]);
			strcat(temp, " and a");
			strcat(temp, itoa_loop);
			strcat(temp, "=");
			strcat(temp, itoa_move);
		}
		else break;
		itoa_move[0] = '\0';
		itoa_loop[0] = '\0';
	}
	sprintf( sql, "SELECT * FROM %s where wingame = %d %s"\
		, TABLE, odd_even(round_num) == 1? 2:1, temp);
	
	mysql_query(conn, sql);
	result = mysql_store_result(conn);
	
	if ( mysql_num_rows(result) == 0 ) {
		sprintf(sql, "SELECT * FROM %s where wingame = 3 %s"\
		, TABLE, temp);
	}
	
	printf("<br>%s<br>", sql);
	
    if (mysql_query(conn, sql)) {
        printf( "Fail to query student: %s\n", mysql_error(conn));
        return(0);
    }    
    else {
        result = mysql_store_result(conn);
        printf("<br>There are %d results.", *result);
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
					  printf( "<br>%s %s %s %s %s %s %s %s %s %s\n"\
					  				, a1, a2, a3, a4, a5, a6, a7, a8, a9, wingame);	// ------> Can use row[x] instead of a1, a2....??
            }
            mysql_free_result(result);
        }
        	sprintf(sql, "SELECT * FROM %s where wingame = 3 %s", TABLE, temp);
    }
    return(0);
}

void computer_move() {
	// Call select_records here ?	  or    Rename select_records -> computer_move ?
}


main() {
	output_header();
	printf("<h4>%s</h4><br>", FILE);
	receive_input();
	move_sequence();
	open_db();
	status();
	if (game_result == 0) {
		if (odd_even(round_num) == 1 && turn == 1) {
			player_move();
			status();
		}
		else if (turn != 1) {
			player_move();
			status();
		}
	}
	if (game_result == 0 && skip == 0) {
		if (odd_even(round_num) == 2 && turn == 1) {
			random_move();
			status();
		}
		else if (turn != 1) {
			random_move();
			status();
		}
	}
	move_sequence();
	select_records();
	board();
	
	if (game_result != 0) {
		insert_record();
		close_db();
		printf("<br><br>Record inserted<br>");
	}
	
	
	printf("<br><br><br>button = |%d|<br />", button);
	printf("now turn = |%d|<br />", turn);
	printf("round_num = |%d|<br />", round_num);
	printf("game_result = |%d|<br />", game_result);
	int i, j;
	printf("m = ");
	for (i = 0; i < 9; i++) {
		printf("[%d]", m[i]);
	}
	printf("<br>move = ");
	for (j = 0; j < 9; j++) {
		printf("[%d]", move[j]);
	}
}
