// TinyTavern


#include "TTSpriteActor.h"

#include "PaperSpriteComponent.h"

// Sets default values
ATTSpriteActor::ATTSpriteActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpriteActor = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite Actor");
	RootComponent = SpriteActor.Get();
	
	SpriteActorHead = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite Actor Head");
	SpriteActorHead.Get()->SetupAttachment(SpriteActor.Get());

	SpriteActorHand = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite Actor Hand");
	SpriteActorHand.Get()->SetupAttachment(SpriteActor.Get());

	NotificationSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Notification Sprite");
	NotificationSprite.Get()->SetupAttachment(SpriteActor.Get());
}

// Called when the game starts or when spawned
void ATTSpriteActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATTSpriteActor::ChangeToHoveredSprite(bool ToHover)
{
	if(ToHover)
	{
		SpriteActor.Get()->SetSprite(SpriteHovered);
		SpriteActor.Get()->SetSpriteColor(HoveredColor);
	}
	else
	{
		SpriteActor.Get()->SetSprite(NormalSprite);
		SpriteActor.Get()->SetSpriteColor(NormalColor);
		SpriteActorHand.Get()->SetVisibility(false);
	}
}

void ATTSpriteActor::ShowOutline(bool ShowIt)
{
	if(!ShouldLightUp)
	{
		if(ShowIt)
		{
			SpriteActor.Get()->SetSpriteColor(HoveredColor);
			SpriteActorHead.Get()->SetSpriteColor(HoveredColor);
			NotificationSprite.Get()->SetSpriteColor(HoveredColor);
		}
		else
		{
			SpriteActor.Get()->SetSpriteColor(NormalColor);
			SpriteActorHead.Get()->SetSpriteColor(NormalColor);
			NotificationSprite.Get()->SetSpriteColor(NormalColor);
		}
	}
}

void ATTSpriteActor::ChangeToHappySprite()
{
	SpriteActorHead.Get()->SetSprite(HappyFaceSprite);
}

void ATTSpriteActor::ChangeToSadSprite()
{
	SpriteActorHead.Get()->SetSprite(SadFaceSprite);
}

void ATTSpriteActor::ChangeToNormalSprite(bool IsGooker)
{
	SpriteActorHead.Get()->SetSprite(NormalFaceSprite);
	if(IsGooker)
	{
		SpriteActor.Get()->SetSprite(NormalSprite);
		SpriteActorHand.Get()->SetVisibility(false);
	}
}

void ATTSpriteActor::ChangeToUniqueSprite(bool IsGooker)
{
	if(IsGooker)
	{
		SpriteActorHead.Get()->SetSprite(UniqueFaceSprite);
		SpriteActor.Get()->SetSprite(UniqueBodySprite);
		SpriteActorHand.Get()->SetVisibility(true);
		SpriteActorHand.Get()->SetSprite(UniqueHandSprite);
	}
	else
	{
		SpriteActorHead.Get()->SetSprite(UniqueFaceSprite);	
	}
}

void ATTSpriteActor::ChangeToPassedExpression(TObjectPtr<UPaperSprite> PassedExpressionSprite)
{
	SpriteActorHead.Get()->SetSprite(PassedExpressionSprite);
}

void ATTSpriteActor::ShowNotification(bool ShowIt)
{
	NotificationSprite.Get()->SetVisibility(ShowIt);
}



