#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENT_NB 10

struct element{
unsigned int id;
unsigned char vaild;
unsigned char cnt;
};

struct table{
	struct element ele[MAX_ELEMENT_NB];
	unsigned int nb;
}table;

void table_init(void)
{
	return;
}

static unsigned short find_site(void)
{
	unsigned short i;
	if( table.nb < MAX_ELEMENT_NB){
		for(i = 0; i < MAX_ELEMENT_NB; i++ ){
			if(table.ele[i].vaild == 0){ 		
				break;
			}
		}
	}
	else{
		i = MAX_ELEMENT_NB;
	}
	return i;
}

unsigned short check_table_sit(unsigned int id)
{
	unsigned short i,cnt;
	for(i = 0,cnt = 0; cnt < table.nb && i < MAX_ELEMENT_NB; i++ ){
		if(table.ele[i].vaild != 0){			
			if( table.ele[i].id == id){
				break;
			}
			cnt++;
		}
	}
	return cnt < table.nb ? i : MAX_ELEMENT_NB;
}





unsigned char table_add(unsigned  int id)
{
	unsigned short i;
	unsigned char ret=0;
	if((i = check_table_sit(id)) < MAX_ELEMENT_NB){
		printf("old table \n");
	}
	else if((i = find_site()) < MAX_ELEMENT_NB){
		table.ele[table.nb].cnt= 5;
		table.ele[table.nb].id= id;
		table.ele[table.nb].vaild= 1;
		table.nb ++;	
		//printf("new table\n");
	}
	return ret;
}

void table_loop(void)
{
	unsigned int i;
	for(i=0; i<MAX_ELEMENT_NB; i++){
		if(table.ele[i].cnt > 0){
			table.ele[i].cnt--;
			if(table.ele[i].cnt == 0 && table.nb > 0)
				table.nb--;
				
		}
	}
}

void table_printf(void)
{
	unsigned int i;
	printf("id table:\n");
	for(i=0; i<MAX_ELEMENT_NB; i++){
		if(table.ele[i].cnt > 0){
			printf("id: %03d\n", table.ele[i].id);
		}
	}
	printf("\n");
}

int main(int argc ,char** argv)
{
	int i;
	while(1){
		table_add(6000+ (i++));
		table_loop();
		table_printf();
		sleep(1);
	}
	return 0;
}




