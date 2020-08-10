//
//  test-utils.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../src/bc-ur.hpp"

#define assert_throws(X) \
    { bool thrown = false; try { X; } catch(...) { thrown = true; } assert(thrown); }

ur::ByteVector make_message(size_t len, const std::string& seed = "Wolf");
ur::UR make_message_ur(size_t len, const std::string& seed = "Wolf");

#endif /* TEST_UTILS_H */
