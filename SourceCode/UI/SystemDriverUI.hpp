#pragma once

#include <memory>    // std::unique_ptr

#include "TechnicalServices/Logging/LoggerHandler.hpp"

#include "UI/UserInterfaceHandler.hpp"




namespace UI
{
  /*****************************************************************************
  ** SystemDriver UI definition
  **   SystemDriver UI is a console application meant only as a driver to the Domain Layer application. This UI will someday be
  **   replaced by a more sophisticated, user friendly implementation
  **
  **   This driver executes very specific scenarios, and is not intended to interact with an end-user
  ******************************************************************************/
  class SystemDriverUI : public UI::UserInterfaceHandler
  {
  public:
    // Constructors
    SystemDriverUI();


    // Operations
    void launch() override;


    // Destructor
    ~SystemDriverUI() noexcept override;


  private:
    // These smart pointers hold pointers to lower architectural layer's interfaces
    std::unique_ptr<TechnicalServices::Logging::LoggerHandler> _loggerPtr;

    // convenience reference object enabling standard insertion syntax
    // This line must be physically after the definition of _loggerPtr
    TechnicalServices::Logging::LoggerHandler & _logger = *_loggerPtr;
  };
}    // namespace UI
