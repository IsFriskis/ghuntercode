// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "UpgradeUI.h"
#include "HealthUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UHealthUpgrade : public UUpgradeUI
{
	GENERATED_BODY()

public:
	virtual void ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable) override;
	virtual bool ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance) override;

private:
	void UpgradeHealth(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue);
	void CheckAmountOfRelics();
};
