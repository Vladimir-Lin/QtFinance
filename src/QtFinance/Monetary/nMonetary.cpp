#include <qtfinance.h>

N::Monetary:: Monetary(void)
{
}

N::Monetary::~Monetary(void)
{
}

int N::Monetary::count(void)
{
  return ISO . count ( ) ;
}

void N::Monetary::clear(void)
{
  ID      . clear ( ) ;
  ISO     . clear ( ) ;
  Uuids   . clear ( ) ;
  IdMap   . clear ( ) ;
  UuidMap . clear ( ) ;
  MapId   . clear ( ) ;
  MapUuid . clear ( ) ;
  UuidIso . clear ( ) ;
  IsoUuid . clear ( ) ;
  Names   . clear ( ) ;
}

QString N::Monetary::operator[](int index)
{
  if ( ! IdMap . contains ( index ) ) return "" ;
  return IdMap            [ index ]             ;
}

int N::Monetary::operator[](SUID index)
{
  if ( ! MapUuid . contains ( index ) ) return 0 ;
  return MapUuid            [ index ]            ;
}

int N::Monetary::operator[](QString name)
{
  if ( ! MapId . contains ( name ) ) return 0 ;
  return MapId            [ name ]            ;
}

QString N::Monetary::Name(int index)
{
  if ( ! Names . contains ( index ) ) return "" ;
  return Names            [ index ]             ;
}

QString N::Monetary::Pairname(int pair)
{
  QPair<int,int> P = DecodeId ( pair )   ;
  int f = P . first                      ;
  int t = P . second                     ;
  if ( ! ID . contains ( f ) ) return "" ;
  if ( ! ID . contains ( t ) ) return "" ;
  return IdMap [ f ] + "/" + IdMap [ t ] ;
}

int N::Monetary::SymbolId(QString symbol,int first,int second)
{
  int     sid = 0                               ;
  QString F   = symbol . mid ( first  , 3 )     ;
  QString S   = symbol . mid ( second , 3 )     ;
  int     f   = 0                               ;
  int     s   = 0                               ;
  if ( MapId . contains ( F ) ) f = MapId [ F ] ;
  if ( MapId . contains ( S ) ) s = MapId [ S ] ;
  nKickOut ( nEqual ( f , 0 ) , 0 )             ;
  nKickOut ( nEqual ( s , 0 ) , 0 )             ;
  return ComposeId ( f , s )                    ;
}

int N::Monetary::ComposeId(int first,int second)
{
  int f = first  % 1000       ;
  int s = second % 1000       ;
  return ( ( f * 1000 ) + s ) ;
}

QPair<int,int> N::Monetary::DecodeId(int PairId)
{
  QPair<int,int> P                 ;
  int f = ( PairId / 1000 ) % 1000 ;
  int s = ( PairId % 1000 )        ;
  P . first  = f                   ;
  P . second = s                   ;
  return P                         ;
}

bool N::Monetary::load(SqlConnection & SC,QString table)
{
  bool    success = false              ;
  QString Q                            ;
  clear ( )                            ;
  Q = SC . sql . SelectFrom            (
        "id,uuid,shortname"            ,
        table                          ,
        SC . OrderByAsc("shortname") ) ;
  SqlLoopNow ( SC , Q )                ;
    int     id    =  SC . Int    ( 0 ) ;
    SUID    suid  =  SC . Uuid   ( 1 ) ;
    QString name  =  SC . String ( 2 ) ;
    ID            << id                ;
    Uuids         << suid              ;
    ISO           << name              ;
    IdMap   [ id   ] =  name           ;
    UuidMap [ id   ] =  suid           ;
    MapId   [ name ] =  id             ;
    MapUuid [ suid ] =  id             ;
    UuidIso [ suid ] =  name           ;
    IsoUuid [ name ] =  suid           ;
    success          = true            ;
  SqlLoopErr ( SC , Q )                ;
  SqlLoopEnd ( SC , Q )                ;
  return success                       ;
}

bool N::Monetary::loadName(SqlConnection & SC,QString table,int LanguageId)
{
  QString N                 ;
  int     id                ;
  Names . clear ( )         ;
  foreach ( id , ID )       {
    SUID u = UuidMap [ id ] ;
    N = SC . getName        (
          table             ,
          "uuid"            ,
          LanguageId        ,
          u               ) ;
    Names [ id ] = N        ;
  }                         ;
  return true               ;
}

bool N::Monetary::loadPairs(SqlConnection & SC,QString table)
{
  PairNames . clear ( )                 ;
  PairUuid  . clear ( )                 ;
  UuidPair  . clear ( )                 ;
  Pairs     . clear ( )                 ;
  QString Q                             ;
  ///////////////////////////////////////
  Q = SC . sql . SelectFrom             (
        "uuid,fromcurrency,tocurrency"  ,
        table                           ,
        SC . OrderByAsc ( "id" )      ) ;
  SqlLoopNow ( SC , Q )                 ;
    SUID suid = SC . Uuid ( 0         ) ;
    int  fid  = SC . Int  ( 1         ) ;
    int  tid  = SC . Int  ( 2         ) ;
    int  pid  = ComposeId ( fid , tid ) ;
    Pairs             << pid            ;
    PairUuid [ pid  ]  = suid           ;
    UuidPair [ suid ]  = pid            ;
  SqlLoopErr ( SC , Q )                 ;
  SqlLoopEnd ( SC , Q )                 ;
  ///////////////////////////////////////
  return true                           ;
}

bool N::Monetary::ToCpp(QByteArray & d)
{
  d . append ( "typedef struct {\n"
               "  int                id   ;\n"
               "  unsigned long long uuid ;\n"
               "  const char       * name ;\n"
               "} private_currency;\n\n"                                   ) ;
  d . append ( "private_currency DefaultCurrency [] = {\n"                 ) ;
  for (int i=0;i<ID.count();i++)                                             {
    int     id = ID    [ i ]                                                 ;
    SUID    u  = Uuids [ i ]                                                 ;
    QString n  = ISO   [ i ]                                                 ;
    QString t                                                                ;
    t = QString("  {%1,%2LL,\"%3\"} ,\n").arg(id).arg(u).arg(n)              ;
    d . append ( t . toUtf8 ( ) )                                            ;
  }                                                                          ;
  d . append ( "{  0,                0LL ,\"\"   }\n\n"                    ) ;
  d . append ( "};\n\n"                                                    ) ;
  d . append ( "typedef struct              {\n"
               "  const char * fromCurrency ;\n"
               "  const char * toCurrency   ;\n"
               "} private_currencyexchange  ;\n\n"                         ) ;
  d . append ( "private_currencyexchange DefaultCurrencyExchange [] = {\n" ) ;
  for (int i = 0 ; i < Pairs . count ( ) ; i++ )                             {
    int            pair = Pairs    [ i    ]                                  ;
    QPair<int,int> P    = DecodeId ( pair )                                  ;
    QString        f    = IdMap [ P . first  ]                               ;
    QString        s    = IdMap [ P . second ]                               ;
    QString        t                                                         ;
    t = QString("  {\"%1\",\"%2\"} ,\n").arg(f).arg(s)                       ;
    d . append ( t . toUtf8 ( ) )                                            ;
  }                                                                          ;
  d . append ( "{ 0   , 0   }\n\n"                                         ) ;
  d . append ( "};\n\n"                                                    ) ;
  return true                                                                ;
}

// five parts in memory
// Stream Header Signature => For identification
// Header
//  => ISO 4217 Currency definition count
//  => ISO 4217 Currency table offset
//  => Currency pairs count
//  => Currency pairs table offset
//  => Names count
//  => Names table offset
// ISO 4217 table       => id uuid shortname
// Currency Pairs table => uuid first second
// Names table          => uuid length string

#pragma pack(push,1)

// Monetary version of StreamHeader
typedef struct                 { // 64 bytes
  char          CIOS[4]        ; // 04
  qint32        Type           ; // 08
  quint64       Uuid           ; // 16
  char          Signature [12] ; // 28
  quint32       Flags          ; // 32
  unsigned char Data      [28] ; // 60
  quint32       Adler          ; // 64
}               MonetaryHeader ;

typedef struct                 { // 256 bytes
  qint32        Block          ; // Memory block size
  qint32        Actual         ; // Actual used size
  qint32        Iso            ; // ISO 4217 Currency definition count
  qint32        Pairs          ; // Currency pairs count
  qint32        Names          ; // Names count
  qint32        IsoOffset      ; // ISO 4217 Currency table offset
  qint32        PairOffset     ; // Currency Pairs table Offset
  qint32        NameOffset     ; // Names table Offset
  char          Buffer [ 224 ] ; // Not used
}               HeaderEntry    ;

typedef struct                 { // 32 bytes
  qint32        id             ; // ISO ID
  quint64       uuid           ; // ISO uuid
  char          name   [  20 ] ; // ISO short name
}               IsoEntry       ;

typedef struct                 { // 16 bytes
  quint64       uuid           ; // Currency pair uuid
  qint32        first          ; // First ISO ID
  qint32        second         ; // Second ISO ID
}               PairEntry      ;

typedef struct                 {
  quint64       uuid           ; // string uuid
  qint32        length         ; // string length
  char          name [ 1 ]     ; // string body
}               NameEntry      ;

#pragma pack(pop)

#define SIGNATURE "Monetary"

bool N::Monetary::FromByteArray(QByteArray & data)
{
  if ( data . size ( ) < sizeof(MonetaryHeader) ) return false               ;
  StreamHeader SH                                                            ;
  ::memcpy ( SH . data ( ) , data . data ( ) , sizeof(MonetaryHeader)      ) ;
  if ( ! SH . isValid     (           ) ) return false                       ;
  if ( ! SH . isSignature ( SIGNATURE ) ) return false                       ;
  ////////////////////////////////////////////////////////////////////////////
  int pos = sizeof(MonetaryHeader)                                           ;
  if ( data . size ( ) < ( pos + sizeof(HeaderEntry) ) ) return false        ;
  MonetaryHeader * mh  = (MonetaryHeader *)   data . data ( )                ;
  HeaderEntry    * he  = (HeaderEntry    *) ( data . data ( ) + pos )        ;
  if ( mh -> Flags > 0 ) return false                                        ;
  mh -> Flags |= 1                                                           ;
  clear ( )                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  pos = he -> IsoOffset                                                      ;
  for (int i = 0 ; i < he -> Iso ; i++ )                                     {
    int endp = pos + sizeof(IsoEntry)                                        ;
    if ( data . size ( ) < endp ) return false                               ;
    IsoEntry * ie      = (IsoEntry *) ( data . data ( ) + pos )              ;
    int        id      = ie -> id                                            ;
    SUID       uuid    = ie -> uuid                                          ;
    QString    name    = QString::fromUtf8 ( ie -> name )                    ;
    ID                << id                                                  ;
    Uuids             << uuid                                                ;
    ISO               << name                                                ;
    IdMap   [ id   ]   = name                                                ;
    UuidMap [ id   ]   = uuid                                                ;
    MapId   [ name ]   = id                                                  ;
    MapUuid [ uuid ]   = id                                                  ;
    UuidIso [ uuid ]   = name                                                ;
    IsoUuid [ name ]   = uuid                                                ;
    pos                = endp                                                ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  pos = he -> PairOffset                                                     ;
  for (int i = 0 ; i < he -> Pairs ; i++ )                                   {
    int endp = pos + sizeof(PairEntry)                                       ;
    if ( data . size ( ) < endp ) return false                               ;
    PairEntry * pe     = (PairEntry *) ( data . data ( ) + pos )             ;
    SUID        uuid   = pe -> uuid                                          ;
    int         first  = pe -> first                                         ;
    int         second = pe -> second                                        ;
    int         pair   = ComposeId ( first , second )                        ;
    Pairs             << pair                                                ;
    PairUuid [ pair ]  = uuid                                                ;
    UuidPair [ uuid ]  = pair                                                ;
    pos                = endp                                                ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  NAMEs n                                                                    ;
  pos = he -> NameOffset                                                     ;
  for (int i = 0 ; i < he -> Names ; i++ )                                   {
    int endp = pos + sizeof(quint64) + sizeof(qint32)                        ;
    if ( data . size ( ) < endp ) return false                               ;
    NameEntry * pe     = (NameEntry *) ( data . data ( ) + pos )             ;
    if ( data . size ( ) <  ( endp + pe -> length ) ) return false           ;
    n [ pe -> uuid ]   = QString::fromUtf8( data . data ( ) + endp )         ;
    pos                = endp + pe -> length                                 ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int  id                                                                    ;
  SUID u                                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  foreach ( id , ID    )                                                     {
    u = MapUuid [ id ]                                                       ;
    if ( n . contains ( u ) ) Names     [ id ] = n [ u ]                     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  foreach ( id , Pairs )                                                     {
    u = PairUuid [ id ]                                                      ;
    if ( n . contains ( u ) ) PairNames [ id ] = n [ u ]                     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  mh -> Flags  = 0                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::Monetary::FromByteArray(void * pointer,int size)
{
  char * data = (char *) pointer                                             ;
  if ( size < sizeof(MonetaryHeader) ) return false                          ;
  StreamHeader SH                                                            ;
  ::memcpy ( SH . data ( ) , data , sizeof(MonetaryHeader)                 ) ;
  if ( ! SH . isValid     (           ) ) return false                       ;
  if ( ! SH . isSignature ( SIGNATURE ) ) return false                       ;
  ////////////////////////////////////////////////////////////////////////////
  int pos = sizeof(MonetaryHeader)                                           ;
  if ( size < ( pos + sizeof(HeaderEntry) ) ) return false                   ;
  MonetaryHeader * mh  = (MonetaryHeader *)   data                           ;
  HeaderEntry    * he  = (HeaderEntry    *) ( data + pos )                   ;
  if ( mh -> Flags > 0 ) return false                                        ;
  mh -> Flags |= 1                                                           ;
  clear ( )                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  pos = he -> IsoOffset                                                      ;
  for (int i = 0 ; i < he -> Iso ; i++ )                                     {
    int endp = pos + sizeof(IsoEntry)                                        ;
    if ( size < endp ) return false                                          ;
    IsoEntry * ie      = (IsoEntry *) ( data + pos )                         ;
    int        id      = ie -> id                                            ;
    SUID       uuid    = ie -> uuid                                          ;
    QString    name    = QString::fromUtf8 ( ie -> name )                    ;
    ID                << id                                                  ;
    Uuids             << uuid                                                ;
    ISO               << name                                                ;
    IdMap   [ id   ]   = name                                                ;
    UuidMap [ id   ]   = uuid                                                ;
    MapId   [ name ]   = id                                                  ;
    MapUuid [ uuid ]   = id                                                  ;
    UuidIso [ uuid ]   = name                                                ;
    IsoUuid [ name ]   = uuid                                                ;
    pos                = endp                                                ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  pos = he -> PairOffset                                                     ;
  for (int i = 0 ; i < he -> Pairs ; i++ )                                   {
    int endp = pos + sizeof(PairEntry)                                       ;
    if ( size < endp ) return false                                          ;
    PairEntry * pe     = (PairEntry *) ( data + pos )                        ;
    SUID        uuid   = pe -> uuid                                          ;
    int         first  = pe -> first                                         ;
    int         second = pe -> second                                        ;
    int         pair   = ComposeId ( first , second )                        ;
    Pairs             << pair                                                ;
    PairUuid [ pair ]  = uuid                                                ;
    UuidPair [ uuid ]  = pair                                                ;
    pos                = endp                                                ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  NAMEs n                                                                    ;
  pos = he -> NameOffset                                                     ;
  for (int i = 0 ; i < he -> Names ; i++ )                                   {
    int endp = pos + sizeof(quint64) + sizeof(qint32)                        ;
    if ( size <   endp                  ) return false                       ;
    NameEntry * pe     = (NameEntry *) ( data + pos )                        ;
    if ( size < ( endp + pe -> length ) ) return false                       ;
    n [ pe -> uuid ]   = QString::fromUtf8( data + endp )                    ;
    pos                = endp + pe -> length                                 ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int  id                                                                    ;
  SUID u                                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  foreach ( id , ID    )                                                     {
    u = MapUuid [ id ]                                                       ;
    if ( n . contains ( u ) ) Names     [ id ] = n [ u ]                     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  foreach ( id , Pairs )                                                     {
    u = PairUuid [ id ]                                                      ;
    if ( n . contains ( u ) ) PairNames [ id ] = n [ u ]                     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  mh -> Flags  = 0                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::Monetary::ToByteArray(QByteArray & data,bool resize)
{
  if ( ! resize )                                                            {
    if ( data . size ( ) < sizeof(MonetaryHeader) ) return false             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( resize )                                                              {
    if ( data . size ( ) < sizeof(MonetaryHeader) )                          {
      data . resize ( sizeof(MonetaryHeader) )                               ;
      data . fill   ( 0                      )                               ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  StreamHeader SH                                                            ;
  ::memcpy ( SH . data ( ) , data . data ( ) , sizeof(MonetaryHeader)      ) ;
  if ( ! SH . isValid ( ) )                                                  {
    SH . Default      (                 )                                    ;
    SH . setType      ( Types::Currency )                                    ;
    SH . setSignature ( SIGNATURE       )                                    ;
    SH . setUuid      ( 0               )                                    ;
    SH . setChecksum  (                 )                                    ;
    ::memcpy ( data . data ( ) , SH . data ( ) , sizeof(MonetaryHeader)    ) ;
  }                                                                          ;
  if ( ! SH . isSignature ( SIGNATURE ) ) return false                       ;
  ////////////////////////////////////////////////////////////////////////////
  int pos = sizeof(MonetaryHeader)                                           ;
  ////////////////////////////////////////////////////////////////////////////
  if ( resize && ( data . size ( ) < ( pos + sizeof(MonetaryHeader) ) ) )    {
    data . resize ( pos + sizeof(MonetaryHeader) )                           ;
    ::memset ( data . data ( ) , pos , sizeof(HeaderEntry) )                 ;
  }                                                                          ;
  if ( data . size ( ) < ( pos + sizeof(MonetaryHeader) ) ) return false     ;
  ////////////////////////////////////////////////////////////////////////////
  MonetaryHeader * mh = (MonetaryHeader *)   data . data ( )                 ;
  HeaderEntry    * he = (HeaderEntry    *) ( data . data ( ) + pos )         ;
  int              total                                                     ;
  if ( mh -> Flags > 0 ) return false                                        ;
  mh -> Flags |= 2                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  pos             += sizeof(HeaderEntry)                                     ;
  he -> IsoOffset  = pos                                                     ;
  he -> Iso        = ID        . count ( )                                   ;
  total            = ID        . count ( )                                   ;
  for (int i = 0 ; i < total ; i++ )                                         {
    if ( resize && ( data . size ( ) < ( pos + sizeof(IsoEntry) ) ) )        {
      data . resize ( pos + sizeof(IsoEntry) )                               ;
    }                                                                        ;
    if ( data . size ( ) < ( pos + sizeof(IsoEntry) ) ) return false         ;
    IsoEntry IE                                                              ;
    ::memset ( &IE , 0 , sizeof(IsoEntry) )                                  ;
    IE . id   = ID    [ i ]                                                  ;
    IE . uuid = Uuids [ i ]                                                  ;
    CopyToArray ( IE . name , 20 , ISO [ i ] )                               ;
    ::memcpy(  ( data . data ( ) + pos ) , &IE , sizeof(IsoEntry) )          ;
    pos += sizeof(IsoEntry)                                                  ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> PairOffset  = pos                                                    ;
  he -> Pairs       = Pairs     . count ( )                                  ;
  total             = Pairs     . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    if ( resize && ( data . size ( ) < ( pos + sizeof(PairEntry) ) ) )       {
      data . resize ( pos + sizeof(PairEntry) )                              ;
    }                                                                        ;
    if ( data . size ( ) < ( pos + sizeof(PairEntry) ) ) return false        ;
    PairEntry      PE                                                        ;
    int            pair = Pairs [ i ]                                        ;
    QPair<int,int> P    = DecodeId ( pair )                                  ;
    PE . uuid       = PairUuid [ pair ]                                      ;
    PE . first      = P . first                                              ;
    PE . second     = P . second                                             ;
    ::memcpy(  ( data . data ( ) + pos ) , &PE , sizeof(PairEntry) )         ;
    pos += sizeof(PairEntry)                                                 ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> NameOffset  = pos                                                    ;
  he -> Names       = PairNames . count ( ) + Names . count ( )              ;
  total             = Names     . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    int        id   = ID        [ i    ]                                     ;
    SUID       uuid = Uuids     [ i    ]                                     ;
    QString    name = Names     [ id   ]                                     ;
    QByteArray B    = name . toUtf8 ( )                                      ;
    int        len  = sizeof(quint64) + sizeof(qint32) + B . size ( )        ;
    if ( resize && ( data . size ( ) < ( pos + len ) ) )                     {
      data . resize ( pos + len )                                            ;
    }                                                                        ;
    if ( data . size ( ) < ( pos + len ) ) return false                      ;
    NameEntry * ne  = (NameEntry *) ( data . data ( ) + pos )                ;
    ne  -> uuid     = uuid                                                   ;
    ne  -> length   = B . size ( )                                           ;
    ::memcpy ( ne -> name , B . data ( ) , B . size ( ) )                    ;
    pos          += len                                                      ;
  }                                                                          ;
  total             = PairNames . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    int        pair = Pairs     [ i    ]                                     ;
    SUID       uuid = PairUuid  [ pair ]                                     ;
    QString    name = PairNames [ pair ]                                     ;
    QByteArray B    = name . toUtf8 ( )                                      ;
    int        len  = sizeof(quint64) + sizeof(qint32) + B . size ( )        ;
    if ( resize && ( data . size ( ) < ( pos + len ) ) )                     {
      data . resize ( pos + len )                                            ;
    }                                                                        ;
    if ( data . size ( ) < ( pos + len ) ) return false                      ;
    NameEntry * ne  = (NameEntry *) ( data . data ( ) + pos )                ;
    ne  -> uuid     = uuid                                                   ;
    ne  -> length   = B . size ( )                                           ;
    ::memcpy ( ne -> name , B . data ( ) , B . size ( ) )                    ;
    pos          += len                                                      ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> Block  = data . size ( )                                             ;
  he -> Actual = pos                                                         ;
  mh -> Flags  = 0                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::Monetary::ToByteArray(void * pointer,int size)
{
  char * data = (char *) pointer                                             ;
  if ( size < sizeof(MonetaryHeader) ) return false                          ;
  ////////////////////////////////////////////////////////////////////////////
  StreamHeader SH                                                            ;
  ::memcpy ( SH . data ( ) , data , sizeof(MonetaryHeader)                 ) ;
  if ( ! SH . isValid ( ) )                                                  {
    SH . Default      (                 )                                    ;
    SH . setType      ( Types::Currency )                                    ;
    SH . setSignature ( SIGNATURE       )                                    ;
    SH . setUuid      ( 0               )                                    ;
    SH . setChecksum  (                 )                                    ;
    ::memcpy ( data , SH . data ( ) , sizeof(MonetaryHeader)               ) ;
  }                                                                          ;
  if ( ! SH . isSignature ( SIGNATURE ) ) return false                       ;
  ////////////////////////////////////////////////////////////////////////////
  int pos = sizeof(MonetaryHeader)                                           ;
  ////////////////////////////////////////////////////////////////////////////
  if ( size < ( pos + sizeof(MonetaryHeader) ) ) return false                ;
  ////////////////////////////////////////////////////////////////////////////
  MonetaryHeader * mh = (MonetaryHeader *)   data                            ;
  HeaderEntry    * he = (HeaderEntry    *) ( data + pos )                    ;
  int              total                                                     ;
  if ( mh -> Flags > 0 ) return false                                        ;
  mh -> Flags |= 2                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  pos             += sizeof(HeaderEntry)                                     ;
  he -> IsoOffset  = pos                                                     ;
  he -> Iso        = ID        . count ( )                                   ;
  total            = ID        . count ( )                                   ;
  for (int i = 0 ; i < total ; i++ )                                         {
    if ( size < ( pos + sizeof(IsoEntry) ) ) return false                    ;
    IsoEntry IE                                                              ;
    ::memset ( &IE , 0 , sizeof(IsoEntry) )                                  ;
    IE . id   = ID    [ i ]                                                  ;
    IE . uuid = Uuids [ i ]                                                  ;
    CopyToArray ( IE . name , 20 , ISO [ i ] )                               ;
    ::memcpy(  ( data + pos ) , &IE , sizeof(IsoEntry) )                     ;
    pos += sizeof(IsoEntry)                                                  ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> PairOffset  = pos                                                    ;
  he -> Pairs       = Pairs     . count ( )                                  ;
  total             = Pairs     . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    if ( size < ( pos + sizeof(PairEntry) ) ) return false                   ;
    PairEntry      PE                                                        ;
    int            pair = Pairs [ i ]                                        ;
    QPair<int,int> P    = DecodeId ( pair )                                  ;
    PE . uuid       = PairUuid [ pair ]                                      ;
    PE . first      = P . first                                              ;
    PE . second     = P . second                                             ;
    ::memcpy(  ( data + pos ) , &PE , sizeof(PairEntry) )                    ;
    pos += sizeof(PairEntry)                                                 ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> NameOffset  = pos                                                    ;
  he -> Names       = PairNames . count ( ) + Names . count ( )              ;
  total             = Names     . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    int        id   = ID        [ i    ]                                     ;
    SUID       uuid = Uuids     [ i    ]                                     ;
    QString    name = Names     [ id   ]                                     ;
    QByteArray B    = name . toUtf8 ( )                                      ;
    int        len  = sizeof(quint64) + sizeof(qint32) + B . size ( )        ;
    if ( size < ( pos + len ) ) return false                                 ;
    NameEntry * ne  = (NameEntry *) ( data + pos )                           ;
    ne  -> uuid     = uuid                                                   ;
    ne  -> length   = B . size ( )                                           ;
    ::memcpy ( ne -> name , B . data ( ) , B . size ( ) )                    ;
    pos          += len                                                      ;
  }                                                                          ;
  total             = PairNames . count ( )                                  ;
  for (int i = 0 ; i < total ; i++ )                                         {
    int        pair = Pairs     [ i    ]                                     ;
    SUID       uuid = PairUuid  [ pair ]                                     ;
    QString    name = PairNames [ pair ]                                     ;
    QByteArray B    = name . toUtf8 ( )                                      ;
    int        len  = sizeof(quint64) + sizeof(qint32) + B . size ( )        ;
    if ( size < ( pos + len ) ) return false                                 ;
    NameEntry * ne  = (NameEntry *) ( data + pos )                           ;
    ne  -> uuid     = uuid                                                   ;
    ne  -> length   = B . size ( )                                           ;
    ::memcpy ( ne -> name , B . data ( ) , B . size ( ) )                    ;
    pos          += len                                                      ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  he -> Block  = size                                                        ;
  he -> Actual = pos                                                         ;
  mh -> Flags  = 0                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::Monetary::canRead(QByteArray & data)
{
  if ( data . size ( ) < sizeof(MonetaryHeader) ) return false ;
  MonetaryHeader * mh = (MonetaryHeader *) data . data ( )     ;
  return ( 0 == ( mh -> Flags & 1 ) )                          ;
}

bool N::Monetary::canWrite(QByteArray & data)
{
  if ( data . size ( ) < sizeof(MonetaryHeader) ) return false ;
  MonetaryHeader * mh = (MonetaryHeader *) data . data ( )     ;
  return ( 0 == ( mh -> Flags & 2 ) )                          ;
}
