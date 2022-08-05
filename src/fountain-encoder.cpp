//
//  fountain-encoder.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "fountain-encoder.hpp"
#include <assert.h>
#include <cmath>
#include <optional>
#include <vector>
#include <limits>
#include <cbor.h>
using namespace std;

namespace ur {

size_t FountainEncoder::find_nominal_fragment_length(size_t message_len, size_t min_fragment_len, size_t max_fragment_len) {
    assert(message_len > 0);
    assert(min_fragment_len > 0);
    assert(max_fragment_len >= min_fragment_len);
    auto max_fragment_count = message_len / min_fragment_len;
    optional<size_t> fragment_len;
    for(size_t fragment_count = 1; fragment_count <= max_fragment_count; fragment_count++) {
        fragment_len = size_t(ceil(double(message_len) / fragment_count));
        if(fragment_len <= max_fragment_len) {
            break;
        }
    }
    assert(fragment_len.has_value());
    return *fragment_len;
}

vector<ByteVector> FountainEncoder::partition_message(const ByteVector &message, size_t fragment_len) {
    auto remaining = message;
    vector<ByteVector> fragments;
    while(!remaining.empty()) {
        auto a = split(remaining, fragment_len);
        auto fragment = a.first;
        remaining = a.second;
        auto padding = fragment_len - fragment.size();
        while(padding > 0) {
            fragment.push_back(0);
            padding--;
        }
        fragments.push_back(fragment);
    }
    return fragments;
}

FountainEncoder::Part::Part(const ByteVector& cbor) {

   CborParser parser;
   CborValue value;
   CborError cberr = cbor_parser_init(&cbor[0], cbor.size(), CborValidateCompleteData, &parser, &value);
   if (cberr != CborNoError || !cbor_value_is_array(&value)) {
       abort();
   }
   size_t array_len = 0;
   cberr = cbor_value_get_array_length(&value, &array_len);
   if (cberr != CborNoError || array_len != 5) {
       abort();
   }

   CborValue arrayItem;
   cberr = cbor_value_enter_container(&value, &arrayItem);
   if (cberr != CborNoError || !cbor_value_is_valid(&arrayItem) || !cbor_value_is_unsigned_integer(&arrayItem)) {
       abort();
   }

   uint64_t n;
   cberr = cbor_value_get_uint64(&arrayItem, &n);
   if (cberr != CborNoError) {
       abort();
   }
   cberr = cbor_value_advance(&arrayItem);
   if (cberr != CborNoError || !cbor_value_is_valid(&arrayItem) || !cbor_value_is_unsigned_integer(&arrayItem)) {
       abort();
   }
   if(n > std::numeric_limits<decltype(seq_num_)>::max()) {
       abort();
   }
   seq_num_ = n;
   cberr = cbor_value_get_uint64(&arrayItem, &n);
   if (cberr != CborNoError) {
       abort();
   }
   if(n > std::numeric_limits<decltype(seq_len_)>::max()) { 
       abort();
   }
   seq_len_ = n;

   cberr = cbor_value_advance(&arrayItem);
   if (cberr != CborNoError || !cbor_value_is_valid(&arrayItem) || !cbor_value_is_unsigned_integer(&arrayItem)) {
       abort();
   }
   cberr = cbor_value_get_uint64(&arrayItem, &n);
   if (cberr != CborNoError) {
       abort();
   }
   cberr = cbor_value_advance(&arrayItem);
   if (cberr != CborNoError || !cbor_value_is_valid(&arrayItem) || !cbor_value_is_unsigned_integer(&arrayItem)) {
       abort();
   }
   if(n > std::numeric_limits<decltype(message_len_)>::max()) {
       abort();
   }
   message_len_ = n;
   cberr = cbor_value_get_uint64(&arrayItem, &n);
   if (cberr != CborNoError) {
       abort();
   }
   cberr = cbor_value_advance(&arrayItem);
   if (cberr != CborNoError || !cbor_value_is_valid(&arrayItem) || !cbor_value_is_byte_string(&arrayItem)) {
       abort();
   }

   if(n > std::numeric_limits<decltype(checksum_)>::max()) {
       abort();
   }
   checksum_ = n;

   size_t byteLen = 0;
   cberr = cbor_value_get_string_length(&arrayItem, &byteLen);
   if (cberr != CborNoError || byteLen == 0) {
       abort();
   }
   const size_t oldsize = data_.size();
   data_.reserve(byteLen + data_.size());
   data_.resize(byteLen + data_.size());
   cberr = cbor_value_copy_byte_string(&arrayItem, &data_[oldsize], &byteLen, NULL);
   if (cberr != CborNoError || byteLen == 0) {
       abort();
   }
}

ByteVector FountainEncoder::Part::cbor() const {

    ByteVector result;
    auto data_res = data();
    // leave some extra headroom (13 minimum)
    result.reserve(30 + data_res.size());
    result.resize(30 + data_res.size());

    CborEncoder root_encoder;
    cbor_encoder_init(&root_encoder, &result[0], result.size(), 0);
    CborEncoder array_encoder;
    CborError cberr = cbor_encoder_create_array(&root_encoder, &array_encoder, 5);
    assert(cberr == CborNoError);
    cberr = cbor_encode_uint(&array_encoder, seq_num());
    assert(cberr == CborNoError);
    cberr = cbor_encode_uint(&array_encoder, seq_len());
    assert(cberr == CborNoError);
    cberr = cbor_encode_uint(&array_encoder, message_len());
    assert(cberr == CborNoError);
    cberr = cbor_encode_uint(&array_encoder, checksum());
    assert(cberr == CborNoError);
    cberr = cbor_encode_byte_string(&array_encoder, &data_res[0], data_res.size());
    assert(cberr == CborNoError);

    cberr = cbor_encoder_close_container(&root_encoder, &array_encoder);
    assert(cberr == CborNoError);

    const size_t cbor_len = cbor_encoder_get_buffer_size(&root_encoder, &result[0]);
    assert(cbor_len <= result.size());
    result.resize(cbor_len);

    return result;
}

FountainEncoder::FountainEncoder(const ByteVector& message, size_t max_fragment_len, uint32_t first_seq_num, size_t min_fragment_len) {
    assert(message.size() <= std::numeric_limits<uint32_t>::max());
    message_len_ = message.size();
    checksum_ = crc32_int(message);
    fragment_len_ = find_nominal_fragment_length(message_len_, min_fragment_len, max_fragment_len);
    fragments_ = partition_message(message, fragment_len_);
    seq_num_ = first_seq_num;
}

ByteVector FountainEncoder::mix(const PartIndexes& indexes) const {
    ByteVector result(fragment_len_, 0);
    for(auto index: indexes) { xor_into(result, fragments_[index]); }
    return result;
}

FountainEncoder::Part FountainEncoder::next_part() {
    seq_num_ += 1; // wrap at period 2^32
    auto indexes = choose_fragments(seq_num_, seq_len(), checksum_);
    auto mixed = mix(indexes);
    return Part(seq_num_, seq_len(), message_len_, checksum_, mixed); 
}

string FountainEncoder::Part::description() const {
    return "seqNum:" + to_string(seq_num_) + ", seqLen:" + to_string(seq_len_) + ", messageLen:" + to_string(message_len_) + ", checksum:" + to_string(checksum_) + ", data:" + data_to_hex(data_);
}

}
