// TinyTavern


#include "FinisherAbility.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Containers/Ticker.h"
#include "../../Player/TTPlayerController.h"
#include "FinisherAbilityTask.h"
#include "../../Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "../../TT_GameplayTags.h"
#include "InputActionValue.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "../../Recipes/CheckRecipesAbility.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TTPlayAnimNotify.h"
#include "AIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Weapons/QTE/FinisherAbility.h"

#include "TinyTavernGame/Weapons/MeleeDash/MeleeDashTask.h"


struct FFinisherData;

void UFinisherAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData != nullptr)
	{
		const FGameplayAbilityTargetData* TargetData = TriggerEventData->TargetData.Get(0);

		if (const FFinisherData* FinisherData = static_cast<const FFinisherData*>(TargetData))
		{
			Player = FinisherData->PlayerActor;
			Enemy = FinisherData->EnemyToKill;
			MeleeWeaponLevel = FinisherData->MeleeWeaponLevel;

			if (Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Burnt))
			{
				CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
				return;
			}

			StartQTE();
		}
	}
}

void UFinisherAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		CleanAnimNotifies();
	}

	PlayerController->InterruptRangedAbility();

	// Resume Inputs
	PlayerController->SwitchToGameplayInputContext();

	QTEWidgetInstance = nullptr;

	// Equip ranged weapon 
	Player.Get()->GetWeaponComponent()->ChangeWeaponToRange();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFinisherAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFinisherAbility::OnQTEButtonPressed(FGameplayTag CutType)
{
	// Need to handle everything in EndQTE here, maybe make a function to call - OnInterrupted()?
	if (!Enemy)
	{
		EndQTE();
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
		return;
	}

	Player->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_QTESelection);
	CutTypeSelected = CutType;
	ApplyNormalTimeDilation();
	PrepareMinigame();
}



void UFinisherAbility::StartQTE()
{
	//Show QTE Widget
	PlayerController = Cast<ATTPlayerController>(Player->GetController());

	if (PlayerController)
	{
		// Equip melee weapon
		Player.Get()->GetWeaponComponent()->ChangeWeaponToQTE();

		// Lock Player Rotation
		PlayerController->NotAim(FInputActionValue());

		PlayerController->InterruptRangedAbility();

		// QTE Input Context
		PlayerController->SwitchToQTEInputContext();

		// UI
		ShowQTEWidget(PlayerController);

		// Disable hit react overlay
		DisablePlayerAndEnemyOverlay();

		// Disable Skeletal Mesh Collisions
		ActivateEnemySkeletalMeshCollision(false);

		// Invulnerability
		ApplyPlayerInvulnerability();

		// Selection Buttons Animation
		QTEWidgetInstance->RunSelectionButtonsAnimation();

		// Slowdown
		ApplyTimeDilation(TimeDilationForActorsGlobal);

		// Play Start QTE sound
		Player.Get()->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_QTEStart);

		// Cancel EnemyAbilities
		HandleEnemyTagsForQTE();

		//Create FinisherTask
		TObjectPtr<UFinisherAbilityTask> FinisherTask = UFinisherAbilityTask::CreateFinisherTask(this, TimeForBasicQTE, MeleeWeaponLevel, QTEWidgetInstance);
		FinisherTask->OnPress.AddDynamic(this, &UFinisherAbility::OnQTEButtonPressed);
		FinisherTask->OnTimeEnded.AddDynamic(this, &UFinisherAbility::OnSelectionTimeRanOut);
		FinisherTask->ReadyForActivation();
	}
}

void UFinisherAbility::EndQTE()
{
	if (PlayerController)
	{
		// UI
		HideQTEWidget();

		// Reactivate enemy collision
		//ActivateEnemySkeletalMeshCollision(true);

		// Disable hit react overlay
		DisablePlayerAndEnemyOverlay();

		// Resume Time
		ApplyTimeDilation(OriginalGlobalTimeDilation);

		// Not called on the next frame because need to reverse order on QTE end
		ResetCustomTimeDilation();

		if (Enemy)
		{
			// Unfreeze enemy
			Enemy.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Enemy_InQTE);
			if (!Enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Channeling))
			{
				USkeletalMeshComponent* SkeletalMesh = Enemy.Get()->GetComponentByClass<USkeletalMeshComponent>();
				UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->StopAllMontages(0.25f);
				}
			}
			
		}
	}
}

// Plays UI Animation and plays Faceplant Animation afterwards
void UFinisherAbility::OnSelectionTimeRanOut()
{
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::EndQTE);
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::PlayQTEFailAnimation);
	QTEWidgetInstance->RunMinigameResultAnimation(false);
}


void UFinisherAbility::OnMinigameSucceeded()
{
	MinigameSuccess = true;
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::SaveEnemyRecipeData);
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::PlayFinisherAnimation);
	QTEWidgetInstance->RunMinigameResultAnimation(MinigameSuccess);
}

void UFinisherAbility::OnMinigameFailed()
{
	MinigameSuccess = false;
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::EndQTE);
	QTEWidgetInstance->OnMinigameAnimationFinished.AddDynamic(this, &UFinisherAbility::PlayFinisherAnimation);
	QTEWidgetInstance->RunMinigameResultAnimation(MinigameSuccess);
}


void UFinisherAbility::PlayQTEFailAnimation()
{
	UAnimMontage* AnimationToPlay = nullptr;
	AnimationToPlay = *FinisherAnimations.Find(FGameplayTags::Get().Events_Player_QTEFail);

	UAbilityTask_PlayMontageAndWait* QTEFailAnimTask;
	QTEFailAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "QTEFail", AnimationToPlay, 1.0f, FName("QTEFail"), false);
	UTTAnimNotify* CastAnimNotify = Cast<UTTAnimNotify>(AnimationToPlay->Notifies[0].Notify.Get());
	// causing crash if done twice
	CastAnimNotify->OnNotified.AddDynamic(this, &UFinisherAbility::DisablePlayerInvulnerability);
	QTEFailAnimTask->OnCompleted.AddDynamic(this, &UFinisherAbility::OnFacePlantFinished);
	QTEFailAnimTask->OnInterrupted.AddDynamic(this, &UFinisherAbility::OnFacePlantFinished);
	QTEFailAnimTask->ReadyForActivation();
}

void UFinisherAbility::OnFacePlantFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UFinisherAbility::PlayFinisherAnimation()
{
	UAnimMontage* AnimationToPlay = nullptr;
	AnimationToPlay = *FinisherAnimations.Find(CutTypeSelected);

	int AmountOfNotifies = AnimationToPlay->Notifies.Num();

	if (MinigameSuccess)
	{
		if (AnimationToPlay != nullptr && AmountOfNotifies > 0)
		{
			for (int i = 0; i < AmountOfNotifies; i++)
			{
				UTTAnimNotify* CastAnimNotify = Cast<UTTAnimNotify>(AnimationToPlay->Notifies[i].Notify.Get());
				if (!CastAnimNotify)
				{
					continue;
				}

				if (i == AmountOfNotifies - 1)
				{
					// Final notify is OnFinishingHit
					CastAnimNotify->OnNotified.AddDynamic(this, &UFinisherAbility::OnFinishingHit);
				}
				else
				{
					// The rest are OnHit
					CastAnimNotify->OnNotified.AddDynamic(this, &UFinisherAbility::OnHit);
				}
			}
			// Play Gnome Animation
			FinisherMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Finisher", AnimationToPlay, 1.0f, FName("Finisher"), false);
			FinisherMontageTask.Get()->OnInterrupted.AddDynamic(this, &UFinisherAbility::OnFacePlantFinished);
			FinisherMontageTask->ReadyForActivation();
		}
	}
	else
	{

		PlayQTEFailAnimation();
		//for (int i = 0; i < AmountOfNotifies - 1; i++)
		//{
		//	UTTPlayAnimNotify* CastAnimNotify = Cast<UTTPlayAnimNotify>(AnimationToPlay->Notifies[i].Notify.Get());
		//	if (CastAnimNotify)
		//	{
		//		CastAnimNotify->OnNotified.AddDynamic(this, &UFinisherAbility::PlayQTEFailAnimation);
		//	}
		//}
	}


}

void UFinisherAbility::PrepareMinigame()
{
	FGameplayEventData GameplayEvent;
	GameplayEvent.Instigator = Player;

	FMinigameData* MinigamePayload = new FMinigameData();
	MinigamePayload->Enemy = Enemy;
	MinigamePayload->QTEWidget = QTEWidgetInstance;
	MinigamePayload->OnSuccessDelegate.AddDynamic(this, &UFinisherAbility::OnMinigameSucceeded);
	MinigamePayload->OnFailureDelegate.AddDynamic(this, &UFinisherAbility::OnMinigameFailed);

	FGameplayAbilityTargetDataHandle handle;
	handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(MinigamePayload));
	GameplayEvent.TargetData = handle;

	Player->GetAbilitySystemComponent()->HandleGameplayEvent(CutTypeSelected, &GameplayEvent);
}


void UFinisherAbility::OnFinishingHit()
{
	if (Enemy)
	{
		// Spawn blood
		GetWorld()->SpawnActor<AActor>(BloodVFXFinisher, Enemy->GetActorLocation(), Enemy->GetActorRotation());

		// Call enemy death ability
		TObjectPtr<UAbilitySystemComponent> TargetASC = Enemy->GetAbilitySystemComponent();
		FGameplayEventData DeathPayload;
		TargetASC.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Enemy_CheckDie, &DeathPayload);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UFinisherAbility::CallRecipeAbility, NewDishDelay, false, -1);
	EndQTE();
}

void UFinisherAbility::OnHit()
{
	if (Enemy)
	{
		// Spawn blood
		TObjectPtr<UAbilitySystemComponent> TargetASC = Enemy->GetAbilitySystemComponent();
		AActor* BloodActor = GetWorld()->SpawnActor<AActor>(BloodVFX, Enemy->GetActorLocation(), Enemy->GetActorRotation());

		// Speed up blood because game is slowed down
		BloodActor->CustomTimeDilation = 1.0 / GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();

		// Random Sound
		int RandomValue = FMath::RandRange(0, 1);
		if (RandomValue == 0)
		{
			TargetASC->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_HitSound1);
		}
		else
		{
			TargetASC->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_HitSound2);
		}
	}
	else
	{
		EndQTE();
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UFinisherAbility::CallRecipeAbility()
{
	if (Enemy)
	{
		// Custom payload
		FRecipeTagsPayload* RecipePayload = new FRecipeTagsPayload();
		RecipePayload->EnemyTagsInRecipe = EnemyTagsInRecipe;

		// Prepare custom payload
		TObjectPtr<UAbilitySystemComponent> PlayerASC = Player->GetAbilitySystemComponent();
		FGameplayEventData GameplayEvent;
		FGameplayAbilityTargetDataHandle Handle;
		Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(RecipePayload));
		GameplayEvent.TargetData = Handle;

		// Activate Event
		PlayerASC.Get()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Events.Player.Interact")), &GameplayEvent);
		DisablePlayerInvulnerability();
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UFinisherAbility::CleanAnimNotifies()
{
	if (CutTypeSelected != FGameplayTag::EmptyTag)
	{
		for (FAnimNotifyEvent AnimNotify : (*FinisherAnimations.Find(CutTypeSelected))->Notifies)
		{
			UTTAnimNotify* CastAnimNotify = Cast<UTTAnimNotify>(AnimNotify.Notify.Get());
			if (CastAnimNotify)
			{
				CastAnimNotify->OnNotified.RemoveAll(this);
			}
			else
			{
				UTTPlayAnimNotify* CastPlayAnimNotify = Cast<UTTPlayAnimNotify>(AnimNotify.Notify.Get());
				if (CastPlayAnimNotify)
				{
					CastPlayAnimNotify->OnNotified.RemoveAll(this);
				}
			}
		}
	}
	UAnimMontage* FailAnimation = *FinisherAnimations.Find(FGameplayTags::Get().Events_Player_QTEFail);
	Cast<UTTAnimNotify>(FailAnimation->Notifies[0].Notify.Get())->OnNotified.RemoveAll(this);
}

void UFinisherAbility::HandleEnemyTagsForQTE()
{
	UAbilitySystemComponent* EnemyASC = Enemy.Get()->GetAbilitySystemComponent();

	// Tag to freeze/stun enemy
	EnemyASC->AddLooseGameplayTag(FGameplayTags::Get().Events_Enemy_InQTE);

	Enemy.Get()->AIController.Get()->StopMovement();
	Enemy.Get()->CancelAllEnemyAbilitiesExcept(FGameplayTags::Get().Events_Enemy_Channeling);

}

void UFinisherAbility::ShowQTEWidget(ATTPlayerController* Controller)
{
	if (QTEWidgetClass)
	{
		QTEWidgetInstance = CreateWidget<UQTEOverlay>(Controller, QTEWidgetClass.Get());
		if (QTEWidgetInstance)
		{
			QTEWidgetInstance->AddToViewport();
			QTEWidgetInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if(!Player.Get()->GetHUDIsHidden())
			{
				Player.Get()->SetHUDVisibility(true);
			}
		}
	}
}

void UFinisherAbility::HideQTEWidget()
{
	//Hide QTE Widget
	if (QTEWidgetInstance)
	{
		QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		QTEWidgetInstance->RemoveFromParent();
		if(Player.Get()->GetHUDIsHidden())
		{
			Player.Get()->SetHUDVisibility(false);
		}
	}
}

void UFinisherAbility::ApplyTimeDilation(float TimeDilation)
{
	if (UWorld* World = GetWorld())
	{
		World->GetWorldSettings()->SetTimeDilation(TimeDilation);
	}
}

void UFinisherAbility::ApplyPlayerInvulnerability()
{
	TObjectPtr<UAbilitySystemComponent> TargetASC = Player->GetAbilitySystemComponent();
	if (InvulnerabilityEffect && TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(InvulnerabilityEffect, 1, EffectContextHandle);
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UFinisherAbility::ActivateEnemySkeletalMeshCollision(bool ShouldEnable)
{
	USkeletalMeshComponent* EnemyMesh = Enemy.Get()->GetMesh();
	if (ShouldEnable)
	{
		EnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void UFinisherAbility::DisablePlayerAndEnemyOverlay()
{
	if (!Player.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_LowHealth))
	{
		Player->GetMesh()->SetOverlayMaterial(nullptr);
		Player->WeaponSkeletalMesh->SetOverlayMaterial(nullptr);
	}

	USkeletalMeshComponent* EnemySkeletalMesh = Enemy.Get()->GetMesh();
	EnemySkeletalMesh->SetOverlayMaterial(nullptr);
	if (UStaticMeshComponent* EnemyMesh = Cast<UStaticMeshComponent>(EnemySkeletalMesh->GetChildComponent(0)))
	{
		EnemyMesh->SetOverlayMaterial(nullptr);
	}
}

void UFinisherAbility::SaveEnemyRecipeData()
{
	if (Enemy)
	{
		// Get tags from Enemy
		TObjectPtr<UAbilitySystemComponent> EnemyASC = Enemy->GetAbilitySystemComponent();
		EnemyASC.Get()->GetOwnedGameplayTags(EnemyTagsInRecipe);
		EnemyTagsInRecipe.AddTag(CutTypeSelected);
	}
}

void UFinisherAbility::DisablePlayerInvulnerability()
{
	if (InvulnerabilityEffect)
	{
		TObjectPtr<UAbilitySystemComponent> PlayerASC = Player.Get()->GetAbilitySystemComponent();
		PlayerASC->RemoveActiveGameplayEffectBySourceEffect(InvulnerabilityEffect, PlayerASC, 1);
	}
}

void UFinisherAbility::ApplyNormalTimeDilation()
{
	Player.Get()->CustomTimeDilation = TimeDilationForActorsInQTE / TimeDilationForActorsGlobal;
	if (Enemy)
	{
		if (Enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Channeling))
		{
			USkeletalMeshComponent* SkeletalMeshComp = Enemy.Get()->GetMesh();
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
			 
			if (AnimInstance)
			{
				// This will return the currently playing montage
				UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
				AnimInstance->Montage_Pause(Montage);
			}
		}
		else
		{
			Enemy.Get()->CustomTimeDilation = TimeDilationForActorsInQTE / TimeDilationForActorsGlobal;
		}
		
	}

}

void UFinisherAbility::ResetCustomTimeDilation()
{
	Player.Get()->CustomTimeDilation = OriginalGlobalTimeDilation;
	if (Enemy)
	{
		if (Enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Channeling))
		{
			USkeletalMeshComponent* SkeletalMeshComp = Enemy.Get()->GetMesh();
			UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();

			if (AnimInstance)
			{
				// This will return the currently playing montage
				UAnimMontage* Montage = AnimInstance->GetCurrentActiveMontage();
				AnimInstance->Montage_Resume(Montage);
			}
		}
		else
		{
			Enemy.Get()->CustomTimeDilation = OriginalGlobalTimeDilation;
		}	
	}
}

