// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "../TT_CommonActivatableWidget.h"
#include "RecipeBook.generated.h"

class UGuncleNotes;
class UOptions;
class UBestiary;
class UBookMap;
class UCanvasPanel;
class UInventory;
class UImage;
class URecipeBookTabList;
class UCommonActivatableWidgetSwitcher;
class UButtonBase;
class AUIPlayer;
class ATTPlayerController;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URecipeBook : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void RecipeBookInTavern();
	
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> RecipeBookSwitcher;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventory> Inventory;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBookMap> Map;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBestiary> Bestiary;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UGuncleNotes> GuncleNotes;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOptions> Options;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> LBImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RBImage;
	

	//For Backing with Gamepad
	UPROPERTY(EditAnywhere, Category = "TT| Recipe Book Info| Sound")
	TObjectPtr<USoundBase> NextPageSound;
	UPROPERTY(EditAnywhere, Category = "TT| Recipe Book Info| Sound")
	TObjectPtr<USoundBase> PreviousPageSound;
	UPROPERTY(EditDefaultsOnly, Category = "TT| Recipe Book Info")
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	//For Switching Book index with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT| Recipe Book Info")
	FDataTableRowHandle PreviousInputActionData;
	FUIActionBindingHandle PreviousInputHandle;

	//For Switching Book index with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT| Recipe Book Info")
	FDataTableRowHandle NextInputActionData;
	FUIActionBindingHandle NextInputHandle;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateIndexButtons(int CurrentWidgetIndex);
	
	virtual void NativeConstruct() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	//Reference to UI Player, like in Tavern
	TObjectPtr<AUIPlayer> UIPlayer;
	//Reference to player
	TObjectPtr<ATTPlayerController> Player;
	TObjectPtr<ULocalPlayer> LocalPlayer;
	//Controls on which widget of the secondary tab list is
	int CurrentWidgetIndexOfSubsectionSwitcher = 0;
	
	UFUNCTION()
	void BackLogic();
	UFUNCTION()
	void PreviousIndex();
	UFUNCTION()
	void NextIndex();
	void ClosePauseMenu();

	int CurrentIndex = 0;
};
