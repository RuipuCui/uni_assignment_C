/* A simplified community detection algorithm:
 *algorithms are fun
 *gcc -Wall -o program program.c
./program < test0.txt
 * Skeleton code written by Jianzhong Qi, May 2023
 * Edited by: [Ruipu Cui 1298198]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */
#define MAX_USERS_NUM 50
#define MAX_HASHTAGS 10
#define MAX_LETTERS 20

typedef struct {
	/* add your user_t struct definition */
	int user_num;
	int year_joined;
	char hashtags[MAX_HASHTAGS][MAX_LETTERS + 1];
	int hashtags_count;
} user_t;

typedef char data_t;							  /* to be modified for Stage 4 */

/* linked list type definitions below, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/
typedef struct node node_t;

struct node {
	char data[MAX_LETTERS + 1];
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

/****************************************************************/

/* function prototypes */
list_t *make_empty_list(void);
void free_list(list_t *list);
void print_list(list_t *list);
list_t *insert_unique_in_order(list_t *list, char value[MAX_LETTERS + 1]);

void print_stage_header(int stage_num);

void stage_one(user_t user_profiles[], int* total_num);
void stage_two(int total_num, int matrix[][total_num]);
void stage_three(int total_num, int matrix[][total_num], double soc_matrix[][total_num]);
void stage_four(int total_num, double soc_matrix[][total_num], user_t user_profiles[]);

/* add your own function prototypes here */
void read_number(int total_num, int matrix[][total_num], int start_line, int finish_line);
double soc_function(int total_num, int matrix[][total_num], int user1, int user2);
void print_hashtags(int core_user, int friends_indexs[], 
		int num_close_friends, user_t user_profiles[], int total_num);

/****************************************************************/

/* main function controls all the action; modify if needed */
int
main(int argc, char *argv[]) {
	/* add variables to hold the input data */
	user_t user_profiles[MAX_USERS_NUM];
	int total_num = 0;

	/* stage 1: read user profiles */
	stage_one(user_profiles, &total_num);
	
	/* stage 2: compute the strength of connection between u0 and u1 */
	int matrix[total_num][total_num];
	stage_two(total_num, matrix);
	
	/* stage 3: compute the strength of connection for all user pairs */
	double soc_matrix[total_num][total_num];
	stage_three(total_num, matrix, soc_matrix);
	
	/* stage 4: detect communities and topics of interest */
	stage_four(total_num, soc_matrix, user_profiles);
	
	/* all done; take some rest */
	return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read user profiles */
void 
stage_one(user_t user_profiles[], int* total_num) {
	/* add code for stage 1 */
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);
	int max_hashtags = 0;
	int max_hashtags_user = 0;
	//read a integer fellow by 'u' and another integer
	while(scanf("u%d %d ", &user_profiles[*total_num].user_num, 
			&user_profiles[*total_num].year_joined) == 2){
		int hashtag_num = 0;
		while(scanf("#%s ", user_profiles[*total_num].hashtags[hashtag_num])){
			hashtag_num++;
		}

		user_profiles[*total_num].hashtags_count = hashtag_num;
		if(hashtag_num >= max_hashtags){
			max_hashtags = hashtag_num;
			max_hashtags_user = user_profiles[*total_num].user_num;
		}

		*total_num += 1;
	}

	printf("Number of users: %d\n", *total_num);
	printf("u%d has the largest number of hashtags:\n", max_hashtags_user);
	int i;
	for(i = 0; i < user_profiles[max_hashtags_user].hashtags_count; i++){
		if(i != 0){
			printf(" ");
		}

		printf("#%s", user_profiles[max_hashtags_user].hashtags[i]);
	}

	printf("\n");
	printf("\n");
}

/* stage 2: compute the strength of connection between u0 and u1 */
void 
stage_two(int total_num, int matrix[][total_num]){
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);
	//for stage 2 only 2 line need to read
	int start_line = 0;
	int finish_line = 1;
	read_number(total_num, matrix, start_line, finish_line);

	int user1 = 0;
	int user2 = 1;
	double res = soc_function(total_num, matrix, user1, user2);

	printf("Strength of connection between u0 and u1: %4.2f", res);
	printf("\n");
	printf("\n");
}

/* stage 3: compute the strength of connection for all user pairs */
void 
stage_three(int total_num, int matrix[][total_num], double soc_matrix[][total_num]){
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);
	int start_line = 2;
	int finish_line = total_num - 1;
	read_number(total_num, matrix, start_line, finish_line);

	int row;
	int col;
	for(row = 0; row < total_num; row++){
		for(col = 0; col < total_num; col++){
			if(row == col){
				soc_matrix[row][col] = 0;
			}else{
				soc_matrix[row][col] = soc_function(total_num, matrix, row, col);
			}

			// add a space after the first number readed
			if(col != 0){
				printf(" ");
			}

			printf("%4.2f", soc_matrix[row][col]);
		}

		printf("\n");
	}

	printf("\n");
}

/* stage 4: detect communities and topics of interest */
void stage_four(int total_num, double soc_matrix[][total_num], user_t user_profiles[]) {
	/* add code for stage 4 */
	/* print stage header */
	print_stage_header(STAGE_NUM_FOUR);
	double ths;
	int thc;
	scanf("%lf %d", &ths, &thc);
	int row;
	int col;
	//declera a array to store all close friend user's num
	int friends_indexs[total_num];
	int i;
	for(i = 0; i < total_num; i++){
		friends_indexs[i] = 0;
	}

	for(row = 0; row < total_num; row++){
		int num_close_friends = 0;
		for(col = 0; col < total_num; col++){
			if(soc_matrix[row][col] > ths){
				friends_indexs[num_close_friends] = col;
				num_close_friends++;
			}
		}

		if(num_close_friends > thc){
			printf("Stage 4.1. Core user: u%d; close friends:", row);
			int j;
			for(j = 0; j < num_close_friends; j++){
				printf(" u%d", friends_indexs[j]);
			}

			printf("\n");
			printf("Stage 4.2. Hashtags:\n");
			print_hashtags(row, friends_indexs, num_close_friends, user_profiles, total_num);
		}

	}

}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* linked list implementation below, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/

/* create an empty list */
list_t
*make_empty_list(void) {
	list_t *list;

	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;

	return list;
}

/* free the memory allocated for a list (and its nodes) */
void
free_list(list_t *list) {
	node_t *curr, *prev;

	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}

	free(list);
}

/* insert a new data element into a linked list, keeping the
	data elements in the list unique and in alphabetical order
*/
list_t
*insert_unique_in_order(list_t *list, data_t value[MAX_LETTERS + 1]){
	/* the following code inserts a new node to the end of list.
		Modify it to suit Stage 4.2. Start by modifying it to 
		insert a new node while keeping an alphabetical order
		(think about how to insert in the middle of a list), 
		then, only insert when value is not in list already.
		[remove this comment to save line space if needed]
	*/

	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	strcpy(new->data, value);
	new->next = NULL;

	if(list->head == NULL) {
        /* empty list */
		list->head = list->foot = new;
	}else if(strcmp(list->head->data, value) == 0){
		free(new);
		return list;
	}else if(strcmp(list->head->data, value) > 0){
		/* insert at the beginning of the list */
		new->next = list->head;
		list->head = new;
	}else if(strcmp(list->foot->data, value) < 0){
		/* insert at the end of the list */
		list->foot->next = new;
		list->foot = new;
	}else{
		node_t* current = list->head;
		while (current->next != NULL && strcmp(current->next->data, value) < 0){
			current = current->next;
		}
		/* check if the value already exists in the list */
		if(strcmp(current->next->data, value) == 0){
			free(new); 
			return list;
		}
		/* insert in the middle of the list */
		new->next = current->next;
		current->next = new;
	}

	return list;
}


/* print the data contents of a list */
void
print_list(list_t *list) {
	/* add code to print list */
	node_t* current = list->head;
	int num_printed = 0;
	while (current != NULL) {
		printf("#%s", current->data);
		num_printed ++;
		/* start a new line when 5 hashtags are printed*/
		if(num_printed % 5 == 0){
			printf("\n");
		}else if(current->next != NULL){
			printf(" ");
		}

		current = current->next;
	}

	/*the last line of hashtags*/
	if(num_printed % 5 != 0){
		printf("\n");
	}
	
}

/****************************************************************/
/*
	Write your time complexity analysis below for Stage 4.2, 
	assuming U users, C core users, H hashtags per user, 
	and a maximum length of T characters per hashtag:

*/

//read input number from a setted start line of matrix and a finish line
void read_number(int total_num, int matrix[][total_num], int start_line, int finish_line){
	int num_readed = 0;
	int row_index = start_line;
	int col_index = 0;	
	while(num_readed < (finish_line - start_line + 1) * total_num){
		if(num_readed == total_num){
			row_index++;
			col_index = 0;
		}

		scanf("%d", &matrix[row_index][col_index]);
		col_index++;
		num_readed++;
	}

}

//calculate the soc value
double 
soc_function(int total_num, int matrix[][total_num], int user1, int user2){
	double intersection = 0;
	double both = 0;
	int i;
	// if they are not friend, return 0
	if(matrix[user1][user2] != 1){
		return 0;
	}

	for(i = 0; i < total_num; i++){
		if(matrix[user1][i] == 1 && matrix[user2][i] == 1){
			intersection++;
		}

		if(matrix[user1][i] == 1 || matrix[user2][i] == 1){
			both++;
		}

	}
	
	double soc;
	soc = intersection / both;
	return soc;
}

//print all unique hashtags
void print_hashtags(int core_user, int friends_indexs[], 
		int num_close_friends, user_t user_profiles[], int total_num){
	int i;
	//create a linked list
	list_t *hashtag_link = make_empty_list();
	//insert core user's hashtags
	for(i = 0; i < user_profiles[core_user].hashtags_count; i++){
		hashtag_link = insert_unique_in_order(hashtag_link, user_profiles[core_user].hashtags[i]);
	}

	int j;
	//insert core user's friend's hashtags
	for(j = 0; j < num_close_friends; j++){
		int k;
		for(k = 0; k < user_profiles[friends_indexs[j]].hashtags_count; k++){
			hashtag_link = insert_unique_in_order(hashtag_link, 
					user_profiles[friends_indexs[j]].hashtags[k]);
		}

	}

	print_list(hashtag_link);
	free_list(hashtag_link);
}

/* here is my stage 4 code structure
  U users, C core users, H hashtags per user, 
  F close friends, and a maximum length of T per hashtag.

 for(every user){
     for(every user){
		....... //after that, core user's close friends are found 
	 }
	 print_hashtags()
 }

 structure for print_hashtags:
 for(every hashtags){
	insert_unique_in_order    //insert core user's hashtags
 }
 for(every close friends){
	for(every hashtags){      //insert core user's friends hashtags
		insert_unique_in_order
	}
 }
 print_list(hashtag_link);
 free_list(hashtag_link);

 the time complexity of insert_unique_in_order depend on how many hashtags
 that already in the list. If insert at the head or foot, the time complexity 
 is O(1). other case is that go over the list until strcmp(current->next->data, value) 
 < 0. the strcmp function inside the insert_unique_in_order has a time complexity 
 of O(2T) => O(T). Therefore the time complexity of
		for(number of hashtags){     
			insert_unique_in_order
		}
 is O( H(H+T) );
 print list has time complexity of O(H*T) as H number of hashtag printed and
 each hashtags has a length T.
 free list has a time complexity of O(H) as H number of node need to be free
 Then the overall time complexity is 
 O(U* (U+ H(H+T) + F*H(H+T) + HT + H) )
 simplified:
 O(U* F*H(H+T))                                                        */
 















