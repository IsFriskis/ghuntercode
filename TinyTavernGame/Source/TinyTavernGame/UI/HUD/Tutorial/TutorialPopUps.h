// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TinyTavernGame/Actor/Tutorial/TutorialDataTable.h"
#include "TutorialPopUps.generated.h"


/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTutorialPopUps : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetTutorialData(FPopUpsTableRow Tutorial);
	void SetTutorialData_Implementation(FPopUpsTableRow Tutorial);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPopUpsTableRow TutorialRow;
	
};
