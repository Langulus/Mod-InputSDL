///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/Input.hpp>
#include <Langulus/Testing.hpp>


SCENARIO("Input handler creation", "[input]") {
   static Allocator::State memoryState;
   
   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("InputSDL");

         WHEN("The input gatherer is created via abstractions") {
            auto gatherer = root.CreateUnit<Things::InputGatherer>();
            auto listener = root.CreateUnit<Things::InputListener>();

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gatherer.GetCount() == 1);
            REQUIRE(gatherer.CastsTo<Things::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<Things::InputListener>(1));
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
            REQUIRE(gatherer.CastsTo<Things::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<Things::InputListener>(1));
            REQUIRE(listener.IsSparse());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

