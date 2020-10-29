#include "UI/SystemDriverUI.hpp"

#include "Domain/Session/SessionHandler.hpp"

#include "TechnicalServices/Logging/LoggerHandler.hpp"




namespace UI
{
  // Default constructor
  SystemDriverUI::SystemDriverUI()
  : _loggerPtr( TechnicalServices::Logging::LoggerHandler::create() )
  {
    _logger << "Contracted UI being used and has been successfully initialized";
  }




  // Destructor
  SystemDriverUI::~SystemDriverUI() noexcept
  {
    _logger << "Contracted UI shutdown successfully";
  }




  // Operations
  void SystemDriverUI::launch()
  {
    {
      _logger << "Starting scenario 1: Booking a Reservation Successfully";

      auto session  = Domain::Session::SessionHandler::userAuthenticates( {"Tom", "CPSC 462 Rocks!", {"Borrower"}} );
      auto commands = session->getCommands();
      auto results  = session->executeCommand( "Checkout Book", {"Applied UML and Patterns", "Larman", "0-13-148906-2"} );
      //session->signOff();

      _logger << "Completed scenario 1";
    }
  }
}    // namespace UI
