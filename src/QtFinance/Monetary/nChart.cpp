#include <qtfinance.h>

N::Finance::Chart:: Chart   ( void            )
                  : barType ( N::Forex::NoBar )
                  , ZigZag  ( 0               )
{
}

N::Finance::Chart::~Chart(void)
{
}

// dV = Volume / (High - Low + 1)
double N::Finance::Chart::DivideVolume(void)
{
  double pt = (double)iVolume  ;
  int    dt = iHigh - iLow + 1 ;
  nKickOut ( dt <= 0 , 0 )     ;
  return ( pt / dt )           ;
}

void N::Finance::Chart::Pressure(int lowest,int highest,double * volume)
{
  nDropOut ( nGreater ( lowest , highest ) ) ;
  double dp = DivideVolume()                 ;
  for (int i=iLow;i<=iHigh;i++)              {
    if (i>=lowest && i<=highest)             {
      volume[i-lowest] += dp                 ;
    }                                        ;
  }                                          ;
}

void N::Finance::Chart::Calculation(void)
{
  bool process = false                     ;
  barType = N::Forex::NoBar                ;
  if (iOpen==iClose)                       {
    if ((iOpen!=iHigh) && (iClose!=iLow))  {
      process = true                       ;
      barType = N::Forex::Flat             ;
    } else
    if ((iOpen==iHigh) && (iClose==iHigh)) {
      process = true                       ;
      barType = N::Forex::Top              ;
    } else
    if ((iOpen==iLow ) && (iClose==iLow )) {
      process = true                       ;
      barType = N::Forex::Bottom           ;
    }                                      ;
  } else
  if (iOpen> iClose)                       {
    if (iClose==iLow)                      {
      process = true                       ;
      barType = N::Forex::BidSide          ;
    } else
    if (iClose> iLow)                      {
      process = true                       ;
      barType = N::Forex::BidBack          ;
    }                                      ;
  } else
  if (iOpen< iClose)                       {
    if (iOpen==iHigh)                      {
      process = true                       ;
      barType = N::Forex::AskSide          ;
    } else
    if (iOpen<iHigh)                       {
      process = true                       ;
      barType = N::Forex::AskBack          ;
    }                                      ;
  }                                        ;
  if (!process)                            {
    barType = N::Forex::Others             ;
  }                                        ;
}

void N::Finance::Chart::toStructure(ChartStructure * ts)
{
  ts -> Server  = Server  ;
  ts -> UTC     = UTC     ;
  ts -> Local   = Local   ;
  ts -> Open    = Open    ;
  ts -> Close   = Close   ;
  ts -> High    = High    ;
  ts -> Mid     = Mid     ;
  ts -> Low     = Low     ;
  ts -> Volume  = Volume  ;
  ts -> iOpen   = iOpen   ;
  ts -> iClose  = iClose  ;
  ts -> iHigh   = iHigh   ;
  ts -> iMid    = iMid    ;
  ts -> iLow    = iLow    ;
  ts -> iVolume = iVolume ;
  ts -> barType = barType ;
  ts -> ZigZag  = ZigZag  ;
}

void N::Finance::Chart::fromStructure(ChartStructure * ts)
{
  Server  = ts -> Server  ;
  UTC     = ts -> UTC     ;
  Local   = ts -> Local   ;
  Open    = ts -> Open    ;
  Close   = ts -> Close   ;
  High    = ts -> High    ;
  Mid     = ts -> Mid     ;
  Low     = ts -> Low     ;
  Volume  = ts -> Volume  ;
  iOpen   = ts -> iOpen   ;
  iClose  = ts -> iClose  ;
  iHigh   = ts -> iHigh   ;
  iMid    = ts -> iMid    ;
  iLow    = ts -> iLow    ;
  iVolume = ts -> iVolume ;
  barType = ts -> barType ;
  ZigZag  = ts -> ZigZag  ;
}

bool N::Finance::Chart::FromSQL(SqlConnection & SC,int multiply)
{
  Server  = SC . Tuid ( 1 )      ;
  iOpen   = SC . Int  ( 2 )      ;
  iHigh   = SC . Int  ( 3 )      ;
  iLow    = SC . Int  ( 4 )      ;
  iClose  = SC . Int  ( 5 )      ;
  iVolume = SC . Tuid ( 6 )      ;
  ////////////////////////////////
  iMid    = ( iHigh + iLow ) / 2 ;
  ////////////////////////////////
  Open    = iOpen                ;
  Close   = iClose               ;
  High    = iHigh                ;
  Low     = iLow                 ;
  Volume  = iVolume              ;
  ////////////////////////////////
  Open   /= multiply             ;
  Close  /= multiply             ;
  High   /= multiply             ;
  Low    /= multiply             ;
  Volume /= 100000               ;
  Mid     = ( High + Low ) / 2   ;
  ////////////////////////////////
  return true                    ;
}

bool N::Finance::Chart::Within(TUID t,int period)
{
  QDateTime T                                  ;
  T . setTimeSpec  ( Qt::UTC                 ) ;
  T . setDate      ( QDate ( 1970 , 1 , 1  ) ) ;
  T . setTime      ( QTime ( 0 , 0 , 0 , 0 ) ) ;
  T = T . addMSecs ( t                       ) ;
  return Within ( T , period )                 ;
}

bool N::Finance::Chart::Within(QDateTime t,int period)
{
  int  T  = 0                                                                ;
  TUID dT = 0                                                                ;
  TUID u                                                                     ;
  TUID e                                                                     ;
  TUID z                                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  z  = t . toTime_t ( )                                                      ;
  z *= 1000                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( period )                                                          {
    case Forex::M1                                                           :
    case Forex::M5                                                           :
    case Forex::M15                                                          :
    case Forex::M30                                                          :
    case Forex::D1                                                           :
    case Forex::W1                                                           :
      dT = period                                                            ;
    break                                                                    ;
    case Forex::MN1                                                          :
      T = 1                                                                  ;
    break                                                                    ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( 0 == T )                                                              {
    dT *= 1000                                                               ;
    u   = UTC                                                                ;
    e   = u + dT - 1                                                         ;
    if ( z < u ) return false                                                ;
    if ( z > e ) return false                                                ;
    return true                                                              ;
  } else                                                                     {
    QDateTime Z                                                              ;
    Z . setTimeSpec  ( Qt::UTC )                                             ;
    Z = Z . addMSecs ( UTC     )                                             ;
    if ( Z . date ( ) . year  ( ) != t . date ( ) . year  ( ) ) return false ;
    if ( Z . date ( ) . month ( ) != t . date ( ) . month ( ) ) return false ;
    return true                                                              ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return false                                                               ;
}

void N::Finance::Chart::Empty(void)
{
  Open   = -1           ;
  Close  = 1000000000.0 ;
  High   = -1           ;
  Mid    = 0            ;
  Low    = 1000000000.0 ;
  Volume = 0            ;
}

bool N::Finance::Chart::Within(Tick & tick,int period)
{
  return Within ( tick . UTC , period ) ;
}

void N::Finance::Chart::Add(Tick & tick)
{
  if ( Open < 0 ) Open = tick . Bid          ;
  Close = tick . Bid                         ;
  if ( High < tick . Ask ) High = tick . Ask ;
  if ( Low  > tick . Bid ) Low  = tick . Bid ;
  if ( tick . BidVolume > tick . AskVolume ) {
    Volume += tick . BidVolume               ;
  } else                                     {
    Volume += tick . AskVolume               ;
  }                                          ;
}

void N::Finance::Chart::TickFinal(int multiply)
{
  Mid      = ( High + Low ) / 2 ;
  ///////////////////////////////
  iOpen    = Open               ;
  iClose   = Close              ;
  iHigh    = High               ;
  iLow     = Low                ;
  iMid     = Mid                ;
  iVolume  = Volume             ;
  ///////////////////////////////
  iOpen   *= multiply           ;
  iClose  *= multiply           ;
  iHigh   *= multiply           ;
  iLow    *= multiply           ;
  iMid    *= multiply           ;
  iVolume *= 100000             ;
}
