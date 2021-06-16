#include <qtfinance.h>

#define ManagerMutex  MutexArray [ 0          ]
#define DealerMutex   MutexArray [ 1          ]
#define ListMutex     MutexArray [ 2          ]
#define PumpMutex     MutexArray [ 3          ]
#define FlowMutex     MutexArray [ 4          ]
#define UserMutex     MutexArray [ 5          ]
#define TransmitMutex MutexArray [ 6          ]
#define OrderMutex    MutexArray [ 7          ]
#define HistoryMutex  MutexArray [ 8          ]
#define ApiMutex(id)  MutexArray [ ( id - 1 ) ]

N::MtFeeder:: MtFeeder    ( QObject * parent )
            : QObject     (           parent )
            , QScriptable (                  )
            , Thread      ( 0       , false  )
            , Enabler     (                  )
            , QtMtAPI     (                  )
{
  setEnabled ( "Connected"        , false )                       ;
  setEnabled ( "KeepConnected"    , false )                       ;
  setEnabled ( "Explain"          , true  )                       ;
  setEnabled ( "GUI"              , false )                       ;
  setEnabled ( "ShowTicks"        , true  )                       ;
  setEnabled ( "CheckTradings"    , true  )                       ;
  setEnabled ( "CheckBidAsk"      , true  )                       ;
  setEnabled ( "CheckingBidAsk"   , false )                       ;
  setEnabled ( "UpdateBidAsk"     , false )                       ;
  setEnabled ( "UpdateTrades"     , false )                       ;
  setEnabled ( "CheckingTrades"   , false )                       ;
  setEnabled ( "Pumping"          , false )                       ;
  setEnabled ( "SymbolsTimeout"   , false )                       ;
  setEnabled ( "ObtainNews"       , false )                       ;
  setEnabled ( "ObtainNewsBody"   , false )                       ;
  setEnabled ( "ObtainOnlines"    , false )                       ;
  setEnabled ( "UpdateGroups"     , false )                       ;
  setEnabled ( "UpdateUsers"      , false )                       ;
  setEnabled ( "PumpingPing"      , false )                       ;
  setEnabled ( "PingTransmission" , false )                       ;
  setEnabled ( "SyncUsers"        , false )                       ;
  setEnabled ( "ReadOnly"         , true  )                       ;
  setEnabled ( "OrderEA"          , false )                       ;
  /////////////////////////////////////////////////////////////////
  Engines [ -1 ] = qobject_cast<QScriptEngine *>(parent)          ;
  /////////////////////////////////////////////////////////////////
  Scripts . Creators [ "FTP"           ] = FtpAttachement         ;
  Scripts . Creators [ "SQL"           ] = SqlAttachment          ;
  Scripts . Creators [ "File"          ] = FileAttachment         ;
  Scripts . Creators [ "Directory"     ] = DirectoryAttachment    ;
  Scripts . Creators [ "Socket"        ] = SocketAttachment       ;
  Scripts . Creators [ "Settings"      ] = SettingsAttachment     ;
  Scripts . Creators [ "ByteArray"     ] = ByteArrayAttachment    ;
  Scripts . Creators [ "ScriptStrings" ] = ScriptCreateString     ;
  Scripts . Creators [ "Mutex"         ] = MutexAttachment        ;
  Scripts . Creators [ "Transformer"   ] = TransformerAttachment  ;
  Scripts . Creators [ "MtAPI"         ] = MtFeederAttachment     ;
  Scripts . Creators [ "Dukascopy"     ] = DukascopyCreate        ;
  Scripts . Creators [ "RpcServer"     ] = RPC::ServerAttachement ;
  Scripts . Creators [ "RpcClient"     ] = RPC::ClientAttachement ;
  /////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( EmitGroups  ( ) )                    ,
             this , SLOT   ( SendGroups  ( ) )                  ) ;
  nConnect ( this , SIGNAL ( EmitUsers   ( ) )                    ,
             this , SLOT   ( SendUsers   ( ) )                  ) ;
  nConnect ( this , SIGNAL ( EmitOnlines ( ) )                    ,
             this , SLOT   ( SendOnlines ( ) )                  ) ;
}

N::MtFeeder::~MtFeeder (void)
{
}

QString N::MtFeeder::TradeString (int cmd)
{
  switch ( cmd )            {
    case OP_BUY             :
    return tr("Buy")        ;
    case OP_SELL            :
    return tr("Sell")       ;
    case OP_BUY_LIMIT       :
    return tr("Buy Limit")  ;
    case OP_SELL_LIMIT      :
    return tr("Sell Limit") ;
    case OP_BUY_STOP        :
    return tr("Buy Stop")   ;
    case OP_SELL_STOP       :
    return tr("Sell Stop")  ;
    case OP_BALANCE         :
    return tr("Balance")    ;
    case OP_CREDIT          :
    return tr("Credit")     ;
  }                         ;
  return ""                 ;
}

void N::MtFeeder::run(int type,N::ThreadData * data)
{
  switch ( type )                                     {
    case 10001                                        :
      KeepConnected  ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10002                                        :
      KeepQuotes     ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10010                                        :
      MonitorSilence ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10021                                        :
      MaintainEquity ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10022                                        :
      MaintainHealth ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10023                                        :
      HealthEquity   ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10024                                        :
      ForcelyClose   ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10025                                        :
      EquityCheck    ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10026                                        :
      Time::msleep   ( 500                          ) ;
      OrdersSync     ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10027                                        :
      Time::msleep   ( 1000                         ) ;
      HistorySync    ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 10028                                        :
      ServerJournal  ( data -> Arguments , data->Id ) ;
    break                                             ;
    case 11001                                        :
      SyncUsers      (                              ) ;
    break                                             ;
    case 11002                                        :
      GetOnlines     (                              ) ;
    break                                             ;
    case 12001                                        :
      if ( data -> Arguments . count ( ) > 3 )        {
        QString   symbol                              ;
        QDateTime CTM                                 ;
        double    bid                                 ;
        double    ask                                 ;
        symbol = data->Arguments[0].toString  ()      ;
        CTM    = data->Arguments[1].toDateTime()      ;
        bid    = data->Arguments[2].toDouble  ()      ;
        ask    = data->Arguments[3].toDouble  ()      ;
        TransmitQuote ( symbol , CTM , bid , ask )    ;
      }                                               ;
    break                                             ;
    case 12002                                        :
      if ( data -> Arguments . count ( ) > 3 )        {
        QString   symbol                              ;
        QDateTime CTM                                 ;
        double    bid                                 ;
        double    ask                                 ;
        symbol = data->Arguments[0].toString  ()      ;
        CTM    = data->Arguments[1].toDateTime()      ;
        bid    = data->Arguments[2].toDouble  ()      ;
        ask    = data->Arguments[3].toDouble  ()      ;
        OrdersQuote   ( symbol , CTM , bid , ask )    ;
      }                                               ;
    break                                             ;
    default                                           :
      Extras ( type , data )                          ;
    break                                             ;
  }                                                   ;
}

void N::MtFeeder::Extras(int type,N::ThreadData * data)
{
}

bool N::MtFeeder::isDrop(void)
{
  if ( ! isEnabled ("Connected"    ) ) return true ;
  if ( ! isEnabled ("KeepConnected") ) return true ;
  return false                                     ;
}

// keep equity health interval callback multiply keyword PermitToClose
// keep connected interval pumping
// keep quotes ip pumping
// keep journal report interval callback mode duration timezone filter
// start equity maintenance interval callback multiply keyword
// start equity health interval callback multiply fromlist tolist
// start symbols timeout msecs interval callback
// close account orders forcely interval keyword callback
// close account orders equity start-date interval keyword callback commentContains
// sync orders interval
// sync history interval start-date
bool N::MtFeeder::Start(QString command)
{
  QStringList s = N::CommandTokens ( command )    ;
  QString     p                                   ;
  QString     l                                   ;
  N::VarArgs  args                                ;
  if ( s . count ( ) <= 0 ) return false          ;
  p = s . first   (   )                           ;
  s . takeAt      ( 0 )                           ;
  l = p . toLower (   )                           ;
  /////////////////////////////////////////////////
  if ( "keep" == l && ( s  .count ( ) > 0 ) )     {
    if ( s . count ( ) > 1 )                      {
      l = s . first   ( )                         ;
      l = l . toLower ( )                         ;
      /////////////////////////////////////////////
      if ( "connected" == l )                     { // keep connected
        s . takeAt ( 0 )                          ;
        if ( s . count ( ) > 0 ) args << s        ;
        start ( 10001 , args )                    ;
        return true                               ;
      }                                           ;
      if ( "quotes" == l )                        { // keep quotes
        s . takeAt ( 0 )                          ;
        if ( s . count ( ) > 0 ) args << s        ;
        start ( 10002 , args )                    ;
        return true                               ;
      }                                           ;
      if ( "equity" == l )                        {
        l = s [ 1 ]                               ;
        l = l . toLower ( )                       ;
        if ( "health" == l )                      { // keep equity health interval callback multiply keyword PermitToClose
          s . takeAt ( 0 )                        ;
          s . takeAt ( 0 )                        ;
          if ( s . count ( ) > 0 ) args << s      ;
          start ( 10023 , args )                  ;
          return true                             ;
        }                                         ;
      }                                           ;
      if ( "journal" == l )                       { // keep journal report interval callback mode duration filter
        l = s [ 1 ]                               ;
        l = l . toLower ( )                       ;
        if ( "report" == l )                      {
          s . takeAt ( 0 )                        ;
          s . takeAt ( 0 )                        ;
          if ( s . count ( ) > 0 ) args << s      ;
          start ( 10028 , args )                  ;
          return true                             ;
        }                                         ;
      }                                           ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  if ( "start" == l && ( s  .count ( ) > 0 ) )    {
    if ( s . count ( ) > 1 )                      {
      l = s . first   ( )                         ;
      l = l . toLower ( )                         ;
      /////////////////////////////////////////////
      if ( "equity" == l )                        {
        l = s [ 1 ]                               ;
        l = l . toLower ( )                       ;
        if ( "maintenance" == l )                 { // start equity maintenance interval callback multiply keyword
          s . takeAt ( 0 )                        ;
          s . takeAt ( 0 )                        ;
          if ( s . count ( ) > 0) args << s       ;
          TriggerEquity ( args )                  ;
          return true                             ;
        }                                         ;
        if ( "health" == l )                      { // start equity health interval callback multiply fromlist tolist
          s . takeAt ( 0 )                        ;
          s . takeAt ( 0 )                        ;
          if ( s . count ( ) > 0) args << s       ;
          TriggerHealth ( args )                  ;
          return true                             ;
        }                                         ;
      }                                           ;
      /////////////////////////////////////////////
      if ( "symbols" == l )                       { // start symbols timeout msecs interval callback
        l = s [ 1 ]                               ;
        l = l . toLower ( )                       ;
        if ( "timeout" == l )                     {
          s . takeAt ( 0 )                        ;
          s . takeAt ( 0 )                        ;
          if ( s . count ( ) > 0) args << s       ;
          SymbolsTimeout ( args )                 ;
          return true                             ;
        }                                         ;
      }                                           ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  if ( "close"  == l && ( s  .count ( ) > 0 ) )   {
    if ( s . count ( ) > 1 )                      {
      l = s . first   ( )                         ;
      l = l . toLower ( )                         ;
      /////////////////////////////////////////////
      if ( "account" == l && ( s.count() > 1) )   {
        l = s [ 1 ]                               ;
        l = l . toLower ( )                       ;
        if ( "orders" == l  && ( s.count() > 2))  {
          l = s [ 2 ]                             ;
          l = l . toLower ( )                     ;
          if ( "forcely" == l )                   { // close account orders forcely interval keyword callback
            s . takeAt ( 0 )                      ;
            s . takeAt ( 0 )                      ;
            s . takeAt ( 0 )                      ;
            if ( s . count ( ) > 0) args << s     ;
            TriggerCloseAll ( args )              ;
            return true                           ;
          }                                       ;
          if ( "equity" == l )                    { // close account orders equity start-date interval keyword callback
            s . takeAt ( 0 )                      ;
            s . takeAt ( 0 )                      ;
            s . takeAt ( 0 )                      ;
            if ( s . count ( ) > 0) args << s     ;
            TriggerEquityClose ( args )           ;
            return true                           ;
          }                                       ;
        }                                         ;
      }                                           ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  if ( "sync"  == l && ( s  .count ( ) > 0 ) )    {
    if ( s . count ( ) > 1 )                      {
      l = s . first   ( )                         ;
      l = l . toLower ( )                         ;
      /////////////////////////////////////////////
      if ( "orders"  == l && ( s.count() > 1) )   {
        s . takeAt ( 0 )                          ;
        if ( s . count ( ) > 0 ) args << s        ;
        start ( 10026 , args )                    ;
      }                                           ;
      /////////////////////////////////////////////
      if ( "history" == l && ( s.count() > 1) )   {
        s . takeAt ( 0 )                          ;
        if ( s . count ( ) > 0 ) args << s        ;
        start ( 10027 , args )                    ;
      }                                           ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  if ( "stop"  == l && ( s  .count ( ) > 0 ) )    {
  }                                               ;
  /////////////////////////////////////////////////
  return false                                    ;
}

void N::MtFeeder::report(QString msg)
{
  if ( ! isEnabled ( "Explain" ) ) return ;
  if (   isEnabled ( "GUI" )     )        {
  } else                                  {
    N::printf      ( msg , true , true )  ;
  }                                       ;
}

void N::MtFeeder::TickAppend(QString symbol)
{
  RemoveTransmitters ( )        ;
  TickTransmitter * tt          ;
  TransmitMutex . lock   ( )    ;
  foreach ( tt , Transmitters ) {
    tt -> Append ( symbol )     ;
  }                             ;
  TransmitMutex . unlock ( )    ;
}

void N::MtFeeder::TickDigits(QString symbol,int digits)
{
  RemoveTransmitters ( )               ;
  TickTransmitter * tt                 ;
  TransmitMutex . lock   ( )           ;
  foreach ( tt , Transmitters )        {
    tt -> setDigits( symbol , digits ) ;
  }                                    ;
  TransmitMutex . unlock ( )           ;
}

void N::MtFeeder::TickHide(QString symbol)
{
  RemoveTransmitters ( )        ;
  TickTransmitter * tt          ;
  TransmitMutex . lock   ( )    ;
  foreach ( tt , Transmitters ) {
    tt -> Hide ( symbol )       ;
  }                             ;
  TransmitMutex . unlock ( )    ;
}

void N::MtFeeder::TickQuote(QString symbol,QDateTime CTM,double bid,double ask)
{
  TransmitQuote ( symbol , CTM , bid , ask ) ;
  OrdersQuote   ( symbol , CTM , bid , ask ) ;
}

void N::MtFeeder::TransmitQuote(QString symbol,QDateTime CTM,double bid,double ask)
{
  RemoveTransmitters ( )                     ;
  TickTransmitter * tt                       ;
  ////////////////////////////////////////////
  TransmitMutex . lock   ( )                 ;
  foreach ( tt , Transmitters )              {
    tt -> Quote ( symbol , CTM , bid , ask ) ;
  }                                          ;
  TransmitMutex . unlock ( )                 ;
}

void N::MtFeeder::OrdersQuote(QString symbol,QDateTime CTM,double bid,double ask)
{
  ////////////////////////////////////////////////
  if ( isEnabled("OrderEA") )                    {
    OrderMutex . lock   ( )                      ;
    for (int i=0;i<allOrders.count();i++)        {
      Finance::Orders * orders = allOrders [ i ] ;
      if ( NULL != orders )                      {
        QScriptEngine * e = orders -> Engine ( ) ;
        if ( NULL != e )                         {
          QScriptValue api                       ;
          api = e -> newQObject ( this )         ;
          orders -> runEngine                    (
            api                                  ,
            symbol                               ,
            CTM                                  ,
            bid                                  ,
            ask                                ) ;
        }                                        ;
      }                                          ;
    }                                            ;
    OrderMutex . unlock ( )                      ;
  }                                              ;
}

void N::MtFeeder::TickUpdate(void)
{
  RemoveTransmitters ( )        ;
  TickTransmitter * tt          ;
  TransmitMutex . lock   ( )    ;
  foreach ( tt , Transmitters ) {
    tt -> Update ( )            ;
  }                             ;
  TransmitMutex . unlock ( )    ;
}

void N::MtFeeder::TickReady(void)
{
  RemoveTransmitters ( )        ;
  TickTransmitter * tt          ;
  TransmitMutex . lock   ( )    ;
  foreach ( tt , Transmitters ) {
    tt -> Ready ( )             ;
  }                             ;
  TransmitMutex . unlock ( )    ;
}

int N::MtFeeder::addTransmitter(TickTransmitter * transmitter)
{
  if ( ! Transmitters . contains ( transmitter ) ) {
    TransmitMutex . lock   ( )                     ;
    Transmitters << transmitter                    ;
    TransmitMutex . unlock ( )                     ;
  }                                                ;
  return Transmitters . count ( )                  ;
}

int N::MtFeeder::addChannels(MtChannels * channels)
{
  if ( ! ByteChannels . contains ( channels ) ) {
    FlowMutex .   lock ( )                      ;
    ByteChannels << channels                    ;
    FlowMutex . unlock ( )                      ;
  }                                             ;
  return ByteChannels . count ( )               ;
}

void N::MtFeeder::RemoveChannels(void)
{
  QList<MtChannels *> MCs                          ;
  MtChannels * mc                                  ;
  foreach ( mc , ByteChannels )                    {
    if ( mc -> Value ( "Deletion" ) . toBool ( ) ) {
      MCs << mc                                    ;
    }                                              ;
  }                                                ;
  //////////////////////////////////////////////////
  if ( MCs . count ( ) <= 0 ) return               ;
  //////////////////////////////////////////////////
  FlowMutex .   lock ( )                           ;
  foreach ( mc , MCs )                             {
    int index                                      ;
    index = ByteChannels . indexOf ( mc )          ;
    if ( index >= 0 )                              {
      ByteChannels . takeAt ( index )              ;
    }                                              ;
    delete mc                                      ;
  }                                                ;
  FlowMutex . unlock ( )                           ;
}

void N::MtFeeder::RemoveTransmitters(void)
{
  QList<TickTransmitter *> TTs            ;
  TickTransmitter * tt                    ;
  foreach ( tt , Transmitters )           {
    if ( tt -> isEnabled ( "Deletion" ) ) {
      TTs << tt                           ;
    }                                     ;
  }                                       ;
  /////////////////////////////////////////
  if ( TTs . count ( ) <= 0 ) return      ;
  /////////////////////////////////////////
  TransmitMutex . lock   ( )              ;
  foreach ( tt , TTs )                    {
    int index                             ;
    index = Transmitters . indexOf ( tt ) ;
    if ( index >= 0 )                     {
      Transmitters . takeAt ( index )     ;
    }                                     ;
    delete tt                             ;
  }                                       ;
  TransmitMutex . unlock ( )              ;
}

int N::MtFeeder::addUsers(Finance::Users * users)
{
  if ( ! allUsers . contains ( users ) ) {
    UserMutex . lock   ( )               ;
    allUsers << users                    ;
    UserMutex . unlock ( )               ;
  }                                      ;
  return allUsers . count ( )            ;
}

void N::MtFeeder::RemoveUsers(void)
{
  QList<Finance::Users *> TTs                        ;
  Finance::Users * tt                                ;
  foreach ( tt , allUsers )                          {
    if ( tt -> functions.hasEnabled ( "Deletion" ) ) {
      TTs << tt                                      ;
    }                                                ;
  }                                                  ;
  ////////////////////////////////////////////////////
  if ( TTs . count ( ) <= 0 ) return                 ;
  ////////////////////////////////////////////////////
  UserMutex . lock   ( )                             ;
  foreach ( tt , TTs )                               {
    int index                                        ;
    index = allUsers . indexOf ( tt )                ;
    if ( index >= 0 )                                {
      allUsers . takeAt ( index )                    ;
    }                                                ;
    delete tt                                        ;
  }                                                  ;
  UserMutex . unlock ( )                             ;
}

QString N::MtFeeder::GroupOfUser(int api,int login)
{
  QString grp                     ;
  ApiMutex ( api ) . lock   ( )   ;
  grp = UserGroup ( api , login ) ;
  ApiMutex ( api ) . unlock ( )   ;
  return grp                      ;
}

int N::MtFeeder::addOrders(Finance::Orders * orders)
{
  if ( ! allOrders . contains ( orders ) ) {
    OrderMutex . lock   ( )                ;
    allOrders << orders                    ;
    OrderMutex . unlock ( )                ;
  }                                        ;
  return allOrders . count ( )             ;
}

void N::MtFeeder::RemoveOrders(void)
{
  QList<Finance::Orders *> TTs                       ;
  Finance::Orders * tt                               ;
  foreach ( tt , allOrders )                         {
    if ( tt -> functions.hasEnabled ( "Deletion" ) ) {
      TTs << tt                                      ;
    }                                                ;
  }                                                  ;
  ////////////////////////////////////////////////////
  if ( TTs . count ( ) <= 0 ) return                 ;
  ////////////////////////////////////////////////////
  OrderMutex . lock   ( )                            ;
  foreach ( tt , TTs )                               {
    int index                                        ;
    index = allOrders . indexOf ( tt )               ;
    if ( index >= 0 )                                {
      allOrders . takeAt ( index )                   ;
    }                                                ;
    delete tt                                        ;
  }                                                  ;
  OrderMutex . unlock ( )                            ;
}

int N::MtFeeder::addHistory(Finance::Orders * orders)
{
  if ( ! allHistory . contains ( orders ) ) {
    HistoryMutex . lock   ( )               ;
    allHistory << orders                    ;
    HistoryMutex . unlock ( )               ;
  }                                         ;
  return allHistory . count ( )             ;
}

void N::MtFeeder::RemoveHistory(void)
{
  QList<Finance::Orders *> TTs                       ;
  Finance::Orders * tt                               ;
  foreach ( tt , allHistory )                        {
    if ( tt -> functions.hasEnabled ( "Deletion" ) ) {
      TTs << tt                                      ;
    }                                                ;
  }                                                  ;
  ////////////////////////////////////////////////////
  if ( TTs . count ( ) <= 0 ) return                 ;
  ////////////////////////////////////////////////////
  OrderMutex . lock   ( )                            ;
  foreach ( tt , TTs )                               {
    int index                                        ;
    index = allHistory . indexOf ( tt )              ;
    if ( index >= 0 )                                {
      allHistory . takeAt ( index )                  ;
    }                                                ;
    delete tt                                        ;
  }                                                  ;
  OrderMutex . unlock ( )                            ;
}

bool N::MtFeeder::SetEnabled(int Id,bool enable)
{
  return N::Enabler::setEnabled ( Id , enable ) ;
}

bool N::MtFeeder::IsEnabled(int Id)
{
  return N::Enabler::isEnabled ( Id ) ;
}

bool N::MtFeeder::SetEnabled(QString Id,bool enable)
{
  return N::Enabler::setEnabled ( Id , enable ) ;
}

bool N::MtFeeder::IsEnabled(QString Id)
{
  return N::Enabler::isEnabled ( Id ) ;
}

void N::MtFeeder::Interfaces (int APIs)
{
  QtMtAPI :: Initialize ( APIs ) ;
}

void N::MtFeeder::setLogin(QString account,QString pass)
{
  Account  = account ;
  Password = pass    ;
}

bool N::MtFeeder::ConnectTo(QString ip,bool PumpingEx)
{
  return QtMtAPI :: FullMount ( ip , PumpingEx ) ;
}

bool N::MtFeeder::IsConnected (void)
{
  bool connected = QtMtAPI :: isConnected ( ) ;
  setEnabled ( "Connected" , connected )      ;
  return connected                            ;
}

void N::MtFeeder::Disconnect (void)
{
  setEnabled ( "Connected"     , false ) ;
  setEnabled ( "KeepConnected" , false ) ;
  QtMtAPI :: disconnect ( )              ;
  BringDown             ( )              ;
}

int N::MtFeeder::SrvRestart(int api)
{
  SafeMtAPI ( api , mi , 0 )  ;
  return mi -> SrvRestart ( ) ;
}

int N::MtFeeder::SrvChartsSync(int api)
{
  SafeMtAPI ( api , mi , 0 )     ;
  return mi -> SrvChartsSync ( ) ;
}

int N::MtFeeder::SrvLiveUpdateStart(int api)
{
  SafeMtAPI ( api , mi , 0 )          ;
  return mi -> SrvLiveUpdateStart ( ) ;
}

int N::MtFeeder::SrvFeedsRestart(int api)
{
  SafeMtAPI ( api , mi , 0 )       ;
  return mi -> SrvFeedsRestart ( ) ;
}

int N::MtFeeder::Ping(int api)
{
  SafeMtAPI ( api , mi , 0 ) ;
  return mi -> Ping ( )      ;
}

int N::MtFeeder::AddRef(int api)
{
  SafeMtAPI ( api , mi , 0 ) ;
  return mi -> AddRef ( )    ;
}

int N::MtFeeder::Release(int api)
{
  SafeMtAPI ( api , mi , 0 ) ;
  return mi -> Release ( )   ;
}

int N::MtFeeder::NewsTotal(int api)
{
  SafeMtAPI ( api , mi , 0 ) ;
  return mi -> NewsTotal ( ) ;
}

int N::MtFeeder::BytesSent(int api)
{
  CManagerInterface * mi = API ( (MtApiFlags) api ) ;
  if ( NULL == mi ) return 0                        ;
  int bs = mi -> BytesSent ( )                      ;
  if ( bs < 0 ) bs = 0                              ;
  return bs                                         ;
}

int N::MtFeeder::BytesReceived(int api)
{
  CManagerInterface * mi = API ( (MtApiFlags) api ) ;
  if ( NULL == mi ) return 0                        ;
  int br = mi -> BytesReceived ( )                  ;
  if ( br < 0 ) br = 0                              ;
  return br                                         ;
}

QDateTime N::MtFeeder::ServerTime(int api)
{
  SafeMtAPI                    ( api , mi , nTimeNow ) ;
  time_t t = mi -> ServerTime  (                     ) ;
  return QDateTime::fromTime_t ( t ,Qt::UTC          ) ;
}

int N::MtFeeder::SymbolsRefresh(int API)
{
  symbols = Symbols ( (MtApiFlags)API ) ;
  return symbols . count ( )            ;
}

int N::MtFeeder::TotalSymbols(void)
{
  return symbols . count ( ) ;
}

QString N::MtFeeder::Symbol(int at)
{
  if ( at >= symbols.count() ) return "" ;
  return symbols [ at ]                  ;
}

bool N::MtFeeder::hasSymbol(QString symbol)
{
  return symbols . contains ( symbol ) ;
}

QStrings N::MtFeeder::AllSymbols(void)
{
  return symbols ;
}

int N::MtFeeder::SymbolAdd(int api,QString symbol)
{
  if (symbols.count()<0) return 0                                   ;
  if (!isConnected((MtApiFlags)api)) return 0                       ;
  CManagerInterface * cpi = API ((MtApiFlags) api )                 ;
  CManagerInterface * cmi = API ( MtApiManager    )                 ;
  char              * s   = (char *)symbol.toUtf8().constData()     ;
  int                 r   = RET_OK                                  ;
  QDateTime           DT                                            ;
  if ( NULL != cpi ) r = cpi -> SymbolAdd (s)                       ;
  if ( NULL != cmi )                                                {
    DT  = QDateTime::fromTime_t ( cmi->ServerTime() , Qt::UTC , 0 ) ;
  } else                                                            {
    DT = nTimeNow                                                   ;
  }                                                                 ;
  LastestTicks [ symbol ] = DT                                      ;
  Lastest                 = DT                                      ;
  return r                                                          ;
}

bool N::MtFeeder::CloseOrder(int api,int order,double lots)
{
  QString comment                                       ;
  bool    correct                                       ;
  comment = tr("Close %1 by MtJS").arg(order)           ;
  ApiMutex ( api ) . lock   ( )                         ;
  correct = QtMtAPI::CloseTrade(api,order,lots,comment) ;
  ApiMutex ( api ) . unlock ( )                         ;
  return correct                                        ;
}

bool N::MtFeeder::CloseUserOrders(int api,int login)
{
  QString comment = tr("Close %1 by MtJS")             ;
  bool    correct                                      ;
  ApiMutex ( api ) . lock   ( )                        ;
  correct = QtMtAPI::CloseAllOrders(api,login,comment) ;
  ApiMutex ( api ) . unlock ( )                        ;
  return correct                                       ;
}

bool N::MtFeeder::setReadOnly(int login,bool readOnly)
{
  bool correct                                                ;
  ApiMutex ( MtApiManager ) . lock   ( )                      ;
  correct = QtMtAPI::setReadOnly(MtApiManager,login,readOnly) ;
  ApiMutex ( MtApiManager ) . unlock ( )                      ;
  if ( ! correct )                                            {
    report ( tr( "No account info %1").arg(login) )           ;
  }                                                           ;
  return correct                                              ;
}

double N::MtFeeder::AccountDeposit(int api,int login,QDateTime from,QString comment)
{
  double balance = 0                                             ;
  ApiMutex ( api ) . lock       (                              ) ;
  balance = QtMtAPI::Depository ( api , login , from , comment ) ;
  ApiMutex ( api ) . unlock     (                              ) ;
  return balance                                                 ;
}

QString N::MtFeeder::errorString(int code)
{
  switch ( code )                                     {
    case -4                                           :
    return tr("No order available")                   ;
    case -3                                           :
    return tr("Close an order which does not exist")  ;
    case -2                                           :
    return tr("Account does not match")               ;
    case -1                                           :
    return tr("No MT4 Manager API channel available") ;
    case RET_OK                                       :
    return tr("Okay")                                 ;
    case RET_OK_NONE                                  :
    return tr("Okay none")                            ;
    case RET_ERROR                                    :
    return tr("Error")                                ;
    case RET_INVALID_DATA                             :
    return tr("Invalid data")                         ;
    case RET_TECH_PROBLEM                             :
    return tr("Technic problem")                      ;
    case RET_OLD_VERSION                              :
    return tr("Old version")                          ;
    case RET_NO_CONNECT                               :
    return tr("No connection")                        ;
    case RET_NOT_ENOUGH_RIGHTS                        :
    return tr("Not enough rights")                    ;
    case RET_TOO_FREQUENT                             :
    return tr("Too frequent")                         ;
    case RET_MALFUNCTION                              :
    return tr("Malfunction")                          ;
    case RET_GENERATE_KEY                             :
    return tr("Generate key")                         ;
    case RET_SECURITY_SESSION                         :
    return tr("Security session")                     ;
    case RET_ACCOUNT_DISABLED                         :
    return tr("Account disabled")                     ;
    case RET_BAD_ACCOUNT_INFO                         :
    return tr("Bad account information")              ;
    case RET_PUBLIC_KEY_MISSING                       :
    return tr("Public key missing")                   ;
    case RET_TRADE_TIMEOUT                            :
    return tr("Trade bad timeout.")                   ;
    case RET_TRADE_BAD_PRICES                         :
    return tr("Trade bad prices.")                    ;
    case RET_TRADE_BAD_STOPS                          :
    return tr("Trade bad stops.")                     ;
    case RET_TRADE_BAD_VOLUME                         :
    return tr("Trade bad volume.")                    ;
    case RET_TRADE_MARKET_CLOSED                      :
    return tr("Trade market closed.")                 ;
    case RET_TRADE_DISABLE                            :
    return tr("Trade disable.")                       ;
    case RET_TRADE_NO_MONEY                           :
    return tr("Trade no money.")                      ;
    case RET_TRADE_PRICE_CHANGED                      :
    return tr("Trade price changed.")                 ;
    case RET_TRADE_OFFQUOTES                          :
    return tr("Trade off quotes.")                    ;
    case RET_TRADE_BROKER_BUSY                        :
    return tr("Trade broker busy.")                   ;
    case RET_TRADE_REQUOTE                            :
    return tr("Trade requote.")                       ;
    case RET_TRADE_ORDER_LOCKED                       :
    return tr("Trade order locked.")                  ;
    case RET_TRADE_LONG_ONLY                          :
    return tr("Trade long only.")                     ;
    case RET_TRADE_TOO_MANY_REQ                       :
    return tr("Trade too many requests.")             ;
    case RET_TRADE_ACCEPTED                           :
    return tr("Trade accepted.")                      ;
    case RET_TRADE_PROCESS                            :
    return tr("Trade process.")                       ;
    case RET_TRADE_USER_CANCEL                        :
    return tr("Trade user cancel.")                   ;
    case RET_TRADE_MODIFY_DENIED                      :
    return tr("Trade modify denied.")                 ;
    case RET_TRADE_CONTEXT_BUSY                       :
    return tr("Trade context busy.")                  ;
    case RET_TRADE_EXPIRATION_DENIED                  :
    return tr("Trade expiration denied.")             ;
    case RET_TRADE_TOO_MANY_ORDERS                    :
    return tr("Trade too many orders.")               ;
    case RET_TRADE_HEDGE_PROHIBITED                   :
    return tr("Trade hedge prohibited.")              ;
    case RET_TRADE_PROHIBITED_BY_FIFO                 :
    return tr("Trade prohibited by FIFO.")            ;
  }                                                   ;
  return ""                                           ;
}

bool N::MtFeeder::PlaceOrder (
       int          api      ,
       int          login    ,
       QString      symbol   ,
       int          type     ,
       int          cmd      ,
       int          volume   ,
       double       price    ,
       double       sl       ,
       double       tp       ,
       QString      comment  ,
       N::VarArgs & results  )
{
  CManagerInterface * cmi = API ( (MtApiFlags) api )                         ;
  if ( NULL == cmi )                                                         {
    results << RET_NO_CONNECT                                                ;
    results << errorString(-1)                                               ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QByteArray     C = comment . toUtf8 ( )                                    ;
  TradeTransInfo TTI                                                         ;
  int            result                                                      ;
  char           cmt [ 32 ]                                                  ;
  ::memset ( &TTI , 0 , sizeof(TradeTransInfo) )                             ;
  ::memset ( cmt  , 0 , 32                     )                             ;
  if ( C . size ( ) > 31 ) C = C . left ( 31 )                               ;
  if ( C . size ( ) > 0  ) ::memcpy ( cmt , C . data() , C . size ( ) )      ;
  TTI . orderby = login                                                      ;
  TTI . type    = type                                                       ;
  TTI . cmd     = cmd                                                        ;
  TTI . volume  = volume                                                     ;
  TTI . price   = price                                                      ;
  TTI . sl      = sl                                                         ;
  TTI . tp      = tp                                                         ;
  ::strcpy ( TTI . symbol  , symbol  . toUtf8 ( ) . constData ( ) )          ;
  ::memcpy ( TTI . comment , cmt , 32                             )          ;
  ApiMutex ( api ) . lock   ( )                                              ;
  result = cmi -> TradeTransaction ( &TTI )                                  ;
  ApiMutex ( api ) . unlock ( )                                              ;
  results << result                                                          ;
  results << errorString ( result )                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return nEqual ( result , RET_OK )                                          ;
}

bool N::MtFeeder::CloseOrder (
       int          api      ,
       int          login    ,
       int          order    ,
       double       lots     ,
       QString      c        ,
       N::VarArgs & results  )
{
  CManagerInterface * cmi = API ( (MtApiFlags) api )                         ;
  if ( NULL == cmi )                                                         {
    results << RET_NO_CONNECT                                                ;
    results << errorString ( -1 )                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  TradeRecord   TRX                                                          ;
  TradeRecord * TRU                                                          ;
  int           r                                                            ;
  int           total = 1                                                    ;
  ApiMutex ( api ) . lock   ( )                                              ;
  TRU = cmi -> TradeRecordsRequest ( &order , &total )                       ;
  ApiMutex ( api ) . unlock ( )                                              ;
  if ( ( total <= 0 ) || ( NULL == TRU ) )                                   {
    results << -3                                                            ;
    results << tr("order %1 does not exist.").arg(order)                     ;
    return false                                                             ;
  }                                                                          ;
  ::memcpy ( &TRX , TRU , sizeof(TradeRecord) )                              ;
  ApiMutex ( api ) . lock   ( )                                              ;
  cmi -> MemFree ( TRU )                                                     ;
  ApiMutex ( api ) . unlock ( )                                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( login != TRX . login )                                                {
    QString es                                                               ;
    results << -2                                                            ;
    es = tr  ( "Order %1 is owned by %2, but given login %3 is not the same" )
        .arg ( order                                                         )
        .arg ( TRX . login                                                   )
        .arg ( login                                                       ) ;
    results << es                                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QByteArray     C = c . toUtf8 ( )                                          ;
  TradeTransInfo TTI                                                         ;
  char           comment [ 32 ]                                              ;
  int            volume = (int) ( lots * 100 )                               ;
  ::memset ( &TTI    , 0 , sizeof(TradeTransInfo) )                          ;
  ::memset ( comment , 0 , 32                     )                          ;
  if ( C . size ( ) > 31 ) C = C . left ( 31 )                               ;
  if ( C . size ( ) > 0  ) ::memcpy ( comment , C . data() , C . size ( ) )  ;
  TTI . order   = TRX . order                                                ;
  TTI . orderby = TRX . login                                                ;
  TTI . cmd     = TRX . cmd                                                  ;
  TTI . price   = TRX . close_price                                          ;
  if ( ( lots < 0 ) || ( volume >= TRX . volume ) )                          {
    TTI . type    = TT_BR_ORDER_CLOSE                                        ;
    TTI . volume  = TRX . volume                                             ;
  } else                                                                     {
    TTI . type    = TT_BR_ORDER_CLOSE                                        ;
    TTI . volume  = volume                                                   ;
  }                                                                          ;
  ::strcpy ( TTI . symbol  , TRX . symbol )                                  ;
  ::memcpy ( TTI . comment , comment , 32 )                                  ;
  ////////////////////////////////////////////////////////////////////////////
  ApiMutex ( api ) . lock   ( )                                              ;
  r = cmi -> TradeTransaction ( &TTI )                                       ;
  ApiMutex ( api ) . unlock ( )                                              ;
  results << r                                                               ;
  results << errorString ( r  )                                              ;
  return ( r == RET_OK )                                                     ;
}

bool N::MtFeeder::CloseAllOrders (
       int          login        ,
       QString      group        ,
       QString      c            ,
       N::VarArgs & results      )
{
  CManagerInterface * cpi = API ( MtApiPump    )                             ;
  CManagerInterface * cmi = API ( MtApiManager )                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ( NULL == cmi ) || ( NULL == cpi ) )                                  {
    results << RET_NO_CONNECT                                                ;
    results << errorString ( -1 )                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  TradeRecord * TRX                                                          ;
  int           total = 0                                                    ;
  QByteArray    GRP   = group . toUtf8 ( )                                   ;
  ////////////////////////////////////////////////////////////////////////////
  PumpMutex . lock   ( )                                                     ;
  TRX = cpi -> TradesGetByLogin ( login , (LPCSTR) GRP.constData() ,&total ) ;
  PumpMutex . unlock ( )                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ( total <= 0 ) || ( NULL == TRX ) )                                   {
    results << -4                                                            ;
    results << errorString ( -4 )                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  QByteArray     C = c . toUtf8 ( )                                          ;
  TradeTransInfo TTI                                                         ;
  char           comment [ 32 ]                                              ;
  ::memset ( comment , 0 , 32 )                                              ;
  ::memset ( comment , 0 , 32                     )                          ;
  if ( C . size ( ) > 31 ) C = C . left ( 31 )                               ;
  if ( C . size ( ) > 0  ) ::memcpy ( comment , C . data() , C . size ( ) )  ;
  for (int i=0;i<total;i++)                                                  {
    ::memset( &TTI , 0 , sizeof(TradeTransInfo) )                            ;
    TTI . type    = TT_BR_ORDER_CLOSE                                        ;
    TTI . order   = TRX[i] . order                                           ;
    TTI . orderby = TRX[i] . login                                           ;
    TTI . cmd     = TRX[i] . cmd                                             ;
    TTI . price   = TRX[i] . close_price                                     ;
    TTI . volume  = TRX[i] . volume                                          ;
    ::strcpy ( TTI . symbol  , TRX[i] . symbol )                             ;
    ::memcpy ( TTI . comment , comment , 32    )                             ;
    //////////////////////////////////////////////////////////////////////////
    ManagerMutex . lock   ( )                                                ;
    cmi -> TradeTransaction ( &TTI )                                         ;
    ManagerMutex . unlock ( )                                                ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  PumpMutex . lock   ( )                                                     ;
  cpi -> MemFree ( TRX )                                                     ;
  PumpMutex . unlock ( )                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::MtFeeder::ModifyOrder (
       int          api       ,
       int          login     ,
       int          order     ,
       double       sl        ,
       double       tp        ,
       QString      c         ,
       N::VarArgs & results   )
{
  CManagerInterface * cmi = API ( (MtApiFlags) api )                         ;
  if ( NULL == cmi )                                                         {
    results << RET_NO_CONNECT                                                ;
    results << errorString ( -1 )                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  TradeRecord   TRX                                                          ;
  TradeRecord * TRU                                                          ;
  int           r                                                            ;
  int           total = 1                                                    ;
  ApiMutex ( api ) . lock   ( )                                              ;
  TRU = cmi -> TradeRecordsRequest ( &order , &total )                       ;
  ApiMutex ( api ) . unlock ( )                                              ;
  if ( ( total <= 0 ) || ( NULL == TRU ) )                                   {
    results << -3                                                            ;
    results << tr("order %1 does not exist.").arg(order)                     ;
    return false                                                             ;
  }                                                                          ;
  ::memcpy ( &TRX , TRU , sizeof(TradeRecord) )                              ;
  ApiMutex ( api ) . lock   ( )                                              ;
  cmi -> MemFree ( TRU )                                                     ;
  ApiMutex ( api ) . unlock ( )                                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( login != TRX . login )                                                {
    QString es                                                               ;
    results << -2                                                            ;
    es = tr  ( "Order %1 is owned by %2, but given login %3 is not the same" )
        .arg ( order                                                         )
        .arg ( TRX . login                                                   )
        .arg ( login                                                       ) ;
    results << es                                                            ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int TT = TT_BR_ORDER_MODIFY                                                ;
  if ( ( sl < 0 ) && ( tp < 0 ) ) TT = TT_BR_ORDER_COMMENT                   ;
  if ( sl < 0 ) sl = TRX . sl                                                ;
  if ( tp < 0 ) tp = TRX . tp                                                ;
  ////////////////////////////////////////////////////////////////////////////
  QByteArray     C = c . toUtf8 ( )                                          ;
  TradeTransInfo TTI                                                         ;
  char           comment [ 32 ]                                              ;
  ::memset ( &TTI    , 0 , sizeof(TradeTransInfo) )                          ;
  ::memset ( comment , 0 , 32                     )                          ;
  if ( C . size ( ) > 31 ) C = C . left ( 31 )                               ;
  if ( C . size ( ) > 0  ) ::memcpy ( comment , C . data() , C . size ( ) )  ;
  TTI . type    = TT                                                         ;
  TTI . order   = TRX . order                                                ;
  TTI . orderby = TRX . login                                                ;
  TTI . cmd     = TRX . cmd                                                  ;
  TTI . price   = TRX . open_price                                           ;
  TTI . sl      = sl                                                         ;
  TTI . tp      = tp                                                         ;
  TTI . volume  = TRX . volume                                               ;
  ::strcpy ( TTI . symbol  , TRX . symbol )                                  ;
  ::memcpy ( TTI . comment , comment , 32 )                                  ;
  ////////////////////////////////////////////////////////////////////////////
  ApiMutex ( api ) . lock   ( )                                              ;
  r = cmi -> TradeTransaction ( &TTI )                                       ;
  ApiMutex ( api ) . unlock ( )                                              ;
  results << r                                                               ;
  results << errorString ( r  )                                              ;
  return ( r == RET_OK )                                                     ;
}

qint64 N::MtFeeder::SymbolSlience(QString symbol)
{
  if ( ! LastestTicks . contains ( symbol ) ) return -1 ;
  return LastestTicks [ symbol ] . msecsTo ( Lastest )  ;
}

void N::MtFeeder::TickUpdated (
       QString   symbol      ,
       QDateTime CTM         ,
       double    bid         ,
       double    ask         )
{
  LastestTicks [ symbol ] = CTM                           ;
  TickQuote ( symbol , CTM , bid , ask )                  ;
  if ( ! isEnabled ( "ShowTicks" ) ) return               ;
  QString   M                                             ;
  M  = QString ( "%1 %2 %3 %4"                            )
      . arg    ( symbol                                   )
      . arg    ( CTM . toString ( "yyyy/MM/dd hh:mm:ss" ) )
      . arg    ( bid                                      )
      . arg    ( ask                                    ) ;
  N::printf    ( M , true , true                        ) ;
}

void N::MtFeeder::UpdateBidAsk(int type,void * data)
{
  Q_UNUSED ( type )                          ;
  Q_UNUSED ( data )                          ;
  if ( isEnabled ( "UpdateBidAsk" ) ) return ;
  setEnabled ( "UpdateBidAsk" , true )       ;
  ////////////////////////////////////////////
  if ( isEnabled ( "CheckBidAsk" ) )         {
    if ( ! isEnabled ( "CheckingBidAsk" ) )  {
      CheckBidAsk ( )                        ;
    }                                        ;
  }                                          ;
  ////////////////////////////////////////////
  setEnabled ( "UpdateBidAsk" , false )      ;
}

void N::MtFeeder::CheckBidAsk(void)
{
  if (   symbols . count ( ) < 0   ) return                     ;
  if ( ! isConnected ( MtApiPump ) ) return                     ;
  setEnabled ( "CheckingBidAsk" , true )                        ;
  CManagerInterface * cmi = API (MtApiPump)                     ;
  ///////////////////////////////////////////////////////////////
  if ( isEnabled ( "ShowTicks" ) )                              {
    report ( "==============================================" ) ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  QString s                                                     ;
  int     cxx = 0                                               ;
  foreach ( s , symbols )                                       {
    int        total = 0                                        ;
    char     * v     = (char *)s.toUtf8().constData()           ;
    TickInfo * ti                                               ;
    PumpMutex . lock  ( )                                       ;
    ti = cmi->TickInfoLast((LPCSTR)v,&total)                    ;
    if ( ( total > 0 ) && ( NULL != ti ) )                      {
      QDateTime DT                                              ;
      for (int i = 0;i<total;i++)                               {
        QString z ( ti->symbol )                                ;
        DT = QDateTime::fromTime_t(ti->ctm,Qt::UTC,0)           ;
        Lastest = DT                                            ;
        TickUpdated ( z , DT , ti -> bid , ti -> ask )          ;
        cxx ++                                                  ;
      }                                                         ;
    }                                                           ;
    if ( NULL != ti ) cmi -> MemFree ( ti )                     ;
    PumpMutex . unlock ( )                                      ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  if ( isEnabled ( "ShowTicks" ) )                              {
    QString m                                                   ;
    m = QString("=========== %1%2 symbols updated ===========").arg(cxx/10).arg(cxx%10) ;
    report ( m )                                                ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  setEnabled ( "CheckingBidAsk" , false )                       ;
}

void N::MtFeeder::UpdateTrades(int type,void * data)
{
  if ( isEnabled ( "UpdateTrades" ) ) return ;
  setEnabled ( "UpdateTrades" , true  )      ;
  if ( isEnabled     ( "CheckTradings"  ) )  {
    if ( ! isEnabled ( "CheckingTrades" ) )  {
      CheckTradings  ( type               )  ;
    }                                        ;
  }                                          ;
  setEnabled ( "UpdateTrades" , false )      ;
}

void N::MtFeeder::CheckTradings(int type)
{
  if (   symbols . count ( ) < 0   ) return            ;
  if ( ! isConnected ( MtApiPump ) ) return            ;
  CManagerInterface * cmi = API (MtApiPump)            ;
  //////////////////////////////////////////////////////
  setEnabled     ( "CheckingTrades" , true  )          ;
  PumpMutex . lock   ( )                               ;
  int           total = 0                              ;
  TradeRecord * trx   = cmi -> TradesGet ( &total )    ;
  CUIDs         LS                                     ;
  if ( ( total > 0 ) && ( NULL != trx ) )              {
    for (int i=0;i<total;i++)                          {
      int l   = trx [ i ] . login                      ;
      int cmd = trx [ i ] . cmd                        ;
      switch ( cmd )                                   {
        case TS_OPEN_NORMAL                            :
        case TS_OPEN_REMAND                            :
        case TS_OPEN_RESTORED                          :
          if ( ! LS . contains ( l ) ) LS << l         ;
        break                                          ;
      }                                                ;
    }                                                  ;
    Tradings = LS                                      ;
  }                                                    ;
  if ( NULL != trx ) cmi -> MemFree ( trx )            ;
  PumpMutex . unlock ( )                               ;
  N::Time:: msleep(100)                                ;
  setEnabled     ( "CheckingTrades" , false )          ;
}

void N::MtFeeder::StartPumping(int type,void * data)
{
  Q_UNUSED ( type )                                          ;
  Q_UNUSED ( data )                                          ;
  setEnabled ( "Pumping" , true )                            ;
  PumpMutex . lock   (                     )                 ;
  Update             ( symbols , MtApiPump )                 ;
  PumpMutex . unlock (                     )                 ;
  report ( tr("============= Start pumping =============") ) ;
}

void N::MtFeeder::StopPumping(int type,void * data)
{
  Q_UNUSED   ( type                                           ) ;
  Q_UNUSED   ( data                                           ) ;
  setEnabled ( "Pumping" , false                              ) ;
  report     ( tr("------------- Stop pumping -------------") ) ;
}

void N::MtFeeder::ObtainNews(int type,void * data)
{
  if ( ! isEnabled   ( "ObtainNews" ) ) return         ;
  if ( ! isConnected ( MtApiPump    ) ) return         ;
  CManagerInterface * cmi = API (MtApiPump)            ;
  int                 total                            ;
  NewsTopic         * news                             ;
  PumpMutex . lock   ( )                               ;
  news = cmi -> NewsGet ( &total )                     ;
  if ( ( total > 0 ) && ( NULL != news ) )             {
    for (int i=0;i<total;i++)                          {
      QString t = QString::fromUtf8(news[i].topic    ) ;
      QString k = QString::fromUtf8(news[i].keywords ) ;
      QString b                                        ;
      QString m                                        ;
      int     l = news[i].langid                       ;
      if ( NULL != news[i].body )                      {
        b = QString::fromUtf8(news[i].body)            ;
      }                                                ;
        m = QString("%1\n%2\n%3\n%4\n").arg(l).arg(t).arg(k).arg(b) ;
        report ( m ) ;
    }                                                  ;
    cmi -> MemFree ( news )                            ;
  }                                                    ;
  PumpMutex . unlock ( )                               ;
}

void N::MtFeeder::ObtainNewsBody(int type,void * data)
{
  if ( ! isEnabled ( "ObtainNewsBody" ) ) return ;
  report ( QString("PUMP_UPDATE_NEWS_BODY : Type is %1").arg(type) ) ;
}

void N::MtFeeder::SendOnlines(void)
{
  emit OnlinesChanged ( this ) ;
}

CUIDs N::MtFeeder::AllOnlines(void)
{
  return OnlineLogins ;
}

QString N::MtFeeder::OnlineIP(int login)
{
  return LoginIPs [ login ] ;
}

int N::MtFeeder::OnlineCounter(int login)
{
  return LoginCounters [ login ] ;
}

void N::MtFeeder::GetOnlines(void)
{
  if ( ! isConnected ( MtApiPump       ) ) return      ;
  CManagerInterface * cmi = API ( MtApiPump )          ;
  int                 total                            ;
  OnlineRecord      * onlines                          ;
  CUIDs               IDs                              ;
  EMAPs               IPs                              ;
  IMAPs               LCs                              ;
  PumpMutex . lock   ( )                               ;
  onlines = cmi -> OnlineGet ( &total )                ;
  if ( ( total > 0 ) && ( NULL != onlines ) )          {
    for (int i=0;i<total;i++)                          {
      int             login = onlines [ i ] . login    ;
      int             ctr   = onlines [ i ] . counter  ;
      unsigned int    ip    = onlines [ i ] . ip       ;
      unsigned char * IP    = (unsigned char *)&ip     ;
      QString         IPS                              ;
      IDs << login                                     ;
      IPS = QString ( "%1.%2.%3.%4"                    )
            . arg   ( IP [ 0 ]                         )
            . arg   ( IP [ 1 ]                         )
            . arg   ( IP [ 2 ]                         )
            . arg   ( IP [ 3 ]                       ) ;
      IPs [ login ] = IPS                              ;
      LCs [ login ] = ctr                              ;
    }                                                  ;
    cmi -> MemFree ( onlines )                         ;
  }                                                    ;
  PumpMutex . unlock ( )                               ;
  if ( IDs . count ( ) > 0 )                           {
    OnlineLogins  = IDs                                ;
    LoginIPs      = IPs                                ;
    LoginCounters = LCs                                ;
    emit EmitOnlines ( )                               ;
  }                                                    ;
}

void N::MtFeeder::ObtainOnlines(int type,void * data)
{
  Q_UNUSED ( type )                             ;
  Q_UNUSED ( data )                             ;
  if ( ! isEnabled ( "ObtainOnlines" ) ) return ;
  if (   isEnabled ( "SyncUsers"     ) )        {
    SyncUsers  ( )                              ;
  } else                                        {
    GetOnlines ( )                              ;
  }                                             ;
}

QStrings N::MtFeeder::AllGroups(void)
{
  return groups ;
}

void N::MtFeeder::SendGroups(void)
{
  emit GroupsChanged ( this ) ;
}

void N::MtFeeder::UpdateGroups(int type,void * data)
{
  Q_UNUSED ( type )                                 ;
  Q_UNUSED ( data )                                 ;
  if ( ! isEnabled   ( "UpdateGroups" ) ) return    ;
  SafeMtCall ( MtApiPump , cmi )                    ;
  int        total                                  ;
  ConGroup * group                                  ;
  QStrings   s                                      ;
  PumpMutex . lock   ( )                            ;
  group = cmi -> GroupsGet ( &total )               ;
  if ( ( total > 0 ) && ( NULL != group ) )         {
    for (int i=0;i<total;i++)                       {
      QString g                                     ;
      g = QString::fromUtf8 ( group [ i ] . group ) ;
      if ( ! s . contains ( g ) ) s << g            ;
    }                                               ;
    cmi -> MemFree ( group )                        ;
  }                                                 ;
  PumpMutex . unlock ( )                            ;
  if ( s . count ( ) > 0 )                          {
    groups = s                                      ;
    emit EmitGroups ( )                             ;
  }                                                 ;
}

CUIDs N::MtFeeder::AllUsers(void)
{
  return logins ;
}

void N::MtFeeder::SendUsers(void)
{
  emit UsersChanged ( this ) ;
}

void N::MtFeeder::GetUsers(void)
{
  if ( ! isConnected ( MtApiPump     ) ) return   ;
  CManagerInterface * cmi = API ( MtApiPump )     ;
  UserRecord        * UR                          ;
  int                 total = 0                   ;
  CUIDs               IDs                         ;
  PumpMutex . lock   ( )                          ;
  UR = cmi -> UsersGet ( &total )                 ;
  if ( ( total > 0 ) && NotNull ( UR ) )          {
    for (int i=0;i<total;i++) IDs << UR[i].login  ;
    cmi -> MemFree ( UR )                         ;
  }                                               ;
  PumpMutex . unlock ( )                          ;
  if ( IDs . count ( ) > 0 )                      {
    logins = IDs                                  ;
    emit EmitUsers ( )                            ;
  }                                               ;
}

void N::MtFeeder::UpdateUsers(int type,void * data)
{
  Q_UNUSED ( type )                           ;
  Q_UNUSED ( data )                           ;
  if ( ! isEnabled ( "UpdateUsers" ) ) return ;
  if (   isEnabled ( "SyncUsers"   ) )        {
    SyncUsers ( )                             ;
  } else                                      {
    GetUsers  ( )                             ;
  }                                           ;
}

void N::MtFeeder::PumpingPing(int type,void * data)
{ // type normally always 0 , data normally always NULL
  Q_UNUSED ( type )                           ;
  Q_UNUSED ( data )                           ;
  if ( ! isEnabled ( "PumpingPing" ) ) return ;
  if (   isEnabled ( "PingTransmission" ) )   {
    RemoveChannels (                        ) ;
    Transmissions  (                        ) ;
  }                                           ;
}

void N::MtFeeder::SyncUsers(void)
{ QMutexLocker Locker ( & UserMutex )             ;
  if ( allUsers . count ( ) <= 0       ) return   ;
  if ( ! isConnected ( MtApiPump     ) ) return   ;
  RemoveUsers ( )                                 ;
  /////////////////////////////////////////////////
  CManagerInterface * cmi = API ( MtApiPump )     ;
  if ( NULL == cmi ) return                       ;
  UserRecord        * UR                          ;
  MarginLevel       * ML                          ;
  OnlineRecord      * XR                          ;
  int                 totalusers   = 0            ;
  int                 totalmargins = 0            ;
  int                 totalonlines = 0            ;
  PumpMutex . lock   ( )                          ;
  UR = cmi -> UsersGet   ( &totalusers   )        ;
  ML = cmi -> MarginsGet ( &totalmargins )        ;
  XR = cmi -> OnlineGet  ( &totalonlines )        ;
  for (int i=0;i<allUsers.count();i++)            {
    Finance::Users * u = allUsers [ i ]           ;
    u -> update ( totalusers   , UR )             ;
    u -> update ( totalmargins , ML )             ;
    u -> update ( totalonlines , XR )             ;
  }                                               ;
  if ( NULL != XR ) cmi -> MemFree ( XR )         ;
  if ( NULL != ML ) cmi -> MemFree ( ML )         ;
  if ( NULL != UR ) cmi -> MemFree ( UR )         ;
  PumpMutex . unlock ( )                          ;
  for (int i=0;i<allUsers.count();i++)            {
    Finance::Users * u = allUsers [ i ]           ;
    u -> operate ( )                              ;
  }                                               ;
}

int N::MtFeeder::TotalTradings(void)
{
  return Tradings . count ( ) ;
}

void N::MtFeeder::SymbolsTimeout(N::VarArgs & args)
{
  if ( isEnabled ( "SymbolsTimeout" ) ) return ;
  start ( 10010 , args )                       ;
}

void N::MtFeeder::TriggerEquity(N::VarArgs & args)
{
  start ( 10021 , args ) ;
}

void N::MtFeeder::TriggerHealth(N::VarArgs & args)
{
  start ( 10022 , args ) ;
}

QScriptEngine * N::MtFeeder::GetEngine(int id)
{
  if ( ! Engines . contains ( -1 ) ) return NULL                             ;
  if ( Engines . contains ( id ) ) return Engines [ id ]                     ;
  ////////////////////////////////////////////////////////////////////////////
  QScriptValue     global = Engines [ -1 ] -> globalObject (            )    ;
  QScriptValue     source = global          . property     ( "Original" )    ;
  QString          script = source          . toString     (            )    ;
  ////////////////////////////////////////////////////////////////////////////
  if ( script.length() <= 0 ) return NULL                                    ;
  ////////////////////////////////////////////////////////////////////////////
  QScriptEngine *  Engine = new QScriptEngine    (      )                    ;
  QScriptValue     mtapi  = Engine -> newQObject ( this )                    ;
  QScriptValue     func                                                      ;
  QScriptValue     g                                                         ;
  QScriptValue     src ( script )                                            ;
  ////////////////////////////////////////////////////////////////////////////
  Scripts . AttachCreators         ( *Engine                               ) ;
  func    = Engine -> evaluate     ( script                                ) ;
  g       = Engine -> globalObject (                                       ) ;
  g       . setProperty            ( "gMtAPI"   , mtapi                    ) ;
  g       . setProperty            ( "Original" , src                      ) ;
  ////////////////////////////////////////////////////////////////////////////
  Engines [ id ] = Engine                                                    ;
  return Engines [ id ]                                                      ;
}

void N::MtFeeder::SymbolStopped(QString symbol,qint64 timeout,QString callback,int Id)
{
  if ( callback . length ( ) <= 0    ) return               ;
  QScriptEngine * e = GetEngine ( Id )                      ;
  if ( NULL == e ) return                                   ;
  QScriptValue global = e      -> globalObject (          ) ;
  QScriptValue silent = global  . property     ( callback ) ;
  if ( silent . isFunction ( ) )                            {
    QScriptValueList args                                   ;
    args   << symbol                                        ;
    args   << (int)timeout                                  ;
    silent  . call           ( QScriptValue ( ) , args )    ;
    e      -> collectGarbage (                         )    ;
  }                                                         ;
}

void N::MtFeeder::MonitorSilence(N::VarArgs & args,int Id)
{
  qint64  msecs    = 5000                                           ;
  qint64  interval = 1000                                           ;
  QString callback = ""                                             ;
  if ( args . count ( ) > 0 ) msecs    = args [ 0 ] . toInt    ( )  ;
  if ( args . count ( ) > 1 ) interval = args [ 1 ] . toInt    ( )  ;
  if ( args . count ( ) > 2 ) callback = args [ 2 ] . toString ( )  ;
  setEnabled ( "SymbolsTimeout" , true  )                           ;
  ///////////////////////////////////////////////////////////////////
  if ( isEnabled ( "ShowTicks" ) )                                  {
    report ( "============= Monitor silence started ============" ) ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  GetEngine ( Id )                                                  ;
  while ( ! isDrop ( ) )                                            {
    QStringList l = symbols                                         ;
    QString     s                                                   ;
    foreach ( s , l )                                               {
      qint64 t = SymbolSlience ( s )                                ;
      if ( t > msecs ) SymbolStopped ( s , t , callback , Id )      ;
    }                                                               ;
    N::Time::msleep(interval)                                       ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  if ( isEnabled ( "ShowTicks" ) )                                  {
    report ( "============= Monitor silence stopped ============" ) ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  setEnabled ( "SymbolsTimeout" , false )                           ;
}

bool N::MtFeeder::EquityHealth (
       int     login          ,
       double  balance        ,
       double  equity         ,
       double  margin         ,
       int     multiply       ,
       QString callback       ,
       int     Id             )
{
  if ( callback . length ( ) <= 0 ) return false             ;
  QScriptEngine * g = GetEngine ( Id )                       ;
  if ( NULL == g ) return false                              ;
  QScriptValue global = g      -> globalObject (          )  ;
  QScriptValue eval   = global  . property     ( callback )  ;
  bool         result = false                                ;
  if ( eval . isFunction ( ) )                               {
    int              b = (int) ( balance * multiply )        ;
    int              e = (int) ( equity  * multiply )        ;
    int              m = (int) ( margin  * multiply )        ;
    QScriptValue     r                                       ;
    QScriptValueList args                                    ;
    args  << QScriptValue    ( login                   )     ;
    args  << QScriptValue    ( b                       )     ;
    args  << QScriptValue    ( e                       )     ;
    args  << QScriptValue    ( m                       )     ;
    args  << QScriptValue    ( multiply                )     ;
    r       = eval . call    ( QScriptValue ( ) , args )     ;
    result  = r    . toBool  (                         )     ;
    g      -> collectGarbage (                         )     ;
  }                                                          ;
  return result                                              ;
}

void N::MtFeeder::MaintainEquity(N::VarArgs & args,int Id)
{
  qint64  interval = 1000                                          ;
  QString tag      = "Monitor"                                     ;
  QString callback = ""                                            ;
  bool    cb       = false                                         ;
  int     multiply = 100                                           ;
  CUIDs   enlists                                                  ;
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt    ( ) ;
  if ( args . count ( ) > 1 ) callback = args [ 1 ] . toString ( ) ;
  if ( args . count ( ) > 2 ) multiply = args [ 2 ] . toInt    ( ) ;
  if ( args . count ( ) > 3 ) tag      = args [ 3 ] . toString ( ) ;
  cb = ( callback . length ( ) > 0 )                               ;
  ListMutex . lock   ( )                                           ;
  TradingOrders [ tag ] = enlists                                  ;
  ListMutex . unlock ( )                                           ;
  //////////////////////////////////////////////////////////////////
  GetEngine ( 3 )                                                  ;
  while ( ! isDrop ( ) )                                           {
    CUIDs tradings = Tradings                                      ;
    enlists . clear ( )                                            ;
    ListMutex . lock   ( )                                         ;
    TradingOrders [ tag ] = enlists                                ;
    ListMutex . unlock ( )                                         ;
    if ( tradings . count ( ) > 0 )                                {
      CManagerInterface * cmi = API ( MtApiManager )               ;
      CManagerInterface * cpi = API ( MtApiPump    )               ;
      if ( ( NULL != cmi ) && ( cpi != NULL ) )                    {
        int       total = 0                                        ;
        QDateTime Now   = nTimeNow                                 ;
        if ( cb )                                                  {
          MarginLevel * MLs   = NULL                               ;
          PumpMutex . lock   ( )                                   ;
          report ( tr("======== %1 started ========").arg(tag)   ) ;
          MLs = cpi ->MarginsGet ( &total )                        ;
          for (int i=0;i<total;i++)                                {
            int    login   = 0                                     ;
            double balance = 0                                     ;
            double equity  = 0                                     ;
            double margin  = 0                                     ;
            login   = MLs [ i ] . login                            ;
            balance = MLs [ i ] . balance                          ;
            equity  = MLs [ i ] . equity                           ;
            margin  = MLs [ i ] . margin                           ;
            if ( tradings.contains(login) )                        {
              bool eh = EquityHealth                               (
                          login                                    ,
                          balance                                  ,
                          equity                                   ,
                          margin                                   ,
                          multiply                                 ,
                          callback                                 ,
                          Id                                     ) ;
              if ( eh )                                            {
                ListMutex . lock   ( )                             ;
                TradingOrders [ tag ] << login                     ;
                ListMutex . unlock ( )                             ;
              }                                                    ;
            }                                                      ;
          }                                                        ;
          if ( NULL != MLs ) cpi -> MemFree ( MLs )                ;
          PumpMutex . unlock ( )                                   ;
        }                                                          ;
        report(tr("------ %1 , %2 accounts take %3 ms ------").arg(tag).arg(total).arg(Now.msecsTo(nTimeNow)) ) ;
        ////////////////////////////////////////////////////////////
        if ( TradingOrders [ tag ] . count ( ) > 0 )               {
          QString M                                                ;
          M = tr    ( "%1 accounts were listed in `%2`."           )
              . arg ( TradingOrders [ tag ] . count ( )            )
              . arg ( tag                                        ) ;
          report ( M )                                              ;
        }                                                          ;
      }                                                            ;
    }                                                              ;
    N::Time::msleep(interval)                                      ;
  }                                                                ;
}

void N::MtFeeder::MaintainHealth(N::VarArgs & args,int Id)
{
  qint64  interval = 1000                                          ;
  QString fromlist = ""                                            ;
  QString tolist   = ""                                            ;
  QString callback = ""                                            ;
  int     multiply = 100                                           ;
  bool    cb       = false                                         ;
  CUIDs   enlists                                                  ;
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt    ( ) ;
  if ( args . count ( ) > 1 ) callback = args [ 1 ] . toString ( ) ;
  if ( args . count ( ) > 2 ) multiply = args [ 2 ] . toInt    ( ) ;
  if ( args . count ( ) > 3 ) fromlist = args [ 3 ] . toString ( ) ;
  if ( args . count ( ) > 4 ) tolist   = args [ 4 ] . toString ( ) ;
  cb = ( callback . length ( ) > 0 )                               ;
  if ( fromlist . length ( ) <= 0 ) return                         ;
  if ( tolist   . length ( ) <= 0 ) return                         ;
  TradingOrders [ tolist ] = enlists                               ;
  //////////////////////////////////////////////////////////////////
  GetEngine ( Id )                                                 ;
  while ( ! isDrop ( ) )                                           {
    CUIDs tradings                                                 ;
    ListMutex . lock   ( )                                         ;
    enlists                  . clear         (          )          ;
    tradings                 = TradingOrders [ fromlist ]          ;
    TradingOrders [ tolist ] = enlists                             ;
    ListMutex . unlock ( )                                         ;
    if ( tradings . count ( ) > 0 )                                {
      CManagerInterface * cmi = API ( MtApiManager )               ;
      if ( NULL != cmi )                                           {
        int login                                                  ;
        foreach ( login , tradings )                               {
          MarginLevel ml                                           ;
          int         r                                            ;
          double      balance = 0                                  ;
          double      equity  = 0                                  ;
          double      margin  = 0                                  ;
          ManagerMutex . lock   ( )                                ;
          r = cmi -> MarginLevelRequest ( login , &ml )            ;
          if ( cb && ( RET_OK == r ) )                             {
            balance = ml . balance                                 ;
            equity  = ml . equity                                  ;
            margin  = ml . margin                                  ;
          }                                                        ;
          ManagerMutex . unlock ( )                                ;
          if ( cb && ( RET_OK == r ) )                             {
            bool eh = EquityHealth                                 (
                        login                                      ,
                        balance                                    ,
                        equity                                     ,
                        margin                                     ,
                        multiply                                   ,
                        callback                                   ,
                        Id                                       ) ;
            if ( eh )                                              {
              ListMutex . lock   ( )                               ;
              TradingOrders [ tolist ] << ml . login               ;
              ListMutex . unlock ( )                               ;
            }                                                      ;
          } else                                                   {
            report ( QString("Can not obtain margin level %1").arg(login) ) ;
          }                                                        ;
        }                                                          ;
        ////////////////////////////////////////////////////////////
        if ( TradingOrders [ tolist ] . count ( ) > 0 )            {
          QString M                                                ;
          M = tr    ( "%1 accounts were listed in `%2`."           )
              . arg ( TradingOrders [ tolist ] . count ( )         )
              . arg ( tolist                                     ) ;
          report ( M )                                             ;
        }                                                          ;
      }                                                            ;
    }                                                              ;
    N::Time::msleep(interval)                                      ;
  }                                                                ;
}

void N::MtFeeder::HealthEquity(N::VarArgs & args,int Id)
{
  qint64  interval = 1000                                          ;
  QString tag      = "RecoveryHealth"                              ;
  QString callback = ""                                            ;
  QString permit   = ""                                            ;
  bool    cb       = false                                         ;
  int     multiply = 100                                           ;
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt    ( ) ;
  if ( args . count ( ) > 1 ) callback = args [ 1 ] . toString ( ) ;
  if ( args . count ( ) > 2 ) multiply = args [ 2 ] . toInt    ( ) ;
  if ( args . count ( ) > 3 ) tag      = args [ 3 ] . toString ( ) ;
  if ( args . count ( ) > 4 ) permit   = args [ 4 ] . toString ( ) ;
  cb = ( callback . length ( ) > 0 )                               ;
  //////////////////////////////////////////////////////////////////
  GetEngine ( Id )                                                 ;
  while ( ! isDrop ( ) )                                           {
    CUIDs tradings                                                 ;
    ListMutex . lock   ( )                                         ;
    tradings = TradingOrders [ tag ]                               ;
    ListMutex . unlock ( )                                         ;
    if ( tradings . count ( ) > 0 )                                {
      CManagerInterface * cmi = API ( MtApiManager )               ;
      if ( NULL != cmi )                                           {
        int login                                                  ;
        foreach ( login , tradings )                               {
          bool eradicate = true                                    ;
          while ( isEnabled("Connected") && eradicate )            {
            MarginLevel ml                                         ;
            int         r                                          ;
            double      balance = 0                                ;
            double      equity  = 0                                ;
            double      margin  = 0                                ;
            ManagerMutex . lock   ( )                              ;
            r = cmi -> MarginLevelRequest ( login , &ml )          ;
            if ( cb && ( RET_OK == r ) )                           {
              balance = ml . balance                               ;
              equity  = ml . equity                                ;
              margin  = ml . margin                                ;
            }                                                      ;
            ManagerMutex . unlock ( )                              ;
            if ( cb && ( RET_OK == r ) )                           {
              bool eh = EquityHealth                               (
                          login                                    ,
                          balance                                  ,
                          equity                                   ,
                          margin                                   ,
                          multiply                                 ,
                          callback                                 ,
                          Id                                     ) ;
              if ( eh )                                            {
                if ( ! CloseMaxLoss ( login , permit , Id ) )      {
                  eradicate = false                                ;
                }                                                  ;
              } else                                               {
                eradicate = false                                  ;
              }                                                    ;
            } else                                                 {
              eradicate = false                                    ;
            }                                                      ;
          }                                                        ;
        }                                                          ;
      }                                                            ;
    }                                                              ;
    N::Time::msleep(interval)                                      ;
  }                                                                ;
}

bool N::MtFeeder::PermitToClose (
       int     login            ,
       int     order            ,
       int     cmd              ,
       double  profit           ,
       QString callback         ,
       int     Id               )
{
  if ( callback . length ( ) <= 0    ) return false         ;
  QScriptEngine * e = GetEngine ( Id )                      ;
  if ( NULL == e ) return false                             ;
  QScriptValue global = e      -> globalObject (          ) ;
  QScriptValue eval   = global  . property     ( callback ) ;
  bool         result = false                               ;
  if ( eval . isFunction ( ) )                              {
    QScriptValue     r                                      ;
    QScriptValueList args                                   ;
    args  << QScriptValue ( login    )                      ;
    args  << QScriptValue ( order    )                      ;
    args  << QScriptValue ( cmd      )                      ;
    args  << QScriptValue ( profit   )                      ;
    r       = eval . call    ( QScriptValue ( ) , args )    ;
    result  = r    . toBool  (                         )    ;
    e      -> collectGarbage (                         )    ;
  }                                                         ;
  return result                                             ;
}

bool N::MtFeeder::CloseMaxLoss(int login,QString permit,int Id)
{
  CManagerInterface * cmi = API ( MtApiManager )               ;
  CManagerInterface * cpi = API ( MtApiPump    )               ;
  if ( NULL == cmi ) return false                              ;
  if ( NULL == cpi ) return false                              ;
  UserRecord   * UR                                            ;
  TradeRecord  * trx                                           ;
  TradeTransInfo TTI                                           ;
  MarginLevel    ml                                            ;
  int            total      =  0                               ;
  int            order      = -1                               ;
  int            cmd        = -1                               ;
  int            r                                             ;
  double         profit     = 0                                ;
  double         closeprice = 0                                ;
  int            logins [ 2 ]                                  ;
  logins [ 0 ] = login                                         ;
  ManagerMutex . lock   (       )                              ;
  UR           = User   ( login )                              ;
  if ( NULL != UR )                                            {
    cmi -> MarginLevelRequest ( login , &ml )                  ;
  }                                                            ;
  ManagerMutex . unlock (       )                              ;
  if ( NULL == UR )                                            {
    report ( tr( "No account info %1").arg(login) )            ;
    return false                                               ;
  }                                                            ;
  ::memset ( &TTI , 0 , sizeof(TradeTransInfo) )               ;
  PumpMutex . lock   ( )                                       ;
  trx = cpi -> TradesGetByLogin ( login , UR->group , &total ) ;
  if ( ( total > 0 ) && ( NULL != trx ) )                      {
    for (int i = 0 ; i < total ; i++ )                         {
      if ( ( login == trx [ i ] . login ) )                    {
        switch ( trx[i] . cmd )                                {
          case TS_OPEN_NORMAL                                  :
          case TS_OPEN_REMAND                                  :
          case TS_OPEN_RESTORED                                :
           if ( ( profit > trx [ i ] . profit )               &&
                ( PermitToClose ( login                        ,
                                  trx [ i ] . order            ,
                                  trx [ i ] . cmd              ,
                                  trx [ i ] . profit           ,
                                  permit                       ,
                                  Id                     ) ) ) {
             profit        = trx [ i ] . profit                ;
             order         = trx [ i ] . order                 ;
             cmd           = trx [ i ] . cmd                   ;
             closeprice    = trx [ i ] . close_price           ;
             TTI . order   = order                             ;
             TTI . orderby = login                             ;
             TTI . cmd     = cmd                               ;
             TTI . volume  = trx [ i ] . volume                ;
			 TTI.price = closeprice                            ;
			 ::strcpy ( TTI . symbol , trx [ i ] . symbol )    ;
           }                                                   ;
          break                                                ;
        }                                                      ;
      }                                                        ;
    }                                                          ;
    if ( NULL != trx ) cmi -> MemFree ( trx )                  ;
  } else                                                       {
    report ( tr("Can not obtain trades from %1").arg(login)  ) ;
  }                                                            ;
  PumpMutex . unlock ( )                                       ;
  //////////////////////////////////////////////////////////////
  if ( order < 0 )                                             {
    delete UR                                                  ;
    return false                                               ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  QString mc                                                   ;
  QString pc      = "0"                                        ;
  QString ac      = ""                                         ;
  int     percent = 0                                          ;
  double  bidv    = 0                                          ;
  double  askv    = 0                                          ;
  if ( ( ml . balance > 0 ) && ( ml . equity > 0 ) )           {
    percent  = ml . equity                                     ;
    percent *= 10000                                           ;
    percent /= ml . balance                                    ;
    pc       = QString::number ( percent / 100               ) ;
    pc       . append ( "."                                  ) ;
    percent   = percent % 100                                  ;
    for (int xi=0;xi<2;xi++)                                   {
      ac . prepend ( QString::number(percent%10) )             ;
      percent /= 10                                            ;
    }                                                          ;
    pc . append ( ac )                                         ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  mc = tr("[stopout] %1%").arg(pc);
  ::strcpy ( TTI . comment , mc . toUtf8 ( ) . constData ( ) ) ;
  TTI . type = TT_BR_ORDER_CLOSE                               ;
  //////////////////////////////////////////////////////////////
  delete UR                                                    ;
  //////////////////////////////////////////////////////////////
  ManagerMutex . lock   ( )                                    ;
  r = cmi -> TradeTransaction ( &TTI )                         ;
  ManagerMutex . unlock ( )                                    ;
  if ( r <= RET_OK_NONE ) return true                               ;
  mc = tr("Stop out %1 #%2 failure err_code: %3").arg(login).arg(order).arg(r)      ;
  report ( mc )                                                ;
  return false                                                 ;
}

void N::MtFeeder::TriggerCloseAll(N::VarArgs & args)
{
  start ( 10024 , args ) ;
}

void N::MtFeeder::TriggerEquityClose(N::VarArgs & args)
{
  start ( 10025 , args ) ;
}

void N::MtFeeder::ForcelyClose(N::VarArgs & args,int Id)
{ // close account orders forcely interval keyword callback
  // interval keyword callback
  qint64  interval = 1000                                                    ;
  QString keyword  = ""                                                      ;
  QString callback = ""                                                      ;
  bool    cb       = false                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  if ( args    . count  ( ) >  0 ) interval = args [ 0 ] . toInt    ( )      ;
  if ( args    . count  ( ) >  1 ) keyword  = args [ 1 ] . toString ( )      ;
  if ( args    . count  ( ) >  2 ) callback = args [ 2 ] . toString ( )      ;
  if ( keyword . length ( ) <= 0 ) return                                    ;
  cb = ( callback . length ( ) > 0 )                                         ;
  if ( cb ) GetEngine ( Id )                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  CUIDs enlists                                                              ;
  TradingOrders [ keyword ] = enlists                                        ;
  ////////////////////////////////////////////////////////////////////////////
  while ( ! isDrop ( ) )                                                     {
    CUIDs tradings                                                           ;
    //////////////////////////////////////////////////////////////////////////
    ListMutex . lock   ( )                                                   ;
    enlists                   . clear         (         )                    ;
    tradings                  = TradingOrders [ keyword ]                    ;
    TradingOrders [ keyword ] = enlists                                      ;
    ListMutex . unlock ( )                                                   ;
    //////////////////////////////////////////////////////////////////////////
    if ( tradings . count ( ) > 0 )                                          {
      bool allow = true                                                      ;
      int  login                                                             ;
      foreach ( login , tradings )                                           {
        allow = true                                                         ;
        if ( cb    ) allow = CloseThisAccount ( MtApiPump,login,callback,Id) ;
        if ( allow )                                                         {
          CloseUserOrders ( MtApiPump , login )                              ;
          if ( isEnabled ( "ReadOnly" ) ) setReadOnly ( login , true )       ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    N::Time::msleep(interval)                                                ;
  }                                                                          ;
}

bool N::MtFeeder::CloseThisAccount (
       int     api                 ,
       int     login               ,
       QString callback            ,
       int     Id                  )
{
  if ( callback . length ( ) <= 0    ) return false           ;
  QScriptEngine * e = GetEngine ( Id )                        ;
  if ( NULL == e ) return false                               ;
  CManagerInterface * cmi = API ( (MtApiFlags) api )          ;
  if ( NULL == cmi ) return false                             ;
  if ( ! isConnected ( (MtApiFlags) api ) ) return false      ;
  /////////////////////////////////////////////////////////////
  MarginLevel  ml                                             ;
  int          r       = -1                                   ;
  /////////////////////////////////////////////////////////////
  ApiMutex ( api ) . lock   ( )                               ;
  r  = cmi -> MarginLevelRequest ( login , &ml )              ;
  ApiMutex ( api ) . unlock ( )                               ;
  if ( RET_OK != r  ) return false                            ;
  /////////////////////////////////////////////////////////////
  QScriptValue global = e      -> globalObject (            ) ;
  QScriptValue eval   = global  . property     ( callback   ) ;
  bool         result = false                                 ;
  QString      grp    = QString::fromUtf8      ( ml . group ) ;
  if ( eval . isFunction ( ) )                                {
    QScriptValue     r                                        ;
    QScriptValueList args                                     ;
    args  << QScriptValue    ( login                   )      ;
    args  << QScriptValue    ( grp                     )      ;
    args  << QScriptValue    ( ml . balance            )      ;
    args  << QScriptValue    ( ml . equity             )      ;
    args  << QScriptValue    ( ml . leverage           )      ;
    args  << QScriptValue    ( ml . margin             )      ;
    args  << QScriptValue    ( ml . margin_free        )      ;
    args  << QScriptValue    ( ml . margin_level       )      ;
    args  << QScriptValue    ( ml . margin_type        )      ;
    args  << QScriptValue    ( ml . level_type         )      ;
    r       = eval . call    ( QScriptValue ( ) , args )      ;
    result  = r    . toBool  (                         )      ;
    e      -> collectGarbage (                         )      ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return result                                               ;
}

bool N::MtFeeder::AIAEquityCheck (
       int       api             ,
       int       login           ,
       QString   callback        ,
       QDateTime from            ,
       QString   commentContains ,
       int       Id              )
{
  CManagerInterface * cmi    = API ( (MtApiFlags) api )         ;
  QScriptEngine     * e      = NULL                             ;
  bool                result = false                            ;
  if ( NULL == cmi ) return false                               ;
  if ( ! isConnected ( (MtApiFlags) api ) ) return false        ;
  if ( callback . length ( ) > 0 )                              {
    e = GetEngine ( Id )                                        ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  MarginLevel  ml                                               ;
  int          r       = -1                                     ;
  double       deposit                                          ;
  deposit = AccountDeposit ( api,login,from,commentContains )   ;
  ///////////////////////////////////////////////////////////////
  ApiMutex ( api ) . lock   ( )                                 ;
  r  = cmi -> MarginLevelRequest ( login , &ml )                ;
  ApiMutex ( api ) . unlock ( )                                 ;
  if ( RET_OK != r  ) return false                              ;
  ///////////////////////////////////////////////////////////////
  QString grp = QString::fromUtf8 ( ml . group )                ;
  if ( NULL != e )                                              {
    QScriptValue global = e      -> globalObject (            ) ;
    QScriptValue eval   = global  . property     ( callback   ) ;
    if ( eval . isFunction ( ) )                                {
      QScriptValue     r                                        ;
      QScriptValueList args                                     ;
      args  << QScriptValue    ( login                   )      ;
      args  << QScriptValue    ( grp                     )      ;
      args  << QScriptValue    ( ml . balance            )      ;
      args  << QScriptValue    ( ml . equity             )      ;
      args  << QScriptValue    ( ml . leverage           )      ;
      args  << QScriptValue    ( ml . margin             )      ;
      args  << QScriptValue    ( ml . margin_free        )      ;
      args  << QScriptValue    ( ml . margin_level       )      ;
      args  << QScriptValue    ( ml . margin_type        )      ;
      args  << QScriptValue    ( ml . level_type         )      ;
      args  << QScriptValue    ( deposit                 )      ;
      r       = eval . call    ( QScriptValue ( ) , args )      ;
      result  = r    . toBool  (                         )      ;
      e      -> collectGarbage (                         )      ;
    }                                                           ;
  } else                                                        {
    if ( "KRC" == grp )                                         {
      deposit *= 0.86                                           ;
      result   = ( ml . equity < deposit )                      ;
    }                                                           ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  return result                                                 ;
}

void N::MtFeeder::EquityCheck(N::VarArgs & args,int Id)
{ // close account orders equity start-date interval keyword callback commentContains
  // start-date interval keyword callback commentContains
  qint64    interval        = 1000                                           ;
  QString   keyword         = ""                                             ;
  QString   callback        = ""                                             ;
  QString   commentContains = ""                                             ;
  bool      cb              = false                                          ;
  QDateTime from                                                             ;
  from = QDateTime::fromString("2010/01/01 00:00:00","yyyy/MM/dd hh:mm:ss")  ;
  ////////////////////////////////////////////////////////////////////////////
  if ( args    . count  ( ) >  0 )                                           {
    QString Zs = args [ 0 ] . toString ( )                                   ;
    QDate   Z  = QDate::fromString ( Zs , "yyyy/MM/dd" )                     ;
    QTime   U ( 0 , 0 , 0 , 0 )                                              ;
    from = QDateTime ( Z , U )                                               ;
  }                                                                          ;
  if ( args    . count  ( ) >  1 ) interval = args [ 1 ] . toInt    ( )      ;
  if ( args    . count  ( ) >  2 ) keyword  = args [ 2 ] . toString ( )      ;
  if ( args    . count  ( ) >  3 ) callback = args [ 3 ] . toString ( )      ;
  if ( keyword . length ( ) <= 0 ) return                                    ;
  cb = ( callback . length ( ) > 0 )                                         ;
  if ( cb ) GetEngine ( Id )                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  CUIDs enlists                                                              ;
  TradingOrders [ keyword ] = enlists                                        ;
  ////////////////////////////////////////////////////////////////////////////
  while ( ! isDrop ( ) )                                                     {
    CUIDs tradings                                                           ;
    //////////////////////////////////////////////////////////////////////////
    ListMutex . lock   ( )                                                   ;
    enlists                   . clear         (         )                    ;
    tradings                  = TradingOrders [ keyword ]                    ;
    TradingOrders [ keyword ] = enlists                                      ;
    ListMutex . unlock ( )                                                   ;
    //////////////////////////////////////////////////////////////////////////
    if ( tradings . count ( ) > 0 )                                          {
      bool allow = true                                                      ;
      int  login                                                             ;
      foreach ( login , tradings )                                           {
        allow = false                                                        ;
        if ( cb )                                                            {
          allow = AIAEquityCheck ( MtApiPump                                 ,
                                   login                                     ,
                                   callback                                  ,
                                   from                                      ,
                                   commentContains                           ,
                                   Id                                      ) ;
        }                                                                    ;
        if ( allow )                                                         {
          CloseUserOrders ( MtApiPump , login )                              ;
          if ( isEnabled ( "ReadOnly" ) ) setReadOnly ( login , true )       ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    N::Time::msleep(interval)                                                ;
  }                                                                          ;
}

void N::MtFeeder::OrdersSync(N::VarArgs & args,int Id)
{ // sync orders interval
  qint64 interval = 1000                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt ( )              ;
  ////////////////////////////////////////////////////////////////////////////
  while ( ! isDrop ( ) )                                                     {
    //////////////////////////////////////////////////////////////////////////
    if ( allOrders . count ( ) > 0 )                                         {
      CManagerInterface * cmi = API ( MtApiPump )                            ;
      if ( NULL != cmi )                                                     {
        bool all = false                                                     ;
        //////////////////////////////////////////////////////////////////////
        for (int i=0;i<allOrders.count();i++)                                {
          if ( 0 == allOrders [ i ] -> login ) all = true                    ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        if ( all )                                                           {
          int           total = 0                                            ;
          TradeRecord * TRX                                                  ;
          PumpMutex . lock   ( )                                             ;
          TRX = cmi -> TradesGet ( &total )                                  ;
          PumpMutex . unlock ( )                                             ;
          if ( ( total > 0 ) && ( NULL != TRX ) )                            {
            for (int i = 0 ; i < allOrders . count ( ) ; i++ )               {
              allOrders [ i ] -> assign  ( total , TRX )                     ;
              allOrders [ i ] -> operate (             )                     ;
            }                                                                ;
            PumpMutex . lock   ( )                                           ;
            cmi -> MemFree ( TRX )                                           ;
            PumpMutex . unlock ( )                                           ;
          }                                                                  ;
        } else                                                               {
          for (int i = 0 ; i < allOrders . count ( ) ; i++ )                 {
            int           LOGIN = allOrders [ i ] -> login                   ;
            int           total = 0                                          ;
            QString       GROUP                                              ;
            QByteArray    GRP                                                ;
            TradeRecord * TRX                                                ;
            GROUP = allOrders [ i ] -> variables [ "Group" ] . toString ( )  ;
            GRP   = GROUP . toUtf8 ( )                                       ;
            PumpMutex . lock   ( )                                           ;
            TRX = cmi -> TradesGetByLogin ( LOGIN                            ,
                                            GRP . constData ( )              ,
                                            &total                         ) ;
            PumpMutex . unlock ( )                                           ;
            allOrders [ i ] -> assign  ( total , TRX )                       ;
            allOrders [ i ] -> operate (             )                       ;
            PumpMutex . lock   ( )                                           ;
            cmi -> MemFree ( TRX )                                           ;
            PumpMutex . unlock ( )                                           ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    RemoveOrders    (          )                                             ;
    N::Time::msleep ( interval )                                             ;
  }                                                                          ;
}

void N::MtFeeder::HistorySync(N::VarArgs & args,int Id)
{ // sync history interval start-date
  qint64    interval = 1000                                                  ;
  QDateTime FROM     = QDateTime ( QDate (2001,1,1) , QTime(0,0,0,0) )       ;
  time_t    from                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt ( )              ;
  if ( args . count ( ) > 1 )                                                {
    QString df = args [ 1 ] . toString ( )                                   ;
    QDate   DD = QDate::fromString(df,"yyyy/MM/dd")                          ;
    FROM = QDateTime ( DD , QTime(0,0,0,0) )                                 ;
  }                                                                          ;
  from = FROM.toTime_t ( )                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  while ( ! isDrop ( ) )                                                     {
    if ( allHistory . count ( ) > 0 )                                        {
      CManagerInterface * cmi = API ( MtApiManager )                         ;
      if ( NULL != cmi )                                                     {
        for (int i = 0 ; i < allHistory . count ( ) ; i++ )                  {
          int           LOGIN = allHistory [ i ] -> login                    ;
          int           total = 0                                            ;
          time_t        ft    = from                                         ;
          time_t        to    = nTimeNow.toTime_t() + 86400                  ;
          TradeRecord * TRX                                                  ;
          if ( allHistory [ i ] -> variables . contains ( "From" ) )         {
            QDateTime FT                                                     ;
            FT = allHistory [ i ] -> variables [ "From" ] . toDateTime ( )   ;
            ft = FT . toTime_t ( )                                           ;
          }                                                                  ;
          ManagerMutex . lock   ( )                                          ;
          TRX = cmi -> TradesUserHistory ( LOGIN , ft , to , &total )        ;
          ManagerMutex . unlock ( )                                          ;
          allHistory [ i ] -> assign  ( total , TRX )                        ;
          allHistory [ i ] -> operate (             )                        ;
          ManagerMutex . lock   ( )                                          ;
          cmi -> MemFree ( TRX )                                             ;
          ManagerMutex . unlock ( )                                          ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    RemoveHistory   (          )                                             ;
    N::Time::msleep ( interval )                                             ;
  }                                                                          ;
}

void N::MtFeeder::Transmissions(void)
{ QMutexLocker Locker ( & FlowMutex )         ;
  if ( ByteChannels . count ( ) <= 0 ) return ;
  for (int i=0;i<ByteChannels.count();i++)    {
    ByteChannels [ i ] -> Update  ( *this )   ;
    ByteChannels [ i ] -> Operate (       )   ;
  }                                           ;
}

void N::MtFeeder::KeepConnected(N::VarArgs & args,int Id)
{ // keep connected interval pumping
  int  interval = 1000                                           ;
  bool pump     = true                                           ;
  if ( isEnabled ( "KeepConnected") ) return                     ;
  setEnabled ( "KeepConnected"  , true  )                        ;
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toInt  ( ) ;
  if ( args . count ( ) > 1 ) pump     = args [ 1 ] . toBool ( ) ;
  while ( isEnabled ("KeepConnected") )                          {
    FullMount       ( IP , pump )                                ;
    RemoveChannels  (           )                                ;
    Transmissions   (           )                                ;
    N::Time::msleep ( interval  )                                ;
  }                                                              ;
  setEnabled ( "KeepConnected"  , false )                        ;
}

void N::MtFeeder::KeepQuotes(N::VarArgs & args,int Id)
{
  CManagerInterface * cmi = API ( MtApiManager )                  ;
  if ( NULL == cmi ) return                                       ;
  QString s                                                       ;
  QString ip                                                      ;
  bool    pump      = false                                       ;
  bool    connected = false                                       ;
  if ( args . count ( ) > 0 ) ip   = args [ 0 ] . toString ( )    ;
  if ( args . count ( ) > 1 ) pump = args [ 1 ] . toBool   ( )    ;
  ConnectTo      ( ip , pump   )                                  ;
  do                                                              {
    connected = IsConnected ( )                                   ;
    if ( ! connected )                                            {
      Time::msleep ( 1000 )                                       ;
      if ( ! isEnabled ( "KeepConnected" ) ) connected = true     ;
    }                                                             ;
  } while ( ! connected )                                         ;
  SymbolsRefresh ( 1           )                                  ;
  foreach        ( s , symbols )                                  {
    ConSymbol cs                                                  ;
    int       r                                                   ;
    SymbolAdd ( MtApiPump , s )                                   ;
    TickAppend ( s )                                              ;
    r = cmi -> SymbolGet ( s . toUtf8 ( ) . constData ( ) , &cs ) ;
    if ( RET_OK == r )                                            {
      TickDigits ( s , cs . digits )                              ;
      SymbolDigits [ s ] = cs . digits                            ;
    }                                                             ;
  }                                                               ;
  TickReady ( )                                                   ;
  BringUp   ( )                                                   ;
}

void N::MtFeeder::JournalCallback(N::VarArgs & params,int Id,QString callback)
{
  if ( callback . length ( ) <= 0 ) return                  ;
  QScriptEngine * g = GetEngine ( Id )                      ;
  if ( NULL == g ) return                                   ;
  QScriptValue global = g      -> globalObject (          ) ;
  QScriptValue eval   = global  . property     ( callback ) ;
  if ( eval . isFunction ( ) )                              {
    QScriptValue     r                                      ;
    QScriptValueList args                                   ;
    args << params [ 0 ] . toInt    ( )                     ;
    args << params [ 1 ] . toString ( )                     ;
    args << params [ 2 ] . toString ( )                     ;
    args << params [ 3 ] . toString ( )                     ;
    r     = eval . call    ( QScriptValue ( ) , args      ) ;
    g    -> collectGarbage (                              ) ;
  }                                                         ;
}

void N::MtFeeder::ServerJournal(N::VarArgs & args,int Id)
{ // keep journal report interval callback mode duration timezone filter
  qint64  interval = 1000                                                    ;
  QString callback = ""                                                      ;
  int     mode     = LOG_MODE_ENABLED                                        ;
  QString filter   = ""                                                      ;
  qint64  duration = 5000                                                    ;
  qint64  timezone = 0                                                       ;
  ////////////////////////////////////////////////////////////////////////////
  if ( args . count ( ) > 0 ) interval = args [ 0 ] . toLongLong ( )         ;
  if ( args . count ( ) > 1 ) callback = args [ 1 ] . toString   ( )         ;
  if ( args . count ( ) > 2 ) mode     = args [ 2 ] . toInt      ( )         ;
  if ( args . count ( ) > 3 ) duration = args [ 3 ] . toLongLong ( )         ;
  if ( args . count ( ) > 4 ) timezone = args [ 4 ] . toLongLong ( )         ;
  if ( args . count ( ) > 5 ) filter   = args [ 5 ] . toString   ( )         ;
  ////////////////////////////////////////////////////////////////////////////
  time_t toTime   = nTimeNow . toTime_t ( )                                  ;
  time_t fromTime = toTime - ( ( duration + 999 ) / 1000 )                   ;
  char   FILTER [ 1024 ]                                                     ;
  ::memset ( FILTER , 0 , 1024 )                                             ;
  if ( filter . length ( ) > 0 )                                             {
    ::strcpy ( FILTER , filter . toUtf8 ( ) . constData ( ) )                ;
  }                                                                          ;
  while ( ! isDrop ( ) )                                                     {
    //////////////////////////////////////////////////////////////////////////
    CManagerInterface * cmi = API ( MtApiManager )                           ;
    if ( NULL != cmi )                                                       {
      int         total = 0                                                  ;
      ServerLog * sl    = NULL                                               ;
      VarArgs     logs                                                       ;
      toTime   = nTimeNow . toTime_t ( )                                     ;
      if ( toTime != fromTime )                                              {
        ManagerMutex . lock   ( )                                            ;
        sl = cmi -> JournalRequest ( mode,fromTime,toTime,FILTER,&total)     ;
        ManagerMutex . unlock ( )                                            ;
        fromTime = toTime                                                    ;
        if ( ( total > 0 ) && ( NULL != sl ) )                               {
          for (int i=0;i<total;i++)                                          {
            QString     dt  = QString::fromUtf8 ( sl [ i ] . time )          ;
            QStringList DTs = dt . split (' ')                               ;
            QDateTime   DT                                                   ;
            time_t      cdt                                                  ;
            if ( ( DTs . count ( ) > 1 ) && ( DTs [ 1 ] . length ( ) > 0 ) ) {
              DT   = QDateTime::fromString(dt,"yyyy.MM.dd hh:mm:ss")         ;
              cdt  = DT . toTime_t ( )                                       ;
              cdt += timezone                                                ;
              DT   = QDateTime::fromTime_t ( cdt )                           ;
            } else                                                           {
              QDate DD                                                       ;
              DD   = QDate::fromString(dt,"yyyy.MM.dd")                      ;
              DT   = QDateTime ( DD , QTime ( 0 , 0 , 0 , 0 ) )              ;
              cdt  = DT . toTime_t ( )                                       ;
            }                                                                ;
            //////////////////////////////////////////////////////////////////
            if ( cdt > fromTime )                                            {
              logs  . clear ( )                                              ;
              logs << sl [ i ] . code                                        ;
              logs << dt                                                     ;
              logs << QString::fromUtf8 ( sl [ i ] . ip      )               ;
              logs << QString::fromUtf8 ( sl [ i ] . message )               ;
              JournalCallback ( logs , Id , callback )                       ;
            }                                                                ;
          }                                                                  ;
          ManagerMutex . lock   ( )                                          ;
          cmi -> MemFree ( sl )                                              ;
          ManagerMutex . unlock ( )                                          ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    N::Time::msleep ( interval )                                             ;
  }                                                                          ;
}

void N::MtFeeder::BringUp(void)
{
}

void N::MtFeeder::BringDown(void)
{
}

QScriptValue N::MtFeederAttachment(QScriptContext * context,QScriptEngine * engine)
{
  Q_UNUSED ( context )                                 ;
  N::MtFeeder * api       = new N::MtFeeder ( engine ) ;
  QString       MtApiPath = ""                         ;
  QScriptValue  global                                 ;
  QScriptValue  path                                   ;
  global = engine -> globalObject (             )      ;
  path   = global  . property     ( "MtApiPath" )      ;
  if ( path . isString ( ) )                           {
    MtApiPath = path . toString ( )                    ;
  }                                                    ;
  api -> Load ( MtApiPath )                            ;
  return engine -> newQObject ( api )                  ;
}
