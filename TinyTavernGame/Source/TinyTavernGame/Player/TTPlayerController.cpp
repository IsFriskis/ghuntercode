// Fill out your copyright notice in the Description page of Project Settings.

#include "../Player/TTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ScreenPass.h"
#include "../Components/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "TinyTavernGame/UI/TTHUD.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "../TinyTavernUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "Windows/WindowsPlatformInput.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/RecipeBook.h"


ATTPlayerController::ATTPlayerController()
{
	bReplicates = true;
}

void ATTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetPawn<APlayerCharacter>();
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TTInputMappingContext, 0);
	}
	LocalPlayer = Cast<ULocalPlayer>(Player);
	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);
	CurrentMouseCursor = EMouseCursor::Crosshairs;
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = EKeys::Invalid;
	SetShowMouseCursor(false);
	TTGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
}

void ATTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//Move Input
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATTPlayerController::Move);

	//Aim Input
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ATTPlayerController::Aim);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATTPlayerController::NotAim);
	EnhancedInputComponent->BindAction(RotateCamAction, ETriggerEvent::Triggered, this, &ATTPlayerController::RotateCam);
	EnhancedInputComponent->BindAction(RotateCamAction, ETriggerEvent::Completed, this, &ATTPlayerController::EndRotateCam);

	//Ability Input
	EnhancedInputComponent->BindAction(FireMeleeAbilityAction, ETriggerEvent::Started, this, &ATTPlayerController::FireMeleeAbility);
	EnhancedInputComponent->BindAction(FireMeleeAbilityAction, ETriggerEvent::Completed, this, &ATTPlayerController::CompletedFireMeleeAbility);
	EnhancedInputComponent->BindAction(FireRangeAbilityAction, ETriggerEvent::Started, this, &ATTPlayerController::FireRangeAbility);
	EnhancedInputComponent->BindAction(FireRangeAbilityAction, ETriggerEvent::Completed, this, &ATTPlayerController::CompletedFireRangeAbility);
	EnhancedInputComponent->BindAction(FireIngredientAbilityAction, ETriggerEvent::Started, this, &ATTPlayerController::FireIngredientAbility);
	EnhancedInputComponent->BindAction(FireIngredientAbilityAction, ETriggerEvent::Completed, this, &ATTPlayerController::CompletedFireIngredientAbility);

	//ChangeWeapons Input
	EnhancedInputComponent->BindAction(ChangeRangeWeaponAction, ETriggerEvent::Started, this, &ATTPlayerController::ChangeRangeWeapon);
	EnhancedInputComponent->BindAction(ChangeIngredientWeaponAction, ETriggerEvent::Started, this, &ATTPlayerController::ChangeIngredientWeapon);

	//Open Settings Input
	EnhancedInputComponent->BindAction(OpenSettingsAction, ETriggerEvent::Started, this, &ATTPlayerController::OpenSettingsMenu);
	EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Started, this, &ATTPlayerController::Back);

	//Open Quests Input
	EnhancedInputComponent->BindAction(OpenQuestsAction, ETriggerEvent::Started, this, &ATTPlayerController::OpenQuestsMenu);
	EnhancedInputComponent->BindAction(CloseQuestsAction, ETriggerEvent::Started, this, &ATTPlayerController::CloseQuestsMenu);
	EnhancedInputComponent->BindAction(NextQuestsAction, ETriggerEvent::Started, this, &ATTPlayerController::SelectNextQuest);
	EnhancedInputComponent->BindAction(LastQuestsAction, ETriggerEvent::Started, this, &ATTPlayerController::SelectLastQuest);

	//Interact Input
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATTPlayerController::Interact);

	//Quick-Time-Event Input
	EnhancedInputComponent->BindAction(QTECleaverAction, ETriggerEvent::Started, this, &ATTPlayerController::QTECleaver);
	EnhancedInputComponent->BindAction(QTETenderizerAction, ETriggerEvent::Started, this, &ATTPlayerController::QTETenderizer);
	EnhancedInputComponent->BindAction(QTESkewerAction, ETriggerEvent::Started, this, &ATTPlayerController::QTESkewer);
	EnhancedInputComponent->BindAction(QTEButtonAAction, ETriggerEvent::Started, this, &ATTPlayerController::QTEButtonA);
}

void ATTPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsAimingPC || IsAimingController)
	{
		double gameRotation;
		if (IsAimingPC)
		{
			FVector MouseWorldLocation, MouseWorldDirection;
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

			// Ahora necesitamos encontrar dónde la dirección del mouse intersecta con el plano donde está el jugador
			FVector PlaneOrigin = PlayerCharacter->GetActorLocation();
			FVector PlaneNormal = FVector(0, 0, 1); // Normal del plano horizontal
			FVector IntersectionPoint;
			FMath::SegmentPlaneIntersection(MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000, FPlane(PlaneOrigin, PlaneNormal), IntersectionPoint);

			FVector PlayerLocation = PlayerCharacter->GetActorLocation();

			FVector AimDirection = (IntersectionPoint - PlayerLocation).GetSafeNormal();
			
			FRotator NewRotation = AimDirection.Rotation();
			FRotator currentRotation = GetControlRotation();
			FRotator newRotation = FMath::RInterpConstantTo(currentRotation, NewRotation,DeltaSeconds, TTGameInstance.Get()->InterpolationSpeed);

			// Solo rotas en el eje Z (Yaw) si es un juego isométrico
			SetControlRotation(newRotation);
		}
		else
		{
			//Controller coordinates
			FVector2d pointTarget = FVector2d(AimInput.X, AimInput.Y);
			double rotationRadians = atan2(pointTarget.Y, pointTarget.X);
			double degreeRotation = FMath::RadiansToDegrees(rotationRadians);
			gameRotation = degreeRotation - 45; // 45 is an offset because of the mesh
			FRotator rollRotation = FRotator(0, gameRotation, 0);
			FRotator currentRotation = GetControlRotation();
			FRotator newRotation = FMath::RInterpConstantTo(currentRotation, rollRotation,DeltaSeconds, TTGameInstance.Get()->InterpolationSpeed);

			SetControlRotation(newRotation);
		}
	}
}

void ATTPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = PlayerCharacter->GetCamera()->GetComponentRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATTPlayerController::Aim(const FInputActionValue& InputActionValue)
{
	if (!PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_AimBlocked))
	{
		if(!PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming))
		{
			PlayerCharacter.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		}
		IsAimingPC = true;
		float XMouse;
		float YMouse;
		GetMousePosition(XMouse, YMouse);
		////Activate Strafe
		PlayerCharacter.Get()->SetStrafe(true);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		SetShowMouseCursor(true);
	}
}

void ATTPlayerController::NotAim(const FInputActionValue& InputActionValue)
{
	if(PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming))
	{
		PlayerCharacter.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
	IsAimingPC = false;
	//Deactivate Strafe
	PlayerCharacter.Get()->SetStrafe(false);
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	SetShowMouseCursor(false);
}

void ATTPlayerController::OpenQuestsMenu()
{
	OnQuestMenuOpenOrClose.Broadcast(true);
}

void ATTPlayerController::CloseQuestsMenu()
{
	OnQuestMenuOpenOrClose.Broadcast(false);
}

void ATTPlayerController::SelectNextQuest()
{
	OnQuestNextOrLast.Broadcast(true);
}

void ATTPlayerController::SelectLastQuest()
{
	OnQuestNextOrLast.Broadcast(false);
}

void ATTPlayerController::RotateCam(const FInputActionValue& InputActionValue)
{
	if (!PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_AimBlocked))
	{
		if(!PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming))
		{
			PlayerCharacter.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		}
		const FVector2d InputAxisVector = FVector2d(InputActionValue.Get<FVector2d>().X, InputActionValue.Get<FVector2d>().Y);
		//Activate Strafe
		PlayerCharacter.Get()->SetStrafe(true);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		AimInput = InputAxisVector;
		IsAimingController = true;
	}
}

void ATTPlayerController::EndRotateCam(const FInputActionValue& InputActionValue)
{
	if(PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming))
	{
		PlayerCharacter.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
	//Deactivate Strafe
	IsAimingController = false;
	PlayerCharacter.Get()->SetStrafe(false);
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ATTPlayerController::FireMeleeAbility(const FInputActionValue& InputActionValue)
{
	OnHoldMelee.Broadcast();
}

void ATTPlayerController::CompletedFireMeleeAbility(const FInputActionValue& InputActionValue)
{
	FGameplayEventData payload;
	PlayerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().Events_Weapon_FinishMeleeWeaponAbility, &payload);
}

void ATTPlayerController::FireRangeAbility(const FInputActionValue& InputActionValue)
{
	OnRange.Broadcast();
}

void ATTPlayerController::CompletedFireRangeAbility(const FInputActionValue& InputActionValue)
{
	FGameplayEventData payload;
	PlayerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().Events_Weapon_FinishRangeWeaponAbility, &payload);
	OnReleaseRange.Broadcast();
}

void ATTPlayerController::FireIngredientAbility(const FInputActionValue& InputActionValue)
{
	OnHoldIngredient.Broadcast();
}

void ATTPlayerController::CompletedFireIngredientAbility(const FInputActionValue& InputActionValue)
{
	FGameplayEventData payload;
	PlayerCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().Events_Weapon_FinishIngredientWeaponAbility, &payload);
	//Input for ingredient
}

void ATTPlayerController::ChangeRangeWeapon(const FInputActionValue& InputActionValue)
{
	OnRangeWeaponChange.Broadcast();
}

void ATTPlayerController::ChangeIngredientWeapon(const FInputActionValue& InputActionValue)
{
	OnIngredientWeaponChange.Broadcast();
}

void ATTPlayerController::Interact(const FInputActionValue& InputActionValue)
{
	OnInteract.Broadcast();
}

void ATTPlayerController::Back(const FInputActionValue& InputActionValue)
{
	OnBackAction.Broadcast();
}

void ATTPlayerController::QTECleaver(const FInputActionValue& InputActionValue)
{
	OnQTECleaver.Broadcast();
}

void ATTPlayerController::QTETenderizer(const FInputActionValue& InputActionValue)
{
	OnQTETenderizer.Broadcast();
}

void ATTPlayerController::QTESkewer(const FInputActionValue& InputActionValue)
{
	OnQTESkewer.Broadcast();
}

void ATTPlayerController::QTEButtonA(const FInputActionValue& InputActionValue)
{
	OnQTEButtonA.Broadcast();
}

void ATTPlayerController::SwitchToGameplayInputContext()
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController());
	if (InputSubsystem)
	{
		InputSubsystem->RemoveMappingContext(TTQTEMappingContext);
		InputSubsystem->AddMappingContext(TTInputMappingContext, 1); // High priority
	}
}

void ATTPlayerController::SwitchToQTEInputContext()
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController());
	if (InputSubsystem)
	{
		InputSubsystem->RemoveMappingContext(TTInputMappingContext);
		InputSubsystem->AddMappingContext(TTQTEMappingContext, 1); // High priority
	}
}

void ATTPlayerController::OpenSettingsMenu()
{
	if (!InPauseMenu && CanInteractWithRecipeBook)
	{
		CanInteractWithRecipeBook = false;
		CurrentMouseCursor = EMouseCursor::Default;
		SetShowMouseCursor(true);
		InPauseMenu = true;
		if(!IsPaused())
		{
			Pause();
		}
		else
		{
			IsGamePaused = true;
		}
		CanInteractWithRecipeBook = true;
		//Button A acts like Click
		FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
		virtual_key = FPlatformInput::GetGamepadAcceptKey();
		PauseMenuWidget = Cast<URecipeBook>(CreateWidget(GetWorld(),PauseMenuWidgetClass));
		PauseMenuWidget.Get()->AddToViewport(2);
		PauseMenuWidget.Get()->ActivateWidget();
		if(!PlayerCharacter.Get()->GetHUDIsHidden())
		{
			PlayerCharacter.Get()->SetHUDVisibility(true);
			HUDWasInvisible = false;
		}
		else
		{
			HUDWasInvisible = true;
		}
		//OnSettingsMenu.Broadcast(InPauseMenu);
	}
	else if (CanInteractWithRecipeBook)
	{
		if(!IsGamePaused && IsPaused())
		{
			Pause();
		}
		IsGamePaused = false;
		CanInteractWithRecipeBook = false;
		HideCursorAndSetCorrectInputMode();
		InPauseMenu = false;
		PauseMenuWidget.Get()->RemoveFromParent();
		if(PlayerCharacter.Get()->GetHUDIsHidden() && !HUDWasInvisible)
		{
			PlayerCharacter.Get()->SetHUDVisibility(false);
		}
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle, // handle to cancel timer at a later time
			this, // the owning object
			&ATTPlayerController::PlayerIsAbleToCloseRecipeBook, // function to call on elapsed
			TimeOfRecipeBookAnimation, // float delay until elapsed
			false);
		//OnSettingsMenu.Broadcast(InPauseMenu);
	}
}

void ATTPlayerController::PlayerIsAbleToCloseRecipeBook()
{
	CanInteractWithRecipeBook = true;
	if (InPauseMenu)
	{
		Pause();
	}
}


// #if WITH_EDITOR

void ATTPlayerController::MaxAmmo()
{
	TObjectPtr<UTT_AttributeSet> TTAttributeSet = Cast<UTT_AttributeSet>(PlayerCharacter.Get()->GetAttributeSet());
	TTAttributeSet.Get()->SetMaxGrilledAmmo(10000);
	TTAttributeSet.Get()->SetMaxBoiledAmmo(10000);
	TTAttributeSet.Get()->SetMaxFriedAmmo(10000);
	TTAttributeSet.Get()->SetGrilledAmmo(TTAttributeSet.Get()->GetMaxGrilledAmmo());
	TTAttributeSet.Get()->SetBoiledAmmo(TTAttributeSet.Get()->GetMaxBoiledAmmo());
	TTAttributeSet.Get()->SetFriedAmmo(TTAttributeSet.Get()->GetMaxFriedAmmo());
	TTAttributeSet.Get()->SetMaxCheeseAmmo(5);
	TTAttributeSet.Get()->SetMaxWineAmmo(5);
	TTAttributeSet.Get()->SetMaxJamAmmo(5);
	TTAttributeSet.Get()->SetCheeseAmmo(5);
	TTAttributeSet.Get()->SetWineAmmo(5);
	TTAttributeSet.Get()->SetJamAmmo(5);
}

void ATTPlayerController::ReceiveDamageMode()
{
	PlayerCharacter.Get()->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	PlayerCharacter.Get()->GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldStatic);
}

void ATTPlayerController::NoDamageMode()
{
	PlayerCharacter.Get()->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void ATTPlayerController::TakeXDamage(float Amount)
{
	TObjectPtr<UTT_AttributeSet> TTAttributeSet = Cast<UTT_AttributeSet>(PlayerCharacter.Get()->GetAttributeSet());
	TTAttributeSet.Get()->SetHealth(TTAttributeSet.Get()->GetHealth() - Amount);

}

void ATTPlayerController::ChangeSpeed(float Amount)
{
	TObjectPtr<UTT_AttributeSet> TTAttributeSet = Cast<UTT_AttributeSet>(PlayerCharacter.Get()->GetAttributeSet());
	TTAttributeSet.Get()->SetMaxSpeed(Amount);
	PlayerCharacter.Get()->GetCharacterMovement()->MaxWalkSpeed = Amount;
}

void ATTPlayerController::TPToTavern()
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->SaveAmmunition();
	UGameplayStatics::OpenLevel(GetWorld(), "TavernGym");
}

void ATTPlayerController::TPToAdventure()
{
	UGameplayStatics::OpenLevel(GetWorld(), "P_Map");
}

void ATTPlayerController::CompleteMainOrder()
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	for (TTuple<int, bool> order : GI.Get()->MainOrders)
	{
		GI.Get()->MainOrders[order.Key] = true;
	}
}

void ATTPlayerController::CompleteSecondaryOrders()
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	for (FSecondaryCommand order : GI.Get()->AcceptedOrders)
	{
		GI.Get()->FinishedOrders.Add(order);
	}
	GI.Get()->AcceptedOrders.Empty();
}

void ATTPlayerController::CompleteASecondaryOrder(int PositionOfSecondaryOrder)
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	PositionOfSecondaryOrder = FMath::Clamp(PositionOfSecondaryOrder, 0, GI.Get()->AcceptedOrders.Num() - 1);
	FSecondaryCommand plate = GI.Get()->AcceptedOrders[PositionOfSecondaryOrder];
	GI.Get()->FinishedOrders.Add(plate);
	GI.Get()->AcceptedOrders.Remove(plate);
}

void ATTPlayerController::AddExtraPlates(int Number)
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	switch (Number)
	{
	case 1:
		GI.Get()->ExtraPlatesInventory.Add(111100);
		break;
	case 2:
		GI.Get()->ExtraPlatesInventory.Add(111100);
		GI.Get()->ExtraPlatesInventory.Add(113010);
		break;
	case 3:
		GI.Get()->ExtraPlatesInventory.Add(111100);
		GI.Get()->ExtraPlatesInventory.Add(113010);
		GI.Get()->ExtraPlatesInventory.Add(112000);
		break;
	case 4:
		GI.Get()->ExtraPlatesInventory.Add(111100);
		GI.Get()->ExtraPlatesInventory.Add(113010);
		GI.Get()->ExtraPlatesInventory.Add(112000);
		GI.Get()->ExtraPlatesInventory.Add(113100);
		break;
	case 5:
		GI.Get()->ExtraPlatesInventory.Add(111100);
		GI.Get()->ExtraPlatesInventory.Add(113010);
		GI.Get()->ExtraPlatesInventory.Add(112000);
		GI.Get()->ExtraPlatesInventory.Add(113100);
		GI.Get()->ExtraPlatesInventory.Add(432111);
		break;
	default:;
	}
}

void ATTPlayerController::AddGoctorRelic(int Number)
{
	Number = FMath::Clamp(Number, 0, 3);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->RelicsInventory[0] = Number;
}

void ATTPlayerController::AddGailorRelic(int Number)
{
	Number = FMath::Clamp(Number, 0, 4);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->RelicsInventory[1] = Number;
}

void ATTPlayerController::AddGmithRelic(int Number)
{
	Number = FMath::Clamp(Number, 0, 3);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->RelicsInventory[2] = Number;
}

void ATTPlayerController::AddAllRelics()
{
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->RelicsInventory[0] = 3;
	GI.Get()->RelicsInventory[1] = 4;
	GI.Get()->RelicsInventory[2] = 3;
}

void ATTPlayerController::AddGoctorUpgrade(int Number)
{
	Number = FMath::Clamp(Number, 0, 1);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->UpgradeTier[0] = Number;
}

void ATTPlayerController::AddGailorUpgrade(int Number)
{
	Number = FMath::Clamp(Number, 0, 1);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->UpgradeTier[1] = Number;
}

void ATTPlayerController::AddGmithUpgrade(int Number)
{
	Number = FMath::Clamp(Number, 0, 1);
	TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	GI.Get()->UpgradeTier[2] = Number;
}

void ATTPlayerController::AddHealth(int Number)
{
	UTT_AttributeSet* AS = Cast<UTT_AttributeSet>(PlayerCharacter->GetAttributeSet());
	AS->SetHealth(AS->GetHealth() + Number);
}

void ATTPlayerController::AddMaxJamAmmo(int Number)
{
	UTT_AttributeSet* AS = Cast<UTT_AttributeSet>(PlayerCharacter->GetAttributeSet());
	AS->SetMaxJamAmmo(AS->GetMaxJamAmmo() + Number);
}


void ATTPlayerController::BasiliscoCookbar30()
{
	TArray<AActor*> Basiliscos;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyBase::StaticClass(), Basiliscos);
}

// #endif
FVector ATTPlayerController::GetMouseProjectionToWorld()
{
	float MouseX, MouseY;
	if (GetMousePosition(MouseX, MouseY))
	{
		FVector WorldLocation, WorldDirection;
		if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
		{
			// Optionally, perform a ray trace to find a hit in the world
			FHitResult HitResult;
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // Adjust the multiplier as needed for your use case

			FCollisionQueryParams Params;
			Params.bTraceComplex = true;
			Params.AddIgnoredActor(GetPawn()); // Ignore the player's pawn

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
			{
				// Hit something in the world
				return HitResult.Location;
			}
			else
			{
				return FVector::ZeroVector;
			}
		}
	}
	return FVector::ZeroVector;
}

void ATTPlayerController::HideCursorAndSetCorrectInputMode()
{
	UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringMouseDown);
	//First set game mode only to make the cursos dissapear
	FInputModeGameOnly inputModeGameOnly;
	SetInputMode(inputModeGameOnly);
	//After that, establish the correct input mode
	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetWidgetToFocus(nullptr);
	SetInputMode(inputMode);
	CurrentMouseCursor = EMouseCursor::Crosshairs;
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = EKeys::Invalid;
	SetShowMouseCursor(false);
}

void ATTPlayerController::InterruptRangedAbility()
{
	CompletedFireRangeAbility(FInputActionValue());
}

