// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "UpgradeUI.h"
#include "WeaponUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UWeaponUpgrade : public UUpgradeUI
{
	GENERATED_BODY()

public:
	virtual void ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable) override;
	virtual bool ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance) override;

private:
	void UpgradeWeapons(TObjectPtr<UTT_GameInstance> TTGameInstance, int Number);
};