#include <qtfinance.h>

N::Finance::Symbol:: Symbol   ( void   )
                   : symbol   ( ""     )
                   , digits   ( 4      )
                   , multiply ( 10000  )
                   , point    ( 0.0001 )
{
}

N::Finance::Symbol::~Symbol(void)
{
}

N::Finance::Symbol & N::Finance::Symbol::operator = (Symbol & s)
{
  nMemberCopy ( s , digits     ) ;
  nMemberCopy ( s , multiply   ) ;
  nMemberCopy ( s , point      ) ;
  nMemberCopy ( s , symbol     ) ;
  nMemberCopy ( s , currencies ) ;
  return      ( *this          ) ;
}

void N::Finance::Symbol::set(QString f,QString s,int d,double p)
{
  digits   = d           ;
  point    = p           ;
  symbol   = f + "/" + s ;
  multiply = 1           ;
  currencies << f        ;
  currencies << s        ;
  for (int i=0;i<d;i++)  {
    multiply *= 10       ;
  }                      ;
}

bool N::Finance::Symbol::operator == (Symbol & s)
{
  return ( s.symbol == symbol ) ;
}
