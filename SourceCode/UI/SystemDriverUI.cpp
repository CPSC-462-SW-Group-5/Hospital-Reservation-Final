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

      auto session  = Domain::Session::SessionHandler::userAuthenticates( {"mytest@email.com", "team5", {"Patient"}} );
      auto commands = session->getCommands();
      auto results  = session->executeCommand( "Create Medical Appointment", {"Insomnia"} );
      results = session->executeCommand("Request Doctor", {"Dr. Jones"});
      results = session->executeCommand("Book Appointment", { "Dr. Jones", "10/01/20", "10:00 AM" });
      results = session->executeCommand("Sign Out", {"Sign Me Out"});
      

      _logger << "====Completed scenario 1====";
     
      

    }
  }
}    // namespace UI
