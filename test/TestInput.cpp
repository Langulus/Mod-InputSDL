///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/Input.hpp>
#include <Langulus/Testing.hpp>
#include <Langulus/Allocator.hpp>

using namespace Langulus;
using Things::Thing;


SCENARIO("Input handler creation") {
   static MemoryState memoryState;
   
   GIVEN("Init and shutdown cycles") {
      for (int repeat = 0; repeat != 10; ++repeat) {
         // Create root entity                                          
         auto root = Thing::Root<false>("InputSDL");

         WHEN("The input gatherer is created via abstractions") {
            auto gatherer = root.CreatePart<Things::InputGatherer>();
            auto listener = root.CreatePart<Things::InputListener>();

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gatherer.GetCount() == 1);
            REQUIRE(gatherer.CastsTo<Things::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<Things::InputListener>(1));
            REQUIRE(listener.IsSparse());

            REQUIRE(root.GetParts().GetCount() == 2);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The input gatherer is created via tokens") {
            auto gatherer = root.CreatePartToken("InputGatherer");
            auto listener = root.CreatePartToken("InputListener");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(gatherer.GetCount() == 1);
            REQUIRE(gatherer.CastsTo<Things::InputGatherer>(1));
            REQUIRE(gatherer.IsSparse());

            REQUIRE(listener.GetCount() == 1);
            REQUIRE(listener.CastsTo<Things::InputListener>(1));
            REQUIRE(listener.IsSparse());

            REQUIRE(root.GetParts().GetCount() == 2);
         }
      #endif

         REQUIRE(memoryState.Assert());
         REQUIRE_FALSE(Allocator::CollectGarbage());
      }
   }
}

