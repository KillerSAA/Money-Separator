#include <string>
#include <stdio.h>
#include <mod/amlmod.h>
#include <mod/logger.h>

uintptr_t pGTASA;
void* hGTASA = NULL;

static std::string AddSeparators(std::string aValue, char aThousandSep = '.') 
{
int len = aValue.length();
int value = ((len) && (aValue[0] == '-')) ? 2 : 1;
int size = 3;

while ((len - value) > size)
{
aValue.insert(len - size, 1, aThousandSep);

size += 4;
len += 1;
}

return aValue;
}
