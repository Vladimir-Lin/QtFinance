#include <qtfinance.h>

typedef struct                            {
  int       type                          ;
  int       size                          ;
  int       symbols                       ;
  bool      filled                        ;
  char      symbol [ MAX_SYMBOLS ] [ 16 ] ;
  ConSymbol consym [ MAX_SYMBOLS        ] ;
} HiddenMetaTraderSymbols                 ;

#define HMTS sizeof(HiddenMetaTraderSymbols)
#define MTSC ( ( HiddenMetaTraderSymbols * ) SharingBlock )

N::SymbolsCenter:: SymbolsCenter (void)
                 : SharingBlock  (NULL)
{
}

N::SymbolsCenter::~SymbolsCenter (void)
{
}

bool N::SymbolsCenter::exists(QString name,DataManipulator & manipulator)
{
  if ( NULL != SharingBlock ) return true      ;
  QString k = QString("%1[Symbols]").arg(name) ;
  SharingBlock = manipulator . Memory ( k )    ;
  if ( NULL != SharingBlock ) return true      ;
  if ( manipulator . isEnabled ( "Create" ) )  {
    return false                               ;
  }                                            ;
  if ( manipulator . Open ( k , HMTS ) )       {
    SharingBlock = manipulator . Memory ( k )  ;
  }                                            ;
  return ( NULL != SharingBlock )              ;
}

bool N::SymbolsCenter::create(QString name,DataManipulator & manipulator)
{
  QString k = QString("%1[Symbols]").arg(name) ;
  if ( manipulator . Open   ( k , HMTS ) )     {
    SharingBlock = manipulator . Memory ( k )  ;
  }                                            ;
  if ( NULL != SharingBlock ) return true      ;
  //////////////////////////////////////////////
  if ( manipulator . Create ( k , HMTS ) )     {
    SharingBlock = manipulator . Memory ( k )  ;
  }                                            ;
  //////////////////////////////////////////////
  if ( NULL != SharingBlock )                  {
    ::memset ( SharingBlock , 0 , HMTS )       ;
    MTSC -> type = 1                           ;
    MTSC -> size = HMTS                        ;
  }                                            ;
  //////////////////////////////////////////////
  return ( NULL != SharingBlock )              ;
}

bool N::SymbolsCenter::setSymbols(QStringList symbols)
{
  if ( NULL == SharingBlock ) return false ;
  QString    s                             ;
  QByteArray b                             ;
  MTSC -> symbols = symbols . count ( )    ;
  for (int i=0;i<symbols.count();i++)      {
    s = symbols    [ i ]                   ;
    b = s . toUtf8 (   )                   ;
    ::strcpy ( MTSC -> symbol [ i ]        ,
               b . constData  (   )      ) ;
  }                                        ;
  return true                              ;
}

QStringList N::SymbolsCenter::Symbols(void)
{
  QStringList s                                     ;
  if ( NULL == SharingBlock ) return s              ;
  for (int i=0;i<MTSC->symbols;i++)                 {
    s << QString::fromUtf8 ( MTSC -> symbol [ i ] ) ;
  }                                                 ;
  return s                                          ;
}

void * N::SymbolsCenter::Details(void)
{
  if ( NULL == SharingBlock ) return NULL ;
  return MTSC -> consym                   ;
}

bool N::SymbolsCenter::assignMetaTrader(int total,void * CS)
{
  if ( NULL == SharingBlock ) return false                       ;
  if ( total <= 0           ) return false                       ;
  if ( NULL == CS           ) return false                       ;
  QStrings    s                                                  ;
  ConSymbol * cs = (ConSymbol *) CS                              ;
  for (int i=0;i<total;i++)                                      {
    s << QString::fromUtf8 (cs[i].symbol)                        ;
  }                                                              ;
  setSymbols ( s                                               ) ;
  ::memcpy   ( MTSC -> consym , cs , sizeof(ConSymbol) * total ) ;
  return true                                                    ;
}

bool N::SymbolsCenter::ObtainPIPs(WMAPs & v)
{
  if ( NULL == SharingBlock ) return false                        ;
  if ( MTSC -> symbols <= 0 ) return false                        ;
  QString s                                                       ;
  QString k                                                       ;
  for (int i=0;i<MTSC->symbols;i++)                               {
    s       = QString::fromUtf8 ( MTSC -> consym [ i ] . symbol ) ;
    k       = QString("PIPs[%1]").arg(s)                          ;
    v [ k ] = MTSC -> consym [ i ] . stops_level                  ;
  }                                                               ;
  return true                                                     ;
}
