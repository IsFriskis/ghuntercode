// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "QuestsOverlay.generated.h"

struct FGameplayTag;
class UCommonActionWidget;
class USizeBox;
class UImage;
class UTT_GameInstance;
class URecipeTicket;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQuestsOverlay : public UTTUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	
	void OpenQuestsMenu();
	void CloseQuestsMenu();
	void HoverNextQuest();
	void HoverLastQuest();
	void SelectQuest();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> MainVerticalBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> MainDishesVBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> AcceptedDishesVBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> MainDishesBanner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> AcceptedDishesBanner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> OpenTicketsAction;
	
	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenMainQuestsAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CloseMainQuestsAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenAcceptedQuestsAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CloseAcceptedQuestsAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenMenuAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CloseMenuAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Recipes")
	TSubclassOf<UUserWidget> RecipeWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	bool ShouldHide = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TT|Recipes")
	FDataTableRowHandle LeftArrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TT|Recipes")
	FDataTableRowHandle RightArrowAction;
	
	UFUNCTION()
	void AddRecipesToMenu();
	UFUNCTION()
	void OnRecipeCompleted(int ID, FGameplayTag NPCTag,bool PremiumCompleted);
	UFUNCTION()
	void HideRecipesInMenu();
	UFUNCTION()
	void OnMenuCloseAnimationFinished();
	UFUNCTION()
	void OnMenuOpenAnimationFinished();
	
private:
	TArray<TObjectPtr<URecipeTicket>> MainOrders;
	TArray<TObjectPtr<URecipeTicket>> AcceptedOrders;
	FWidgetAnimationDynamicEvent CloseMenuAnimationEvent;
	FWidgetAnimationDynamicEvent OpenMenuAnimationEvent;
	bool IsOpen = false;
	bool IsMainOrdersHovered = true;
	bool IsAnyOrderSelected = false;
	TObjectPtr<URecipeTicket> SelectedOrder;
	int HoveredQuest = 0;
	TObjectPtr<UTT_GameInstance> GameInstance;
	bool OnAnimation = false;

	bool CheckPremium(int PlateID, int RecipePlateID);
};
