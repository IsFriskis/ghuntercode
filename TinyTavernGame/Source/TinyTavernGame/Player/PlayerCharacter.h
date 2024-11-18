// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "../Character/BasicCharacter.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHudVisibilityChangeSignature, bool, shouldHide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryFullSignature, bool, InventoryFull = false);
class UPointLightComponent;
class UTT_GameInstance;
class UMotionWarpingComponent;
class UWeaponComponent;
class UImage;
class UGameplayAbility;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TINYTAVERNGAME_API APlayerCharacter : public ABasicCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//--------------VARIABLES-------------//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> WeaponStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TT| Niagara")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> WeaponIndicatorStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> WeaponIndicatorStaticMesh2;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT| LowHealth Percentage")
	float LowHealth;

	//--------------FUNCTIONS-------------//
	
	UCameraComponent* GetCamera() const {return Camera;}
	UMotionWarpingComponent* GetWarpComponent() const {return PlayerMotionWarping;}
	//Function to Move the springArm (and the camera) to target when aiming
	void DisplaceCamera(bool IsAiming);
	bool GetIsAiming(){return IsAiming;}
	
	//True if HUD should be hidden 
	void SetHUDVisibility(bool ShouldHide);
	
	//Notifies player of inventory full
	void InventoryFull(bool IsInventoryFull = false);
	
	// ------------ PERCEPTION SYSTEM STIMULUS -------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	UPROPERTY(BlueprintReadWrite)
	bool CanFall = false;
	
	UFUNCTION(BlueprintCallable)
	void DisplayNotificationPopUp(FGameplayTag Weapon);
	UFUNCTION(BlueprintCallable)
	void SetStrafe(bool _StrafeActive){StrafeActive = _StrafeActive;}
	UFUNCTION(BlueprintCallable)
	bool GetStrafe(){return StrafeActive;}
	UFUNCTION(BlueprintCallable)
	void ExecuteFireAnimation(){PlayAnimMontage(FireMontage, 0.5f);}
	UFUNCTION(BlueprintCallable)
	void StopFireAnimation(){StopAnimMontage(FireMontage);}
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetActivePopUpWidget();
	UFUNCTION(BlueprintCallable)
	void SetActivePopUpWidget(UUserWidget* NewWidget);
	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent() { return WeaponComponent.Get(); }
	void SetCombatCam(bool _CombatCam){ this->CombatCam = _CombatCam;}
	void SetHUDIsHidden(bool _HUDIsHidden){HUDIsHidden = _HUDIsHidden;}
	bool GetHUDIsHidden(){return HUDIsHidden;}

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT| Materials")
	TObjectPtr<UMaterialInterface> LowHealthMaterial;
	
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnHudVisibilityChangeSignature OnHudVisibilityChange;
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnInventoryFullSignature OnInventoryFullChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MapAreaID;
	
protected:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT| Camera")
	float OffsetSpringArm;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT| Animations")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionWarpingComponent> PlayerMotionWarping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPointLightComponent> PlayerPointLight;

	// ------------ FUNCTIONS -------------//
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void CheckOnAir();
	virtual void Tick(float DeltaSeconds) override;
	void CameraCombat(float DeltaSeconds);
	void CameraNormal(float DeltaSeconds);

	void NoFallingSystem();
	bool FallingTest();
	
	bool PerformDownwardRaycast(FVector Direction, float ForwardUnits, float DownwardsUnits);
	void HitReactTagChanged(const FGameplayTag CallbackTag,int32 NewCount);
	void DieTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

private:
	// ------------ VARIABLES -------------//
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	UPROPERTY(EditAnywhere, Category = "TT| Tools")
	TObjectPtr<AActor> EquippedTool;
	TArray<TObjectPtr<AActor>> UnlockedTools;

	TObjectPtr<UUserWidget> ActivePopUpWidget;
	//Strafe
	bool StrafeActive = false;
	//Variables to manage movement of Camera when aiming and stop aiming
	bool IsAiming = false;
	bool IsCameraAttachedToPlayer = true;
	float InterpolationAlpha = 0;
	bool IsFalling = false;
	FVector LastGroundPos = FVector::ZeroVector;
	FTransform LastPosition = FTransform::Identity; 
	bool CombatCam = false;
	bool HUDIsHidden = false;
	
	UPROPERTY(EditAnywhere, Category = "TT| Camera")
	float CameraSpeed;
	UPROPERTY(EditAnywhere, Category = "TT| Camera")
	float InterpolationSpeed;
	UPROPERTY(EditAnywhere, Category = "TT| Camera")
	float JitterThreshold;
	UPROPERTY(EditAnywhere, Category = "TT| Camera")
	float TransitionToCombatCamBlendTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "TT:GE_PoisonRegneration")
	TSubclassOf<UGameplayEffect> GEPoisonRegen;
	// ------------ FUNCTIONS -------------//
	virtual void InitAbilityActorInfo() override;
	void MoveCameraToAim(float DeltaSeconds);
	void MoveCameraBackToPlayer(float DeltaSeconds);
	void InitializePoisonRegeneration();


	FAlphaBlend AlphaBlendCombat;
	bool FirstExecCameraCombat = true;
	
	FAlphaBlend AlphaBlendNormal;
	bool FirstExecCameraNormal = true;
	
	float StartCamValue = 0;
};
