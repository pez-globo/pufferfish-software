/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * catch2.cpp
 *
 *  Created on: July 14, 2020
 *      Author: Ethan Li
 *
 * Unit tests to confirm that catch2 will find and run tests.
 *
 */

#include "catch2/catch.hpp"

TEST_CASE("catch2 runs", "[catch2]") {
  REQUIRE(true);
}
