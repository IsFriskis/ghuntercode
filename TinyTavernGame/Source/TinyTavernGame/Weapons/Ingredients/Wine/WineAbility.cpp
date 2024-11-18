// TinyTavern


#include "WineAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "WineAimTask.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Weapons/Ingredients/Jam/JamSlamArea.h"
#include "TinyTavernGame/Weapons/Ingredients/Mine/Mine.h"

void UWineAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	//Check if Ammo > 0
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	if(!attributeSet || attributeSet.Get()->GetWineAmmo() <= 0)
	{
		Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfWineAmmo);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else
	{
		//Subscribe when ability is finished by input
		WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().Events_Weapon_FinishIngredientWeaponAbility);
		WaitGameplayEvent.Get()->EventReceived.AddDynamic(this, &UWineAbility::FinishAbility);
		WaitGameplayEvent.Get()->ReadyForActivation();
		GetCurrentAbilitySpec()->InputID = 1;
		WineAimTask = UWineAimTask::ExecuteWineAimTask(this, Player, WineRadius, HoldAnim, WineIndicatorMesh);
		WineAimTask.Get()->ReadyForActivation();
		//Add blocked_weapon Tag to Player
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
}

void UWineAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
	//Remove binding
	if(WineAnimNotify)
	{
		WineAnimNotify.Get()->OnNotified.RemoveAll(this);
	}
	//Remove blocked_weapon Tag to Player
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);
	//Remove one to the wine ammo
	InputHasBeenReleased = false;
	bAlreadyEnding = false;
}

void UWineAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	Player.Get()->StopAnimMontage();
	if(WaitGameplayEvent)
	{
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	}
}

void UWineAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if(!bAlreadyEnding)
	{
		bAlreadyEnding = true;
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
		WineAimTask.Get()->EndTask();
		PlayWeaponAnimation();
		RunWineLogic();
	}
	
	
}

void UWineAbility::RunWineLogic()
{
	//Play Wine Animation, and on a notify, ExecuteSplash()
	//AnimNotify = Cast<UTTAnimNotify>(AnimMontage->AnimNotifyTracks[0].Notifies[0]->Notify.Get());
	WineAnimNotify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
	if(WineAnimNotify)
	{
		WineAnimNotify.Get()->OnNotified.AddDynamic(this, &UWineAbility::ExecuteSplash);
	}

	//Add blocked_weapon Tag to Player
	//Player->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
}

void UWineAbility::ExecuteSplash()
{
	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	if(attributeSet)
	{
		attributeSet.Get()->SetWineAmmo(attributeSet.Get()->GetWineAmmo() - 1);
	}
	//Remove Mesh
	Player.Get()->GetWeaponComponent()->ClearWeapons();
	//Wine Explosion GameplayCue
	Player.Get()->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_WineSplashExplosion);
	//Collision Configuration	
	TArray<FHitResult> hits;
	FCollisionShape collisionShape;
	collisionShape.SetSphere(WineRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), WineRadius, 8, FColor::Cyan, false, 5.0f);
#endif
	//Check all colliding entities in the weapons shape range indicator
	bool isHit = GetWorld()->SweepMultiByChannel(hits, Player.Get()->GetActorLocation(), Player.Get()->GetActorLocation()+FVector(0.001f,0.001f,0.0f), Player.Get()->GetActorQuat(), ECC_GameTraceChannel1, collisionShape, QueryParams);
	if (isHit)
	{
		for (int i = 0; i < hits.Num(); i++)
		{
			//If collided with enemy, apply drunk and vulnerable
			if (TObjectPtr<AEnemyBase> enemyBase = Cast<AEnemyBase>(hits[i].GetActor()))
			{
				TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(enemyBase);
				//Drunk Effect
				FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
				effectContextHandle.AddSourceObject(this);
				FGameplayEffectSpecHandle drunkSpecHandle = targetASC.Get()->MakeOutgoingSpec(DrunkEffect, 1, effectContextHandle);

				targetASC.Get()->ApplyGameplayEffectSpecToSelf(*drunkSpecHandle.Data.Get());
				//Add Wine Tag and Alert Tag
				targetASC.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_Ingredient_Wine);
				targetASC.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
				FGameplayEventData Payload;
				targetASC.Get()->HandleGameplayEvent(FGameplayTags::Get().States_HitReact, &Payload);
				//Jam Achievement
				TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
				if (ttGameInstance)
				{
					ttGameInstance.Get()->WineApplied();
				}
			}
			//If collided with a mine, increase slow percentage
			else if(TObjectPtr<AMine> Mine = Cast<AMine>(hits[i].GetActor()))
			{
				Mine.Get()->ChangeWasAffectedByWine();
			}
			//If collided with a jam, increase effect area
			else if(TObjectPtr<AJamSlamArea> JamSlamArea = Cast<AJamSlamArea>(hits[i].GetActor()))
			{
				JamSlamArea.Get()->IncreasePetrificationDuration();
			}
		}
	}
	
}

void UWineAbility::FinishAbility(FGameplayEventData Payload)
{
	if(!Player.Get()->GetWeaponComponent()->bIsIngredientActive)
	{
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
		WineAimTask.Get()->EndTask();
		PlayWeaponAnimation();
		RunWineLogic();
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

