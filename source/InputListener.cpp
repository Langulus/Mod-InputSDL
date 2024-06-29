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
///   @param ant - the anticipator                                            
///   @return the new anticipator                                             
Anticipator& InputListener::AddAnticipator(const Anticipator& ant) {
   const auto foundEvent = mAnticipators.FindIt(ant.mEvent.mType);
   if (foundEvent) {
      const auto foundState = foundEvent.mValue->FindIt(ant.mEvent.mState);
      if (foundState) {
         // Anticipator already exists, merge scripts                   
         TODO();// foundState.mValue->mFlow.Merge(ant.mFlow);
         return *foundState.mValue;
      }
      foundEvent.mValue->Insert(ant.mEvent.mState, ant);
      return (*foundEvent.mValue)[ant.mEvent.mState];
   }
   else {
      mAnticipators.Insert(ant.mEvent.mType);
      auto& newGroup = mAnticipators[ant.mEvent.mType];
      newGroup.Insert(ant.mEvent.mState, ant);
      return newGroup[ant.mEvent.mState];
   }
}

/// Remove an anticipator                                                     
///   @param ant - the anticipator to remove                                  
void InputListener::RemoveAnticipator(const Anticipator& ant) {
   const auto foundEvent = mAnticipators.FindIt(ant.mEvent.mType);
   if (foundEvent) {
      const auto foundState = foundEvent.mValue->FindIt(ant.mEvent.mState);
      if (foundState) {
         foundEvent.mValue->RemoveIt(foundState);
         if (foundEvent.mValue->IsEmpty())
            mAnticipators.RemoveIt(foundEvent);
      }
   }
}

/// Create/remove anticipators to the listener                                
///   @param verb - the creation verb                                         
void InputListener::Create(Verb& verb) {
   verb.ForEachDeep([&](const Anticipator& e) {
      // Add anticipators                                               
      auto& ant = AddAnticipator(e);

      // We have to compile separately after anticipator has been       
      // emplaced, because its flow reuses local addresses              
      //TODO just recompile on move? but that would mean double compilation time... then just compile on demand?
      try { ant.Compile(GetOwners()); }
      catch (...) {
         // Make sure anticipator is removed on compilation failure     
         RemoveAnticipator(ant);
         throw;
      }

      // Success, if reached                                            
      VERBOSE_INPUT("Anticipator added: ", ant);
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
            // tick. This is essentially a 'hold' event                 
            VERBOSE_INPUT("Hold event triggered: ", ant_state.mValue.mEvent);
            #if VERBOSE_INPUT_ENABLED()
               ant_state.mValue.mFlow.Dump();
            #endif

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
Anticipator::Anticipator(const Anticipator& moved)
   : mEvent  {moved.mEvent}
   , mActive {moved.mActive}
   , mScript {moved.mScript} {}

/// Descriptor constructor                                                    
///   @param desc - descriptor                                                
Anticipator::Anticipator(Anticipator&& moved)
   : mEvent  {Move(moved.mEvent)}
   , mActive {Move(moved.mActive)}
   , mScript {Move(moved.mScript)} {}

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
   LANGULUS_ASSERT(desc->ExtractData(mScript),
      Construct, "Missing script for anticipator from: ", *desc);
}

/// Precompile the script                                                     
void Anticipator::Compile(const Entity::Hierarchy& owners) {
   // Add hierarchy and event payload as a contexts, they will get      
   // updated on each interaction/listener environment refresh          
   mFlow.Push(&owners, &mEvent.mPayload, mScript.Parse());
}

/// Interact with the anticipator                                             
///   @param events - the events                                              
///   @return true if the anticipator is a 'hold' event and needs to be       
///      handled in the Update() routine instead                              
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

         VERBOSE_INPUT("Point event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

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

         VERBOSE_INPUT("Begin event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

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

         VERBOSE_INPUT("End event triggered: ", mEvent);
         #if VERBOSE_INPUT_ENABLED()
            mFlow.Dump();
         #endif

         mFlow.Reset();
         mFlow.Update();
      }
   }
   else {
      // Anticipator activates on Begin event, deactivates on an End    
      // event, and shall execute its script on each tick inbetween     
      // This is a 'hold' event and is handled from the Update routine  
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
