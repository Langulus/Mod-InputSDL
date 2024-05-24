///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "InputListener.hpp"
#include "InputSDL.hpp"


/// Listener construction                                                     
///   @param producer - the producer                                          
///   @param descriptor - instructions for configuring the listener           
InputListener::InputListener(InputGatherer* producer, const Neat& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_INPUT("Initializing...");
   Couple(descriptor);
   VERBOSE_INPUT("Initialized");
}

/// React on environmental change                                             
void InputListener::Refresh() {

}

