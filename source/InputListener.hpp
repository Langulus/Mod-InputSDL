///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>
#include <Flow/Time.hpp>
#include <Flow/Verbs/Create.hpp>

struct Anticipator;


///                                                                           
///   Input listener                                                          
///                                                                           
/// Reacts on events by executing custom scripts in the context of the owner  
///                                                                           
struct InputListener final : A::InputListener, ProducedFrom<InputGatherer> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) InputGatherer;
   LANGULUS_BASES(A::InputListener);
   LANGULUS_VERBS(Verbs::Create);

private:
   // Control factor (zero means no control, 1 means full control)      
   // Acts as mass modifier for executed scripts                        
   Real mControlFactor = 1;
   // Anticipators that react on events                                 
   TUnorderedMap<DMeta, TUnorderedMap<EventState, Anticipator>> mAnticipators;

   Anticipator& AddAnticipator(const Anticipator&);
   void RemoveAnticipator(const Anticipator&);
   void AutoBind();

public:
   InputListener(InputGatherer*, const Neat&);

   void Create(Verb&);
   void Update(const Time&, const EventList&);
   void Refresh();
};


///                                                                           
///   Anticipator                                                             
///                                                                           
/// An input pair used to map an event pattern to a script, track time since  
/// last interaction, count interactions, track state, etc. This anticipator  
/// should anticipate more complex patterns in the future, like gestures.     
///                                                                           
struct Anticipator {
   LANGULUS_CONVERTS_TO(Text);

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
   Anticipator(const Anticipator&);
   Anticipator(Anticipator&&);
   Anticipator(Describe&&);

   bool Interact(const EventList&);
   void Compile(const Entity::Hierarchy&);

   explicit operator Text() const;

protected:
   Text Self() const { return operator Text() + ": "; }
};