#include <qtfinance.h>

N::Finance::Tick:: Tick(void)
{
  Type      = MetaTrader ;
  symbol    = ""         ;
  Time      = 0          ;
  UTC       = 0          ;
  Local     = 0          ;
  TimeZone  = 0          ;
  Bid       = 0          ;
  Ask       = 0          ;
  BidVolume = 0          ;
  AskVolume = 0          ;
}

N::Finance::Tick::~Tick(void)
{
}

void N::Finance::Tick::setTime(time_t time,time_t fix)
{
  TimeZone = fix        ;
  Time     = time + fix ;
  Time    *= 1000       ;
  UTC      = time       ;
  UTC     *= 1000       ;
}

QDateTime N::Finance::Tick::Timestamp(void)
{
  QDateTime D                                    ;
  D = QDateTime::fromTime_t((time_t)(Time/1000)) ;
  D = D.addMSecs((int)(Time%1000))               ;
  return    D                                    ;
}

void N::Finance::Tick::toStructure(TickStructure * ts)
{
  ts -> Type      = (int) Type                                     ;
  ts -> Time      = Time                                           ;
  ts -> UTC       = UTC                                            ;
  ts -> Local     = Local                                          ;
  ts -> TimeZone  = TimeZone                                       ;
  ts -> Bid       = Bid                                            ;
  ts -> Ask       = Ask                                            ;
  ts -> Mid       = Mid                                            ;
  ts -> BidVolume = BidVolume                                      ;
  ts -> AskVolume = AskVolume                                      ;
  ts -> iBid      = iBid                                           ;
  ts -> iAsk      = iAsk                                           ;
  ts -> iMid      = iMid                                           ;
  ::strcpy ( ts -> Symbol , (char *) symbol.toUtf8().constData() ) ;
}

void N::Finance::Tick::fromStructure(TickStructure * ts)
{
  Type      = (enum TickType) ts -> Type         ;
  symbol    = QString::fromUtf8 ( ts -> Symbol ) ;
  Time      = ts -> Time                         ;
  UTC       = ts -> UTC                          ;
  Local     = ts -> Local                        ;
  TimeZone  = ts -> TimeZone                     ;
  Bid       = ts -> Bid                          ;
  Ask       = ts -> Ask                          ;
  Mid       = ts -> Mid                          ;
  BidVolume = ts -> BidVolume                    ;
  AskVolume = ts -> AskVolume                    ;
  iBid      = ts -> iBid                         ;
  iAsk      = ts -> iAsk                         ;
  iMid      = ts -> iMid                         ;
}

bool N::Finance::Tick::FromSQL(SqlConnection & SC,int multiply)
{
  Time       = SC . Tuid ( 1 )     ;
  iBid       = SC . Int  ( 2 )     ;
  iAsk       = SC . Int  ( 3 )     ;
  BidVolume  = SC . Tuid ( 4 )     ;
  AskVolume  = SC . Tuid ( 5 )     ;
  //////////////////////////////////
  UTC        = Time - TimeZone     ;
  iMid       = ( iBid + iAsk ) / 2 ;
  BidVolume /= 100000              ;
  AskVolume /= 100000              ;
  //////////////////////////////////
  Bid        = iBid                ;
  Ask        = iAsk                ;
  Mid        = iMid                ;
  //////////////////////////////////
  Bid       /= multiply            ;
  Ask       /= multiply            ;
  Mid       /= multiply            ;
  return true                      ;
}

#ifdef QT_QTMTAPI_LIB

void N::Finance::Tick::operator = (SymbolInfo & tick)
{
  Type    = N::Finance::Tick::MetaTrader ;
  symbol  = tick . symbol                ;
  Time    = tick . lasttime              ;
  UTC     = Time - TimeZone              ;
  Time   *= 1000                         ;
  UTC    *= 1000                         ;
  Ask     = tick . ask                   ;
  Bid     = tick . bid                   ;
}

void N::Finance::Tick::operator = (TickInfo & tick)
{
  Type    = N::Finance::Tick::MetaTrader ;
  symbol  = tick . symbol                ;
  Time    = tick . ctm                   ;
  UTC     = Time - TimeZone              ;
  Time   *= 1000                         ;
  UTC    *= 1000                         ;
  Ask     = tick . ask                   ;
  Bid     = tick . bid                   ;
}

#endif
