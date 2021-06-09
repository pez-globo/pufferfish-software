/*
 * Vector.tpp
 * Test1 
 *  Created on: April 07, 2021
 *      Author: Raavi Lagoo
 *
 * */

#pragma once

#include <cstring>

#include "Pufferfish/Util/Vector.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;


//constexpr size_t buffer_size = 252UL;
SCENARIO( " The function in Util Vectors works correctly" ) {
  GIVEN("Vector with empty internal buffer with 256 buffer size"){
  constexpr size_t buffer_size = 256UL;
  PF::Util::Vector<uint8_t,buffer_size> vector1;
      WHEN("A vector of 256UL is passed as input and output is 5UL vector"){
        auto push_back = vector1.push_back(0x06);
        auto re_vector = vector1.resize(5);
        THEN("Vector is resized correctly to 256 "){
         REQUIRE("re_vector = PF::IndexStatus::ok");
         REQUIRE(vector1[0] == 0x06);
        }
        THEN("Returns correct avaliable size"){
         REQUIRE(vector1.available() == 251);
        }
        THEN("Vector size is as expectd"){
          REQUIRE(vector1.size() == 5);
        }
        THEN("Returns expected 255 for avaliable function"){
          REQUIRE(vector1.full() == false);
        }
        THEN("Returns ok for erase"){
          REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
        }
        THEN("Returns out of bounds for , for out of bound index"){
          REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
        }
      }

      WHEN("A vector of 256UL is passed as inpput"){
        auto push_back = vector1.push_back(0x06);
        auto re_vector = vector1.resize(400);
        THEN("Result is as expected"){
          REQUIRE(re_vector == PF::IndexStatus::out_of_bounds);
          REQUIRE(vector1[0] == 0x06);
          REQUIRE(vector1.size() == 1);
        }
      }


      WHEN("A vector of 2UL size is passed as input"){
        constexpr size_t buffer_size = 10UL;
        PF::Util::Vector<uint8_t,buffer_size> vector;
        auto data = std::string("\xe3\x03\x08\x0f\x0a\xff\x83");
        for (auto& ch : data) {
            vector.push_back(ch);
        }
        auto re_vector = vector.resize(4);
        THEN("Resize returns ok"){
          REQUIRE(re_vector == PF::IndexStatus::ok);
        }
        THEN("Returns correct avaliable size"){
          REQUIRE(vector.available() == 6);        ;
        }
        THEN("Returns correct size"){
          REQUIRE(vector.size() == 4);
        }
        THEN("Returns false for full functin"){
          REQUIRE(vector.full() == false);

        }
        THEN("Returned vector has correct data in every index[0xe3,0x03,0x08,0x0f]"){
          REQUIRE(vector[0] == 0xe3);
          REQUIRE(vector[1]== 0x03);
          REQUIRE(vector[2] == 0x08 );
          REQUIRE(vector[3] == 0x0f);
        }

        THEN("Returns ok for erase function for index 1"){
          REQUIRE(vector.erase(1) == PF::IndexStatus::ok);
        }
        THEN("Returns out of bound for erase function for index 5"){
          REQUIRE(vector.erase(5) == PF::IndexStatus::out_of_bounds);
        }

      }


  }

}


SCENARIO("The function in Vector push_back works correctly"){
  GIVEN("Vector with an empty internal buffer with capacity 8"){
    constexpr size_t buffer_size = 8UL;
    PF::Util::Vector<uint8_t,buffer_size> vector1;
    WHEN("vector of capacity 8 is passed as input and output is called"){
        for(int i =0;i<=6;i++){
          auto push_back = vector1.push_back(i);
        }
        auto push_back = vector1.push_back(0x08);
      THEN("Returns expected vector [0,1,2,3,4,5,6]"){
        REQUIRE(push_back == PF::IndexStatus::ok);
        REQUIRE(vector1[7] == 0x08);
      }
      THEN("Returns expected size 8"){
        REQUIRE(vector1.size()== 8);
      }
      THEN("Returns expected avaliable buffer"){
        REQUIRE(vector1.available() == 0);
      }
      THEN("Returns expected true for full function"){
        REQUIRE(vector1.full() == true);
      }
      THEN("Returns ok for erase function used on index 5 "){
        REQUIRE(vector1.erase(5) == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for erase function used on index 10"){
        REQUIRE(vector1.erase(10) == PF::IndexStatus::out_of_bounds);
      }
    }
  }  
  
}

SCENARIO("The function in Vector: copy_from works correctly"){
  GIVEN("Vector with completely filled buffer with 4UL capacity"){
    constexpr size_t buffer_size = 4UL;
    PF::Util::Vector<uint8_t,buffer_size> vector1;
        PF::Util::Vector<uint8_t,buffer_size> input_buffer;
        auto data = std::string("\x08\x00\x04\x89");
        for (auto& ch : data) {
            input_buffer.push_back(ch);
        }
    WHEN("Vector of 4UL is passed as input and output is called"){
            auto vector_copy = vector1.copy_from(
          input_buffer.buffer(), input_buffer.size(), 0);

     THEN("Vector is copied correctly"){
       REQUIRE(vector_copy == PF::IndexStatus::ok);
      }
    }

    WHEN("Vector of 4UL is passed as input and output is called for dest index 3" ){
      auto vector_copy = vector1.copy_from(
       input_buffer.buffer(), input_buffer.size(), 3);
       THEN("Returns vector as expected "){
         REQUIRE(vector_copy == PF::IndexStatus::ok);
         REQUIRE(input_buffer.operator[](0) == 0x08);
         REQUIRE(input_buffer.operator[](5) == 0);
       }
       THEN("Returns currect size of vector"){
         REQUIRE(input_buffer.size() == 1);
       } 
       THEN("Returns correct avaliable size of vector"){
         REQUIRE(input_buffer.available() == 3);
       }
       THEN("Returns ok for erase on 3rd index"){
         REQUIRE(input_buffer.erase(0) == PF::IndexStatus::ok);
       }
       THEN("Rrturns out of bounds if index is out of bounds 5"){
         REQUIRE(input_buffer.erase(5) == PF::IndexStatus::out_of_bounds);
       }
    }

    WHEN("Vector of 4UL is passed as input for dest_start_index 5"){
       auto vector_copy = vector1.copy_from(
       input_buffer.buffer(), input_buffer.size(), 5);
       THEN("Returns out of bound, vector copy fails"){
         REQUIRE(vector_copy == PF::IndexStatus::out_of_bounds);
       }

    }


   WHEN("Vector of 4UL is passed as input, output is expected on 2UL vector"){
    constexpr size_t data_size = 2UL;
    PF::Util::Vector<uint8_t,data_size> vector_copy; 
    auto data_copy = vector_copy.copy_from(
          input_buffer.buffer(), data.size(), 0);
          auto data = input_buffer.buffer();
    THEN("First two bytes are copied to vector_copy correctly, index out of bounds, returns 0"){
      REQUIRE(data_copy == PF::IndexStatus::ok);
      for(int i =0;i<=1;i++){
        REQUIRE(vector_copy.operator[](i)== input_buffer.operator[](i) );
      }
    }
    }


  }

  GIVEN("Vector with completely filled buffer with 4UL capacity"){
    auto data = PF::Util::make_array<uint8_t>(0x0f, 0x36);
    constexpr size_t data_size = 4UL;
    PF::Util::Vector<uint8_t,data_size> vector2; 
    
    WHEN("An array of 2UL size is passed as input"){
      auto copy = vector2.copy_from(
          data.data(), data.size(), 0);
          THEN("Result is as expected"){
            REQUIRE(copy == PF::IndexStatus::ok);
            REQUIRE(vector2[0]== 0x0f);
            REQUIRE(vector2[1] == 0x36);
          }
          THEN("Returns correct size"){
            REQUIRE(vector2.size() == 2);
          }
          THEN("Returns correct avaliable buffers"){
            REQUIRE(vector2.available() == 2);
          }
          THEN("Returns false for full"){
            REQUIRE(vector2.full() == false);
          }
          THEN("Retuns ok for erase at index 2"){
            REQUIRE(vector2.erase(1) == PF::IndexStatus::ok);
          }
          THEN("retuns out of bounds for erase used on out of bound index"){
            REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
          }
    }

    WHEN("An array of 2UL size is passed as input for dest index 6"){
        auto copy = vector2.copy_from(
        data.data(), data.size(), 6);
          THEN("Result is as expected"){
            REQUIRE(copy == PF::IndexStatus::out_of_bounds); 
          }
    }
   
  }

  GIVEN("Vector with completely filled buffer with 4UL capacity"){
      constexpr size_t buffer_size = 2UL;
      constexpr size_t size = 2UL;
      PF::Util::Vector<uint8_t,size> vector1;
      PF::Util::Vector<uint8_t,buffer_size> input_buffer;
      auto data = std::string("\x08\x0a");
      for (auto& ch : data) {
            input_buffer.push_back(ch);
        }
      WHEN("Vector of 2UL is passed as input"){
        auto copy = vector1.copy_from(
         input_buffer.buffer(), buffer_size);
        THEN("Result is as expected ,[0x08,0x0a]"){
           REQUIRE(copy == PF::IndexStatus::ok);
           REQUIRE(vector1.operator[](0) == 0x08);
           REQUIRE(vector1.operator[](1)== 0x0a);
        }
        THEN("Size of vector is as expected"){
          REQUIRE(vector1.size() == 2);
        }
        THEN("Vector is not full"){
          REQUIRE(vector1.full() == true);
        }
        THEN("Retuns avaliable buffer 0"){
          REQUIRE(vector1.available()== 0);
        }
        THEN("Erase returns ok, index in in bounds"){
          REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
        }
        THEN("Erase returns out of bounds , index is out of bounds"){
          REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
        }
      }

  }
} 

SCENARIO("The function in Vector:Element *buffer() works correctly"){
  GIVEN("Vector with partially filled with buffer size 100UL"){
    constexpr size_t buffer_size = 100UL;
      PF::Util::Vector<uint8_t, buffer_size> vector1;
      auto data = std::string("\x08\x0a\x83\x02");
      for (auto& ch : data) {
            vector1.push_back(ch);
        }
    WHEN("Elent *buffer() function is used on vector"){
      vector1.resize(4);
      auto buffer = vector1.buffer();
      THEN("Returned vector is as expected at every index (0x08,0x0a,0x83,0x02)"){
        REQUIRE(buffer[0] == 0x08);
        REQUIRE(buffer[1] == 0x0a);
        REQUIRE(buffer[2] == 0x83);
        REQUIRE(buffer[3] == 0x02);
      }
    }

    WHEN("Vector is resized to 2, Element *buffer() gives correct values"){
      vector1.resize(2);
      auto size = vector1.size();
      auto buffer = vector1.buffer();
      THEN("Return vector size is 2 as expected"){
        REQUIRE(size == 2);
      }
      THEN("Return vector has expected data (0x08,0x0a)"){
        REQUIRE(buffer[0] == 0x08);
        REQUIRE(buffer[1] == 0x0a);
      }
      
    }

  } 



}


