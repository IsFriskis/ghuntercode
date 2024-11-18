// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "InventorySlot.generated.h"

class UCommonRichTextBlock;
struct FGameplayTag;
class UOverlaySlot;
class UProgressBar;
class USizeBox;
class UCommandRecipeRectangle;

USTRUCT(BlueprintType)
struct FStatuePlates : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StatueID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlateID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PlateTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PlateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PlateImportantText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonButtonStyle> StatuePlateStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonTextStyle> StatuePlateTextStyle;
};

class UOverlay;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotClicked, UInventorySlot*, InventorySlot);

class UTT_GameInstance;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UInventorySlot : public UButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "TT InventorySlot Clicked")
	FOnInventorySlotClicked OnInventorySlotClicked;

	int GetPlateID(){return PlateID;}
	void SetInventorySlot(int _PlateID, UTexture2D* Plate, bool Ingredient1, bool Ingredient2,
							bool Ingredient3, FGameplayTag EnemyTag, FGameplayTag CookTag,
							FGameplayTag CutTag, int NumberOfStars, FText _CommandName,TObjectPtr<UTT_GameInstance> _TTGameInstance, bool IsInGavernDay = false);
	void EnableInventorySlot();
	void MakeStatuePlate(FText PlateTitle, FText _PlateText, TSubclassOf<UCommonButtonStyle> StatuePlateStyle, TSubclassOf<UCommonTextStyle> StatuePlateTextStyle);
	void StatuePlatePickedUp(TSubclassOf<UCommonButtonStyle> StatuePlateStyle, TSubclassOf<UCommonTextStyle> StatuePlateTextStyle,FText PlateTitle);
	UFUNCTION(BlueprintImplementableEvent)
	FText BP_FormatTextForEnter();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PlateText;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> StatuePlateTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> StatuePlateTitle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommandName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NPCStamp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> PlateImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> MossImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandRecipeRectangle> CommandRecipeTriangle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CommandCompletedImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CompletedText;
	//Stars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> StarSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> StarBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ExtraStar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<TSubclassOf<UCommonButtonStyle>> ScrollStyles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<float> DistanceToSubtractDependingOnAmountOfStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	TArray<float> PercentageToRemoveDependingOnStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Sprites")
	FVector2D StatuePlateScale = FVector2D(0.8f, 0.8f);
	
	virtual void NativeConstruct() override;
	virtual void NativeOnClicked() override;
	
private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	int PlateID;
	TObjectPtr<UOverlaySlot> StarBoxOverlaySlot;
	//Show Stars
	void SetUpStars(int Amount, bool IsGavernDay = false);
	
};
