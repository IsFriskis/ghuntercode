// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "TTSpriteActor.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;

UCLASS()
class TINYTAVERNGAME_API ATTSpriteActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATTSpriteActor();

	void ChangeToHoveredSprite(bool ToHover);
	void ShowOutline(bool ShowIt);
	void ChangeToHappySprite();
	void ChangeToSadSprite();
	void ChangeToNormalSprite(bool IsGooker = false);
	void ChangeToUniqueSprite(bool IsGooker = false);
	//When clicked, maintain hovered state
	void LightUp(bool _ShouldLightUp){ShouldLightUp = _ShouldLightUp;}
	void InActiveSprite(){SpriteActor.Get()->SetSprite(DisabledSprite);}
	void ChangeToPassedExpression(TObjectPtr<UPaperSprite> PassedExpressionSprite);
	void ShowNotification(bool ShowIt);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> SpriteActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> SpriteActorHead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> SpriteActorHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> NotificationSprite;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "TT | Sprites")
	TObjectPtr<UPaperSprite> NormalSprite;
	UPROPERTY(EditAnywhere, Category = "TT | Sprites")
	TObjectPtr<UPaperSprite> SpriteHovered;
	UPROPERTY(EditAnywhere, Category = "TT | Sprites")
	TObjectPtr<UPaperSprite> DisabledSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> NormalFaceSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> HappyFaceSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> SadFaceSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> UniqueFaceSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> UniqueBodySprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	TObjectPtr<UPaperSprite> UniqueHandSprite;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	FLinearColor NormalColor;
	UPROPERTY(EditAnywhere, Category = "TT | NPC Sprites")
	FLinearColor HoveredColor = FLinearColor::White;

	bool ShouldLightUp = false;
};
