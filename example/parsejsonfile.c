#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

int keycount;

char * readjsonfile(const char* filename);

void printkeys(const char *json, jsmntok_t *t, int tokcount);
int findkeys(const char *json, jsmntok_t *t, int tokcount, int *keys);
void printvalues(const char *json, jsmntok_t *t, int tokcount, int *keys);
void printall(const char *json, jsmntok_t *t, int tokcount);

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
		
	int keyarray[128];
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	
	char * JSON_STRING = readjsonfile("data.json");
	printf("%s\n",JSON_STRING);
	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	printall(JSON_STRING, t, r);
//	printkeys(JSON_STRING, t, r);
	
//	keycount = findkeys(JSON_STRING, t, r, keyarray);
	
//	printvalues(JSON_STRING, t, r, keyarray);

	return EXIT_SUCCESS;
}

char * readjsonfile(const char* filename){
	char temp[100];
	char *return_ptr;	

	FILE * fp;
	fp = fopen(filename, "r");
	

	return_ptr = (char *)malloc(1);

	while(fgets(temp, sizeof(temp), fp) != NULL){
		return_ptr = (char *)realloc(return_ptr, strlen(return_ptr) + strlen(temp));
		strcat(return_ptr,temp);
	}
	

	fclose(fp);
	return return_ptr;
}

void printkeys(const char *json, jsmntok_t *t, int tokcount){
	int i;
	int n = 1;

	printf("****** All keys ******\n");

	for(i = 1; i  < tokcount; i++) {
		char *type = malloc(20*sizeof(char));
		int j, k;

		if(t[i].size == 0)
			continue;
		else if(t[i].type != 3)
			continue;

		if(t[i].type == 1)
			type = "JSMN_OBJECT";
		else if(t[i].type == 2)
			type = "JSMN_ARRAY";
		else if(t[i].type == 3)
			type = "JSMN_STRING";
		else if(t[i].type == 4)
			type = "JSMN_PRIMITIVE";
		else
			type = "UNDEFINED";
	
		char *subString = (char *)malloc(200*sizeof(char));
		
		for(j = 0; j < 200; j++)
		subString[j] = '\0';

		for(j = t[i].start; j < t[i].end; j++)
		subString[j-t[i].start]=json[j];

		subString[j] = '\0';

		printf("[%2d] %s (%d)\n", n++, subString, i);

		free(subString);
	}
	printf("\n");
}

int findkeys(const char *json, jsmntok_t *t, int tokcount, int *keys){

	int i;
	int n = 0;

	for(i = 1; i < tokcount; i++)
	{
		if(t[i].size==0)
			continue;
		else if(t[i].type != 3)
			continue;
		
		{
		keys[n] = i;
		n++;
		}
	}
	return n;
}

void printvalues(const char *json, jsmntok_t *t, int tokcount, int *keys){
	int i, j;
	int iskey;
	
	printf("****** Print values ******\n");

	for (i = 1; i < tokcount; i++) {
		iskey = 0;
		for(j = 0; j <keycount; j++)
		{
			if(i==keys[j])
			iskey = 1;
			break;
		}
		
		if(iskey == 1) {
			printf("- %.*s: ", t[i].end-t[i].start, json +t[i].start);
		}
		else {
			printf("%.*s\n", t[i].end-t[i].start, json + t[i].start);
		}

	}
}

void printall(const char *json, jsmntok_t *t, int tokcount){
	int i;
	
	printf("****** All Tokens ******\n");

	for (i = 1; i < tokcount; i++){
		printf("[%2d] ", i); 
		printf("%.*s ", t[i].end - t[i].start, json+t[i].start);
		printf("size=%d, %d~%d )\n", t[i].size, t[i].start, t[i].end);

	}	
/*	
	for(i = 1; i < tokcount; i++) {
	#ifdef JSMN_PARENT_LINKS
	printf("[%2d] %.*s (size=%d, &d~%d, %s) P=%d\n", i, t[i].end-t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type], t[i].parent);
	#else
	printf("[%2d] %.*s (size=%d, %d~%d, %s)\n", i, t[i].end-t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type]);
	#endif
	}

*/
	printf("\n");
} 
