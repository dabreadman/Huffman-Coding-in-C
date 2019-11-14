// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
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

//find the lowest two frequency characters
void lowestTwo(struct huffcoder* this){
	//find the index of lowest two nodes
	int * index = calloc(2,sizeof(int));
	index[0]=0;
	index[1]=1;

	for(int i=0;i<this->n_node;i++){
		if(this->list[i]->freq <= this->list[index[0]]->freq 
				&& this->list[i]->seqno < this->list[index[0]]->seqno){
			index[0]=this->list[i];
}
		else if(this->list[i]->freq <= this->list[index[1]]->freq 
				&& this->list[i]->seqno < this->list[index[1]]->seqno){
			index[1]=this->list[i];
}
	}

	//checks if node 1 is smaller than node 2
	if((this->list[index[0]]->freq > this->list[index[1]]->freq) || 
			((this->list[index[0]]->freq == this->list[index[1]]->freq) && 
			(this->list[index[0]]->seqno >this->list[index[1]]->seqno))){
		int temp = index[0];
		index[0]=index[1];
		index[1]=temp;	       
	}
 //:   printf("Index 1: %f, Index 2: %f",index[0],index[1]);	
/*	//create a union huffchar
	int temp2 = this->list[index[0]]->freq;
	int temp3 = this->list[index[1]];
	printf("Creating a node of frequency %d + %d\n" ,temp2, temp3);

	this->list[index[0]]= new_union_huff(this->list[index[0]]->freq+this->list[index[1]]->freq,
			this->n_seq++, this->list[index[0]],this->list[index[1]]);
	this->list[index[1]]=this->list[--this->n_node];*/
}



// create a new huffcoder structure
struct huffcoder *  huffcoder_new(){
	struct huffcoder * result = calloc(1,sizeof(struct huffcoder));
	for(int i=0; i< NUM_CHARS;i++){
		result->freqs[i]=0;
		result->code_lengths[i]=0;
		result->codes[i]=0;
	}
	result->n_seq = calloc(1,sizeof(int));
	result->n_seq = NUM_CHARS;
	result->n_node= calloc(1,sizeof(int));
	result->n_node = NUM_CHARS;
	result->tree = malloc(sizeof(struct huffchar));

	result->list = malloc(NUM_CHARS*sizeof(struct huffchar *));
	for(int i=0;i<NUM_CHARS;i++){
		result->list[i]=new_char_huff(result->freqs[i],i,i);
	}

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
	for (int i = 0; i < NUM_CHARS; i++)
		if (this->freqs[i] == 0)
			this->freqs[i] = 1;
	fclose(file);
}


// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
//	while(this->n_node>1){
		lowestTwo(this);	
//	}
//	this->tree = this->list[0];
}


// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object

void huffcoder_tree2table(struct huffcoder * this)
{
	huffchar_tree2table(this->tree);
}	

// using a huffchar node
// with the huffcoder object
void huffchar_tree2table(struct huffchar * this)
{
	if(this->is_compound==0)
	printf("%d\n",this->u.c);
	else{
	huffchar_tree2table(this->u.compound.left);
	huffchar_tree2table(this->u.compound.right);
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
		for ( j = this->code_lengths[i]-1; j >= 0; j--) {
			buffer[j] = ((this->codes[i] >> j) & 1) + '0';
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
