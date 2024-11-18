// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "../../TT_GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HighResScreenshot.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


//Apply attribute modifications until use
void UTT_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	APlayerCharacter* Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());

		if (NewValue < GetMaxHealth() * Character->LowHealth)
		{
			LowHealth = true;
			if(!Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_LowHealth))
			{
				Character->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_LowHealth);
			}
		}

		if (LowHealth)
		{
			Character->GetMesh()->SetOverlayMaterial(Character->LowHealthMaterial);
			Character->WeaponSkeletalMesh->SetOverlayMaterial(Character->LowHealthMaterial);
			
			if (NewValue > GetMaxHealth() * Character->LowHealth)
			{
				Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_LowHealth);
				Character->GetMesh()->SetOverlayMaterial(nullptr);
				Character->WeaponSkeletalMesh->SetOverlayMaterial(nullptr);
				LowHealth = false;
				
			}
		}
	}

	if (Attribute == GetPoisonResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxPoisonResistance());
	}

	if (Attribute == GetCookbarAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxCookbar());
	}

	if (Attribute == GetBoiledAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxBoiledAmmo());
	}

	if (Attribute == GetGrilledAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxGrilledAmmo());
	}

	if (Attribute == GetFriedAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxFriedAmmo());
	}

	if (Attribute == GetWineAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxWineAmmo());
	}

	if (Attribute == GetCheeseAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxCheeseAmmo());
	}

	if (Attribute == GetJamAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxJamAmmo());
	}
}

void UTT_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

bool UTT_AttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	FGameplayTag tagToApply;
	FEffectProperties Props;

	// Ensure we are applying to the correct attribute
	if (Data.EvaluatedData.Attribute == GetCookbarAttribute())
	{
		if (Data.EffectSpec.SetByCallerTagMagnitudes.Num() <= 0)
		{
			return true;
		}
		// Get the tags that this effect would apply
		tagToApply = Data.EffectSpec.SetByCallerTagMagnitudes.Get(FSetElementId::FromInteger(0)).Key;

		FGameplayTagContainer targetTags;
		FGameplayTagContainer printedTags;
		Data.Target.GetOwnedGameplayTags(targetTags);
		Data.Target.GetOwnedGameplayTags(printedTags);

		FGameplayEventData Payload;
		GetOwningAbilitySystemComponent()->HandleGameplayEvent(FGameplayTags::Get().States_HitReact, &Payload);

		if (GetCookbar() <= 0)
		{
			if (AEnemyBase* Enemy = Cast<AEnemyBase>(Data.Target.GetOwnerActor()))
			{
				if (Data.EvaluatedData.Magnitude > Enemy->RawPoint)
				{
					UTT_AttributeSet* TargetAS = Cast<UTT_AttributeSet>(Enemy->GetAttributeSet());
					Data.Target.AddLooseGameplayTag(tagToApply);
					OnCookbarTypeChanged.Broadcast(tagToApply);
					TargetAS->SetCookbar(Enemy->RawPoint);
					return false;
				}
			}
		}

		if (GetCookbar() == GetMaxCookbar())
		{
			return false;
		}


		if (!Data.Target.HasMatchingGameplayTag(tagToApply))
		{
			if (AEnemyBase* Enemy = Cast<AEnemyBase>(Data.Target.GetOwnerActor()))
			{
				if (GetCookbar() >= Enemy->RawPoint)
				{
					return false;
				}
				else
				{
					FGameplayTagContainer cookingTagsContainer;
					FGameplayTag cookingTag = FGameplayTag::RequestGameplayTag(FName("States.Cook"));
					cookingTagsContainer.AddTag(cookingTag);
					FGameplayTagContainer filteredTags;
					filteredTags = targetTags.Filter(cookingTagsContainer);
					Data.Target.RemoveLooseGameplayTags(filteredTags);
					Data.Target.AddLooseGameplayTag(tagToApply);
					OnCookbarTypeChanged.Broadcast(tagToApply);


					if (Data.EvaluatedData.Magnitude > Enemy->RawPoint)
					{
						UTT_AttributeSet* TargetAS = Cast<UTT_AttributeSet>(Enemy->GetAttributeSet());
						TargetAS->SetCookbar(Enemy->RawPoint);
					}
					else
					{
						UTT_AttributeSet* targetAS = Cast<UTT_AttributeSet>(Enemy->GetAttributeSet());
						targetAS->SetCookbar(Data.EvaluatedData.Magnitude);
					}

					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (AEnemyBase* Enemy = Cast<AEnemyBase>(Data.Target.GetOwnerActor()))
			{
				if (GetCookbar() < Enemy->RawPoint && GetCookbar() + Data.EvaluatedData.Magnitude > Enemy->RawPoint)
				{
					UTT_AttributeSet* TargetAS = Cast<UTT_AttributeSet>(Enemy->GetAttributeSet());
					TargetAS->SetCookbar(Enemy->RawPoint);
					return false;
				}
			}
			return true;
		}
	}
	else
	{
		return true;
	}
}

//Apply attribute modifications before use

void UTT_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = GetHealth();
		if (Props.TargetASC.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_PlayerShield))
		{
			float x = Data.EvaluatedData.Magnitude;
			//If player has shield add one life, because it already reduced one life to execute
			//GA_HitReact
			SetHealth(NewHealth - Data.EvaluatedData.Magnitude);
		}
		else
		{
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		}

		const bool Die = NewHealth <= 0.f;

		//Cancel All Abilities
		Props.TargetASC.Get()->CancelAbilities();
		if (Die)
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTags::Get().States_Dead);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
		else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive && Data.EvaluatedData.Magnitude < 0)
		{
			//Apply Hit Ability
			// FGameplayTagContainer TagContainer;
			// TagContainer.AddTag(FGameplayTags::Get().States_HitReact);
			// Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			FGameplayEventData Payload;
			Props.TargetASC.Get()->HandleGameplayEvent(FGameplayTags::Get().States_HitReact, &Payload);
		}
	}

	//Player and Enemy PostAttributeChange of the PoisonResistance:: It clamps the regenerating effects not being able to surpass the base value to negative values
	if (Data.EvaluatedData.Attribute == GetPoisonResistanceAttribute())
	{
		const float NewPoisonResistance = GetPoisonResistance();
		SetPoisonResistance(FMath::Clamp(NewPoisonResistance, 0.f, GetMaxPoisonResistance()));
	}
}


void UTT_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	//Source = causer od effect, Target = target of the effect (owner of this AS)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->
		AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController != nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
