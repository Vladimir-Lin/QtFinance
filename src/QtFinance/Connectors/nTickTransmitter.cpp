#include <qtfinance.h>

N::TickTransmitter:: TickTransmitter (void)
                   : Enabler         (    )
{
  setEnabled ( "Ready"    , false ) ;
  setEnabled ( "Transmit" , false ) ;
  setEnabled ( "Deletion" , false ) ;
}

N::TickTransmitter::~TickTransmitter (void)
{
}
