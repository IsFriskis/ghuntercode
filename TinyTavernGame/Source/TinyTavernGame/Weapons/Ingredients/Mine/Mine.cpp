// TinyTavern


#include "Mine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../../Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraActor.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"

AMine::AMine()
{
	//TODO Remove when vfx ready that show range
	PrimaryActorTick.bCanEverTick = true;
	//
	MineSphere = CreateDefaultSubobject<USphereComponent>("Mine Sphere");
	RootComponent = MineSphere.Get();

	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mine Mesh");
	MineMesh.Get()->SetupAttachment(MineSphere);

	CheeseIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mine Indicator Mesh");
	CheeseIndicatorMesh.Get()->SetupAttachment(MineSphere);

	CheeseSplatterMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cheese Mine Splatter");
	CheeseSplatterMesh.Get()->SetupAttachment(MineSphere);

	WineVFX = CreateDefaultSubobject<UNiagaraComponent>("Wine VFX");
	WineVFX.Get()->SetupAttachment(MineSphere);
	
	MineExtraVFX = CreateDefaultSubobject<UNiagaraComponent>("Mine Extra VFX");
	MineExtraVFX.Get()->SetupAttachment(MineSphere);

	MineAreaVFX = CreateDefaultSubobject<UNiagaraComponent>("Wine Area VFX");
	MineAreaVFX.Get()->SetupAttachment(MineSphere);
}

void AMine::ChangeWasAffectedByWine()
{
	WasAffectedByWine = true;
	WineVFX.Get()->SetAsset(WineParticlesInMine);
	WineVFX.Get()->Activate();
}

void AMine::ModifySlowDownEffect(bool ShouldChangeSpeed)
{
	if(ShouldChangeSpeed)
	{
		SlowDownPercentage = AfterWineSlowDownPercentage;
	}
	else
	{
		SlowDownPercentage = NormalSlowDownPercentage;
	}
}

void AMine::SetIndicatorMesh(TObjectPtr<UStaticMesh> IndicatorMesh)
{
	if(CheeseIndicatorMesh)
	{
		CheeseIndicatorMesh.Get()->SetStaticMesh(IndicatorMesh);
		CheeseIndicatorMesh.Get()->SetRelativeScale3D(FVector(MineSphere.Get()->GetScaledSphereRadius() / 100, MineSphere.Get()->GetScaledSphereRadius() / 100,1.0f));
	}
}

void AMine::BeginPlay()
{
	Super::BeginPlay();

	SlowDownPercentage = NormalSlowDownPercentage;
	CheeseSplatterMesh.Get()->SetHiddenInGame(true);
	MineSphere.Get()->OnComponentBeginOverlap.AddDynamic(this, &AMine::OnOverlapBegin);
	MineSphere.Get()->OnComponentEndOverlap.AddDynamic(this, &AMine::OnOverlapEnd);
}

void AMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Remove Bindings
	MineSphere.Get()->OnComponentBeginOverlap.RemoveAll(this);
	MineSphere.Get()->OnComponentEndOverlap.RemoveAll(this);
	
	if (OuterMine != nullptr)
	{
		OuterMine.Get()->Destroy();
	}
	//Remove Infinite Effects
	//RemoveInfiniteGameplayEffect
	if(!ActiveInfiniteSlowDownEffectHandles.IsEmpty())
	{
		for (TTuple<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent>> HandlePair : ActiveInfiniteSlowDownEffectHandles)
		{
			if (HandlePair.Value.Get() && HandlePair.Value.Get()->IsValidLowLevel())
			{
				if(HandlePair.Key.IsValid())
				{
					HandlePair.Value->RemoveActiveGameplayEffect(HandlePair.Key,1);
				}
			}
		}
		ActiveInfiniteSlowDownEffectHandles.Empty();
	}
}

void AMine::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//TODO REMOVE When VFX, and remove Tick
#if WITH_EDITOR
	if(MineTrigger)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), MineSphere.Get()->GetScaledSphereRadius(), 8, FColor::Yellow, false, -1);
	}
	else
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), MineSphere.Get()->GetScaledSphereRadius(), 8, FColor::Red, false, -1);
	}
#endif
}


void AMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<AEnemyBase> enemy = Cast<AEnemyBase>(OtherActor);
	if (enemy)
	{
		//Remove Indicator
		if(CheeseIndicatorMesh)
		{
			CheeseIndicatorMesh = nullptr;
		}
		//Add mine ingredient tag
		TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		targetASC.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_Ingredient_Cheese);
		targetASC.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
		FGameplayEventData Payload;
		targetASC.Get()->HandleGameplayEvent(FGameplayTags::Get().States_HitReact, &Payload);
		//Force to only collide once
		if (!AlreadyTriggerBeginOverlap)
		{
			AlreadyTriggerBeginOverlap = true;
			//If Mine Triggers, to achieve stun effect
			//We must first cancel all its abilities and then execute the stun effect
			if (MineTrigger)
			{
				//Execute Effects(If there are) 
				OnOverlap(OtherActor);

				MineAreaVFX.Get()->Deactivate();
				MineExtraVFX.Get()->Deactivate();
				MineMesh.Get()->SetHiddenInGame(true);
				//After trigger, deactivate mine trigger and activate mine effect radius
				MineSphere.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				OuterMine = GetWorld()->SpawnActor<AMine>(OuterMineClass, GetActorTransform());
				OuterMine.Get()->ModifySlowDownEffect(WasAffectedByWine);
				OuterMine.Get()->CheeseSplatterMesh.Get()->SetHiddenInGame(false);
				OuterMine.Get()->MineSphere.Get()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				SetLifeSpan(LifeSpanOfMine);

				TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
				if (ttGameInstance)
				{
					ttGameInstance.Get()->CheeseApplied();
				}
			}
			else
			{
				//Get Attribute Max Speed for later use
				SlowDownPercentage *= Cast<UTT_AttributeSet>(enemy.Get()->GetAttributeSet())->GetMaxSpeed();
				//This is do to Spawning an actor with OverlapBox that spawns with an entity in its range
				//With the if (!AlreadyTriggerBeginOverlap) was solved, but know that the speed needs to be changed
				//If wine hit it, it does not change the speed until de second OnOverlapBegin
				ApplySlowDownEffectInfinite(targetASC);
			}
		}
	}
}

void AMine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Number used to control begin and end overlap
	++NumberOfOverlaps;
	if (!MineTrigger && NumberOfOverlaps >= 2)
	{
		//Execute Slow Down effect after End Overlap, do not apply Stun Effect
		//Remove Slow Down Infinite
		TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		RemoveInfiniteGameplayEffect(targetASC);
		//Apply Slow Down Duration
		ApplySlowDownEffectDuration(targetASC);
	}
	
}

void AMine::ApplySlowDownEffectInfinite(TObjectPtr<UAbilitySystemComponent> TargetASC)
{
	//Slow Down Effect depending on slow down percentage
	FGameplayEffectContextHandle effectContextHandle = TargetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle slowdownSpecHandle = TargetASC.Get()->MakeOutgoingSpec(InfiniteSlowDownEffect, 1, effectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(slowdownSpecHandle, FGameplayTags::Get().States_Slowdown, SlowDownPercentage);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*slowdownSpecHandle.Data.Get());
	ActiveInfiniteSlowDownEffectHandles.Add(ActiveEffectHandle, TargetASC.Get());
}

void AMine::ApplySlowDownEffectDuration(TObjectPtr<UAbilitySystemComponent> TargetASC)
{
	//Slow Down Effect depending on slow down percentage
	FGameplayEffectContextHandle effectContextHandle = TargetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle slowdownSpecHandle = TargetASC.Get()->MakeOutgoingSpec(DurationSlowDownEffect, 1, effectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(slowdownSpecHandle, FGameplayTags::Get().States_Slowdown, SlowDownPercentage);

	TargetASC.Get()->ApplyGameplayEffectSpecToSelf(*slowdownSpecHandle.Data.Get());
}

void AMine::RemoveInfiniteGameplayEffect(TObjectPtr<UAbilitySystemComponent> TargetASC)
{
	TArray<FActiveGameplayEffectHandle> HandlessToRemove;
		
	for (TTuple<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent>> HandlePair : ActiveInfiniteSlowDownEffectHandles)
	{
		if (TargetASC == HandlePair.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
			HandlessToRemove.Add(HandlePair.Key);
		}
	}
	for (FActiveGameplayEffectHandle& Handle : HandlessToRemove)
	{
		ActiveInfiniteSlowDownEffectHandles.FindAndRemoveChecked(Handle);
	}
}
