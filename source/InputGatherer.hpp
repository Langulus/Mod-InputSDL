///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "InputListener.hpp"
#include <Langulus/Factory.hpp>
#include <Langulus/Producible.hpp>
#include <Langulus/Verbs/Create.hpp>
#include <Langulus/Verbs/Emit.hpp>


///                                                                           
///   Input gatherer                                                          
///                                                                           
/// Collects all input events that are provided either by SDL, or by any      
/// Verbs::Interact that happens to occur in this context. After gathering    
/// all events, they will be compiled and sent to all listeners on each tick. 
///                                                                           
struct InputGatherer final : Things::InputGatherer, ProducedFrom<InputSDL> {
   using CTTI_Abstract = No;
   using CTTI_Producer = InputSDL;
   using CTTI_Bases    = Things::InputGatherer;
   using CTTI_Ability  = Types<Verbs::Create, Verbs::Emit>;

private:
   // List of created input listeners                                   
   TFactory<InputListener> mListeners;

   // Mouse and keyboard inputs always require a window in order to     
   // work relatively. This window will be a small borderless one.      
   SDL_Window* mInputFocus {};

public:
    InputGatherer(InputSDL*, Many const&);
   ~InputGatherer();

   void Create(Verb&);
   void Interact(Verb&);

   bool Update(Time, const EventList&);
   void Refresh();
   void Teardown();
};