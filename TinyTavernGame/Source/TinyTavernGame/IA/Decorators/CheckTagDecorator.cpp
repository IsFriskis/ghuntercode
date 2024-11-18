// TinyTavern


#include "CheckTagDecorator.h"

#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


UCheckTagDecorator::UCheckTagDecorator()
{
	bNotifyProcessed = true;
	NodeName = "Check Tag Decorator";
}

void UCheckTagDecorator::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(SearchData.OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if(ASC && SingleTagToCheck)
	{
		if(ASC->HasMatchingGameplayTag(TagToCheck))
		{
			NodeResult = EBTNodeResult::Succeeded;
		}
		else
		{
			NodeResult = EBTNodeResult::Failed;
		}
	}
	else if(ASC && !SingleTagToCheck && !OrCheckTag)
	{
		FGameplayTagContainer TagList;
		for (FGameplayTag Tag : TagsToCheck)
		{
			TagList.AddTag(Tag);
		}
		if(ASC->HasAllMatchingGameplayTags(TagList))
		{
			NodeResult = EBTNodeResult::Succeeded;
		}
		else
		{
			NodeResult = EBTNodeResult::Failed;
		}
	}
	else if(ASC && !SingleTagToCheck && OrCheckTag)
	{
		FGameplayTagContainer TagList;
		for (FGameplayTag Tag : TagsToCheck)
		{
			TagList.AddTag(Tag);
		}
		if(ASC->HasAnyMatchingGameplayTags(TagList))
		{
			NodeResult = EBTNodeResult::Succeeded;
		}
		else
		{
			NodeResult = EBTNodeResult::Failed;
		}
	}
}

bool UCheckTagDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	bool hasTag = false;
	if(ASC && SingleTagToCheck)
	{
		if(ASC->HasMatchingGameplayTag(TagToCheck))
		{
			hasTag = true;
			return hasTag;
		}
		return hasTag;
	}
	else if(ASC && !SingleTagToCheck && !OrCheckTag)
	{
		FGameplayTagContainer TagList;
		for (FGameplayTag Tag : TagsToCheck)
		{
			TagList.AddTag(Tag);
		}
		if(ASC->HasAllMatchingGameplayTags(TagList))
		{
			hasTag = true;
			return hasTag;
		}
		return hasTag;
	}
	else if(ASC && !SingleTagToCheck && OrCheckTag)
	{
		FGameplayTagContainer TagList;
		for (FGameplayTag Tag : TagsToCheck)
		{
			TagList.AddTag(Tag);
		}
		if(ASC->HasAnyMatchingGameplayTags(TagList))
		{
			hasTag = true;
			return hasTag;
		}
		return hasTag;
	}
	return hasTag;
}

