// TinyTavern


#include "ResourceManager.h"

#include "Resource.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


void UResourceManager::NotifyCanObtainFryResource()
{
	for (AResource* Resource : FryResources)
	{
		Resource->OnState();
	}
}

void UResourceManager::NotifyCanObtainBoilResource()
{
	for (AResource* Resource : BoilResources)
	{
		Resource->OnState();
	}
}

void UResourceManager::Init()
{
	for(TTuple<FGameplayTag, bool> WeaponTag : Cast<UTT_GameInstance>(GetGameInstance())->UnlockedWeapons)
	{
		if(WeaponTag.Key == FGameplayTags::Get().Weapon_Range_Fry)
		{
			if(WeaponTag.Value)
			{
			  NotifyCanObtainFryResource();
			}
		}
		else if(WeaponTag.Key == FGameplayTags::Get().Weapon_Range_Boil)
		{
			if(WeaponTag.Value)
			{
				NotifyCanObtainBoilResource();
			}
		}
	}
	
}


