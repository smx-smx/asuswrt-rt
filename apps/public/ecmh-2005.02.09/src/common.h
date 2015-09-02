/*****************************************************
 ecmh - Easy Cast du Multi Hub - Common Functions
******************************************************
 $Author: josephxu $
 $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/ecmh-2005.02.09/src/common.h#1 $
 $Date: 2010/10/01 $
*****************************************************/

void dolog(int level, const char *fmt, ...);
int huprunning(void);
void savepid(void);
void cleanpid(int i);
