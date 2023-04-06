#include "DES_JournalStruct.h"

FDES_JournalStruct::FDES_JournalStruct()
{
	Entries = TMap<FString, FDES_JournalEntryStruct>();

	Entries.Reserve(0);
}

FDES_JournalStruct::~FDES_JournalStruct()
{

}
