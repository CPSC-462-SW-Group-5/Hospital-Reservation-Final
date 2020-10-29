#include <memory>    // unique_ptr

#include "TechnicalServices/Logging/LoggerHandler.hpp"
#include "TechnicalServices/Logging/SimpleLogger.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"


namespace TechnicalServices::Logging
{
  std::unique_ptr<LoggerHandler> LoggerHandler::create( std::ostream & loggingStream )
  {
    auto & persistantData  = TechnicalServices::Persistence::PersistenceHandler::instance();
    auto   requestedLogger = persistantData["Component.Logger"];

    if( requestedLogger == "Simple Logger" ) return std::make_unique<SimpleLogger>( loggingStream );

    throw BadLoggerRequest( "Unknown Logger object requested: \"" + requestedLogger + "\"\n  detected in function " + __func__ );
  }
}    // namespace TechnicalServices::Logging
