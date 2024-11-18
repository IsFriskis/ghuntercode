// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TTPlayAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyPlayAnimSignature);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTTPlayAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnNotifyPlayAnimSignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
