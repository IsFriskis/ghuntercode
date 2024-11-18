// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TTPlayerController.generated.h"

class UTT_GameInstance;
class UWidget;
class UEnhancedInputLocalPlayerSubsystem;
class APlayerCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class URecipeBook;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsMenuControllerSignature, bool, InPauseMenu);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestsMenuOpenOrCloseSignature, bool, Open);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestsMenuNextOrLastSignature, bool, Next);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseMeleeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHoldMeleeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseIngredientSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHoldIngredientSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRangeWeaponChangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleaseRangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractControllerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIngredientWeaponChangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackActionSignature);

//QTE Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTECleaverSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTETenderizerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTESkewerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTEButtonASignature);


/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ATTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ATTPlayerController();

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TTInputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> TTQTEMappingContext;

	// Shooting
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FHoldMeleeSignature OnHoldMelee;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FReleaseMeleeSignature OnReleaseMelee;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FHoldIngredientSignature OnHoldIngredient;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FReleaseIngredientSignature OnReleaseIngredient;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnRangeSignature OnRange;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnReleaseRangeSignature OnReleaseRange;

	//Change weapon
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnRangeWeaponChangeSignature OnRangeWeaponChange;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnRangeWeaponChangeSignature OnIngredientWeaponChange;
	UPROPERTY(BlueprintAssignable, Category = "Settings Menu")
	FOnSettingsMenuControllerSignature OnSettingsMenu;
	UPROPERTY(BlueprintAssignable, Category = "Interact")
	FOnInteractControllerSignature OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Back")
	FOnBackActionSignature OnBackAction;
	
	UPROPERTY(BlueprintAssignable, Category = "QTE")
	FOnQTECleaverSignature OnQTECleaver;

	UPROPERTY(BlueprintAssignable, Category = "QTE")
	FOnQTETenderizerSignature OnQTETenderizer;

	UPROPERTY(BlueprintAssignable, Category = "QTE")
	FOnQTESkewerSignature OnQTESkewer;

	UPROPERTY(BlueprintAssignable, Category = "QTE")
	FOnQTEButtonASignature OnQTEButtonA;
	
	UPROPERTY(BlueprintAssignable, Category = "Quests Menu")
	FOnQuestsMenuOpenOrCloseSignature OnQuestMenuOpenOrClose;

	UPROPERTY(EditAnywhere, Category = "Pause Menu")
	TSubclassOf<URecipeBook> PauseMenuWidgetClass;

	bool GetIsAimingPC() const
	{
		return IsAimingPC;
	}

	bool GetIsAimingController() const
	{
		return IsAimingController;
	}

	UPROPERTY(BlueprintAssignable, Category = "Quests Menu")
	FOnQuestsMenuNextOrLastSignature OnQuestNextOrLast;
	
	//Binding for opening/closing pause menu
	//Need it here so that UPauseMenu can call it
	void OpenSettingsMenu();
	void SwitchToGameplayInputContext();
	void SwitchToQTEInputContext();
	void NotAim(const FInputActionValue& InputActionValue);

	//Quests Menu
	void OpenQuestsMenu();
	void CloseQuestsMenu();
	void SelectNextQuest();
	void SelectLastQuest();

	//Getters & Setters
	
	TObjectPtr<UInputAction> GetAimAction() const {return AimAction;}

	void SetAimAction(TObjectPtr<UInputAction> _AimAction){this->AimAction = _AimAction;}

	FVector2d AimInput;

	FVector GetMouseProjectionToWorld();
	UFUNCTION(BlueprintCallable)
	void HideCursorAndSetCorrectInputMode();

	void InterruptRangedAbility();
	
protected:
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	
	float CameraRotation;
	
private:
	TObjectPtr<URecipeBook> PauseMenuWidget;
	//Move Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	//Aim Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;
	
	//Rotate cam with GamePad
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RotateCamAction;
	//Shoot/Attack/Use Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireMeleeAbilityAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireRangeAbilityAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireIngredientAbilityAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> BackAction;
	//Settings Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> OpenSettingsAction;
	//Quest Menu Inputs
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> OpenQuestsAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CloseQuestsAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> NextQuestsAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LastQuestsAction;

	//Weapons Management Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChangeRangeWeaponAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChangeIngredientWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> QTECleaverAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> QTETenderizerAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> QTESkewerAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> QTEButtonAAction;

	//Aiming
	ULocalPlayer* LocalPlayer;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	bool IsAimingPC = false;
	bool IsAimingController = false;
	bool IsGamePaused = false;
	//Interact Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	//Open RecipeBook
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	bool InPauseMenu = false;
	bool CanInteractWithRecipeBook = true;
	FTimerHandle TimerHandle;
	float TimeOfRecipeBookAnimation = 0.5f;
	bool HUDWasInvisible = false;
	UFUNCTION()
	void PlayerIsAbleToCloseRecipeBook();
	//FViewport* Viewport;
	
	void Move(const FInputActionValue& InputActionValue);
	void Aim(const FInputActionValue& InputActionValue);
	void RotateCam(const FInputActionValue& InputActionValue);
	void EndRotateCam(const FInputActionValue& InputActionValue);
	void FireMeleeAbility(const FInputActionValue& InputActionValue);
	void CompletedFireMeleeAbility(const FInputActionValue& InputActionValue);
	void FireRangeAbility(const FInputActionValue& InputActionValue);
	void CompletedFireRangeAbility(const FInputActionValue& InputActionValue);
	void FireIngredientAbility(const FInputActionValue& InputActionValue);
	void CompletedFireIngredientAbility(const FInputActionValue& InputActionValue);
	void ChangeRangeWeapon(const FInputActionValue& InputActionValue);
	void ChangeIngredientWeapon(const FInputActionValue& InputActionValue);
	void Interact(const FInputActionValue& InputActionValue);
	void Back(const FInputActionValue& InputActionValue);

	void QTECleaver(const FInputActionValue& InputActionValue);
	void QTETenderizer(const FInputActionValue& InputActionValue);
	void QTESkewer(const FInputActionValue& InputActionValue);
	void QTEButtonA(const FInputActionValue& InputActionValue);

	
// #if WITH_EDITOR
	UFUNCTION(Exec, Category = "TT Commands")
	void MaxAmmo();
	UFUNCTION(Exec, Category = "TT Commands")
	void NoDamageMode();
	UFUNCTION(Exec, Category = "TT Commands")
	void ReceiveDamageMode();
	UFUNCTION(Exec, Category = "TT Commands")
	void TakeXDamage(float Amount);
	UFUNCTION(Exec, Category = "TT Commands")
	void ChangeSpeed(float Amount);
	UFUNCTION(Exec, Category = "TT Commands")
	void TPToTavern();
	UFUNCTION(Exec, Category = "TT Commands")
	void TPToAdventure();
	UFUNCTION(Exec, Category = "TT Commands")
	void BasiliscoCookbar30();

	UFUNCTION(Exec, Category = "TT Commands")
	void CompleteMainOrder();
	UFUNCTION(Exec, Category = "TT Commands")
	void CompleteSecondaryOrders();
	UFUNCTION(Exec, Category = "TT Commands")
	void CompleteASecondaryOrder(int PositionOfSecondaryOrder);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddExtraPlates(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGoctorRelic(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGailorRelic(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGmithRelic(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddAllRelics();
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGoctorUpgrade(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGailorUpgrade(int Number);
	UFUNCTION(Exec, Category = "TT Commands")
	void AddGmithUpgrade(int Number);
	UFUNCTION(Exec, Category = "TT Attributes")
	void AddHealth(int Number);
	UFUNCTION(Exec, Category = "TT Attribute")
	void AddMaxJamAmmo(int Number);
// #endif
};
