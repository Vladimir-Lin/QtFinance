#include <qtfinance.h>

N::MtChannel:: MtChannel(void)
{
  QDateTime T = nTimeNow           ;
  Time = T . toMSecsSinceEpoch ( ) ;
  for (int i=0;i<4;i++)            {
    Sent     [ i ] = 0             ;
    Received [ i ] = 0             ;
  }                                ;
}

N::MtChannel:: MtChannel(const MtChannel & channel)
{
  Time = channel . Time                       ;
  for (int i=0;i<4;i++)                       {
    Sent     [ i ] = channel . Sent     [ i ] ;
    Received [ i ] = channel . Received [ i ] ;
  }                                           ;
}

N::MtChannel::~MtChannel(void)
{
}

N::MtChannel & N::MtChannel::operator = (const MtChannel & channel)
{
  Time = channel . Time                       ;
  for (int i=0;i<4;i++)                       {
    Sent     [ i ] = channel . Sent     [ i ] ;
    Received [ i ] = channel . Received [ i ] ;
  }                                           ;
  return ME                                   ;
}

N::MtChannel & N::MtChannel::operator -= (const MtChannel & channel)
{
  Sent     [ 0 ] = 0                              ;
  Received [ 0 ] = 0                              ;
  for (int i=1;i<4;i++)                           {
    qint64 s = channel . Sent     [ i ]           ;
    qint64 r = channel . Received [ i ]           ;
    if ( s < Sent     [ i ] ) Sent     [ i ] -= s ;
    if ( r < Received [ i ] ) Received [ i ] -= r ;
    Sent     [ 0 ] += Sent     [ i ]              ;
    Received [ 0 ] += Received [ i ]              ;
  }                                               ;
  return ME                                       ;
}

void N::MtChannel::setSent(int API,qint64 value)
{
  switch ( API )               {
    case QtMtAPI::MtApiManager :
      Sent [ 0 ] += value      ;
      Sent [ 1 ]  = value      ;
    break                      ;
    case QtMtAPI::MtApiDealer  :
      Sent [ 0 ] += value      ;
      Sent [ 2 ]  = value      ;
    break                      ;
    case QtMtAPI::MtApiPump    :
      Sent [ 0 ] += value      ;
      Sent [ 3 ]  = value      ;
    break                      ;
  }                            ;
}

void N::MtChannel::setReceived(int API,qint64 value)
{
  switch ( API )               {
    case QtMtAPI::MtApiManager :
      Received [ 0 ] += value  ;
      Received [ 1 ]  = value  ;
    break                      ;
    case QtMtAPI::MtApiDealer  :
      Received [ 0 ] += value  ;
      Received [ 2 ]  = value  ;
    break                      ;
    case QtMtAPI::MtApiPump    :
      Received [ 0 ] += value  ;
      Received [ 3 ]  = value  ;
    break                      ;
  }                            ;
}
