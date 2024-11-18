// TinyTavern


#include "PuffBall.h"

#include "AbilitySystemComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AbilitySystemComponent/TT_ASC.h"

// Sets default values
APuffBall::APuffBall()
{
	SphereTriggerShape = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTriggerShape"));
	RootComponent = SphereTriggerShape;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PuffBallMesh"));
	Mesh->SetupAttachment(RootComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UTT_ASC>("AbilitySystemComponent");
	
}

UAbilitySystemComponent* APuffBall::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void APuffBall::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

// Called every frame
void APuffBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuffBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SphereTriggerShape->OnComponentBeginOverlap.RemoveAll(this);
	SphereTriggerShape->OnComponentEndOverlap.RemoveAll(this);
}

void APuffBall::InitAbilityActorInfo()
{
	check(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddCharacterAbilities();
	}
}

void APuffBall::AddCharacterAbilities()
{
	UTT_ASC* TT_ASC = Cast<UTT_ASC>(AbilitySystemComponent);
	TT_ASC->AddCharacterAbilities(StartupAbilities);
}




