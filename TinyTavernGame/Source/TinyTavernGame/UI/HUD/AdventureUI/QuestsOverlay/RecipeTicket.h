// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "RecipeTicket.generated.h"

class UCommonActionWidget;
class UCommandRecipeRectangle;
class UTextBlock;
struct FLinearColor;
class UHorizontalBox;
class UTT_GameInstance;
class UImage;
class UButton;
class USizeBox;
class UDataTable;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URecipeTicket : public UTTUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void SetRecipe(int ID, bool IsMain, FGameplayTag _NPCTag);
	void MarkAsHovered();
	void MarkAsUnhovered(bool ShouldPlayAnimation);
	int GetID() const;
	FGameplayTag GetNPCTag(){return NPCTag;}
	void MarkAsCompleted();
	void MakeOrderVisible();
	bool IsSelected = false;
	void SetUpDownAction(bool Show);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Ticket;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Check;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandRecipeRectangle> RecipeRectangle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> UpDownTicketsAction;

	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnHoveredAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnUnHoveredAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnHoveredAnim1Ing;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnUnHoveredAnim1Ing;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnHoveredAnim2Ing;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnUnHoveredAnim2Ing;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnHoveredAnim3Ing;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnUnHoveredAnim3Ing;

	//Widget Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TT|WidgetSettings")
	TObjectPtr<UDataTable> RecipesImagesData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TT|WidgetSettings")
	TObjectPtr<UDataTable> RecipesData;

private:
	int m_ID;
	int NumberOfIngredients = 0;
	
	FGameplayTag EmemyTag;
	FGameplayTag CookTag;
	FGameplayTag CheeseTag;
	FGameplayTag JamTag;
	FGameplayTag WineTag;
	FGameplayTag CutTag;
	FGameplayTag NPCTag;

	
	FSlateBrush NormalBrush;
	FSlateBrush HoveredBrush;
	FSlateBrush PressedBrush;
	
	bool HasCheese = false;
	bool HasJam = false;
	bool HasWine = false;
};
