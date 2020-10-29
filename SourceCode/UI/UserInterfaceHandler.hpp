#pragma once

#include <memory>       // unique_ptr
#include <stdexcept>    // runtime_error




namespace UI
{
  // User Interface Layer Interface class
  class UserInterfaceHandler
  {
    public:
      // Exceptions
      struct UIException    : std::runtime_error {using runtime_error::runtime_error;};
      struct   BadUIRequest : UIException        {using UIException  ::UIException;  };


      // User Interface Factory, throws BadUIRequest
      static std::unique_ptr<UserInterfaceHandler> createUI();                                 // must be static


      // Operations, throws UIException
      virtual void launch() = 0;                                                               // must be virtual and pure


      // Interface class destructor
      virtual ~UserInterfaceHandler() noexcept = 0;                                            // must be virtual, best practice to be pure

    protected:
      // Copy assignment operators protected to prevent mixed derived-type assignments
      UserInterfaceHandler & operator=( const UserInterfaceHandler  & ) = default;             // copy assignment
      UserInterfaceHandler & operator=(       UserInterfaceHandler && ) = default;             // move assignment
  };
} // namespace UI
