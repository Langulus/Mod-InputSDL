///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>


///                                                                           
///   Input listener                                                          
///                                                                           
/// Reacts on events by executing custom scripts in the context of the owner  
///                                                                           
struct InputListener final : A::Input, ProducedFrom<InputGatherer> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) InputGatherer;
   LANGULUS_BASES(A::Input);

public:
   InputListener(InputGatherer*, const Neat&);

   void Update(Time) {}
   void Refresh();
};

