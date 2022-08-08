#ifndef BC_UR_CDECODER_H
#define BC_UR_CDECODER_H

#include <stddef.h>

#define URDECODER_SIZE 288

int urcreate_decoder(void** const decoder);
void urcreate_placement_decoder(void* const decoder, size_t decoder_len);
void urfree_decoder(void* const decoder);
void urfree_placement_decoder(void* const decoder);

bool urreceive_part_decoder(void* const decoder, const char* string);
bool uris_success_decoder(void* const decoder);
bool uris_failure_decoder(void* const decoder);
bool uris_complete_decoder(void* const decoder);
void urresult_ur_decoder(void* const decoder, uint8_t** result, size_t* result_len, const char** type);

#endif // BC_UR_CDECODER_H
