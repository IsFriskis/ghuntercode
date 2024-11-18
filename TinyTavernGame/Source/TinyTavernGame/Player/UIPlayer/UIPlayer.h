// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UIPlayer.generated.h"

class UTavern;
class UTavernMenu;
class UArticyFlowPlayer;
class UCommonActivatableWidget;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class TINYTAVERNGAME_API AUIPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUIPlayer();
	//Articy Object, place it in BP_Player
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UArticyFlowPlayer> PlayerArticyFlowPlayer;

	//Get a Reference to the Tavern
	void AddTavern(TObjectPtr<UTavernMenu> _TavernMenu, TObjectPtr<UTavern> _Tavern);
	//Functions that manages if pause menu show appear or disappear
	UFUNCTION()
	void OpenSettingsMenu();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TTInputMappingContext;
	//OpenSettings Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> OpenSettingsAction;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	TObjectPtr<UTavern> Tavern;
	TObjectPtr<UTavernMenu> TavernMenu;
	TObjectPtr<UCommonActivatableWidget> RecipeBook;
	bool SettingsMenuOpen = false;
	bool CanInteractWithRecipeBook = true;
	FTimerHandle TimerHandle;
	float TimeOfRecipeBookAnimation = 0.4f;
	UFUNCTION()
	void PlayerIsAbleToCloseRecipeBook();
};
