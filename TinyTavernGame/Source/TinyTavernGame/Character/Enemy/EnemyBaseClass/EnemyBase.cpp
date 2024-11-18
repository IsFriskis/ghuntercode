// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyAAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "TinyTavernGame/AbilitySystem/AbilitySystemComponent/TT_ASC.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "EnemyGroup.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/UI/HUD/3DHuds/Hud3D.h"
#include "TinyTavernGame/UI/HUD/EnemyUI/EnemyCookBar.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/BaseBar.h"

AEnemyBase::AEnemyBase()
{
	EnemyMotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

AEnemyBase::~AEnemyBase()
{
	
}

void AEnemyBase::BeginDestroy()
{
	Super::BeginDestroy();
	if(group)
	{
		group->DeleteEnemyInGroup(this);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	SpawnPosition = this->GetActorLocation();
	OnActorHit.AddDynamic(this, &AEnemyBase::OnEnemyHit);
	AbilitySystemComponent->AddLooseGameplayTag(EnemyType_Tag);
	GI = Cast<UTT_GameInstance>(GetGameInstance());
	if(IsPremium)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().PremiumEnemy);
	}
	AIController = Cast<AEnemyAAIController>(GetController());
	
	if(const TObjectPtr<UTT_AttributeSet> EnemyASC = Cast<UTT_AttributeSet>(AttributeSet))
	{
		//Binds delegate AttributeChanged
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyASC.Get()
			->GetMaxSpeedAttribute()).AddUObject(this,&ABasicCharacter::SetEnemySpeed);
	}
	//Set the max walk speed to the current value of the speed attribute for the enemy (To change it you should look for (GE_"EnemyName"Attribute))
	GetCharacterMovement()->MaxWalkSpeed = Cast<UTT_AttributeSet>(AttributeSet)->MaxSpeed.GetCurrentValue();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EnemyDesesperatelyState = DesesperatelyPercentaje * RawPoint;
	EnemyFalseFleeState = FalseFleePercentaje * RawPoint;

	if (TObjectPtr<UHud3D> EnemyWidget = Cast<UHud3D>(WidgetBar->GetUserWidgetObject()))
	{
		EnemyWidget->GetCookBar()->SetWidgetController(this);
	}

	//Delegates used to broadcast information to the widget bar
	UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(AttributeSet);
	if (EnemyAS)
	{
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAS->GetCookbarAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				OnCookbarChanged.Broadcast(Data.NewValue);
			}
		);		
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAS->GetMaxCookbarAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				
				OnMaxCookbarChanged.Broadcast(Data.NewValue);
			}
		);
		OnCookbarChanged.Broadcast(EnemyAS->GetCookbar());
		OnMaxCookbarChanged.Broadcast(RawPoint);
	}
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	HighlightEnemy();
	HideShowUI();
	CheckCookbar();
	EliminateEnemy();

	if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Escaped) && !AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().Events_Enemy_InQTE))
	{
		GetGroup()->enemiesInGroupCount -= 1;
		Destroy();
	}
}

void AEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->GetCapsuleComponent()->OnComponentHit.RemoveAll(this);
	if(group)
	{
		group->DeleteEnemyInGroup(this);
	}
}

void AEnemyBase::CancelAllEnemyAbilitiesExcept(FGameplayTag AbilityTag)
{
	// Ability Tag to protect
	FGameplayTagContainer ProtectedAbilityTags;
	ProtectedAbilityTags.AddTag(AbilityTag);

	// Get all abilities
	TArray<FGameplayAbilitySpecHandle> EnemyAbilities;
	AbilitySystemComponent->GetAllAbilities(EnemyAbilities);

	// Filter by tag
	for (FGameplayAbilitySpecHandle AbilityHandle : EnemyAbilities)
	{
		UGameplayAbility* AbilityToCheck = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilityHandle)->Ability;
		if (AbilityToCheck->AbilityTags.HasAny(ProtectedAbilityTags))
		{
			AbilitySystemComponent->CancelAbility(AbilityToCheck);
		}
	}
	
	
}

void AEnemyBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	InitializeAttributes();
	InitializeTags();
	// InitializeCookbarRegeneration();
	AddCharacterAbilities();

	UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(AttributeSet);
	
	EnemyAS->SetMaxCookbar(RawPoint + CookedPoint);
}	
//This method is called when the health of the enemy is equal to his max health and highlight the enemy with a material outline based on the type of cooking
void AEnemyBase::HighlightEnemy()
{
	if(!HighlightMaterial.IsEmpty())
	{
		
		if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Cooked))
		{
			if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Cook.Grilled"))))
			{
				GetMesh()->SetOverlayMaterial(HighlightMaterial[0]);
			}
			else if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Cook.Fried"))))
			{
				GetMesh()->SetOverlayMaterial(HighlightMaterial[2]);
			}
			else if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Cook.Boiled"))))
			{
				GetMesh()->SetOverlayMaterial(HighlightMaterial[1]);
			}
		}
		if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Burnt))
		{
			GetMesh()->SetOverlayMaterial(HighlightMaterial[3]);
		}
	}
}
//Hide and shows the UI when the enemy cookbar is increased
void AEnemyBase::HideShowUI()
{

	TObjectPtr<UHud3D> EnemyWidget = Cast<UHud3D>(WidgetBar->GetUserWidgetObject());
	UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(AttributeSet);

	if (!UIIsInitialized)
	{
		Cast<UBaseBar>(EnemyWidget->GetCookBar())->GetEnemyCookbar()->InitializeUI(this);
		UIIsInitialized = true;
	}

	if (EnemyAS->GetCookbar() != 0 ||
			AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Ingredient_Cheese) ||
			AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Ingredient_Jam) ||
			AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTags::Get().States_Ingredient_Wine))
	{
		ESlateVisibility Visibility = ESlateVisibility::Visible;
		EnemyWidget->GetCookBar()->SetVisibility(Visibility);
	}
	else
	{
		ESlateVisibility Visibility = ESlateVisibility::Hidden;
		EnemyWidget->GetCookBar()->SetVisibility(Visibility);
	}
}

//TODO: DELEGATES
void AEnemyBase::CheckCookbar()
{
	UTT_AttributeSet* EnemyAS = CastChecked<UTT_AttributeSet>(AttributeSet);
	EnemyAS->SetMaxCookbar(RawPoint + CookedPoint);
	float CurrentCookbar = EnemyAS->GetCookbar();
	
	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
	if(CurrentCookbar > 0.0f)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
		AIController->CurrentAlertDetectionTime = 0.0f;
	}
	if(CurrentCookbar <= 0.0f && !AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().States_Alert))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_Alert);
	}
	if (CurrentCookbar >= 0.0f && EnemyAS->GetCookbar() < RawPoint)
	{
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(RawGE, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
	if(EnemyAS->GetCookbar() >= EnemyDesesperatelyState)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_DesperatelyFlee);
	}
	// else if(CurrentCookbar < EnemyDesesperatelyState)
	// {
	// 	AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_DesperatelyFlee);
	// }
	if(EnemyFalseFleeState > 0)
	{
		if(CurrentCookbar >= EnemyFalseFleeState)
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_IA_FalseFlee);
		}
	}
	if(CurrentCookbar >= RawPoint)
	{
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(CookedGE, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
	if(CurrentCookbar >= EnemyAS->GetMaxCookbar())
	{
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(BurntGE, 1, Handle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

//Initialize the regeneration effect for the enemy
// void AEnemyBase::InitializeCookbarRegeneration()
// {
// 	check(AbilitySystemComponent);
// 	check(RegenerationGE);
// 	FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
// 	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(RegenerationGE, 1, Handle);
// 	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
// }
//TODO::Implement a HEADCOMPONENT TO MANAGE THE DEATH OF THE ENEMY READING THE ATTRIBUTE COOKBAR USING DELEGATES
void AEnemyBase::EliminateEnemy()
{
	if (AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Dead"))) &&
		!AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Burnt"))))
	{
		if(EnemyCorpse != nullptr)
		{
			SpawnCorpse();
			this->GetGroup()->enemiesInGroupCount--;
			// if(this->GetGroup()->GetEnemiesInGroup().IsEmpty())
			// {
			// 	Aqui va un delegado si supiera hacerlo asi
			// }
			Destroy();
		}
		else
		{
			// Define la ubicación y la rotación para el actor
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;

			// Define las opciones de spawn
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// Hacer el spawn del actor
			GetWorld()->SpawnActor<AActor>(DeathBody,SpawnLocation,SpawnRotation,SpawnParams);
			//Add tag to EnemyDiscoveredTag in TT_GameInstance
			GI.Get()->EnemiesDiscoveredTags.Add(EnemyType_Tag);
			//Check if first of a creature was defeated for achievements
			GI.Get()->CheckCreature(EnemyType_Tag);
			//Eliminate enemies from this group
			this->GetGroup()->enemiesInGroupCount--;
			// if(this->GetGroup()->GetEnemiesInGroup().IsEmpty())
			// {
			//	 Aqui va un delegado si supiera hacerlo asi
			// }
			Destroy();
		}
	}
	else if(AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Burnt"))))
	{
		if(AbilitySystemComponent.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Dead"))))
		{
			this->GetGroup()->enemiesInGroupCount--;
			Destroy();
		}
	}
}

//Spawn a corpse in the same location as the enemy when he dies passing through the tags of the enemy to the corpse
void AEnemyBase::SpawnCorpse()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	
	FGameplayTagContainer tagsToPass;
	GetAbilitySystemComponent()->GetOwnedGameplayTags(tagsToPass);
	
	AEnemyCorpse* Corpse = GetWorld()->SpawnActor<AEnemyCorpse>(EnemyCorpse, SpawnLocation, SpawnRotation);
	UAbilitySystemComponent* CorpseASC = Corpse->GetAbilitySystemComponent();

	if (Corpse)
	{
		CorpseASC->AddLooseGameplayTags(tagsToPass);
	}
}

void AEnemyBase::OnEnemyHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if((OtherActor != nullptr) && (OtherActor != this))
	{
		if(OtherActor->ActorHasTag("Player") && !AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTags::Get().States_Alert))
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
			AIController->CurrentAlertDetectionTime = 0.0f;
		}
	}
}




