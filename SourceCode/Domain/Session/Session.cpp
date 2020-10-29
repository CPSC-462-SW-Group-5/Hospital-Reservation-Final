#include "Domain/Session/Session.hpp"

#include <string>
#include <any>
#include <vector>

namespace  // anonymous (private) working area
{
  // 1)  First define all system events (commands, actions, requests, etc.)
  #define STUB(functionName)  std::any functionName( Domain::Session::SessionBase & /*session*/, const std::vector<std::string> & /*args*/ ) \
                              { return {}; }  // Stubbed for now

  STUB( help         )
  STUB( shutdown     )


// this function takes in a session and a number of arguments
// we should change this with our own function or like createMedicalAppt()
  std::any createMedicalAppt( Domain::Session::SessionBase & session, const std::vector<std::string> & args )
  {
    // TO-DO  Verify there is such a book and the mark the book as being checked out by user
    std::string results = args[0];
    std::string output = "Here is the list of available doctors\n";
    session._logger << "Patient reported to have issue:  " + results;
    std::vector<std::string> doctor_list = {"Dr. Jones", "Dr. Rich", "Dr. Stone", "Dr. Mike", "Dr. Nguyen"};
    int count = 1;
    for(auto it = doctor_list.begin(); it != doctor_list.end(); ++it){
      session._logger << "Doctor " + std::to_string(count) + ": " + *it + " ";
      ++count;
    
    }

    return {output};
  }

  std::any reqDoctor( Domain::Session::SessionBase & session, const std::vector<std::string> & args) 
  {
    session._logger << "Patient requested " + args[0];
    if(args[0] == "Dr. Jones") {
      
      std::string results = "Available times: \n 10:00 AM \n 11:00 AM \n 2:00 PM \n";
      return {results};
    }
    else if(args[0] == "Dr. Rich") {
      std::string results = "Available times: \n 1:00 PM \n 3:00 PM \n 5:00 PM \n";
      return {results};
    }
    else if(args[0] == "Dr. Stone") {
      std::string results = "Available times: \n 2:30 PM \n 4:00 PM \n 5:00 PM \n";
      return {results};
    
    }
    else if(args[0] == "Dr. Mike") {
      std::string results = "Available times: \n 5:00 PM \n 6:00 PM \n 7:00 PM \n";
      return {results};
 
    }
    else if(args[0] == "Dr. Nguyen") {
      std::string results = "Available times: \n 8:00 PM \n 9:00 PM \n 10:00 PM \n";
      return {results};
    }
  }

  std::any bookAppt( Domain::Session::SessionBase & session, const std::vector<std::string> & args) 
  {
    std::string output = "Here is your reservation: Your appointment with " + args[0] + " will be on " + args[1] + " at " + args[2] + "\n A receipt copy has been sent to the patient's email.";
    return {output};
  }

  std::any generateServicesList( Domain::Session::SessionBase & session, const std::vector<std::string> & args )
{
  std::string results = args[0];
  std::string output = "Here is the list of the services offered by the system \n";

  std::vector<std::string> services_list = {"Create Medical Appointment", "Pay Medical Invoice", "Generate Medical Report", "Generate Prescription History"};
  int count = 1;
  session._logger << "List of Services \n";
  for(auto it = services_list.begin(); it != services_list.end(); ++it) {
    session._logger << std::to_string(count) + ". " + *it + " ";
    ++count;
  }

  return {output};
}

std::any reqPayService ( Domain::Session::SessionBase & session, const std::vector<std::string> & args )
{
  session._logger << "Patient requested " + args[0];
  if(args[0] == "Create Medical Appointment") {
    std::string results = "Payment methods: \n Credit card \n Debit card \n Cash";
    return {results};
  }

  else if(args[0] == "Pay Medical Invoice") {
    std::string results = "Payment methods: \n Credit card \n Debit card \n Direct deposit \n Cash";
    return {results};
  }

  else if(args[0] == "Generate Medical Report") {
    std::string results = "Payment methods: \n Credit card \n Debit card";
    return {results};
  }

  else if(args[0] == "Generate Prescription History") {
    std::string results = "Payment methods: \n Credit card \n Debit card \n Direct deposit";
    return {results};
  }
}

std::any reqPaymentOption ( Domain::Session::SessionBase & session, const std::vector<std::string> & args )
{
  std::string output = "Here is the your payment receipt: You made a payment using the card ending with " + args[0] + "with the cvv code of " + args[1] + ".";
  return {output};
}

 /*
   std::any userAuthenticate( Domain::Session::SessionBase & session, const std::vector<std::string> & args)
  {
    // should the authentication go here?
    std::string results = "testing";
    session._logger << "this is a test: " + results;
    return {results};
  }
  */
}    // anonymous (private) working area

 


// these are the different Session namespaces
namespace Domain::Session
{
  // maybe this can be used
  SessionBase::SessionBase( const std::string & description, const UserCredentials & credentials ) : _credentials( credentials ), _name( description )
  {
    _logger << "Session \"" + _name + "\" being used and has been successfully initialized";
  }




  SessionBase::~SessionBase() noexcept
  {
    _logger << "Session \"" + _name + "\" shutdown successfully";
  }




  std::vector<std::string> SessionBase::getCommands()
  {
    std::vector<std::string> availableCommands;
    availableCommands.reserve( _commandDispatch.size() );

    for( const auto & [command, function] : _commandDispatch ) availableCommands.emplace_back( command );

    return availableCommands;
  }




  std::any SessionBase::executeCommand( const std::string & command, const std::vector<std::string> & args )
  {
    std::string parameters;
    for( const auto & arg : args )  parameters += '"' + arg + "\"  ";
    _logger << "Responding to \"" + command + "\" request with parameters: " + parameters;

    auto it = _commandDispatch.find( command );
    if( it == _commandDispatch.end() )
    {
      std::string message = __func__;
      message += " attempt to execute \"" + command + "\" failed, no such command";

      _logger << message;
      throw BadCommand( message );
    }

    auto results = it->second( *this, args);

    if( results.has_value() )
    {
      // The type of result depends on function called.  Let's assume strings for now ...
      _logger << "Responding with: \"" + std::any_cast<const std::string &>( results ) + '"';
    }

    return results;
  }








  // 2) Now map the above system events to roles authorized to make such a request.  Many roles can request the same event, and many
  //    events can be requested by a single role.
  
  // this works
  PatientSession::PatientSession( const UserCredentials & credentials ) : SessionBase( "Patient", credentials )
  {
    _commandDispatch = { {"Help",            help        },
                         {"Shutdown System", shutdown    },
                         {"Create Medical Appointment", createMedicalAppt}, 
                         {"Request Doctor", reqDoctor},
                         {"Generate List of Services", generateServicesList},
                         {"Book Appointment", bookAppt},
                         {"Pay for Service", reqPayService},
                         {"Request Payment Option", reqPaymentOption }};
  }
  

/*

    AdministratorSession::AdministratorSession( const UserCredentials & credentials ) : SessionBase( "Administrator", credentials )
  {
    _commandDispatch = { {"Help",            help        },
                         {"Reset Account",   resetAccount},
                         {"Shutdown System", shutdown    } };
  }

  BorrowerSession::BorrowerSession( const UserCredentials & credentials ) : SessionBase( "Borrower", credentials )
  {
    _commandDispatch = { {"Checkout Book", checkoutBook},
                         {"Help",          help        },
                         {"Pay Fines",     payFines    },
                         {"Return Book",   returnBook  } };
  }




  LibrarianSession::LibrarianSession( const UserCredentials & credentials ) : SessionBase( "Librarian", credentials )
  {
    _commandDispatch = { {"Checkout Book", checkoutBook},
                         {"Collect Fines", collectFines},
                         {"Help",          help        },
                         {"Open Archives", openArchives} };
  }




  ManagementSession::ManagementSession( const UserCredentials & credentials ) : SessionBase( "Management", credentials )
  {
    _commandDispatch = { {"Bug People", bugPeople},
                         {"Help",       help} };
  }
*/
}    // namespace Domain::Session
