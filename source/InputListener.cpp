///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
      if (foundState)
         return;
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
   for (auto& ant : mAnticipators) {
      for (auto& ant_state : ant.mValue) {
         if (ant_state.mValue.Interact(events))
            ant_state.mValue.mFlow.Update(deltaTime);
      }
   }
}

/// Automatically create anticipators by analyzing owner's abilities,         
/// searching for events associated with these abilities, and binding them as 
/// anticipators                                                              
void InputListener::AutoBind() {
   // Collect link's abilities                                          
   auto selector = Verb::From<Verbs::Select>({}, DataID::Of<VerbID>);
   DoInHierarchy(selector, SeekStyle::Here);
   if (selector.GetOutput().IsEmpty())
      return;

   // Find the events associated with the abilities                     
   // Create anticipators for all abilities                             
   selector.GetOutput().ForEach([this](const VerbID& v) {
      // Use the whole system for interpretation                        
      auto interpreter = Verb::From<Verbs::Interpret>({}, v);
      DoInHierarchy(interpreter);

      // Then rely on the associate verb here                           
      if (!interpreter.GetOutput().IsEmpty()) {
         auto associator = Verb::From<Verbs::Interpret>({}, interpreter.GetOutput());
         Associate(associator);
      }
   });
}


/// Descriptor constructor                                                    
///   @param desc - descriptor                                                
Anticipator::Anticipator(Describe&& desc) {
   TODO();
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
      // executed once and then reset.                                  
      const auto foundState1 = foundEvent.mValue->FindIt(EventState::Point);
      const auto foundState2 = foundEvent.mValue->FindIt(EventState::Begin);
      if (foundState1 or foundState2) {
         if (foundState1)
            mEvent = *foundState1.mValue;
         if (foundState2 and mStartTime < foundState2.mValue->mTimestamp)
            mStartTime = mNow = foundState2.mValue->mTimestamp;

         mFlow.Reset();
         mFlow.Update();
      }
   }
   else if (mEvent.mState == EventState::Begin) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on a Begin event.                                
      const auto foundState = foundEvent.mValue->FindIt(EventState::Begin);
      if (foundState) {
         mTimestamp = foundState.mValue->mTimestamp;
         mFlow.Reset();
         mFlow.Update();
      }
   }
   else if (mEvent.mState == EventState::End) {
      // Anticipator doesn't activate - its script will just be         
      // executed once on an End event.                                 
      const auto foundState = foundEvent.mValue->FindIt(EventState::End);
      if (foundState) {
         mTimestamp = foundState.mValue->mTimestamp;
         mFlow.Reset();
         mFlow.Update();
      }
   }
   else {
      // Anticipator activates on Begin event, deactivates on an End    
      // event, and shall execute its script on each tick inbetween.    
      if (not mActive) {
         const auto foundState = foundEvent.mValue->FindIt(EventState::Begin);
         if (foundState) {
            mActive = true;
            mTimestamp = foundState.mValue->mTimestamp;
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