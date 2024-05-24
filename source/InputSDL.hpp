///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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

   // Mouse and keyboard inputs always require a window in order to     
   // work relatively. This window will be a small borderless one.      
   SDL_Window* mInputFocus {};

public:
    InputSDL(Runtime*, const Neat&);
   ~InputSDL();

   bool Update(Time);
   void Create(Verb&);
};

