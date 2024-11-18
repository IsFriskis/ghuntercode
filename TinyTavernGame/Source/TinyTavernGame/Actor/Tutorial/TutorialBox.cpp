// TinyTavern


#include "TutorialBox.h"

#include "TutorialDataTable.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/Tutorial/TutorialPopUps.h"


// Sets default values
ATutorialBox::ATutorialBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneComponent");
	SetRootComponent(DefaultSceneComponent);
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATutorialBox::BeginPlay()
{
	Super::BeginPlay();
	
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	if (GameInstance->CurrentLevel == -1 && DisplayOnBeginPlay)
	{
		ShowTutorialWidget();
	}
	BoxCollider.Get()->OnComponentBeginOverlap.AddDynamic(this, &ATutorialBox::OnBeginOverlap);
}

void ATutorialBox::ShowTutorialWidget()
{
	FPopUpsTableRow* RowData = TutorialRow.GetRow<FPopUpsTableRow>("");
	TutorialWidget = CreateWidget<UTutorialPopUps>(GetWorld(),RowData->WidgetClassToDisplay);
	if(TutorialWidget)
	{
		TutorialWidget->SetTutorialData(*RowData);
		TutorialWidget->AddToViewport();
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		PlayerCharacter->SetActivePopUpWidget(TutorialWidget);
		GetWorld()->GetTimerManager().SetTimer(TutorialWidgetTimer, this,&ATutorialBox::RemoveWidget, RowData->Duration,false);
		if(PauseGame)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), PauseGame);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Cast<APlayerController>(PlayerCharacter->GetController()),TutorialWidget);
		}
	}
}

void ATutorialBox::RemoveWidget()
{
	if(TutorialWidget.Get()->IsInViewport())
	{
		TutorialWidget.Get()->RemoveFromParent();
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		PlayerCharacter->SetActivePopUpWidget(nullptr);
	}
	Destroy();
}

void ATutorialBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(!DisplayOnBeginPlay && !AlreadyActivated)
	{
		if(APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
		{
			if(UUserWidget* ActiveWidget = Player->GetActivePopUpWidget())
			{
				ActiveWidget->RemoveFromParent();
				ShowTutorialWidget();
			}
			else
			{
				ShowTutorialWidget();
			}
			AlreadyActivated = true;
		}
	}
}


