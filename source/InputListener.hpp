///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Factory.hpp>
#include <Langulus/Producible.hpp>
#include <Langulus/Time.hpp>
#include <Langulus/Verbs/Create.hpp>

struct Anticipator;


///                                                                           
///   Input listener                                                          
///                                                                           
/// Reacts on events by executing custom scripts in the context of the owner  
///                                                                           
struct InputListener final : Things::InputListener, ProducedFrom<InputGatherer> {
   using CTTI_Abstract = No;
   using CTTI_Producer = InputGatherer;
   using CTTI_Bases    = Things::InputListener;
   using CTTI_Ability  = Verbs::Create;

private:
   // Control factor (zero means no control, 1 means full control)      
   // Acts as mass modifier for executed scripts                        
   Real mControlFactor = 1;
   // Anticipators that react on events                                 
   TFactoryUnique<Anticipator> mAnticipators;

   void AutoBind();

public:
   InputListener(InputGatherer*, Many const&);

   void Create(Verb&);
   void Update(const Time&, const EventList&);
   void Refresh();
   void Teardown();
};


///                                                                           
///   Anticipator                                                             
///                                                                           
/// An input pair used to map an event pattern to a script, track time since  
/// last interaction, count interactions, track state, etc. This anticipator  
/// should anticipate more complex patterns in the future, like gestures.     
///                                                                           
struct Anticipator : Referenced, ProducedFrom<InputListener> {
   LANGULUS_CONVERTS_TO(Text);
   LANGULUS_PRODUCER() InputListener;

   // Event and state on which anticipator reacts                       
   // Contained payload acts as a context for the precompiled flow      
   Event mEvent;
   // Marks the anticipator as active in case of Begin/End events       
   bool mActive = false;
   // Script                                                            
   Code mScript;
   // Precompiled mScript to execute as event reaction                  
   Temporal mFlow;

public:
   Anticipator(InputListener*, Many const&);

   bool Interact(const EventList&);

   explicit operator Text() const;

protected:
   Text Self() const { return operator Text() + ": "; }
};

