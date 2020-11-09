#pragma once
#include <string>
#include <vector>

// this is our Domain for Payment
namespace Domain::Payment    
{
  // Reservation Package within the Domain Layer Abstract class
  class PaymentHandler   // this refers to the session that's being handled
  {
  public:
    virtual ~PaymentHandler() noexcept = 0;    // no exception

    virtual std::vector<std::string> generateServicesList(int visit_number) = 0;
    virtual std::vector<std::string> reqPayService(std::string nameOfService)        = 0;
    virtual std::string              reqPaymentOption( std::string cardNumber, std::string cvv ) = 0;
    // pass strings by referce tip from Professor
    // the header files look good, the return value matches with what the SSD intends
  protected:
    // Copy assignment operators, protected to prevent mix derived-type assignments
    PaymentHandler & operator=( const PaymentHandler & rhs ) = default;    // copy assignment
    PaymentHandler & operator=( PaymentHandler && rhs ) = default;         // move assignment
  };

  inline PaymentHandler::~PaymentHandler() noexcept = default;


}    // namespace Domain::Reservation