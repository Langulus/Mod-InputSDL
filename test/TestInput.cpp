///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Main.hpp"
#include <Langulus/Input.hpp>
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Input handler creation", "[input]") {
   static Allocator::State memoryState;
   
   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("InputSDL");

         WHEN("The input gatherer is created via abstractions") {
            auto gatherer = root.CreateUnit<A::InputGatherer>();
            auto listener = root.CreateUnit<A::InputListener>();

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gatherer.GetCount() == 1);
            REQUIRE(gatherer.CastsTo<A::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<A::InputListener>(1));
            REQUIRE(listener.IsSparse());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The input gatherer is created via tokens") {
            auto gatherer = root.CreateUnitToken("InputGatherer");
            auto listener = root.CreateUnitToken("InputListener");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gatherer.GetCount() == 1);
            REQUIRE(gatherer.CastsTo<A::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<A::InputListener>(1));
            REQUIRE(listener.IsSparse());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

