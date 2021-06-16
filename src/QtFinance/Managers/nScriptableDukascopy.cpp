#include <qtfinance.h>

typedef struct      {
  int32_t time      ;
  int32_t bid       ;
  int32_t ask       ;
  float   bidvolume ;
  float   askvolume ;
} BI5               ;

QScriptValue N::DukascopyCreate  (QScriptContext * context,QScriptEngine * engine)
{
  N::ScriptableDukascopy * mt = new N::ScriptableDukascopy ( engine ) ;
  return engine -> newQObject ( mt )                                  ;
}

N::ScriptableDukascopy:: ScriptableDukascopy ( QObject * parent )
                       : QObject             (           parent )
                       , QScriptable         (                  )
                       , Thread              ( 0 , false        )
                       , Dukascopy           (                  )
{
  setEnabled ( "MergeCSV"    , true )          ;
  setEnabled ( "SeparateCSV" , true )          ;
  setEnabled ( "LastestSql"  , true )          ;
  Values     [ "Download"           ] = 0      ;
  Values     [ "SQL"                ] = 0      ;
  Values     [ "CSV"                ] = 0      ;
  Values     [ "Import"             ] = 0      ;
  Values     [ "Optimize"           ] = 0      ;
  Values     [ "Investigate"        ] = 0      ;
  Values     [ "CsvToSql"           ] = 0      ;
  Values     [ "Bi5ToSql"           ] = 0      ;
  Values     [ "CsvBlocks"          ] = 100000 ;
  Values     [ "Files"              ] = 0      ;
  Values     [ "MappingThreads"     ] = 12     ;
  Values     [ "ImportThreads"      ] =  8     ;
  Values     [ "InvestigateThreads" ] =  8     ;
  Values     [ "OptimizeThreads"    ] =  4     ;
  Values     [ "RealTimeThreads"    ] =  7     ;
}

N::ScriptableDukascopy::~ScriptableDukascopy (void)
{
  finalize ( 200 ) ;
}

bool N::ScriptableDukascopy::SetEnabled(int Id,bool enable)
{
  return setEnabled ( Id , enable ) ;
}

bool N::ScriptableDukascopy::HasEnabled(int Id)
{
  return hasEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::IsEnabled(int Id)
{
  return isEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::SetEnabled(SUID Id,bool enable)
{
  return setEnabled ( Id , enable ) ;
}

bool N::ScriptableDukascopy::HasEnabled(SUID Id)
{
  return hasEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::IsEnabled(SUID Id)
{
  return isEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::SetEnabled(QString Id,bool enable)
{
  return setEnabled ( Id , enable ) ;
}

bool N::ScriptableDukascopy::HasEnabled(QString Id)
{
  return hasEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::IsEnabled(QString Id)
{
  return isEnabled ( Id ) ;
}

bool N::ScriptableDukascopy::IsAllTrue(void)
{
  return isAllTrue ( ) ;
}

bool N::ScriptableDukascopy::IsAllFalse(void)
{
  return isAllFalse ( ) ;
}

bool N::ScriptableDukascopy::CanLock(void)
{
  return canLock ( ) ;
}

bool N::ScriptableDukascopy::SetLock(bool lock)
{
  return setLock ( lock ) ;
}

void N::ScriptableDukascopy::CleanEnabler(void)
{
  ClearEnabler ( ) ;
}

void N::ScriptableDukascopy::CleanSymbols(void)
{
  Symbols . clear ( ) ;
}

void N::ScriptableDukascopy::AddSymbol(QString symbol)
{
  Symbols << symbol ;
}

int N::ScriptableDukascopy::AllSymbols(void)
{
  return Symbols . count ( ) ;
}

QString N::ScriptableDukascopy::SymbolAt(int index)
{
  if ( index >= Symbols . count ( ) ) return "" ;
  return Symbols [ index ]                      ;
}

qint64 N::ScriptableDukascopy::KeyValue(QString key)
{
  return Values [ key ] ;
}

qint64 N::ScriptableDukascopy::setKeyValue(QString key,qint64 value)
{
  Values [ key ] = value ;
  return Values [ key ] ;
}

void N::ScriptableDukascopy::ThreadEvent(void)
{
  qApp -> processEvents ( ) ;
}

void N::ScriptableDukascopy::print(QString s)
{
  N::printf ( s , true , true ) ;
}

void N::ScriptableDukascopy::setDirectory(QString path)
{
  setRoot ( path ) ;
}

void N::ScriptableDukascopy::setTemporary(QString path)
{
  setTemp ( path ) ;
}

void N::ScriptableDukascopy::setZip(QString path)
{
  setZIP ( path ) ;
}

void N::ScriptableDukascopy::setPattern(QString pattern)
{
  setDecode ( pattern ) ;
}

void N::ScriptableDukascopy::setDataFeed(QString feed)
{
  setData ( feed ) ;
}

void N::ScriptableDukascopy::Initialize(void)
{
  Setup ( ) ;
}

void N::ScriptableDukascopy::Take(QString symbol)
{
  int index                       ;
  index = Symbols.indexOf(symbol) ;
  if ( index < 0 ) return         ;
  Symbols . takeAt ( index )      ;
}

void N::ScriptableDukascopy::TakeFirst(int count)
{
  for (int i=0;i<count;i++) {
    Symbols . takeFirst ( ) ;
  }                         ;
}

void N::ScriptableDukascopy::TakeLast(int count)
{
  for (int i=0;i<count;i++) {
    Symbols . takeLast ( )  ;
  }                         ;
}

void N::ScriptableDukascopy::Tell(QString symbol,int year,int month,int day,int hour,int digits)
{
  QString    YY = QString::number(year)                                      ;
  QString    MM = QString("%1%2").arg((month-1)/10).arg((month-1)%10)        ;
  QString    DD = QString("%1%2").arg(day/10).arg(day%10)                    ;
  QString    HH = QString("%1%2").arg(hour/10).arg(hour%10)                  ;
  QString    DI = QString("%1h_ticks").arg(HH)                               ;
  QString    BI = QString("%1.bi5").arg(DI)                                  ;
  QString    FF = QString("%1/%2/%3/%4/%5").arg(symbol).arg(YY).arg(MM).arg(DD).arg(BI) ;
  QString    FS = Root . absoluteFilePath ( FF )                             ;
  QByteArray BB                                                              ;
  BI5      * bi5                                                             ;
  int        total                                                           ;
  int        multiply = 1                                                    ;
  QString    m                                                               ;
  float      bid                                                             ;
  float      ask                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! Get ( symbol , FS , DI , BB ) ) return                              ;
  total = BB.size() / sizeof(BI5)                                            ;
  if ( total <= 0 ) return                                                   ;
  for (int i=0;i<digits;i++) multiply *= 10                                  ;
  ////////////////////////////////////////////////////////////////////////////
  bi5 = (BI5 *) BB . data ( )                                                ;
  for (int i=0;i<total;i++)                                                  {
    QDateTime DT ( QDate(year,month,day) , QTime(hour,0,0) , Qt::UTC )       ;
    DT  = DT . addMSecs ( bi5 [ i ] . time )                                 ;
    bid = bi5[i].bid ; bid /= multiply                                       ;
    ask = bi5[i].ask ; ask /= multiply                                       ;
    m   = QString ( "%1,%2,%3,%4,%5"                                         )
          . arg   ( DT . toString ( "yyyy/MM/dd hh:mm:ss.zzz" )              )
          . arg   ( bid                                                      )
          . arg   ( ask                                                      )
          . arg   ( bi5 [ i ] . bidvolume                                    )
          . arg   ( bi5 [ i ] . askvolume                                  ) ;
    print ( m )                                                              ;
  }                                                                          ;
}

void N::ScriptableDukascopy::Download(QString symbol,int year,int month,int day,int hour)
{
  download ( symbol , year , month , day , hour ) ;
}

void N::ScriptableDukascopy::Import(QString symbol,int year,int month,int day,int hour)
{
  import ( symbol , year , month , day , hour ) ;
}

void N::ScriptableDukascopy::run(int T,ThreadData * data)
{
  if ( ! IsContinue ( data ) ) return                                ;
  switch ( T )                                                       {
    case 10001                                                       :
      Dukascopy::Check    ( data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Download"    ] --                                    ;
    break                                                            ;
    case 10002                                                       :
      Dukascopy::Import   ( data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "SQL"         ] --                                    ;
    break                                                            ;
    case 10003                                                       :
      Build               ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Import"      ] --                                    ;
    break                                                            ;
    case 10004                                                       :
      Optimize                                                       (
                            data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Optimize"    ] --                                    ;
    break                                                            ;
    case 10005                                                       :
      Investigate                                                    (
                            data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Investigate" ] --                                    ;
    break                                                            ;
    case 10006                                                       :
      ToCSV               ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "CSV"         ] --                                    ;
    break                                                            ;
    case 10007                                                       :
      CsvToSql            ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "CsvToSql"    ] --                                    ;
    break                                                            ;
    case 20001                                                       :
      PeriodicOptimize    ( data                                   ) ;
    break                                                            ;
    case 20002                                                       :
      PeriodicInvestigate ( data                                   ) ;
    break                                                            ;
    case 21001                                                       :
      Building            ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Importing" ] --                                      ;
    break                                                            ;
    case 21002                                                       :
      Mapping             ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "Mapping"   ] --                                      ;
    break                                                            ;
    case 21003                                                       :
      RealTime            ( data                                     ,
                            data -> Arguments [ 0 ] . toString ( ) ) ;
      Values [ "RealTime"  ] --                                      ;
    break                                                            ;
  }                                                                  ;
}

void N::ScriptableDukascopy::Report(QDateTime Start,QDateTime & Now,QString section)
{
  if ( Now . secsTo ( nTimeNow ) < 60 ) return                           ;
  int     ds   = Start.secsTo(nTimeNow)                                  ;
  int     days = ds / 86400                                              ;
  qint64  bps  = 0                                                       ;
  QTime   T ( 0 , 0 , 0 , 0 )                                            ;
  QString m                                                              ;
  T = T . addSecs ( ds % 86400 )                                         ;
  if ( ds > 0 ) bps = Received / ds                                      ;
  m = tr ("%1 days %2, %3 bytes %4 files downloaded, BPS %5, %6 threads" )
      . arg ( days                                                       )
      . arg ( T . toString ( "hh:mm:ss" )                                )
      . arg ( Received                                                   )
      . arg ( TotalFiles                                                 )
      . arg ( bps                                                        )
      . arg ( Values [ section ]                                       ) ;
  print ( m )                                                            ;
  Now = nTimeNow                                                         ;
}

void N::ScriptableDukascopy::ReportSql(QDateTime Start,QDateTime & Now,QString section)
{
  if ( Now . secsTo ( nTimeNow ) < 2 ) return                     ;
  int     ds    = Start . secsTo ( nTimeNow )                     ;
  int     days  = ds / 86400                                      ;
  qint64  total = 0                                               ;
  qint64  tps   = 0                                               ;
  QTime   T ( 0 , 0 , 0 , 0 )                                     ;
  QString m                                                       ;
  foreach ( m , Symbols )                                         {
    if ( Values . contains ( m ) )                                {
      total += Values [ m ]                                       ;
    }                                                             ;
  }                                                               ;
  T = T . addSecs ( ds % 86400 )                                  ;
  if ( ds > 0 ) tps = ( total / ds )                              ;
  m = tr    ( "%1 days %2, %3 ticks, %4 t/s, %5 files, %6 threads")
      . arg ( days                                                )
      . arg ( T . toString ( "hh:mm:ss" )                         )
      . arg ( total                                               )
      . arg ( tps                                                 )
      . arg ( Values [ "Files" ]                                  )
      . arg ( Values [ section ]                                ) ;
  N::printf ( m , false , true                                  ) ;
  Now = nTimeNow                                                  ;
}

void N::ScriptableDukascopy::Investigate(ThreadData * data,QString s)
{
  QTime       zero ( 0 , 0 , 0 , 0 )                                         ;
  QDate       from = TimeRecords [ s ] . toDate   (              )           ;
  QString     fs   = from              . toString ( "yyyy/MM/dd" )           ;
  QDateTime   ft   = QDateTime ( from , zero )                               ;
  QDateTime   tt   = nTimeNow                                                ;
  QStringList lost                                                           ;
  QString     m                                                              ;
  ////////////////////////////////////////////////////////////////////////////
  m = tr("Investigate %1 from %2 to the lastest").arg(s).arg(fs)             ;
  print ( m )                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  tt = tt . addSecs ( -(23*3600) )                                           ;
  while ( IsContinue ( data ) && ( tt > ft ) )                               {
    if ( Dukascopy::Investigate  ( s , ft . date ( ) ) )                     {
      WriteCompletion ( s , ft . date ( ) )                                  ;
    } else                                                                   {
      bool c = true                                                          ;
      for (int i=0;c && i<24;i++) c = download ( s , ft . date ( ) )         ;
      if ( Dukascopy::Investigate  ( s , ft . date ( ) ) )                   {
        WriteCompletion ( s , ft . date ( ) )                                ;
      } else                                                                 {
        m = ft . date ( ) . toString ( "yyyy/MM/dd" )                        ;
        lost << m                                                            ;
      }                                                                      ;
    }                                                                        ;
    ft = ft . addSecs ( 86400      )                                         ;
    tt = nTimeNow                                                            ;
    tt = tt . addSecs ( -(31*3600) )                                         ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QDir    d = Root . absoluteFilePath ( s             )                      ;
  QString f = d    . absoluteFilePath ( "missing.txt" )                      ;
  ////////////////////////////////////////////////////////////////////////////
  if ( lost . count( ) <= 0 )                                                {
    QByteArray B                                                             ;
    QDir       S                                                             ;
    m = tr("%1 is downloaded completely.").arg(s)                            ;
    print         ( m )                                                      ;
    QFile::remove ( f )                                                      ;
    ft = ft      . addSecs             ( -86400       )                      ;
    m  = ft      . date ( ) . toString ( "yyyy/MM/dd" )                      ;
    lost << m                                                                ;
    m = nTimeNow . date ( ) . toString ( "yyyy/MM/dd" )                      ;
    lost << m                                                                ;
    m = lost . join             ( "\r\n"       )                             ;
    B = m    . toUtf8           (              )                             ;
    S = Root . absoluteFilePath ( s            )                             ;
    f = S    . absoluteFilePath ( "status.txt" )                             ;
    File::toFile                ( f , B        )                             ;
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int        c = lost . count            (               )                   ;
  QByteArray B                                                               ;
  m = lost . join   ( "\r\n" )                                               ;
  B = m    . toUtf8 (        )                                               ;
  m = tr("%1 has %2 days missing, writing %3").arg(s).arg(c).arg(f)          ;
  print             ( m      )                                               ;
  File::toFile      ( f , B  )                                               ;
}

void N::ScriptableDukascopy::ToCSV(ThreadData * data,QString s)
{
  QTime       zero ( 0 , 0 , 0 , 0 )                                         ;
  QDate       from  = TimeRecords [ s ] . toDate  (              )           ;
  QDir        d     = Root     . absoluteFilePath ( s            )           ;
  QString     f     = d        . absoluteFilePath ( "status.txt" )           ;
  QDateTime   ft    = QDateTime ( from , zero )                              ;
  QDateTime   tt    = nTimeNow                                               ;
  QString     CSV   = ""                                                     ;
  QFileInfo   FI ( f )                                                       ;
  QString     m                                                              ;
  QStringList R                                                              ;
  QDate       Last                                                           ;
  QDate       Current                                                        ;
  QString     cs                                                             ;
  QString     cf                                                             ;
  cs = tr("To create %1 CSV, you need to complete the %1 download.").arg(s)  ;
  cf = tr("Incorrect format in %1.").arg(f)                                  ;
  if ( ! FI . exists ( ) )                                                   {
    print ( cs )                                                             ;
    return                                                                   ;
  }                                                                          ;
  R = File::StringList  ( f , "\n" )                                         ;
  R = File::PurifyLines ( R        )                                         ;
  if ( 2 != R . count ( ) )                                                  {
    print ( cf )                                                             ;
    return                                                                   ;
  }                                                                          ;
  Last    = QDate::fromString ( R[0] , "yyyy/MM/dd" )                        ;
  Current = QDate::fromString ( R[1] , "yyyy/MM/dd" )                        ;
  QDateTime DL ( Last    , zero )                                            ;
  QDateTime DC ( Current , zero )                                            ;
  if ( DL . secsTo ( nTimeNow ) > ( 58 * 3600 )                             ||
       DC . secsTo ( nTimeNow ) > ( 31 * 3600 )                            ) {
    print ( cs )                                                             ;
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QString csv = Root . absoluteFilePath ( "CSV" )                            ;
  QDir    DXV ( csv )                                                        ;
  QString sdv = DXV  . absoluteFilePath ( s     )                            ;
  QDir    SYM ( sdv )                                                        ;
  QFile   CSVF                                                               ;
  DXV . mkpath ( sdv )                                                       ;
  f = SYM . absoluteFilePath ( "status.txt" )                                ;
  R . clear ( )                                                              ;
  R = File::StringList  ( f , "\n" )                                         ;
  R = File::PurifyLines ( R        )                                         ;
  ////////////////////////////////////////////////////////////////////////////
  tt = tt . addSecs ( -(23*3600) )                                           ;
  while ( tt > ft )                                                          {
    m  = ft . date() . toString ( "yyyy/MM" )                                ;
    if ( ! R . contains ( m ) )                                              {
      if ( m != CSV )                                                        {
        if ( CSVF . isOpen ( ) )                                             {
          if ( ( CSV . length ( ) > 0 ) && ( ! R.contains(CSV) ) ) R << CSV  ;
          CSVF . close (         )                                           ;
          CsvStatus    ( SYM , R )                                           ;
        }                                                                    ;
        CSV  = m                                                             ;
        m    = ft . date() . toString ( "yyyy-MM"            )               ;
        m    . append                 ( ".csv"               )               ;
        m    = SYM . absoluteFilePath ( m                    )               ;
        CSVF . setFileName            ( m                    )               ;
        CSVF . open                   ( QIODevice::WriteOnly )               ;
        m    = tr                     ( "Creating %1"        ) . arg ( m )   ;
        print                         ( m                    )               ;
      }                                                                      ;
      Dukascopy::ToCSV                ( s , ft.date() , CSVF )               ;
    }                                                                        ;
    ft = ft . addSecs                 ( 86400                )               ;
    tt = nTimeNow                                                            ;
    tt = tt . addSecs                 ( -(23*3600)           )               ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( CSVF . isOpen ( ) ) CSVF . close ( )                                  ;
  CsvStatus ( SYM , R )                                                      ;
  if ( ! isEnabled ( "MergeCSV" ) ) return                                   ;
  if ( ( CSV . length ( ) > 0 ) && ( ! R.contains(CSV) ) ) R << CSV          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( R . count ( ) <= 0 ) return                                           ;
  sdv = QString("%1.csv").arg(s)                                             ;
  sdv = DXV . absoluteFilePath ( sdv )                                       ;
  cs = tr("Creating %1...").arg(sdv)                                         ;
  print ( cs )                                                               ;
  QFile ACSV ( sdv )                                                         ;
  if ( ! ACSV . open ( QIODevice::WriteOnly ) ) return                       ;
  foreach ( f , R )                                                          {
    m = f                                                                    ;
    m = m   . replace          ( "/" , "-" )                                 ;
    m       . append           ( ".csv"    )                                 ;
    m = SYM . absoluteFilePath ( m         )                                 ;
    QFileInfo SYF ( m )                                                      ;
    if ( SYF . exists ( ) )                                                  {
      QFile      FYS ( m )                                                   ;
      QByteArray V                                                           ;
      if ( FYS . open ( QIODevice::ReadOnly ) )                              {
        m = tr("%1 %2 joins %3").arg(s).arg(f).arg(sdv)                      ;
        print ( m )                                                          ;
        do                                                                   {
          V = FYS . read ( 65536 )                                           ;
          if ( V . size ( ) > 0 ) ACSV . write ( V )                         ;
        } while ( 65536 == V . size ( ) )                                    ;
        FYS . close ( )                                                      ;
      }                                                                      ;
    }                                                                        ;
  }                                                                          ;
  ACSV . close ( )                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  QFileInfo ZCSV ( sdv )                                                     ;
  m = tr("%1 is %2 bytes").arg(sdv).arg(ZCSV.size())                         ;
  print ( m )                                                                ;
}

bool N::ScriptableDukascopy::ImportCSV(QString symbol,QString filename)
{
  QFileInfo  FI ( filename )                                                 ;
  bool       correct = false                                                 ;
  int        blocks  = Values [ "CsvBlocks" ]                                ;
  qint64     total   = 0                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ! FI . exists ( )  ) return false                                     ;
  if ( FI . size ( ) <= 0 ) return true                                      ;
  if ( ! Initialized . contains ( symbol ) ) Initialized [ symbol ] = false  ;
  ////////////////////////////////////////////////////////////////////////////
  SqlConnection SC ( sql )                                                   ;
  if ( SC . open ( QtUUID::createUuidString() , QString ( __FUNCTION__ ) ) ) {
    QFile           X ( filename )                                           ;
    Forex::Importer fi                                                       ;
    QString         s = symbol                                               ;
    int             level = Level.at(0).toLatin1() - 'A'                     ;
    //////////////////////////////////////////////////////////////////////////
    s . insert ( 3 , "/" )                                                   ;
    s = s . replace ( "CNH" , CNH )                                          ;
    //////////////////////////////////////////////////////////////////////////
    fi . identifier = Identifier                                             ;
    fi . symbol     = s                                                      ;
    fi . FromYear   = FromYear                                               ;
    fi . ToYear     = ToYear                                                 ;
    fi . method     = Method                                                 ;
    fi . setDigits ( Digits [ symbol ] )                                     ;
    fi . setLevel  ( level             )                                     ;
    fi . setPeriod ( Period            )                                     ;
    if ( ( ! Initialized [ symbol ] ) && ( ! fi . PeriodExists ( SC ) ) )    {
      fi . CreatePeriods ( SC )                                              ;
    }                                                                        ;
    Initialized [ symbol ] = true                                            ;
    //////////////////////////////////////////////////////////////////////////
    if ( X . open ( QIODevice::ReadOnly ) )                                  {
      QByteArray  B                                                          ;
      QStringList LL                                                         ;
      QString     R                                                          ;
      bool        keep                                                       ;
      int         totalx                                                     ;
      do                                                                     {
        LL . clear ( )                                                       ;
        keep = true                                                          ;
        for (int i = 0 ; keep && i < blocks ; i++ )                          {
          B = X . readLine ( )                                               ;
          R = QString::fromUtf8 ( B )                                        ;
          R = R . replace ( "\r" , "" )                                      ;
          R = R . replace ( "\n" , "" )                                      ;
          LL << R                                                            ;
          if ( R . length ( ) <= 0 ) keep = false                            ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        totalx = LL . count ( )                                              ;
        if ( totalx > 0 )                                                    {
          for (int i=0;i<totalx;i++)                                         {
            R = LL [ i ]                                                     ;
            if ( R . length ( ) > 0 )                                        {
              R = R . replace ( "/" , "." )                                  ;
              if ( fi . Insert ( SC , R ) )                                  {
                total             ++                                         ;
                Values [ symbol ] ++                                         ;
              }                                                              ;
            }                                                                ;
          }                                                                  ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
      } while ( B . size ( ) > 0 )                                           ;
      Values [ "Files" ] ++                                                  ;
      X . close ( )                                                          ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    addTicks ( total )                                                       ;
    correct = true                                                           ;
    SC . close (  )                                                          ;
  }                                                                          ;
  SC . remove ( )                                                            ;
  ////////////////////////////////////////////////////////////////////////////
  return correct                                                             ;
}

void N::ScriptableDukascopy::CsvToSql(ThreadData * d,QString s)
{
  QDir        CSV        = Root . absoluteFilePath ( "CSV" ) ;
  QDir        SYM        = CSV  . absoluteFilePath ( s     ) ;
  bool        separation = isEnabled ( "SeparateCSV" )       ;
  QStringList processed                                      ;
  QStringList R                                              ;
  ////////////////////////////////////////////////////////////
  Values [ s ] = 0                                           ;
  ////////////////////////////////////////////////////////////
  if ( ! separation )                                        {
    QString f = QString("%1.csv").arg(s)                     ;
    f = CSV . absoluteFilePath ( f )                         ;
    QFileInfo CSVF ( f )                                     ;
    if ( CSVF . exists ( ) && ( CSVF . size ( ) > 0 ) )      {
      R << f                                                 ;
    } else                                                   {
      separation = true                                      ;
    }                                                        ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  if (   separation )                                        {
    QStringList   filters                                    ;
    QFileInfoList fils                                       ;
    QString       f = SYM . absoluteFilePath ( "sql.txt" )   ;
    processed = File::StringList ( f , "\n" )                ;
    filters << "*.csv"                                       ;
    fils = SYM . entryInfoList ( filters )                   ;
    for (int i=0;i<fils.count();i++)                         {
      QString bn = fils [ i ] . fileName ( )                 ;
      if ( ! processed . contains ( bn ) )                   {
        R << fils [ i ] . absoluteFilePath ( )               ;
      }                                                      ;
    }                                                        ;
  }                                                          ;
  if ( R . count ( ) <= 0 ) return                           ;
  ////////////////////////////////////////////////////////////
  QString filename                                           ;
  foreach ( filename , R )                                   {
    if ( ImportCSV ( s , filename ) && separation )          {
      QFileInfo FXI ( filename )                             ;
      QString   fn = FXI . fileName ( )                      ;
      if ( ! processed . contains ( fn ) )                   {
        processed << fn                                      ;
        SqlStatus ( SYM , processed )                        ;
      }                                                      ;
    }                                                        ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  if ( separation && processed . count ( ) > 0 )             {
    processed . takeLast ( )                                 ;
    SqlStatus ( SYM , processed )                            ;
  }                                                          ;
}

bool N::ScriptableDukascopy::ImportBI5(QString symbol,QDate today,int hour)
{
  int        year  = today . year  ( )                                       ;
  int        month = today . month ( )                                       ;
  int        day   = today . day   ( )                                       ;
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
      fi . CreatePeriods ( SC )                                              ;
    }                                                                        ;
    Initialized [ symbol ] = true                                            ;
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
      if ( fi . Insert ( SC , tick ) )                                       {
        Values [ symbol ] ++                                                 ;
      }                                                                      ;
    }                                                                        ;
    addTicks ( total )                                                       ;
    correct = true                                                           ;
    SC . close (  )                                                          ;
  }                                                                          ;
  SC . remove ( )                                                            ;
  ////////////////////////////////////////////////////////////////////////////
  Values [ "Files" ] ++                                                      ;
  return correct                                                             ;
}

bool N::ScriptableDukascopy::ImportBI5(QString symbol,QDate today)
{
  int        yy = today . year  ( )                                          ;
  int        mm = today . month ( )                                          ;
  int        dd = today . day   ( )                                          ;
  int        hh = 0                                                          ;
  QString    YY = QString::number(yy)                                        ;
  QString    MM = QString("%1%2").arg((mm-1)/10).arg((mm-1)%10)              ;
  QString    DD = QString("%1%2").arg(dd/10).arg(dd%10)                      ;
  QString    FS = QString("%1/%2/%3/%4").arg(symbol).arg(YY).arg(MM).arg(DD) ;
  QString    FP = Root.absoluteFilePath(FS)                                  ;
  QDir       DS = QDir ( FP )                                                ;
  QString    SS = DS.absoluteFilePath("sql.txt")                             ;
  QString    HH                                                              ;
  QString    DI                                                              ;
  QString    BI                                                              ;
  QString    FI                                                              ;
  QByteArray BB                                                              ;
  QFileInfo  FX ( SS )                                                       ;
  ////////////////////////////////////////////////////////////////////////////
  if ( FX . exists ( ) )                                                     {
    File::toByteArray ( SS , BB )                                            ;
    FI = QString::fromUtf8(BB)                                               ;
    if ( "24:00:00" == FI ) return true ; else                               {
      QTime TX                                                               ;
      TX = QTime::fromString(FI,"hh:mm:ss")                                  ;
      hh = TX . hour ( )                                                     ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  while ( hh < 24 )                                                          {
    HH = QString("%1%2").arg(hh/10).arg(hh%10)                               ;
    DI = QString("%1h_ticks").arg(HH)                                        ;
    BI = QString("%1.bi5").arg(DI)                                           ;
    FI = DS.absoluteFilePath(BI)                                             ;
    QFileInfo FX ( FI )                                                      ;
    if ( FX . exists ( ) )                                                   {
      if ( ImportBI5 ( symbol , today , hh ) )                               {
        hh++                                                                 ;
        FI = QString("%1%2:00:00").arg(hh/10).arg(hh%10)                     ;
        BB = FI . toUtf8 ( )                                                 ;
        File::toFile ( SS , BB )                                             ;
      } else hh++                                                            ;
    } else hh++                                                              ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

void N::ScriptableDukascopy::Build(ThreadData * d,QString symbol)
{
  QDate     SD = TimeRecords [ symbol ] . toDate ( ) ;
  QDateTime ST ( SD , QTime ( 0 , 0 , 0 , 0 ) )      ;
  Values [ symbol ] = 0                              ;
  while ( IsContinue ( d ) && ( ST < nTimeNow ) )    {
    ImportBI5 ( symbol , ST . date ( ) )             ;
    ST = ST . addSecs ( 86400 )                      ;
  }                                                  ;
}

void N::ScriptableDukascopy::Optimize(ThreadData * d,QString symbol)
{
  QStringList T                                                              ;
  QString     s = symbol                                                     ;
  QString     pattern                                                        ;
  QString     Q                                                              ;
  s       . insert  ( 3 , "_" )                                              ;
  pattern = QString ("%1_%2_%3_t00_").arg(Identifier).arg(Level).arg(s)      ;
  pattern = pattern . toLower (     )                                        ;
  pattern . append            ( "%" )                                        ;
  ////////////////////////////////////////////////////////////////////////////
  SqlConnection SC ( sql )                                                   ;
  if ( SC . open ( FunctionString ) )                                        {
    T . clear ( )                                                            ;
    //////////////////////////////////////////////////////////////////////////
    Q = QString("show tables like '%1' ;").arg(pattern)                      ;
    if ( SC . Query ( Q ) ) while ( IsContinue ( d ) && SC . Next ( ) )      {
      T << SC . String ( 0 )                                                 ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( T . count ( ) > 0 )                                                 {
      ProgressReporter pr                                                    ;
      qSort ( T . begin ( ) , T . end ( ) )                                  ;
      pr . total = T . count ( )                                             ;
      pr . Progress      ( tr("Optimize %1").arg(symbol)                     ,
                           tr("%v/%m tables")                              ) ;
      pr . Start         (                                                 ) ;
      pr . ProgressReady ( 500                                             ) ;
      pr . setRange      ( 0 , pr . total                                  ) ;
      pr . setFrequency  ( tr("%1.%2 tables/s") , tr("%1.%2 s/table")      ) ;
      while ( pr . running && IsContinue ( d ) && ( T . count ( ) > 0 ) )    {
        ++ pr                                                                ;
        Q = QString("optimize table %1 ;") . arg ( T . first ( ) )           ;
        print          ( Q )                                                 ;
        SC . Query     ( Q )                                                 ;
        T  . takeFirst (   )                                                 ;
      }                                                                      ;
      pr . Finish ( )                                                        ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                          ;
  } else                                                                     {
    print ( tr("Dukascopy database can not be connected.") )                 ;
    SC . close  ( )                                                          ;
  }                                                                          ;
  SC   . remove ( )                                                          ;
}

void N::ScriptableDukascopy::Start(int msec,int downloads,int threads)
{
  bool      correct = true                                                 ;
  bool      display = false                                                ;
  int       i       = 0                                                    ;
  int       j       = 0                                                    ;
  QDateTime Start   = nTimeNow                                             ;
  QString   m                                                              ;
  //////////////////////////////////////////////////////////////////////////
  while ( correct )                                                        {
    ////////////////////////////////////////////////////////////////////////
    if ( Values [ "Download" ] < downloads )                               {
      VarArgs V                                                            ;
      V << Symbols [ i ]                                                   ;
      Values [ "Download" ] ++                                             ;
      start ( 10001 , V )                                                  ;
      i++                                                                  ;
      i %= Symbols . count ( )                                             ;
      display = false                                                      ;
    }                                                                      ;
    ////////////////////////////////////////////////////////////////////////
    if ( Values [ "SQL" ] < threads )                                      {
      VarArgs V                                                            ;
      V << Symbols [ j ]                                                   ;
      Values [ "SQL" ] ++                                                  ;
      start ( 10002 , V )                                                  ;
      j++                                                                  ;
      j %= Symbols . count ( )                                             ;
    }                                                                      ;
    ////////////////////////////////////////////////////////////////////////
    Time::msleep(msec)                                                     ;
    ////////////////////////////////////////////////////////////////////////
    if ( ! display && ( 0 == i ) )                                         {
      display = true                                                       ;
      int    ds   = Start.secsTo(nTimeNow)                                 ;
      int    days = ds / 86400                                             ;
      qint64 bps  = 0                                                      ;
      QTime  T ( 0 , 0 , 0 , 0 )                                           ;
      T = T . addSecs ( ds % 86400 )                                       ;
      if ( ds > 0 ) bps = Received / ds                                    ;
      m = tr ("%1 days %2, %3 bytes %4 files downloaded, BPS %5, %6 ticks" )
          . arg ( days                                                     )
          . arg ( T . toString ( "hh:mm:ss" )                              )
          . arg ( Received                                                 )
          . arg ( TotalFiles                                               )
          . arg ( bps                                                      )
          . arg ( TotalTicks                                             ) ;
      N::printf ( m )                                                      ;
    }                                                                      ;
  }                                                                        ;
}

void N::ScriptableDukascopy::WaitCompletion(QString section,QDateTime & Start,QDateTime & Now)
{
  while ( Values [ section ]  > 0 )        {
    Time::msleep ( 100                   ) ;
    Report       ( Start , Now , section ) ;
  }                                        ;
}

void N::ScriptableDukascopy::WaitSqlCompletion(QString section,QDateTime & Start,QDateTime & Now)
{
  while ( Values [ section ]  > 0 )        {
    Time::msleep ( 100                   ) ;
    ReportSql    ( Start , Now , section ) ;
  }                                        ;
}

void N::ScriptableDukascopy::AddExternal(QString section)
{
  if ( Externals . contains ( section ) ) {
    qint64 * p = Externals [ section ]    ;
    if ( NULL != p ) (*p) ++              ;
  }                                       ;
}

void N::ScriptableDukascopy::SubExternal(QString section)
{
  if ( Externals . contains ( section ) ) {
    qint64 * p = Externals [ section ]    ;
    if ( NULL != p ) (*p) --              ;
  }                                       ;
}

void N::ScriptableDukascopy::RealTime(ThreadData * d,QString symbol)
{
  if ( IsContinue ( d ) ) Dukascopy::Check  ( symbol ) ;
  if ( IsContinue ( d ) ) Dukascopy::Import ( symbol ) ;
}

void N::ScriptableDukascopy::RealTime(ThreadData * d)
{
  int       threads = Values [ "RealTimeThreads" ]         ;
  QString   section = "RealTime"                           ;
  int       type    = 21003                                ;
  bool      correct = true                                 ;
  int       j       = 0                                    ;
  int       msec    = 100                                  ;
  QDateTime Start   = nTimeNow                             ;
  QDateTime Now     = nTimeNow                             ;
  //////////////////////////////////////////////////////////
  ProgressReporter pr                                      ;
  pr . total = Symbols . count ( )                         ;
  pr . Progress      ( tr("Update ticks")                  ,
                       tr("%v/%m symbols")               ) ;
  pr . Start         (                                   ) ;
  pr . ProgressReady ( 500                               ) ;
  pr . setRange      ( 0 , pr . total                    ) ;
  pr . setFrequency  ( tr("%1.%2 symbols/s")               ,
                       tr("%1.%2 s/symbol" )             ) ;
  //////////////////////////////////////////////////////////
  Values      [ section ]  = 0                             ;
  AddExternal ( section )                                  ;
  //////////////////////////////////////////////////////////
  while ( pr . running && IsContinue ( d ) && correct )    {
    if ( Values [ section ] < threads )                    {
      VarArgs V                                            ;
      V << Symbols [ j ]                                   ;
      Values [ section ] ++                                ;
      start ( type , V )                                   ;
      j++                                                  ;
      ++pr                                                 ;
      if ( j == Symbols . count ( ) ) correct = false      ;
    }                                                      ;
    ////////////////////////////////////////////////////////
    if ( ! Hourly ( ) ) correct = false                    ;
    Time::msleep ( msec )                                  ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  if ( IsContinue ( d ) )                                  {
    WaitSqlCompletion ( section , Start , Now )            ;
    SubExternal       ( section               )            ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  pr . Finish ( )                                          ;
}

void N::ScriptableDukascopy::Mapping(ThreadData * d,QString symbol)
{
  QString   S = symbol                                             ;
  QString   M = S                                                  ;
  QDateTime CT                                                     ;
  qint64    cdx                                                    ;
  bool      correct = true                                         ;
  int       msec    = 10                                           ;
  //////////////////////////////////////////////////////////////////
  M . insert ( 3 , "/" )                                           ;
  CT  = TimeValue ( M , "Completed" )                              ;
  cdx = TimeArray [ M ] . toIndex ( CT )                           ;
  //////////////////////////////////////////////////////////////////
  while ( IsContinue ( d ) && correct && ( CT < nTimeNow ) )       {
    if ( ! TimeArray [ M ] . at ( CT ) )                           {
      int yy = CT . date ( ) . year  ( )                           ;
      int mm = CT . date ( ) . month ( )                           ;
      int dd = CT . date ( ) . day   ( )                           ;
      int hh = CT . time ( ) . hour  ( )                           ;
      if ( ! TickExists ( S , CT ) )                               {
        if ( download ( S , yy , mm , dd , hh ) )                  {
          TimeArray [ M ] . setBool ( CT , TickExists ( S , CT ) ) ;
        }                                                          ;
      } else                                                       {
        TimeArray [ M ] . setBool ( CT , true )                    ;
      }                                                            ;
    }                                                              ;
    CT = CT . addSecs ( 3600 )                                     ;
    if ( Hourly ( ) ) correct = false                              ;
    Time::msleep ( msec )                                          ;
  }                                                                ;
  //////////////////////////////////////////////////////////////////
  cdx = TimeArray [ M ] . FirstFalse ( cdx )                       ;
  if ( cdx > 0 )                                                   {
    CT = TimeArray [ M ] . toDateTime ( cdx )                      ;
    setTime ( M , "Completed" , CT )                               ;
  }                                                                ;
}

void N::ScriptableDukascopy::Mapping(ThreadData * d)
{
  int       threads = Values [ "MappingThreads" ]          ;
  QString   section = "Mapping"                            ;
  int       type    = 21002                                ;
  bool      correct = true                                 ;
  int       j       = 0                                    ;
  int       msec    = 100                                  ;
  QDateTime Start   = nTimeNow                             ;
  QDateTime Now     = nTimeNow                             ;
  //////////////////////////////////////////////////////////
  ProgressReporter pr                                      ;
  pr . total = Symbols . count ( )                         ;
  pr . Progress      ( tr("Update missing ticks")          ,
                       tr("%v/%m symbols")               ) ;
  pr . Start         (                                   ) ;
  pr . ProgressReady ( 500                               ) ;
  pr . setRange      ( 0 , pr . total                    ) ;
  pr . setFrequency  ( tr("%1.%2 symbols/s")               ,
                       tr("%1.%2 s/symbol" )             ) ;
  //////////////////////////////////////////////////////////
  Values      [ section ]  = 0                             ;
  AddExternal ( section )                                  ;
  //////////////////////////////////////////////////////////
  while ( pr . running && IsContinue ( d ) && correct )    {
    if ( Values [ section ] < threads )                    {
      VarArgs V                                            ;
      V << Symbols [ j ]                                   ;
      Values [ section ] ++                                ;
      start ( type , V )                                   ;
      j++                                                  ;
      ++pr                                                 ;
      if ( j == Symbols . count ( ) ) correct = false      ;
    }                                                      ;
    ////////////////////////////////////////////////////////
    if ( Hourly ( ) ) correct = false                      ;
    Time::msleep ( msec )                                  ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  if               ( IsContinue ( d )      )               {
    WaitCompletion ( section , Start , Now )               ;
    SubExternal    ( section               )               ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  SaveMaps ( Root . absoluteFilePath ( "Download.raw" )  ) ;
  pr . Finish ( )                                          ;
}

void N::ScriptableDukascopy::Building(ThreadData * d,QString symbol)
{
  QDate     SD = TailTime [ symbol ] . toDate ( )             ;
  QDateTime ST ( SD , QTime ( 0 , 0 , 0 , 0 ) )               ;
  bool      correct = true                                    ;
  while ( IsContinue ( d ) && ( ST < nTimeNow ) && correct )  {
    print ( tr("Importing %1 %2").arg(symbol).arg(ST.toString("yyyy/MM/dd")) ) ;
    ImportBI5 ( symbol , ST . date ( ) )                      ;
    ST = ST . addSecs ( 86400 )                               ;
    if ( Hourly ( ) ) correct = false                         ;
  }                                                           ;
}

void N::ScriptableDukascopy::Importing(ThreadData * d)
{
  int       threads   = Values [ "ImportThreads" ]      ;
  QString   section   = "Importing"                     ;
  int       type      = 21001                           ;
  bool      correct   = true                            ;
  int       j         = 0                               ;
  int       msec      = 100                             ;
  QDateTime Start     = nTimeNow                        ;
  QDateTime Now       = nTimeNow                        ;
  ///////////////////////////////////////////////////////
  ProgressReporter pr                                   ;
  pr . total = Symbols . count ( )                      ;
  pr . Progress      ( tr("Import ticks")               ,
                       tr("%v/%m symbols")            ) ;
  pr . Start         (                                ) ;
  pr . ProgressReady ( 500                            ) ;
  pr . setRange      ( 0 , pr . total                 ) ;
  pr . setFrequency  ( tr("%1.%2 symbols/s")            ,
                       tr("%1.%2 s/symbol" )          ) ;
  ///////////////////////////////////////////////////////
  Values      [ section ] = 0                           ;
  AddExternal ( section )                               ;
  ///////////////////////////////////////////////////////
  while ( pr . running && IsContinue ( d ) && correct ) {
    if ( Values [ section ] < threads )                 {
      VarArgs V                                         ;
      V << Symbols [ j ]                                ;
      Values [ section ] ++                             ;
      start ( type , V )                                ;
      j++                                               ;
      ++pr                                              ;
      if ( j == Symbols . count ( ) ) correct = false   ;
    }                                                   ;
    /////////////////////////////////////////////////////
    if ( Hourly ( ) ) correct = false                   ;
    Time::msleep    ( msec                  )           ;
    ReportSql       ( Start , Now , section )           ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  if ( IsContinue ( d ) )                               {
    WaitSqlCompletion ( section , Start , Now )         ;
    SubExternal       ( section               )         ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  pr . Finish ( )                                       ;
}

void N::ScriptableDukascopy::Investigate(ThreadData * d)
{
  int       threads   = Values [ "InvestigateThreads" ] ;
  QString   section   = "Investigate"                   ;
  int       type      = 10005                           ;
  bool      correct   = true                            ;
  int       j         = 0                               ;
  int       msec      = 100                             ;
  QDateTime Start     = nTimeNow                        ;
  QDateTime Now       = nTimeNow                        ;
  ///////////////////////////////////////////////////////
  ProgressReporter pr                                   ;
  pr . total = Symbols . count ( )                      ;
  pr . Progress      ( tr("Investigate ticks")          ,
                       tr("%v/%m symbols")            ) ;
  pr . Start         (                                ) ;
  pr . ProgressReady ( 500                            ) ;
  pr . setRange      ( 0 , pr . total                 ) ;
  pr . setFrequency  ( tr("%1.%2 symbols/s")            ,
                       tr("%1.%2 s/symbol" )          ) ;
  ///////////////////////////////////////////////////////
  Values      [ section ] = 0                           ;
  AddExternal ( section )                               ;
  ///////////////////////////////////////////////////////
  while ( pr . running && IsContinue ( d ) && correct ) {
    if ( Values [ section ] < threads )                 {
      VarArgs V                                         ;
      V << Symbols [ j ]                                ;
      Values [ section ] ++                             ;
      start ( type , V )                                ;
      j++                                               ;
      ++pr                                              ;
      if ( j == Symbols . count ( ) ) correct = false   ;
    }                                                   ;
    /////////////////////////////////////////////////////
    if ( Hourly ( ) ) correct = false                   ;
    Time::msleep ( msec                  )              ;
    Report       ( Start , Now , section )              ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  if               ( IsContinue ( d )      )            {
    WaitCompletion ( section , Start , Now )            ;
    SubExternal    ( section               )            ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  pr . Finish ( )                                       ;
}

void N::ScriptableDukascopy::PeriodicInvestigate(ThreadData * d)
{
  QString OptimizeAt                                                         ;
  bool    required  = true                                                   ;
  int     dayOfWeek = 6                                                      ;
  int     roll      = 0                                                      ;
  QTime   T ( 14 , 0 , 0 , 0 )                                               ;
  QTime   X                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  while ( IsContinue ( d ) )                                                 {
    if ( required )                                                          {
      QString     ini = Root . absoluteFilePath ( "settings.ini" )           ;
      Settings    settings ( ini , QSettings::IniFormat )                    ;
      QStringList L                                                          ;
      settings . beginGroup ( "dukascopy" )                                  ;
      OptimizeAt = settings . valueString ( "OptimizeAt" )                   ;
      settings . endGroup   (             )                                  ;
      required = false                                                       ;
      L        = OptimizeAt . split ( " " )                                  ;
      if ( L . count ( ) >= 2 )                                              {
        dayOfWeek = L [ 0 ] . toInt ( )                                      ;
        T         = QTime::fromString ( L [ 1 ] , "hh:mm:ss" )               ;
      }                                                                      ;
      Time::msleep ( 1000 )                                                  ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    bool dow = ( dayOfWeek == nTimeNow . date ( ) . dayOfWeek ( ) )          ;
    if ( dow )                                                               {
      int h = nTimeNow . time ( ) . hour ( )                                 ;
      if ( h < 9 ) dow = false                                               ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ! dow )                                                             {
      if ( IsContinue ( d ) )                                                {
        if ( Hourly ( ) )                                                    {
          RealTime        ( d )                                              ;
        } else                                                               {
          switch ( roll )                                                    {
            case 0                                                           :
              Importing   ( d )                                              ;
            break                                                            ;
            case 1                                                           :
              Investigate ( d )                                              ;
            break                                                            ;
            case 2                                                           :
              Mapping     ( d )                                              ;
            break                                                            ;
          }                                                                  ;
          roll ++                                                            ;
          roll %= 3                                                          ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    Time::msleep ( 250 )                                                     ;
    //////////////////////////////////////////////////////////////////////////
    if ( ! required )                                                        {
      X        = nTimeNow . time ( )                                         ;
      required = ( ( 0 == X . minute ( ) ) && ( 0 == X . second ( ) ) )      ;
    }                                                                        ;
  }                                                                          ;
}

void N::ScriptableDukascopy::Optimize(ThreadData * d)
{
  Optimize ( this , d ) ;
}

void N::ScriptableDukascopy::Optimize(Thread * t,ThreadData * d)
{
  QDateTime StartTime = nTimeNow                                     ;
  int       threads   = Values [ "OptimizeThreads" ]                 ;
  QString   section   = "Optimize"                                   ;
  int       type      = 10004                                        ;
  bool      correct   = true                                         ;
  int       j         = 0                                            ;
  int       msec      = 100                                          ;
  QDateTime Start     = nTimeNow                                     ;
  QDateTime Now       = nTimeNow                                     ;
  ////////////////////////////////////////////////////////////////////
  Values      [ section ] = 0                                        ;
  AddExternal ( section )                                            ;
  ////////////////////////////////////////////////////////////////////
  while ( t -> IsContinue ( d ) && correct )                         {
    if ( Values [ section ] < threads )                              {
      VarArgs V                                                      ;
      V << Symbols [ j ]                                             ;
      Values [ section ] ++                                          ;
      start ( type , V )                                             ;
      j++                                                            ;
      if ( j == Symbols . count ( ) ) correct = false                ;
    }                                                                ;
    //////////////////////////////////////////////////////////////////
    Time::msleep ( msec                  )                           ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  if               ( IsContinue ( d )      )                         {
    WaitCompletion ( section , Start , Now )                         ;
    SubExternal    ( section               )                         ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  QDateTime EndTime = nTimeNow                                       ;
  print ( tr    ( "Optimize dukascopy time from %1 to %2."           )
          . arg ( StartTime . toString ( "yyyy/MM/dd-hh:mm:ss" )     )
          . arg ( EndTime   . toString ( "yyyy/MM/dd-hh:mm:ss" ) ) ) ;
}

void N::ScriptableDukascopy::PeriodicOptimize(ThreadData * d)
{
  PeriodicOptimize ( this , d ) ;
}

void N::ScriptableDukascopy::PeriodicOptimize(Thread * t,ThreadData * d)
{
  if ( NULL == t ) return                                                    ;
  if ( NULL == d ) return                                                    ;
  ////////////////////////////////////////////////////////////////////////////
  QString OptimizeAt                                                         ;
  bool    required  = true                                                   ;
  int     dayOfWeek = 6                                                      ;
  QTime   T ( 14 , 0 , 0 , 0 )                                               ;
  QTime   X                                                                  ;
  ////////////////////////////////////////////////////////////////////////////
  while ( t -> IsContinue ( d ) )                                            {
    if ( required )                                                          {
      QString     ini = Root . absoluteFilePath ( "settings.ini" )           ;
      Settings    settings ( ini , QSettings::IniFormat )                    ;
      QStringList L                                                          ;
      settings . beginGroup ( "dukascopy" )                                  ;
      OptimizeAt = settings . valueString ( "OptimizeAt" )                   ;
      settings . endGroup   (             )                                  ;
      required = false                                                       ;
      L        = OptimizeAt . split ( " " )                                  ;
      if ( L . count ( ) >= 2 )                                              {
        dayOfWeek = L [ 0 ] . toInt ( )                                      ;
        T         = QTime::fromString ( L [ 1 ] , "hh:mm:ss" )               ;
      }                                                                      ;
      Time::msleep ( 1000 )                                                  ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( dayOfWeek == nTimeNow . date ( ) . dayOfWeek ( ) )                  {
      QTime tz = nTimeNow . time   (    )                                    ;
      int   dt = T        . secsTo ( tz )                                    ;
      if ( ( dt >= 0 ) && ( dt < 10 ) )                                      {
        Optimize ( t , d )                                                   ;
        required = true                                                      ;
      } else                                                                 {
        Time::msleep ( 250 )                                                 ;
      }                                                                      ;
    } else                                                                   {
      Time::msleep ( 250 )                                                   ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ! required )                                                        {
      X        = nTimeNow . time ( )                                         ;
      required = ( ( 0 == X . minute ( ) ) && ( 0 == X . second ( ) ) )      ;
    }                                                                        ;
  }                                                                          ;
}

void N::ScriptableDukascopy::Investigate(void)
{
  start ( 20002 ) ;
}

void N::ScriptableDukascopy::Optimize(void)
{
  start ( 20001 ) ;
}

bool N::ScriptableDukascopy::GetSettings(Settings & INI,qint32 * p)
{
  INI . beginGroup ( "dukascopy" )                                         ;
  //////////////////////////////////////////////////////////////////////////
  if ( INI . contains ( "OptimizeThreads" ) )                              {
    setKeyValue                                                            (
      "OptimizeThreads"                                                    ,
      INI . value ( "OptimizeThreads" ) . toInt ( )                      ) ;
  } else                                                                   {
    if ( NULL != p )                                                       {
      setKeyValue ( "OptimizeThreads" , (*p) - 1                         ) ;
    } else                                                                 {
      setKeyValue ( "OptimizeThreads" , 2                                ) ;
    }                                                                      ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  if ( INI . contains ( "InvestigateThreads" ) )                           {
    setKeyValue                                                            (
      "InvestigateThreads"                                                 ,
      INI . value ( "InvestigateThreads" ) . toInt ( )                   ) ;
  } else                                                                   {
    if ( NULL != p )                                                       {
      setKeyValue ( "InvestigateThreads" , (*p) / 2                      ) ;
    } else                                                                 {
      setKeyValue ( "InvestigateThreads" , 2                             ) ;
    }                                                                      ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  if ( INI . contains ( "ImportThreads" ) )                                {
    setKeyValue                                                            (
      "ImportThreads"                                                      ,
      INI . value ( "ImportThreads" ) . toInt ( )                        ) ;
  } else                                                                   {
    if ( NULL != p )                                                       {
      setKeyValue ( "ImportThreads" , (*p) - 1                           ) ;
    } else                                                                 {
      setKeyValue ( "ImportThreads" , 2                                  ) ;
    }                                                                      ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  if ( INI . contains ( "MappingThreads" ) )                               {
    setKeyValue                                                            (
      "MappingThreads"                                                     ,
      INI . value ( "MappingThreads" ) . toInt ( )                       ) ;
  } else                                                                   {
    if ( NULL != p )                                                       {
      setKeyValue ( "MappingThreads" , (*p) - 1                          ) ;
    } else                                                                 {
      setKeyValue ( "MappingThreads" , 2                                 ) ;
    }                                                                      ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  if ( INI . contains ( "RealTimeThreads" ) )                              {
    setKeyValue                                                            (
      "RealTimeThreads"                                                    ,
      INI . value ( "RealTimeThreads" ) . toInt ( )                      ) ;
  } else                                                                   {
    if ( NULL != p )                                                       {
      setKeyValue ( "RealTimeThreads" , (*p) - 1                         ) ;
    } else                                                                 {
      setKeyValue ( "RealTimeThreads" , 2                                ) ;
    }                                                                      ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  INI . endGroup   (             )                                         ;
  return true                                                              ;
}

void N::ScriptableDukascopy::LaunchSql(int threads,int type,QString section)
{
  ThreadData * d       = drawer ( )                   ;
  bool         correct = true                         ;
  int          j       = 0                            ;
  int          msec    = 100                          ;
  QDateTime    Start   = nTimeNow                     ;
  QDateTime    Now     = nTimeNow                     ;
  /////////////////////////////////////////////////////
  Values [ "Files" ] = 0                              ;
  Values [ section ] = 0                              ;
  while ( correct )                                   {
    if ( NULL != d )                                  {
      if ( ! IsContinue ( d ) )                       {
        correct = false                               ;
        continue                                      ;
      }                                               ;
    }                                                 ;
    ///////////////////////////////////////////////////
    if ( Values [ section ] < threads )               {
      VarArgs V                                       ;
      V << Symbols [ j ]                              ;
      Values [ section ] ++                           ;
      start ( type , V )                              ;
      j++                                             ;
      if ( j == Symbols . count ( ) ) correct = false ;
    }                                                 ;
    ///////////////////////////////////////////////////
    Time::msleep ( msec                  )            ;
    ReportSql    ( Start , Now , section )            ;
  }                                                   ;
  /////////////////////////////////////////////////////
  while ( Values [ section ]  > 0 )                   {
    Time::msleep ( msec                  )            ;
    ReportSql    ( Start , Now , section )            ;
  }                                                   ;
}

void N::ScriptableDukascopy::Launch(int threads,int type,QString section)
{
  ThreadData * d       = drawer ( )                   ;
  bool         correct = true                         ;
  int          j       = 0                            ;
  int          msec    = 100                          ;
  QDateTime    Start   = nTimeNow                     ;
  QDateTime    Now     = nTimeNow                     ;
  /////////////////////////////////////////////////////
  Values [ section ] = 0                              ;
  while ( correct )                                   {
    if ( NULL != d )                                  {
      if ( ! IsContinue ( d ) )                       {
        correct = false                               ;
        continue                                      ;
      }                                               ;
    }                                                 ;
    ///////////////////////////////////////////////////
    if ( Values [ section ] < threads )               {
      VarArgs V                                       ;
      V << Symbols [ j ]                              ;
      Values [ section ] ++                           ;
      start ( type , V )                              ;
      j++                                             ;
      if ( j == Symbols . count ( ) ) correct = false ;
    }                                                 ;
    ///////////////////////////////////////////////////
    Time::msleep ( msec                  )            ;
    Report       ( Start , Now , section )            ;
  }                                                   ;
  /////////////////////////////////////////////////////
  while ( Values [ section ]  > 0 )                   {
    Time::msleep ( msec                  )            ;
    Report       ( Start , Now , section )            ;
  }                                                   ;
}

void N::ScriptableDukascopy::FullImport(int threads)
{
  LaunchSql ( threads , 10003 , "Import"      ) ;
}

void N::ScriptableDukascopy::Optimize(int threads)
{
  Launch    ( threads , 10004 , "Optimize"    ) ;
}

void N::ScriptableDukascopy::Investigate(int threads)
{
  Launch    ( threads , 10005 , "Investigate" ) ;
}

void N::ScriptableDukascopy::ToCSV(int threads)
{
  Launch    ( threads , 10006 , "CSV"         ) ;
}

void N::ScriptableDukascopy::CsvToSql(int threads)
{
  LaunchSql ( threads , 10007 , "CsvToSql"    ) ;
}
