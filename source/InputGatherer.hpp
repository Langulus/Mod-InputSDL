///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "InputListener.hpp"


///                                                                           
///   FTXUI GUI system and window interface                                   
///                                                                           
///   Manages and produces GUI items that interact with each other within an  
/// isolated system. Also acts as A::Window, since ASCII graphics are         
/// displayed in a console window, and usually there's only one associated    
/// with a process at any given time.                                         
///                                                                           
struct InputGatherer final : A::Input, ProducedFrom<InputSDL> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) InputSDL;
   LANGULUS_BASES(A::Input);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created input listeners                                   
   TFactory<InputListener> mListeners;

public:
    InputGatherer(InputSDL*, const Neat&);
   ~InputGatherer();

   void Create(Verb&);
   bool Update(Time);
   void Refresh();
};