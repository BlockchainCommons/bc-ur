//
//  ur.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "ur.hpp"
#include "utils.hpp"

#include <iostream>
using namespace std;

namespace ur {

UR::UR(const std::string &type, const ByteVector &cbor)
    : type_(type), cbor_(cbor)
{}

bool UR::is_valid() const {
    return !cbor_.empty() && !type_.empty() && is_ur_type(type_);
}

bool operator==(const UR& lhs, const UR& rhs) {
    return lhs.type() == rhs.type() && lhs.cbor() == rhs.cbor();
}

} // namespace ur
