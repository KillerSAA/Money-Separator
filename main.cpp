#include "main.h"
// a port from pc to android (https://github.com/The-Musaigen/money-separator) 

using namespace std;

MYMOD(net.KillerSA.MoneySeparator, Money separator, 1.0, KillerSA)

void (*AsciiToGxt)(const char* txt, unsigned short* ret);

extern "C" void AsciiToGxtChar(const char* aSource, unsigned short* aTarget)
{
	std::string source = std::string{ aSource };
	std::string sep = AddSeparators(source);
    aSource = sep.c_str();
    
    AsciiToGxt(aSource, aTarget);
	return;
}

uintptr_t ret;
__attribute__((optnone)) __attribute__((naked)) void Money_Inject(void){
    asm volatile(
        "MOV R0, R10\n"
        "BL AsciiToGxtChar\n"
        "LDRB R0, [R6]\n");
    asm volatile("MOV PC, %0" :: "r"(ret));
}

extern "C" void OnModLoad(){
    logger->SetTag("Money Separator");
    
    void* hGTASA = aml->GetLibHandle("libGTASA.so");
    uintptr_t pGTASA = aml->GetLib("libGTASA.so");

    SET_TO(AsciiToGxt, aml->GetSym(hGTASA, "_Z14AsciiToGxtCharPKcPt"));
    ret = pGTASA + 0x2BD274 + 0x1;

    aml->Redirect(pGTASA + 0x2BD26C, (uintptr_t)Money_Inject);
}
