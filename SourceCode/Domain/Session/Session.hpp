#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "Domain/Session/SessionHandler.hpp"
#include "TechnicalServices/Logging/LoggerHandler.hpp"


namespace Domain::Session
{
  class SessionBase : public SessionHandler
  {
    public:
      SessionBase( const std::string & description,  const UserCredentials & credentials );

      // Operations
      std::vector<std::string> getCommands   ()                                                                     override;    // retrieves the list of actions (commands)
      std::any                 executeCommand( const std::string & command, const std::vector<std::string> & args ) override;    // executes one of the actions retrieved


      // Destructor
      // Pure virtual destructor helps force the class to be abstract, but must still be implemented
      ~SessionBase() noexcept override = 0;

  protected: 
  public:  // Dispatched functions need access to these attributes, so for now make these public instead of protected
    // Types
    using DispatchTable = std::map<std::string, std::any (*)( Domain::Session::SessionBase &, const std::vector<std::string> & )>;
    friend class Policy;

    // Instance Attributes
    std::unique_ptr<TechnicalServices::Logging::LoggerHandler> _loggerPtr = TechnicalServices::Logging::LoggerHandler::create();
    TechnicalServices::Logging::LoggerHandler &                _logger    = *_loggerPtr;

    UserCredentials const                                      _credentials;
    std::string     const                                      _name      = "Undefined";
    DispatchTable                                              _commandDispatch;
  };    // class SessionBase

  // take these out later 
  /*
  struct AdministratorSession : SessionBase{ AdministratorSession( const UserCredentials & credentials ); };
  struct BorrowerSession      : SessionBase{ BorrowerSession     ( const UserCredentials & credentials ); };
  struct LibrarianSession     : SessionBase{ LibrarianSession    ( const UserCredentials & credentials ); };
  struct ManagementSession    : SessionBase{ ManagementSession   ( const UserCredentials & credentials ); };
  */
  // create our patient session 
  // TODO
  struct PatientSession : SessionBase{ PatientSession(const UserCredentials & credentials ); };


} // namespace Domain::Session
