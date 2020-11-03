#include "UI/SimpleUI.hpp"

#include <any>         // any_cast()
#include <iomanip>     // setw()
#include <iostream>    // streamsize
#include <limits>      // numeric_limits
#include <memory>      // unique_ptr, make_unique<>()
#include <string>      // string, getline()
#include <vector>

//#include "Domain/Library/Books.hpp"    // Include for now - will replace next increment
#include "Domain/Session/SessionHandler.hpp"
#include "Domain/Reservation/ReservationHandler.hpp"
#include "TechnicalServices/Logging/LoggerHandler.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"




namespace UI
{
  // Default constructor
  SimpleUI::SimpleUI()
  : //_bookHandler   ( std::make_unique<Domain::Library::Books>()                     ),   // will replace these with factory calls in the next increment
    //_reserve (std::make_unique<Domain::Reservation::ReservationHandler>()     ),
    _loggerPtr     ( TechnicalServices::Logging::LoggerHandler::create()            ),
    _persistentData( TechnicalServices::Persistence::PersistenceHandler::instance() )
  {
    _logger << "Simple UI being used and has been successfully initialized";
  }




  // Destructor
  SimpleUI::~SimpleUI() noexcept
  {
    _logger << "Simple UI shutdown successfully";
  }




  // Operations
  void SimpleUI::launch()
  {
    // 1) Fetch Role legal value list
    std::vector<std::string> roleLegalValues = _persistentData.findRoles();


    // 2) Present login screen to user and get username, password, and valid role
    Domain::Session::UserCredentials credentials  = {"", "", {""}};           // ensures roles[0] exists
    auto &                           selectedRole = credentials.roles[0];     // convenience alias

    std::unique_ptr<Domain::Session::SessionHandler> sessionControl;

    do
    {
      std::cin.ignore(  std::numeric_limits<std::streamsize>::max(), '\n' );

      std::cout << "  email: ";
      std::getline( std::cin, credentials.userName );

      std::cout << "  password: ";
      std::getline( std::cin, credentials.passPhrase );

      /* uncomment for original code
      unsigned menuSelection;
      
      do
      {
        for( unsigned i = 0; i != roleLegalValues.size(); ++i )   std::cout << std::setw( 2 ) << i << " - " << roleLegalValues[i] << '\n';
        std::cout << "  role (0-" << roleLegalValues.size()-1 << "): ";
        std::cin  >> menuSelection;
      } while( menuSelection >= roleLegalValues.size() );
      
      selectedRole = roleLegalValues[menuSelection]; 
      */

      selectedRole = roleLegalValues[0]; // Hardcoded so that it logs in patients only

      // 3) Validate user is authorized for this role, and if so create session
      sessionControl = Domain::Session::SessionHandler::userAuthenticates( credentials );
      int visit_number = 0;
      if( sessionControl != nullptr )
      { 
        ++visit_number;
        
        _logger << "Login Successful for \"" + credentials.userName + "\" as role \"" + selectedRole + "\" with visiting number: \"" + std::to_string(visit_number);
        //_logger << "atient Visiting Number: " + visit_number ;
        break;
      }

      std::cout << "** Login failed\n";
      _logger << "Login failure for \"" + credentials.userName + "\" as role \"" + selectedRole + "\"";

    } while( true );


    // 4) Fetch functionality options for this role
    do
    {
      auto        commands = sessionControl->getCommands();
      std::string selectedCommand;
      unsigned    menuSelection;

      do
      {
        for( unsigned i = 0; i != commands.size(); ++i ) std::cout << std::setw( 2 ) << i << " - " << commands[i] << '\n';
        std::cout << std::setw( 2 ) << commands.size() << " - " << "Sign Off\n";

        std::cout << "  action (0-" << commands.size() << "): ";
        std::cin >> menuSelection;
      } while( menuSelection > commands.size() );

      if( menuSelection == commands.size() ) break;

      selectedCommand = commands[menuSelection];
      _logger << "Command selected \"" + selectedCommand + '"';


      /******************************************************************************************************************************
      **  5) The user interface will collect relevant information to execute the chosen command.  This section requires the UI to
      **     know what information to collect, and hence what the available commands are.  Our goal is loose (minimal) coupling, not
      **     no coupling. This can be achieved in a variety of ways, but one common way is to pass strings instead of strong typed
      **     parameters.
      ******************************************************************************************************************************/
      if( selectedCommand == "Create Medical Appointment" )
      {
        std::vector<std::string> parameters( 1 );

        std::cout << " Enter your issue/medical concern:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );

        auto results = sessionControl->executeCommand( selectedCommand, parameters );
        //if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }

      else if( selectedCommand == "Request Doctor")
      {
        std::vector<std::string> parameters( 1 );
          std::cout << " Enter your desired Doctor:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );

        auto results = sessionControl->executeCommand( selectedCommand, parameters );
       // if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }

       else if( selectedCommand == "Generate List of Services")
      {
        std::vector<std::string> parameters( 1 );
          std::cout << " Enter your visting number:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );

        auto results = sessionControl->executeCommand( selectedCommand, parameters );
        //if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }
       else if( selectedCommand == "Book Appointment")
      {
        std::vector<std::string> parameters( 3 );
          std::cout << " Enter requested doctor:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );
          std::cout << " Enter requested date:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[1] );
          std::cout << " Enter requested time:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[2] );
        auto results = sessionControl->executeCommand( selectedCommand, parameters );
        //if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }
      else if( selectedCommand == "Pay for Service")
      {
        std::vector<std::string> parameters( 1 );
          std::string options = "Please select one of the following options (1-4)";
          std::cout << " 1) Create Medical Appointment \n" << "2) Pay Medical Invoice \n" << "3) Generate Medical Report \n" << "4) Generate Prescription History \n";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );
          if(parameters[0] == "1"){
            parameters[0] = "Create Medical Appointment"; 
          }
          else if (parameters[0] == "2"){
            parameters[0] = "Pay Medical Invoice"; 
          }
          else if (parameters[0] == "3"){
            parameters[0] = "Generate Medical Report"; 
          }
          else if (parameters[0] == "4"){
            parameters[0] = "Generate Prescription History"; 
          }

        auto results = sessionControl->executeCommand( selectedCommand, parameters );
        //if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }
       else if( selectedCommand == "Request Payment Option")
      {
        std::vector<std::string> parameters( 2 );
          std::cout << " Enter Credit Card Number:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );
          std::cout << " Enter CVV:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[1] );

        auto results = sessionControl->executeCommand( selectedCommand, parameters );
        //if( results.has_value() ) _logger << "Received reply: \"" + std::any_cast<const std::string &>( results ) + '"';
      }
      //"Request Payment Option"
      else if( selectedCommand == "Another command" ) /* ... */ {}

      else sessionControl->executeCommand( selectedCommand, {} );
    } while( true );

    _logger << "Ending session and terminating";
  }
}
