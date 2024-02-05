/* A simplified learned index implementation:
 *gcc -Wall -o program program.c
./program < test0.txt
 * Skeleton code written by Jianzhong Qi, April 2023
 * Edited by: [ruipu cui  1298198]
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define DATASET_SIZE 100					  /* number of input integers */
#define DATA_OUTPUT_SIZE 10					  /* output size for stage 1 */

#define BS_NOT_FOUND (-1)					  /* used by binary search */
#define BS_FOUND 0

#define FIRST_LOCATION 0

typedef int data_t; 				  		  /* data type */
typedef struct{
    int a_value; 
    int b_value;
    int max_element;
}mapping_t;
/****************************************************************/

/* function prototypes */
void print_stage_header(int stage_num);
int cmp(data_t *x1, data_t *x2);
void swap_data(data_t *x1, data_t *x2);
void partition(data_t dataset[], int n, data_t *pivot,
		int *first_eq, int *first_gt);
void quick_sort(data_t dataset[], int n);
int binary_search(data_t dataset[], int lo, int hi, 
	data_t *key, int *locn);

void stage_one(data_t dataset[], int size);
void stage_two(data_t dataset[], int* max_err_pointer);
void stage_three(mapping_t mapping_function[], data_t dataset[], 
        int prediction_error, int* num_function);
void stage_four(mapping_t mapping_function[], data_t dataset[], 
        int num_function, int prediction_error);

/* add your own function prototypes here */
int function_key(int key, int a, int b);
void struct_binary_search(mapping_t mapping_function[], int lo, int hi, 
		data_t* key, int* locn);
int max_value(int first_value, int second_value);
int min_value(int first_value, int second_value);
/****************************************************************/

/* main function controls all the action */
int
main(int argc, char *argv[]) {
	
    /* to hold all input data */
    data_t dataset[DATASET_SIZE];
    int max_err = 0;

    /* to hold the mapping functions */


    /* stage 1: read and sort the input */
    stage_one(dataset, DATASET_SIZE); 

    /* stage 2: compute the first mapping function */
    stage_two(dataset, &max_err);
	
    /* stage 3: compute more mapping functions */
    /* declear a struct array that store each set of a, b and max_element */
    mapping_t mapping_function[DATASET_SIZE];
    int prediction_error;
    int num_function = 0;
    scanf("%d", &prediction_error);
    stage_three(mapping_function, dataset, prediction_error, &num_function);


	/* stage 4: perform exact-match queries */
    stage_four(mapping_function, dataset, num_function, prediction_error);

    /* all done; take some rest */
    return 0;
}

/****************************************************************/

/* add your code below */

/* stage 1: read and sort the input */
void 
stage_one(data_t dataset[], int size)  {
	
    /* add code for stage 1 */
    /* print stage header */
    print_stage_header(STAGE_NUM_ONE);
    
    data_t num;
    int i;
    for(i = 0; i < size; i++){
        scanf("%d", &num);
        dataset[i] = num;
    }
    quick_sort(dataset, size);

    int j;
    printf("First 10 numbers:");
    for(j = 0; j < DATA_OUTPUT_SIZE; j++){
        printf(" %d", dataset[j]);
    }
    printf("\n");
    printf("\n");
}

 
/* stage 2: compute the first mapping function */
void  
stage_two(data_t dataset[], int* max_err_pointer) {

    /* add code for stage 2 */
    /* print stage header */
    print_stage_header(STAGE_NUM_TWO);
    int a = dataset[FIRST_LOCATION + 1] * FIRST_LOCATION - 
        dataset[FIRST_LOCATION] * (FIRST_LOCATION + 1);
    int b = dataset[FIRST_LOCATION + 1] - dataset[FIRST_LOCATION];
    if(b == 0){
        a = FIRST_LOCATION;
    }
    
    int i;
    int max_err_position = 0;
    int current_err;
    for(i=0; i<DATASET_SIZE; i++){
        current_err = abs(function_key(dataset[i], a, b) - i);
        if(current_err > *max_err_pointer){
            *max_err_pointer = current_err;
            max_err_position = i;
        }
    }

    printf("Maximum prediction error: %d\n", *max_err_pointer);
    printf("For key: %d\n", dataset[max_err_position]);
    printf("At position: %d\n", max_err_position);
    printf("\n");   
}

/* stage 3: compute more mapping functions */ 
void
stage_three(mapping_t mapping_function[], data_t dataset[], 
        int prediction_error, int* num_function)  {
	/* add code for stage 3 */
	/* print stage header */
    print_stage_header(STAGE_NUM_THREE);
    int i;
    int j = 0;
    int not_exceed = 0;
    int num_functions = 0;  

    for(i = 0; i < DATASET_SIZE; i++){
        //after exceed the max error, record a new set of a and b
        if(not_exceed == 0){
            int a = dataset[i+1] * (i) - dataset[i] * (i+1);
            int b = dataset[i+1] - dataset[i];            
            mapping_function[j].a_value = a;
            mapping_function[j].b_value = b;
            if(mapping_function[j].b_value == 0){
                mapping_function[j].a_value = i;
            }
            not_exceed = 1;
        //if exceed the max error, record the max element
        }else if(i >= 2 && abs(function_key(dataset[i], mapping_function[j].a_value, 
            mapping_function[j].b_value) - i) > prediction_error){
            mapping_function[j].max_element = dataset[i-1];
            num_functions++;
            not_exceed = 0;           
            j++;
            i = i - 1;
        //if the loop reach the last data
        }else if(i == DATASET_SIZE - 1){
            mapping_function[j].max_element = dataset[i];            
            num_functions++;
        }
    }

    *num_function = num_functions;
    int k;
    printf("Target maximum prediction error: %d\n", prediction_error);
    for(k = 0; k < num_functions; k++){
        printf("Function %2d: a = %4d, b = %3d, max element = %3d\n", k, 
        mapping_function[k].a_value, mapping_function[k].b_value, mapping_function[k].max_element);
    }

    printf("\n");
}


/* stage 4: perform exact-match queries */
void
stage_four(mapping_t mapping_function[], data_t dataset[], 
    int num_function, int prediction_error) {	
    /* add code for stage 4 */
	/* print stage header */
    print_stage_header(STAGE_NUM_FOUR);
    data_t key;
    while(scanf("%d", &key) == 1){
        printf("Searching for %d:\n", key);
        printf("Step 1:");
        if(key < dataset[0] || key > dataset[DATASET_SIZE-1]){
            printf(" not found!\n");
            break;
        }else{
            printf(" search key in data domain.");
        }

        printf("\n");
        printf("Step 2:");
        int location;
        struct_binary_search(mapping_function, 0, num_function, &key, &location);
        printf("\n");
        printf("Step 3:");
        int a, b;
        //use next set of a and b if the key is bigger than the last max element
        if(key > mapping_function[location].max_element){
            a = mapping_function[location+1].a_value;
            b = mapping_function[location+1].b_value;
        }else{
            a = mapping_function[location].a_value;
            b = mapping_function[location].b_value;
        }

        int start = max_value(0, function_key(key, a, b)-prediction_error);
        int end = min_value(DATASET_SIZE - 1, function_key(key, a, b)+prediction_error)+1;
        int location_dataset;
        int result = binary_search(dataset, start, end, &key, &location_dataset);
        if(result == BS_FOUND){
            printf(" @ dataset[%d]!", location_dataset);
        }

        if(result == BS_NOT_FOUND){
            printf(" not found!");
        }

        printf("\n");
    }

    printf("\n");   
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
    printf(STAGE_HEADER, stage_num);
}

/* data swap function used by quick sort, adpated from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/sortscaffold.c
*/
void
swap_data(data_t *x1, data_t *x2) {
    data_t t;
    t = *x1;
    *x1 = *x2;
    *x2 = t;
}

/* partition function used by quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
partition(data_t dataset[], int n, data_t *pivot,
    int *first_eq, int *first_gt) {
    int next=0, fe=0, fg=n, outcome;

    while (next<fg) {
        if ((outcome = cmp(dataset+next, pivot)) < 0) {
            swap_data(dataset+fe, dataset+next);
            fe += 1;
            next += 1;  
        } else if (outcome > 0) {
            fg -= 1;
            swap_data(dataset+next, dataset+fg);
        } else {
            next += 1;
        }   
    }

    *first_eq = fe;
    *first_gt = fg;
    return;
}

/* quick sort function, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
quick_sort(data_t dataset[], int n) {
    data_t pivot;
    int first_eq, first_gt;
    if (n<=1) {
        return;
    }
    /* array section is non-trivial */
    pivot = dataset[n/2]; // take the middle element as the pivot
    partition(dataset, n, &pivot, &first_eq, &first_gt);
    quick_sort(dataset, first_eq);
    quick_sort(dataset + first_gt, n - first_gt);
}

/* comparison function used by binary search and quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
cmp(data_t *x1, data_t *x2) {
    return (*x1-*x2);
}

/* binary search between dataset[lo] and dataset[hi-1], adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
binary_search(data_t dataset[], int lo, int hi, 
		data_t* key, int *locn) {
    int mid, outcome;
	/* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
    if (lo>=hi) {
        return BS_NOT_FOUND;    
    }

    mid = (lo+hi)/2;
    printf(" %d", dataset[mid]); 
    if ((outcome = cmp(key, dataset+mid)) < 0) {
        return binary_search(dataset, lo, mid, key, locn);
    } else if (outcome > 0) {
        return binary_search(dataset, mid+1, hi, key, locn);
    } else {
        *locn = mid;
        return BS_FOUND;
    }

}

//calculate fkey value and return the smallest integer greater or equal to it
int
function_key(int key, int a, int b){
    if(b == 0){
        return a;
    }
    int res = (key + a) / b;
    if((key + a) % b != 0){
        res++;
    }
    return res;
}

//binary search in the struct array mapping_function
void
struct_binary_search(mapping_t mapping_function[], int lo, int hi, 
    	data_t* key, int* location) {
    int mid, outcome;
    /* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
    if (lo>=hi) {
        return ;
    }

    mid = (lo+hi)/2;
    printf(" %d", mapping_function[mid].max_element);
    *location = mid;
    if ((outcome = cmp(key, &(mapping_function+mid)->max_element)) < 0) {
        struct_binary_search(mapping_function, lo, mid, key, location);
    } else if (outcome > 0) {
        struct_binary_search(mapping_function, mid+1, hi, key, location);
    } else {
        return;
    }

}

//return the maximum value
int 
max_value(int first_value, int second_value){
    if(first_value > second_value){
        return first_value;
    }

    return second_value;
}

//return the minimum value
int 
min_value(int first_value, int second_value){
    if(first_value < second_value){
        return first_value;
    }

    return second_value;
}



