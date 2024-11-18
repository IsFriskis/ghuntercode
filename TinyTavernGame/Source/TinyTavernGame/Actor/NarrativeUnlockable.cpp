// TinyTavern


#include "NarrativeUnlockable.h"

#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/GuncleNoteBig.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/GuncleNotes_Datatable.h"
#include "TinyTavernGame/ArticyGenerated/Tutorial_Dialogue_1st_DraftGlobalVariables.h"


// Sets default values
ANarrativeUnlockable::ANarrativeUnlockable()
{
	NarrativeTrigger = CreateDefaultSubobject<UBoxComponent>("Narrative Trigger");
	RootComponent = NarrativeTrigger.Get();

	NarrativeAudio = CreateDefaultSubobject<UAudioComponent>("Narrative Audio Sound");
	NarrativeAudio.Get()->SetupAttachment(NarrativeTrigger.Get());

	NarrativeWidget = CreateDefaultSubobject<UWidgetComponent>("Narrative Widget");
	NarrativeWidget.Get()->SetupAttachment(NarrativeTrigger.Get());
}

// Called when the game starts or when spawned
void ANarrativeUnlockable::BeginPlay()
{
	Super::BeginPlay();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	if(CheckIfShouldBeEnabled())
	{
		NarrativeWidget.Get()->SetVisibility(false);	
		NarrativeTrigger.Get()->OnComponentBeginOverlap.AddDynamic(this, &ANarrativeUnlockable::OnOverlapBegin);
		NarrativeTrigger.Get()->OnComponentEndOverlap.AddDynamic(this, &ANarrativeUnlockable::OnEndOverlap);
	}
}

void ANarrativeUnlockable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	NarrativeTrigger.Get()->OnComponentBeginOverlap.RemoveAll(this);
	NarrativeTrigger.Get()->OnComponentEndOverlap.RemoveAll(this);
	if(PlayerController)
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
	}
}

void ANarrativeUnlockable::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		NarrativeWidget.Get()->SetVisibility(true);
		PlayerInsideTrigger = true;
		PlayerController = Cast<ATTPlayerController>(Player.Get()->GetController());
		if(PlayerController)
		{
			PlayerController.Get()->OnInteract.AddDynamic(this, &ANarrativeUnlockable::PlayerInteracted);
		}
	}
}

void ANarrativeUnlockable::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		PlayerInsideTrigger = false;
		if(PlayerController)
		{
			NarrativeWidget.Get()->SetVisibility(false);
			PlayerController.Get()->OnInteract.RemoveAll(this);
		}
	}
}

void ANarrativeUnlockable::PlayerInteracted()
{
	if(!Player.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_OnDashAttack))
	{
		if(PlayerInsideTrigger)
		{
			PlayerController.Get()->OnInteract.RemoveAll(this);
			switch (TypeOfNarrative)
			{
				case BOTH:
					GuncleNoteUnlocked();
					ObtainNewNPCEntry();
					break;
				case GUNCLENOTE:
					GuncleNoteUnlocked();
					break;
				case NPCENTRY:
						NarrativeAudio.Get()->SetSound(NPCEntrySound);
						ObtainNewNPCEntry();
					break;
			}
			NarrativeAudio.Get()->Play();
			if(ShouldUpdateArticyGlobalVariable)
			{
				ArticyGlobalVariables = static_cast<UTutorial_Dialogue_1st_DraftGlobalVariables*>(UArticyGlobalVariables::GetDefault(GetWorld()));
				ArticyGlobalVariables.Get()->SetBoolVariable(GlobalVariable, true);
			}
		}
	}
}

void ANarrativeUnlockable::ObtainNote()
{
	TTGameInstance.Get()->GuncleNoteFound[GuncleNoteID] = true;
	NarrativeTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Obtain All Notes Achievement
	TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
	if (ttGameInstance)
	{
		ttGameInstance.Get()->ObtainAllNotes();
	}
}

void ANarrativeUnlockable::ObtainNewNPCEntry()
{
	for (FNewNPCDialog newNPCDialog : NewNPCDialogs)
	{
		for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
		{
			if(npcInformation.Key == newNPCDialog.NPCName)
			{
				npcInformation.Value.NPCDialogs.Add(newNPCDialog.DialogIDUnlocked);
				TTGameInstance.Get()->NPCInformationMap[npcInformation.Key] = npcInformation.Value;
				break;
			}
		}
	}
	NarrativeTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3);
}

bool ANarrativeUnlockable::CheckIfShouldBeEnabled()
{
	switch (TypeOfNarrative) {
	case GUNCLENOTE:
			if(TTGameInstance.Get()->GuncleNoteFound[GuncleNoteID])
			{
				Destroy();
				return false;
			}
		break;
	case NPCENTRY:
		for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
		{
			if(npcInformation.Key == NewNPCDialogs[0].NPCName)
			{
				for (FArticyId dialogID : npcInformation.Value.NPCDialogs)
				{
					if(dialogID == NewNPCDialogs[0].DialogIDUnlocked)
					{
						Destroy();
						return false;
					}
				}
			}
		}
		break;
	case BOTH:
		if(TTGameInstance.Get()->GuncleNoteFound[GuncleNoteID])
		{
			Destroy();
			return false;
		}
		break;
	}
	return true;
}

void ANarrativeUnlockable::GuncleNoteUnlocked()
{
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	TArray<FGuncleNotes_Struct*> guncleNotes;
	NarrativeAudio.Get()->SetSound(GuncleNoteSound);
	BigNote = CreateWidget<UGuncleNoteBig>(PlayerController,GuncleNoteBigWidget);
	BigNote.Get()->ActivateWidget();
	BigNote.Get()->AddToViewport(-1);
	if(!PlayerController.Get()->IsPaused())
	{
		PlayerController.Get()->Pause();
	}
	TTGameInstance.Get()->GuncleNotesDataTable->GetAllRows("", guncleNotes);
	for (FGuncleNotes_Struct* guncleNote : guncleNotes)
	{
		if(guncleNote->ID == GuncleNoteID)
		{
			BigNote.Get()->SetBigNoteImage(guncleNote->Note, guncleNote->NoteText, guncleNote->SignatureText);
			BigNote.Get()->OnFinishedReadingNote.AddDynamic(this, &ANarrativeUnlockable::GuncleNoteFinishedReading);
			break;
		}
	}
	ObtainNote();
	if(!Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(true);
	}
}

void ANarrativeUnlockable::GuncleNoteFinishedReading()
{
	BigNote.Get()->OnFinishedReadingNote.RemoveAll(this);
	BigNote.Get()->RemoveFromParent();
	if(PlayerController.Get()->IsPaused())
	{
		PlayerController.Get()->Pause();	
	}
	
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
	Destroy();
}
