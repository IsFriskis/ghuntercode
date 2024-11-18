// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TutorialDataTable.generated.h"

class UTutorialPopUps;

USTRUCT(BlueprintType)
struct FPopUpsTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTutorialPopUps> WidgetClassToDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDataTableRowHandle> Buttons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> GIFMaterial;
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTutorialDataTable : public UDataTable
{
	GENERATED_BODY()
};
