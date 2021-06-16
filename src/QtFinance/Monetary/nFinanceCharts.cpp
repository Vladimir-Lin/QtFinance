#include <qtfinance.h>

N::Finance::Charts:: Charts (void)
{
  setValue ( "Digits"   , 5      ) ;
  setValue ( "Multiply" , 100000 ) ;
}

N::Finance::Charts::~Charts (void)
{
}

void N::Finance::Charts::setDigits(int digits)
{
  int multiply = 1                          ;
  for (int i=0;i<digits;i++) multiply *= 10 ;
  setValue ( "Digits"   , digits )          ;
  setValue ( "Multiply" , multiply )        ;
}

QList<N::Finance::Chart> & N::Finance::Charts::list(void)
{
  return charts ;
}

QVariant & N::Finance::Charts::value(QString key)
{
  return variables [ key ] ;
}

QVariant & N::Finance::Charts::setValue (QString key,QVariant v)
{
  variables [ key ] = v    ;
  return variables [ key ] ;
}

int N::Finance::Charts::add(Chart & chart)
{
  charts . push_back( chart ) ;
  return charts . count ( )   ;
}

int N::Finance::Charts::count(void)
{
  return charts . count ( )  ;
}

int N::Finance::Charts::takeAt(int index)
{
  charts . takeAt ( index ) ;
  return charts . count ( ) ;
}

N::Finance::Chart & N::Finance::Charts::first(void)
{
  return charts . first ( ) ;
}

N::Finance::Chart & N::Finance::Charts::last(void)
{
  return charts . last ( ) ;
}

const N::Finance::Chart & N::Finance::Charts::at(int index)
{
  return charts . at ( index ) ;
}

N::Finance::Chart & N::Finance::Charts::operator = (int index)
{
  return charts [ index ] ;
}

void N::Finance::Charts::MinMax(void)
{
  double highest = -1                            ;
  double lowest  = 10000000                      ;
  for (int i=0;i<count();i++)                    {
    double h = at ( i ) . High                   ;
    double l = at ( i ) . Low                    ;
    if ( h > highest ) highest = h               ;
    if ( l < lowest  ) lowest  = l               ;
  }                                              ;
  setValue ( "Highest"  , highest              ) ;
  setValue ( "Lowest"   , lowest               ) ;
  setValue ( "hasRange" , ( highest > lowest ) ) ;
}

void N::Finance::Charts::operate(void)
{
}

int N::Finance::Charts::FromSQL(SqlConnection & SC,QString Q)
{
  int     multiply = variables [ "Multiply"  ] . toInt    ( ) ;
  qint64  tz       = variables [ "TimeZone"  ] . toInt    ( ) ;
  qint64  lz       = variables [ "LocalZone" ] . toInt    ( ) ;
  tz *= 1000                                                  ;
  lz *= 1000                                                  ;
  SqlLoopNow ( SC , Q )                                       ;
    Chart chart                                               ;
    chart . FromSQL ( SC , multiply )                         ;
    chart . UTC   = chart . Server - tz                       ;
    chart . Local = chart . UTC    - lz                       ;
    charts << chart                                           ;
  SqlLoopErr ( SC , Q )                                       ;
  SqlLoopEnd ( SC , Q )                                       ;
  return charts . count ( )                                   ;
}
