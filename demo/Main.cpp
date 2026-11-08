///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/Input.hpp>
#include <Langulus/Time.hpp>
#include <thread>

using namespace Langulus;


int main(int, char**) {
   Framerate<60> fps;

   // Create root entity                                                
   auto root = Thing::Root<false>("InputSDL");
   root.CreateParts<Things::InputGatherer, Things::InputListener>();

   while (true) {
      // Update until quit                                              
      if (not root.Update(16ms))
         break;

      fps.Tick();
   }

   return 0;
}
