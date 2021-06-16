#include <qtfinance.h>

N::Finance::Orders:: Orders (void)
                   : login  (0   )
{
  functions . setEnabled ( "Trading" , true ) ;
}

N::Finance::Orders::~Orders (void)
{
  CUIDs keys = orders . keys ( ) ;
  int   id                       ;
  foreach ( id , keys )          {
    delete orders [ id ]         ;
//    take ( id )                  ;
  }                              ;
  orders . clear ( )             ;
}

#ifdef QT_QTMTAPI_LIB

N::Finance::Orders & N::Finance::Orders::assign(int total,TradeRecord * trade)
{
  CUIDs   keys = orders . keys ( )      ;
  CUIDs   mews                          ;
  int     id                            ;
  ///////////////////////////////////////
  if ( total <= 0 )                     {
    foreach ( id , keys )               {
      enabled [ id ] = 0                ;
    }                                   ;
    return (*this)                      ;
  }                                     ;
  ///////////////////////////////////////
  for (int i=0;i<total;i++)             {
    if ( login == 0 )                   {
      id = trade [ i ] . order          ;
      assure ( id )                     ;
      (*orders[id]) = trade [ i ]       ;
      mews << id                        ;
      enabled [ id ] = 1                ;
    } else
    if ( login == trade [ i ] . login ) {
      id = trade [ i ] . order          ;
      assure ( id )                     ;
      (*orders[id]) = trade [ i ]       ;
      mews << id                        ;
      enabled [ id ] = 1                ;
    }                                   ;
  }                                     ;
  ///////////////////////////////////////
  foreach ( id , keys )                 {
    if ( ! mews . contains ( id ) )     {
      enabled [ id ] = 0                ;
    }                                   ;
  }                                     ;
  ///////////////////////////////////////
  return (*this)                        ;
}

#endif

void N::Finance::Orders::assure(int order)
{
  if ( orders . contains ( order ) ) return            ;
  orders  [ order ]  = new Order            ( )        ;
  enabled [ order ]  = 0                               ;
}

bool N::Finance::Orders::contains(int order)
{
  return orders . contains ( order ) ;
}

N::Finance::Order & N::Finance::Orders::operator [] (int order)
{
  return ( * ( orders [ order ] ) ) ;
}

N::Finance::Order & N::Finance::Orders::operator += (const Order & order)
{
  int id = order . order            ;
  if ( ! orders . contains ( id ) ) {
    orders [ id ] = new Order ( )   ;
  }                                 ;
  ( * ( orders [ id ] ) ) = order   ;
  return ( * ( orders [ id ] ) )    ;
}

int N::Finance::Orders::count(void)
{
  return orders . count ( ) ;
}

CUIDs N::Finance::Orders::keys(void)
{
  return orders . keys ( ) ;
}

int N::Finance::Orders::take(int order)
{
  if ( orders.contains(order))   {
    Order * p = orders [ order ] ;
    orders . remove ( order )    ;
    delete p                     ;
  }                              ;
  return orders . count ( )      ;
}

void N::Finance::Orders::operate(void)
{
}

bool N::Finance::Orders::hasEngine(void)
{
  return false ;
}

QScriptEngine * N::Finance::Orders::Engine(void)
{
  return NULL ;
}

void N::Finance::Orders::runEngine (
       QScriptValue & api          ,
       QString        symbol       ,
       QDateTime      CTM          ,
       double         bid          ,
       double         ask          )
{
}
