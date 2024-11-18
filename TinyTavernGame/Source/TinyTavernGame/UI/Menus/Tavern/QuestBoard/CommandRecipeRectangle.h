// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "CommandRecipeRectangle.generated.h"

class UWrapBox;
class UWrapBoxSlot;

USTRUCT(BlueprintType)
struct FNPCTriangleCommandInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush GamusinoImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush CucaferaImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush BasiliscoImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SlicedImageUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SlicedImageDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SmashedImageUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SmashedImageDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SkeweredImageUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush SkeweredImageDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush GrillImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush BoilImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FSlateBrush FryImage;
};

class UOverlay;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCommandRecipeRectangle : public UTTUserWidget
{
	GENERATED_BODY()

public:
	//Use SetUpTriangle First
	UFUNCTION(BlueprintCallable)
	void SetUpTriangles(bool Ingredient1, bool Ingredient2, bool Ingredient3);
	UFUNCTION(BlueprintCallable)
	void SetEnemyTag(FGameplayTag _EnemyTag){EnemyTag = _EnemyTag;}
	UFUNCTION(BlueprintCallable)
	void SetCutStyle(FGameplayTag _WeaponStyle){WeaponStyle = _WeaponStyle;}
	UFUNCTION(BlueprintCallable)
	void SetCookStyle(FGameplayTag _CookStyle){CookStyle = _CookStyle;}
	UFUNCTION(BlueprintCallable)
	void SetNPC(FGameplayTag NPCName);
	void MainCommand(bool Ingredient1, bool Ingredient2, bool Ingredient3);
	void IsPremium();
	void ResetTriangles();
	//Icons will not collapse into the middle
	void SetIconsToRemainToTheLeft();
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWrapBox> TriangleBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleCookStyle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> Ingredient1Overlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleIngredient1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> Ingredient2Overlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleIngredient2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> Ingredient3Overlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleIngredient3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> CutOverlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TriangleWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> PremiumOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Recipe Rectangle")
	float MinSeparationIfOnlyCutAndCookStyle = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Command Recipe Rectangle")
	float NormalSeparationIfOnlyCutAndCookStyle = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
	FSlateBrush CheeseUpImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
	FSlateBrush CheeseDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
	FSlateBrush JamUpImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
	FSlateBrush JamDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
	FSlateBrush WineUpImage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredient Images")
    FSlateBrush WineDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalCheeseUpImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalCheeseDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalJamUpImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalJamDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalWineUpImage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
    FSlateBrush RoyalWineDownImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Royal Ingredient Images")
	FSlateBrush RoyalWhateverIngredientImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FNPCTriangleCommandInfo GoctorCommandImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FNPCTriangleCommandInfo GailorCommandImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FNPCTriangleCommandInfo GmithCommandImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FNPCTriangleCommandInfo GavernCommandImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Images")
	FNPCTriangleCommandInfo MainCommandImages;

	virtual void NativeConstruct() override;
	
private:
	bool UpTriangleForWeapon = false;
	FGameplayTag EnemyTag;
	FGameplayTag WeaponStyle;
	FGameplayTag CookStyle;
	//Canvas Panel Slot of Weapon Triangle used for sizing the widget
	TObjectPtr<UWrapBoxSlot> CutOverlayWrapBoxSlot;

	void SetEnemyCutAndCookStyle(FNPCTriangleCommandInfo NPCCommandInfo);
};
