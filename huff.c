// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>

#include "huff.h"
#include "bitfile.h"


struct huffchar * new_char_huff(int f, int seq, char c){
    struct huffchar * result = malloc(sizeof(struct huffchar));
	result->freq = f;
	result->is_compound = 0;
	result->seqno = seq;
	result->u.c = c;
	return result;
}

struct huffchar * new_union_huff(int f, int seq,struct huffchar* l, struct huffchar* r){
    struct huffchar * result = malloc(sizeof(struct huffchar));
	result->freq = f;
	result->is_compound = 1;
	result->seqno = seq; 
	result->u.compound.left = l;
	result->u.compound.right = r;
	return result;
}

struct huffchar * removeLowest(struct huffcoder * coder) {
    int index = 0;
    for (int i=0; i<coder->n_node;i++){
        if(coder->list[i]->freq < coder->list[index]->freq)
            index = i;
        else if (coder->list[i]->freq == coder->list[index]->freq && 
                coder->list[i]->seqno < coder->list[index]->seqno)
            index = i;
    }    

    struct huffchar * result = coder->list[index];
    coder->n_node--;
    coder->list[index]=coder->list[coder->n_node];
    return result;
}



// create a new huffcoder structure
struct huffcoder *  huffcoder_new(){
	struct huffcoder * result = malloc(sizeof(struct huffcoder));
	for(int i=0; i< NUM_CHARS;i++){
		result->freqs[i]=0;
		result->code_lengths[i]=0;
		result->codes[i]=0;
	}
	result->n_seq = NUM_CHARS;
	result->n_node = NUM_CHARS;
	result->tree = malloc(sizeof(struct huffchar));

	result->list = malloc(NUM_CHARS*sizeof(struct huffchar *));

	return result;
}


// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
	unsigned char c;  // we need the character to be
	// unsigned to use it as an index
	FILE * file;

	file = fopen(filename, "r");
	assert( file != NULL );
	c = fgetc(file);	// attempt to read a byte
    while( !feof(file) ) {
		c = fgetc(file);
		this->freqs[c]++;
	}
    //set 0's -> 1s
	for (int i = 0; i < NUM_CHARS; i++)
		if (this->freqs[i] == 0)
			this->freqs[i] = 1;
	fclose(file);

	for(int i=0;i<NUM_CHARS;i++){
		this->list[i]=new_char_huff(this->freqs[i],i,i);
	}
}


// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
    
    while(this->n_node>1){
       struct huffchar * huffc1 = removeLowest(this);
       struct huffchar * huffc2 = removeLowest(this);
       int freq= huffc1->freq + huffc2->freq;
       this->list[this->n_node] =
           new_union_huff(freq, this->n_seq++,huffc1,huffc2);
       this->n_node++;
    }

    this->tree = this->list[0];
}

void huffchar_tree2table_itr(struct huffcoder *coder, struct huffchar * this, uint64_t code,uint64_t length);

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object

void huffcoder_tree2table(struct huffcoder * this)
{
    huffchar_tree2table_itr(this, this->tree, 0,0);

}	
// using a huffchar node
// with the huffcoder object
void huffchar_tree2table_itr(struct huffcoder * coder, struct huffchar * this, uint64_t code, uint64_t length)
{
    if(!this->is_compound){
        coder->codes[this->u.c] = code;
        coder->code_lengths[this->u.c]=length;
    } 
    else{
        code<<=1;
        huffchar_tree2table_itr(coder, this->u.compound.left,code,length+1);
        huffchar_tree2table_itr(coder, this->u.compound.right, code+1,length+1);
    }
}


// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder * this)
{
	int i, j;
	char buffer[NUM_CHARS];

	for ( i = 0; i < NUM_CHARS; i++ ) {
		// put the code into a string
		assert(this->code_lengths[i] < NUM_CHARS);
        int k = 0;
		for ( j = this->code_lengths[i]-1; j >= 0; j--) {
			buffer[k++] = ((this->codes[i] >> j) & 1) + '0';
		}
		// don't forget to add a zero to end of string
		buffer[this->code_lengths[i]] = '\0';

		// print the code
		printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], buffer);;
	}
}



// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
		char * output_filename)
{

}

// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
		char * output_filename)
{

}
