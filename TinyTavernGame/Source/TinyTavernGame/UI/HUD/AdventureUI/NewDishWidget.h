// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "NewDishWidget.generated.h"

class UTT_GameInstance;
class UCanvasPanel;
class UWidgetSwitcher;
class USizeBox;
class UTextBlock;
class UImage;
class URecipes_DataTable;
class UCommandRecipeRectangle;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNewDishWidget : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void SetDishData(int RecipeID);
	void SetPlayer(TObjectPtr<APlayerCharacter> _PlayerCharacter);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> NewDishCanvas;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DishImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DishImage_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DishName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DishName_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> NewDishSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> NewDishDiscoveredSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandRecipeRectangle> RecipeRectangle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandRecipeRectangle> RecipeRectangle_1;
	
	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> NewDishAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> NewDishDiscoveredAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> NewDishDiscoveredCloseAnimation;

	UPROPERTY(EditAnywhere, Category = "TT| Sound")
	TObjectPtr<USoundBase> NewDishDiscoveredSound;
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT")
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "TT")
	FDataTableRowHandle AcceptInputActionData;
	FUIActionBindingHandle AcceptHandle;
	
	UPROPERTY(EditAnywhere, Category = "TT")
	TObjectPtr<UDataTable> RecipesDataTable;

	//Animations 
	UPROPERTY(BlueprintReadOnly)
	bool InNewDishDiscoveredAnim = false;
	bool InNewDishDiscoveredCloseAnim = false;
	
	UFUNCTION(BlueprintCallable)
	void BackLogic();
private:

	UFUNCTION()
	void Dissapear();
	UFUNCTION()
	void OnNewDishDiscoveredFinishAnimation();
	
	FWidgetAnimationDynamicEvent NewDishAnimationEvent;
	FWidgetAnimationDynamicEvent NewDishDiscoveredAnimationEvent;
	FWidgetAnimationDynamicEvent NewDishDiscoveredCloseAnimationEvent;
	
	FGameplayTag EmemyTag;
	FGameplayTag CookTag;
	FGameplayTag CheeseTag;
	FGameplayTag JamTag;
	FGameplayTag WineTag;
	FGameplayTag CutTag;
	FGameplayTag NPCTag;
	
	bool HasCheese = false;
	bool HasJam = false;
	bool HasWine = false;

	FText CurrentOrderName;
	bool PlateDiscovered = false;
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	TObjectPtr<UTT_GameInstance> GameInstance;
};
