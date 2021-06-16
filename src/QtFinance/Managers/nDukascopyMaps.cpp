#include <qtfinance.h>

#pragma pack(push,1)

typedef struct            { // 4096 bytes
  qint32  Total           ;
  quint64 Flags           ;
  char    Usage  [  256 ] ;
  char    Buffer [ 3828 ] ;
} DukascopyHeader         ;

typedef struct            { // 256 bytes
  char    Symbol [  16  ] ; // Currency Pair Symbol
  char    Start  [  64  ] ; // Start Time
  qint64  Offset          ; // Currency Bit Array Offset
  qint64  End             ; // End       = ( End Time            - Start Time ) seconds
  qint64  Real            ; // Real      = ( Real Start Time     - Start Time ) seconds
  qint64  Completed       ; // Completed = ( Completed Time      - Start Time ) seconds
  qint64  Imported        ; // Imported  = ( Imported Time       - Start Time ) seconds
  qint64  Final           ; // Final     = ( Lastest update Time - Start Time ) seconds
  char    Buffer [ 128 ]  ;
} DukascopyCurrencyStatus ;

#pragma pack(pop)

#define DHTOFFSET ( 64                                  )
#define DCSOFFSET ( DHTOFFSET + sizeof(DukascopyHeader) )
#define DTBOFFSET ( 64 * 1024                           )

#define DHTADDR         (DukascopyHeader         *) ( ( (char *) memoryData ) + DHTOFFSET          )
#define DCSADDR         (DukascopyCurrencyStatus *) ( ( (char *) memoryData ) + DCSOFFSET          )
#define DTBADDR(offset) (unsigned char           *) ( ( (char *) memoryData ) + DTBOFFSET + offset )
#define DCSPACK(symbol) ( (DukascopyCurrencyStatus *) SymbolArray [ symbol ] )

#define SIGNATURE "Dukascopy"

N::DukascopyMaps:: DukascopyMaps ( void )
                 : memoryData    ( NULL )
                 , memorySize    ( 0    )
{
}

N::DukascopyMaps::DukascopyMaps (const DukascopyMaps & dukas)
{
  memoryData = dukas . memoryData ;
  memorySize = dukas . memorySize ;
  TimeArray  = dukas . TimeArray  ;
  DateTimes  = dukas . DateTimes  ;
}

N::DukascopyMaps::~DukascopyMaps (void)
{
}

N::DukascopyMaps & N::DukascopyMaps::operator = (const DukascopyMaps & dukas)
{
  memoryData = dukas . memoryData ;
  memorySize = dukas . memorySize ;
  TimeArray  = dukas . TimeArray  ;
  DateTimes  = dukas . DateTimes  ;
  return ( *this )                ;
}

N::TimeBits & N::DukascopyMaps::operator [ ] (QString key)
{
  return TimeArray [ key ] ;
}

void N::DukascopyMaps::setData(QByteArray & data)
{
  memoryData = data . data ( ) ;
  memorySize = data . size ( ) ;
}

void N::DukascopyMaps::setData(void * data,qint64 size)
{
  memoryData = data ;
  memorySize = size ;
}

QDateTime N::DukascopyMaps::TimeValue(QString key)
{
  return DateTimes [ key ] ;
}

void N::DukascopyMaps::setTime(QString key,QDateTime time)
{
  DateTimes [ key ] = time ;
}

QDateTime N::DukascopyMaps::TimeValue(QString symbol,QString key)
{
  QDateTime start                                                            ;
  QString   k  = key                                                         ;
  qint64    dt = 0                                                           ;
  k      = k . toLower ( )                                                   ;
  start  = QDateTime::fromString ( DCSPACK(symbol) -> Start                  ,
                                   "yyyy/MM/dd-hh:mm:ss"                   ) ;
  ////////////////////////////////////////////////////////////////////////////
  if ( "end"       == k ) dt = DCSPACK(symbol) -> End                   ; else
  if ( "real"      == k ) dt = DCSPACK(symbol) -> Real                  ; else
  if ( "completed" == k ) dt = DCSPACK(symbol) -> Completed             ; else
  if ( "imported"  == k ) dt = DCSPACK(symbol) -> Imported              ; else
  if ( "final"     == k ) dt = DCSPACK(symbol) -> Final                      ;
  ////////////////////////////////////////////////////////////////////////////
  start = start . addSecs ( dt )                                             ;
  return start                                                               ;
}

void N::DukascopyMaps::setTime(QString symbol,QString key,QDateTime time)
{
  QDateTime start                                                            ;
  QString   k  = key                                                         ;
  qint64    dt = 0                                                           ;
  k      = k . toLower ( )                                                   ;
  start  = QDateTime::fromString ( DCSPACK(symbol) -> Start                  ,
                                   "yyyy/MM/dd-hh:mm:ss"                   ) ;
  dt     = start . secsTo        ( time                                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  if ( "end"       == k ) DCSPACK(symbol) -> End       = dt             ; else
  if ( "real"      == k ) DCSPACK(symbol) -> Real      = dt             ; else
  if ( "completed" == k ) DCSPACK(symbol) -> Completed = dt             ; else
  if ( "imported"  == k ) DCSPACK(symbol) -> Imported  = dt             ; else
  if ( "final"     == k ) DCSPACK(symbol) -> Final     = dt                  ;
}

bool N::DukascopyMaps::LoadMaps(QString filename)
{
  QFile F ( filename )                                     ;
  if ( ! F . open ( QIODevice::ReadOnly ) ) return false   ;
  qint64 r = F . read ( (char *) memoryData , memorySize ) ;
  F . close ( )                                            ;
  return ( r == memorySize )                               ;
}

bool N::DukascopyMaps::SaveMaps(QString filename)
{
  QFile F ( filename )                                      ;
  if ( ! F . open ( QIODevice::WriteOnly ) ) return false   ;
  qint64 r = F . write ( (char *) memoryData , memorySize ) ;
  F . close ( )                                             ;
  return ( r == memorySize )                                ;
}

void N::DukascopyMaps::setUsage(QString usage)
{
  DukascopyHeader * dht = DHTADDR            ;
  CopyToArray ( dht -> Usage , 256 , usage ) ;
}

QString N::DukascopyMaps::Usage(void)
{
  DukascopyHeader * dht = DHTADDR           ;
  return QString::fromUtf8 ( dht -> Usage ) ;
}

bool N::DukascopyMaps::PrepareMaps(void)
{
  StreamHeader SH                                                            ;
  ::memcpy ( SH . data ( ) , memoryData , 64                               ) ;
  if ( ! SH . isValid ( ) )                                                  {
    SH . Default      (                                                    ) ;
    SH . setType      ( Types::CurrencyPair                                ) ;
    SH . setSignature ( SIGNATURE                                          ) ;
    SH . setUuid      ( 0                                                  ) ;
    SH . setChecksum  (                                                    ) ;
    ::memcpy          ( memoryData , SH . data ( ) , 64                    ) ;
  }                                                                          ;
  if ( ! SH . isSignature ( SIGNATURE ) ) return false                       ;
  ////////////////////////////////////////////////////////////////////////////
  DukascopyHeader         * dht = DHTADDR                                    ;
  DukascopyCurrencyStatus * dcs = DCSADDR                                    ;
  ////////////////////////////////////////////////////////////////////////////
  dht -> Flags |= 1                                                          ;
  if ( dht -> Total > 0 )                                                    {
    for (int i = 0 ; i < dht -> Total ; i++ )                                {
      QString   symbol                                                       ;
      QDateTime start                                                        ;
      symbol = QString::fromUtf8     ( dcs[i].Symbol                       ) ;
      start  = QDateTime::fromString ( dcs[i].Start,"yyyy/MM/dd-hh:mm:ss"  ) ;
      appendSymbol                   ( symbol                                ,
                                       start  . date ( )                     ,
                                       dcs[i] . Offset                     ) ;
      SymbolArray [ symbol ] = & dcs [ i ]                                   ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::DukascopyMaps::BrandNew(void)
{
  return ( TimeArray . count ( ) <= 0 ) ;
}

void N::DukascopyMaps::assignTo (
       void * data              ,
       qint64 size              ,
       QDate  start             ,
       QDate  end               )
{
  QTime Z ( 0 , 0 , 0 , 0                     ) ;
  setData ( data , size                       ) ;
  setTime ( "Start" , QDateTime ( start , Z ) ) ;
  setTime ( "End"   , QDateTime ( end   , Z ) ) ;
}

QStringList N::DukascopyMaps::TimeSymbols(void)
{
  return TimeArray . keys ( ) ;
}

bool N::DukascopyMaps::addSymbol(QString symbol,QDate start)
{
  if ( IsNull ( memoryData ) ) return false                                  ;
  DukascopyHeader         * dht = DHTADDR                                    ;
  DukascopyCurrencyStatus * dcs = DCSADDR                                    ;
  QDateTime xt = QDateTime ( start , QTime ( 0 , 0 , 0 , 0 ) )               ;
  QString   ds = DateTimes [ "Start" ] . toString ( "yyyy/MM/dd-hh:mm:ss"  ) ;
  qint64    et = DateTimes [ "Start" ] . secsTo ( DateTimes [ "End" ]      ) ;
  qint64    st = DateTimes [ "Start" ] . secsTo ( xt                       ) ;
  qint64    bs = TimeBits::sizeHint                                          (
                   DateTimes [ "Start" ]                                     ,
                   DateTimes [ "End"   ]                                     ,
                   3600                                                    ) ;
  qint64   pos = ( ( ( ( bs / 64 ) + 1 ) * 8 ) * dht -> Total )              ;
  TimeArray [ symbol ] . setParameters                                       (
                           DateTimes [ "Start" ]                             ,
                           xt                                                ,
                           DateTimes [ "End"   ]                             ,
                           3600                                            ) ;
  TimeArray   [ symbol ] . setBlob ( bs , DTBADDR ( pos )                  ) ;
  SymbolArray [ symbol ] = & dcs [ dht -> Total ]                            ;
  CopyToArray ( dcs [ dht -> Total ] . Symbol , 16 , symbol                ) ;
  CopyToArray ( dcs [ dht -> Total ] . Start  , 64 , ds                    ) ;
  dcs [ dht -> Total ] . Offset    = ( DTBOFFSET + pos )                     ;
  dcs [ dht -> Total ] . End       = et                                      ;
  dcs [ dht -> Total ] . Real      = st                                      ;
  dcs [ dht -> Total ] . Completed = st                                      ;
  dcs [ dht -> Total ] . Imported  = st                                      ;
  dcs [ dht -> Total ] . Final     = st                                      ;
  dht       -> Total               = TimeArray . count ( )                   ;
  return true                                                                ;
}

bool N::DukascopyMaps::appendSymbol(QString symbol,QDate start,qint64 offset)
{
  if ( IsNull ( memoryData ) ) return false                                  ;
  QDateTime xt = QDateTime ( start , QTime ( 0 , 0 , 0 , 0 ) )               ;
  qint64    bs = TimeBits::sizeHint                                          (
                   DateTimes [ "Start" ]                                     ,
                   DateTimes [ "End"   ]                                     ,
                   3600                                                    ) ;
  TimeArray [ symbol ] . setParameters                                       (
                           DateTimes [ "Start" ]                             ,
                           xt                                                ,
                           DateTimes [ "End"   ]                             ,
                           3600                                            ) ;
  TimeArray [ symbol ] . setBlob                                             (
    bs                                                                       ,
    (unsigned char *) ( ( (char *) memoryData ) + offset )                 ) ;
  return true                                                                ;
}
