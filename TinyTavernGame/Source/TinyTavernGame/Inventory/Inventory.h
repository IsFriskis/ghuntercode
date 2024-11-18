// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "Inventory.generated.h"

class UCommonTextBlock;
class UCommonAnimatedSwitcher;
class UButtonBase;
struct FStatuePlates;
class UImage;
class UCommandUI;
class UGridPanel;
struct FRecipes_Struct;
struct FCommandImage;
class UWrapBox;
class UTT_GameInstance;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UInventory : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	//Method called in statues so that the inventory slot buttons behave as buttons
	void SetInventoryEnabled();
	TArray<TObjectPtr<UInventorySlot>> GetInventorySlots();
	void ShowSecondaryCommands();
	void HideNoSecondaryCommands();
	
protected:
	//Book Image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RecipeBookImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> NoCommandsTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> EmptySecondaryCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> NoExtraPlatesTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> EmptyExtraPlateCommand;
	//Left Page
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand6;
	
	//Secondary Commands, there are 3 of each to obtain a better look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> SecondaryCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> SecondaryCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> SecondaryCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> SecondaryCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> SecondaryCommand5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> LeftSecondaryCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> LeftSecondaryCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> LeftSecondaryCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> LeftSecondaryCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> LeftSecondaryCommand5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> RightSecondaryCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> RightSecondaryCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> RightSecondaryCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> RightSecondaryCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> RightSecondaryCommand5;

	//Extra Plates, there are 3 of each to obtain a better look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> ExtraPlateCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> ExtraPlateCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> ExtraPlateCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> ExtraPlateCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> ExtraPlateCommand5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> LeftExtraPlateCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> LeftExtraPlateCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> LeftExtraPlateCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> LeftExtraPlateCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> LeftExtraPlateCommand5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> RightExtraPlateCommand1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> RightExtraPlateCommand2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> RightExtraPlateCommand3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> RightExtraPlateCommand4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventorySlot> RightExtraPlateCommand5;

	//Buttons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> LeftSecondaryStatueButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> RightSecondaryStatueButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> LeftExtraPlateButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> RightExtraPlateButton;

	//Switcher
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> SecondaryStatueSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> LeftSecondaryStatueSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> RightSecondaryStatueSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> ExtraPlatesSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> LeftExtraPlatesSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> RightExtraPlatesSwitcher;
	
	//Inventory Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Inventory Info")
	TSubclassOf<UInventorySlot> InventorySlotWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Inventory Info")
	TSubclassOf<UCommandUI> CommandWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Inventory Info")
	bool IsInQuestBoard = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Inventory Info| Sounds")
	TObjectPtr<USoundBase> NextCommandSound;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TArray<FCommandImage*> ImagesOfRecipe;
	TArray<FRecipes_Struct*> Recipes;
	TArray<FStatuePlates* > StatuePlates;
	TArray<TObjectPtr<UCommandUI>> MainCommands;
	TArray<TObjectPtr<UCommandUI>> SecondaryCommands;
	TArray<TObjectPtr<UCommandUI>> LeftSecondaryCommands;
	TArray<TObjectPtr<UCommandUI>> RightSecondaryCommands;
	TArray<TObjectPtr<UInventorySlot>> CurrentStatuePlates;
	TArray<TObjectPtr<UInventorySlot>> ExtraPlates;
	TArray<TObjectPtr<UInventorySlot>> LeftExtraPlates;
	TArray<TObjectPtr<UInventorySlot>> RightExtraPlates;
	int NumberOfVisibleExtraPlates = 0;
	void InitializeExtraPlates();
	void InitializeSecondaryPlates();
	FSlateBrush GetRecipeImageFromTag(FGameplayTag ImageTag);
	FStatuePlates* GetStatuePlateInfo(int StatuePlateID);
	void CreateStatuePlatesInventory();
	void CreateExtraPlatesInventory();
	void ShowMainCommands();
	void SearchFirstVisibleSecondaryStatueCommand();
	void SearchVisibleExtraPlates();
	UFUNCTION()
	void MoveSecondaryStatueToTheLeft();
	UFUNCTION()
	void MoveSecondaryStatueToTheRight();
	UFUNCTION()
	void MoveExtraPlatesToTheLeft();
	UFUNCTION()
	void MoveExtraPlatesToTheRight();
	//Plays a sound when changing a command from a switcher
	void MoveSwitcherWidgetIndex(int Index, TObjectPtr<UCommonAnimatedSwitcher> Switcher, int LastVisibleWidgetIndex);
	//Plays a sound when changing a command from a switcher
	void MoveSwitcherSecondaryStatueWidgetIndex(int Index, TObjectPtr<UCommonAnimatedSwitcher> Switcher, bool MovingRight);
	void CleanSecondaryStatueSwitcher();
};
