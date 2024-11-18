// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TTAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifiedSignature);
/**
 Class used to broadcast a delegate when an animation notify is executed 
 */
UCLASS()
class TINYTAVERNGAME_API UTTAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnNotifiedSignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
