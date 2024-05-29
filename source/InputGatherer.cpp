///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
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
   // Create an invisible window so that we can capture and track       
   // the global mouse                                                  
   mInputFocus = SDL_CreateWindow(
      "Input Handle", 1, 1,
      SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS
   );

   LANGULUS_ASSERT(mInputFocus, Construct,
      "SDL failed to create input window. SDL_Error: ",
      SDL_GetError()
   );

   LANGULUS_ASSERT(SDL_SetRelativeMouseMode(true) >= 0, Construct,
      "SDL failed to set relative mouse mode. SDL_Error: ",
      SDL_GetError()
   );

   Couple(descriptor);
   VERBOSE_INPUT("Initialized");
}

/// Shutdown the module                                                       
InputGatherer::~InputGatherer() {
   if (mInputFocus)
      SDL_DestroyWindow(mInputFocus);
}

/// Produce GUI elements in the system                                        
///   @param verb - creation verb to satisfy                                  
void InputGatherer::Create(Verb& verb) {
   mListeners.Create(verb);
}

/// Interact with all listeners                                               
///   @param verb - interaction verb                                          
void InputGatherer::Interact(Verb& verb) {
   // Gather the relevant events                                        
   verb.ForEachDeep([&](const Event& e) {
      PushEvent(e);
      verb.Done();
   });
}

/// System update routine                                                     
///   @param deltaTime - time between updates                                 
///   @param globalEvents - global list of events                             
///   @return false if the system has been terminated by user request         
bool InputGatherer::Update(Time deltaTime, const EventList& globalEvents) {
   // React to the gathered inputs                                      
   for (auto& listener : mListeners) {
      listener.Update(deltaTime, globalEvents);
      listener.Update(deltaTime, mEventQueue);
   }

   // Consume the events                                                
   mEventQueue.Clear();
   return true;
}

/// React on environmental change                                             
void InputGatherer::Refresh() {

}
