///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "InputGatherer.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   Raw input module using SDL                                              
///                                                                           
/// Manages and produces input gatherers                                      
///                                                                           
struct InputSDL final : A::InputModule {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::InputModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created input gatherers                                   
   TFactory<InputGatherer> mGatherers;

   // Global list of events                                             
   EventList mGlobalEvents;

public:
    InputSDL(Runtime*, const Many&);
   ~InputSDL();

   void Create(Verb&);

   bool Update(Time);
   void PushEvent(const Event&);
   void Teardown();
};