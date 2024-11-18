// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Engine/DataAsset.h"
#include "QuestsDataBase.generated.h"

USTRUCT(BlueprintType)
struct FOrders
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly)
	UImage* Image;
	
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQuestsDataBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FOrders> Orders;
};
