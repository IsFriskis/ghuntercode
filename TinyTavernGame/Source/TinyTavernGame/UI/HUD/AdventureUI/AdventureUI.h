// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "AdventureUI.generated.h"

class UCommonTextBlock;
class UOverlayWidgetController;
class UImage;
struct FGameplayTagContainer;
class UQuestsOverlay;
class UArmoryOverlay;
class UHealthOverlay;
class UTT_GameInstance;
class UVerticalBox;
class URecipeTicket;
class UCommonActivatableWidget;
class URecipeBook;
class UCommonActivatableWidgetStack;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAdventureUI : public UTTUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHealthOverlay> HealthOverlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UArmoryOverlay> ArmoryOverlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQuestsOverlay> QuestsOverlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> PoisonImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GavernUpgradeImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> T_InventoryText;

	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LowHealthAnimation;
	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BeginPlayAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InventoryAnimation;
	
	//Menus that appear, like Settings or Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> AdventureUIStack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Menus")
	TSubclassOf<URecipeBook> PauseMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Recipes")
	TSubclassOf<UUserWidget> RecipeWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gavern Upgrades")
	FSlateBrush ShieldGavernPotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gavern Upgrades")
    FSlateBrush ExtraReputationGavernPotion;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gavern Upgrades")
	FSlateBrush PuffballResistanceGavernPotion;

	virtual void WidgetControllerSet_Implementation() override;

private:
	TObjectPtr<UOverlayWidgetController> OverlayWidget;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TArray<TObjectPtr<URecipeTicket>> Recipes;
	bool ShouldHide = false;
	bool IsBeginPlay = true;
	int CurrentHealth = 0;
	float MaxPoisonResistance;
	bool QuestOverlayAlreadyOpen = false;
	
	UFUNCTION()
	void OnHealthChange(float newHealth);
	UFUNCTION()
	void OpenCloseQuestsMenu(bool Open);
	UFUNCTION()
	void SelectNextOrLastQuest(bool Next);
	UFUNCTION()
	void SelectRecipe();
	UFUNCTION()
	void OnPoisonResistanceChange(float newPoisonResistance);
	UFUNCTION()
	void OnMaxPoisonResistanceChange(float newMaxPoisonResistance);
	//Function to broadcast when the HUD shoul Hide or be Visible;
	UFUNCTION()
	void HudVisibilityChange(bool _ShoulHide);
	UFUNCTION()
	void InventoryFull(bool IsInventoryFull);
	//If it is Gavern day show it in hud
	void CheckGavernDay();
};
