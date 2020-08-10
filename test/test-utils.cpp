//
//  test-utils.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "test-utils.hpp"

using namespace ur;

ByteVector make_message(size_t len, const std::string& seed) {
    auto rng = Xoshiro256(seed);
    return rng.next_data(len);
}

UR make_message_ur(size_t len, const std::string& seed) {
    auto message = make_message(len, seed);
    ByteVector cbor;
    CborLite::encodeBytes(cbor, message);
    return UR("bytes", cbor);
}
