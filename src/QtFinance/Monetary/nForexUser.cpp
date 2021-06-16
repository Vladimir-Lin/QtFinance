#include <qtfinance.h>

N::Finance::User:: User(void)
{
  login                  = -1       ;
  group                  = ""       ;
  password               = ""       ;
  online                 = false    ;
  IP                     = ""       ;
  counter                =  0       ;
  enable                 =  0       ;
  enable_change_password =  0       ;
  enable_read_only       =  0       ;
  password_investor      = ""       ;
  password_phone         = ""       ;
  name                   = ""       ;
  country                = ""       ;
  city                   = ""       ;
  state                  = ""       ;
  zipcode                = ""       ;
  address                = ""       ;
  phone                  = ""       ;
  email                  = ""       ;
  comment                = ""       ;
  id                     = ""       ;
  status                 = ""       ;
  regdate                = nTimeNow ;
  lastdate               = nTimeNow ;
  leverage               = -1       ;
  agent_account          = -1       ;
  timestamp              = nTimeNow ;
  last_ip                = -1       ;
  volume                 = -1       ;
  balance                =  0       ;
  prevmonthbalance       =  0       ;
  prevbalance            =  0       ;
  credit                 =  0       ;
  interestrate           =  0       ;
  taxes                  =  0       ;
  prevmonthequity        =  0       ;
  prevequity             =  0       ;
  reserved2[0]           =  0       ;
  reserved2[1]           =  0       ;
  send_reports           = -1       ;
  mqid                   =  0       ;
  equity                 =  0       ;
  margin                 =  0       ;
  margin_free            =  0       ;
  margin_level           =  0       ;
  margin_type            = -1       ;
  level_type             = -1       ;
}

N::Finance::User:: User (const User & user)
{
  ME = user ;
}

N::Finance::User::~User(void)
{
}

N::Finance::User & N::Finance::User::operator = (const User & user)
{
  login                  = user . login                  ;
  group                  = user . group                  ;
  password               = user . password               ;
  online                 = user . online                 ;
  IP                     = user . IP                     ;
  counter                = user . counter                ;
  enable                 = user . enable                 ;
  enable_change_password = user . enable_change_password ;
  enable_read_only       = user . enable_read_only       ;
  password_investor      = user . password_investor      ;
  password_phone         = user . password_phone         ;
  name                   = user . name                   ;
  country                = user . country                ;
  city                   = user . city                   ;
  state                  = user . state                  ;
  zipcode                = user . zipcode                ;
  address                = user . address                ;
  phone                  = user . phone                  ;
  email                  = user . email                  ;
  comment                = user . comment                ;
  id                     = user . id                     ;
  status                 = user . status                 ;
  regdate                = user . regdate                ;
  lastdate               = user . lastdate               ;
  leverage               = user . leverage               ;
  agent_account          = user . agent_account          ;
  timestamp              = user . timestamp              ;
  last_ip                = user . last_ip                ;
  balance                = user . balance                ;
  prevmonthbalance       = user . prevmonthbalance       ;
  prevbalance            = user . prevbalance            ;
  credit                 = user . credit                 ;
  interestrate           = user . interestrate           ;
  taxes                  = user . taxes                  ;
  prevmonthequity        = user . prevmonthequity        ;
  prevequity             = user . prevequity             ;
  reserved2[0]           = user . reserved2[0]           ;
  reserved2[1]           = user . reserved2[1]           ;
  publickey              = user . publickey              ;
  send_reports           = user . send_reports           ;
  mqid                   = user . mqid                   ;
  equity                 = user . equity                 ;
  margin                 = user . margin                 ;
  margin_free            = user . margin_free            ;
  margin_level           = user . margin_level           ;
  margin_type            = user . margin_type            ;
  level_type             = user . level_type             ;
  return ME                                              ;
}

#ifdef QT_MTMANAPI_H

N::Finance::User & N::Finance::User::operator = (UserRecord & user)
{
  login                  = user . login                                      ;
  group                  = QString::fromUtf8(user . group            )       ;
  password               = QString::fromUtf8(user . password         )       ;
  enable                 = user . enable                                     ;
  enable_change_password = user . enable_change_password                     ;
  enable_read_only       = user . enable_read_only                           ;
  password_investor      = QString::fromUtf8(user . password_investor)       ;
  password_phone         = QString::fromUtf8(user . password_phone   )       ;
  name                   = QString::fromUtf8(user . name             )       ;
  country                = QString::fromUtf8(user . country          )       ;
  city                   = QString::fromUtf8(user . city             )       ;
  state                  = QString::fromUtf8(user . state            )       ;
  zipcode                = QString::fromUtf8(user . zipcode          )       ;
  address                = QString::fromUtf8(user . address          )       ;
  phone                  = QString::fromUtf8(user . phone            )       ;
  email                  = QString::fromUtf8(user . email            )       ;
  comment                = QString::fromUtf8(user . comment          )       ;
  id                     = QString::fromUtf8(user . id               )       ;
  status                 = QString::fromUtf8(user . status           )       ;
  regdate                = QDateTime::fromTime_t ( user . regdate    )       ;
  lastdate               = QDateTime::fromTime_t ( user . lastdate   )       ;
  leverage               = user . leverage                                   ;
  agent_account          = user . agent_account                              ;
  timestamp              = QDateTime::fromTime_t ( user . timestamp  )       ;
  last_ip                = user . last_ip                                    ;
  balance                = user . balance                                    ;
  prevmonthbalance       = user . prevmonthbalance                           ;
  prevbalance            = user . prevbalance                                ;
  credit                 = user . credit                                     ;
  interestrate           = user . interestrate                               ;
  taxes                  = user . taxes                                      ;
  prevmonthequity        = user . prevmonthequity                            ;
  prevequity             = user . prevequity                                 ;
  reserved2[0]           = user . reserved2[0]                               ;
  reserved2[1]           = user . reserved2[1]                               ;
  publickey              = QByteArray ( user . publickey , PUBLIC_KEY_SIZE ) ;
  send_reports           = user . send_reports                               ;
  mqid                   = user . mqid                                       ;
  return ME                                                                  ;
}

N::Finance::User & N::Finance::User::operator = (MarginLevel & Margin)
{
  volume       = Margin . volume       ;
  equity       = Margin . equity       ;
  margin       = Margin . margin       ;
  margin_free  = Margin . margin_free  ;
  margin_level = Margin . margin_level ;
  margin_type  = Margin . margin_type  ;
  level_type   = Margin . level_type   ;
  return ME                            ;
}

N::Finance::User & N::Finance::User::operator = (OnlineRecord & Online)
{
  unsigned char * ip                   ;
  ip = (unsigned char *) & Online . ip ;
  online  = true                       ;
  counter = Online . counter           ;
  IP      = QString ( "%1.%2.%3.%4"    )
            . arg   ( ip [ 0 ]         )
            . arg   ( ip [ 1 ]         )
            . arg   ( ip [ 2 ]         )
            . arg   ( ip [ 3 ]       ) ;
  return ME                            ;
}

#endif

bool N::Finance::User::enabled(void)
{
  return ( 0 != enable ) ;
}

double N::Finance::User::lots(void)
{
  double v    ;
  v  = volume ;
  v /= 100.0  ;
  return v    ;
}
