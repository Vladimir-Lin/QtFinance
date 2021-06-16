#include <qtfinance.h>

#pragma pack(push,1)

typedef struct                  {
  qint32 Keys                   ;
  qint32 Providers              ;
  qint32 Tradings               ;
  qint32 Connections            ;
  int    TradingsArray [ 1024 ] ;
} ForexShareHeader              ;

#pragma pack(pop)

#define HEADERSIZE  ( 16 * 1024 * 1024 )
#define HEADER(id)  ForexShareHeader * id = ( (ForexShareHeader *) DataManipulator::Memory ( Master ) )

#define COMMSIZE    ( 64 * 1024 * 1024 )
#define COMMSTR     "Communication"

#define VENDORSIZE  (      1024 * 1024 )
#define VENDORS     "Providers"

#define SQLCSIZE    (        32 * 1024 )
#define SQLCSTR     "SqlConnections"

#define ISO4217SIZE (        32 * 1024 )
#define ISO4217STR  "Forex[ISO4217]"

#define DUKASSIZE   (  4 * 1024 * 1024 )
#define DUKASSTR    "Forex[Dukascopy]"

N::ForexManipulator:: ForexManipulator ( QString master )
                    : DataManipulator  (                )
{
  setEnabled ( "FOREX"    , true        ) ;
  setEnabled ( "Create"   , true        ) ;
  Manager    ( master     , HEADERSIZE  ) ;
  Mount      ( COMMSTR    , COMMSIZE    ) ;
  Mount      ( VENDORS    , VENDORSIZE  ) ;
  Mount      ( SQLCSTR    , SQLCSIZE    ) ;
  Mount      ( ISO4217STR , ISO4217SIZE ) ;
  Mount      ( DUKASSTR   , DUKASSIZE   ) ;
  AsStrings  ( SQLCSTR                  ) ;
}

N::ForexManipulator::~ForexManipulator (void)
{
}

int N::ForexManipulator::Providers(void)
{
  HEADER ( FSH )              ;
  if ( NULL == FSH ) return 0 ;
  return FSH -> Providers     ;
}

int N::ForexManipulator::setProviders(SUID division,UUIDs & U)
{
  HEADER ( FSH )                                         ;
  if ( NULL == FSH ) return 0                            ;
  void * providers = DataManipulator::Memory ( VENDORS ) ;
  if ( NULL != providers )                               {
    StreamHeader B                                       ;
    QByteArray   S                                       ;
    QByteArray   L                                       ;
    qint64     * v                                       ;
    qint64       s                                       ;
    B    . setType             ( Types::Organization   ) ;
    B    . setSignature        ( "ForexManipulator"    ) ;
    B    . setUuid             ( division              ) ;
    L    = toByteArray         ( U                     ) ;
    S    . resize ( sizeof(qint64)                     ) ;
    v    = (qint64 *) S . data (                       ) ;
    (*v) = (qint64  ) L . size (                       ) ;
    B    . setStreamInfo       ( S                     ) ;
    B    . setChecksum         (                       ) ;
    B    . append              ( L                     ) ;
    s    = (qint64  ) B . size (                       ) ;
    ::memcpy ( providers , B . data ( ) , s )            ;
    FSH -> Providers = U . count ( )                     ;
  }                                                      ;
  return FSH -> Providers                                ;
}

bool N::ForexManipulator::Providers(UUIDs & uuids)
{
  HEADER ( FSH )                                                    ;
  if ( NULL == FSH ) return false                                   ;
  void * providers = DataManipulator::Memory ( VENDORS )            ;
  uuids . clear ( )                                                 ;
  if ( ( NULL != providers ) && ( FSH -> Providers > 0 ) )          {
    StreamHeader B                                                  ;
    ::memcpy ( B . data ( ) , providers , B . size ( ) )            ;
    if ( B . isValid ( ) )                                          {
      int    t = FSH -> Providers                                   ;
      SUID * u = (SUID *) ( ( (char *) providers ) + B . size ( ) ) ;
      for (int i=0;i<t;i++) uuids << u [ i ]                        ;
    }                                                               ;
  }                                                                 ;
  return ( uuids . count ( ) > 0 )                                  ;
}

int N::ForexManipulator::Tradings(void)
{
  HEADER ( FSH )         ;
  return FSH -> Tradings ;
}

int N::ForexManipulator::setTradings(int tradings)
{
  HEADER ( FSH )             ;
  FSH -> Tradings = tradings ;
  return FSH -> Tradings     ;
}

int * N::ForexManipulator::TradingsArray(void)
{
  HEADER ( FSH )              ;
  return FSH -> TradingsArray ;
}

int N::ForexManipulator::Connections(void)
{
  HEADER ( FSH )            ;
  return FSH -> Connections ;
}

bool N::ForexManipulator::GetDBs(QStringList & DBs)
{
  N::Streaming::Strings s                      ;
  if ( ! Obtain ( SQLCSTR , s ) ) return false ;
  if ( ! s . isSignature ( )    ) return false ;
  return s . toStrings ( DBs )                 ;
}

bool N::ForexManipulator::SetDBs(QStringList & DBs)
{
  N::Streaming::Strings s                      ;
  if ( ! Obtain ( SQLCSTR , s ) ) return false ;
  if ( ! s . isSignature ( )    ) return false ;
  return s . toByteArray ( DBs )               ;
}
