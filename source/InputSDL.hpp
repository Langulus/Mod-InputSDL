///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "InputGatherer.hpp"
#include <Langulus/Verbs/Create.hpp>


///                                                                           
///   Raw input module using SDL                                              
///                                                                           
/// Manages and produces input gatherers                                      
struct InputSDL final : Things::InputModule {
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Things::InputModule;
   using CTTI_Ability   = Verbs::Create;

private:
   // List of created input gatherers                                   
   TFactory<InputGatherer> mGatherers;

   // Global list of events                                             
   EventQueue mGlobalEvents;

public:
    InputSDL(Runtime*, Neat const&);
   ~InputSDL();

   void Create(Verb&);

   bool Update(Time);
   void PushEvent(Event const&);
   void Teardown();
};