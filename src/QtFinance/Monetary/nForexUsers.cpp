#include <qtfinance.h>

N::Finance::Users:: Users(void)
{
  functions . setEnabled ( "Updated" , false ) ;
}

N::Finance::Users:: Users(const Users & u)
{
  users = u . users ;
}

N::Finance::Users::~Users(void)
{
}

bool N::Finance::Users::contains(int login)
{
  return users . contains ( login ) ;
}

N::Finance::User & N::Finance::Users::operator [] (int login)
{
  assure ( login                 ) ;
  return ( * ( users [ login ] ) ) ;
}

N::Finance::User & N::Finance::Users::operator += (const User & user)
{
  assure ( user . login )                 ;
  ( * ( users [ user . login ] ) ) = user ;
  enabled [ user . login ] = 1            ;
  return ( * ( users [ user . login ] ) ) ;
}

#ifdef QT_MTMANAPI_H

N::Finance::User & N::Finance::Users::operator += (UserRecord & user)
{
  assure ( user . login )                 ;
  ( * ( users [ user . login ] ) ) = user ;
  enabled [ user . login ] = 1            ;
  return ( * ( users [ user . login ] ) ) ;
}

N::Finance::User & N::Finance::Users::operator += (MarginLevel & Margin)
{
  assure ( Margin . login )                   ;
  ( * ( users [ Margin . login ] ) ) = Margin ;
  enabled [ Margin . login ] = 1              ;
  return ( * ( users [ Margin . login ] ) )   ;
}

N::Finance::User & N::Finance::Users::operator += (OnlineRecord & Online)
{
  assure ( Online . login )                   ;
  ( * ( users [ Online . login ] ) ) = Online ;
  enabled [ Online . login ] = 1              ;
  return ( * ( users [ Online . login ] ) )   ;
}

N::Finance::Users & N::Finance::Users::update(int total,UserRecord * u)
{
  if ( total <= 0 ) return ME     ;
  if ( NULL  == u ) return ME     ;
  CUIDs logins = users . keys ( ) ;
  int   login                     ;
  foreach ( login , logins )      {
    enabled [ login ] = 0         ;
  }                               ;
  for (int i=0;i<total;i++)       {
    assure ( u [ i ] . login )    ;
    ME += u [ i ]                 ;
  }                               ;
  return ME                       ;
}

N::Finance::Users & N::Finance::Users::update(int total,MarginLevel * m)
{
  if ( total <= 0 ) return ME  ;
  if ( NULL  == m ) return ME  ;
  for (int i=0;i<total;i++)    {
    assure ( m [ i ] . login ) ;
    ME += m [ i ]              ;
  }                            ;
  return ME                    ;
}

N::Finance::Users & N::Finance::Users::update(int total,OnlineRecord * o)
{
  if ( total <= 0 ) return ME  ;
  if ( NULL  == o ) return ME  ;
  for (int i=0;i<total;i++)    {
    assure ( o [ i ] . login ) ;
    ME += o [ i ]              ;
  }                            ;
  return ME                    ;
}

#endif

int N::Finance::Users::count(void)
{
  return users . count ( ) ;
}

CUIDs N::Finance::Users::logins(void)
{
  return users . keys ( ) ;
}

int N::Finance::Users::take(int login)
{
  if ( ! users . contains ( login ) ) {
    return users . count ( )          ;
  }                                   ;
  User * u = users [ login ]          ;
  users   . remove ( login )          ;
  enabled . remove ( login )          ;
  delete u                            ;
  return users . count ( )            ;
}

void N::Finance::Users::assure(int login)
{
  if ( users . contains ( login ) ) return ;
  users   [ login ] = new User ( )         ;
  enabled [ login ] = 0                    ;
}

void N::Finance::Users::operate(void)
{
}
