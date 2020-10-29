#include "TechnicalServices/Persistence/SimpleDB.hpp"

#include <fstream>    // streamsize
#include <iomanip>    // quoted()
#include <limits>     // numeric_limits
#include <memory>     // make_unique()
#include <string>
#include <vector>

#include "TechnicalServices/Logging/SimpleLogger.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"





namespace
{
  // User defined manipulator with arguments that allows std::istream::ignore to be called "in-line" (chained)
  // Usage example:
  //    stream >> first >> ignore(',') second >> ignore('\n') ;
  struct ignore
  {
    char _seperator;
    ignore( char delimiter = '\n' ) : _seperator( delimiter ) {}
  };

  std::istream & operator>>( std::istream & s, ignore && delimiter )
  {
    s.ignore( std::numeric_limits<std::streamsize>::max(), delimiter._seperator );
    return s;
  }
}    // namespace




namespace TechnicalServices::Persistence
{
  // Design decision/Programming note:
  //  - The persistence database contains adaptation data, and one of the adaptable items is which Logger component to use
  //  - The factory function TechnicalServices::Logging::create(std::ostream &) depends of the persistence database to obtain
  //    through adaptation data which kind of Logging component to create
  //  - However, the Persistence database implementations, like this one, should (must?) be able to log messages
  //  - Therefore, to maintain the design decision to allow Logging to depend on Persistence, but not Persistence to depend on
  //    Logging, we mustn't create this logger through the LoggingHandler interface, but rather select and create a specific Logger
  SimpleDB::SimpleDB() : _loggerPtr( std::make_unique<TechnicalServices::Logging::SimpleLogger>() )
  {
    _logger << "Simple DB being used and has been successfully initialized";

    // NO NEED TO CHANGE
    // Let's look for an adaptation data file, and if found load the contents.  Otherwise create some default values.
    std::ifstream adaptationDataFile( "Library_System_AdaptableData.dat", std::ios::binary );

    if( adaptationDataFile.is_open() )
    {
      // Expected format:  key = value
      // Notes:
      //   1) if key or value contain whitespace, they must be enclosed in double quotes
      //   2) if the same Key appears more than once, last one wins
      //   3) everything after the value is ignored, allowing that space to be used as comments
      //   4) A Key of "//" is ignored, allowing the file to contain comment lines of the form // = ...
      std::string key, value;
      while( adaptationDataFile >> std::quoted( key ) >> ignore( '=' ) >> std::quoted( value ) >> ignore( '\n' ) )   _adaptablePairs[key] = value;
      _adaptablePairs.erase( "//" );
    }

    else
    {
      _adaptablePairs = { /* KEY */               /* Value*/
                          {"Component.Logger",    "Simple Logger"},
                          {"Component.UI",        "Simple UI"}
//                        {"Component.UI",        "Contracted UI"}
                        };
    }
  }


// no need to change

  SimpleDB::~SimpleDB() noexcept
  {
    _logger << "Simple DB shutdown successfully";
  }




  std::vector<std::string> SimpleDB::findRoles()
  {
    //return { "Borrower", "Librarian", "Administrator", "Management" };
    return { "Patient"};
  }



// added test user here
  UserCredentials SimpleDB::findCredentialsByName( const std::string & name )
  {
    static std::vector<UserCredentials> storedUsers =
    {
    // Username    Pass Phrase         Authorized roles
    /*
      {"Tom",     "CPSC 462 Rocks!",  {"Borrower",     "Management"}},
      {"Barbara", "Why am I here?",   {"Borrower"                  }},
      {"Amanda",  "",                 {"Administrator"             }}
    */
      {"mytest@email.com", "team5", {"Patient"}}
    };

    for( const auto & user : storedUsers ) if( user.userName == name ) return user;

    // Name not found, log the error and throw something
    std::string message = __func__;
    message += " attempt to find user \"" + name + "\" failed";

    _logger << message;
    throw PersistenceHandler::NoSuchUser( message );
  }


// no need to touch this for now?

  const std::string & SimpleDB::operator[]( const std::string & key ) const
  {
    auto pair = _adaptablePairs.find( key );
    if( pair != _adaptablePairs.cend() ) return pair->second;

    // Key not found - error
    std::string message = __func__;
    message += " attempt to access adaptation data with Key = \"" + key + "\" failed, no such Key";

    _logger << message;
    throw NoSuchProperty( message );
  }
} // namespace TechnicalServices::Persistence
