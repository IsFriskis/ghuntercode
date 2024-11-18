// TinyTavern


#include "EnemyCookBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "Kismet/GameplayStatics.h"


void UEnemyCookBar::OnCookbarChanged(float NewCookbar)
{
	// IsRaw
	if (NewCookbar < EnemyActor->RawPoint)
	{
		CookbarProgressBar.Get()->SetPercent(NewCookbar / EnemyActor->RawPoint);
	}
	else if (NewCookbar < EnemyActor->CookedPoint + EnemyActor->RawPoint)
	{
		float CurrentTime = 0.0f;
		CookbarProgressBar.Get()->SetPercent(EnemyActor->RawPoint);
		float BurnPercent = (NewCookbar - EnemyActor->RawPoint) / EnemyActor->CookedPoint;
		BurnbarProgressBar.Get()->SetPercent(BurnPercent);
		if (IsBurning)
		{
			CurrentTime = GetAnimationCurrentTime(BurningAnimation);
			StopAnimation(BurningAnimation);
		}
		else
		{
			RunCookedAnimation();
			if (CookedSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, CookedSound, EnemyActor->GetActorLocation());
			}
			IsBurning = true;
		}
		float AnimationSpeed = FMath::Lerp(0.5f, 3.0f, BurnPercent);
		PlayAnimation(BurningAnimation, CurrentTime, 0, EUMGSequencePlayMode::Forward, AnimationSpeed); // Second 0 is to indicate looping;
			
			
		
	}
	else
	{
		OnMaxBurnBarReached();
	}
}

void UEnemyCookBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyCookBar::InitializeUI(AEnemyBase* Enemy)
{
	EnemyActor = Enemy;

	if (EnemyActor && EnemyActor->GetAbilitySystemComponent())
	{
		// Bind Delegate
		Cast<UTT_AttributeSet>(EnemyActor->GetAttributeSet())->OnCookbarTypeChanged.AddDynamic(this, &UEnemyCookBar::SetProgressBarImages);
		EnemyActor->OnCookbarChanged.AddDynamic(this, &UEnemyCookBar::OnCookbarChanged);
		CookbarProgressBar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BurnbarProgressBar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BrokenCookbarImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEnemyCookBar::OnMaxBurnBarReached()
{
	CookbarImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	CookbarProgressBar.Get()->SetPercent(0.0f);
	BurnbarProgressBar.Get()->SetVisibility(ESlateVisibility::Hidden);
	BrokenCookbarImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (BrokenGlassSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BrokenGlassSound, EnemyActor->GetActorLocation());
	}

}

void UEnemyCookBar::RunCookedAnimation()
{
	if (CookMode == FGameplayTags::Get().States_Cook_Grilled)
	{
		PlayAnimation(CookedGrillAnimation, 0, 0);
	}
	else if (CookMode == FGameplayTags::Get().States_Cook_Boiled)
	{
		PlayAnimation(CookedBoilAnimation, 0, 0);
	}
	else if (CookMode == FGameplayTags::Get().States_Cook_Fried)
	{
		PlayAnimation(CookedFryAnimation, 0, 0);
	}
}

void UEnemyCookBar::SetProgressBarImages(FGameplayTag Tag)
{
	UAbilitySystemComponent* EnemyASC = EnemyActor->GetAbilitySystemComponent();

	CookMode = Tag;

	if (EnemyASC->HasMatchingGameplayTag(Tag))
	{
		CookbarImage.Get()->SetBrushFromTexture(*CookingIcons.Find(Tag));
		CookbarProgressBar.Get()->SetFillColorAndOpacity(*FillColors.Find(Tag));
		BurnbarProgressBar.Get()->SetFillColorAndOpacity(*TintColors.Find(Tag));
		BrokenCookbarImage.Get()->SetColorAndOpacity(*FillColors.Find(Tag));
	}
}
