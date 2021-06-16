#include <qtfinance.h>

N::Finance::Order:: Order (void)
{
  login             = -1       ;
  order             = -1       ;
  symbol            = ""       ;
  digits            =  0       ;
  cmd               = -1       ;
  volume            = -1       ;
  OpenTime          = nTimeNow ;
  state             = -1       ;
  OpenPrice         =  0       ;
  sl                =  0       ;
  tp                =  0       ;
  vSL               =  0       ;
  vTP               =  0       ;
  bid               =  0       ;
  ask               =  0       ;
  CloseTime         = nTimeNow ;
  GwVolume          = -1       ;
  Expiration        = nTimeNow ;
  reason            =  0       ;
  conv_rates  [ 0 ] =  0       ;
  conv_rates  [ 1 ] =  0       ;
  commission        =  0       ;
  commission_agent  =  0       ;
  storage           =  0       ;
  ClosePrice        =  0       ;
  profit            =  0       ;
  taxes             =  0       ;
  magic             = -1       ;
  comment           = ""       ;
  gw_order          = -1       ;
  activation        = -1       ;
  gw_open_price     =  0       ;
  gw_close_price    =  0       ;
  margin_rate       =  0       ;
  timestamp         = nTimeNow ;
  multiply          = 1        ;
}

N::Finance::Order:: Order (const Order & o)
{
  ME = o ;
}

N::Finance::Order::~Order (void)
{
}

N::Finance::Order & N::Finance::Order::operator = (const Order & o)
{
  login             = o . login            ;
  order             = o . order            ;
  symbol            = o . symbol           ;
  digits            = o . digits           ;
  cmd               = o . cmd              ;
  volume            = o . volume           ;
  OpenTime          = o . OpenTime         ;
  state             = o . state            ;
  OpenPrice         = o . OpenPrice        ;
  sl                = o . sl               ;
  tp                = o . tp               ;
  vSL               = o . vSL              ;
  vTP               = o . vTP              ;
  bid               = o . bid              ;
  ask               = o . ask              ;
  CloseTime         = o . CloseTime        ;
  GwVolume          = o . GwVolume         ;
  Expiration        = o . Expiration       ;
  reason            = o . reason           ;
  conv_rates  [ 0 ] = o . conv_rates [ 0 ] ;
  conv_rates  [ 1 ] = o . conv_rates [ 1 ] ;
  commission        = o . commission       ;
  commission_agent  = o . commission_agent ;
  storage           = o . storage          ;
  ClosePrice        = o . ClosePrice       ;
  profit            = o . profit           ;
  taxes             = o . taxes            ;
  magic             = o . magic            ;
  comment           = o . comment          ;
  gw_order          = o . gw_order         ;
  activation        = o . activation       ;
  gw_open_price     = o . gw_open_price    ;
  gw_close_price    = o . gw_close_price   ;
  margin_rate       = o . margin_rate      ;
  timestamp         = o . timestamp        ;
  multiply          = o . multiply         ;
  return ME                                ;
}

#ifdef QT_QTMTAPI_LIB

N::Finance::Order & N::Finance::Order::operator = (TradeRecord & trade)
{
  login             = trade . login                                ;
  order             = trade . order                                ;
  symbol            = QString::fromUtf8 ( trade . symbol  )        ;
  digits            = trade . digits                               ;
  cmd               = trade . cmd                                  ;
  volume            = trade . volume                               ;
  state             = trade . state                                ;
  OpenPrice         = trade . open_price                           ;
  sl                = trade . sl                                   ;
  tp                = trade . tp                                   ;
  GwVolume          = trade . gw_volume                            ;
  reason            = trade . reason                               ;
  conv_rates  [ 0 ] = trade . conv_rates [ 0 ]                     ;
  conv_rates  [ 1 ] = trade . conv_rates [ 1 ]                     ;
  commission        = trade . commission                           ;
  commission_agent  = trade . commission_agent                     ;
  storage           = trade . storage                              ;
  ClosePrice        = trade . close_price                          ;
  profit            = trade . profit                               ;
  taxes             = trade . taxes                                ;
  magic             = trade . magic                                ;
  comment           = QString::fromUtf8 ( trade . comment )        ;
  gw_order          = trade . gw_order                             ;
  activation        = trade . activation                           ;
  gw_open_price     = trade . gw_open_price                        ;
  gw_close_price    = trade . gw_close_price                       ;
  margin_rate       = trade . margin_rate                          ;
  OpenTime          = QDateTime::fromTime_t ( trade . open_time  ) ;
  CloseTime         = QDateTime::fromTime_t ( trade . close_time ) ;
  Expiration        = QDateTime::fromTime_t ( trade . expiration ) ;
  timestamp         = QDateTime::fromTime_t ( trade . timestamp  ) ;
  multiply          = 1                                            ;
  for (int i=0;i<digits;i++) multiply *= 10                        ;
  return ME                                                        ;
}

#endif

double N::Finance::Order::LOT(void)
{
  double L = volume ;
  L /= 100          ;
  return L          ;
}

int N::Finance::Order::mOpenPrice(void)
{
  return (int) ( multiply * OpenPrice ) ;
}

int N::Finance::Order::mClosePrice(void)
{
  return (int) ( multiply * ClosePrice ) ;
}

int N::Finance::Order::mSL(void)
{
  return (int) ( multiply * sl ) ;
}

int N::Finance::Order::mTP(void)
{
  return (int) ( multiply * tp ) ;
}

int N::Finance::Order::mvSL(void)
{
  return (int) ( multiply * vSL ) ;
}

int N::Finance::Order::mvTP(void)
{
  return (int) ( multiply * vTP ) ;
}

bool N::Finance::Order::isSL(int spread)
{
  if ( spread < 0 ) return false   ;
  int dp = mClosePrice() - mSL ( ) ;
  if ( dp < 0 ) dp = -dp           ;
  return ( spread > dp )           ;
}

bool N::Finance::Order::isTP(int spread)
{
  if ( spread < 0 ) return false   ;
  int dp = mClosePrice() - mTP ( ) ;
  if ( dp < 0 ) dp = -dp           ;
  return ( spread > dp )           ;
}

QString N::Finance::Order::PriceString(double price)
{
  int     d = (int) ( price * multiply )       ;
  int     m = d % multiply                     ;
  int     p = d / multiply                     ;
  QString s                                    ;
  for (int i=0;i<digits;i++)                   {
    s . prepend ( QString::number ( m % 10 ) ) ;
    m /= 10                                    ;
  }                                            ;
  s . prepend ( "."                   )        ;
  s . prepend ( QString::number ( p ) )        ;
  return s                                     ;
}

QString N::Finance::Order::LotString(void)
{
  int     d = volume / 100                             ;
  int     m = volume % 100                             ;
  return QString("%1.%2%3").arg(d).arg(m/10).arg(m%10) ;
}
