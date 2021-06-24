/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Parse.cpp
 *
 *  Created on: 24 May
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Bytes
 *
 */
#include "Pufferfish/Util/Bytes.h"

#include <cstddef>

#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The get_byte function in Bytes correctly gets bytes from the given position") {
  GIVEN("get_byte function") {
//value = 0x86
    WHEN(
        "the function is called with byte index 0 and input type uint8_t as the template "
        "parameters and with 0x86 as the input value ") {
      const uint8_t number = 0x86;
      auto value = PF::Util::get_byte<0, uint8_t>(number);
      THEN("the function returns the value at index 0, which is 0x86") { REQUIRE(value == 0x86); }
    }
//value = 0x86
    WHEN(
        "the function is called with byte index 1 and input type uint8_t as the template "
        "parameters and with 0x86 as the input value") {
      const uint8_t number = 0x86;      
      auto value = PF::Util::get_byte<1, uint8_t>(number);
      THEN("the function returns the value at index 1, which is 0x00") { REQUIRE(value == 0x00); }
    }

  
//value = 0x860a
    WHEN(
        "the function is called with byte index 0 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      uint16_t number = 0x860a;
      auto value1 = PF::Util::get_byte<0, uint16_t>(number);
      THEN("the function returns the value at index 0, which is 0x0a") { REQUIRE(value1 == 0x0a); }
    }
//value = 0x860a
    WHEN(
        "the function is called with byte index 1 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      uint16_t number = 0x860a;      
      auto value = PF::Util::get_byte<1, uint16_t>(number);
      THEN("the function returns the value at index 1, which is 0x86") { REQUIRE(value == 0x86); }
    }
//value = 0x860a
    WHEN(
        "the function is called with byte index 2 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      uint16_t number = 0x860a;      
      auto value = PF::Util::get_byte<2, uint16_t>(number);
      THEN("the function returns the value at index 2, which is 0x00") { REQUIRE(value == 0x00); }
    }
//value = 0x860a
// this test case fails, the byte index is greater than size of the type
    // WHEN("the function is called with byte index 2 and input type uint8_t as the template parameters and with 0x860a as the input value"){
    //   uint16_t number = 0x860a;      
    //   auto value = PF::Util::get_byte<2, uint8_t>(number);
    //   THEN("the function returns the value at index 2, which is 0x00"){
    //     REQUIRE(value == 0x00);
    //   }
    // }
  
//value = 0x86010f0a    
    WHEN(
        "the function is called with byte index 0 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      uint32_t number = 0x86010f0a;
      auto value = PF::Util::get_byte<0, uint32_t>(number);
      THEN("the function returns the value at index 0, which is 0x0a ") { REQUIRE(value == 0x0a); }
    }
//value = 0x86010f0a 
    WHEN(
        "the function is called with byte index 1 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      uint32_t number = 0x86010f0a;
      auto value1 = PF::Util::get_byte<1, uint32_t>(number);
      THEN("the function returns the value at index 1, which is 0x0f") { REQUIRE(value1 == 0x0f); }
    }
//value = 0x86010f0a 
    WHEN(
        "the function is called with byte index 2 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      uint32_t number = 0x86010f0a;
      auto value2 = PF::Util::get_byte<2, uint32_t>(number);
      THEN("the function returns the value at index 2, which is 0x01") {
         REQUIRE(value2 == 0x01); 
         }
    }
//value = 0x86010f0a 
    WHEN(
        "the function is called with byte index 3 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      uint32_t number = 0x86010f0a;
      auto value3 = PF::Util::get_byte<3, uint32_t>(number);
      THEN("the function returns the value at index 3, which is 0x86") { REQUIRE(value3 == 0x86); }
    }
//value = 0x86010f0a 
// this test case fails, the byte index is greater than size of the type
    // WHEN("the function is called with byte index 2 and input type uint8_t as the template parameters and with 0x86010f0a as the input value "){
    //   uint32_t number = 0x86010f0a;
    //   auto value =PF::Util::get_byte<2, uint8_t>(number);
    //   THEN("the function returns the value at index 2, which is 0x00") {
    //     REQUIRE(value == 0x00);
    //   }
    // }
//value = 0x86010f0a     
// this test case fails, the byte index is greater than size of the type
    // WHEN("the function is called with byte index 3 and input type uint16_t as the template parameters and with 0x86010f0a as the input value "){
    //   uint32_t number = 0x86010f0a;
    //   auto value =PF::Util::get_byte<2, uint16_t>(number);
    //   THEN("the function returns the value at index 2, which is 0x00") {
    //     REQUIRE(value == 0x00);
    //   }
    // }
  }
}

SCENARIO("The  set_byte function correctly sets bytes to the given position") {
  GIVEN("Util::Bytes set_byte function ") {
//value = 0x83    
    WHEN(
        "the function is called with byte index 0 and input type uint8_t as the template "
        "parameters and with 0x83 as the input value ") {
      const uint8_t number = 0x83;
      auto value = PF::Util::set_byte<0, uint8_t>(number);
      THEN("the function returns the value at index 0, which is 0x86") { REQUIRE(value == 0x83); }
    }
//value = 0x83 
    WHEN(
        "the function is called with byte index 1 and input type uint8_t as the template "
        "parameters and with 0x83 as the input value") {
      const uint8_t number = 0x83;
      auto value = PF::Util::set_byte<1, uint8_t>(number);
      THEN("the function returns the value at index 1, which is 0x00") { REQUIRE(value == 0x00); }
    }
  
//value = 0x86
    WHEN(
        "the function is called with byte index 0 and on type uint16_t as the template parameters "
        "and with 0x86 as the input value") {
      const uint8_t number = 0x86;
      auto value = PF::Util::set_byte<0, uint16_t>(number);
      THEN("the function returns the value at index 0, which is 0x86") { REQUIRE(value == 0x86); }
    }
//value = 0x86
    WHEN(
        "the function is called with byte index 1 and on type uint16_t as the template parameters "
        "and with 0x86 as the input value") {
      const uint8_t number = 0x86;
      auto value = PF::Util::set_byte<1, uint16_t>(number);
      THEN("the function returns the value at index 1, which is 0x8600") { REQUIRE(value == 0x8600); }
    }
//value = 0x59
    WHEN(
        "the function is called with byte index 2 and on type uint32_t as the template parameters "
        "and with 0x59 as the input value") {
      const uint8_t number = 0x59;
      auto value = PF::Util::set_byte<2, uint32_t>(number);
      THEN("the function returns the value at index 2, which is 0x590000") { REQUIRE(value == 0x590000); }
    }
//value = 0x59
    WHEN(
        "the function is called with byte index 3 and on type uint32_t as the template parameters "
        "and with 0x59 as the input value") {
      const uint8_t number = 0x59;
      auto value = PF::Util::set_byte<3, uint32_t>(number);
      THEN("the function returns the value at index 3, which is 0x59000000") { REQUIRE(value == 0x59000000); }
    }

// this test case fails, the byte index is greater than size of the type    
    // WHEN(
    //     "the function is called with byte index 3 and on type uint32_t as the template parameters "
    //     "and with 0x59 as the input value") {
    //   const uint8_t number = 0x59;
    //   auto value = PF::Util::set_byte<5, uint32_t>(number);
    //   THEN("the function returns the value at index 3, which is 0x59000000") { REQUIRE(value == 0x59000000); }
    // }

  }
}
