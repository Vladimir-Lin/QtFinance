#include <qtfinance.h>

extern int TickDTS [ 32 ] ;

N::Forex::Reader:: Reader      ( void     )
                 : Equations   (          )
{
}

N::Forex::Reader::~Reader(void)
{
}

bool N::Forex::Reader::Renew(SqlConnection & SC,QString table)
{
  QString Q                                          ;
  ////////////////////////////////////////////////////
  Q = SC . sql . SelectFrom                          (
        "id"                                         ,
        table                                        ,
        SC . sql . Where                             (
          3                                          ,
          "uuid"                                     ,
          "pair"                                     ,
          "name"                          )          ,
        SC.sql.Limit(0,1)                 )          ;
  SC . Prepare ( Q                        )          ;
  SC . Bind    ( "uuid"   , Supplier      )          ;
  SC . Bind    ( "pair"   , Pair          )          ;
  SC . Bind    ( "name"   , Name.toUtf8() )          ;
  IfSqlExec ( SC ) Id = SC . Int ( 0 )               ;
  ////////////////////////////////////////////////////
  if ( Id <= 0 )                                     {
    Q  = SC . sql . InsertInto                       (
           table                                     ,
           10                                        ,
           "uuid"                                    ,
           "connexion"                               ,
           "pair"                                    ,
           "digits"                                  ,
           "full"                                    ,
           "timezone"                                ,
           "level"                                   ,
           "fromYear"                                ,
           "toYear"                                  ,
           "name"                                  ) ;
    SC . Prepare ( Q                               ) ;
    SC . Bind    ( "uuid"      , Supplier          ) ;
    SC . Bind    ( "connexion" , SQL               ) ;
    SC . Bind    ( "pair"      , Pair              ) ;
    SC . Bind    ( "digits"    , Digits            ) ;
    SC . Bind    ( "full"      , Full              ) ;
    SC . Bind    ( "timezone"  , TimeZone.toUtf8() ) ;
    SC . Bind    ( "level"     , Level             ) ;
    SC . Bind    ( "fromYear"  , FromYear          ) ;
    SC . Bind    ( "toYear"    , ToYear            ) ;
    SC . Bind    ( "name"      , Name.toUtf8()     ) ;
    SC . Exec    (                                 ) ;
    Q = SC . sql . SelectFrom                        (
          "id"                                       ,
          table                                      ,
          SC . sql . Where                           (
            3                                        ,
            "uuid"                                   ,
            "pair"                                   ,
            "name"                          )        ,
          SC.sql.Limit(0,1)                 )        ;
    SC . Prepare ( Q                        )        ;
    SC . Bind    ( "uuid"   , Supplier      )        ;
    SC . Bind    ( "pair"   , Pair          )        ;
    SC . Bind    ( "name"   , Name.toUtf8() )        ;
    IfSqlExec ( SC ) Id = SC . Int ( 0 )             ;
  } else                                             {
    Q  = SC . sql . Update                           (
           table                                     ,
           SC . sql . Where ( 1 , "id" )             ,
           10                                        ,
           "uuid"                                    ,
           "connexion"                               ,
           "pair"                                    ,
           "digits"                                  ,
           "full"                                    ,
           "timezone"                                ,
           "level"                                   ,
           "fromYear"                                ,
           "toYear"                                  ,
           "name"                                  ) ;
    SC . Prepare ( Q                               ) ;
    SC . Bind    ( "id"        , Id                ) ;
    SC . Bind    ( "uuid"      , Supplier          ) ;
    SC . Bind    ( "connexion" , SQL               ) ;
    SC . Bind    ( "pair"      , Pair              ) ;
    SC . Bind    ( "digits"    , Digits            ) ;
    SC . Bind    ( "full"      , Full              ) ;
    SC . Bind    ( "timezone"  , TimeZone.toUtf8() ) ;
    SC . Bind    ( "level"     , Level             ) ;
    SC . Bind    ( "fromYear"  , FromYear          ) ;
    SC . Bind    ( "toYear"    , ToYear            ) ;
    SC . Bind    ( "name"      , Name.toUtf8()     ) ;
    SC . Exec    (                                 ) ;
  }                                                  ;
  ////////////////////////////////////////////////////
  return ( Id > 0 )                                  ;
}

QDateTime N::Forex::Reader::NextTime(QDateTime Now,QDateTime ToTime)
{
  return NextBlock ( QuotePeriod , Now , ToTime ) ;
}

bool N::Forex::Reader::Obtain  (
       SqlConnection   & SC    ,
       Finance::Ticks  & ticks ,
       QString           Q     )
{
  int cnt = ticks . FromSQL ( SC , Q  ) ;
  return                    ( cnt > 0 ) ;
}

bool N::Forex::Reader::Obtain   (
       SqlConnection   & SC     ,
       Finance::Charts & charts ,
       QString           Q      )
{
  int cnt = charts . FromSQL ( SC , Q  ) ;
  return                     ( cnt > 0 ) ;
}

bool N::Forex::Reader::Obtain (
       SqlConnection  & SC    ,
       TickUnion      & tick  )
{
  tick . id                  = SC . Tuid ( 0 ) ;
  tick . at                  = SC . Tuid ( 1 ) ;
  tick . Integer . bid       = SC . Int  ( 2 ) ;
  tick . Integer . ask       = SC . Int  ( 3 ) ;
  tick . Integer . bidvolume = SC . Tuid ( 4 ) ;
  tick . Integer . askvolume = SC . Tuid ( 5 ) ;
  return true                                  ;
}

bool N::Forex::Reader::Obtain  (
       SqlConnection  & SC     ,
       PeriodUnion    & period )
{
  period . id               = SC . Tuid ( 0 ) ;
  period . at               = SC . Tuid ( 1 ) ;
  period . Integer . open   = SC . Int  ( 2 ) ;
  period . Integer . high   = SC . Int  ( 3 ) ;
  period . Integer . low    = SC . Int  ( 4 ) ;
  period . Integer . close  = SC . Tuid ( 5 ) ;
  period . Integer . volume = SC . Tuid ( 6 ) ;
  return true                                 ;
}

bool N::Forex::Reader::Obtain      (
       Finance::Ticks  & ticks     ,
       QDateTime         FromTime  ,
       QDateTime         ToTime    )
{
  ticks . setValue ( "Multiply" , Multiply ) ;
  if ( FromTime > ToTime ) return false                                      ;
  ////////////////////////////////////////////////////////////////////////////
  SqlConnection FC ( Connexion )                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( FC . open ( QtUUID::createUuidString() , QString(__FUNCTION__) ) )    {
    QDateTime st = FromTime                                                  ;
    QDateTime tt = ToTime                                                    ;
    QString   Q                                                              ;
    QString   items = "`id`,`quotetime`,`bidprice`,`askprice`,`bidvolume`,`askvolume`" ;
    QString   order = FC . OrderByAsc ( "`quotetime`" )                      ;
    QString   where                                                          ;
    QString   table                                                          ;
    TUID      stime = st . toTime_t ( )                                      ;
    TUID      etime = tt . toTime_t ( )                                      ;
    //////////////////////////////////////////////////////////////////////////
    stime *= 1000                                                            ;
    etime *= 1000                                                            ;
    where  = QString("where ( `quotetime` >= %1 ) and (`quotetime` <= %2)").arg(stime).arg(etime) ;
    //////////////////////////////////////////////////////////////////////////
    do                                                                       {
      stime  = st . toTime_t ( )                                             ;
      stime *= 1000                                                          ;
      table  = QuoteTableName ( stime )                                      ;
      Q = FC . sql . SelectFrom ( items , table , where , order )            ;
      Obtain ( FC , ticks , Q )                                              ;
      st = NextTime ( st , tt )                                              ;
    } while ( st <= tt )                                                     ;
    //////////////////////////////////////////////////////////////////////////
    FC . close ( )                                                           ;
  }                                                                          ;
  FC   . close ( )                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return ( ticks . count ( ) > 0 )                                           ;
}

bool N::Forex::Reader::Obtain     (
       Finance::Charts & charts   ,
       QDateTime         FromTime ,
       QDateTime         ToTime   )
{
  charts . setValue ( "Multiply" , Multiply )                                ;
  if ( FromTime > ToTime ) return false                                      ;
  ////////////////////////////////////////////////////////////////////////////
  SqlConnection FC ( Connexion )                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( FC . open ( QtUUID::createUuidString() , QString(__FUNCTION__) ) )    {
    QDateTime st = FromTime                                                  ;
    QDateTime tt = ToTime                                                    ;
    QString   Q                                                              ;
    QString   items = "`id`,`quote`,`open`,`high`,`low`,`close`,`volume`"    ;
    QString   order = FC . OrderByAsc ( "`quote`" )                          ;
    QString   where                                                          ;
    QString   table                                                          ;
    TUID      stime = st . toTime_t ( )                                      ;
    TUID      etime = tt . toTime_t ( )                                      ;
    //////////////////////////////////////////////////////////////////////////
    stime *= 1000                                                            ;
    etime *= 1000                                                            ;
    where  = QString("where ( `quote` >= %1 ) and (`quote` <= %2)").arg(stime).arg(etime) ;
    //////////////////////////////////////////////////////////////////////////
    do                                                                       {
      stime  = st . toTime_t ( )                                             ;
      stime *= 1000                                                          ;
      table  = QuoteTableName ( stime )                                      ;
      Q = FC . sql . SelectFrom ( items , table , where , order )            ;
      Obtain ( FC , charts , Q )                                             ;
      st = NextTime ( st , tt )                                              ;
    } while ( st <= tt )                                                     ;
    //////////////////////////////////////////////////////////////////////////
    FC . close ( )                                                           ;
  }                                                                          ;
  FC   . close ( )                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  return ( charts . count ( ) > 0 )                                          ;
}
