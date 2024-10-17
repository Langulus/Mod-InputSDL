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
///   @param producer - the producer                                          
///   @param descriptor - instructions for configuring the listener           
InputListener::InputListener(InputGatherer* producer, const Many& descriptor)
   : Resolvable    {this}
   , ProducedFrom  {producer, descriptor} {
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
///   @param verb - the creation verb                                         
void InputListener::Create(Verb& verb) {
   mAnticipators.Create(this, verb);
}

/// React on events                                                           
///   @param deltaTime - time between Update calls                            
///   @param events - events to react to                                      
void InputListener::Update(const Time& deltaTime, const EventList& events) {
   // Execute all active anticipators' scripts                          
   for (auto& ant : mAnticipators) {
      if (not ant.Interact(events))
         continue;
      
      // The anticipator is active and needs to be updated each         
      // tick. This is essentially a 'hold' event                       
      VERBOSE_INPUT("Hold event triggered: ", ant.mEvent);
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
///   @param producer - the producer of the anticipator                       
///   @param desc - descriptor                                                
Anticipator::Anticipator(InputListener* producer, const Many& desc)
   : ProducedFrom {producer, desc} {
   // What event are we anticipating?                                   
   LANGULUS_ASSERT(
         desc.ExtractData(mEvent)
      or desc.ExtractData(mEvent.mType),
      Construct, "Invalid event for anticipator from: ", desc);

   // Optional state override                                           
   desc.ExtractData(mEvent.mState);

   // How do we react on trigger?                                       
   LANGULUS_ASSERT(desc.ExtractData(mScript),
      Construct, "Missing script for anticipator from: ", desc);

   // Add hierarchy and event payload as contexts, they will get        
   // updated on each interaction/listener environment refresh          
   mFlow.Push(&producer->GetOwners(), &mEvent.mPayload, mScript.Parse());

   VERBOSE_INPUT("Anticipator for ", mEvent.mType, " ", mEvent.mState, " compiled: ");
   #if VERBOSE_INPUT_ENABLED()
      mFlow.Dump();
   #endif
}

/// Interact with the anticipator                                             
///   @param events - the events                                              
///   @return true if the anticipator is a 'hold' event and needs to be       
///      handled in the Update() routine instead                              
bool Anticipator::Interact(const EventList& events) {
   Many unusedSideEffects;
   auto foundEvent = events.FindIt(mEvent.mType);
   if (not foundEvent)
      return false;

   if (mEvent.mState == EventState::Point) {
      // Anticipator doesn't activate - its script will just be         
      // executed once and then reset                                   
      const auto foundState1 = foundEvent.GetValue().FindIt(EventState::Point);
      const auto foundState2 = foundEvent.GetValue().FindIt(EventState::Begin);
      if (foundState1 or foundState2) {
         if (foundState1)
            mEvent = foundState1.GetValue();
         if (foundState2 and mEvent.mTimestamp < foundState2.GetValue().mTimestamp)
            mEvent = foundState2.GetValue();

         VERBOSE_INPUT("Point event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

         mFlow.Reset();
         mFlow.Update({}, unusedSideEffects);
      }
   }
   else if (mEvent.mState == EventState::Begin) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on a Begin event                                 
      const auto foundState = foundEvent.GetValue().FindIt(EventState::Begin);
      if (foundState) {
         mEvent = foundState.GetValue();

         VERBOSE_INPUT("Begin event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

         mFlow.Reset();
         mFlow.Update({}, unusedSideEffects);
      }
   }
   else if (mEvent.mState == EventState::End) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on an End event                                  
      const auto foundState = foundEvent.GetValue().FindIt(EventState::End);
      if (foundState) {
         mEvent = foundState.GetValue();

         VERBOSE_INPUT("End event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

         mFlow.Reset();
         mFlow.Update({}, unusedSideEffects);
      }
   }
   else {
      // Anticipator activates on Begin event, deactivates on an End    
      // event, and shall execute its script on each tick inbetween     
      // This is a 'hold' event and is handled from the Update routine  
      if (not mActive) {
         const auto foundState = foundEvent.GetValue().FindIt(EventState::Begin);
         if (foundState) {
            mEvent = foundState.GetValue();
            mActive = true;
            mFlow.Reset();
         }
      }
      else {
         const auto foundState = foundEvent.GetValue().FindIt(EventState::End);
         if (foundState)
            mActive = false;
      }
   }

   return mActive;
}

/// Stringify the anticipator                                                 
Anticipator::operator Text() const {
   return Text::TemplateRt(
      "{}({}, {})", MetaOf<Anticipator>(), mEvent.mType, mScript
   );
}
