#pragma once

#include <memory>    // std::unique_ptr

//#include "Domain/Library/MaintainBooksHandler.hpp"
#include "Domain/Reservation/ReservationHandler.hpp"
#include "TechnicalServices/Logging/LoggerHandler.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

#include "UI/UserInterfaceHandler.hpp"




namespace UI
{
  /*****************************************************************************
  ** Simple UI definition
  **   Simple UI is a console application meant only as a driver to the Domain Layer
  **   application. This UI will someday be replaced by a more sophisticated, user
  **   friendly implementation
  ******************************************************************************/
  class SimpleUI : public UI::UserInterfaceHandler
  {
    public:
      // Constructors
      SimpleUI();


      // Operations
      void launch() override;


      // Destructor
      ~SimpleUI() noexcept override;


    private:
      // These smart pointers hold pointers to lower architectural layer's interfaces
      //std::unique_ptr<Domain::Library::MaintainBooksHandler>                _bookHandler; 
      
      //so we need to make our new layer and place it here
      /*
      TODO:
      add your new domain here
      */
      std::unique_ptr<Domain::Reservation::ReservationHandler> _reserve;

      std::unique_ptr<TechnicalServices::Logging::LoggerHandler>            _loggerPtr;
      TechnicalServices::Persistence::PersistenceHandler                  & _persistentData;


      // convenience reference object enabling standard insertion syntax
      // This line must be physically after the definition of _loggerPtr
      TechnicalServices::Logging::LoggerHandler                            & _logger = *_loggerPtr;
  };
} // namespace UI
