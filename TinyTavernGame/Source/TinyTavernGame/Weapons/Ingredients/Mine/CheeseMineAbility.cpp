// TinyTavern


#include "CheeseMineAbility.h"
#include "CheeseMineTask.h"
#include "../../../Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "CheeseAimTask.h"
#include "../../../TT_GameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "Mine.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/CapsuleComponent.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/UI/HUD/3DHuds/Hud3D.h"

void UCheeseMineAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	GetCurrentAbilitySpec()->InputID = 1;
	
	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	//Check if Ammo > 0
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	if(!attributeSet || attributeSet.Get()->GetCheeseAmmo() <= 0)
	{
		Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfCheeseAmmo);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		//GetCurrentAbilitySpec()->InputID = 1;
		//Subscribe when ability is finished by input
		WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().Events_Weapon_FinishIngredientWeaponAbility);
		WaitGameplayEvent.Get()->EventReceived.AddDynamic(this, &UCheeseMineAbility::InputReleasedAbility);
		WaitGameplayEvent.Get()->ReadyForActivation();
		
		CheeseAimTask = UCheeseAimTask::ExecuteCheeseAimTask(this, Player, CheeseMineInnerRadius, CheeseMineOuterRadius, HoldAnim, MinePlacementDistanceFromPlayer, CheeseIndicatorMesh);
		CheeseAimTask.Get()->ReadyForActivation();
		//Add blocked_weapon Tag to Player
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
}

void UCheeseMineAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
	// if(CheeseMineTask)
	// {
		//Set Speed Back
		TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
		Player.Get()->GetCharacterMovement()->MaxWalkSpeed =
				attributeSet->MaxSpeed.GetCurrentValue() * attributeSet->Speed.GetCurrentValue();
		Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();

		//Remove one to the cheese ammo
	
	
		//Remove blocked_weapon Tag to Player
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);

		//Gameplay Cues
		Player.Get()->GetAbilitySystemComponent()->RemoveGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_CheesePlant);
		Player.Get()->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_CheeseSuccesfulPlant);
	
		//Remove Binding
		//CheeseMineTask.Get()->OnCheeseMinePlanted.RemoveAll(this);
		//CheeseMineTask = nullptr;
		if(CheeseAnimNotify)
		{
			CheeseAnimNotify.Get()->OnNotified.RemoveAll(this);
		}
	//}
	InputHasBeenReleased = false;
	OnLogic = false;
}

void UCheeseMineAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	// if(CheeseMineTask)
	// {
		//Set Speed Back
		TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
		Player.Get()->GetCharacterMovement()->MaxWalkSpeed =
				attributeSet->MaxSpeed.GetCurrentValue() * attributeSet->Speed.GetCurrentValue();
		Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	
		//Remove blocked_weapon Tag to Player
		// Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);

		//Stop
		//CheeseMineTask.Get()->CancelMine();
		Player.Get()->StopAnimMontage();
	
		//Remove Binding
		// CheeseMineTask.Get()->OnCheeseMinePlanted.RemoveAll(this);
		// CheeseMineTask = nullptr;
		if(CheeseAnimNotify)
		{
			CheeseAnimNotify.Get()->OnNotified.RemoveAll(this);
		}
	//}
	InputHasBeenReleased = false;
	OnLogic = false;
	if(WaitGameplayEvent)
	{
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	}
}

void UCheeseMineAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	CheeseAimTask.Get()->EndTask();
	PlantCheeseMine();
}

void UCheeseMineAbility::PlantCheeseMine()
{

	
	CheeseAnimNotify = Cast<UTTAnimNotify>(PlantMineMontage->Notifies[0].Notify.Get());
	if(CheeseAnimNotify && !CheeseAnimNotify.Get()->OnNotified.IsBound())
	{
		CheeseAnimNotify.Get()->OnNotified.AddDynamic(this, &UCheeseMineAbility::FinishAbility);
	}
	//Play PlantAnim
	Player.Get()->PlayAnimMontage(PlantMineMontage);
		
	//Set Walk Speed To 0.0f
	Player.Get()->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	Player.Get()->GetCharacterMovement()->DisableMovement();

	Player.Get()->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_CheesePlant);
}


void UCheeseMineAbility::FinishAbility()
{
	//Remove Mesh
	Player.Get()->GetWeaponComponent()->ClearWeapons();
	//Spawn Cheese Mine
		FVector MinePosition = Player.Get()->GetActorForwardVector() * FVector(MinePlacementDistanceFromPlayer,MinePlacementDistanceFromPlayer,0);
	float PlayerCapsuleHeight = Player.Get()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	TObjectPtr<AMine> mine = GetWorld()->SpawnActor<AMine>(CheeseMine,
		Player.Get()->GetActorLocation() + FVector(MinePosition.X,MinePosition.Y,-PlayerCapsuleHeight),
		Player.Get()->GetActorRotation());
	if(mine)
	{
		mine.Get()->SetIndicatorMesh(CheeseIndicatorMesh);
	}
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	attributeSet.Get()->SetCheeseAmmo(attributeSet.Get()->GetCheeseAmmo()-1);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UCheeseMineAbility::InputReleasedAbility(FGameplayEventData Payload)
{
	if(!Player.Get()->GetWeaponComponent()->bIsIngredientActive)
	{
		if(!OnLogic)
		{
			OnLogic = true;
			WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
			CheeseAimTask.Get()->EndTask();
			PlantCheeseMine();
		}
	} else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
	}
}
