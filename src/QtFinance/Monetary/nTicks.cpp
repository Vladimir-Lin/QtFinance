#include <qtfinance.h>

N::Finance::Ticks:: Ticks (void)
{
}

N::Finance::Ticks::~Ticks (void)
{
}

QList<N::Finance::Tick> & N::Finance::Ticks::list(void)
{
  return ticks ;
}

QVariant & N::Finance::Ticks::value(QString key)
{
  return variables [ key ] ;
}

QVariant & N::Finance::Ticks::setValue (QString key,QVariant v)
{
  variables [ key ] = v    ;
  return variables [ key ] ;
}

int N::Finance::Ticks::add(Tick & tick)
{
  ticks . push_back( tick ) ;
  return ticks . count ( )  ;
}

int N::Finance::Ticks::count(void)
{
  return ticks . count ( )  ;
}

int N::Finance::Ticks::takeAt(int index)
{
  ticks . takeAt ( index ) ;
  return ticks . count ( ) ;
}

N::Finance::Tick & N::Finance::Ticks::first(void)
{
  return ticks . first ( ) ;
}

N::Finance::Tick & N::Finance::Ticks::last(void)
{
  return ticks . last ( ) ;
}

const N::Finance::Tick & N::Finance::Ticks::at(int index)
{
  return ticks . at ( index ) ;
}

N::Finance::Tick & N::Finance::Ticks::operator = (int index)
{
  return ticks [ index ] ;
}

void N::Finance::Ticks::operate(void)
{
  bool td = false                                       ;
  if ( variables . contains ( "Range" ) )               {
    RuleOut ( variables [ "Range" ] . toInt ( ) )       ;
  }                                                     ;
  if ( variables [ "Highest" ] . toBool ( ) ) td = true ;
  if ( variables [ "Lowest"  ] . toBool ( ) ) td = true ;
  if ( td ) MinMax ( )                                  ;
}

void N::Finance::Ticks::RuleOut(int msec)
{
  if ( count ( ) <= 0 ) return                          ;
  TUID B = last ( ) . Time                              ;
  B -= msec                                             ;
  while ( ( count ( ) > 0 ) && ( first() . Time < B ) ) {
    takeAt ( 0 )                                        ;
  }                                                     ;
}

void N::Finance::Ticks::MinMax(void)
{
  double smallest = 1000000000.0              ;
  double biggest  = -1.0                      ;
  int    t        = count ( )                 ;
  double bid                                  ;
  double ask                                  ;
  for (int i=0;i<t;i++)                       {
    bid = ticks [ i ] . Bid                   ;
    ask = ticks [ i ] . Ask                   ;
    if ( biggest  < ask ) biggest  = ask      ;
    if ( smallest > bid ) smallest = bid      ;
  }                                           ;
  if ( variables [ "Highest" ] . toBool ( ) ) {
    variables [ "High" ] = biggest            ;
  }                                           ;
  if ( variables [ "Lowest"  ] . toBool ( ) ) {
    variables [ "Low"  ] = smallest           ;
  }                                           ;
}

int N::Finance::Ticks::Array(int item,int size,double * array)
{
  int total = count ( )                 ;
  int s     = 0                         ;
  if ( size > total ) size = total      ;
  s = total - size                      ;
  switch ( item )                       {
    case BidItem                        :
      for (int i=0;i<size;i++,s++)      {
        array [ i ] = ticks [ s ] . Bid ;
      }                                 ;
    break                               ;
    case AskItem                        :
      for (int i=0;i<size;i++,s++)      {
        array [ i ] = ticks [ s ] . Ask ;
      }                                 ;
    break                               ;
    case MidItem                        :
      for (int i=0;i<size;i++,s++)      {
        array [ i ] = ticks [ s ] . Mid ;
      }                                 ;
    break                               ;
  }                                     ;
  return size                           ;
}

int N::Finance::Ticks::FromSQL(SqlConnection & SC,QString Q)
{
  int     multiply = variables [ "Multiply"  ] . toInt    ( ) ;
  qint64  tz       = variables [ "TimeZone"  ] . toInt    ( ) ;
  qint64  lz       = variables [ "LocalZone" ] . toInt    ( ) ;
  QString symbol   = variables [ "Symbol"    ] . toString ( ) ;
  tz *= 1000                                                  ;
  lz *= 1000                                                  ;
  SqlLoopNow ( SC , Q )                                       ;
    Tick tick                                                 ;
    tick   . symbol   = symbol                                ;
    tick   . TimeZone = tz                                    ;
    tick   . FromSQL ( SC , multiply )                        ;
    tick   . Local = tick . UTC + lz                          ;
    ticks << tick                                             ;
  SqlLoopErr ( SC , Q )                                       ;
  SqlLoopEnd ( SC , Q )                                       ;
  return ticks . count ( )                                    ;
}
