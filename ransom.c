
#include <assert.h>
#include <limits.h>																
#include <math.h>																
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//defines and typedefs
typedef struct key key;
typedef struct map map;
#define SAME 1
#define FIVE 5


 /* key is the link list nodes
  * in each key we save
  * word -> a word from magazine or/and note.
  * appears_note -> how much times the word appeared in the note
  * appears_magazine -> how much times the word appeared in the magazine
  * next -> ptr to the next key in the map (install to Ox0 in the start)
  */
struct key {
    char *word;
    int appears_note;
    int appears_magazine;
    key *next;
};
/* map is the link list
 * head -> ptr of the first key of the map
 * tail -> ptr of the last key of the map (install to the head)
 */
struct map {
    key *head;
    key *tail;
};
// declartions
char* readline();
char** split_string(char*);
void print_yes_or_no(int state);
void checkMagazine(int magazine_count,
		           char** magazine, 
				   int note_count, 
				   char** note);
struct key *create_new_key(char **word_data ,int where);
void insert_into_map(struct map **map, char *word_data,int where);
void free_map (struct key *key);
void map_maker(struct map **map ,char **data ,int length, int where);
void chk_map(struct map *map);
int my_strcmp(char *str1, char *str2);



int main()
{
    char** mn = split_string(readline());

    char* m_endptr;															
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;															
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());						

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

/*prints "YES" or "NO" according to a flag input */

void print_yes_or_no(int state){
    if(!state){
    	printf("NO");
    }
    else{
    	printf("YES");
    }

}
/*the func insert the words from magazine and note to map (link list)
 * and then send the map to func that check if we can make a note
 * in the end free all the map.
 */
void checkMagazine(int magazine_count,
		           char** magazine,
				   int note_count,
				   char** note) {
/*tell what we insert to the map*/
    int where = 1;
    char *temp_str=*magazine;
    struct map *map=malloc(sizeof(struct map));
    insert_into_map(&map,temp_str,where);
/*insert the first key to the map
 * insert the rest of magazine to the map*/
    map_maker( &map , (magazine +1) , magazine_count, where);
/*insert note to the map*/
    map_maker( &map , note ,note_count,!(where));
/* check if we can make note out of magazine */
    chk_map(map);
/*free the map keys*/
    free_map(map -> head);
/*free map ptr */
    free(map);
}

/* creates a new key and returns it.
 * if where=0 ->insert to magazine
 * if where=1 ->insert to note
 */
struct key *create_new_key(char **word_data ,int where){
	struct key *key_ptr = malloc(sizeof(struct key));
	key_ptr -> word = *word_data;
	if (where){
		key_ptr -> appears_magazine = 1;
        key_ptr -> appears_note = 0;
	}
	else {
		key_ptr -> appears_note = 1;
        key_ptr -> appears_magazine = 0;
	}
		
	key_ptr -> next = NULL;
	return key_ptr;
	
}
/*insert new key into the map  */

void insert_into_map(struct map **map, char *word_data,int where){
	struct key *key_ptr = create_new_key(&word_data,where);
	if(!(*map)->head){
		(*map) -> head = key_ptr;
	}
	else {
		(*map) -> tail -> next = key_ptr;
	}
	(*map) -> tail = key_ptr;
			
}

/*free the map from a given key*/
void free_map (struct key *key){
	while (key){
		struct key *temp = key;
		key = key -> next;
		free(temp);
	}
}

/*creates the maps without make double keys
 * temp-> the current word that save in data
 * str1->the current word that save in the current key
 * a-> get 1 if temp and str1 is the same and 0 if isn't
 * where -> if we working on magazine or note;
 */
void map_maker(struct map **map ,char **data ,int length, int where){
    for(int i=0 ;i < length;i++){
        struct key *curr_key = ((*map) -> head);
    	char *temp = *(data +i);
/* if we get to the end of data "\0" we break for, the loop*/
    	if (!temp){
    	    break;
    	}
    	else{
    		while ((curr_key)!= NULL ){
/* we check if the 2 strings if the same and if they the same
 * we update the filed in the current key
 */
				char *str1= (curr_key -> word);
				int a = my_strcmp(str1,temp);
				if (a){
					if (where){
						curr_key -> appears_magazine
						= (curr_key -> appears_magazine) +1;
					}
					else{
						curr_key -> appears_note
						= (curr_key -> appears_note) +1;
					}
					break;
				}
/*if the strings arn't the same we continue going on the map keys
*until we find  word that is the same as the word in data
*or we arrived to the end of the map and insert a new key
*that in the key word we put the word in data
*update curr_key to the next key (or the end if inserting new key
*/
				else {
					if (!(curr_key -> next)){
						insert_into_map(map ,temp, where);
						curr_key = curr_key -> next ->next;
					}
					else{
						curr_key = curr_key -> next;
					}
				}
    		}
    	}
    }
}
/* we check if for any appear of word in note there is appear in magazine
 * if there more appears of a word in note then magazine we sent to
 * print_yes_or_no -> 0 can't make a note
 * if there is the same appears in magazine we continue to the next key
 * if we went out of the loop -> we can make a note
 * print_yes_or_no ->1 can make a note
 */
void chk_map(struct map *map){
    int can_make_note = 1;
    key *curr_key = (map -> head);
    while(curr_key != NULL){
        if((curr_key -> appears_magazine) < (curr_key -> appears_note)){
            can_make_note = !(can_make_note);
            break;
        }
        else{
            curr_key = curr_key -> next;
        }
    }
    print_yes_or_no(can_make_note);
}

/* compering between 2 strings
 * the max chars of string is 5
 */

int my_strcmp(char *str1, char *str2){
/* check if the string isn't empty */
    if((str1==NULL)||(str2==NULL)){
    	return !SAME;
    }

    else{
        int i=0;
/* Subtract between the 2 strings and if the result isn't 0
 *  not the same return not the same
 *  else continue to go over the strings if reach /0
 *  return the same
 */
        while(str1[i] || str2[i]){
        	if(!!(str1[i]-str2[i])) {
                return !SAME;
             }
             else{
                 i++;
             }
          }
    }
    return SAME;
}
