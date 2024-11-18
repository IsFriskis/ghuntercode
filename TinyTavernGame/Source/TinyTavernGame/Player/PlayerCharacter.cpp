// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "AlphaBlend.h"
#include "MotionWarpingComponent.h"
#include "TTPlayerController.h"
#include "TinyTavernGame/UI/TTHUD.h"
#include "../TT_GameplayTags.h"
#include "../Components/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/3DHuds/Hud3D.h"


class UTT_GameInstance;

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon Component"));
	
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Skeletal Mesh"));
	WeaponSkeletalMesh.Get()->SetupAttachment(GetMesh());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent.Get()->SetupAttachment(GetMesh());
	
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Melee Mesh"));
	WeaponStaticMesh.Get()->SetupAttachment(GetMesh());
	
	WeaponIndicatorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Indicator Mesh"));
	WeaponIndicatorStaticMesh.Get()->SetupAttachment(GetRootComponent());
	
	WeaponIndicatorStaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Indicator Mesh 2"));
	WeaponIndicatorStaticMesh2.Get()->SetupAttachment(GetRootComponent());
	
	PlayerMotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Player Motion Warping"));

	OffsetSpringArm = 100;

	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	
	PlayerPointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PlayerPointLight"));
	PlayerPointLight->SetupAttachment(GetRootComponent());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPointLight.Get()->SetVisibility(false);
	TTGameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	InitAbilityActorInfo();
	AddCharacterAbilities();
	GetCharacterMovement()->MaxWalkSpeed = Cast<UTT_AttributeSet>(AttributeSet)->MaxSpeed.GetCurrentValue();

	NiagaraComponent.Get()->Activate();
	
	if(const TObjectPtr<UTT_AttributeSet> PlayerASC = Cast<UTT_AttributeSet>(AttributeSet))
	{
		//Binds delegate AttributeChanged
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerASC.Get()
			->GetSpeedAttribute()).AddUObject(this,&ABasicCharacter::SetMaxSpeed);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerASC.Get()
			->GetPoisonResistanceAttribute()).AddUObject(this,&ABasicCharacter::InflictPoisonDamage);

		//Active HitReactTagChanged when tag States_HitReact Change
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTags::Get().States_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,&APlayerCharacter::HitReactTagChanged);
	}
	//Apply Settings for map
	TTGameInstance.Get()->ApplySettings();

	AlphaBlendCombat.SetBlendTime(TransitionToCombatCamBlendTime);
	AlphaBlendNormal.SetBlendTime(TransitionToCombatCamBlendTime);

	
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Remove Bindings
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Cast<UTT_AttributeSet>(GetAttributeSet())
		->GetSpeedAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Cast<UTT_AttributeSet>(GetAttributeSet())
		->GetPoisonResistanceAttribute()).RemoveAll(this);

	//Active HitReactTagChanged when tag States_HitReact Change
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTags::Get().States_HitReact,EGameplayTagEventType::NewOrRemoved).RemoveAll(
		this);
}

void APlayerCharacter::CheckOnAir()
{
	FVector PlayerPosition = GetActorLocation();
	FVector DownwardDirection = FVector(0, 0, -1);
	FVector EndPosition = PlayerPosition + (DownwardDirection * 200);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
	HitResult,
	PlayerPosition,
		EndPosition,
	ECC_Visibility,
		Params
	);
	if(bHit)
	{
	//	printf("HitName: %s", *HitResult.GetActor()->GetName());
		LastGroundPos = HitResult.ImpactPoint;
	}
	else
	{
		SetActorLocation(LastGroundPos);
	}
		// Optionally, draw the raycast for debugging
	//DrawDebugLine(GetWorld(), PlayerPosition, EndPosition, FColor::Green, false, -1.0f, 0, 1);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsAiming)
	{
		//GetAbilitySystemComponent()->AddGameplayCue(FGameplayTags::Get().GameplayCue_Player_Arrow);
		MoveCameraToAim(DeltaSeconds);
	}
	else if(!IsCameraAttachedToPlayer)
	{
		//GetAbilitySystemComponent()->RemoveGameplayCue(FGameplayTags::Get().GameplayCue_Player_Arrow);
		MoveCameraBackToPlayer(DeltaSeconds);
	}

	CheckOnAir();
	
	if(!CanFall)
	{
		NoFallingSystem();
	}

	CombatCam ? CameraCombat(DeltaSeconds) : CameraNormal(DeltaSeconds);
}

void APlayerCharacter::CameraCombat(float DeltaSeconds)
{
	if(FirstExecCameraCombat)
	{
		AlphaBlendNormal.Reset();
		AlphaBlendNormal.SetBlendTime(TransitionToCombatCamBlendTime);
		FirstExecCameraCombat = false;
		FirstExecCameraNormal = true;
		StartCamValue = SpringArmComponent->TargetArmLength;
	}
	if(!AlphaBlendCombat.IsComplete())
	{	
		// Update the alpha blend
		AlphaBlendCombat.SetBlendTime(TransitionToCombatCamBlendTime);
		AlphaBlendCombat.Update(DeltaSeconds);

		// Interpolate the spring arm length
		float NewLength = FMath::Lerp(StartCamValue, 2400.0f, AlphaBlendCombat.GetAlpha());
		SpringArmComponent->TargetArmLength = NewLength;
	}
}

void APlayerCharacter::CameraNormal(float DeltaSeconds)
{
	if(FirstExecCameraNormal)
	{
		AlphaBlendCombat.Reset();
		AlphaBlendCombat.SetBlendTime(TransitionToCombatCamBlendTime);
		FirstExecCameraNormal = false;
		FirstExecCameraCombat = true;
		StartCamValue = SpringArmComponent->TargetArmLength;
	}
	if(!AlphaBlendNormal.IsComplete())
	{
		
		AlphaBlendNormal.SetBlendTime(TransitionToCombatCamBlendTime);
		// Update the alpha blend
		AlphaBlendNormal.Update(DeltaSeconds);

		// Interpolate the spring arm length
		float NewLength = FMath::Lerp(StartCamValue, 1800.0f, AlphaBlendNormal.GetAlpha());
		SpringArmComponent->TargetArmLength = NewLength;
	}

}

void APlayerCharacter::NoFallingSystem()
{
	//PSEUDOCODE
	//Check if falling
	if(!IsFalling)
	{
		LastPosition = GetActorTransform();	
	}

	if(FallingTest()) //TRUE IS FALLING FALSE IS NOT FALLING
	{
		SetActorLocation(LastPosition.GetLocation());
	//	SetActorRotation(LastPosition.GetRotation());
	}
	
}

bool APlayerCharacter::FallingTest()
{
	//9 perform a raycast downwards
	int Result = 0;

	FVector Direction = GetCharacterMovement()->Velocity;
	Direction.Normalize();
	if(Direction.Size() == 0)
	{
		Direction = GetActorForwardVector();
	}
	//printVector(Direction);
	float Rotation = 20;

	FRotator RotationNeeded(0, Rotation, 0);
	FVector RotatedDirection = RotationNeeded.RotateVector(Direction);
	FRotator MinusRotationNeeded(0, -Rotation, 0);
	FVector MinusRotatedDirection = MinusRotationNeeded.RotateVector(Direction);

	//
	PerformDownwardRaycast(Direction, 50, 100) ? Result++ : Result;
	PerformDownwardRaycast(Direction, 100, 150) ? Result++ : Result;
	PerformDownwardRaycast(Direction, 150, 200) ? Result++ : Result;

	PerformDownwardRaycast(RotatedDirection, 50, 100) ? Result++ : Result;
	PerformDownwardRaycast(RotatedDirection, 100, 150)? Result++ : Result;
	PerformDownwardRaycast(RotatedDirection, 150, 200)? Result++ : Result;
	
	PerformDownwardRaycast(MinusRotatedDirection, 50, 100)? Result++ : Result;
	PerformDownwardRaycast(MinusRotatedDirection, 100, 150)? Result++ : Result;
	PerformDownwardRaycast(MinusRotatedDirection, 150, 200)? Result++ : Result;

	if(Result < 5)
	{
		//IF MINUS THRESHOLD IS FALLING 
		IsFalling = true;
		return true;
	}
	else
	{
		IsFalling = false;
		return false;
	}

}

bool APlayerCharacter::PerformDownwardRaycast(FVector Direction, float ForwardUnits, float DownwardsUnits)
{
	FVector PlayerPosition = GetActorLocation();
	FVector ForwardDirection = Direction;
	FVector DownwardDirection = FVector(0, 0, -1);

	// First raycast: forward 100 units
	FVector ForwardEnd = PlayerPosition + (ForwardDirection * ForwardUnits);

		// Second raycast: downward 100 units from the end of the first raycast
		FVector DownwardEnd = ForwardEnd + (DownwardDirection * DownwardsUnits);
		FHitResult DownwardHitResult;
		FCollisionQueryParams DownwardParams;
		bool bDownwardHit = GetWorld()->LineTraceSingleByChannel(
			DownwardHitResult,
			ForwardEnd,
			DownwardEnd,
			ECC_Visibility,
			DownwardParams
		);

		// Draw the second raycast for debugging
	//	DrawDebugLine(GetWorld(), ForwardEnd, DownwardEnd, FColor::Green, false, -1, 0, 2);
	
	if (bDownwardHit)
	{
		FVector ImpactNormal = DownwardHitResult.ImpactNormal;
		FVector UpwardVector = FVector(0, 0, 1);

		// Calculate the angle between ImpactNormal and UpwardVector
		float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ImpactNormal, UpwardVector)));

		FVector NormalEnd = DownwardHitResult.Location + (ImpactNormal * 50);
	//	DrawDebugLine(GetWorld(), DownwardHitResult.Location, NormalEnd, FColor::Red, false, -1, 0, 2);
		// Check if the angle differs more than 80 degrees
		if (Angle > 30.0f)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}


void APlayerCharacter::DisplaceCamera(bool _isAiming)  
{
	IsAiming = _isAiming;
	InterpolationAlpha = 0;
}

void APlayerCharacter::SetHUDVisibility(bool ShouldHide)
{
	OnHudVisibilityChange.Broadcast(ShouldHide);
}

void APlayerCharacter::InventoryFull(bool IsInventoryFull)
{
	if(IsInventoryFull)
	{
		DisplayNotificationPopUp(FGameplayTags::Get().Notification_FullInventory);
	}
	OnInventoryFullChange.Broadcast(IsInventoryFull);
}

void APlayerCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	HitReactingTagCount = NewCount > 0;
}

void APlayerCharacter::DieTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	DieTagCount = NewCount > 0;
}

void APlayerCharacter::InitAbilityActorInfo()
{
	InitializeAttributes();
	InitializePoisonRegeneration();
	//Access Attributes DataTable
	for (FTTAttributesStruct playerAttribute : TTGameInstance.Get()->AttributesData)
	{
		float magnitude;
		playerAttribute.Attribute.ModifierMagnitude.GetStaticMagnitudeIfPossible(0,magnitude);
		AbilitySystemComponent.Get()->ApplyModToAttribute(playerAttribute.Attribute.Attribute, playerAttribute.Attribute.ModifierOp, magnitude);
	}
	if(TTGameInstance.Get()->IsGavernDay)
	{
		if(TTGameInstance.Get()->GavernUpgrade == SHIELD)
		{
			AbilitySystemComponent.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerShield, TTGameInstance.Get()->NumberOfHeartShield);
		}
		else if(TTGameInstance.Get()->GavernUpgrade == PUFFBALLRESISTANCE)
		{
			TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(GetAttributeSet());
			attributeSet.Get()->SetMaxPoisonResistance(attributeSet.Get()->GetMaxPoisonResistance() + attributeSet.Get()->GetMaxPoisonResistance() * 0.5f);
			//attributeSet.Get()->SetPoisonResistance(attributeSet.Get()->GetMaxPoisonResistance());
		}
	}
	//--
	if(ATTPlayerController* PC = Cast<ATTPlayerController>(GetController()))
	{
		if(TObjectPtr<ATTHUD> TTHUD = Cast<ATTHUD>(PC->GetHUD()))
		{
			TTHUD->InItOverlay(PC,AbilitySystemComponent,AttributeSet, WeaponComponent);
		}
	}
}

void APlayerCharacter::MoveCameraToAim(float DeltaSeconds)
{
	IsCameraAttachedToPlayer = false;
	FVector CameraLocation = SpringArmComponent->GetComponentLocation();
	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector TargetLocation = GetActorLocation() + (ForwardVector * OffsetSpringArm);
	FVector Direction = TargetLocation - CameraLocation;

	if(CameraLocation != TargetLocation)
	{
		CameraLocation += Direction * CameraSpeed * DeltaSeconds;
		SpringArmComponent->SetWorldLocation(CameraLocation);
	}
	else
	{
		SpringArmComponent->SetWorldLocation(TargetLocation);
	}
	
	//TODO Don´t Delete
	//IsCameraAttachedToPlayer = false;
	//FVector CameraLocation = SpringArmComponent->GetComponentLocation();
	//FVector ForwardVector = GetRootComponent()->GetForwardVector();
	//FVector TargetLocation = GetActorLocation() + (ForwardVector * m_OffsetSpringArm);
	//FVector Direction = TargetLocation - CameraLocation;

	//// Calculate the distance between camera and target
	//float Distance = FVector::Distance(CameraLocation, TargetLocation);

	//printf("Distance: %f", Distance);

	//// Check if the distance is below a certain threshold
	//if (Distance < JitterThreshold)
	//{
	//	// Set camera directly to target location
	//	SpringArmComponent->SetWorldLocation(TargetLocation);
	//	return;
	//}

	//// Adjust camera speed based on distance (optional)
	//float AdjustedCameraSpeed = CameraSpeed * DeltaSeconds;

	//// Smooth the increase of interpolation factor (Alpha) over time
	//InterpolationAlpha = FMath::FInterpTo(InterpolationAlpha, 1.0f, DeltaSeconds, InterpolationSpeed);

	//// Perform linear interpolation
	//FVector NewCameraLocation = FMath::Lerp(CameraLocation, TargetLocation, InterpolationAlpha);

	//// Set the new camera location
	//SpringArmComponent->SetWorldLocation(NewCameraLocation);
}

void APlayerCharacter::MoveCameraBackToPlayer(float DeltaSeconds)
{
	FVector CameraLocation = SpringArmComponent->GetComponentLocation();
	FVector Direction = GetActorLocation() - CameraLocation;
	if(CameraLocation != GetActorLocation())
	{
		CameraLocation += Direction * CameraSpeed * DeltaSeconds;
		SpringArmComponent->SetWorldLocation(CameraLocation);
	}
	else
	{
		IsCameraAttachedToPlayer = true;
	}
}

void APlayerCharacter::InitializePoisonRegeneration()
{
	check(AbilitySystemComponent);
	check(GEPoisonRegen);
	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GEPoisonRegen, 1, Handle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void APlayerCharacter::DisplayNotificationPopUp(FGameplayTag Weapon)
{
	Cast<UHud3D>(GetWidgetBar())->ShowNotificationPopUp(Weapon);
}

UUserWidget* APlayerCharacter::GetActivePopUpWidget()
{
	return ActivePopUpWidget;
}

void APlayerCharacter::SetActivePopUpWidget(UUserWidget* NewWidget)
{
	ActivePopUpWidget = NewWidget;
}
