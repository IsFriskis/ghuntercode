// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "UpgradeUI.generated.h"

class USpinningWidget;
struct FGameplayTag;
class UImage;
class UTT_GameInstance;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UUpgradeUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable){}
	virtual bool ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance){ return false;}
	TObjectPtr<UProgressBar> GetSatistactionBar(){return NPCSatisfactionBar;}
	TObjectPtr<UProgressBar> GetFutureSatistactionBar(){return NPCFutureSatisfactionBar;}
	void UpdateSatisfactionBar(int Level);
	float GetPercentageToFill(int NumberOfStars);
	void AddPercentNormalSatisfactionbar(float NumberOfStars);
	void AddPercentFutureSatisfactionbar(float NumberOfStars);
	void ShowLight();
	void HideLight();
	bool DoesPlayerHaveRelic(){return PlayerHasRelic;}
	bool PlayerCompletedAllTiers(){return CompletedAllTiers;}
	
protected:
	//Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> UpgradeBackground;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RelicImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> UpgradeImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> NPCSatisfactionBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> NPCFutureSatisfactionBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USpinningWidget> SpinningLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush UpgradeNormalBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush UpgradeAvailableBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	float Upgrade1ValueToAdd = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	float Upgrade2ValueToAdd = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	float Upgrade3ValueToAdd = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	float Upgrade4ValueToAdd = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	TObjectPtr<UMaterialInstance> SatisfactionBarMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	bool HasLevel4 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	bool FoodUpgrade = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	TArray<float> SatisfactionBarPercentagesLevel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	TArray<float> SatisfactionBarPercentagesLevel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	TArray<float> SatisfactionBarPercentagesLevel3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	TArray<float> SatisfactionBarPercentagesLevel4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush SatisfactionBarImageLevel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush SatisfactionBarImageLevel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush SatisfactionBarImageLevel3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrade")
	FSlateBrush SatisfactionBarImageLevel4;

	bool PlayerHasRelic = false;
	bool CompletedAllTiers = false;
	virtual void NativeConstruct() override;
	void ResetNPCOrdersAndQuestBoardOrder(FGameplayTag NPCTag, TObjectPtr<UTT_GameInstance> TTGameInstance);
	void CheckPlayerRelics(int NumberOfRelics, int NumberOfUpgrade);
	
private:
	TArray<float> CurrentSatisfactionBarPercentages;
	
};
