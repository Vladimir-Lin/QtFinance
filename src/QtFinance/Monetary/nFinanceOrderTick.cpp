#include <qtfinance.h>

N::Finance::OrderTick:: OrderTick   ( QObject * parent )
                      : QObject     (           parent )
                      , QScriptable (                  )
                      , Order       (                  )
{
}

N::Finance::OrderTick:: OrderTick   ( QObject * parent , const Order & o )
                      : QObject     (           parent                   )
                      , QScriptable (                                    )
                      , Order       (                                  o )
{
}

N::Finance::OrderTick::~OrderTick (void)
{
}

int N::Finance::OrderTick::Login(void)
{
  return login ;
}

int N::Finance::OrderTick::OrderId(void)
{
  return order ;
}

QString N::Finance::OrderTick::Symbol(void)
{
  return symbol ;
}

int N::Finance::OrderTick::Digits(void)
{
  return digits ;
}

int N::Finance::OrderTick::Cmd(void)
{
  return cmd ;
}

double N::Finance::OrderTick::Lots(void)
{
  double lots = volume ;
  lots /= 100          ;
  return lots          ;
}

QDateTime N::Finance::OrderTick::PlaceTime(void)
{
  return OpenTime ;
}

QDateTime N::Finance::OrderTick::Now(void)
{
  return nTimeNow ;
}

double N::Finance::OrderTick::Open(void)
{
  return OpenPrice ;
}

double N::Finance::OrderTick::SL(void)
{
  return sl ;
}

double N::Finance::OrderTick::TP(void)
{
  return tp ;
}

double N::Finance::OrderTick::Bid(void)
{
  return bid ;
}

double N::Finance::OrderTick::Ask(void)
{
  return ask ;
}

double N::Finance::OrderTick::Commission(void)
{
  return commission ;
}

double N::Finance::OrderTick::Storage(void)
{
  return storage ;
}

double N::Finance::OrderTick::Profit(void)
{
  return profit ;
}

double N::Finance::OrderTick::Taxes(void)
{
  return taxes ;
}

int N::Finance::OrderTick::Magic(void)
{
  return magic ;
}

QString N::Finance::OrderTick::Comment(void)
{
  return comment ;
}

double N::Finance::OrderTick::MarginRate(void)
{
  return margin_rate ;
}
