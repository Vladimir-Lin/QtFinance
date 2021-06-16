#include <qtfinance.h>

extern int TickDTS [ 32 ] ;

N::Forex::Importer:: Importer    ( void     )
                   : Equations   (          )
{
}

N::Forex::Importer::~Importer (void)
{
}

QString N::Forex::Importer::ConvertTable(void)
{
  QString T                                                                  ;
  switch ( QuotePeriod )                                                     {
    case Forex::Ticks                                                        :
    return ""                                                                ;
    case Forex::M1                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::Ticks ]                                 )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::M5                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M5    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::M15                                                          :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M15   ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::M30                                                          :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M30   ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::H1                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::H4                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( periods [ Forex::H4    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::D1                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::W1                                                           :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( periods [ Forex::W1    ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
    case Forex::MN1                                                          :
      T = QString ( "%1_%2_%3_%4_%5"                                         )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( periods [ Forex::MN1   ]                                 )
          . arg   ( level                                                    )
          . arg   ( symbol                                                 ) ;
      T = T . replace ( "/"   , "_"   )                                      ;
      T = T . replace ( "CNH" , "CNY" )                                      ;
    return T                                                                 ;
  }                                                                          ;
  return ""                                                                  ;
}

QString N::Forex::Importer::ConvertMerger(void)
{
  QString T                                                                  ;
  switch ( QuotePeriod )                                                     {
    case Forex::Ticks                                                        :
    return ""                                                                ;
    case Forex::M1                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::Ticks ]                                 )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::M5                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M5    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::M15                                                          :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M15   ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::M30                                                          :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::M30   ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::H1                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::M1    ]                                 )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::H4                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( periods [ Forex::H4    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::D1                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::H1    ]                                 )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::W1                                                           :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( periods [ Forex::W1    ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
    case Forex::MN1                                                          :
      T = QString ( "%1_%2_%3_%4"                                            )
          . arg   ( identifier                                               )
          . arg   ( periods [ Forex::D1    ]                                 )
          . arg   ( periods [ Forex::MN1   ]                                 )
          . arg   ( level                                                  ) ;
    return T                                                                 ;
  }                                                                          ;
  return ""                                                                  ;
}

bool N::Forex::Importer::Renew(SqlConnection & SC,QString table)
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

bool N::Forex::Importer::CreatePeriods(SqlConnection & SC)
{
  switch ( QuotePeriod )        {
    case Forex::Ticks           :
    return CreateTicks   ( SC ) ;
    case Forex::M1              :
    return CreateMonthly ( SC ) ;
    case Forex::M5              :
    return CreateMonthly ( SC ) ;
    case Forex::M15             :
    return CreateMonthly ( SC ) ;
    case Forex::M30             :
    return CreateYearly  ( SC ) ;
    case Forex::H1              :
    return CreateYearly  ( SC ) ;
    case Forex::H4              :
    return CreateYearly  ( SC ) ;
    case Forex::D1              :
    return CreateYearly  ( SC ) ;
    case Forex::W1              :
    return CreateYearly  ( SC ) ;
    case Forex::MN1             :
    return CreateTable   ( SC ) ;
  }                             ;
  return false                  ;
}

bool N::Forex::Importer::Insert (
       SqlConnection & SC       ,
       TUID            quote    ,
       int             open     ,
       int             high     ,
       int             low      ,
       int             close    ,
       TUID            volume   ,
       QDateTime       Now      )
{
  QString table                                                              ;
  int     yy = Now . date ( ) . year  ( )                                    ;
  int     mm = Now . date ( ) . month ( )                                    ;
  switch ( QuotePeriod )                                                     {
    case Forex::Ticks                                                        :
    return false                                                             ;
    case Forex::M1                                                           :
    case Forex::M5                                                           :
    case Forex::M15                                                          :
      table = QString("%1_%2_%3%4").arg(base).arg(yy).arg(mm/10).arg(mm%10)  ;
    break                                                                    ;
    case Forex::M30                                                          :
    case Forex::H1                                                           :
    case Forex::H4                                                           :
    case Forex::D1                                                           :
    case Forex::W1                                                           :
      table = QString("%1_%2").arg(base).arg(yy)                             ;
    break                                                                    ;
    case Forex::MN1                                                          :
      table = base                                                           ;
    break                                                                    ;
    default                                                                  :
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return Insert ( SC                                                         ,
                  table                                                      ,
                  quote                                                      ,
                  open                                                       ,
                  high                                                       ,
                  low                                                        ,
                  close                                                      ,
                  volume                                                   ) ;
}

bool N::Forex::Importer::CreateTicks(SqlConnection & SC)
{
  int         value = 0                          ;
  QStringList LT                                 ;
  QStringList YT                                 ;
  QString     Y                                  ;
  for (int yy = FromYear ; yy <= ToYear ; yy++ ) {
    YT . clear ( )                               ;
    for (int mm=1;mm<=12;mm++)                   {
      for (int dd=0;dd<4;dd++)                   {
        QString t                                ;
        int     ai                               ;
        int     day = ( dd * 8 ) + 1             ;
        value++                                  ;
        ai = value * 800000                      ;
        t = QString ( "%1_%2_%3%4_%5%6"          )
            . arg   ( base                       )
            . arg   ( yy                         )
            . arg   ( mm  / 10                   )
            . arg   ( mm  % 10                   )
            . arg   ( day / 10                   )
            . arg   ( day % 10                 ) ;
        LT << t                                  ;
        YT << t                                  ;
        if ( CreateTick ( SC , t ) )             {
          AutoIncrement ( SC , t , ai + 1 )      ;
        }                                        ;
      }                                          ;
    }                                            ;
    Y = QString ( "%1_%2"                        )
        . arg   ( base                           )
        . arg   ( yy                           ) ;
    CreateTicks ( SC , Y , YT )                  ;
  }                                              ;
  return CreateTicks ( SC , base , LT )          ;
}

bool N::Forex::Importer::CreateMonthly(SqlConnection & SC)
{
  int         value = 0                          ;
  QStringList LT                                 ;
  QStringList YT                                 ;
  QString     Y                                  ;
  for (int yy = FromYear ; yy <= ToYear ; yy++ ) {
    YT . clear ( )                               ;
    for (int mm=1;mm<=12;mm++)                   {
      QString t                                  ;
      int     ai                                 ;
      value++                                    ;
      ai = value * 100000                        ;
      t = QString ( "%1_%2_%3%4"                 )
          . arg   ( base                         )
          . arg   ( yy                           )
          . arg   ( mm / 10                      )
          . arg   ( mm % 10                    ) ;
      LT << t                                    ;
      YT << t                                    ;
      if ( Create ( SC , t ) )                   {
        AutoIncrement ( SC , t , ai + 1 )        ;
      }                                          ;
    }                                            ;
    Y = QString ( "%1_%2"                        )
        . arg   ( base                           )
        . arg   ( yy                           ) ;
    CreateMerge ( SC , Y , YT )                  ;
  }                                              ;
  return CreateMerge ( SC , base , LT )          ;
}

bool N::Forex::Importer::CreateYearly(SqlConnection & SC)
{
  int         value = 0                          ;
  QStringList LT                                 ;
  for (int yy = FromYear ; yy <= ToYear ; yy++ ) {
    QString t                                    ;
    int     ai                                   ;
    value++                                      ;
    ai = value * 100000                          ;
    t = QString ( "%1_%2"                        )
        . arg   ( base                           )
        . arg   ( yy                           ) ;
    LT << t                                      ;
    if ( Create ( SC , t ) )                     {
      AutoIncrement ( SC , t , ai + 1 )          ;
    }                                            ;
  }                                              ;
  return CreateMerge ( SC , base , LT )          ;
}

bool N::Forex::Importer::CreateTable(SqlConnection & SC)
{
  return Create ( SC , base ) ;
}

bool N::Forex::Importer::Create(SqlConnection & SC,QString table)
{
  QString Q                                                                  ;
  Q = QString ( "show create table `%1` ;").arg(table)                       ;
  if ( SC . Fetch ( Q ) ) return false                                       ;
  Q = QString ( "create table `%1` ("
                "`id` integer not null primary key auto_increment,"
                "`quote` bigint default 0,"
                "`open` integer default 0,"
                "`high` integer default 0,"
                "`low` integer default 0,"
                "`close` integer default 0,"
                "`volume` bigint default 0,"
                "unique (`quote`),"
                "index (`open`),"
                "index (`high`),"
                "index (`low`),"
                "index (`close`),"
                "index (`volume`)"
                ") Engine = MyISAM default Charset = Utf8 ;" ).arg ( table ) ;
  return SC . Query ( Q )                                                    ;
}

bool N::Forex::Importer::CreateConvert(SqlConnection  & SC,QString table,int autoincrement)
{
  QString Q                                                                  ;
  Q = QString ( "show create table `%1` ;").arg(table)                       ;
  if ( SC . Fetch ( Q ) ) return false                                       ;
  Q = QString                                                                (
        "create table `%1` ("
        "`id` integer primary key auto_increment,"
        "`pair` bigint unsigned not null,"
        "`time` bigint not null,"
        "`flags` bigint unsigned default 0,"
        "`ltime` timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,"
        "unique `pairtime` (`pair`,`time`),"
        "key `pair` (`pair`),"
        "key `time` (`time`),"
        "key `flags` (`flags`)"
        ") Engine = MyISAM default Charset = Utf8 ;"
      ) . arg ( table )                                                      ;
  if ( SC . Query ( Q ) )                                                    {
    AutoIncrement ( SC , table , autoincrement )                             ;
  }                                                                          ;
  return false                                                               ;
}

bool N::Forex::Importer::MergeConverters (
       SqlConnection  & SC               ,
       QString          table            ,
       QStringList      Union            )
{
  QString Q                                                     ;
  Q = QString ( "show create table `%1` ;").arg(table)          ;
  if ( SC . Fetch ( Q ) ) return true                           ;
  Q  = QString ( "drop table `%1` ;" ) . arg ( table )          ;
  SC . Query ( Q )                                              ;
  Q = QString                                                   (
        "create table `%1` ("
        "`id` integer primary key auto_increment,"
        "`pair` bigint unsigned not null,"
        "`time` bigint not null,"
        "`flags` bigint unsigned default 0,"
        "`ltime` timestamp default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,"
        "unique `pairtime` (`pair`,`time`),"
        "key `pair` (`pair`),"
        "key `time` (`time`),"
        "key `flags` (`flags`)"
        ") Engine = MRG_MyISAM default Charset = Utf8"
        " INSERT_METHOD=Last Union=(%2);")
      . arg ( table                )
      . arg ( Union . join ( "," ) )                            ;
  return SC . Query ( Q )                                       ;
}

bool N::Forex::Importer::MergeConverters (
       SqlConnection  & SC               ,
       QStringList      Union            )
{
  return MergeConverters ( SC , ConvertMerger ( ) , Union ) ;
}

bool N::Forex::Importer::AutoIncrement(SqlConnection & SC,QString t,int ai)
{
  QString Q                                                     ;
  Q = QString ( "select `id` from `%1`"
                " order by `id` desc limit 0,1 ;" ) . arg ( t ) ;
  if ( SC . Fetch ( Q ) )                                       {
    int last = SC . Int ( 0 )                                   ;
    last = last + 1                                             ;
    if ( last > ai ) ai = last                                  ;
  }                                                             ;
  Q = QString ( "alter table `%1` auto_increment = %2 ;"        )
      . arg   ( t                                               )
      . arg   ( ai                                            ) ;
  return SC . Query ( Q )                                       ;
}

bool N::Forex::Importer::CreateMerge(SqlConnection & SC,QString table,QStringList Union)
{
  QString Q                                                     ;
  Q = QString ( "show create table `%1` ;").arg(table)          ;
  if ( SC . Fetch ( Q ) ) return true                           ;
  Q  = QString ( "drop table `%1` ;" ) . arg ( table )          ;
  SC . Query ( Q )                                              ;
  Q = QString("create table `%1` ("
              "`id` integer not null primary key auto_increment,"
              "`quote` bigint default 0,"
              "`open` integer default 0,"
              "`high` integer default 0,"
              "`low` integer default 0,"
              "`close` integer default 0,"
              "`volume` bigint default 0,"
              "unique (`quote`),"
              "index (`open`),"
              "index (`high`),"
              "index (`low`),"
              "index (`close`),"
              "index (`volume`)"
              ") Engine = MRG_MyISAM default Charset = Utf8"
              " INSERT_METHOD=Last Union=(%2);")
      . arg ( table                )
      . arg ( Union . join ( "," ) )                            ;
  return SC . Query ( Q )                                       ;
}

bool N::Forex::Importer::CreateTick(SqlConnection & SC,QString table)
{
  QString Q                                                                  ;
  Q  = QString ( "show create table `%1` ;").arg(table)                      ;
  if ( SC . Fetch ( Q ) ) return false                                       ;
  Q = QString ( "create table `%1` ("
                "`id` bigint not null primary key auto_increment,"
                "`quotetime` bigint default 0,"
                "`bidprice` integer default 0,"
                "`askprice` integer default 0,"
                "`bidvolume` bigint default 0,"
                "`askvolume` bigint default 0,"
                "unique (`quotetime`),"
                "index (`bidprice`),"
                "index (`askprice`),"
                "index (`bidvolume`),"
                "index (`askvolume`)"
                ") Engine = MyISAM default Charset = Utf8 ;" ).arg ( table ) ;
  return SC . Query ( Q )                                                    ;
}

bool N::Forex::Importer::CreateTicks(SqlConnection & SC,QString table,QStringList Union)
{
  QString Q                                                    ;
  Q = QString ( "show create table `%1` ;" ) . arg ( table )   ;
  if ( SC . Fetch ( Q ) ) return true                          ;
  Q  = QString ( "drop table `%1` ;" ) . arg ( table )         ;
  SC . Query ( Q )                                             ;
  Q = QString("create table `%1` ("
              "`id` bigint not null primary key auto_increment,"
              "`quotetime` bigint default 0,"
              "`bidprice` integer default 0,"
              "`askprice` integer default 0,"
              "`bidvolume` bigint default 0,"
              "`askvolume` bigint default 0,"
              "unique (`quotetime`),"
              "index (`bidprice`),"
              "index (`askprice`),"
              "index (`bidvolume`),"
              "index (`askvolume`)"
              ") Engine = MRG_MyISAM default Charset = Utf8"
              " INSERT_METHOD=Last Union=(%2) ;")
      . arg ( table                  )
      . arg ( Union . join ( " , " ) )                         ;
  return SC . Query ( Q )                                      ;
}

bool N::Forex::Importer::Insert(SqlConnection & SC,QString line)
{
  if ( Forex::Ticks == QuotePeriod ) {
    return InsertTick  ( SC , line ) ;
  }                                  ;
  return   InsertChart ( SC , line ) ;
}

bool N::Forex::Importer::InsertTick(SqlConnection & SC,QString line)
{
  QStringList S                                                        ;
  S = line . split ( "," )                                             ;
  if ( S . count ( ) < 4 ) return false                                ;
  //////////////////////////////////////////////////////////////////////
  QString   fmt = "yyyy.MM.dd hh:mm:ss.zzz"                            ;
  QString   Ts                                                         ;
  QDateTime Now                                                        ;
  QDateTime X                                                          ;
  TUID      Z                                                          ;
  double    bid                                                        ;
  double    ask                                                        ;
  double    bidv                                                       ;
  double    askv                                                       ;
  int       BID                                                        ;
  int       ASK                                                        ;
  TUID      BIDV                                                       ;
  TUID      ASKV                                                       ;
  //////////////////////////////////////////////////////////////////////
  Ts      = S [ 0 ]                                                    ;
  X       = QDateTime::fromString   ( Ts , fmt                       ) ;
  Now     . setTimeZone             ( TZ                             ) ;
  Now     . setDate                 ( X . date ( )                   ) ;
  Now     . setTime                 ( X . time ( )                   ) ;
  Z       = Now . toMSecsSinceEpoch (                                ) ;
  Ts      = QuoteTableName          ( Z                              ) ;
  bid     = S [ 1 ] . toDouble ( )                                     ;
  ask     = S [ 2 ] . toDouble ( )                                     ;
  bidv    = S [ 3 ] . toDouble ( )                                     ;
  askv    = S [ 4 ] . toDouble ( )                                     ;
  BID     = toInteger ( bid )                                          ;
  ASK     = toInteger ( ask )                                          ;
  BIDV    = bidv * 100000                                              ;
  ASKV    = askv * 100000                                              ;
  return InsertTick                                                    (
           SC                                                          ,
           Ts                                                          ,
           Z                                                           ,
           BID                                                         ,
           ASK                                                         ,
           BIDV                                                        ,
           ASKV                                                      ) ;
}

bool N::Forex::Importer::Insert(SqlConnection & SC,Finance::Tick & tick)
{
  QString Ts = QuoteTableName ( tick . Time ) ;
  return InsertTick                           (
           SC                                 ,
           Ts                                 ,
           tick . Time                        ,
           tick . iBid                        ,
           tick . iAsk                        ,
           tick . BidVolume                   ,
           tick . AskVolume                 ) ;
}

bool N::Forex::Importer::Insert(SqlConnection  & SC,Finance::Chart & chart)
{
  TUID    Z                  ;
  QString Ts                 ;
  Z  = chart . Server        ;
  Z *= 1000                  ;
  Ts = QuoteTableName ( Z )  ;
  return Insert              (
           SC                ,
           Ts                ,
           Z                 ,
           chart . iOpen     ,
           chart . iHigh     ,
           chart . iLow      ,
           chart . iClose    ,
           chart . iVolume ) ;
}

bool N::Forex::Importer::InsertChart(SqlConnection & SC,QString line)
{
  QStringList S                                                        ;
  S = line . split ( "," )                                             ;
  if ( S . count ( ) < 6 ) return false                                ;
  //////////////////////////////////////////////////////////////////////
  QString   fmt = "yyyy.MM.dd hh:mm:ss"                                ;
  QString   Ts                                                         ;
  QDateTime Now                                                        ;
  QDateTime X                                                          ;
  TUID      Z                                                          ;
  double    open                                                       ;
  double    high                                                       ;
  double    low                                                        ;
  double    close                                                      ;
  int       OPEN                                                       ;
  int       HIGH                                                       ;
  int       LOW                                                        ;
  int       CLOSE                                                      ;
  TUID      volume                                                     ;
  //////////////////////////////////////////////////////////////////////
  Ts      = QString ( "%1 %2:00" ) . arg ( S [ 0 ] ) . arg ( S [ 1 ] ) ;
  X       = QDateTime::fromString   ( Ts , fmt                       ) ;
  Now     . setTimeZone             ( TZ                             ) ;
  Now     . setDate                 ( X . date ( )                   ) ;
  Now     . setTime                 ( X . time ( )                   ) ;
  Z       = Now . toTime_t          (                                ) ;
  Z      *= 1000                                                       ;
  open    = S [ 2 ] . toDouble ( )                                     ;
  high    = S [ 3 ] . toDouble ( )                                     ;
  low     = S [ 4 ] . toDouble ( )                                     ;
  close   = S [ 5 ] . toDouble ( )                                     ;
  volume  = S [ 6 ] . toInt    ( )                                     ;
  OPEN    = toInteger ( open  )                                        ;
  HIGH    = toInteger ( high  )                                        ;
  LOW     = toInteger ( low   )                                        ;
  CLOSE   = toInteger ( close )                                        ;
  volume *= 100000                                                     ;
  return Insert                                                        (
           SC                                                          ,
           Z                                                           ,
           OPEN                                                        ,
           HIGH                                                        ,
           LOW                                                         ,
           CLOSE                                                       ,
           volume                                                      ,
           Now                                                       ) ;
}

bool N::Forex::Importer::Insert (
       SqlConnection & SC       ,
       QString         table    ,
       TUID            quote    ,
       int             open     ,
       int             high     ,
       int             low      ,
       int             close    ,
       TUID            volume   )
{
  QString Q                                                     ;
  Q = QString ( "%1 into `%2`"
                " (`quote`,`open`,`high`,`low`,`close`,`volume`)"
                " values (%3,%4,%5,%6,%7,%8) ;"                 )
      . arg ( method                                            )
      . arg ( table                                             )
      . arg ( quote                                             )
      . arg ( open                                              )
      . arg ( high                                              )
      . arg ( low                                               )
      . arg ( close                                             )
      . arg ( volume                                          ) ;
  return SC . Query ( Q )                                       ;
}

bool N::Forex::Importer::InsertTick (
       SqlConnection & SC           ,
       QString         table        ,
       TUID            quote        ,
       int             bidprice     ,
       int             askprice     ,
       TUID            bidvolume    ,
       TUID            askvolume    )
{
  QString Q                                                                  ;
  Q = QString ( "%1 into `%2`"
                " (`quotetime`,`bidprice`,`askprice`,`bidvolume`,`askvolume`)"
                " values (%3,%4,%5,%6,%7) ;"                                 )
      . arg   ( method                                                       )
      . arg   ( table                                                        )
      . arg   ( quote                                                        )
      . arg   ( bidprice                                                     )
      . arg   ( askprice                                                     )
      . arg   ( bidvolume                                                    )
      . arg   ( askvolume                                                  ) ;
  return SC . Query ( Q )                                                    ;
}

bool N::Forex::Importer::InsertFlag (
       SqlConnection  & SC          ,
       QString          Table       ,
       QDateTime      & timestamp   )
{
  QString Q                                                         ;
  qint64  ts = timestamp . toTime_t ( )                             ;
  ts *= 1000                                                        ;
  Q = QString ( "insert into `%1` (`pair`,`time`) values (%2,%3) ;" )
      . arg   ( Table                                               )
      . arg   ( Pair                                                )
      . arg   ( ts                                                ) ;
  return SC . Query ( Q )                                           ;
}

bool N::Forex::Importer::Obtain (
       SqlConnection  & SC      ,
       Finance::Tick  & tick    )
{
  if ( ! SC . Next ( ) ) return false ;
  tick . Time      = SC . Tuid ( 1 )  ;
  tick . Bid       = SC . Int  ( 2 )  ;
  tick . Ask       = SC . Int  ( 3 )  ;
  tick . BidVolume = SC . Tuid ( 4 )  ;
  tick . AskVolume = SC . Tuid ( 5 )  ;
  tick . Bid      /= Multiply         ;
  tick . Ask      /= Multiply         ;
  return true                         ;
}

bool N::Forex::Importer::Obtain (
       SqlConnection  & SC      ,
       Finance::Chart & chart   )
{
  if ( ! SC . Next ( ) ) return false ;
  TUID Z                              ;
  chart . Server  = SC . Tuid ( 1 )   ;
  chart . Open    = SC . Int  ( 2 )   ;
  chart . High    = SC . Int  ( 3 )   ;
  chart . Low     = SC . Int  ( 4 )   ;
  chart . Close   = SC . Int  ( 5 )   ;
  chart . iVolume = SC . Tuid ( 6 )   ;
  chart . Volume  = chart . iVolume   ;
  chart . Open   /= Multiply          ;
  chart . High   /= Multiply          ;
  chart . Low    /= Multiply          ;
  chart . Close  /= Multiply          ;
  chart . Volume /= 100000            ;
  return true                         ;
}

bool N::Forex::Importer::Obtain (
       SqlConnection  & SC      ,
       TickUnion      & tick    )
{
  tick . id                  = SC . Tuid ( 0 ) ;
  tick . at                  = SC . Tuid ( 1 ) ;
  tick . Integer . bid       = SC . Int  ( 2 ) ;
  tick . Integer . ask       = SC . Int  ( 3 ) ;
  tick . Integer . bidvolume = SC . Tuid ( 4 ) ;
  tick . Integer . askvolume = SC . Tuid ( 5 ) ;
  return true                                  ;
}

bool N::Forex::Importer::Obtain (
       SqlConnection  & SC      ,
       PeriodUnion    & period  )
{
  period . id               = SC . Tuid ( 0 ) ;
  period . at               = SC . Tuid ( 1 ) ;
  period . Integer . open   = SC . Int  ( 2 ) ;
  period . Integer . high   = SC . Int  ( 3 ) ;
  period . Integer . low    = SC . Int  ( 4 ) ;
  period . Integer . close  = SC . Int  ( 5 ) ;
  period . Integer . volume = SC . Tuid ( 6 ) ;
  return true                                 ;
}
