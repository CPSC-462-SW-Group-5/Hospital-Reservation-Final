#include "TechnicalServices/Persistence/SimpleDB.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

namespace TechnicalServices::Persistence
{
  PersistenceHandler::~PersistenceHandler() noexcept = default;




  PersistenceHandler & PersistenceHandler::instance()
  {
    // Can't read the DB component preference from the database because the DB has not yet been created. So choosing the database
    // implementation is really a configuration item (set by the vendor before delivery), not an adaptable item (set by the end-user
    // after delivery)
    using SelectedDatabase = SimpleDB;

    static SelectedDatabase instance;    // Note the creation of a DB specialization (derived class), but returning a reference to
                                         // the generalization (base class). Since SimpleDB is-a PersistenceHandler, we can return a
                                         // reference to the base class that refers to a specific derived class.  SimpleDB is
                                         // accessed polymorphicly through the PersistenceHandler interface.  This source file knows
                                         // about the specific SimpleDB derived class, but that's okay.  This source file is not
                                         // delivered with the interface and remains That is, PersistenceHandler.hpp is given to the
                                         // upper architectural layers, but not PersistenceHandler.cpp.
    return instance;
  }
}    // namespace TechnicalServices::Persistence
