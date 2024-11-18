// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Statue.h"
#include "WeaponStatue.generated.h"

UCLASS()
class TINYTAVERNGAME_API AWeaponStatue : public AStatue
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponStatue();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	FGameplayTag WeaponUnlocked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Statue Info")
	FText WeaponCanBeUnlockedText;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PlayerInteracted() override;
};
