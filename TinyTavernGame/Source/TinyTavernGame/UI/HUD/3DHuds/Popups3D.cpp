// TinyTavern


#include "Popups3D.h"
#include "PopUpsData.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "TinyTavernGame/TT_GameplayTags.h"

void UPopups3D::NativeConstruct()
{
	Super::NativeConstruct();
	//Bind to animations finish to show and collapse RecipeTickets
	
	FadeAwayAnimationEvent.BindDynamic(this,&UPopups3D::HidePopUp);
	BindToAnimationFinished(FadeAwayAnimation, FadeAwayAnimationEvent);
}

void UPopups3D::WidgetControllerSet_Implementation()
{
	Super::WidgetControllerSet_Implementation();
}

void UPopups3D::NativeDestruct()
{
	Super::NativeDestruct();
	FadeAwayAnimationEvent.Unbind();
}

void UPopups3D::ShowNotification(FGameplayTag NotificationType)
{
	m_NotificationType = NotificationType;
	const FPopUpInfo Info = GetNotificationInfo();
	TimeBased = Info.IsTimeBased;
	NotificationImage.Get()->SetBrushFromTexture(Info.Texture);
	this->SetVisibility(ESlateVisibility::HitTestInvisible);
	if(!Info.IsTimeBased)
	{
		PlayAnimation(FadeAwayAnimation);
	}
	else
	{
		PlayAnimation(FullRenderOpacityAnimation);
		GetWorld()->GetTimerManager().SetTimer(HidingTimer,this,&UPopups3D::HidePopUp,Info.Duration,false);
	}
	PlaySound(Info.Sound);
}

void UPopups3D::HideNotification()
{
	PlayAnimation(FadeAwayAnimation);
}


void UPopups3D::HidePopUp()
{
	this->RemoveFromParent();
}

FPopUpInfo UPopups3D::GetNotificationInfo() const
{
	if(PopUpsData)
	{
		for (FPopUpInfo Element : PopUpsData->PopUpType)
		{
			if(Element.Type == m_NotificationType)
			{
				return Element;
			}
		}
	}
	FPopUpInfo EmptyInfo;
	return EmptyInfo;
}
