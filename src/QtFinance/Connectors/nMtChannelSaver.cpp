#include <qtfinance.h>

N::MtChannelSaver:: MtChannelSaver (void)
                  : MtChannels     (    )
                  , Thread         (    )
{
  Variables [ "Interval" ] = 15 * 1000         ;
  Variables [ "Running"  ] = true              ;
  Variables [ "MaxSize"  ] = 1024 * 1024 * 128 ;
}

N::MtChannelSaver::~MtChannelSaver (void)
{
}

void N::MtChannelSaver::Update(QtMtAPI & api)
{
  mutex [ 0 ] . lock   (     ) ;
  MtChannels::Update   ( api ) ;
  mutex [ 0 ] . unlock (     ) ;
}

void N::MtChannelSaver::Operate(void)
{
  mutex [ 0 ] . lock   ( ) ;
  MtChannels::Operate  ( ) ;
  mutex [ 0 ] . unlock ( ) ;
}

void N::MtChannelSaver::run(int Type,ThreadData * data)
{
  switch ( Type ) {
    case 10001    :
      Saver ( )   ;
    break         ;
  }               ;
}

void N::MtChannelSaver::Saver(void)
{
  while ( Variables [ "Running" ] . toBool ( ) )          {
    Time::msleep ( Variables [ "Interval" ] . toInt ( ) ) ;
    if ( Variables . contains ( "File" ) ) Saving ( )     ;
  }                                                       ;
  if   ( Variables . contains ( "File" ) ) Saving ( )     ;
}

void N::MtChannelSaver::Saving(void)
{
  if ( Channels . count ( ) <= 0 ) return                               ;
  QString    F = Variables [ "File" ] . toString ( )                    ;
  QByteArray A                                                          ;
  qint64     C [ 9 ]                                                    ;
  QFile      R ( F )                                                    ;
  mutex [ 0 ] . lock   ( )                                              ;
  while ( Channels . count ( ) > 0 )                                    {
    C [ 0 ] = Channels [ 0 ] . Time                                     ;
    for (int i=0;i<4;i++) C [ i + 1 ] = Channels [ 0 ] . Sent     [ i ] ;
    for (int i=0;i<4;i++) C [ i + 5 ] = Channels [ 0 ] . Received [ i ] ;
    A . append ( (const char *) C , sizeof(qint64) * 9 )                ;
    Channels . takeAt ( 0 )                                             ;
  }                                                                     ;
  mutex [ 0 ] . unlock ( )                                              ;
  if ( A . size ( ) <= 0                ) return                        ;
  QFileInfo FS ( F )                                                    ;
  qint64    mv = Variables [ "MaxSize" ] . toLongLong ( )               ;
  if ( FS . size ( ) > mv ) QFile::remove ( F )                         ;
  if ( ! R . open ( QIODevice::Append ) ) return                        ;
  R . write ( A )                                                       ;
  R . close (   )                                                       ;
}
