// TinyTavern


#include "TaskGamePlayAbilityEnded.h"

UTaskGamePlayAbilityEnded* UTaskGamePlayAbilityEnded::ListenForGamePlayAbilityEnded(UAbilitySystemComponent* ASC,
                                                                                    TSubclassOf<UGameplayAbility> abilityClass)
{
	if (!IsValid(ASC))
	{
		return nullptr;
	}

	const UGameplayAbility* const abilityDef = abilityClass.GetDefaultObject();
	if (abilityDef->AbilityTags.IsValid() == false)
	{
		return nullptr;
	}

	UTaskGamePlayAbilityEnded* listener =  NewObject<UTaskGamePlayAbilityEnded>();
	listener->ASC = ASC;
	listener->TagStillApplied = abilityDef->AbilityTags;

	for (const FGameplayTag tag: listener->TagStillApplied)
	{
		FDelegateHandle handle = ASC->RegisterGameplayTagEvent(tag,EGameplayTagEventType::NewOrRemoved).AddUObject(listener,&UTaskGamePlayAbilityEnded::OnCallback);
		listener->HandlesMap.Add(tag,handle);
	}

	return listener;
}

void UTaskGamePlayAbilityEnded::EndTask()
{
	if (IsValid(ASC))
	{
		for (const FGameplayTag tag : TagStillApplied)
		{
			ASC->UnregisterGameplayTagEvent(*HandlesMap.Find(tag), tag, EGameplayTagEventType::NewOrRemoved);
		}
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UTaskGamePlayAbilityEnded::OnCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount == 0) 
	{
		if (TagStillApplied.HasTagExact(CallbackTag))
		{
			TagStillApplied.RemoveTag(CallbackTag);
			ASC->UnregisterGameplayTagEvent(*HandlesMap.Find(CallbackTag), CallbackTag, EGameplayTagEventType::NewOrRemoved);
			OnEnded.Broadcast();
		}
	}
}
