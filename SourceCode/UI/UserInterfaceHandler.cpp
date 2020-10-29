#include "UI/UserInterfaceHandler.hpp"

#include <memory>    // unique_ptr, make_unique

#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

#include "UI/SimpleUI.hpp"
#include "UI/SystemDriverUI.hpp"




namespace UI
{
  UserInterfaceHandler::~UserInterfaceHandler() noexcept = default;




  std::unique_ptr<UserInterfaceHandler> UserInterfaceHandler::createUI()
  {
    auto & persistantData = TechnicalServices::Persistence::PersistenceHandler::instance();
    auto   requesedUI     = persistantData["Component.UI"];


    if     ( requesedUI == "Simple UI"     ) return std::make_unique<UI::SimpleUI>      ();
    else if( requesedUI == "Contracted UI" ) return std::make_unique<UI::SystemDriverUI>();

    throw BadUIRequest( "Unknown User Interface object requested: \"" + requesedUI + "\"\n  detected in function " + __func__);
  }
}    // namespace UI
