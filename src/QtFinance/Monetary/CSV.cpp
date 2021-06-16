#include <qtfinance.h>

bool N::Finance::toChart(QByteArray & Line,Chart & chart)
{
  if ( Line . size ( ) <= 0 ) return false                          ;
  QString     L = QString::fromUtf8(Line)                           ;
  QStringList S                                                     ;
  L = L . replace ( "\r" , "" )                                     ;
  L = L . replace ( "\n" , "" )                                     ;
  S = L . split   ( ","       )                                     ;
  if ( 7 != S . count ( ) ) return false                            ;
  QString   F = S[0] + " " + S[1] + ":00"                           ;
  QDateTime T = QDateTime::fromString ( F , "yyyy.MM.dd hh:mm:ss" ) ;
  QDateTime N                                                       ;
  N . setTimeSpec ( Qt::UTC      )                                  ;
  N . setDate     ( T . date ( ) )                                  ;
  N . setTime     ( T . time ( ) )                                  ;
  chart . Server = N       . toMSecsSinceEpoch ( )                  ;
  chart . Open   = S [ 2 ] . toDouble          ( )                  ;
  chart . High   = S [ 3 ] . toDouble          ( )                  ;
  chart . Low    = S [ 4 ] . toDouble          ( )                  ;
  chart . Close  = S [ 5 ] . toDouble          ( )                  ;
  chart . Volume = S [ 6 ] . toDouble          ( )                  ;
  return true;
}

bool N::Finance::toTick(QByteArray & Line,Tick & tick)
{
  nKickOut ( Line.size() <= 0 , false )                          ;
  QString     L = QString::fromUtf8(Line)                        ;
  QStringList S                                                  ;
  L = L . replace ( "\r" , "" )                                  ;
  L = L . replace ( "\n" , "" )                                  ;
  S = L . split   ( ","       )                                  ;
  nKickOut ( 5 != S . count ( ) , false )                        ;
  QDateTime T                                                    ;
  TUID      Z                                                    ;
  T = QDateTime::fromString ( S[0] , "yyyy.MM.dd hh:mm:ss.zzz" ) ;
  Z = T . toMSecsSinceEpoch ( )                                  ;
  tick . Time      = Z                                           ;
  tick . Bid       = S [ 1 ] . toDouble ( )                      ;
  tick . Ask       = S [ 2 ] . toDouble ( )                      ;
  tick . BidVolume = S [ 3 ] . toDouble ( )                      ;
  tick . AskVolume = S [ 4 ] . toDouble ( )                      ;
  return true                                                    ;
}
