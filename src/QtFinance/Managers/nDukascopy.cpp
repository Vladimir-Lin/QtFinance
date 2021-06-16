#include <qtfinance.h>

typedef struct      {
  int32_t time      ;
  int32_t bid       ;
  int32_t ask       ;
  float   bidvolume ;
  float   askvolume ;
} BI5               ;

void SwapBI5(BI5 & bi5)
{
  unsigned char * d = (unsigned char *) &bi5 ;
  unsigned char   t ;
  #define SBI5(a,b) t = d [ a ] ; d [ a ] = d [ b ] ; d [ b ] = t
  SBI5 (  0 ,  3 ) ; SBI5 (  1 ,  2 ) ;
  SBI5 (  4 ,  7 ) ; SBI5 (  5 ,  6 ) ;
  SBI5 (  8 , 11 ) ; SBI5 (  9 , 10 ) ;
  SBI5 ( 12 , 15 ) ; SBI5 ( 13 , 14 ) ;
  SBI5 ( 16 , 19 ) ; SBI5 ( 17 , 18 ) ;
  #undef SBI5
}

N::Dukascopy:: Dukascopy     ( void        )
             : Enabler       (             )
             , DukascopyMaps (             )
             , Received      ( 0           )
             , TotalFiles    ( 0           )
             , Method        ( "insert"    )
             , Identifier    ( "Dukascopy" )
             , Level         ( "B"         )
             , Period        ( 0           )
             , FromYear      ( 2001        )
             , ToYear        ( 2025        )
             , TimeZone      ( 0           )
             , CNH           ( "CNY"       )
             , TotalTicks    ( 0           )
{
}

N::Dukascopy::~Dukascopy (void)
{
}

void N::Dukascopy::setRoot (QString path)
{
  Root = QDir ( path ) ;
}

void N::Dukascopy::setTemp (QString path)
{
  Temp = QDir ( path ) ;
}

void N::Dukascopy::setZIP(QString path)
{
  Zip = QDir::toNativeSeparators(path) ;
}

void N::Dukascopy::setDecode(QString pattern)
{
  Decode = pattern ;
}

void N::Dukascopy::setData(QString feed)
{
  Datafeed = feed ;
}

QDir N::Dukascopy::TickPath(QString s,QDate t)
{
  int     yy = t . year  ( )                                         ;
  int     mm = t . month ( )                                         ;
  int     dd = t . day   ( )                                         ;
  QString YY = QString::number(yy)                                   ;
  QString MM = QString("%1%2").arg((mm-1)/10).arg((mm-1)%10)         ;
  QString DD = QString("%1%2").arg(dd/10).arg(dd%10)                 ;
  QString FF = QString("%1/%2/%3/%4").arg(s).arg(YY).arg(MM).arg(DD) ;
  QString FS = Root . absoluteFilePath ( FF )                        ;
  return QDir ( FS )                                                 ;
}

void N::Dukascopy::WriteCompletion(QString s,QDate t)
{
  QDir       d = TickPath             ( s , t        ) ;
  QString    f = d . absoluteFilePath ( "status.txt" ) ;
  QString    m                                         ;
  QByteArray B                                         ;
  m = "24:00:00"                                       ;
  B = m . toUtf8 (       )                             ;
  File::toFile   ( f , B )                             ;
}

bool N::Dukascopy::Hourly(void)
{
  QTime X = QTime::currentTime ( )       ;
  if ( X . minute ( ) < 10 ) return true ;
  if ( X . minute ( ) > 45 ) return true ;
  return false                           ;
}

bool N::Dukascopy::TickExists(QString s,QDateTime now)
{
  int     y = now . date ( ) . year  ( )                                     ;
  int     m = now . date ( ) . month ( ) - 1                                 ;
  int     d = now . date ( ) . day   ( )                                     ;
  int     h = now . time ( ) . hour  ( )                                     ;
  QString filename                                                           ;
  QString path                                                               ;
  QString full                                                               ;
  filename = QString ( "%1%2h_ticks.bi5" ) . arg ( h / 10 ) . arg ( h % 10 ) ;
  path     = QString ( "%1/%2%3/%4%5"    ) . arg ( y                         )
                                           . arg ( m / 10 ) . arg ( m % 10   )
                                           . arg ( d / 10 ) . arg ( d % 10 ) ;
  full     = QString ( "%1/%2/%3"        ) . arg ( s                         )
                                           . arg ( path                      )
                                           . arg ( filename                ) ;
  full     = Root . absoluteFilePath ( full )                                ;
  QFileInfo F ( full )                                                       ;
  return F . exists ( )                                                      ;
}

void N::Dukascopy::Setup(void)
{
  QStrings L                                                                 ;
  QStrings R                                                                 ;
  QString  S                                                                 ;
  if ( ! Root . exists ( ) ) return                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = File::StringList  ( Root . absoluteFilePath ( "lists.txt" ) , "\n" )   ;
  L = File::PurifyLines ( L                                              )   ;
  foreach ( S , L )                                                          {
    R = S . split ( " " )                                                    ;
    if ( 3 == R . count ( ) )                                                {
      QDate   D = QDate::fromString ( R[1] , "yyyy/MM/dd" )                  ;
      QString Z = R [ 0 ]                                                    ;
      Z = Z . replace ( "/" , "" )                                           ;
      Symbols << Z                                                           ;
      Digits      [ Z ] = R [ 2 ] . toInt ( )                                ;
      TimeRecords [ Z ] = D                                                  ;
      StartTime   [ Z ] = D                                                  ;
      TailTime    [ Z ] = D                                                  ;
      StartSql    [ Z ] = D                                                  ;
      TailSql     [ Z ] = D                                                  ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QString ini = Root . absoluteFilePath ( "settings.ini" )                   ;
  Settings settings ( ini , QSettings::IniFormat )                           ;
  ////////////////////////////////////////////////////////////////////////////
  settings . beginGroup ( "mysql" )                                          ;
  sql . SqlMode      = "MYSQL"                                               ;
  sql . isForeignKey = true                                                  ;
  sql . Hostname     = settings . valueString ( "Hostname" )                 ;
  sql . Username     = settings . valueString ( "Username" )                 ;
  sql . Password     = settings . valueString ( "Password" )                 ;
  sql . dbName       = settings . valueString ( "Schema"   )                 ;
  Method             = settings . valueString ( "Method"   )                 ;
  settings . endGroup   (         )                                          ;
  ////////////////////////////////////////////////////////////////////////////
  settings . beginGroup ( "dukascopy" )                                      ;
  Identifier = settings . valueString ( "Identifier" )                       ;
  Level      = settings . valueString ( "Level"      )                       ;
  Period     = settings . value       ( "Period"     ) . toInt ( )           ;
  FromYear   = settings . value       ( "From"       ) . toInt ( )           ;
  ToYear     = settings . value       ( "To"         ) . toInt ( )           ;
  TimeZone   = settings . value       ( "TimeZone"   ) . toInt ( )           ;
  CNH        = settings . valueString ( "CNH"        )                       ;
  settings . endGroup   (             )                                      ;
}

void N::Dukascopy::print(QString s)
{
}

void N::Dukascopy::addSize(qint64 size)
{
  Mutex [ 0 ] . lock   ( ) ;
  Received += size         ;
  Mutex [ 0 ] . unlock ( ) ;
}

void N::Dukascopy::addFiles(qint64 files)
{
  Mutex [ 1 ] . lock   ( ) ;
  TotalFiles += files      ;
  Mutex [ 1 ] . unlock ( ) ;
}

void N::Dukascopy::addTicks(qint64 ticks)
{
  Mutex [ 2 ] . lock   ( ) ;
  TotalTicks += ticks      ;
  Mutex [ 2 ] . unlock ( ) ;
}

bool N::Dukascopy::Get(QString symbol,QString filename,QString decoded,QByteArray & data)
{
  QString     F = QDir::toNativeSeparators ( filename )                      ;
  QString     T                                                              ;
  QString     P                                                              ;
  QStringList L                                                              ;
  QString     E                                                              ;
  QString     STDOUT                                                         ;
  QProcess    O (NULL)                                                       ;
  QDir        D ( Temp )                                                     ;
  QString     Z                                                              ;
  QFileInfo   U ( F )                                                        ;
  data . clear ( )                                                           ;
  if ( ! U . exists  ( ) ) return false                                      ;
  if ( 0 == U . size ( ) ) return true                                       ;
  ////////////////////////////////////////////////////////////////////////////
  if ( Zip . length ( ) > 0 )                                                {
    T = QDir::toNativeSeparators ( Temp.absoluteFilePath(symbol) )           ;
    P = QString(Decode).arg(Zip).arg(T).arg(F)                               ;
    L = P . split(" ")                                                       ;
    E = L [ 0 ]                                                              ;
    Z = QDir ( Temp.absoluteFilePath(symbol) ).absoluteFilePath(decoded)     ;
    D . mkpath ( Temp.absoluteFilePath(symbol) )                             ;
    L . takeAt ( 0 )                                                         ;
    QFile::remove(Z)                                                         ;
    STDOUT = D . absoluteFilePath ( QString("%1/Output.txt").arg(symbol) )   ;
    O . setStandardOutputFile ( STDOUT )                                     ;
    O . start                 ( E , L  )                                     ;
    O . waitForFinished       ( -1     )                                     ;
    //////////////////////////////////////////////////////////////////////////
    QFileInfo FI ( Z )                                                       ;
    if ( ! FI . exists ( ) ) return false                                    ;
    File::toByteArray(Z,data)                                                ;
    QFile::remove(Z)                                                         ;
  } else                                                                     {
    QByteArray Src7z                                                         ;
    File::toByteArray  ( filename , Src7z )                                  ;
    Lzma7z::Uncompress ( Src7z    , data  )                                  ;
    #ifdef XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    QMap<QString,QByteArray> SzData                                          ;
    Lzma7z::Uncompress ( filename , SzData )                                 ;
    if ( SzData . contains ( decoded ) )                                     {
      data = SzData [ decoded ]                                              ;
    }  else return false                                                     ;
    SzData . clear ( )                                                       ;
    #endif
    if ( data . size ( ) <= 0 ) return false                                 ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  BI5      * bi5                                                             ;
  int        total                                                           ;
  total = data . size ( ) / sizeof(BI5)                                      ;
  if ( total <= 0 ) return true                                              ;
  bi5 = (BI5 *) data . data ( )                                              ;
  for (int i = 0 ; i < total ; i++ ) SwapBI5 ( bi5 [ i ] )                   ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::Dukascopy::download(QString symbol,int year,int month,int day,int hour)
{
  QString    YY = QString::number(year)                                      ;
  QString    MM = QString("%1%2").arg((month-1)/10).arg((month-1)%10)        ;
  QString    DD = QString("%1%2").arg(day/10).arg(day%10)                    ;
  QString    HH = QString("%1%2").arg(hour/10).arg(hour%10)                  ;
  QString    DI = QString("%1h_ticks").arg(HH)                               ;
  QString    BI = QString("%1.bi5").arg(DI)                                  ;
  QString    FF = QString("%1/%2/%3/%4/%5").arg(symbol).arg(YY).arg(MM).arg(DD).arg(BI) ;
  QString    FS = Root . absoluteFilePath ( FF )                             ;
  QString    FZ = QString("%1.part").arg(FS)                                 ;
  QString    UU = QString("%1/%2").arg(Datafeed).arg(FF)                     ;
  QString    UA = N::XML::RandUserAgents()                                   ;
  QUrl       UL ( UU )                                                       ;
  QFile      FL ( FZ )                                                       ;
  Ftp        HTP                                                             ;
  Ftp        FTP                                                             ;
  QByteArray B                                                               ;
  HttpParser Parser                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HTP.Requests["user-agent"] = UA                                            ;
  FTP.Requests["user-agent"] = UA                                            ;
  B = HTP.header(UL,5000)                                                    ;
  if ( B . size ( ) <= 0 ) return false                                      ;
  ////////////////////////////////////////////////////////////////////////////
  Parser . setHeader ( B )                                                   ;
  addSize ( B . size ( ) )                                                   ;
  if ( 200  != Parser . statusCode ( ) ) return false                        ;
  if ( ! Parser . hasContentLength ( ) ) return false                        ;
  if (   Parser . contentLength ( ) <= 0 )                                   {
    print      ( QObject::tr("Download %1") . arg ( UU ) )                   ;
    FL . open  ( QIODevice::WriteOnly                    )                   ;
    FL . close (                                         )                   ;
    QFileInfo FI ( FS )                                                      ;
    QDir      D = FI.absoluteDir()                                           ;
    D . rename ( FZ , FS )                                                   ;
    return true                                                              ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QFileInfo FI ( FS )                                                        ;
  QDir      D = FI . absoluteDir ( )                                         ;
  D . mkpath ( D . absolutePath ( ) )                                        ;
  print ( QObject::tr("Download %1") . arg ( UU ) )                          ;
  FTP . download ( UL , FL , 45000 )                                         ;
  QFileInfo FX ( FZ )                                                        ;
  if ( ! FX . exists ( ) )                                                   {
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  addSize  ( FX . size ( ) )                                                 ;
  if (   FX . size ( ) != Parser . contentLength ( ) )                       {
    QFile::remove ( FZ )                                                     ;
    return false                                                             ;
  }                                                                          ;
  D . rename ( FZ , FS )                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  QFileInfo FU ( FS )                                                        ;
  if ( FU . exists ( ) ) addFiles ( 1 )                                      ;
  return FU . exists ( )                                                     ;
}

void N::Dukascopy::UpdateSymbol(QString symbol)
{
  QDir      D ( Root . absoluteFilePath ( symbol ) )                ;
  QString   F = D.absoluteFilePath("status.txt")                    ;
  QFileInfo I ( F )                                                 ;
  if ( ! I . exists ( ) ) return                                    ;
  QStrings  L                                                       ;
  L = File::StringList  ( F , "\n" )                                ;
  L = File::PurifyLines ( L        )                                ;
  if ( 2 != L . count ( ) ) return                                  ;
  StartTime [ symbol ] = QDate::fromString ( L [0] , "yyyy/MM/dd" ) ;
  TailTime  [ symbol ] = QDate::fromString ( L [1] , "yyyy/MM/dd" ) ;
}

void N::Dukascopy::SaveSymbol(QString symbol)
{
  QDir        D ( Root . absoluteFilePath ( symbol ) )               ;
  QString     F = D.absoluteFilePath("status.txt")                   ;
  QStringList L                                                      ;
  QString     S                                                      ;
  QByteArray  B                                                      ;
  L << StartTime [ symbol ] . toDate ( ) . toString ( "yyyy/MM/dd" ) ;
  L << TailTime  [ symbol ] . toDate ( ) . toString ( "yyyy/MM/dd" ) ;
  S  = L . join   ( "\r\n" )                                         ;
  B  = S . toUtf8 (        )                                         ;
  File::toFile    ( F , B  )                                         ;
}

bool N::Dukascopy::GetTime(QString symbol,QDateTime & Now,QDateTime & Fetch)
{
  QDate     TT = TailTime [ symbol ] . toDate ( ) ;
  QDateTime DT ( TT , QTime(0,0,0,0) )            ;
  if ( DT . secsTo ( Now ) > 86400 )              {
    DT = Now                                      ;
    DT = DT . addSecs ( -86400 )                  ;
    if ( Completion ( symbol , DT ) )             {
      DT = Now                                    ;
      if ( Completion ( symbol , DT ) )           {
        Fetch = DT                                ;
        return false                              ;
      } else                                      {
        Fetch = DT                                ;
        return ( Now.time().minute() < 20 )       ;
      }                                           ;
    } else                                        {
      Fetch = DT                                  ;
      return true                                 ;
    }                                             ;
  } else                                          {
    if ( Completion ( symbol , DT ) )             {
      Fetch = DT                                  ;
      return false                                ;
    } else                                        {
      Fetch = DT                                  ;
      return ( Now.time().minute() < 20 )         ;
    }                                             ;
  }                                               ;
  return false                                    ;
}

bool N::Dukascopy::BeforeTime(QString symbol,QDateTime & Now,QDateTime & Fetch)
{
  QDate     ST = StartTime [ symbol ] . toDate ( ) ;
  QDateTime DT ( ST , QTime(0,0,0,0) )             ;
  if ( Completion ( symbol , DT ) )                {
    Fetch = DT                                     ;
    return false                                   ;
  } else                                           {
    Fetch = DT                                     ;
    return ( Now.time().minute() > 5 )             ;
  }                                                ;
  return false                                     ;
}

bool N::Dukascopy::Completion(QString symbol,QDateTime & today)
{
  int        year  = today.date().year()                                     ;
  int        month = today.date().month()                                    ;
  int        day   = today.date().day()                                      ;
  QString    YY = QString::number(year)                                      ;
  QString    MM = QString("%1%2").arg((month-1)/10).arg((month-1)%10)        ;
  QString    DD = QString("%1%2").arg(day/10).arg(day%10)                    ;
  QString    FS = QString("%1/%2/%3/%4").arg(symbol).arg(YY).arg(MM).arg(DD) ;
  QString    FP = Root.absoluteFilePath(FS)                                  ;
  QDir       DS = QDir ( FP )                                                ;
  QString    SS = DS.absoluteFilePath("status.txt")                          ;
  QByteArray B                                                               ;
  QString    R                                                               ;
  ////////////////////////////////////////////////////////////////////////////
  DS . mkpath ( FP )                                                         ;
  QFileInfo  FI ( SS )                                                       ;
  today . setTime ( QTime(0,0,0,0) )                                         ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! FI . exists ( ) )                                                   {
    R = "00:00:00"                                                           ;
    B = R.toUtf8()                                                           ;
    File::toFile(SS,B)                                                       ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  File::toByteArray ( SS , B )                                               ;
  R = QString::fromUtf8(B)                                                   ;
  if ( "24:00:00" == R ) return true                                         ;
  QTime X = QTime::fromString(R,"hh:mm:ss")                                  ;
  today . setTime ( X )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  return false                                                               ;
}

bool N::Dukascopy::download(QString symbol,QDate today)
{
  return download ( NULL , NULL , symbol , today ) ;
}

bool N::Dukascopy::download(Thread * t,ThreadData * d,QString symbol,QDate today)
{
  if ( ( NULL != t ) && ( ! t -> IsContinue ( d ) ) ) return false           ;
  ////////////////////////////////////////////////////////////////////////////
  int        year  = today . year  ( )                                       ;
  int        month = today . month ( )                                       ;
  int        day   = today . day   ( )                                       ;
  int        hour  = 0                                                       ;
  QString    YY    = QString::number(year)                                   ;
  QString    MM    = QString("%1%2").arg((month-1)/10).arg((month-1)%10)     ;
  QString    DD    = QString("%1%2").arg(day/10).arg(day%10)                 ;
  QString    FS    = QString("%1/%2/%3/%4").arg(symbol).arg(YY).arg(MM).arg(DD) ;
  QString    FP    = Root.absoluteFilePath(FS)                               ;
  QDir       DS    = QDir ( FP )                                             ;
  QString    SS    = DS.absoluteFilePath("status.txt")                       ;
  QString    HH                                                              ;
  QString    DI                                                              ;
  QString    BI                                                              ;
  QString    FI                                                              ;
  QByteArray BB                                                              ;
  bool       miss = false                                                    ;
  bool       drop = false                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  for (int i = 0 ; ( ! drop ) && ( ! miss ) && ( i < 24 ) ; i++ )            {
    if ( ( NULL != t ) && ( ! t -> IsContinue ( d ) ) )                      {
      drop = true                                                            ;
      continue                                                               ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    HH = QString("%1%2").arg(i/10).arg(i%10)                                 ;
    DI = QString("%1h_ticks").arg(HH)                                        ;
    BI = QString("%1.bi5").arg(DI)                                           ;
    FI = DS . absoluteFilePath ( BI )                                        ;
    QFileInfo FX ( FI )                                                      ;
    //////////////////////////////////////////////////////////////////////////
    if ( ! FX . exists ( ) )                                                 {
      bool creating = false                                                  ;
      if ( 6 == today . dayOfWeek ( ) )                                      {
        creating = true                                                      ;
      }                                                                      ;
      if ( 7 == today . dayOfWeek ( ) )                                      {
        creating = true                                                      ;
      }                                                                      ;
      if ( ! creating ) miss = true ; else                                   {
        QFile FC ( FI )                                                      ;
        if ( FC . open ( QIODevice::WriteOnly ) )                            {
          FC . close ( )                                                     ;
          hour++                                                             ;
        } else miss = true                                                   ;
      }                                                                      ;
    } else hour++                                                            ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( miss )                                                                {
    if ( ( NULL != t ) && ( ! t -> IsContinue ( d ) ) ) return false         ;
    if ( download ( symbol , year , month , day , hour ) )                   {
      hour++                                                                 ;
      FI = QString("%1%2:00:00").arg(hour/10).arg(hour%10)                   ;
      BB = FI . toUtf8 (         )                                           ;
      File::toFile     ( SS , BB )                                           ;
      return true                                                            ;
    }                                                                        ;
  } else                                                                     {
    FI = "24:00:00"                                                          ;
    BB = FI . toUtf8 (         )                                             ;
    File::toFile     ( SS , BB )                                             ;
    return true                                                              ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return false                                                               ;
}

void N::Dukascopy::Check(QString symbol)
{
  QDateTime DT                                                               ;
  QDateTime FT                                                               ;
  bool      updated = false                                                  ;
  bool      correct = false                                                  ;
  DT . setTimeSpec        ( Qt::UTC                                        ) ;
  DT . setMSecsSinceEpoch ( nTimeNow . toMSecsSinceEpoch()                 ) ;
  UpdateSymbol            ( symbol                                         ) ;
  if                      ( GetTime    ( symbol , DT , FT                ) ) {
    if                    ( download   ( symbol , FT . date ( )          ) ) {
      updated = true                                                         ;
      if ( Completion ( symbol , FT ) )                                      {
        FT . setTime      ( QTime(0,0,0,0) )                                 ;
        FT = FT . addSecs ( 86400          )                                 ;
      }                                                                      ;
      TailTime [ symbol ] = FT . date ( )                                    ;
    }                                                                        ;
  }                                                                          ;
  correct = ( TailTime [ symbol ] > StartTime [ symbol ] )                   ;
  if ( updated ) correct = false                                             ;
  if                      ( correct && BeforeTime ( symbol,DT, FT )        ) {
    if ( download         ( symbol , FT . date ( )                       ) ) {
      if ( Completion ( symbol , FT ) )                                      {
        FT . setTime      ( QTime(0,0,0,0) )                                 ;
        FT = FT . addSecs ( 86400          )                                 ;
      }                                                                      ;
      StartTime [ symbol ] = FT . date ( )                                   ;
    }                                                                        ;
  }                                                                          ;
  SaveSymbol              ( symbol                                         ) ;
}

void N::Dukascopy::UpdateSql(QString symbol)
{
  QDir      D ( Root . absoluteFilePath ( symbol ) )               ;
  QString   F = D.absoluteFilePath("sql.txt")                      ;
  QFileInfo I ( F )                                                ;
  if ( ! I . exists ( ) ) return                                   ;
  QStrings  L                                                      ;
  L = File::StringList  ( F , "\n" )                               ;
  L = File::PurifyLines ( L        )                               ;
  if ( 2 != L . count ( ) ) return                                 ;
  StartSql [ symbol ] = QDate::fromString ( L [0] , "yyyy/MM/dd" ) ;
  TailSql  [ symbol ] = QDate::fromString ( L [1] , "yyyy/MM/dd" ) ;
}

void N::Dukascopy::SaveSql(QString symbol)
{
  QDir        D ( Root . absoluteFilePath ( symbol ) )              ;
  QString     F = D.absoluteFilePath("sql.txt")                     ;
  QStringList L                                                     ;
  QString     S                                                     ;
  QByteArray  B                                                     ;
  L << StartSql [ symbol ] . toDate ( ) . toString ( "yyyy/MM/dd" ) ;
  L << TailSql  [ symbol ] . toDate ( ) . toString ( "yyyy/MM/dd" ) ;
  S  = L . join   ( "\r\n" )                                        ;
  B  = S . toUtf8 (        )                                        ;
  File::toFile    ( F , B  )                                        ;
}

bool N::Dukascopy::GetSql(QString symbol,QDateTime & Now,QDateTime & Fetch)
{
  QDate     TT = TailSql [ symbol ] . toDate ( ) ;
  QDateTime DT ( TT , QTime(0,0,0,0) )           ;
  if ( DT . secsTo ( Now ) > 86400 )             {
    DT = Now                                     ;
    DT = DT . addSecs ( -86400 )                 ;
    if ( CompleteSql ( symbol , DT ) )           {
      DT = Now                                   ;
      if ( CompleteSql ( symbol , DT ) )         {
        Fetch = DT                               ;
        return false                             ;
      } else                                     {
        Fetch = DT                               ;
//        return ( Now.time().minute() < 20 )      ;
        return true                              ;
      }                                          ;
    } else                                       {
      Fetch = DT                                 ;
//      return ( Now.time().minute() < 20 )        ;
      return true                              ;
    }                                            ;
  } else                                         {
    if ( CompleteSql ( symbol , DT ) )           {
      Fetch = DT                                 ;
      return false                               ;
    } else                                       {
      Fetch = DT                                 ;
      return true                                ;
    }                                            ;
  }                                              ;
  return false                                   ;
}

bool N::Dukascopy::BeforeSql(QString symbol,QDateTime & Now,QDateTime & Fetch)
{
  QDate     ST = StartSql [ symbol ] . toDate ( ) ;
  QDateTime DT ( ST , QTime(0,0,0,0) )            ;
  if ( CompleteSql ( symbol , DT ) )              {
    Fetch = DT                                    ;
    return false                                  ;
  } else                                          {
    Fetch = DT                                    ;
    return ( Now.time().minute() > 5 )            ;
  }                                               ;
  return false                                    ;
}

bool N::Dukascopy::CompleteSql(QString symbol,QDateTime & today)
{
  int        year  = today.date().year()                                     ;
  int        month = today.date().month()                                    ;
  int        day   = today.date().day()                                      ;
  QString    YY = QString::number(year)                                      ;
  QString    MM = QString("%1%2").arg((month-1)/10).arg((month-1)%10)        ;
  QString    DD = QString("%1%2").arg(day/10).arg(day%10)                    ;
  QString    FS = QString("%1/%2/%3/%4").arg(symbol).arg(YY).arg(MM).arg(DD) ;
  QString    FP = Root.absoluteFilePath(FS)                                  ;
  QDir       DS = QDir ( FP )                                                ;
  QString    SS = DS.absoluteFilePath("sql.txt")                             ;
  QByteArray B                                                               ;
  QString    R                                                               ;
  DS . mkpath ( FP )                                                         ;
  QFileInfo  FI ( SS )                                                       ;
  ////////////////////////////////////////////////////////////////////////////
  today . setTime ( QTime(0,0,0,0) )                                         ;
  if ( ! FI . exists ( ) )                                                   {
    R = "00:00:00"                                                           ;
    B = R.toUtf8 (        )                                                  ;
    File::toFile ( SS , B )                                                  ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  File::toByteArray(SS,B)                                                    ;
  R = QString::fromUtf8(B)                                                   ;
  if ( "24:00:00" == R ) return true                                         ;
  QTime X = QTime::fromString(R,"hh:mm:ss")                                  ;
  today . setTime ( X )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  return false                                                               ;
}

bool N::Dukascopy::import(QString symbol,int year,int month,int day,int hour)
{
  QString    YY = QString::number(year)                                      ;
  QString    MM = QString("%1%2").arg((month-1)/10).arg((month-1)%10)        ;
  QString    DD = QString("%1%2").arg(day/10).arg(day%10)                    ;
  QString    HH = QString("%1%2").arg(hour/10).arg(hour%10)                  ;
  QString    DI = QString("%1h_ticks").arg(HH)                               ;
  QString    BI = QString("%1.bi5").arg(DI)                                  ;
  QString    FF = QString("%1/%2/%3/%4/%5").arg(symbol).arg(YY).arg(MM).arg(DD).arg(BI) ;
  QString    FS = Root . absoluteFilePath ( FF )                             ;
  QFileInfo  FI ( FS )                                                       ;
  QByteArray BB                                                              ;
  bool       correct = false                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! FI . exists ( )  ) return false                                     ;
  if ( FI . size ( ) <= 0 ) return true                                      ;
  if ( ! Get ( symbol , FS , DI , BB ) ) return false                        ;
  ////////////////////////////////////////////////////////////////////////////
  BI5      * bi5                                                             ;
  int        total                                                           ;
  int        multiply = 1                                                    ;
  int        digits   = Digits [ symbol ]                                    ;
  total = BB.size() / sizeof(BI5)                                            ;
  if ( total <= 0 ) return true                                              ;
  for (int i=0 ; i < digits ; i++ ) multiply *= 10                           ;
  bi5 = (BI5 *) BB . data ( )                                                ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! Initialized . contains ( symbol ) ) Initialized [ symbol ] = false  ;
  SqlConnection SC ( sql )                                                   ;
  if ( SC . open ( QtUUID::createUuidString() , QString ( __FUNCTION__ ) ) ) {
    Forex::Importer fi                                                       ;
    QString         s = symbol                                               ;
    QString         m                                                        ;
    int             level = Level.at(0).toLatin1() - 'A'                     ;
    s . insert ( 3 , "/" )                                                   ;
    s = s . replace ( "CNH" , CNH )                                          ;
    fi . identifier = Identifier                                             ;
    fi . symbol     = s                                                      ;
    fi . FromYear   = FromYear                                               ;
    fi . ToYear     = ToYear                                                 ;
    fi . method     = Method                                                 ;
    fi . setDigits ( Digits [ symbol ] )                                     ;
    fi . setLevel  ( level             )                                     ;
    fi . setPeriod ( Period            )                                     ;
    if ( ( ! Initialized [ symbol ] ) && ( ! fi . PeriodExists ( SC ) ) )    {
      m = QString("Create tables for %1").arg(symbol)                        ;
      print ( m )                                                            ;
      fi . CreatePeriods ( SC )                                              ;
    }                                                                        ;
    Initialized [ symbol ] = true                                            ;
    m = QString("Importing %1 ticks from %2").arg(total,5,10,QChar(' ')).arg(FS) ;
    print ( m )                                                              ;
    for (int i=0;i<total;i++)                                                {
      Finance::Tick tick                                                     ;
      QDateTime     DT ( QDate(year,month,day) , QTime(hour,0,0) , Qt::UTC ) ;
      float         bid                                                      ;
      float         ask                                                      ;
      DT  = DT.addMSecs(bi5[i].time)                                         ;
      bid = bi5 [ i ] . bid ; bid /= multiply                                ;
      ask = bi5 [ i ] . ask ; ask /= multiply                                ;
      tick . symbol    = s                                                   ;
      tick . Time      = DT . toMSecsSinceEpoch ( )                          ;
      tick . Time     += TimeZone                                            ;
      tick . TimeZone  = TimeZone                                            ;
      tick . Bid       = bid                                                 ;
      tick . Ask       = ask                                                 ;
      tick . Mid       = ( bid + ask ) / 2                                   ;
      tick . BidVolume = bi5 [ i ] . bidvolume                               ;
      tick . AskVolume = bi5 [ i ] . askvolume                               ;
      tick . iBid      = bi5 [ i ] . bid                                     ;
      tick . iAsk      = bi5 [ i ] . ask                                     ;
      tick . iMid      = ( tick . iBid + tick . iAsk ) / 2                   ;
      fi   . Insert ( SC , tick )                                            ;
    }                                                                        ;
    addTicks ( total )                                                       ;
    correct = true                                                           ;
    SC . close (  )                                                          ;
  }                                                                          ;
  SC . remove ( )                                                            ;
  ////////////////////////////////////////////////////////////////////////////
  return correct                                                             ;
}

bool N::Dukascopy::import(QString symbol,QDate today)
{
  int        year  = today . year  ( )                                       ;
  int        month = today . month ( )                                       ;
  int        day   = today . day   ( )                                       ;
  int        hour  = 0                                                       ;
  QString    YY    = QString::number(year)                                   ;
  QString    MM    = QString("%1%2").arg((month-1)/10).arg((month-1)%10)     ;
  QString    DD    = QString("%1%2").arg(day/10).arg(day%10)                 ;
  QString    FS    = QString("%1/%2/%3/%4").arg(symbol).arg(YY).arg(MM).arg(DD) ;
  QString    FP    = Root.absoluteFilePath(FS)                               ;
  QDir       DS    = QDir ( FP )                                             ;
  QString    SS    = DS.absoluteFilePath("sql.txt")                          ;
  QString    HH                                                              ;
  QString    DI                                                              ;
  QString    BI                                                              ;
  QString    FI                                                              ;
  QByteArray BB                                                              ;
  bool       miss = false                                                    ;
  QFileInfo  FX ( SS )                                                       ;
  ////////////////////////////////////////////////////////////////////////////
  if ( FX . exists ( ) )                                                     {
    File::toByteArray ( SS , BB )                                            ;
    FI = QString::fromUtf8(BB)                                               ;
    if ( "24:00:00" == FI )                                                  {
      return true                                                            ;
    } else                                                                   {
      QTime TX                                                               ;
      TX = QTime::fromString(FI,"hh:mm:ss")                                  ;
      hour = TX . hour ( )                                                   ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! miss )                                                              {
    HH = QString("%1%2").arg(hour/10).arg(hour%10)                           ;
    DI = QString("%1h_ticks").arg(HH)                                        ;
    BI = QString("%1.bi5").arg(DI)                                           ;
    FI = DS.absoluteFilePath(BI)                                             ;
    QFileInfo FX ( FI )                                                      ;
    if ( ! FX . exists ( ) ) return false                                    ;
    if ( import(symbol,year,month,day,hour) )                                {
      hour++                                                                 ;
      FI = QString("%1%2:00:00").arg(hour/10).arg(hour%10)                   ;
      BB = FI . toUtf8 ( )                                                   ;
      File::toFile ( SS , BB )                                               ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return false                                                               ;
}

void N::Dukascopy::Import(QString symbol)
{
  QDateTime DT                                                               ;
  QDateTime FT                                                               ;
  bool      updated = false                                                  ;
  bool      correct = false                                                  ;
  DT . setTimeSpec        ( Qt::UTC                                        ) ;
  DT . setMSecsSinceEpoch ( nTimeNow . toMSecsSinceEpoch()                 ) ;
  UpdateSql               ( symbol                                         ) ;
  if                      ( GetSql     ( symbol , DT , FT                ) ) {
    if                    ( import     ( symbol , FT . date ( )          ) ) {
      updated = true                                                         ;
      if ( CompleteSql ( symbol , FT ) )                                     {
        FT . setTime      ( QTime(0,0,0,0) )                                 ;
        FT = FT . addSecs ( 86400          )                                 ;
      }                                                                      ;
      TailSql [ symbol ] = FT . date ( )                                     ;
    }                                                                        ;
  }                                                                          ;
  correct = ( TailSql [ symbol ] > StartSql [ symbol ] )                     ;
  if ( updated ) correct = false                                             ;
  if                      ( correct && BeforeSql ( symbol,DT, FT )         ) {
    if ( import           ( symbol , FT . date ( )                       ) ) {
      if ( CompleteSql    ( symbol , FT                                  ) ) {
        FT . setTime      ( QTime(0,0,0,0) )                                 ;
        FT = FT . addSecs ( 86400          )                                 ;
      }                                                                      ;
      StartSql [ symbol ] = FT . date ( )                                    ;
    }                                                                        ;
  }                                                                          ;
  SaveSql                 ( symbol                                         ) ;
}

bool N::Dukascopy::Investigate(QDir & d,QDate t)
{
  bool zero    = false                                 ;
  bool correct = true                                  ;
  int  dow     = t . dayOfWeek ( )                     ;
  bool removal                                         ;
  zero = ( ( 6 == dow ) || ( 7 == dow ) )              ;
  for (int i=0;i<24;i++)                               {
    QString   HH = QString("%1%2").arg(i/10).arg(i%10) ;
    QString   DI = QString("%1h_ticks").arg(HH)        ;
    QString   BI = QString("%1.bi5").arg(DI)           ;
    QString   FF = d . absoluteFilePath ( BI )         ;
    QFileInfo FI ( FF )                                ;
    if ( ! FI . exists ( ) ) correct = false           ;
    removal = false                                    ;
    if ( ( ! zero ) && ( 0 == FI . size ( ) ) )        {
      removal = true                                   ;
    }                                                  ;
    if ( ( 5 == dow ) && ( i > 19 ) )                  {
      removal = false                                  ;
    }                                                  ;
    if ( 1  == t . month ( ) )                         {
      if ( 1  == t . day ( ) ) removal = false         ;
      if ( 2  == t . day ( ) ) removal = false         ;
    }                                                  ;
    if ( 12 == t . month ( ) )                         {
      if ( 25 == t . day ( ) ) removal = false         ;
      if ( 26 == t . day ( ) ) removal = false         ;
      if ( 31 == t . day ( ) ) removal = false         ;
    }                                                  ;
    if ( removal )                                     {
//      QFile::remove ( FF )                             ;
//      correct = false                                  ;
    }                                                  ;
  }                                                    ;
  return correct                                       ;
}

bool N::Dukascopy::Investigate(QString s,QDate t)
{
  QDir d = TickPath  ( s , t ) ;
  return Investigate ( d , t ) ;
}

void N::Dukascopy::ToCSV(QString s,QDate t,QFile & f)
{
  QDir d        = TickPath ( s , t )                                         ;
  int  digits   = Digits [ s ]                                               ;
  int  multiply = 1                                                          ;
  for (int i=0;i<digits;i++) multiply *= 10                                  ;
  for (int i=0;i<24;i++)                                                     {
    QString   HH = QString("%1%2").arg(i/10).arg(i%10)                       ;
    QString   DI = QString("%1h_ticks").arg(HH)                              ;
    QString   BI = QString("%1.bi5").arg(DI)                                 ;
    QString   FS = d . absoluteFilePath ( BI )                               ;
    QFileInfo FI ( FS )                                                      ;
    if ( FI . exists ( ) )                                                   {
      QByteArray BB                                                          ;
      if ( Get ( s , FS , DI , BB ) )                                        {
        BI5    * bi5                                                         ;
        int      total = BB . size ( ) / sizeof(BI5)                         ;
        QString  m                                                           ;
        float    bid                                                         ;
        float    ask                                                         ;
        if ( total > 0 )                                                     {
          QStringList R                                                      ;
          bi5 = (BI5 *) BB . data ( )                                        ;
          for (int j=0;j<total;j++)                                          {
            QDateTime DT ( t , QTime(i,0,0) , Qt::UTC )                      ;
            DT   = DT . addMSecs ( bi5 [ j ] . time )                        ;
            bid  = bi5 [ j ] . bid                                           ;
            bid /= multiply                                                  ;
            ask  = bi5 [ j ] . ask                                           ;
            ask /= multiply                                                  ;
            m    = QString ( "%1,%2,%3,%4,%5"                                )
                  . arg    ( DT . toString ( "yyyy.MM.dd hh:mm:ss.zzz" )     )
                  . arg    ( bid                                             )
                  . arg    ( ask                                             )
                  . arg    ( bi5 [ j ] . bidvolume                           )
                  . arg    ( bi5 [ j ] . askvolume                         ) ;
            R << m                                                           ;
          }                                                                  ;
          if ( R . count ( ) > 0 )                                           {
            m  = R . join   ( "\r\n" )                                       ;
            m      . append ( "\r\n" )                                       ;
            BB = m . toUtf8 (        )                                       ;
            f      . write  ( BB     )                                       ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
  }                                                                          ;
}

void N::Dukascopy::ListStatus(QDir & DIR,QString filename,QStringList & R)
{
  if ( R . count ( ) <= 0 ) return                                ;
  if ( R . count ( ) >  0 ) qSort ( R . begin ( ) , R . end ( ) ) ;
  QString    f                                                    ;
  QString    m                                                    ;
  QByteArray B                                                    ;
  f = DIR . absoluteFilePath ( filename )                         ;
  m = R   . join             ( "\r\n"   )                         ;
  B = m   . toUtf8           (          )                         ;
  File::toFile               ( f , B    )                         ;
}

void N::Dukascopy::SqlStatus(QDir & DIR,QStringList & R)
{
  ListStatus ( DIR , "sql.txt" , R ) ;
}

void N::Dukascopy::CsvStatus(QDir & SYM,QStringList & R)
{
  ListStatus ( SYM , "status.txt" , R ) ;
}
