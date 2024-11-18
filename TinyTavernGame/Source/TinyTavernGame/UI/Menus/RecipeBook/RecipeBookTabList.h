// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "RecipeBookTabList.generated.h"

class USizeBox;
class UCommonButtonStyle;
class UHorizontalBox;
class UCommonActionWidget;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URecipeBookTabList : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> PreviousTabInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> NextTabInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> PreviousTabWrapper;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> NextTabWrapper;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> TabButtonBox;

	//Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipe Book Tab List")
	FMargin TabButtonMargin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipe Book Tab List")
	TSubclassOf<UCommonButtonStyle> TabButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipe Book Tab List")
	int MinTabWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipe Book Tab List")
	bool UseButtonStyle = true;
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	virtual void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

private:
	UFUNCTION()
	void TabSelectedEvent(FName TabID);
	UFUNCTION()
	void TabVisibilityChangedEvent(ESlateVisibility InVisibility);
	void RefreshNextPrevVisibility();
	void UpdateTabStyles(UCommonButtonBase* TabButton);
};
