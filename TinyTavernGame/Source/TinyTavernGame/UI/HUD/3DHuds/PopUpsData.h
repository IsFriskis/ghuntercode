// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PopUpsData.generated.h"


class UTexture2D;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FPopUpInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Type;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Sound;
	UPROPERTY(EditAnywhere)
	bool IsTimeBased = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = IsTimeBased, EditConditionHides))
	int Duration = 0;
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPopUpsData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FPopUpInfo> PopUpType;
};
