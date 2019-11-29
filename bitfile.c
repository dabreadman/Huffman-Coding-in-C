// C code file for  a file ADT where we can read a single bit at a
// time, or write a single bit at a time

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitfile.h"

// open a bit file in "r" (read) mode or "w" (write) mode
struct bitfile *bitfile_open(char *filename, char *mode) {
  struct bitfile *result = malloc(sizeof(struct bitfile));
  result->file = fopen(filename, mode);
  result->buffer = 0;
  result->index = 0;
  result->is_EOF = 0;
  result->is_read_mode = strcmp(mode, "r") == 0;
  return result;
}

// write a bit to a file; the file must have been opened in write mode
void bitfile_write_bit(struct bitfile *this, int bit) {
  this->buffer = this->buffer | bit << this->index;
  this->index++;

  if (this->index == 8) {
    this->buffer = fputc(this->buffer, this->file);
    this->buffer = 0;
    this->index = 0;
  }
}

// read a bit from a file; the file must have been opened in read mode
int bitfile_read_bit(struct bitfile *this) {
  if (this->index == 0) {
    this->buffer = fgetc(this->file);
  }
  int bit = this->buffer & 1;
  this->buffer >>= 1;
  this->index++;
  if (this->index == 8)
    this->index = 0;
  return bit;
}

// close a bitfile; flush any partially-filled buffer if file is open
// in write mode
void bitfile_close(struct bitfile *this) {
  if (this->index != 0 || this->buffer != 0) {
    fputc(this->buffer, this->file);
  }
  fclose(this->file);
  free(this);
}

// check for end of file
int bitfile_end_of_file(struct bitfile *this) {
  this->is_EOF = feof(this->file);
  return this->is_EOF;
}
