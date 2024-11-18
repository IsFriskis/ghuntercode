// TinyTavern


#include "StunAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Character/BasicCharacter.h"
#include "../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/UI/HUD/3DHuds/Hud3D.h"


void UStunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	//Set Walk Speed To 0.0f
	TObjectPtr<ABasicCharacter> character = Cast<ABasicCharacter>(ActorInfo->AvatarActor.Get());
	character.Get()->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	//Show Stun Indicator
	Cast<UHud3D>(character.Get()->GetWidgetBar())->ShowStun();
	//Stun Animation
	character.Get()->PlayAnimMontage(character->GetStunMontage_Implementation());
	//Disable Movement
	character.Get()->GetCharacterMovement()->DisableMovement();
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	if(player)
	{
		//Add blocked_weapon Tag to Player
		player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
	else
	{
		character.Get()->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Combat_EnemyHit);
	}
}

void UStunAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//Set Speed Back
	TObjectPtr<ABasicCharacter> character = Cast<ABasicCharacter>(ActorInfo->AvatarActor.Get());
	if(character)
	{
		//Hide Stun Indicator
		TObjectPtr<UHud3D> characterHud = Cast<UHud3D>(character.Get()->GetWidgetBar());
		if(characterHud)
		{
			characterHud.Get()->HideStun();	
		}
		//Set Speed Back
		TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(character.Get()->GetAttributeSet());
		if (TObjectPtr<AEnemyBase> enemyCharacter = Cast<AEnemyBase>(character))
		{
			enemyCharacter.Get()->GetCharacterMovement()->MaxWalkSpeed = attributeSet->MaxSpeed.GetCurrentValue();
		}
		else
		{
			character.Get()->GetCharacterMovement()->MaxWalkSpeed =	attributeSet->MaxSpeed.GetCurrentValue() * attributeSet->Speed.GetCurrentValue();
		}
		
		//Stop Animation
		character.Get()->StopAnimMontage();
		character.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	}
	
	//Remove blocked_weapon Tag to Player
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	if(player)
	{
		player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);
	}
}
