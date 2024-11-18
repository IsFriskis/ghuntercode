// TinyTavern


#include "CheckRecipesAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/BasicCharacter.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/NewDishWidget.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UCheckRecipesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor);

	FGameplayTagContainer enemyTags;

	if (TriggerEventData != nullptr)
	{
		const FGameplayAbilityTargetData* targetData = TriggerEventData->TargetData.Get(0);

		if (const FRecipeTagsPayload* myData = static_cast<const FRecipeTagsPayload*>(targetData))
		{
			enemyTags = myData->EnemyTagsInRecipe;
		}
	}

	if (!enemyTags.IsEmpty())
	{
		int FindID = 0;

		// Enemy Type
		if (enemyTags.HasTag(FGameplayTags::Get().Enemy_Gamusino))
		{
			FindID += 100000;
		}
		else if (enemyTags.HasTag(FGameplayTags::Get().Enemy_Cucafera))
		{
			FindID += 200000;
		}
		else if (enemyTags.HasTag(FGameplayTags::Get().Enemy_Cuelebre))
		{
			FindID += 300000;
		}
		else if (enemyTags.HasTag(FGameplayTags::Get().Enemy_Basilisco))
		{
			FindID += 400000;
		}

		// Cuts
		if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cuts_Sliced))
		{
			FindID += 10000;
		}
		else if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cuts_Smashed))
		{
			FindID += 20000;
		}
		else if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cuts_Skewered))
		{
			FindID += 30000;
		}

		// Cooking type
		if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cook_Grilled))
		{
			FindID += 1000;
		}
		else if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cook_Boiled))
		{
			FindID += 2000;
		}
		else if (enemyTags.HasTagExact(FGameplayTags::Get().States_Cook_Fried))
		{
			FindID += 3000;
		}

		// Ingredients applied
		if (enemyTags.HasTagExact(FGameplayTags::Get().States_Ingredient_Cheese))
		{
			FindID += 100;
		}
		if (enemyTags.HasTagExact(FGameplayTags::Get().States_Ingredient_Jam))
		{
			FindID += 10;
		}
		if (enemyTags.HasTagExact(FGameplayTags::Get().States_Ingredient_Wine))
		{
			FindID += 1;
		}

		TObjectPtr<UWorld> World = ActorInfo->OwnerActor->GetWorld();
		if(World)
		{
			TObjectPtr<UTT_GameInstance> GI = Cast<UTT_GameInstance>(World->GetGameInstance());

			if(GI)
			{
				FGameplayTagContainer premiumTagContainer;
				premiumTagContainer.AddTag(FGameplayTags::Get().PremiumEnemy);
				bool isPremium = enemyTags.HasAny(premiumTagContainer);
				GI->CheckCompleteDishes(FindID, isPremium);
				
				//Show Widget to let the player know he got a new Dish 
				UNewDishWidget* NewDishWidget = Cast<UNewDishWidget>(CreateWidget(GetWorld(),NewDishWidgetClass));
				NewDishWidget->AddToViewport(-1);
				NewDishWidget->SetPlayer(PlayerCharacter);
				NewDishWidget->SetDishData(FindID);
			}
		}
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(),GetCurrentActivationInfo(),true,false);
}

void UCheckRecipesAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool ReplicateEndAbility, bool WasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, ReplicateEndAbility, WasCancelled);
}
