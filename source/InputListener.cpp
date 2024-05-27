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

/// Controller updater                                                        
void InputListener::Update(Time) {
   // First interact with the accumulated events                        
   // Interaction will activate some anticipators                       
   Act(time.Current(), mInputQueue);
   mInputQueue.Clear();

   // Collect all active anticipators' flows                            
   for (auto& ant : mEvents) {
      if (ant.mActive == 0)
         continue;

      VERBOSE_INPUT("Active anticipator: " << ant.mEvent);
      auto clonedScript { ant.mScript.Clone() };
      Verb::ExecuteScope(mOwners, clonedScript);

      // Consume mouse movement and scrolling                           
      if (  ant.mEvent == Keys::MouseMoveHorizontal()
         || ant.mEvent == Keys::MouseMoveVertical()
         || ant.mEvent == Keys::MouseScrollHorizontal()
         || ant.mEvent == Keys::MouseScrollVertical())
         ant.mActive = 0;
   }
}

/// Inner interaction function                                                
///   @param timeFromInit - time from initialization                          
///   @param events - the events                                              
bool InputListener::Act(const Time& timeFromInit, const EventList& events) {
   // Check all anticipators                                            
   Count reacted = 0;
   for (auto ants : mAnticipators) {
      ants.Interact(timeFromInit, events);
      reacted |= ants.mActive;
   }

   return reacted != 0;
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




/// Interact with the anticipator                                             
///   @param timeFromInit - time from initialization                          
///   @param events - the events                                              
void Anticipator::Interact(const Time& timeFromInit, const EventList& events) {
   // Probe every event in the first argument                           
   for (const auto& input : events) {
      if (mEvent.mType != input.mType)
         continue;

      if (mEvent.mState == EventState::Point) {
         // Anticipator reacts only on point events                     
         if (input.mState & EventState::Begin)
            Trigger(timeFromInit, 1);
         else
            mActive = 0;
      }
      else {
         // Anticipator reacts on explicit begin/end events             
         if (mEvent.mState & EventState::Begin and not mActive)
            Trigger(timeFromInit, 1);
         else
            mActive = 0;
      }
   }
}

/// Trigger the anticipator                                                   
///   @param timeFromInit - time from initialization                          
///   @param times - how many times to trigger                                
void Anticipator::Trigger(const Time& timeFromInit, Count times) {
   mActive += times;
   mLastChange = timeFromInit;
}