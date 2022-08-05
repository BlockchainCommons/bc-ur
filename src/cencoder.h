#ifndef BC_UR_CENCODER_H
#define BC_UR_CENCODER_H

#include <stddef.h>

void urcreate_encoder(void** const encoder, const char* type, const uint8_t* cbor, size_t cbor_len, size_t max_fragment_len, uint32_t first_seq_num, size_t min_fragment_len);
void urfree_encoder(void* const encoder);
bool uris_complete_encoder(void* const encoder);
bool uris_single_part_encoder(void* const encoder);
void urnext_part_encoder(void* const encoder, char** next);
void urfree_encoded_encoder(char * encoded);

#endif // BC_UR_CENCODER_H

