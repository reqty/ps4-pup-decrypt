
#ifndef __DECRYPT_H__
#define __DECRYPT_H__
#include "ps4.h"
#include "pup.h"
#include "bls.h"
#include "decryptio.h"
#include "encryptsrv.h"
#include "defines.h"
#include "pupdebug.h"


int verify_segment(const decrypt_state* state, int index, pup_segment* segment, int additional);
int verify_segments(const decrypt_state* state, pup_segment* segments, int segment_count);

int decrypt_segment(const decrypt_state* state, uint16_t index, pup_segment* segment);
int decrypt_segment_blocks(const decrypt_state * state, uint16_t index, pup_segment* segment,
                           uint16_t table_index, pup_segment* table_segment);

int find_table_segment(int index, pup_segment* segments, int segment_count,
                       int* table_index);

int decrypt_pup_data(const decrypt_state * state);
void decrypt_pup(decrypt_state * state, const char * OutputPath);
void decrypt_pups(const char * InputPath, const char * OutputPath);


#endif
