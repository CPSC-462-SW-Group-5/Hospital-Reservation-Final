#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>      // localtime()
#include <iomanip>    // put_time()

#include "TechnicalServices/Logging/LoggerHandler.hpp"


namespace TechnicalServices::Logging
{
  class SimpleLogger : public TechnicalServices::Logging::LoggerHandler
  {
    public:
      //Constructors
      SimpleLogger( std::ostream & loggingStream = std::clog);

      // Operations
      SimpleLogger & operator<< ( const std::string & message ) override;

      // Destructor
      ~SimpleLogger() noexcept override;


    protected:
      std::ostream & _loggingStream;
  }; // class SimpleLogger






  /*****************************************************************************
  ** Inline implementations
  ******************************************************************************/
  inline SimpleLogger::SimpleLogger( std::ostream & loggingStream )
    : _loggingStream( loggingStream )
  {
    // Let's set the buffering behavior of the stream so the order and context of inserted information is preserved. Force the
    // buffer to be flushed after each insertion. One normally doesn't want to do this, but log files are very often viewed in real
    // time as the program executes (e.g. tailing a file).  Waiting for the buffer to fill before seeing the contents may give false
    // information in such cases.
    _loggingStream << std::unitbuf;

    // call operator<< so the time stamp and newline get inserted too
    *this << "Simple Logger being used and has been successfully initialized";
  }


  inline SimpleLogger::~SimpleLogger() noexcept
  {
    *this << "Simple Logger shutdown successfully";
  }



  inline SimpleLogger & SimpleLogger::operator<< ( const std::string & message )
  {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    // Updated 2020-Sep-03
    //
    // C++20 provides a much needed refresh to the Chrono library, but until then I have to used some C functions to get the time
    // formatted how I like.  And even then, MS Visual has (correctly imo) deprecated some of the C functions because they are
    // inherently unsafe - in this case the std::localtime() function.  So I'm going to limp along here until C++20 is released -
    // but (note to self) revisit this and rewrite when the time comes.
    #if defined(_MSC_VER)  &&  _MSC_VER >= 1920     // if using Visual Studio 2019 version 16.0 or earlier  (I believe VSC++ started enforcing the deprecation here)
      std::tm destination;
      ::localtime_s(&destination, &now);
      _loggingStream << std::put_time( &destination, "%Y-%m-%d %X" ) << " | ";
    
    #else
      _loggingStream << std::put_time( std::localtime( &now ), "%Y-%m-%d %X" ) << " | ";
    #endif

    _loggingStream << message << '\n';

    return *this;
  }

} // namespace TechnicalServices::Logging
