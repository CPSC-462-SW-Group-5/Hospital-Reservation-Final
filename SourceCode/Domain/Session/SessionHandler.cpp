#include "Domain/Session/SessionHandler.hpp"

#include <algorithm>    // std::any_of()
#include <memory>       // unique_ptr, make_unique<>()
#include <stdexcept>    // logic_error
#include <string>

#include "Domain/Session/Session.hpp"

#include "TechnicalServices/Persistence/PersistenceHandler.hpp"




namespace Domain::Session
{
  SessionHandler::~SessionHandler() noexcept = default;




  // returns a specialized object specific to the specified role
  std::unique_ptr<SessionHandler> SessionHandler::userAuthenticates( const UserCredentials & credentials )
  {
    // Just as a smart defensive strategy, one should verify this role is one of the roles in the DB's legal value list.  I'll come
    // back to that

    // This is a good example of a Factory - the function takes the "order" (role) and builds the "product" (session) to fulfill the
    // order. This, however, still leaks knowledge of the kinds of sessions to the client, after all the client needs to specify
    // with role.

    // ToDo: Make this an Abstract Factory by:
    //  1) removing the parameter from the function's signature :  std::unique_ptr<SessionHandler>  SessionHandler::userAuthenticates();
    //  2) read the role from a proprieties files or (preferred) look up the role in the persistent data

    // Authenticate the requester
    try
    {
      auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
      UserCredentials credentialsFromDB = persistentData.findCredentialsByName( credentials.userName );

      // 1)  Perform the authentication
      // std::set_intersection might be a better choice, but here I'm assuming there will be one and only one role in the passed-in
      // credentials I just need to verify the requested role is in the set of authorized roles.  Someday, if a user can sign in
      // with many roles combined, I may have to revisit this approach.  But for now, this is good enough.
      if(    credentials.userName   == credentialsFromDB.userName
          && credentials.passPhrase == credentialsFromDB.passPhrase
          && std::any_of( credentialsFromDB.roles.cbegin(), credentialsFromDB.roles.cend(),
                          [&]( const std::string & role ) { return credentials.roles.size() > 0 && credentials.roles[0] == role; }
                        )
        )
      {
        // 2) If authenticated user is authorized for the selected role, create a session specific for that role
        
        // added Patient credential role
        if( credentials.roles[0] == "Patient" ) return std::make_unique<Domain::Session::PatientSession> (credentials);
        throw std::logic_error( "Invalid role requested in function " + std::string(__func__) ); // Oops, should never get here but ...  Throw something
      }
    }
    catch( const TechnicalServices::Persistence::PersistenceHandler::NoSuchUser & ) {}  // Catch and ignore this anticipated condition

    return nullptr;
  }
} // namespace Domain::Session
