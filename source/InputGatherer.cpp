///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "InputGatherer.hpp"
#include "InputSDL.hpp"


/// Gatherer construction                                                     
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
InputGatherer::InputGatherer(InputSDL* producer, const Neat& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor}
   , mListeners   {this} {
   VERBOSE_INPUT("Initializing...");
   Couple(descriptor);
   VERBOSE_INPUT("Initialized");
}

/// Shutdown the module                                                       
InputGatherer::~InputGatherer() {

}

/// Produce GUI elements in the system                                        
///   @param verb - creation verb to satisfy                                  
void InputGatherer::Create(Verb& verb) {
   mListeners.Create(verb);
}

/// System update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the system has been terminated by user request         
bool InputGatherer::Update(Time deltaTime) {
   //TODO gather all inputs
   // Update all listeners elements                                     
   for (auto& listener : mListeners)
      listener.Update(deltaTime);
   return true;
}

/// React on environmental change                                             
void InputGatherer::Refresh() {

}