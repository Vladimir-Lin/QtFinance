#include <qtfinance.h>

N::MtChannels:: MtChannels (void)
{
  Sent     = 0                              ;
  Received = 0                              ;
  Variables [ "Duration" ] = 60 * 60 * 1000 ;
  Variables [ "Deletion" ] = false          ;
}

N::MtChannels::~MtChannels (void)
{
}

void N::MtChannels::Update(QtMtAPI & api)
{
  MtChannel A                                                                ;
  MtChannel B                                                                ;
  A . setSent     ( QtMtAPI::MtApiManager , api . BytesSent     ( QtMtAPI::MtApiManager ) ) ;
  A . setSent     ( QtMtAPI::MtApiDealer  , api . BytesSent     ( QtMtAPI::MtApiDealer  ) ) ;
  A . setSent     ( QtMtAPI::MtApiPump    , api . BytesSent     ( QtMtAPI::MtApiPump    ) ) ;
  A . setReceived ( QtMtAPI::MtApiManager , api . BytesReceived ( QtMtAPI::MtApiManager ) ) ;
  A . setReceived ( QtMtAPI::MtApiDealer  , api . BytesReceived ( QtMtAPI::MtApiDealer  ) ) ;
  A . setReceived ( QtMtAPI::MtApiPump    , api . BytesReceived ( QtMtAPI::MtApiPump    ) ) ;
  B    = A                                                                   ;
  B   -= Base                                                                ;
  Base = A                                                                   ;
  Channels << B                                                              ;
  Sent     += B . Sent     [ 0 ]                                             ;
  Received += B . Received [ 0 ]                                             ;
}

void N::MtChannels::Operate(void)
{
  if ( Variables . contains ( "Duration" ) ) {
    RuleOut ( )                              ;
  }                                          ;
}

QVariant & N::MtChannels::Value(QString key)
{
  return Variables [ key ] ;
}

QVariant & N::MtChannels::setValue(QString key,QVariant value)
{
  Variables [ key ] = value ;
  return Variables [ key ]  ;
}

int N::MtChannels::Obtain(int length,qint64 * times)
{
  int total = Channels . count ( )      ;
  int s                                 ;
  if ( length > total ) length = total  ;
  s = total - length                    ;
  for (int i=0;i<length;i++,s++)        {
    times [ i ] = Channels [ s ] . Time ;
  }                                     ;
  return length                         ;
}

void N::MtChannels::Obtain(int length,int result[8],qint64 ** values)
{
  #pragma omp parallel for
  for ( int i = 0 ; i < 8 ; i++ )                       {
    result [ i ] = Obtain ( i , length , values [ i ] ) ;
  }                                                     ;
}

int N::MtChannels::Obtain(int partindex,int length,qint64 * values)
{
  return Obtain ( partindex / 4 , partindex % 4 , length , values ) ;
}

int N::MtChannels::Obtain(int part,int index,int length,qint64 * values)
{
  int total = Channels . count ( )                         ;
  int s                                                    ;
  if ( length > total ) length = total                     ;
  s = total - length                                       ;
  switch ( part )                                          {
    case 0                                                 :
      for (int i=0;i<length;i++,s++)                       {
        values [ i ] = Channels [ s ] . Sent     [ index ] ;
      }                                                    ;
    break                                                  ;
    case 1                                                 :
      for (int i=0;i<length;i++,s++)                       {
        values [ i ] = Channels [ s ] . Received [ index ] ;
      }                                                    ;
    break                                                  ;
  }                                                        ;
  return length                                            ;
}

int N::MtChannels::count(void)
{
  return Channels . count ( ) ;
}

void N::MtChannels::RuleOut(void)
{
  if ( Channels . count ( ) < 2 ) return             ;
  TUID T = Variables [ "Duration" ] . toLongLong ( ) ;
  TUID Z = Channels . last ( ) . Time                ;
  Z -= T                                             ;
  while ( ( Channels . count ( ) > 0            )   &&
          ( Z > Channels . first ( ) . Time     )  ) {
    Channels . takeAt ( 0 )                          ;
  }                                                  ;
}
