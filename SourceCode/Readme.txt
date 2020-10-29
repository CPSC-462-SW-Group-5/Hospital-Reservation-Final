I have some concerns about the copy and assignment hierarchy.  What semantics are appropriate for these interfaces and implementations?

1) I'm using unique_ptr to manage pointers to interface realizations, but copying and assigning unique_ptr will do a shallow copy and transfer ownership - not really what I want.  Maybe shared_ptr is a better approach?

2) What does it mean to assign one object to another?  Yes, the base class should handle assigning base class attributes, and the derived class's implementation of the assignment operator should call the base class's implementation (it doesn't currently), but is there more than that?  Base classes are abstract, so you can't really assign base class objects to each other (can't create base class objects).  Assigning a derived class to another is straight forward too.  But how should I assign a reference to the base class realized with a derived class to another?  Currently, the base class assignment operator is hidden, meaning there is no operator=() function that takes references to the base, but should there be?  And if there is, should I perform a dynamic_cast to determine if indeed it is the derived object's type?  Or can I assume that because it would have been call if it wasn't?

Anyway, I had to stop before solving these concerns - which really are errors in the current solution that need to be fixed.

Update 1/6/2020
Created SingletonDB as a quick and dirty extension to SimpleDB.  Should look for a more elegant and "correct" way of creating singleton instances of logger and DB objects. (Applies to Domain objects?)

Updated 4/10/2020
Implemented the Checkout Book scenario with SSD
Implemented the Factory pattern and removed visibility of internal layer implementations
Made the Interface to Persistent Data support the singleton pattern
Added Adaptation data and file
Combined the Different Sessions into a policy based class template and created policies for each role
Created a non-interactive driver as a UI replacement.  Select which UI component via adaptation data

3) In version 3, class Books is used via Books.hpp directly in SimpleUI.  MaintainBooksHandler.hpp has not yet implemented a factory function to shield knowledge of Books from the UI.  Need to add the factory pattern to Books, and then use that everywhere.

4) Version 3 added factories, but in doing so the Domain Layer has exposed services by Role instead of by User Goal Use Case.  This smells like the Facade Controller Pattern but we're advocating User Goal based Controller Pattern.  While the current (v3) implementation has loosely coupled "services" (both command and the commands parameters are passed by strings) that I like. But I'm thinking the createSession function should return a collection of User Goal Use Case objects (behind appropriate interfaces) instead of a single Session object.  Each User Goal Use Case object would derive from Session so the collection could hold smart pointers-to-Session.  The UI would have to down cast from Session to specific User Goal Use Case (ugly) to get access to the specific strong typed functions for that use case.  Anyway, need to change the Facade Controller to a User Goal Session Controller pattern.
