// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "UpgradeUI.h"
#include "AmmoUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAmmoUpgrade : public UUpgradeUI
{
	GENERATED_BODY()

public:
	virtual void ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable) override;
	virtual bool ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance) override;

private:
	void UpgradeAmmo(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue);
	void UpgradeIngredient(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue);
};
