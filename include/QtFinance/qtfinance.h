/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001~2016 Neutrino International Inc.                      *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_FINANCE_H
#define QT_FINANCE_H

#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <Essentials>
#include <Mathematics>
#include <QtMtAPI>
#include <QtFTP>
#include <QtRPC>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTFINANCE_LIB)
#      define Q_FINANCE_EXPORT Q_DECL_EXPORT
#    else
#      define Q_FINANCE_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_FINANCE_EXPORT
#endif

namespace N
{

#pragma pack(push,1)

typedef struct                {
  qint32        bid           ;
  qint32        mid           ;
  qint32        ask           ;
  qint64        bidvolume     ;
  qint64        askvolume     ;
}               IntegerTick   ;

typedef struct                {
  double        bid           ;
  double        mid           ;
  double        ask           ;
  double        bidvolume     ;
  double        askvolume     ;
}               DoubleTick    ;

typedef struct                {
  float         bid           ;
  float         mid           ;
  float         ask           ;
  float         bidvolume     ;
  float         askvolume     ;
}               FloatTick     ;

typedef struct                {
  qint64        id            ;
  qint64        at            ;
  IntegerTick   Integer       ;
  DoubleTick    Double        ;
  FloatTick     Float         ;
}               TickUnion     ;

typedef struct                {
  qint32        open          ;
  qint32        high          ;
  qint32        mid           ;
  qint32        low           ;
  qint32        close         ;
  qint64        volume        ;
}               IntegerPeriod ;

typedef struct                {
  double        open          ;
  double        high          ;
  double        mid           ;
  double        low           ;
  double        close         ;
  double        volume        ;
}               DoublePeriod  ;

typedef struct                {
  float         open          ;
  float         high          ;
  float         mid           ;
  float         low           ;
  float         close         ;
  float         volume        ;
}               FloatPeriod   ;

typedef struct                {
  qint64        id            ;
  qint64        at            ;
  IntegerPeriod Integer       ;
  DoublePeriod  Double        ;
  FloatPeriod   Float         ;
}               PeriodUnion   ;

#pragma pack(pop)

class Q_FINANCE_EXPORT Monetary            ;
class Q_FINANCE_EXPORT ForexManipulator    ;
class Q_FINANCE_EXPORT SymbolsCenter       ;
class Q_FINANCE_EXPORT TickTransmitter     ;
class Q_FINANCE_EXPORT MtChannel           ;
class Q_FINANCE_EXPORT MtChannels          ;
class Q_FINANCE_EXPORT MtChannelSaver      ;
class Q_FINANCE_EXPORT MtFeeder            ;
class Q_FINANCE_EXPORT DukascopyMaps       ;
class Q_FINANCE_EXPORT Dukascopy           ;
class Q_FINANCE_EXPORT ScriptableDukascopy ;

namespace Finance
{

class Q_FINANCE_EXPORT Tick      ;
class Q_FINANCE_EXPORT Ticks     ;
class Q_FINANCE_EXPORT Symbol    ;
class Q_FINANCE_EXPORT Price     ;
class Q_FINANCE_EXPORT Chart     ;
class Q_FINANCE_EXPORT Charts    ;
class Q_FINANCE_EXPORT User      ;
class Q_FINANCE_EXPORT Users     ;
class Q_FINANCE_EXPORT Order     ;
class Q_FINANCE_EXPORT Orders    ;
class Q_FINANCE_EXPORT OrderTick ;

Q_FINANCE_EXPORT bool toChart (QByteArray & Line,Chart & chart) ;
Q_FINANCE_EXPORT bool toTick  (QByteArray & Line,Tick  & tick ) ;

}

namespace Forex
{

class Q_FINANCE_EXPORT Equations ;
class Q_FINANCE_EXPORT Importer  ;
class Q_FINANCE_EXPORT Reader    ;
class Q_FINANCE_EXPORT Connector ;
class Q_FINANCE_EXPORT Dealer    ;

// ArrayCalculator(s)
Q_FINANCE_EXPORT void   toMiddleTicks   (int items,unsigned char * array,int gaps,void * funcdata) ;
Q_FINANCE_EXPORT void   toDoubleTicks   (int items,unsigned char * array,int gaps,void * funcdata) ;
Q_FINANCE_EXPORT void   toFloatTicks    (int items,unsigned char * array,int gaps,void * funcdata) ;

Q_FINANCE_EXPORT void   toMiddlePeriods (int items,unsigned char * array,int gaps,void * funcdata) ;
Q_FINANCE_EXPORT void   toDoublePeriods (int items,unsigned char * array,int gaps,void * funcdata) ;
Q_FINANCE_EXPORT void   toFloatPeriods  (int items,unsigned char * array,int gaps,void * funcdata) ;

Q_FINANCE_EXPORT qint64 ToReversalTicks (qint64      ticks  ,
                                         TickUnion * tu     ,
                                         TickUnion * pu     ,
                                         qint64      full ) ;

}

namespace Finance
{

/*****************************************************************************
 *                                                                           *
 *                             Finance Operations                            *
 *                                                                           *
 *****************************************************************************/

#pragma pack(push,1)

typedef struct         {
  int    Type          ;
  char   Symbol [ 16 ] ;
  TUID   Time          ;
  TUID   UTC           ;
  TUID   Local         ;
  int    TimeZone      ;
  double Bid           ;
  double Ask           ;
  double Mid           ;
  double BidVolume     ;
  double AskVolume     ;
  int    iBid          ;
  int    iAsk          ;
  int    iMid          ;
} TickStructure        ;

#pragma pack(pop)

class Q_FINANCE_EXPORT Tick
{
  public:

    enum TickType              {
      MetaTrader = 0           ,
      FIXAPI     = 1 }         ;

    enum TickType Type         ;
    QString       symbol       ;
    TUID          Time         ; // Server Quote Time
    TUID          UTC          ; // UTC Quote Time
    TUID          Local        ; // Local Quote Time
    int           TimeZone     ;
    double        Bid          ; // Sell
    double        Ask          ; // Buy
    double        Mid          ; // ( Bid + Ask ) / 2
    double        BidVolume    ;
    double        AskVolume    ;
    int           iBid         ; // Bid * Symbol.Multiply
    int           iAsk         ; // Ask * Symbol.Multiply
    int           iMid         ; // Mid * Symbol.Multiply

    explicit  Tick          (void) ;
    virtual  ~Tick          (void) ;

    void      setTime       (time_t time,time_t fix);
    QDateTime Timestamp     (void) ;
    void      toStructure   (TickStructure * ts) ;
    void      fromStructure (TickStructure * ts) ;
    bool      FromSQL       (SqlConnection & Connection,int multiply) ;

    #ifdef QT_QTMTAPI_LIB
    void      operator =    (SymbolInfo & tick) ;
    void      operator =    (TickInfo   & tick) ;
    #endif

} ;

class Q_FINANCE_EXPORT Symbol
{ // Currency Pair Symbol
  public:

    int         digits     ; // floating point digits
    int         multiply   ; // value to make floating point into integer
    double      point      ; // incremental value of a single point using this symbol
    QString     symbol     ; // ISO 4217 currency pair symbol
    QStringList currencies ; // ISO 4217 currency name , there are two
    // if Symbol = EURUSD then Currency[0] = EUR , Currency[1] = USD

    explicit Symbol      (void) ;
    virtual ~Symbol      (void) ;

    Symbol & operator =  (Symbol & symbol) ;
    bool     operator == (Symbol & symbol) ; // check if the currency pair is identifical

    // setup currency information
    void     set         (QString first,QString second,int digits,double point) ;

} ;

class Q_FINANCE_EXPORT Price
{ // Price, not just for Forex
  public:

    QDateTime time  ; // time
    double    price ; // price

    explicit Price (void);
    virtual ~Price (void);

} ;

#pragma pack(push,1)

typedef struct      {
  TUID      Server  ;
  TUID      UTC     ;
  TUID      Local   ;
  double    Open    ;
  double    Close   ;
  double    High    ;
  double    Mid     ;
  double    Low     ;
  double    Volume  ;
  int       iOpen   ;
  int       iClose  ;
  int       iHigh   ;
  int       iMid    ;
  int       iLow    ;
  long long iVolume ;
  int       barType ;
  double    ZigZag  ;
} ChartStructure    ;

#pragma pack(pop)

class Q_FINANCE_EXPORT Chart
{
  public:

    TUID      Server  ; // Server Quote Time
    TUID      UTC     ; // UTC Quote Time
    TUID      Local   ; // Local Time
    double    Open    ;
    double    Close   ;
    double    High    ;
    double    Mid     ;
    double    Low     ;
    double    Volume  ;
    int       iOpen   ;
    int       iClose  ;
    int       iHigh   ;
    int       iMid    ;
    int       iLow    ;
    long long iVolume ;
    int       barType ; // enum QuoteBars
    double    ZigZag  ;

    explicit Chart         (void) ;
    virtual ~Chart         (void) ;

    double   DivideVolume  (void) ;
    void     Calculation   (void) ;
    void     Pressure      (int lowest,int highest,double * volume) ;
    void     toStructure   (ChartStructure * cs) ;
    void     fromStructure (ChartStructure * cs) ;
    bool     FromSQL       (SqlConnection & Connection,int multiply) ;
    bool     Within        (TUID t,int period) ;
    bool     Within        (QDateTime t,int period) ;
    void     Empty         (void) ;
    bool     Within        (Tick & tick,int period) ;
    void     Add           (Tick & tick) ;
    void     TickFinal     (int multiply) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Ticks
{
  public:

    enum            {
      BidItem = 0   ,
      AskItem = 1   ,
      MidItem = 2 } ;

    explicit      Ticks      (void) ;
    virtual      ~Ticks      (void) ;

    QList<Tick> & list       (void) ;
    QVariant    & value      (QString key) ;
    QVariant    & setValue   (QString key,QVariant value) ;
    int           add        (Tick & tick) ;
    int           count      (void) ;
    int           takeAt     (int index) ;
          Tick  & first      (void) ;
          Tick  & last       (void) ;
    const Tick  & at         (int index) ;
          Tick  & operator = (int index) ;

    virtual void  operate    (void) ;

    virtual int   Array      (int item,int size,double * array) ;
    virtual int   FromSQL    (SqlConnection & Connection,QString query) ;

  protected:

    QList<Tick> ticks     ;
    WMAPs       variables ;

    void          RuleOut    (int msec) ;
    void          MinMax     (void) ;

  private:

} ;

class Q_FINANCE_EXPORT Charts
{
  public:

    explicit       Charts     (void) ;
    virtual       ~Charts     (void) ;

    void           setDigits  (int digits) ;

    QList<Chart> & list       (void) ;
    QVariant     & value      (QString key) ;
    QVariant     & setValue   (QString key,QVariant value) ;
    int            add        (Chart & tick) ;
    int            count      (void) ;
    int            takeAt     (int index) ;
          Chart  & first      (void) ;
          Chart  & last       (void) ;
    const Chart  & at         (int index) ;
          Chart  & operator = (int index) ;

    virtual void   operate    (void) ;
    virtual void   MinMax     (void) ;

    int            FromSQL    (SqlConnection & Connection,QString query) ;

  protected:

    QList<Chart> charts    ;
    WMAPs        variables ;

  private:

} ;

class Q_FINANCE_EXPORT User
{
  public:

    int               login                  ; // UserRecord
    QString           group                  ;
    QString           password               ;
    bool              online                 ; // OnlineRecord
    QString           IP                     ;
    int               counter                ;
    int               enable                 ; // UserRecord
    int               enable_change_password ;
    int               enable_read_only       ;
    QString           password_investor      ;
    QString           password_phone         ;
    QString           name                   ;
    QString           country                ;
    QString           city                   ;
    QString           state                  ;
    QString           zipcode                ;
    QString           address                ;
    QString           phone                  ;
    QString           email                  ;
    QString           comment                ;
    QString           id                     ;
    QString           status                 ;
    QDateTime         regdate                ;
    QDateTime         lastdate               ;
    int               leverage               ;
    int               agent_account          ;
    QDateTime         timestamp              ;
    int               last_ip                ;
    int               volume                 ;
    double            balance                ;
    double            prevmonthbalance       ;
    double            prevbalance            ;
    double            credit                 ;
    double            interestrate           ;
    double            taxes                  ;
    double            prevmonthequity        ;
    double            prevequity             ;
    double            reserved2[2]           ;
    QByteArray        publickey              ;
    int               send_reports           ;
    unsigned int      mqid                   ; // UserRecord
    double            equity                 ; // Margin Level
    double            margin                 ;
    double            margin_free            ;
    double            margin_level           ;
    int               margin_type            ;
    int               level_type             ;

    explicit User       (void) ;
             User       (const User   & user  ) ;
    virtual ~User       (void) ;

    User &   operator = (const User   & user  ) ;
    #ifdef QT_MTMANAPI_H
    User &   operator = (UserRecord   & user  ) ;
    User &   operator = (MarginLevel  & Margin) ;
    User &   operator = (OnlineRecord & Online) ;
    #endif

    bool     enabled    (void) ;
    double   lots       (void) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Users
{
  public:

    QMap<int,User *> users     ;
    IMAPs            enabled   ;
    Enabler          functions ;

    explicit         Users       (void) ;
                     Users       (const Users  & users ) ;
    virtual         ~Users       (void) ;

    virtual bool     contains    (int login) ;
    virtual User  &  operator [] (int login) ;
    virtual User  &  operator += (const User   & user  ) ;

    #ifdef QT_MTMANAPI_H
    virtual User  &  operator += (UserRecord   & user  ) ;
    virtual User  &  operator += (MarginLevel  & Margin) ;
    virtual User  &  operator += (OnlineRecord & Online) ;
    virtual Users &  update      (int total,UserRecord   * users ) ;
    virtual Users &  update      (int total,MarginLevel  * margin) ;
    virtual Users &  update      (int total,OnlineRecord * online) ;
    #endif

    int              count       (void) ;
    CUIDs            logins      (void) ;

    virtual int      take        (int login) ;

    virtual void     operate     (void) ;

  protected:

    virtual void     assure      (int login) ;

  private:

} ;

class Q_FINANCE_EXPORT Order
{
  public:

    int               login             ;
    int               order             ;
    QString           symbol            ;
    int               digits            ;
    int               cmd               ;
    int               volume            ;
    QDateTime         OpenTime          ;
    int               state             ;
    double            OpenPrice         ;
    double            sl                ;
    double            tp                ;
    double            vSL               ;
    double            vTP               ;
    double            bid               ;
    double            ask               ;
    QDateTime         CloseTime         ;
    int               GwVolume          ;
    QDateTime         Expiration        ;
    char              reason            ;
    double            conv_rates  [ 2 ] ;
    double            commission        ;
    double            commission_agent  ;
    double            storage           ;
    double            ClosePrice        ;
    double            profit            ;
    double            taxes             ;
    int               magic             ;
    QString           comment           ;
    int               gw_order          ;
    int               activation        ;
    short             gw_open_price     ;
    short             gw_close_price    ;
    double            margin_rate       ;
    QDateTime         timestamp         ;
    int               multiply          ;

    explicit Order             (void) ;
             Order             (const Order & order) ;
    virtual ~Order             (void) ;

    Order & operator =         (const Order & order) ;

    #ifdef QT_QTMTAPI_LIB
    Order & operator =         (TradeRecord & trade) ;
    #endif

    virtual double  LOT         (void) ;
    virtual int     mOpenPrice  (void) ;
    virtual int     mClosePrice (void) ;
    virtual int     mSL         (void) ;
    virtual int     mTP         (void) ;
    virtual int     mvSL        (void) ;
    virtual int     mvTP        (void) ;
    virtual bool    isSL        (int spread) ;
    virtual bool    isTP        (int spread) ;

    virtual QString PriceString (double price) ;
    virtual QString LotString   (void) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT OrderTick : public QObject
                                 , public QScriptable
                                 , public Order
{
  Q_OBJECT
  public:

    explicit     OrderTick (QObject * parent = NULL) ;
                 OrderTick (QObject * parent,const Order & order) ;
    virtual     ~OrderTick (void) ;

  protected:

  private:

  public slots:

    virtual int       Login      (void) ;
    virtual int       OrderId    (void) ;
    virtual QString   Symbol     (void) ;
    virtual int       Digits     (void) ;
    virtual int       Cmd        (void) ;
    virtual double    Lots       (void) ;
    virtual QDateTime PlaceTime  (void) ;
    virtual QDateTime Now        (void) ;
    virtual double    Open       (void) ;
    virtual double    SL         (void) ;
    virtual double    TP         (void) ;
    virtual double    Bid        (void) ;
    virtual double    Ask        (void) ;
    virtual double    Commission (void) ;
    virtual double    Storage    (void) ;
    virtual double    Profit     (void) ;
    virtual double    Taxes      (void) ;
    virtual int       Magic      (void) ;
    virtual QString   Comment    (void) ;
    virtual double    MarginRate (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_FINANCE_EXPORT Orders
{
  public:

    int               login     ;
    QMap<int,Order *> orders    ;
    IMAPs             enabled   ;
    Enabler           functions ;
    WMAPs             variables ;

    explicit Orders (void) ;
    virtual ~Orders (void) ;

    #ifdef QT_QTMTAPI_LIB
    virtual Orders & assign      (int total,TradeRecord * trade) ;
    #endif

    virtual bool     contains    (int order) ;
    virtual Order  & operator [] (int order) ;
    virtual Order  & operator += (const Order & order) ;

    int              count       (void) ;
    CUIDs            keys        (void) ;

    virtual int      take        (int order) ;

    virtual void     operate     (void) ;

    virtual bool     hasEngine   (void) ;
    virtual QScriptEngine * Engine (void) ;
    virtual void     runEngine   (QScriptValue & api,
                                  QString symbol,
                                  QDateTime CTM,
                                  double bid,
                                  double ask) ;

  protected:

    virtual void assure      (int order) ;

  private:

} ;

}

namespace Forex
{

class Q_FINANCE_EXPORT Equations
{
  public:

    QString   Name         ;
    SUID      Supplier     ;
    SUID      SQL          ;
    SUID      Pair         ;
    int       Id           ;
    int       Period       ;
    int       Digits       ;
    int       Full         ;
    int       Multiply     ;
    int       Reversal     ;
    qint64    FullDigits   ;
    QString   TimeZone     ;
    int       Level        ;
    int       FromYear     ;
    int       ToYear       ;
    QString   identifier   ;
    QString   level        ;
    QString   symbol       ;
    QString   period       ;
    QString   base         ;
    QString   method       ;
    bool      direction    ;
    KMAPs     replacements ;
    Sql       Connexion    ;
    NAMEs     periods      ;
    int       QuotePeriod  ;
    QTimeZone TZ           ;

    explicit          Equations       (void) ;
    virtual          ~Equations       (void) ;

    virtual QString   TickColumns     (void) ;
    virtual QString   PeriodColumns   (void) ;

    virtual QString   WhereTicks      (TUID from,TUID to) ;
    virtual QString   WherePeriod     (TUID from,TUID to) ;

    virtual QDateTime NextTick        (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextM1          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextM5          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextM15         (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextM30         (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextH1          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextH4          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextD1          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextW1          (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextMN1         (QDateTime Now,QDateTime ToTime) ;
    virtual QDateTime NextBlock       (int period,QDateTime Now,QDateTime ToTime) ;

    virtual QString   PeriodName      (int period) ;
    virtual QString   QuoteTableName  (TUID timestamp) ;
    virtual QString   Replace         (QString symbol) ;

    virtual void      setPeriod       (int Period) ;
    virtual void      setDigits       (int digits) ;
    virtual void      setLevel        (int level) ;

    virtual bool      isCorrect       (void) ;

    virtual int       toInteger       (double value) ;
    virtual int       toReversal      (double value) ;

    virtual QString   FetchIdentifier (SqlConnection   & SC           ,
                                       QString           table        ,
                                       SUID              uuid       ) ;
    virtual SUID      FetchProvider   (SqlConnection   & SC           ,
                                       QString           table        ,
                                       QString           identifier ) ;
    virtual bool      GetConnexion    (SqlConnection  & connection  ,
                                       QString          table     ) ;
    virtual bool      PeriodExists    (SqlConnection  & connection) ;
    virtual bool      Get             (SqlConnection   & connection  ,
                                       QString           table       ,
                                       SUID              supplier    ,
                                       int               level       ,
                                       QString           name      ) ;
    virtual bool      GetSymbols      (SqlConnection   & connection  ,
                                       QString           table       ,
                                       SUID              supplier    ,
                                       int               level       ,
                                       QStringList     & symbols   ) ;

    virtual qint64    ToReversal      (qint64        ticks     ,
                                       TickUnion   * tu        ,
                                       TickUnion   * pu        ,
                                       qint64        full    ) ;
    virtual qint64    ToReversal      (qint64        ticks     ,
                                       TickUnion   * tu        ,
                                       TickUnion   * pu      ) ;
    virtual qint64    ToM1            (qint64        ticks     ,
                                       TickUnion   * tu        ,
                                       PeriodUnion * pu        ,
                                       int         * members ) ;
    virtual qint64    ToMx            (qint64        count     ,
                                       qint64        dt        ,
                                       PeriodUnion * tu        ,
                                       PeriodUnion * pu        ,
                                       int         * members ) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Importer : public Equations
{
  public:

    explicit Importer       (void) ;
    virtual ~Importer       (void) ;

    QString ConvertTable    (void) ;
    QString ConvertMerger   (void) ;
    bool Renew              (SqlConnection  & connection  ,
                             QString          table     ) ;

    bool CreatePeriods      (SqlConnection  & connection) ;
    bool Insert             (SqlConnection  & connection  ,
                             QString          line      ) ;
    bool Insert             (SqlConnection  & connection  ,
                             Finance::Tick  & tick      ) ;
    bool Insert             (SqlConnection  & connection  ,
                             Finance::Chart & chart     ) ;
    bool Insert             (SqlConnection  & connection  ,
                             TUID             quote       ,
                             int              open        ,
                             int              high        ,
                             int              low         ,
                             int              close       ,
                             TUID             volume      ,
                             QDateTime        Now       ) ;
    bool InsertFlag         (SqlConnection  & connection  ,
                             QString          Table       ,
                             QDateTime      & timestamp ) ;

    bool CreateTicks        (SqlConnection  & connection) ;
    bool CreateMonthly      (SqlConnection  & connection) ;
    bool CreateYearly       (SqlConnection  & connection) ;
    bool CreateTable        (SqlConnection  & connection) ;
    bool CreateConvert      (SqlConnection  & connection         ,
                             QString          table              ,
                             int              autoincrement = 1) ;
    bool MergeConverters    (SqlConnection  & connection ,
                             QString          table      ,
                             QStringList      Union    ) ;
    bool MergeConverters    (SqlConnection  & connection ,
                             QStringList      Union    ) ;

    bool Obtain             (SqlConnection  & connection  ,
                             Finance::Tick  & tick      ) ;
    bool Obtain             (SqlConnection  & connection  ,
                             Finance::Chart & chart     ) ;

    bool Obtain             (SqlConnection  & connection  ,
                             TickUnion      & tick      ) ;
    bool Obtain             (SqlConnection  & connection  ,
                             PeriodUnion    & period    ) ;

    bool Create             (SqlConnection & connection  ,
                             QString         table     ) ;
    bool CreateMerge        (SqlConnection & connection  ,
                             QString         table       ,
                             QStringList     Union     ) ;
    bool CreateTick         (SqlConnection & connection  ,
                             QString         table     ) ;
    bool CreateTicks        (SqlConnection & connection  ,
                             QString         table       ,
                             QStringList     Union     ) ;
    bool AutoIncrement      (SqlConnection & connection  ,
                             QString         table       ,
                             int             increment ) ;
    bool InsertChart        (SqlConnection & connection  ,
                             QString         line      ) ;
    bool Insert             (SqlConnection & connection  ,
                             QString         table       ,
                             TUID            quote       ,
                             int             open        ,
                             int             high        ,
                             int             low         ,
                             int             close       ,
                             TUID            volume    ) ;
    bool InsertTick         (SqlConnection & connection  ,
                             QString         line      ) ;
    bool InsertTick         (SqlConnection & connection  ,
                             QString         table       ,
                             TUID            quote       ,
                             int             bidprice    ,
                             int             askprice    ,
                             TUID            bidvolume   ,
                             TUID            askvolume ) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Reader : public Equations
{
  public:

    explicit  Reader          (void) ;
    virtual  ~Reader          (void) ;

    bool      Renew           (SqlConnection   & connection  ,
                               QString           table     ) ;
    bool      Obtain          (Finance::Ticks  & ticks       ,
                               QDateTime         FromTime    ,
                               QDateTime         ToTime    ) ;
    bool      Obtain          (Finance::Charts & charts      ,
                               QDateTime         FromTime    ,
                               QDateTime         ToTime    ) ;

    bool      Obtain          (SqlConnection  & connection  ,
                               TickUnion      & tick      ) ;
    bool      Obtain          (SqlConnection  & connection  ,
                               PeriodUnion    & period    ) ;

    QDateTime NextTime        (QDateTime Now,QDateTime ToTime) ;

    bool      Obtain          (SqlConnection   & connection  ,
                               Finance::Ticks  & ticks       ,
                               QString           query     ) ;
    bool      Obtain          (SqlConnection   & connection  ,
                               Finance::Charts & charts      ,
                               QString           query     ) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Dealer
{
  public:

    QString    Name       ;
    SUID       Id         ;
    SUID       Supplier   ;
    SUID       Connection ;
    SUID       Indicator  ;
    QByteArray Conf       ;
    WMAPs      Variables  ;

    explicit         Dealer             (void) ;
    virtual         ~Dealer             (void) ;

    virtual int      DealerType         (void) const = 0 ;
    virtual bool     startTrade         (void) = 0 ;
    virtual bool     stopTrade          (void) = 0 ;
    virtual int      isTrading          (void) = 0 ;
    virtual bool     load               (QString filename) = 0 ;
    virtual QStrings SuppliedSymbols    (void) = 0 ;
    virtual bool     isSupplied         (QString symbol) = 0 ;

    virtual int      AddTransmitter     (TickTransmitter * transmitter) = 0 ;
    virtual int      AddChannels        (MtChannels * channels) = 0 ;
    virtual int      AddUsers           (Finance::Users * users) = 0 ;
    virtual int      AddOrders          (Finance::Orders * orders) = 0 ;
    virtual int      AddHistory         (Finance::Orders * orders) = 0 ;

    virtual void     connectManipulator (DataManipulator * manipulator) = 0 ;
    virtual void     Link               (QString Type,QObject * object) = 0 ;

    virtual bool     Obtain             (Finance::Ticks  & ticks            ,
                                         QString           supplier         ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Ticks  & ticks            ,
                                         SUID              supplier         ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Ticks  & ticks            ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Ticks  & ticks            ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Ticks  & ticks            ,
                                         QString           symbol           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;

    virtual bool     Obtain             (Finance::Charts & charts           ,
                                         QString           supplier         ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         int               period           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Charts & charts           ,
                                         SUID              supplier         ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         int               period           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Charts & charts           ,
                                         QString           identifier       ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         int               period           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Charts & charts           ,
                                         QString           level            ,
                                         QString           symbol           ,
                                         int               period           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;
    virtual bool     Obtain             (Finance::Charts & charts           ,
                                         QString           symbol           ,
                                         int               period           ,
                                         QDateTime         FromTime         ,
                                         QDateTime         ToTime     ) = 0 ;

    virtual bool     Command            (QString command) = 0 ;
    virtual bool     Command            (QString command,N::VarArgs & results) = 0 ;
    virtual bool     Command            (N::VarArgs command) = 0 ;
    virtual bool     Command            (N::VarArgs command,N::VarArgs & results) = 0 ;

    virtual QString  ErrorString        (int code) = 0 ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT Connector
{
  public:

    explicit         Connector (void) ;
    virtual         ~Connector (void) ;

    virtual int      type      (void) const = 0 ;
    virtual QString  name      (void) const = 0 ;
    virtual Dealer * dealer    (QObject * parent) = 0 ;

  protected:

  private:

} ;

}

class Monetary
{ // Basic configuration of monetary, ISO 4217 definitions and currency pairs
  public:

    CUIDs       ID        ; // ISO 4217 numeric id lists
    QStringList ISO       ; // ISO 4217 three digits name lists
    UUIDs       Uuids     ; // ISO 4217 uuid lists
    EMAPs       IdMap     ; // id             => name
    RMAPs       MapUuid   ; // id             => uuid
    EMAPs       Names     ; // id             => translated name
    CMAPs       UuidMap   ; // uuid           => name
    NAMEs       UuidIso   ; // uuid           => name
    UMAPs       MapId     ; // name           => id
    ZMAPs       IsoUuid   ; // name           => uuid
    CUIDs       Pairs     ; // (first,second) lists
    EMAPs       PairNames ; // (first,second) => name
    CMAPs       PairUuid  ; // (first,second) => uuid
    RMAPs       UuidPair  ; // uuid           => (first,second)

                           Monetary      (void) ;
    virtual               ~Monetary      (void) ;

    virtual int            count         (void) ; // ISO . count ( )
    virtual void           clear         (void) ; // clear everything

    virtual QString        operator [ ]  (int     index) ; // IdMap   [ index ]
    virtual int            operator [ ]  (SUID    index) ; // MapUuid [ index ]
    virtual int            operator [ ]  (QString name ) ; // MapId   [ name  ]

    virtual QString        Name          (int index) ; // Names [ index ]
    virtual QString        Pairname      (int pair) ; // names such as EUR/USD by PairId
    // extract PairId from string, (first,second) are positions in symbol string
    virtual int            SymbolId      (QString symbol,int first,int second) ;
    // a currency pair is composed by ( first * 1000 ) + second, read ISO 4217 standard please
    virtual int            ComposeId     (int first,int second) ;
    // decode PairId into QPair<first,second>
    virtual QPair<int,int> DecodeId      (int PairId) ;
    // You can change the composition method by inherit ComposeId and DecodeId

    // load ISO 4217 from table, in CIOS system, the table is currency
    virtual bool           load          (SqlConnection & Connection,QString table               ) ;
    // load ISO 4217 translated names from table, in CIOS system, the table is names
    virtual bool           loadName      (SqlConnection & Connection,QString table,int LanguageId) ;
    // load currency pairs from table, in CIOS system, the table is currencypairs
    virtual bool           loadPairs     (SqlConnection & Connection,QString table               ) ;
    // write currency settings into C/C++ real code
    virtual bool           ToCpp         (QByteArray & data) ;
    // load information from memory, in case you want to use shared memory or privated memory section to be a storage
    virtual bool           FromByteArray (QByteArray & data) ;
    virtual bool           FromByteArray (void * data,int size) ;
    // write information from memory, in case you want to use shared memory or privated memory section to be a storage
    virtual bool           ToByteArray   (QByteArray & data,bool resize = false) ;
    virtual bool           ToByteArray   (void * data,int size) ;
    // is readable
    virtual bool           canRead       (QByteArray & data) ;
    // is writable
    virtual bool           canWrite      (QByteArray & data) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT ForexManipulator : public DataManipulator
{
  public:

    explicit      ForexManipulator (QString master) ;
    virtual      ~ForexManipulator (void) ;

    virtual int   Providers        (void) ;
    virtual int   setProviders     (SUID division,UUIDs & uuids) ;
    virtual bool  Providers        (UUIDs & uuids) ;

    virtual int   Tradings         (void) ;
    virtual int   setTradings      (int tradings) ;
    virtual int * TradingsArray    (void) ;

    virtual int   Connections      (void) ;

    virtual bool  GetDBs           (QStringList & DBs) ;
    virtual bool  SetDBs           (QStringList & DBs) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT SymbolsCenter
{
  public:

    explicit         SymbolsCenter    (void) ;
    virtual         ~SymbolsCenter    (void) ;

    virtual bool     exists           (QString name,DataManipulator & manipulator) ;
    virtual bool     create           (QString name,DataManipulator & manipulator) ;

    virtual bool     ObtainPIPs       (WMAPs & variables) ;

    virtual bool     setSymbols       (QStringList symbols) ;
    virtual QStrings Symbols          (void) ;
    virtual void *   Details          (void) ;

    virtual bool     assignMetaTrader (int total,void * CONSYMBOLs) ;

  protected:

    void * SharingBlock ;

  private:

} ;

class Q_FINANCE_EXPORT TickTransmitter : public Enabler
{
  public:

    explicit     TickTransmitter (void) ;
    virtual     ~TickTransmitter (void) ;

    virtual void Append          (QString symbol) = 0 ;
    virtual void setDigits       (QString symbol,int digits) = 0 ;
    virtual void Hide            (QString symbol) = 0 ;
    virtual void Quote           (QString symbol,QDateTime CTM,double bid,double ask) = 0 ;
    virtual void Update          (void) = 0 ;
    virtual void Ready           (void) = 0 ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT MtChannel
{
  public:

    qint64 Time           ;
    qint64 Sent     [ 4 ] ;
    qint64 Received [ 4 ] ;

    explicit    MtChannel   (void) ;
                MtChannel   (const MtChannel & channel) ;
    virtual    ~MtChannel   (void) ;

    MtChannel & operator  = (const MtChannel & channel) ;
    MtChannel & operator -= (const MtChannel & channel) ;

    void        setSent     (int API,qint64 value) ;
    void        setReceived (int API,qint64 value) ;

  protected:

  private:

} ;

class Q_FINANCE_EXPORT MtChannels
{
  public:

    MtChannel        Base      ;
    QList<MtChannel> Channels  ;
    qint64           Sent      ;
    qint64           Received  ;
    WMAPs            Variables ;

    explicit     MtChannels (void) ;
    virtual     ~MtChannels (void) ;

    virtual void Update     (QtMtAPI & api) ;
    virtual void Operate    (void) ;

    QVariant   & Value      (QString key) ;
    QVariant   & setValue   (QString key,QVariant value) ;

    int          Obtain     (int length,qint64 * times) ;
    void         Obtain     (int length,int result[8],qint64 ** values) ;
    int          Obtain     (int partindex,int length,qint64 * values) ;
    int          Obtain     (int part,int index,int length,qint64 * values) ;

    int          count      (void) ;

  protected:

    void         RuleOut    (void) ;

  private:

} ;

class Q_FINANCE_EXPORT MtChannelSaver : public MtChannels
                                      , public Thread
{
  public:

    explicit     MtChannelSaver (void) ;
    virtual     ~MtChannelSaver (void) ;

    virtual void Update         (QtMtAPI & api) ;
    virtual void Operate        (void) ;

  protected:

    Mutexz mutex ;

    virtual void run            (int Type,ThreadData * data) ;

    virtual void Saver          (void) ;
    virtual void Saving         (void) ;

  private:

} ;

class Q_FINANCE_EXPORT MtFeeder : public QObject
                                , public QScriptable
                                , public Enabler
                                , public Thread
                                , public QtMtAPI
{
  Q_OBJECT
  public:

    ScriptManager Scripts ;

    explicit          MtFeeder           (QObject * parent = NULL) ;
    virtual          ~MtFeeder           (void) ;

    virtual void      TickAppend         (QString symbol) ;
    virtual void      TickDigits         (QString symbol,int digits) ;
    virtual void      TickHide           (QString symbol) ;
    virtual void      TickQuote          (QString symbol,QDateTime CTM,double bid,double ask) ;
    virtual void      TickUpdate         (void) ;
    virtual void      TickReady          (void) ;
    virtual void      RemoveTransmitters (void) ;
    virtual void      RemoveChannels     (void) ;
    virtual void      RemoveUsers        (void) ;
    virtual void      RemoveOrders       (void) ;
    virtual void      RemoveHistory      (void) ;

    virtual QString   TradeString        (int cmd) ;

    virtual QString   errorString        (int code) ;

    virtual bool      PlaceOrder         (int          api       ,
                                          int          login     ,
                                          QString      symbol    ,
                                          int          type      ,
                                          int          cmd       ,
                                          int          volume    ,
                                          double       price     ,
                                          double       sl        ,
                                          double       tp        ,
                                          QString      comment   ,
                                          N::VarArgs & results ) ;
    virtual bool      CloseOrder         (int          api       ,
                                          int          login     ,
                                          int          order     ,
                                          double       lots      ,
                                          QString      comment   ,
                                          N::VarArgs & results ) ;
    virtual bool      CloseAllOrders     (int          login     ,
                                          QString      group     ,
                                          QString      comment   ,
                                          N::VarArgs & results ) ;
    virtual bool      ModifyOrder        (int          api       ,
                                          int          login     ,
                                          int          order     ,
                                          double       sl        ,
                                          double       tp        ,
                                          QString      comment   ,
                                          N::VarArgs & results ) ;

  protected:

    QStringList               symbols       ;
    QStringList               groups        ;
    CUIDs                     logins        ;
    CUIDs                     OnlineLogins  ;
    UMAPs                     SymbolDigits  ;
    EMAPs                     LoginIPs      ;
    IMAPs                     LoginCounters ;
    QDateTime                 Lastest       ;
    QMap<QString,QDateTime>   LastestTicks  ;
    CUIDs                     Tradings      ;
    QMap<QString,CUIDs      > TradingOrders ;
    QMap<int,QScriptEngine *> Engines       ;
    Mutexz                    MutexArray    ;
    QList<TickTransmitter  *> Transmitters  ;
    QList<MtChannels       *> ByteChannels  ;
    QList<Finance::Users   *> allUsers      ;
    QList<Finance::Orders  *> allOrders     ;
    QList<Finance::Orders  *> allHistory    ;

    virtual void      run                (int type,N::ThreadData * data) ;
    virtual void      Extras             (int type,N::ThreadData * data) ;

    virtual void      TickUpdated        (QString   symbol  ,
                                          QDateTime CTM     ,
                                          double    bid     ,
                                          double    ask   ) ;

    virtual void      MonitorSilence     (N::VarArgs & args,int Id) ;
    virtual void      SymbolStopped      (QString symbol,qint64 timeout,QString callback,int Id) ;

    virtual bool      EquityHealth       (int     login      ,
                                          double  balance    ,
                                          double  equity     ,
                                          double  margin     ,
                                          int     multiply   ,
                                          QString callback   ,
                                          int     Id       ) ;
    virtual void      MaintainEquity     (N::VarArgs & args,int Id) ;
    virtual void      MaintainHealth     (N::VarArgs & args,int Id) ;
    virtual void      HealthEquity       (N::VarArgs & args,int Id) ;
    virtual bool      PermitToClose      (int     login      ,
                                          int     order      ,
                                          int     cmd        ,
                                          double  profit     ,
                                          QString callback   ,
                                          int     Id       ) ;
    virtual bool      CloseMaxLoss       (int login,QString permit,int Id) ;

    virtual void      ForcelyClose       (N::VarArgs & args,int Id) ;
    virtual bool      CloseThisAccount   (int     api               ,
                                          int     login             ,
                                          QString callback          ,
                                          int     Id              ) ;

    virtual void      EquityCheck        (N::VarArgs & args,int Id) ;
    virtual bool      AIAEquityCheck     (int       api             ,
                                          int       login           ,
                                          QString   callback        ,
                                          QDateTime from            ,
                                          QString   commentContains ,
                                          int       Id            ) ;

    virtual void      OrdersSync         (N::VarArgs & args,int Id) ;
    virtual void      HistorySync        (N::VarArgs & args,int Id) ;
    virtual void      KeepConnected      (N::VarArgs & args,int Id) ;
    virtual void      KeepQuotes         (N::VarArgs & args,int Id) ;
    virtual void      JournalCallback    (N::VarArgs & args,int Id,QString callback) ;
    virtual void      ServerJournal      (N::VarArgs & args,int Id) ;
    virtual void      Transmissions      (void) ;

    virtual void      StartPumping       (int type,void * data) ;
    virtual void      UpdateGroups       (int type,void * data) ;
    virtual void      UpdateUsers        (int type,void * data) ;
    virtual void      ObtainOnlines      (int type,void * data) ;
    virtual void      UpdateBidAsk       (int type,void * data) ;
    virtual void      ObtainNews         (int type,void * data) ;
    virtual void      ObtainNewsBody     (int type,void * data) ;
    virtual void      UpdateTrades       (int type,void * data) ;
    virtual void      StopPumping        (int type,void * data) ;
    virtual void      PumpingPing        (int type,void * data) ;

    virtual void      GetOnlines         (void) ;
    virtual void      GetUsers           (void) ;
    virtual void      SyncUsers          (void) ;

    virtual void      TransmitQuote      (QString symbol,QDateTime CTM,double bid,double ask) ;
    virtual void      OrdersQuote        (QString symbol,QDateTime CTM,double bid,double ask) ;

    virtual bool      isDrop             (void) ;
    virtual void      BringUp            (void) ;
    virtual void      BringDown          (void) ;

  private:

    void              TriggerEquity      (N::VarArgs & args) ;
    void              TriggerHealth      (N::VarArgs & args) ;
    void              SymbolsTimeout     (N::VarArgs & args) ;
    void              TriggerCloseAll    (N::VarArgs & args) ;
    void              TriggerEquityClose (N::VarArgs & args) ;
    QScriptEngine   * GetEngine          (int id) ;

  public slots:

    virtual void      report             (QString msg) ;

    virtual int       addTransmitter     (TickTransmitter * transmitter) ;
    virtual int       addChannels        (MtChannels * channels) ;
    virtual int       addUsers           (Finance::Users * users) ;
    virtual int       addOrders          (Finance::Orders * orders) ;
    virtual int       addHistory         (Finance::Orders * orders) ;

    virtual bool      SetEnabled         (int Id,bool enable) ;
    virtual bool      IsEnabled          (int Id) ;
    virtual bool      SetEnabled         (QString Id,bool enable) ;
    virtual bool      IsEnabled          (QString Id) ;

    virtual void      Interfaces         (int APIs) ;
    virtual void      setLogin           (QString account,QString pass) ;
    virtual bool      ConnectTo          (QString ip,bool PumpingEx) ;
    virtual bool      IsConnected        (void) ;
    virtual void      Disconnect         (void) ;

    virtual QStrings  AllGroups          (void) ;
    virtual CUIDs     AllUsers           (void) ;
    virtual CUIDs     AllOnlines         (void) ;
    virtual QString   OnlineIP           (int login) ;
    virtual int       OnlineCounter      (int login) ;
    virtual QString   GroupOfUser        (int api,int login) ;

    virtual int       SymbolsRefresh     (int API) ;
    virtual int       TotalSymbols       (void) ;
    virtual QString   Symbol             (int at) ;
    virtual bool      hasSymbol          (QString symbol) ;
    virtual QStrings  AllSymbols         (void) ;

    virtual int       SymbolAdd          (int API,QString symbol) ;
    virtual qint64    SymbolSlience      (QString symbol) ;

    virtual int       TotalTradings      (void) ;

    virtual bool      Start              (QString command) ;

    virtual void      CheckBidAsk        (void) ;
    virtual void      CheckTradings      (int type) ;

    virtual int       SrvRestart         (int api) ;
    virtual int       SrvChartsSync      (int api) ;
    virtual int       SrvLiveUpdateStart (int api) ;
    virtual int       SrvFeedsRestart    (int api) ;
    virtual int       Ping               (int api) ;
    virtual int       AddRef             (int api) ;
    virtual int       Release            (int api) ;
    virtual int       NewsTotal          (int api) ;
    virtual int       BytesSent          (int api) ;
    virtual int       BytesReceived      (int api) ;
    virtual QDateTime ServerTime         (int api) ;

    // Trade Transaction
    virtual bool      CloseUserOrders    (int api,int login) ;
    virtual bool      CloseOrder         (int api,int order,double lots = -1) ;
    virtual bool      setReadOnly        (int login,bool readOnly) ;
    virtual double    AccountDeposit     (int api,int login,QDateTime from,QString commentContains = "") ;

  protected slots:

  private slots:

    void              SendGroups         (void) ;
    void              SendUsers          (void) ;
    void              SendOnlines        (void) ;

  signals:

    void              EmitGroups         (void) ;
    void              EmitUsers          (void) ;
    void              EmitOnlines        (void) ;
    void              GroupsChanged      (MtFeeder * feeder) ;
    void              UsersChanged       (MtFeeder * feeder) ;
    void              OnlinesChanged     (MtFeeder * feeder) ;

} ;

class Q_FINANCE_EXPORT DukascopyMaps
{
  public:

    explicit            DukascopyMaps (void) ;
                        DukascopyMaps (const DukascopyMaps & dukas) ;
    virtual            ~DukascopyMaps (void) ;

    DukascopyMaps &     operator =    (const DukascopyMaps & dukas) ;
    TimeBits      &     operator [ ]  (QString key) ;

    virtual void        setData       (QByteArray & data) ;
    virtual void        setData       (void * data,qint64 size) ;

    virtual QDateTime   TimeValue     (QString key) ;
    virtual void        setTime       (QString key,QDateTime time) ;

    virtual QDateTime   TimeValue     (QString symbol,QString key) ;
    virtual void        setTime       (QString symbol,QString key,QDateTime time) ;

    virtual bool        PrepareMaps   (void) ;

    virtual QStringList TimeSymbols   (void) ;
    virtual bool        addSymbol     (QString symbol,QDate start) ;

    virtual bool        LoadMaps      (QString filename) ;
    virtual bool        SaveMaps      (QString filename) ;
    virtual void        setUsage      (QString usage) ;
    virtual QString     Usage         (void) ;

    virtual bool        BrandNew      (void) ;
    virtual void        assignTo      (void * data  ,
                                       qint64 size  ,
                                       QDate  start ,
                                       QDate  end ) ;

  protected:

    void                  * memoryData  ;
    qint64                  memorySize  ;
    QMap<QString,TimeBits > TimeArray   ;
    QMap<QString,void    *> SymbolArray ;
    QMap<QString,QDateTime> DateTimes   ;

    virtual bool        appendSymbol  (QString symbol   ,
                                       QDate   start    ,
                                       qint64  offset ) ;

  private:

} ;

class Q_FINANCE_EXPORT Dukascopy : public Enabler
                                 , public DukascopyMaps
{
  public:

    explicit     Dukascopy       (void) ;
    virtual     ~Dukascopy       (void) ;

    virtual void setRoot         (QString path) ;
    virtual void setTemp         (QString path) ;
    virtual void setZIP          (QString path) ;
    virtual void setDecode       (QString pattern) ;
    virtual void setData         (QString feed) ;

    virtual void Setup           (void) ;

    virtual void Check           (QString symbol) ;
    virtual void Import          (QString symbol) ;

    virtual bool Investigate     (QDir & dir,QDate today) ;
    virtual bool Investigate     (QString symbol,QDate today) ;

    virtual void ListStatus      (QDir & dir,QString filename,QStringList & list) ;
    virtual void SqlStatus       (QDir & dir,QStringList & list) ;
    virtual void CsvStatus       (QDir & dir,QStringList & list) ;
    virtual void ToCSV           (QString symbol,QDate today,QFile & file) ;

    virtual bool download        (QString symbol,int year,int month,int day,int hour) ;
    virtual bool download        (QString symbol,QDate today) ;
    virtual bool download        (Thread * thread,ThreadData * data,QString symbol,QDate today) ;
    virtual bool import          (QString symbol,int year,int month,int day,int hour) ;
    virtual bool import          (QString symbol,QDate today) ;

    virtual QDir TickPath        (QString symbol,QDate today) ;

    virtual void print           (QString s) ;

  protected:

    QDir     Root        ;
    QDir     Temp        ;
    QString  Zip         ;
    QString  Decode      ;
    QString  Datafeed    ;
    QStrings Symbols     ;
    UMAPs    Digits      ;
    WMAPs    TimeRecords ;
    WMAPs    StartTime   ;
    WMAPs    TailTime    ;
    Mutexz   Mutex       ;
    qint64   Received    ;
    qint64   TotalFiles  ;
    Sql      sql         ;
    QString  Method      ;
    QString  Identifier  ;
    QString  Level       ;
    int      Period      ;
    int      FromYear    ;
    int      ToYear      ;
    int      TimeZone    ;
    QString  CNH         ;
    WMAPs    StartSql    ;
    WMAPs    TailSql     ;
    qint64   TotalTicks  ;
    LMAPs    Initialized ;

    virtual bool Get             (QString symbol,QString filename,QString decoded,QByteArray & data) ;

    virtual void UpdateSymbol    (QString symbol) ;
    virtual void SaveSymbol      (QString symbol) ;

    virtual bool GetTime         (QString symbol,QDateTime & Now,QDateTime & Fetch) ;
    virtual bool BeforeTime      (QString symbol,QDateTime & Now,QDateTime & Fetch) ;
    virtual bool Completion      (QString symbol,QDateTime & today) ;

    virtual void WriteCompletion (QString symbol,QDate today) ;

    virtual void UpdateSql       (QString symbol) ;
    virtual void SaveSql         (QString symbol) ;

    virtual bool GetSql          (QString symbol,QDateTime & Now,QDateTime & Fetch) ;
    virtual bool BeforeSql       (QString symbol,QDateTime & Now,QDateTime & Fetch) ;
    virtual bool CompleteSql     (QString symbol,QDateTime & today) ;

    virtual void addSize         (qint64 size) ;
    virtual void addFiles        (qint64 files) ;
    virtual void addTicks        (qint64 ticks) ;

    virtual bool Hourly          (void) ;

    virtual bool TickExists      (QString symbol,QDateTime time) ;

  private:

} ;

class Q_FINANCE_EXPORT ScriptableDukascopy : public QObject
                                           , public QScriptable
                                           , public Thread
                                           , public Dukascopy
{
  Q_OBJECT
  public:

    QMap<QString,DukascopyMaps *> Maps      ;
    QMap<QString,qint64        *> Externals ;

    explicit     ScriptableDukascopy (QObject * parent = NULL) ;
    virtual     ~ScriptableDukascopy (void) ;

    virtual void print               (QString s) ;

  protected:

    ZMAPs Values ;

    virtual void ThreadEvent         (void) ;

    virtual void run                 (int type,ThreadData * data) ;

    virtual void Investigate         (ThreadData * data,QString symbol) ;
    virtual void ToCSV               (ThreadData * data,QString symbol) ;
    virtual void CsvToSql            (ThreadData * data,QString symbol) ;
    virtual void Build               (ThreadData * data,QString symbol) ;
    virtual void Optimize            (ThreadData * data,QString symbol) ;
    virtual void RealTime            (ThreadData * data,QString symbol) ;
    virtual void Building            (ThreadData * data,QString symbol) ;
    virtual void Mapping             (ThreadData * data,QString symbol) ;

    virtual void Report              (QDateTime Start,QDateTime & Now,QString section) ;
    virtual void ReportSql           (QDateTime Start,QDateTime & Now,QString section) ;

    virtual void Launch              (int threads,int type,QString section) ;
    virtual void LaunchSql           (int threads,int type,QString section) ;

    virtual void RealTime            (ThreadData * data) ;
    virtual void Investigate         (ThreadData * data) ;
    virtual void Importing           (ThreadData * data) ;
    virtual void Mapping             (ThreadData * data) ;
    virtual void PeriodicInvestigate (ThreadData * data) ;
    virtual void Optimize            (ThreadData * data) ;
    virtual void PeriodicOptimize    (ThreadData * data) ;

    virtual void WaitCompletion      (QString section,QDateTime & Start,QDateTime & Now) ;
    virtual void WaitSqlCompletion   (QString section,QDateTime & Start,QDateTime & Now) ;

    virtual void AddExternal         (QString section) ;
    virtual void SubExternal         (QString section) ;

  private:

  public slots:

    virtual bool SetEnabled          (int Id,bool enable) ;
    virtual bool HasEnabled          (int Id) ;
    virtual bool IsEnabled           (int Id) ;

    virtual bool SetEnabled          (SUID Id,bool enable) ;
    virtual bool HasEnabled          (SUID Id) ;
    virtual bool IsEnabled           (SUID Id) ;

    virtual bool SetEnabled          (QString Id,bool enable) ;
    virtual bool HasEnabled          (QString Id) ;
    virtual bool IsEnabled           (QString Id) ;

    virtual bool IsAllTrue           (void) ;
    virtual bool IsAllFalse          (void) ;

    virtual bool CanLock             (void) ;
    virtual bool SetLock             (bool lock) ;

    virtual void CleanEnabler        (void) ;

    virtual qint64 KeyValue          (QString key) ;
    virtual qint64 setKeyValue       (QString key,qint64 value) ;

    virtual void CleanSymbols        (void) ;
    virtual void AddSymbol           (QString symbol) ;
    virtual int  AllSymbols          (void) ;
    virtual QString SymbolAt         (int index) ;

    virtual void setDirectory        (QString path) ;
    virtual void setTemporary        (QString path) ;
    virtual void setZip              (QString path) ;
    virtual void setPattern          (QString pattern) ;
    virtual void setDataFeed         (QString feed) ;

    virtual void Initialize          (void) ;

    virtual void Tell                (QString symbol,int year,int month,int day,int hour,int digits) ;
    virtual void Download            (QString symbol,int year,int month,int day,int hour) ;
    virtual void Import              (QString symbol,int year,int month,int day,int hour) ;

    virtual void Start               (int msec,int downloads,int threads) ;
    virtual void FullImport          (int threads) ;
    virtual void Optimize            (int threads) ;
    virtual void Investigate         (int threads) ;
    virtual void ToCSV               (int threads) ;

    virtual void Take                (QString symbol) ;
    virtual void TakeFirst           (int count) ;
    virtual void TakeLast            (int count) ;

    virtual bool ImportCSV           (QString symbol,QString filename) ;
    virtual void CsvToSql            (int threads) ;

    virtual bool ImportBI5           (QString symbol,QDate today,int hour) ;
    virtual bool ImportBI5           (QString symbol,QDate today) ;

    virtual void Investigate         (void) ;
    virtual void Optimize            (void) ;
    virtual bool GetSettings         (Settings & settings,qint32 * processors) ;

    virtual void Optimize            (Thread * thread,ThreadData * data) ;
    virtual void PeriodicOptimize    (Thread * thread,ThreadData * data) ;

  protected slots:

  private slots:

  signals:

} ;

Q_FINANCE_EXPORT QScriptValue MtFeederAttachment (QScriptContext * context,QScriptEngine * engine) ;
Q_FINANCE_EXPORT QScriptValue DukascopyCreate    (QScriptContext * context,QScriptEngine * engine) ;

}

Q_DECLARE_METATYPE(N::Finance::Tick)
Q_DECLARE_METATYPE(N::Finance::Ticks)
Q_DECLARE_METATYPE(N::Finance::Symbol)
Q_DECLARE_METATYPE(N::Finance::Price)
Q_DECLARE_METATYPE(N::Finance::Chart)
Q_DECLARE_METATYPE(N::Finance::Charts)
Q_DECLARE_METATYPE(N::Finance::User)
Q_DECLARE_METATYPE(N::Finance::Users)
Q_DECLARE_METATYPE(N::Finance::Order)
Q_DECLARE_METATYPE(N::Finance::Orders)
Q_DECLARE_METATYPE(N::Forex::Equations)
Q_DECLARE_METATYPE(N::Forex::Importer)
Q_DECLARE_METATYPE(N::Forex::Reader)
Q_DECLARE_METATYPE(N::Monetary)
Q_DECLARE_METATYPE(N::SymbolsCenter)
Q_DECLARE_METATYPE(N::MtChannel)
Q_DECLARE_METATYPE(N::MtChannels)
Q_DECLARE_METATYPE(N::MtChannelSaver)
Q_DECLARE_METATYPE(N::DukascopyMaps)
Q_DECLARE_METATYPE(N::Dukascopy)

Q_DECLARE_INTERFACE(N::TickTransmitter  , "com.neutrino.forex.transmitter.tick"   )
Q_DECLARE_INTERFACE(N::Forex::Connector , "com.neutrino.forex.connectors.console" )

QT_END_NAMESPACE

#endif
