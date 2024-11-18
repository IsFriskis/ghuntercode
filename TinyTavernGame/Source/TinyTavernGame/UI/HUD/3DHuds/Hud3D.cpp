// TinyTavern


#include "Hud3D.h"

#include "GameplayTagContainer.h"
#include "Popups3D.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

void UHud3D::NativeConstruct()
{
	Super::NativeConstruct();

	ProgressBar3D.Get()->SetVisibility(ESlateVisibility::Hidden);
	Image3D.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UHud3D::UpdateProgressBar(float Value)
{
	ProgressBar3D.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
	ProgressBar3D.Get()->SetPercent(Value);
	if (Value >= 1)
	{
		ResetProgressBar();
	}
}

void UHud3D::ResetProgressBar()
{
	ProgressBar3D.Get()->SetPercent(0);
	ProgressBar3D.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UHud3D::ShowStun()
{
	Image3D.Get()->SetVisibility(ESlateVisibility::Visible);
	m_StunActive = true;
}

void UHud3D::HideStun()
{
	m_StunActive = false;
	Image3D.Get()->SetVisibility(ESlateVisibility::Hidden);
	Image3D.Get()->SetRenderTransformAngle(0.0f);
	m_Time = 0.0f;
	m_StunAngle = 0.0f;
}

void UHud3D::ShowNotificationPopUp(FGameplayTag NotificationTag)
{
	//If Last Tag is the same, increase the counter
	if(LastGameplayTag == NotificationTag)
	{
		TimesSamePopUp++;
	}
	else //else reset the counter and set the new tag as the last
	{
		LastGameplayTag = NotificationTag;
		TimesSamePopUp = 0;
	}

	//If the counter is lesser than the allowed instances, create and show the PopUp
	if(TimesSamePopUp < AllowedInstancesOfSamePopUp)
	{
		UPopups3D* NewPopUpWidget = Cast<UPopups3D>(CreateWidget(GetWorld(),PopUpClass));
		PopUpOverlay.Get()->AddChildToOverlay(NewPopUpWidget);
		NewPopUpWidget->ShowNotification(NotificationTag);
	}
	if(TimesSamePopUp == AllowedInstancesOfSamePopUp)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UHud3D::RestartAllowedInstancesOfSamePopUp, PopUpReloadTime);
	}
	
}

void UHud3D::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_StunActive)
	{
		m_StunAngle += InDeltaTime * StunRotationSpeed;
		if (m_StunAngle >= 360.0f)
		{
			m_StunAngle = 0.0f;
		}
		Image3D.Get()->SetRenderTransformAngle(m_StunAngle);
	}
}

void UHud3D::RestartAllowedInstancesOfSamePopUp()
{
	TimesSamePopUp = -1;
}

