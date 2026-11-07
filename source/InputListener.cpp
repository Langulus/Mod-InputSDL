///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "InputListener.hpp"
#include "InputSDL.hpp"


/// Listener construction                                                     
///   @param producer the producer                                            
///   @param descriptor instructions for configuring the listener             
InputListener::InputListener(InputGatherer const* producer, Neat const& descriptor)
   : Downcastable {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_INPUT("Initializing...");
   Couple(descriptor);
   VERBOSE_INPUT("Initialized");
}

/// First stage destruction                                                   
void InputListener::Teardown() {
   mAnticipators.Teardown();
}

/// React on environmental change                                             
void InputListener::Refresh() {

}

/// Create/remove anticipators to/from the listener                           
///   @param verb the creation verb                                           
void InputListener::Create(Verb& verb) {
   mAnticipators.Create(this, verb);
}

/// React on events                                                           
///   @param deltaTime time between Update calls                              
///   @param events events to react to                                        
void InputListener::Update(Time const& deltaTime, EventQueue const& events) {
   // Execute all active anticipators' scripts                          
   for (auto& ant : mAnticipators) {
      if (not ant.Interact(events))
         continue;
      
      // The anticipator is active and needs to be updated each         
      // tick. This is essentially a 'hold' event                       
      VERBOSE_INPUT("Hold event triggered: ", ant.mEventType);
      Many unusedSideEffects;
      ant.mFlow.Update(deltaTime, unusedSideEffects);
   }
}

/// Automatically create anticipators by analyzing owner's abilities,         
/// searching for events associated with these abilities, and binding them as 
/// anticipators                                                              
void InputListener::AutoBind() {
   TODO();
}

/// Anticipator constructor                                                   
///   @param producer the producer of the anticipator                         
///   @param desc descriptor                                                  
Anticipator::Anticipator(InputListener const* producer, Neat const& desc)
   : ProducedFrom {producer, desc} {
   // What event are we anticipating?                                   
   Event event;
   if (desc.ExtractData(event))
      Update(event);
   else {
      LglsAssert(desc.ExtractData(mEventType),
         "Invalid event for anticipator from: ", desc);

      // Optional state override                                        
      desc.ExtractData(mEventState);
   }

   // How do we react on trigger?                                       
   LglsAssert(desc.ExtractData(mScript), 
      "Missing script for anticipator from: ", desc);

   // Add hierarchy and event payload as contexts, they will get        
   // updated on each interaction/listener environment refresh          
   mFlow.Push(&producer->GetOwners(), &mEventPayload, mScript.Parse());

   VERBOSE_INPUT("Anticipator for ", mEventType, " (", mEventState, ") compiled: ");
   #if VERBOSE_INPUT_ENABLED()
      mFlow.Dump();
   #endif
}

/// Interact with the anticipator                                             
///   @param events the events                                                
///   @return true if the anticipator is a 'hold' event and needs to be       
///      handled in the Update() routine instead                              
bool Anticipator::Interact(EventQueue const& events) {
   Many unusedSideEffects;
   const auto signature = EventSignature {mEventType, mEventState};
   const auto foundEvent = events.Find(signature);
   if (not foundEvent)
      return false;

   if (mEventState == EventState::Point) {
      // Anticipator doesn't activate - its script will just be         
      // executed once and then reset                                   
      Update(*foundEvent);

      VERBOSE_INPUT("Point event triggered: ", mEventType);
      #if VERBOSE_INPUT_ENABLED()
         mFlow.Dump();
      #endif

      mFlow.Reset();
      mFlow.Update({}, unusedSideEffects);
   }
   else if (mEventState == EventState::Begin) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on a Begin event                                 
      Update(*foundEvent);

      VERBOSE_INPUT("Begin event triggered: ", mEventType);
      #if VERBOSE_INPUT_ENABLED()
         mFlow.Dump();
      #endif

      mFlow.Reset();
      mFlow.Update({}, unusedSideEffects);
   }
   else if (mEventState == EventState::End) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on an End event                                  
      Update(*foundEvent);

      VERBOSE_INPUT("End event triggered: ", mEventType);
      #if VERBOSE_INPUT_ENABLED()
         mFlow.Dump();
      #endif

      mFlow.Reset();
      mFlow.Update({}, unusedSideEffects);
   }
   else {
      // Anticipator activates on Begin event, deactivates on an End    
      // event, and shall execute its script on each tick inbetween     
      // This is a 'hold' event and is handled from the Update routine  
      if (not mActive) {
         Update(*foundEvent);
         mActive = true;
         mFlow.Reset();
      }
      else mActive = false;
   }

   return mActive;
}

/// Stringify the anticipator                                                 
Anticipator::operator Text() const {
   return Text::Format(
      "{}(on {} {}: {})", MetaDataOf<Anticipator>(), mEventState, mEventType, mScript
   );
}
