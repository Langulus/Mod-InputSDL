///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/Input.hpp>
#include <Langulus/Flow/Time.hpp>
#include <thread>

LANGULUS_RTTI_BOUNDARY(RTTI::MainBoundary)
using namespace Langulus;


int main(int, char**) {
   Framerate<60> fps;

   // Create root entity                                                
   auto root = Thing::Root<false>("InputSDL");
   root.CreateUnits<A::InputGatherer, A::InputListener>();

   while (true) {
      // Update until quit                                              
      if (not root.Update(16ms))
         break;

      fps.Tick();
   }

   return 0;
}
