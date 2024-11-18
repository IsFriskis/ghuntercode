// TinyTavern


#include "EnemyCorpse.h"
#include <TinyTavernGame/AbilitySystem/AbilitySystemComponent/TT_ASC.h>
#include <TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h>
#include <TinyTavernGame/Player/PlayerCharacter.h>
#include <TinyTavernGame/Player/TTPlayerController.h>
#include "Components/WidgetComponent.h"

// Sets default values
AEnemyCorpse::AEnemyCorpse()
{	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CorpseMesh"));
	Mesh->SetupAttachment(RootComponent);
	WidgetBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetBar")); 
	WidgetBar->SetupAttachment(RootComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UTT_ASC>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UTT_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AEnemyCorpse::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AEnemyCorpse::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	WidgetBar->SetVisibility(false);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCorpse::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCorpse::OnOverlapEnd);
}


void AEnemyCorpse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCorpse::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TriggerBox->OnComponentBeginOverlap.RemoveAll(this);
	TriggerBox->OnComponentEndOverlap.RemoveAll(this);
}

void AEnemyCorpse::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddCharacterAbilities();
}

void AEnemyCorpse::InitializeAttributes()
{
	check(AbilitySystemComponent);
	check(DefaultAttributes);
	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, Handle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void AEnemyCorpse::AddCharacterAbilities()
{
	UTT_ASC* TT_ASC = Cast<UTT_ASC>(AbilitySystemComponent);
	TT_ASC->AddCharacterAbilities(StartupAbilities);
}

void AEnemyCorpse::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (!hasTookDish)
		{
			WidgetBar->SetVisibility(true);
			canInteract = true;
			Cast<ATTPlayerController>(m_Player.Get()->GetController())->OnInteract.AddDynamic(this, &AEnemyCorpse::InteractWithPrompt);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Puede recoger comanda"));
		}
	}

}

void AEnemyCorpse::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_Player = Cast<APlayerCharacter>(OtherActor);
	if (m_Player)
	{
		WidgetBar->SetVisibility(false);
		canInteract = false;
		Cast<ATTPlayerController>(m_Player.Get()->GetController())->OnInteract.RemoveAll(this);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No puede recoger comanda"));
	}
}

void AEnemyCorpse::InteractWithPrompt()
{
	if (canInteract)
	{
		FGameplayEventData Payload;
		AbilitySystemComponent.Get()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Events.Player.Interact")), &Payload);
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WidgetBar->SetVisibility(false);
		hasTookDish = true;
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ha recogido la comanda"));
#endif
	}
}


