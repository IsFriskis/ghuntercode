// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "WeaponType.generated.h"

USTRUCT()
struct FWeaponTextures
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere, meta = (Categories = "Weapons"))
	FGameplayTag Tag;
};

class UTexture2D;
class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UWeaponType : public UTTUserWidget
{
	GENERATED_BODY()

public:
	void UpdateImage(FGameplayTag newWeapon);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SelectedWeapon;
	UPROPERTY(EditAnywhere, Category = "TT|Images")
	TArray<FWeaponTextures> Textures;
};
