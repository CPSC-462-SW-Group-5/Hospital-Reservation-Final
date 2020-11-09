#pragma once
#include <string>
#include <vector>
// this is our Domain for Reservation
namespace Domain::Reservation    
{
  // Reservation Package within the Domain Layer Abstract class
  class ReservationHandler // this refers to the session that's being handled
  {
  public:
    virtual ~ReservationHandler() noexcept = 0; // no exception

    virtual std::vector<std::string> createMedicalAppt( std::string issue ) = 0;
    virtual std::vector<std::string> reqDoctor( std::string doctor_name )   = 0;
    virtual std::string              bookAppt( std::string doctor_name, std::string date, std::string time ) = 0;

  protected:
    // Copy assignment operators, protected to prevent mix derived-type assignments
    ReservationHandler & operator=( const ReservationHandler & rhs ) = default;    // copy assignment
    ReservationHandler & operator=( ReservationHandler && rhs ) = default;    // move assignment
  
  };

  inline ReservationHandler::~ReservationHandler() noexcept = default;


}