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
InputListener::InputListener(InputGatherer* producer, const Neat& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_INPUT("Initializing...");
   Couple(descriptor);
   VERBOSE_INPUT("Initialized");
}

/// React on environmental change                                             
void InputListener::Refresh() {

}

/// Check if controller already has an anticipator of some kind               
/// If anticipator doesn't exist - create a new one                           
///   @param event - the event that triggers the anticipator                  
///   @return the anticipator corresponding to the event                      
void InputListener::AddAnticipator(const Anticipator& ant) {
   const auto foundEvent = mAnticipators.FindIt(ant.mEvent.mType);
   if (foundEvent) {
      const auto foundState = foundEvent.mValue->FindIt(ant.mEvent.mState);
      if (foundState) {
         // Anticipator already exists, merge scripts                   
         foundState.mValue->mFlow.Merge(ant.mFlow);
         return;
      }
      foundEvent.mValue->Insert(ant.mEvent.mState, ant);
   }
   else {
      mAnticipators.Insert(ant.mEvent.mType);
      auto& newGroup = mAnticipators[ant.mEvent.mType];
      newGroup.Insert(ant.mEvent.mState, ant);
   }
}

/// Create/remove anticipators to the listener                                
///   @param verb - the creation verb                                         
void InputListener::Create(Verb& verb) {
   // Add anticipators                                                  
   verb.ForEachDeep([&](const Anticipator& e) {
      AddAnticipator(e);
      verb.Done();
   });
}

/// React on events                                                           
///   @param deltaTime - time between Update calls                            
///   @param events - events to react to                                      
void InputListener::Update(const Time& deltaTime, const EventList& events) {
   // Execute all active anticipators' scripts                          
   for (auto ant : mAnticipators) {
      for (auto ant_state : ant.mValue) {
         if (ant_state.mValue.Interact(events)) {
            // The anticipator is active and needs to be updated each   
            // tick                                                     
            ant_state.mValue.mFlow.Update(deltaTime);
         }
      }
   }
}

/// Automatically create anticipators by analyzing owner's abilities,         
/// searching for events associated with these abilities, and binding them as 
/// anticipators                                                              
void InputListener::AutoBind() {
   TODO();
}

/// Descriptor constructor                                                    
///   @param desc - descriptor                                                
Anticipator::Anticipator(Describe&& desc) {
   // What event are we anticipating?                                   
   LANGULUS_ASSERT(
         desc->ExtractData(mEvent)
      or desc->ExtractData(mEvent.mType),
      Construct, "Invalid event for anticipator from: ", *desc);

   // Optional state override                                           
   desc->ExtractData(mEvent.mState);

   // How do we react on trigger?                                       
   Code code;
   LANGULUS_ASSERT(desc->ExtractData(code),
      Construct, "Missing script for anticipator from: ", *desc);
}

/// Interact with the anticipator                                             
///   @param dt - delta time between frames                                   
///   @param events - the events                                              
bool Anticipator::Interact(const EventList& events) {
   auto foundEvent = events.FindIt(mEvent.mType);
   if (not foundEvent)
      return false;

   if (mEvent.mState == EventState::Point) {
      // Anticipator doesn't activate - its script will just be         
      // executed once and then reset                                   
      const auto foundState1 = foundEvent.mValue->FindIt(EventState::Point);
      const auto foundState2 = foundEvent.mValue->FindIt(EventState::Begin);
      if (foundState1 or foundState2) {
         if (foundState1)
            mEvent = *foundState1.mValue;
         if (foundState2 and mEvent.mTimestamp < foundState2.mValue->mTimestamp)
            mEvent = *foundState2.mValue;

         mFlow.Reset();
         mFlow.Update();
      }
   }
   else if (mEvent.mState == EventState::Begin) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on a Begin event                                 
      const auto foundState = foundEvent.mValue->FindIt(EventState::Begin);
      if (foundState) {
         mEvent = *foundState.mValue;
         mFlow.Reset();
         mFlow.Update();
      }
   }
   else if (mEvent.mState == EventState::End) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on an End event                                  
      const auto foundState = foundEvent.mValue->FindIt(EventState::End);
      if (foundState) {
         mEvent = *foundState.mValue;
         mFlow.Reset();
         mFlow.Update();
      }
   }
   else {
      // Anticipator activates on Begin event, deactivates on an End    
      // event, and shall execute its script on each tick inbetween     
      if (not mActive) {
         const auto foundState = foundEvent.mValue->FindIt(EventState::Begin);
         if (foundState) {
            mEvent = *foundState.mValue;
            mActive = true;
            mFlow.Reset();
         }
      }
      else {
         const auto foundState = foundEvent.mValue->FindIt(EventState::End);
         if (foundState)
            mActive = false;
      }
   }

   return mActive;
}

Anticipator::operator Text() const {
   return Text::TemplateRt("{}({}, {})", MetaOf<Anticipator>(), mEvent, mFlow);
}
