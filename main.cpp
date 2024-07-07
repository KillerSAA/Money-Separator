// A port from PC to Android (https://github.com/The-Musaigen/money-separator) | thanks for RussJJ
#include <mod/amlmod.h>
#include <mod/config.h>
#include <mod/logger.h>
#include <string>

MYMODCFG(net.KillerSA.moneyseparator, Money Separator, 1.1, KillerSA)

static std::string AddSeparators(std::string aValue) 
{
    const char* sep = cfg->GetString("Separator", ".", "Configs");
    char aThousandSep = sep[0]; // if it says "word" the separator will be "w"

    int len = aValue.length();
    int value = (len > 0 && (aValue[0] == '-')) ? 2 : 1;
    int size = 3;

    while ((len - value) > size)
    {
        aValue.insert(len - size, 1, aThousandSep);

        size += 4;
        len += 1;
    }
    return aValue;
}

DECL_HOOKv(Money_AsciiToGxtChar, const char* aSource, unsigned short* aTarget)
{
    std::string source = std::string { aSource };
    std::string sep = AddSeparators(source);
    
    Money_AsciiToGxtChar(sep.c_str(), aTarget);
}

extern "C" void OnModLoad()
{
    logger->SetTag("Money Separator");
    cfg->Bind("Author", "", "About")->SetString("KillerSA"); cfg->ClearLast();
    cfg->Bind("GitHub", "", "About")->SetString("https://github.com/KillerSAA/Money-Separator/tree/main"); cfg->ClearLast();
    
    uintptr_t pGame = aml->GetLib("libGTASA.so");
    if(pGame)
    {
        HOOKBLX(Money_AsciiToGxtChar, pGame + BYBIT(0x2BD26E + 0x1, 0x37D4C4));
    }
    else
    {
        pGame = aml->GetLib("libGTAVC.so");
        if(pGame)
        {
            HOOKBL(Money_AsciiToGxtChar, pGame + BYBIT(0x1E9F74 + 0x1, 0x2C3AC8));
        }
        else
        {
            logger->Error("This game is unsupported");
            return;
        }
    }
}
