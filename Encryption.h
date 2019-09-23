#include "crypt2.h"
#include "C:\\90\\cassdog.h"
#include "C:\\90\\random.h"
#include "Global.h"
#include "StringsOpe.h"
#include "FilesOpe.h"

//º”√‹
extern unsigned short g_nDogValue;
extern bool g_bHasDog;
extern bool g_bReduceDogTimes;

#ifdef _WIN64
#else
extern bool RSA_Verify_Dog();
#endif

#define RND_ARR_LEN 32
extern unsigned char * mpRandomCharArray;

void ReleaseCMStick();

extern bool CheckLIC();

int CalculateSignature( unsigned char *acData, int cbData );

bool Cass_Register(TCHAR *szCode,unsigned short nType);

bool InitGsmhDog();

bool Reduce_DogTimes();

bool seekdog();