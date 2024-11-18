// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "StatueUI.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;
class UHorizontalBox;
class UCommonRichTextBlock;
struct FStatuePlates;
class UButtonBase;
class UTT_GameInstance;
struct FGameplayTag;
class UInventorySlot;
class UInventory;
class AStatue;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UStatueUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetStatue(int _StatueID, TObjectPtr<AStatue> _Statue, FText _StatueRiddle, bool _IsPlateStatue = true, FGameplayTag _WeaponToBeUnlocked = FGameplayTags::Get().Weapon_Range_Boil);
	UFUNCTION(BlueprintImplementableEvent)
	FText BP_FormatTextForEnter();
	UFUNCTION(BlueprintImplementableEvent)
	FText BP_FormatRiddleForEnter();

protected:
	//Widgets
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> StatueDialogBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> StatueTitleImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatueTitle;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> OfferButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> StatueCanvasPanel;

	//Statue UI Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	FText NoInventoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	FText NoAValidPlateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	TObjectPtr<USoundBase> NoInventorySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	TObjectPtr<USoundBase> NotAValidPlateSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	TObjectPtr<USoundBase> EnteringStatueSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	FText GaliasWords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue UI Info")
	FText GaliasUpgradeWords;
	
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle AcceptInputActionData;
	FUIActionBindingHandle AcceptHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatueRiddle;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TArray<FStatuePlates*> StatuesPlates;
	TObjectPtr<AStatue> Statue;
	int StatueID;
	int StatuePlateID;
	bool IsPlateStatue = true;
	bool IsOnGaliasWords = true;
	FGameplayTag WeaponToBeUnlocked;

	UFUNCTION()
	void AfterGaliasWords();
	UFUNCTION()
	void OfferPlate();
	UFUNCTION()
	void ExitStatue();
	bool CheckIfWeaponCanBeUnlocked();
};
