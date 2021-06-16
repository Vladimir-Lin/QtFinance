#include <qtfinance.h>

int TickDTS [ 32 ] =                        {
  0 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,  1 ,
       9 ,  9 ,  9 ,  9 ,  9 ,  9 ,  9 ,  9 ,
      17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 ,
      25 , 25 , 25 , 25 , 25 , 25 , 25    } ;

//////////////////////////////////////////////////////////////////////////////

void N::Forex::toMiddleTicks(int items,unsigned char * array,int gaps,void * funcdata)
{
  if ( NULL == array ) return                        ;
  TickUnion * tu = (TickUnion *) array               ;
  Math::IntegerMiddleArray                           (
    items                                            ,
    (unsigned char *) & ( tu [ 0 ] . Integer . bid ) ,
    gaps                                             ,
    funcdata                                       ) ;
}

void N::Forex::toDoubleTicks(int items,unsigned char * array,int gaps,void * funcdata)
{
  unsigned char * p = array                                 ;
  double        * d = (double *) funcdata                   ;
  double          z = d [ 0 ]                               ;
  double          w = d [ 1 ]                               ;
  #define v ( (TickUnion *) p )
  while ( items > 0 )                                       {
    v     -> Double . bid        = v -> Integer . bid       ;
    v     -> Double . ask        = v -> Integer . ask       ;
    v     -> Double . bidvolume  = v -> Integer . bidvolume ;
    v     -> Double . askvolume  = v -> Integer . askvolume ;
    v     -> Double . mid        = ( v -> Double . bid + v -> Double . ask ) / 2 ;
    v     -> Double . bid       /= z                        ;
    v     -> Double . mid       /= z                        ;
    v     -> Double . ask       /= z                        ;
    v     -> Double . bidvolume /= w                        ;
    v     -> Double . askvolume /= w                        ;
    p     += gaps                                           ;
    items --                                                ;
  }                                                         ;
  #undef  v
}

void N::Forex::toFloatTicks(int items,unsigned char * array,int gaps,void * funcdata)
{
  unsigned char * p = array                                ;
  float         * d = (float *) funcdata                   ;
  float           z = d [ 0 ]                              ;
  float           w = d [ 1 ]                              ;
  #define v ( (TickUnion *) p )
  while ( items > 0 )                                      {
    v     -> Float . bid        = v -> Integer . bid       ;
    v     -> Float . ask        = v -> Integer . ask       ;
    v     -> Float . bidvolume  = v -> Integer . bidvolume ;
    v     -> Float . askvolume  = v -> Integer . askvolume ;
    v     -> Float . mid        = ( v -> Float . bid + v -> Float . ask ) / 2 ;
    v     -> Float . bid       /= z                        ;
    v     -> Float . mid       /= z                        ;
    v     -> Float . ask       /= z                        ;
    v     -> Float . bidvolume /= w                        ;
    v     -> Float . askvolume /= w                        ;
    p     += gaps                                          ;
    items --                                               ;
  }                                                        ;
  #undef  v
}

void N::Forex::toMiddlePeriods(int items,unsigned char * array,int gaps,void * funcdata)
{
  if ( NULL == array ) return                         ;
  PeriodUnion * pu = (PeriodUnion *) array            ;
  Math::IntegerMiddleArray                            (
    items                                             ,
    (unsigned char *) & ( pu [ 0 ] . Integer . high ) ,
    gaps                                              ,
    funcdata                                        ) ;
}

void N::Forex::toDoublePeriods(int items,unsigned char * array,int gaps,void * funcdata)
{
  unsigned char * p = array                           ;
  double        * d = (double *) funcdata             ;
  double          z = d [ 0 ]                         ;
  double          w = d [ 1 ]                         ;
  #define v ( (PeriodUnion *) p )
  while ( items > 0 )                                 {
    v     -> Double . open    = v -> Integer . open   ;
    v     -> Double . high    = v -> Integer . high   ;
    v     -> Double . low     = v -> Integer . low    ;
    v     -> Double . close   = v -> Integer . close  ;
    v     -> Double . volume  = v -> Integer . volume ;
    v     -> Double . mid     = ( v -> Double . high + v -> Double . low ) / 2 ;
    v     -> Double . open   /= z                     ;
    v     -> Double . high   /= z                     ;
    v     -> Double . mid    /= z                     ;
    v     -> Double . low    /= z                     ;
    v     -> Double . close  /= z                     ;
    v     -> Double . volume /= w                     ;
    p     += gaps                                     ;
    items --                                          ;
  }                                                   ;
  #undef  v
}

void N::Forex::toFloatPeriods(int items,unsigned char * array,int gaps,void * funcdata)
{
  unsigned char * p = array                          ;
  float         * d = (float *) funcdata             ;
  float           z = d [ 0 ]                        ;
  float           w = d [ 1 ]                        ;
  #define v ( (PeriodUnion *) p )
  while ( items > 0 )                                {
    v     -> Float . open    = v -> Integer . open   ;
    v     -> Float . high    = v -> Integer . high   ;
    v     -> Float . low     = v -> Integer . low    ;
    v     -> Float . close   = v -> Integer . close  ;
    v     -> Float . volume  = v -> Integer . volume ;
    v     -> Float . mid     = ( v -> Float . high + v -> Float . low ) / 2 ;
    v     -> Float . open   /= z                     ;
    v     -> Float . high   /= z                     ;
    v     -> Float . mid    /= z                     ;
    v     -> Float . low    /= z                     ;
    v     -> Float . close  /= z                     ;
    v     -> Float . volume /= w                     ;
    p     += gaps                                    ;
    items --                                         ;
  }                                                  ;
  #undef  v
}

qint64 N::Forex::ToReversalTicks (
         qint64      ticks       ,
         TickUnion * tu          ,
         TickUnion * pu          ,
         qint64      full        )
{
  if ( ticks <= 0 ) return 0                                        ;
  for (qint64 i = 0 ; i < ticks ; i++ )                             {
    qint64 bid  = full                                              ;
    qint64 ask  = full                                              ;
    int    iask = tu [ i ] . Integer . ask                          ;
    int    ibid = tu [ i ] . Integer . bid                          ;
    if ( ( iask > 0 ) && ( ibid > 0 ) )                             {
      bid                         /= tu [ i ] . Integer . ask       ;
      ask                         /= tu [ i ] . Integer . bid       ;
    } else                                                          {
      bid                          = 0                              ;
      ask                          = 0                              ;
    }                                                               ;
    pu [ i ] . id                  = tu [ i ] . id                  ;
    pu [ i ] . at                  = tu [ i ] . at                  ;
    pu [ i ] . Integer . ask       = (int) ask                      ;
    pu [ i ] . Integer . bid       = (int) bid                      ;
    pu [ i ] . Integer . askvolume = tu [ i ] . Integer . bidvolume ;
    pu [ i ] . Integer . bidvolume = tu [ i ] . Integer . askvolume ;
  }                                                                 ;
  return ticks                                                      ;
}

//////////////////////////////////////////////////////////////////////////////

N::Forex::Equations:: Equations   ( void        )
                    : Name        ( ""          )
                    , Supplier    ( 0           )
                    , SQL         ( 0           )
                    , Pair        ( 0           )
                    , Id          ( -1          )
                    , Period      ( 10          )
                    , Digits      ( 5           )
                    , Full        ( 10          )
                    , Multiply    ( 100000      )
                    , Reversal    ( 100000      )
                    , FullDigits  ( 10000000000 )
                    , TimeZone    ( ""          )
                    , Level       ( 3           )
                    , FromYear    ( 1990        )
                    , ToYear      ( 2030        )
                    , identifier  ( ""          )
                    , level       ( "D"         )
                    , symbol      ( ""          )
                    , period      ( ""          )
                    , base        ( ""          )
                    , method      ( "insert"    )
                    , direction   ( false       )
                    , QuotePeriod ( 0           )
{
  periods [ Forex::Ticks ] = "T00" ;
  periods [ Forex::M1    ] = "M01" ;
  periods [ Forex::M5    ] = "M05" ;
  periods [ Forex::M15   ] = "M15" ;
  periods [ Forex::M30   ] = "M30" ;
  periods [ Forex::H1    ] = "H01" ;
  periods [ Forex::H4    ] = "H04" ;
  periods [ Forex::D1    ] = "D01" ;
  periods [ Forex::W1    ] = "W01" ;
  periods [ Forex::MN1   ] = "MN1" ;
}

N::Forex::Equations::~Equations (void)
{
}

QString N::Forex::Equations::TickColumns(void)
{
  return "`id`,`quotetime`,`bidprice`,`askprice`,`bidvolume`,`askvolume`" ;
}

QString N::Forex::Equations::PeriodColumns(void)
{
  return "`id`,`quote`,`open`,`high`,`low`,`close`,`volume`" ;
}

QString N::Forex::Equations::WhereTicks(TUID f,TUID t)
{
  return QString ( "where ( `quotetime` >= %1 ) and (`quotetime` < %2)" )
         . arg   ( f                                                    )
         . arg   ( t                                                  ) ;
}

QString N::Forex::Equations::WherePeriod(TUID f,TUID t)
{
  return QString ( "where ( `quote` >= %1 ) and (`quote` < %2)" )
         . arg   ( f                                            )
         . arg   ( t                                          ) ;
}

QDateTime N::Forex::Equations::NextTick(QDateTime Now,QDateTime ToTime)
{
  int yy = Now . date ( ) . year  ( )                                    ;
  int mm = Now . date ( ) . month ( )                                    ;
  int dd = Now . date ( ) . day   ( )                                    ;
  Now    = Now . addDays ( 8 )                                           ;
  yy     = Now . date ( ) . year  ( )                                    ;
  mm     = Now . date ( ) . month ( )                                    ;
  dd     = Now . date ( ) . day   ( )                                    ;
  dd     = TickDTS [ dd ]                                                ;
  return QDateTime ( QDate ( yy , mm , dd ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextM1(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( )                                   ;
  int mm = Now . date ( ) . month ( ) + 1                               ;
  if ( mm > 12 )                                                        {
     mm = 1                                                             ;
     yy++                                                               ;
  }                                                                     ;
  return QDateTime ( QDate ( yy , mm , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextM5(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( )                                   ;
  int mm = Now . date ( ) . month ( ) + 1                               ;
  if ( mm > 12 )                                                        {
     mm = 1                                                             ;
     yy++                                                               ;
  }                                                                     ;
  return QDateTime ( QDate ( yy , mm , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextM15(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( )                                   ;
  int mm = Now . date ( ) . month ( ) + 1                               ;
  if ( mm > 12 )                                                        {
     mm = 1                                                             ;
     yy++                                                               ;
  }                                                                     ;
  return QDateTime ( QDate ( yy , mm , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextM30(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( ) + 1                               ;
  return QDateTime ( QDate ( yy ,  1 , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextH1(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( ) + 1                               ;
  return QDateTime ( QDate ( yy ,  1 , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextH4(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( ) + 1                               ;
  return QDateTime ( QDate ( yy ,  1 , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextD1(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( ) + 1                               ;
  return QDateTime ( QDate ( yy ,  1 , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextW1(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( ToTime )                                                   ;
  int yy = Now . date ( ) . year  ( ) + 1                               ;
  return QDateTime ( QDate ( yy ,  1 , 1 ) , QTime(0,0,0,0) , Qt::UTC ) ;
}

QDateTime N::Forex::Equations::NextMN1(QDateTime Now,QDateTime ToTime)
{ Q_UNUSED ( Now )              ;
  return ToTime . addSecs ( 1 ) ;
}

QDateTime N::Forex::Equations::NextBlock(int p,QDateTime Now,QDateTime ToTime)
{
  switch            ( p            ) {
    case Forex::Ticks                :
    return NextTick ( Now , ToTime ) ;
    case Forex::M1                   :
    return NextM1   ( Now , ToTime ) ;
    case Forex::M5                   :
    return NextM5   ( Now , ToTime ) ;
    case Forex::M15                  :
    return NextM15  ( Now , ToTime ) ;
    case Forex::M30                  :
    return NextM30  ( Now , ToTime ) ;
    case Forex::H1                   :
    return NextH1   ( Now , ToTime ) ;
    case Forex::H4                   :
    return NextH4   ( Now , ToTime ) ;
    case Forex::D1                   :
    return NextD1   ( Now , ToTime ) ;
    case Forex::W1                   :
    return NextW1   ( Now , ToTime ) ;
    case Forex::MN1                  :
    return NextMN1  ( Now , ToTime ) ;
  }                                  ;
  return ToTime   . addSecs ( 1 )    ;
}

QString N::Forex::Equations::QuoteTableName(TUID t)
{
  TUID      dt = t                                                           ;
  QDateTime N                                                                ;
  int       yy                                                               ;
  int       mm                                                               ;
  int       dd                                                               ;
  dt /= 1000                                                                 ;
  N   = QDateTime::fromTime_t ( (time_t) dt , Qt::UTC )                      ;
  yy  = N . date ( ) . year  ( )                                             ;
  mm  = N . date ( ) . month ( )                                             ;
  dd  = N . date ( ) . day   ( )                                             ;
  switch ( QuotePeriod )                                                     {
    case Forex::Ticks                                                        :
      dd = TickDTS [ dd ]                                                    ;
    return QString("%1_%2_%3%4_%5%6").arg(base).arg(yy).arg(mm/10).arg(mm%10).arg(dd/10).arg(dd%10) ;
    case Forex::M1                                                           :
    case Forex::M5                                                           :
    case Forex::M15                                                          :
    return QString("%1_%2_%3%4").arg(base).arg(yy).arg(mm/10).arg(mm%10)     ;
    case Forex::M30                                                          :
    case Forex::H1                                                           :
    case Forex::H4                                                           :
    case Forex::D1                                                           :
    case Forex::W1                                                           :
    return QString("%1_%2").arg(base).arg(yy)                                ;
    case Forex::MN1                                                          :
    return base                                                              ;
  }                                                                          ;
  return ""                                                                  ;
}

bool N::Forex::Equations::GetConnexion(SqlConnection & SC,QString table)
{
  if ( SQL <= 0 ) return false                   ;
  Connexion = SC . GetConnection ( table , SQL ) ;
  return Connexion . isValid ( )                 ;
}

bool N::Forex::Equations::PeriodExists(SqlConnection & SC)
{
  QString Q                                               ;
  Q = QString ( "show create table `%1`" ) . arg ( base ) ;
  return SC . Query ( Q )                                 ;
}

bool N::Forex::Equations::Get (
       SqlConnection & SC     ,
       QString         T      ,
       SUID            S      ,
       int             L      ,
       QString         N      )
{
  QString Q                                              ;
  ////////////////////////////////////////////////////////
  Id = -1                                                ;
  Q  = SC . sql . SelectFrom                             (
         "id"                                            ,
         T                                               ,
         SC . sql . Where                                (
           3                                             ,
           "uuid"                                        ,
           "level"                                       ,
           "name"                                      ) ,
         SC.sql.Limit(0,1)                             ) ;
  SC . Prepare ( Q                     )                 ;
  SC . Bind    ( "uuid"   , S          )                 ;
  SC . Bind    ( "level"  , L          )                 ;
  SC . Bind    ( "name"   , N.toUtf8() )                 ;
  IfSqlExec ( SC ) Id = SC . Int ( 0 )                   ;
  if ( Id < 0 ) return false                             ;
  Q = SC . sql . SelectFrom                              (
        "connexion,pair,digits,full,timezone,fromYear,toYear" ,
        T                                                ,
        QString("where id = %1").arg(Id)                 ,
        SC.sql.Limit(0,1)                              ) ;
  if ( ! SC.Fetch(Q) ) return false                      ;
  SUID       connexion = SC . Uuid   ( 0 )               ;
  SUID       pair      = SC . Uuid   ( 1 )               ;
  int        digits    = SC . Int    ( 2 )               ;
  int        full      = SC . Int    ( 3 )               ;
  QString    tz        = SC . String ( 4 )               ;
  int        fy        = SC . Int    ( 5 )               ;
  int        ty        = SC . Int    ( 6 )               ;
  QByteArray TZB       = tz . toUtf8 (   )               ;
  ////////////////////////////////////////////////////////
  Name     = N                                           ;
  Supplier = S                                           ;
  SQL      = connexion                                   ;
  Pair     = pair                                        ;
  TimeZone = tz                                          ;
  TZ       = QTimeZone ( TZB )                           ;
  FromYear = fy                                          ;
  ToYear   = ty                                          ;
  Full     = full                                        ;
  setDigits ( digits )                                   ;
  setLevel  ( L      )                                   ;
  ////////////////////////////////////////////////////////
  return true                                            ;
}

bool N::Forex::Equations::GetSymbols (
       SqlConnection & SC            ,
       QString         table         ,
       SUID            supplier      ,
       int             L             ,
       QStringList   & symbols       )
{
  QString Q                                                            ;
  Q = SC . sql . SelectFrom                                            (
        "name"                                                         ,
        table                                                          ,
        QString("where uuid = %1 and level = %2").arg(supplier).arg(L) ,
        SC . OrderByAsc ( "id" )                                     ) ;
  SqlLoopNow ( SC , Q )                                                ;
    symbols << SC . String ( 0 )                                       ;
  SqlLoopErr ( SC , Q )                                                ;
  SqlLoopEnd ( SC , Q )                                                ;
  return ( symbols . count ( ) > 0 )                                   ;
}

QString N::Forex::Equations::Replace(QString s)
{
  s = s . replace ( "CNH" , "CNY" ) ;
  return s                          ;
}

void N::Forex::Equations::setPeriod(int Period)
{
  QuotePeriod = (int) Period            ;
  period      = periods [ QuotePeriod ] ;
  base        = QString ( "%1_%2_%3_%4" )
                . arg   ( identifier    )
                . arg   ( level         )
                . arg   ( symbol        )
                . arg   ( period      ) ;
  base = base . replace ( "/" , "_"   ) ;
}

void N::Forex::Equations::setDigits(int digits)
{
  int r      = Full - digits                           ;
  Multiply   = 1                                       ;
  Reversal   = 1                                       ;
  FullDigits = 1                                       ;
  Digits     = digits                                  ;
  for (int i = 0 ; i < Full   ; i++ ) FullDigits *= 10 ;
  for (int i = 0 ; i < digits ; i++ ) Multiply   *= 10 ;
  for (int i = 0 ; i < r      ; i++ ) Reversal   *= 10 ;
}

void N::Forex::Equations::setLevel(int L)
{
  Level = L                                        ;
  level = QString ( QChar ( (char) ( 'A' + L ) ) ) ;
}

bool N::Forex::Equations::isCorrect(void)
{
  if ( Name     . length ( ) <= 0        ) return false ;
  if ( Supplier              <= 0        ) return false ;
  if ( SQL                   <= 0        ) return false ;
  if ( Pair                  <= 0        ) return false ;
  if ( Digits                <  0        ) return false ;
  if ( TimeZone . length ( ) <= 0        ) return false ;
  if ( FromYear              <= 0        ) return false ;
  if ( ToYear                <= 0        ) return false ;
  if ( ToYear                <= FromYear ) return false ;
  return true                                           ;
}

int N::Forex::Equations::toInteger(double v)
{
  return (int)( v * Multiply ) ;
}

int N::Forex::Equations::toReversal(double v)
{
  return (int)( v * Reversal ) ;
}

QString N::Forex::Equations::PeriodName(int p)
{
  return periods [ p ] ;
}

QString N::Forex::Equations::FetchIdentifier (
          SqlConnection & SC                 ,
          QString         table              ,
          SUID            uuid               )
{
  QString Q                  ;
  QString name = ""          ;
  Q = SC.sql.SelectFrom      (
        "name"               ,
        table                ,
        SC.WhereUuid(uuid)   ,
        SC.OrderByDesc("id") ,
        SC.sql.Limit(0,1)  ) ;
  if (SC.Fetch(Q))           {
    name = SC . String ( 0 ) ;
  }                          ;
  return name                ;
}

SUID N::Forex::Equations::FetchProvider (
       SqlConnection & SC               ,
       QString         table            ,
       QString         id               )
{
  QString Q                                 ;
  SUID    provider = 0                      ;
  ///////////////////////////////////////////
  Q = SC . sql . SelectFrom                 (
        "uuid"                              ,
        table                               ,
        SC . sql . Where ( 1 , "name" )   ) ;
  SC . Prepare ( Q                        ) ;
  SC . Bind    ( "name" , id . toUtf8 ( ) ) ;
  if ( SC . Exec ( ) && SC . Next ( ) )     {
    provider = SC . Uuid ( 0 )              ;
  }                                         ;
  ///////////////////////////////////////////
  return provider                           ;
}

qint64 N::Forex::Equations::ToReversal (
         qint64      ticks             ,
         TickUnion * tu                ,
         TickUnion * pu                ,
         qint64      full              )
{
  if ( ticks <= 0 ) return 0                               ;
  ::memset ( pu , 0 , sizeof(TickUnion) * ticks )          ;
  return Forex::ToReversalTicks ( ticks , tu , pu , full ) ;
}

qint64 N::Forex::Equations::ToReversal (
         qint64      ticks             ,
         TickUnion * tu                ,
         TickUnion * pu                )
{
  if ( ticks <= 0 ) return 0                                     ;
  ::memset ( pu , 0 , sizeof(TickUnion) * ticks )                ;
  return Forex::ToReversalTicks ( ticks , tu , pu , FullDigits ) ;
}

qint64 N::Forex::Equations::ToM1 (
         qint64        ticks     ,
         TickUnion   * tu        ,
         PeriodUnion * pu        ,
         int         * members   )
{
  if ( ticks <= 0 ) return 0                                          ;
  /////////////////////////////////////////////////////////////////////
  qint64 st = tu [ 0 ] . at                                           ;
  qint64 et = st                                                      ;
  qint64 ii = 1                                                       ;
  qint64 jj = 0                                                       ;
  /////////////////////////////////////////////////////////////////////
  st /= 1000                                                          ;
  st /= 60                                                            ;
  st *= 60                                                            ;
  et  = st                                                            ;
  et += 60                                                            ;
  st *= 1000                                                          ;
  et *= 1000                                                          ;
  /////////////////////////////////////////////////////////////////////
  pu [ jj ] . at = st                                                 ;
  pu [ jj ] . Integer . open    = tu [ 0 ] . Integer . bid            ;
  pu [ jj ] . Integer . close   = tu [ 0 ] . Integer . bid            ;
  pu [ jj ] . Integer . high    = tu [ 0 ] . Integer . ask            ;
  pu [ jj ] . Integer . low     = tu [ 0 ] . Integer . bid            ;
  pu [ jj ] . Integer . volume  = tu [ 0 ] . Integer . bidvolume      ;
  pu [ jj ] . Integer . volume += tu [ 0 ] . Integer . askvolume      ;
  members [ jj ] = 1                                                  ;
  /////////////////////////////////////////////////////////////////////
  while ( ii < ticks )                                                {
    qint64 at = tu [ ii ] . at                                        ;
    if ( ( at >= st ) && ( at < et ) )                                {
      qint32 bid = tu [ ii ] . Integer . bid                          ;
      qint32 ask = tu [ ii ] . Integer . ask                          ;
      if ( bid < pu [ jj ] . Integer . low  )                         {
        pu [ jj ] . Integer . low  = bid                              ;
      }                                                               ;
      if ( ask < pu [ jj ] . Integer . low  )                         {
        pu [ jj ] . Integer . low  = ask                              ;
      }                                                               ;
      if ( ask > pu [ jj ] . Integer . high )                         {
        pu [ jj ] . Integer . high = ask                              ;
      }                                                               ;
      if ( bid > pu [ jj ] . Integer . high )                         {
        pu [ jj ] . Integer . high = bid                              ;
      }                                                               ;
      pu [ jj ] . Integer . close   = bid                             ;
      pu [ jj ] . Integer . volume += tu [ ii ] . Integer . bidvolume ;
      pu [ jj ] . Integer . volume += tu [ ii ] . Integer . askvolume ;
      members [ jj ] ++                                               ;
    } else                                                            {
      pu [ jj ] . Integer . volume *= 1000                            ;
      /////////////////////////////////////////////////////////////////
      jj ++                                                           ;
      members [ jj ] = 1                                              ;
      /////////////////////////////////////////////////////////////////
      st  = at                                                        ;
      st /= 1000                                                      ;
      st /= 60                                                        ;
      st *= 60                                                        ;
      et  = st                                                        ;
      et += 60                                                        ;
      st *= 1000                                                      ;
      et *= 1000                                                      ;
      /////////////////////////////////////////////////////////////////
      pu [ jj ] . at                = st                              ;
      pu [ jj ] . Integer . open    = tu [ ii ] . Integer . bid       ;
      pu [ jj ] . Integer . close   = tu [ ii ] . Integer . bid       ;
      pu [ jj ] . Integer . high    = tu [ ii ] . Integer . ask       ;
      pu [ jj ] . Integer . low     = tu [ ii ] . Integer . bid       ;
      pu [ jj ] . Integer . volume  = tu [ ii ] . Integer . bidvolume ;
      pu [ jj ] . Integer . volume += tu [ ii ] . Integer . askvolume ;
      /////////////////////////////////////////////////////////////////
    }                                                                 ;
    ii ++                                                             ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  pu [ jj ] . Integer . volume *= 1000                                ;
  /////////////////////////////////////////////////////////////////////
  return jj + 1                                                       ;
}

qint64 N::Forex::Equations::ToMx (
         qint64        count     ,
         qint64        dt        ,
         PeriodUnion * tu        ,
         PeriodUnion * pu        ,
         int         * members   )
{
  if ( count <= 0 ) return 0                                          ;
  /////////////////////////////////////////////////////////////////////
  qint64 st = tu [ 0 ] . at                                           ;
  qint64 et = st                                                      ;
  qint64 ii = 1                                                       ;
  qint64 jj = 0                                                       ;
  /////////////////////////////////////////////////////////////////////
  st /= 1000                                                          ;
  st /= 60                                                            ;
  st *= 60                                                            ;
  et  = st                                                            ;
  et += dt                                                            ;
  st *= 1000                                                          ;
  et *= 1000                                                          ;
  /////////////////////////////////////////////////////////////////////
  pu [ jj ] . at = st                                                 ;
  pu [ jj ] . Integer . open    = tu [ 0 ] . Integer . open           ;
  pu [ jj ] . Integer . close   = tu [ 0 ] . Integer . close          ;
  pu [ jj ] . Integer . high    = tu [ 0 ] . Integer . high           ;
  pu [ jj ] . Integer . low     = tu [ 0 ] . Integer . low            ;
  pu [ jj ] . Integer . volume  = tu [ 0 ] . Integer . volume         ;
  /////////////////////////////////////////////////////////////////////
  while ( ii < count )                                                {
    qint64 at = tu [ ii ] . at                                        ;
    if ( ( at >= st ) && ( at < et ) )                                {
      qint32 high = tu [ ii ] . Integer . high                        ;
      qint32 low  = tu [ ii ] . Integer . low                         ;
      if ( low  < pu [ jj ] . Integer . low  )                        {
        pu [ jj ] . Integer . low  = low                              ;
      }                                                               ;
      if ( high < pu [ jj ] . Integer . low  )                        {
        pu [ jj ] . Integer . low  = high                             ;
      }                                                               ;
      if ( low  > pu [ jj ] . Integer . high )                        {
        pu [ jj ] . Integer . high = low                              ;
      }                                                               ;
      if ( high > pu [ jj ] . Integer . high )                        {
        pu [ jj ] . Integer . high = high                             ;
      }                                                               ;
      pu [ jj ] . Integer . close   = tu [ ii ] . Integer . close     ;
      pu [ jj ] . Integer . volume += tu [ ii ] . Integer . volume    ;
      members [ jj ] ++                                               ;
    } else                                                            {
      jj ++                                                           ;
      members [ jj ] = 1                                              ;
      /////////////////////////////////////////////////////////////////
      st  = at                                                        ;
      st /= 1000                                                      ;
      st /= 60                                                        ;
      st *= 60                                                        ;
      et  = st                                                        ;
      et += dt                                                        ;
      st *= 1000                                                      ;
      et *= 1000                                                      ;
      /////////////////////////////////////////////////////////////////
      pu [ jj ] . at                = st                              ;
      pu [ jj ] . Integer . open    = tu [ ii ] . Integer . open      ;
      pu [ jj ] . Integer . close   = tu [ ii ] . Integer . close     ;
      pu [ jj ] . Integer . high    = tu [ ii ] . Integer . high      ;
      pu [ jj ] . Integer . low     = tu [ ii ] . Integer . low       ;
      pu [ jj ] . Integer . volume  = tu [ ii ] . Integer . volume    ;
      /////////////////////////////////////////////////////////////////
    }                                                                 ;
    ii ++                                                             ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  return jj + 1                                                       ;
}
