///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "InputListener.hpp"
#include <Flow/Verbs/Create.hpp>
#include <Flow/Verbs/Interact.hpp>


///                                                                           
///   Input gatherer                                                          
///                                                                           
/// Collects all input events that are provided either by SDL, or by any      
/// Verbs::Interact that happens to occur in this context. After gathering    
/// all events, they will be compiled and sent to all listeners on each tick. 
///                                                                           
struct InputGatherer final : A::InputGatherer, ProducedFrom<InputSDL> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) InputSDL;
   LANGULUS_BASES(A::InputGatherer);
   LANGULUS_VERBS(Verbs::Create, Verbs::Interact);

private:
   // List of created input listeners                                   
   TFactory<InputListener> mListeners;

   // Mouse and keyboard inputs always require a window in order to     
   // work relatively. This window will be a small borderless one.      
   SDL_Window* mInputFocus {};

public:
    InputGatherer(InputSDL*, const Neat&);
   ~InputGatherer();

   void Create(Verb&);
   void Interact(Verb&);

   bool Update(Time, const EventList&);
   void Refresh();
};