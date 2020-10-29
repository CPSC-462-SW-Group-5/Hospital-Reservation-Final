#pragma once
#include <stdexcept>  // runtime_error
#include <iostream>
#include <iostream>
#include <memory>     // unique_ptr
#include <string>


namespace TechnicalServices::Logging
{
  // Logging Package within the Technical Services Layer Abstract class
  class LoggerHandler
  {
    public:
      // Exceptions
      struct LoggerException    : std::runtime_error {using runtime_error  ::runtime_error;};
      struct   BadLoggerRequest : LoggerException    {using LoggerException::LoggerException;     };

      // Logger Factory, throws BadLoggerRequest
      static std::unique_ptr<LoggerHandler> create( std::ostream & loggingStream = std::clog );    // must be static


      // Operations
      virtual LoggerHandler & operator<< ( const std::string & message ) = 0;


      // Destructor
      // Pure virtual destructor helps force the class to be abstract, but must still be implemented
      virtual ~LoggerHandler() noexcept = 0;


    protected:
      // Copy assignment operators, protected to prevent mix derived-type assignments
      LoggerHandler & operator=( const LoggerHandler  & rhs ) = delete;    // copy assignment
      LoggerHandler & operator=(       LoggerHandler && rhs ) = delete;    // move assignment
  };





  /*****************************************************************************
  ** Inline default implementations
  ******************************************************************************/
  inline LoggerHandler::~LoggerHandler() noexcept = default;
} // namespace TechnicalServices::Logging
